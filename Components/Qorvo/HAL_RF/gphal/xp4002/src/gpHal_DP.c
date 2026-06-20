/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_DP.c
 *   This file contains the implementation of the DataPending functions
 *
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "gpHal.h"
#include "gpHal_DP.h"

// Debug
#include "gpAssert.h"
#include "gpLog.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

// Mac Filter list format:
// Short List:
// src_ch_mask (1 byte)  : bitmask of channel slots
// valid       (1 byte)  : 1=valid, 0=invalid
// srcpanid    (2 bytes) : pan id
// srcaddr     (2 bytes) : short address

// Extended address:
// src_ch_mask (1 byte)  : bitmask of channel slots
// valid       (1 byte)  : 1=valid, 0=invalid
// srcpanid    (2 bytes) : pan id
// srcaddr     (8 bytes) : long address

#define GPHAL_DP_SHORT_LIST_START ((UIntPtr)(&gpHalDP_ShortAddressList[0]))
#define GPHAL_DP_LONG_LIST_START  ((UIntPtr)(&gpHalDP_LongAddressList[0]))
#define GPHAL_DP_SIZE_SHORT_ENTRY 8  // Size of a short address entry
#define GPHAL_DP_SIZE_LONG_ENTRY  12 // Size of a long address entry

#define GPHAL_DP_SHORT_MAX_HW_SIZE 255
#define GPHAL_DP_LONG_MAX_HW_SIZE  255

#ifndef GPHAL_DP_SHORT_LIST_MAX
#define GPHAL_DP_SHORT_LIST_MAX 255
#endif // GPHAL_DP_SHORT_LIST_MAX
#ifndef GPHAL_DP_LONG_LIST_MAX
#define GPHAL_DP_LONG_LIST_MAX 255
#endif // GPHAL_DP_LONG_LIST_MAX

// Arbitrary number to reserve a minimum number of entries for a stack
// Non-reserved entries will be filled on a first come base
#ifndef GPHAL_DP_SHORT_LIST_MIN_STACK
#define GPHAL_DP_SHORT_LIST_MIN_STACK 20
#endif // GPHAL_DP_LONG_LIST_MAX
#ifndef GPHAL_DP_LONG_LIST_MIN_STACK
#define GPHAL_DP_LONG_LIST_MIN_STACK 20
#endif // GPHAL_DP_LONG_LIST_MAX

#define GPHAL_DP_SHORT_LIST_MAX_STACK                                                                                  \
    (GPHAL_DP_SHORT_LIST_MAX - GPHAL_DP_SHORT_LIST_MIN_STACK * (GP_DIVERSITY_NR_OF_STACKS - 1))
#define GPHAL_DP_LONG_LIST_MAX_STACK                                                                                   \
    (GPHAL_DP_LONG_LIST_MAX - GPHAL_DP_LONG_LIST_MIN_STACK * (GP_DIVERSITY_NR_OF_STACKS - 1))

#if GPHAL_DP_SHORT_LIST_MAX > GPHAL_DP_SHORT_MAX_HW_SIZE
#error "The Data Pending Short Address list size exceeds the maximum supported value"
#endif
#if GPHAL_DP_LONG_LIST_MAX > GPHAL_DP_LONG_MAX_HW_SIZE
#error "The Data Pending Extended Address list size exceeds the maximum supported value"
#endif

#define GPHAL_DP_OFFSET_CHANNEL_INDEX 0
#define GPHAL_DP_OFFSET_VALID         1
#define GPHAL_DP_OFFSET_PANID         2
#define GPHAL_DP_OFFSET_ADDR          4

#define GPHAL_DP_ENTRY_VALID   1
#define GPHAL_DP_ENTRY_INVALID 0

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

gpHalDP_StackLookupEntry gpHalDP_StackLookup[GP_DIVERSITY_NR_OF_STACKS];

/* compile time verification of info structures */
GP_COMPILE_TIME_VERIFY((GPHAL_DP_SIZE_SHORT_ENTRY % 4) == 0);
GP_COMPILE_TIME_VERIFY(GPHAL_DP_SIZE_SHORT_ENTRY >= GP_WB_SHORT_SRC_ENTRY_SIZE);
GP_COMPILE_TIME_VERIFY((GPHAL_DP_SIZE_LONG_ENTRY % 2) == 0);
GP_COMPILE_TIME_VERIFY(GPHAL_DP_SIZE_LONG_ENTRY >= GP_WB_LONG_SRC_ENTRY_SIZE);

// Placeholder to allocate space to store address lists in lower ram part
// Aligned @ 16-bit
static UInt16 gpHalDP_ShortAddressList[GPHAL_DP_SIZE_SHORT_ENTRY * GPHAL_DP_SHORT_LIST_MAX / 2] LINKER_SECTION(
    ".rt_fast_ram");
static UInt16 gpHalDP_LongAddressList[GPHAL_DP_SIZE_LONG_ENTRY * GPHAL_DP_LONG_LIST_MAX / 2] LINKER_SECTION(
    ".rt_fast_ram");

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

#ifdef GP_DIVERSITY_DEVELOPMENT
void gpHalDP_DumpTable(Bool dumpShort)
{
#ifdef GP_DIVERSITY_LOG
    gpHal_Address_t addr;
    UInt8 tableSize;
    UIntLoop i;

    UInt8 valid;
    UInt16 panId;

    if(dumpShort)
    {
        // Short list
        UInt16 shortAddr;
        addr = GPHAL_DP_SHORT_LIST_START;
        tableSize = GP_WB_READ_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE();
        GP_LOG_SYSTEM_PRINTF("Sh:%i", 0, tableSize);

        for(i = 0; i < GPHAL_DP_SHORT_LIST_MAX; i++)
        {
            valid = GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_VALID);
            if(valid)
            {
                UInt8 chmask = GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX);
                GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &panId);
                GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_ADDR, &shortAddr);

                GP_LOG_SYSTEM_PRINTF("%i:%x %x [%i]", 0, i, panId, shortAddr, chmask);
                gpLog_Flush();
            }
            addr += GPHAL_DP_SIZE_SHORT_ENTRY;
        }
    }
    else
    {
        MACAddress_t longAddr;
        addr = GPHAL_DP_LONG_LIST_START;
        tableSize = GP_WB_READ_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE();
        GP_LOG_SYSTEM_PRINTF("Lo:%i", 0, tableSize);
        for(i = 0; i < GPHAL_DP_LONG_LIST_MAX; i++)
        {
            valid = GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_VALID);

            if(valid)
            {
                UInt8 chmask = GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX);
                GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &panId);
                GP_HAL_READ_REGS64(addr + GPHAL_DP_OFFSET_ADDR, &longAddr);

                GP_LOG_SYSTEM_PRINTF("%i:%x %lx %lx [%i]", 0, i, panId, (long unsigned int)longAddr.MSB,
                                     (long unsigned int)longAddr.LSB, chmask);
                gpLog_Flush();
            }
            addr += GPHAL_DP_SIZE_LONG_ENTRY;
        }
    }
#endif // GP_DIVERSITY_LOG
}
#endif // GP_DIVERSITY_DEVELOPMENT

static gpHal_Result_t gpHalDP_AddShortEntry(gpHal_AddressInfo_t* pAddressInfo, UInt8 id)
{
    UIntLoop i;
    UInt8 tableSize;
    gpHal_Address_t addr = GPHAL_DP_SHORT_LIST_START;

    // CHANNEL_INDEX = mask of channel slots to apply to this entry
    // We use a fixed mapping stack 0 -> channel slot 0, stack 1 -> channel slot 1, etc.
    UInt8 chmask = (1 << id);

    tableSize = GP_WB_READ_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE();

    if((UInt8)(GPHAL_DP_SHORT_LIST_MAX_STACK) == gpHalDP_StackLookup[id].shortLen)
    {
        // Table completely full
        GP_LOG_PRINTF("AS failed", 0);
        return gpHal_ResultBusy;
    }

    for(i = 0; i < GPHAL_DP_SHORT_LIST_MAX; i++)
    {
        if(GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_VALID) == GPHAL_DP_ENTRY_INVALID)
        {
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX, chmask);
            GP_HAL_WRITE_REGS16(addr + GPHAL_DP_OFFSET_PANID, &pAddressInfo->panId);
            GP_HAL_WRITE_REGS16(addr + GPHAL_DP_OFFSET_ADDR, &pAddressInfo->address.Short);
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_VALID, GPHAL_DP_ENTRY_VALID);
            GP_LOG_PRINTF("A:%i %u %x %x", 0, i, id, pAddressInfo->panId, pAddressInfo->address.Short);
            // Update table size per stack.
            if(tableSize <= i)
            {
                tableSize = i + 1;
            }
            gpHalDP_StackLookup[id].shortLen += 1;
            break;
        }

        // read value in table to see if we need to insert it here
        gpHal_AddressInfo_t matchInfo;
        GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_ADDR, &matchInfo.address.Short);
        GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &matchInfo.panId);

        if((pAddressInfo->address.Short < matchInfo.address.Short) ||
           ((pAddressInfo->address.Short == matchInfo.address.Short) && (pAddressInfo->panId < matchInfo.panId)))
        {
            tableSize += 1;
            // move values in list one position
            memmove((void*)addr + GPHAL_DP_SIZE_SHORT_ENTRY, (void*)addr, GPHAL_DP_SIZE_SHORT_ENTRY * (tableSize - i));
            // add new value
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX, chmask);
            GP_HAL_WRITE_REGS16(addr + GPHAL_DP_OFFSET_PANID, &pAddressInfo->panId);
            GP_HAL_WRITE_REGS16(addr + GPHAL_DP_OFFSET_ADDR, &pAddressInfo->address.Short);
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_VALID, GPHAL_DP_ENTRY_VALID);
            gpHalDP_StackLookup[id].shortLen += 1;
            break;
        }
        addr += GPHAL_DP_SIZE_SHORT_ENTRY;
    }

    GP_WB_WRITE_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE(tableSize);
    // gpHalDP_DumpTable(true);
    return gpHal_ResultSuccess;
}

static gpHal_Result_t gpHalDP_AddLongEntry(gpHal_AddressInfo_t* pAddressInfo, UInt8 id)
{
    UIntLoop i;
    UInt8 tableSize;
    gpHal_Address_t addr = GPHAL_DP_LONG_LIST_START;
    // CHANNEL_INDEX = mask of channel slots to apply to this entry
    UInt8 chmask = (1 << id);

    tableSize = GP_WB_READ_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE();

    if((UInt8)(GPHAL_DP_LONG_LIST_MAX_STACK) == gpHalDP_StackLookup[id].extLen)
    {
        // Table completely full
        GP_LOG_PRINTF("AL failed", 0);
        return gpHal_ResultBusy;
    }

    for(i = 0; i < GPHAL_DP_LONG_LIST_MAX; i++)
    {
        if(GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_VALID) == GPHAL_DP_ENTRY_INVALID)
        {
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX, chmask);
            GP_HAL_WRITE_REGS16(addr + GPHAL_DP_OFFSET_PANID, &pAddressInfo->panId);
            GP_HAL_WRITE_REGS64(addr + GPHAL_DP_OFFSET_ADDR, &pAddressInfo->address.Extended);
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_VALID, GPHAL_DP_ENTRY_VALID);
            GP_LOG_PRINTF("A:st: %i %i %x %lx%lx", 0, i, id, pAddressInfo->panId, pAddressInfo->address.Extended.MSB,
                          pAddressInfo->address.Extended.LSB);

            // Update table size.
            if(tableSize <= i)
            {
                tableSize = i + 1;
            }
            gpHalDP_StackLookup[id].extLen += 1;

            break;
        }

        // read value in table to see if we need to insert it here
        gpHal_AddressInfo_t matchInfo;
        GP_HAL_READ_REGS64(addr + GPHAL_DP_OFFSET_ADDR, &matchInfo.address.Extended);
        GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &matchInfo.panId);

        if((pAddressInfo->address.Extended.MSB < matchInfo.address.Extended.MSB) ||
           ((pAddressInfo->address.Extended.MSB == matchInfo.address.Extended.MSB) &&
            (pAddressInfo->address.Extended.LSB < matchInfo.address.Extended.LSB)) ||

           ((pAddressInfo->address.Extended.MSB == matchInfo.address.Extended.MSB) &&
            (pAddressInfo->address.Extended.LSB == matchInfo.address.Extended.LSB) &&
            (pAddressInfo->panId < matchInfo.panId)))
        {
            // move values in list one position
            memmove((void*)addr + GPHAL_DP_SIZE_LONG_ENTRY, (void*)addr, GPHAL_DP_SIZE_LONG_ENTRY * (tableSize - i));
            // add new value
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX, chmask);
            GP_HAL_WRITE_REGS16(addr + GPHAL_DP_OFFSET_PANID, &pAddressInfo->panId);
            GP_HAL_WRITE_REGS64(addr + GPHAL_DP_OFFSET_ADDR, &pAddressInfo->address.Extended);
            GP_HAL_WRITE_REG(addr + GPHAL_DP_OFFSET_VALID, GPHAL_DP_ENTRY_VALID);
            tableSize = tableSize + 1;
            gpHalDP_StackLookup[id].extLen += 1;
            break;
        }

        addr += GPHAL_DP_SIZE_LONG_ENTRY;
    }

    GP_WB_WRITE_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE(tableSize);
    // gpHalDP_DumpTable(false);
    return gpHal_ResultSuccess;
}

static gpHal_Result_t gpHalDP_RemoveShortEntry(gpHal_AddressInfo_t* pAddressInfo, UInt8 id)
{
    UInt8 chmask = (1 << id);
    UInt8 tableSize, newTableSize;
    Bool foundEntry = false;
    UIntLoop i;
    gpHal_Address_t addr;

    tableSize = GP_WB_READ_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE();
    newTableSize = tableSize;

    addr = GPHAL_DP_SHORT_LIST_START;
    for(i = 0; i < tableSize; i++)
    {
        if(GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_VALID) == GPHAL_DP_ENTRY_VALID)
        {
            Bool matchEntry = false;
            gpHal_AddressInfo_t matchInfo;

            if(GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX) == chmask)
            {
                GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &matchInfo.panId);
                if(matchInfo.panId == pAddressInfo->panId)
                {
                    GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_ADDR, &matchInfo.address.Short);
                    if(matchInfo.address.Short == pAddressInfo->address.Short)
                    {
                        matchEntry = true;
                        foundEntry = true;
                    }
                }
            }

            if(matchEntry)
            {
                // Remove matching entry and shift the remaining entry one position forward.
                GP_LOG_PRINTF("R:%i %u %x", 0, i, id, pAddressInfo->address.Short);
                memmove((void*)addr, (void*)addr + GPHAL_DP_SIZE_SHORT_ENTRY,
                        GPHAL_DP_SIZE_SHORT_ENTRY * (newTableSize - i));
                // invalidate the previous last entry of the list
                GP_HAL_WRITE_REG(
                    GPHAL_DP_SHORT_LIST_START + GPHAL_DP_SIZE_SHORT_ENTRY * newTableSize + GPHAL_DP_OFFSET_VALID,
                    GPHAL_DP_ENTRY_INVALID);
                newTableSize = newTableSize - 1;
                gpHalDP_StackLookup[id].shortLen -= 1;
                addr -= GPHAL_DP_SIZE_SHORT_ENTRY;
            }
        }

        addr += GPHAL_DP_SIZE_SHORT_ENTRY;
    }

    if(!foundEntry)
    {
        GP_LOG_PRINTF("S Not found", 0);
    }

    GP_WB_WRITE_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE(newTableSize);

    return (foundEntry) ? gpHal_ResultSuccess : gpHal_ResultInvalidHandle;
}

static gpHal_Result_t gpHalDP_RemoveLongEntry(gpHal_AddressInfo_t* pAddressInfo, UInt8 id)
{
    UInt8 chmask = (1 << id);
    UInt8 tableSize, newTableSize;
    Bool foundEntry = false;
    UIntLoop i;
    gpHal_Address_t addr;

    tableSize = GP_WB_READ_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE();
    newTableSize = tableSize;

    addr = GPHAL_DP_LONG_LIST_START;
    for(i = 0; i < tableSize; i++)
    {
        if(GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_VALID) == GPHAL_DP_ENTRY_VALID)
        {
            Bool matchEntry = false;
            gpHal_AddressInfo_t matchInfo;

            if(GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX) == chmask)
            {
                GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &matchInfo.panId);
                if(matchInfo.panId == pAddressInfo->panId)
                {
                    GP_HAL_READ_REGS64(addr + GPHAL_DP_OFFSET_ADDR, &matchInfo.address.Extended);
                    if(matchInfo.address.Extended.MSB == pAddressInfo->address.Extended.MSB &&
                       matchInfo.address.Extended.LSB == pAddressInfo->address.Extended.LSB)
                    {
                        matchEntry = true;
                        foundEntry = true;
                    }
                }
            }

            if(matchEntry)
            {
                // Remove matching entry and shift the remaining entry one position forward.
                GP_LOG_PRINTF("R:%i %u %lx%lx", 0, i, id, pAddressInfo->address.Extended.MSB,
                              pAddressInfo->address.Extended.LSB);
                memmove((void*)addr, (void*)addr + GPHAL_DP_SIZE_LONG_ENTRY,
                        GPHAL_DP_SIZE_LONG_ENTRY * (newTableSize - i));
                // invalidate the previous last entry of the list
                GP_HAL_WRITE_REG(
                    GPHAL_DP_LONG_LIST_START + GPHAL_DP_SIZE_LONG_ENTRY * newTableSize + GPHAL_DP_OFFSET_VALID,
                    GPHAL_DP_ENTRY_INVALID);
                newTableSize = newTableSize - 1;
                gpHalDP_StackLookup[id].extLen -= 1;
                addr -= GPHAL_DP_SIZE_LONG_ENTRY;
            }
        }

        addr += GPHAL_DP_SIZE_LONG_ENTRY;
    }

    if(!foundEntry)
    {
        GP_LOG_PRINTF("L Not found", 0);
    }

    GP_WB_WRITE_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE(newTableSize);

    return (foundEntry) ? gpHal_ResultSuccess : gpHal_ResultInvalidHandle;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

static void gpHalDP_InvalidatePan(UInt16 panId)
{
    UIntLoop i;
    gpHal_Address_t addr;
    UInt8 newTableSize;

    // Remove matching entries from short address table.

    newTableSize = GP_WB_READ_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE();
    addr = GPHAL_DP_SHORT_LIST_START;
    if(newTableSize > 0)
    {
        for(i = 0; i < GPHAL_DP_SHORT_LIST_MAX; i++)
        {
            UInt16 matchPanId = 0xFFFF;
            UInt8 id = GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX);
            if(panId != 0xFFFF)
            {
                // clear specific pan ID (0xFFFF means clear all entries)
                GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &matchPanId);
            }

            if(i < GP_WB_READ_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE() && (panId == matchPanId))
            {
                // delete entry
                memmove((void*)addr, (void*)addr + GPHAL_DP_SIZE_SHORT_ENTRY,
                        GPHAL_DP_SIZE_SHORT_ENTRY * (newTableSize) - (addr - GPHAL_DP_SHORT_LIST_START));
                // invalidate the previous last entry of the list
                GP_HAL_WRITE_REG(
                    GPHAL_DP_SHORT_LIST_START + GPHAL_DP_SIZE_SHORT_ENTRY * newTableSize + GPHAL_DP_OFFSET_VALID,
                    GPHAL_DP_ENTRY_INVALID);
                newTableSize = newTableSize - 1;
                gpHalDP_StackLookup[id].shortLen -= 1;
            }
            else
            {
                addr += GPHAL_DP_SIZE_SHORT_ENTRY;
            }
        }

        GP_WB_WRITE_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE(newTableSize);
    }
    // Remove matching entries from long address table.

    newTableSize = GP_WB_READ_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE();
    addr = GPHAL_DP_LONG_LIST_START;

    for(i = 0; i < GPHAL_DP_LONG_LIST_MAX; i++)
    {
        UInt16 matchPanId = 0xFFFF;
        UInt8 id = GP_HAL_READ_REG(addr + GPHAL_DP_OFFSET_CHANNEL_INDEX);

        if(panId != 0xFFFF)
        {
            // clear specific pan ID (0xFFFF means clear all entries)
            GP_HAL_READ_REGS16(addr + GPHAL_DP_OFFSET_PANID, &matchPanId);
        }

        if(i < GP_WB_READ_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE() && (panId == matchPanId))
        {
            // delete entry
            memmove((void*)addr, (void*)addr + GPHAL_DP_SIZE_LONG_ENTRY,
                    GPHAL_DP_SIZE_LONG_ENTRY * (newTableSize) - (addr - GPHAL_DP_LONG_LIST_START));
            // invalidate the previous last entry of the list
            GP_HAL_WRITE_REG(GPHAL_DP_LONG_LIST_START + GPHAL_DP_SIZE_LONG_ENTRY * newTableSize + GPHAL_DP_OFFSET_VALID,
                             GPHAL_DP_ENTRY_INVALID);
            newTableSize = newTableSize - 1;
            gpHalDP_StackLookup[id].extLen -= 1;
        }
        else
        {
            addr += GPHAL_DP_SIZE_LONG_ENTRY;
        }
    }

    GP_WB_WRITE_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE(newTableSize);
}

void gpHal_InitDataPending(void)
{
    UIntLoop id;

    // Init windows for address lists
    COMPILE_TIME_ASSERT((GPHAL_DP_SHORT_LIST_START % 2) == 0);
    COMPILE_TIME_ASSERT((GPHAL_DP_LONG_LIST_START % 2) == 0);

    // Base address in byte resolution
    GP_ASSERT_DEV_EXT((GPHAL_DP_SHORT_LIST_START - GP_MM_RAM_LINEAR_START) <= 0xFFFF);
    GP_ASSERT_DEV_EXT((GPHAL_DP_LONG_LIST_START - GP_MM_RAM_LINEAR_START) <= 0xFFFF);

    GP_WB_WRITE_MACFILT_SHORT_SRC_ADDRESS_TABLE_BASE_ADDRESS(
        (UInt16)((GPHAL_DP_SHORT_LIST_START - GP_MM_RAM_LINEAR_START) & 0xFFFF));
    GP_WB_WRITE_MACFILT_LONG_SRC_ADDRESS_TABLE_BASE_ADDRESS(
        (UInt16)((GPHAL_DP_LONG_LIST_START - GP_MM_RAM_LINEAR_START) & 0xFFFF));

    gpHalDP_InvalidatePan(0xFFFF);

    for(id = 0; id < GP_DIVERSITY_NR_OF_STACKS; id++)
    {
        gpHalDP_StackLookup[id].panId = 0xFFFF;
        gpHalDP_StackLookup[id].shortLen = 0;
        gpHalDP_StackLookup[id].shortMaxLen = GPHAL_DP_SHORT_LIST_MAX / GP_DIVERSITY_NR_OF_STACKS;
        gpHalDP_StackLookup[id].extLen = 0;
        gpHalDP_StackLookup[id].extMaxLen = GPHAL_DP_LONG_LIST_MAX / GP_DIVERSITY_NR_OF_STACKS;
    }

    gpHal_SetFramePendingAckStack(false, 0);
    gpHal_SetFramePendingAckStack(false, 1);
    gpHal_SetFramePendingAckStack(false, 2);
}

gpHal_Result_t gpHal_DpAddEntry(gpHal_AddressInfo_t* pAddressInfo, UInt8 id)
{
    if(id >= GP_DIVERSITY_NR_OF_STACKS)
    {
        return gpHal_ResultInvalidParameter;
    }
    if(gpHalDP_StackLookup[id].panId != pAddressInfo->panId)
    {
        // Changing Pan or providing a new one - clear out packets from old list ?
        gpHal_DpClearEntries(id);
        gpHalDP_StackLookup[id].panId = pAddressInfo->panId;
    }

    if(gpHal_AddressModeShortAddress == pAddressInfo->addressMode)
    {
        // gpHalDP_DumpTable(true);
        return gpHalDP_AddShortEntry(pAddressInfo, id);
    }
    else if(gpHal_AddressModeExtendedAddress == pAddressInfo->addressMode)
    {
        // gpHalDP_DumpTable(false);
        return gpHalDP_AddLongEntry(pAddressInfo, id);
    }

    return gpHal_ResultInvalidParameter;
}

gpHal_Result_t gpHal_DpRemoveEntry(gpHal_AddressInfo_t* pAddressInfo, UInt8 id)
{
    if(id >= GP_DIVERSITY_NR_OF_STACKS)
    {
        return gpHal_ResultInvalidParameter;
    }

    if(gpHal_AddressModeShortAddress == pAddressInfo->addressMode)
    {
        return gpHalDP_RemoveShortEntry(pAddressInfo, id);
    }
    else if(gpHal_AddressModeExtendedAddress == pAddressInfo->addressMode)
    {
        return gpHalDP_RemoveLongEntry(pAddressInfo, id);
    }

    return gpHal_ResultInvalidParameter;
}

gpHal_Result_t gpHal_DpClearEntries(UInt8 id)
{
    UInt16 stackPanId;
    if(id >= GP_DIVERSITY_NR_OF_STACKS)
    {
        return gpHal_ResultInvalidParameter;
    }

    // Check if stack ever added something
    stackPanId = gpHalDP_StackLookup[id].panId;
    if(stackPanId != 0xFFFF)
    {
        // Check if there is another stack with the same PAN.
        UIntLoop otherStackId;
        Bool foundOtherStack = false;
        for(otherStackId = 0; otherStackId < GP_DIVERSITY_NR_OF_STACKS; otherStackId++)
        {
            if((otherStackId != id) && (gpHalDP_StackLookup[otherStackId].panId == stackPanId))
            {
                foundOtherStack = true;
                break;
            }
        }

        if(!foundOtherStack)
        {
            // There is no other stack with the same PAN ID - invalidate all matching entries.
            gpHalDP_InvalidatePan(stackPanId);
        }

        gpHalDP_StackLookup[id].panId = 0xFFFF;
    }

    return gpHal_ResultSuccess;
}

Bool gpHal_DPEntriesPending(void)
{
    if((GP_WB_READ_MACFILT_SHORT_SRC_ADDRESS_TABLE_SIZE() != 0) ||
       (GP_WB_READ_MACFILT_LONG_SRC_ADDRESS_TABLE_SIZE() != 0))
    {
        return true;
    }
    return false;
}

void gpHal_SetFramePendingAckDefault(Bool enable)
{
    // not used in xp4002 gpHal_SetFramePendingAckStack
    GP_ASSERT_DEV_INT(false);
    return;
}

Bool gpHal_GetFramePendingAckDefault(void)
{
    GP_ASSERT_DEV_INT(false);
    return false;
}

gpHal_Result_t gpHal_SetFramePendingAckStack(Bool enable, UInt8 id)
{
    if(id >= GP_DIVERSITY_NR_OF_STACKS)
    {
        return gpHal_ResultInvalidParameter;
    }
    switch(id)
    {
        case 0:
            GP_WB_WRITE_MACFILT_FP_VALUE_WHEN_NO_MATCH_FOUND(enable);
            break;
        case 1:
            GP_WB_WRITE_MACFILT_FP_VALUE_WHEN_NO_MATCH_FOUND_ALT_A(enable);
            break;
        case 2:
            GP_WB_WRITE_MACFILT_FP_VALUE_WHEN_NO_MATCH_FOUND_ALT_B(enable);
            break;
        default:
            return gpHal_ResultInvalidParameter;
    }
    return gpHal_ResultSuccess;
}

Bool gpHal_GetFramePendingAckStack(UInt8 id)
{
    switch(id)
    {
        case 0:
            return GP_WB_READ_MACFILT_FP_VALUE_WHEN_NO_MATCH_FOUND();
        case 1:
            return GP_WB_READ_MACFILT_FP_VALUE_WHEN_NO_MATCH_FOUND_ALT_A();
        case 2:
            return GP_WB_READ_MACFILT_FP_VALUE_WHEN_NO_MATCH_FOUND_ALT_B();
        default:
            GP_ASSERT_DEV_INT(false);
            return 0;
    }
}
