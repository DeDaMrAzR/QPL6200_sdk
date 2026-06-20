/*
 * Copyright (c) 2023, Qorvo Inc
 *
 *  The file contains functionality to program Kx flash
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

#include "global.h"
#include "gpBsp.h"
#include "hal.h"

#include "gpHal.h"
#include "gpLog.h"

#include "hal_kx_Flash.h"
#include "gpHal_kx_Ipc.h"

#include "gpHal_kx_NVM.h"

#ifdef GP_COMP_CHIPEMU
#include "gpChipEmu_Flash.h"
#endif

#ifndef GP_COMP_CHIPEMU
#include "hal_SEMailbox.h"
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

#define FLASH_CHECK_WORD_ALIGNED(address) (((address)&0x3) == 0)
#define FLASH_RANGE_IN_SINGLE_SECTOR(address, length)                                                                  \
    ((length) <= (FLASH_SECTOR_SIZE - ((address) % FLASH_SECTOR_SIZE)))

#ifdef GP_COMP_CHIPEMU
#define FLASH_READ_UINT32(address)                                                                                     \
    ({                                                                                                                 \
        UInt32 ret;                                                                                                    \
        gpHal_NVMRead((NvmPtr)address, 4, (UInt8*)&ret);                                                               \
        ret;                                                                                                           \
    })
#else
#define FLASH_READ_UINT32(address) *((UInt32*)(address))
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Dependencies on external functions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static gpHal_FlashError_t gpHal_FlashEraseSectorInternal(UInt32 address)
{
    gpHal_FlashError_t ret;
    UInt32 data[FLASH_PAGE_SIZE / sizeof(UInt32)];
    memset(data, 0, FLASH_PAGE_SIZE);
    UInt8 page;
    /* there is no erase for RRAM. Emulate it by writing an array of zeros */
    for(page = 0; page < FLASH_PAGES_PER_SECTOR; page++)
    {
        ret = gpHal_FlashWriteNoVerify(address + FLASH_PAGE_SIZE * page, FLASH_PAGE_SIZE / FLASH_WORD_SIZE, &data[0]);
    }

    return ret;
}

static gpHal_FlashError_t gpHal_FlashVerify(FlashPtr address, UInt32* data, UInt16 length_32)
{
    gpHal_FlashError_t ret;
#if !defined(GP_COMP_CHIPEMU)
    ret = MEMCMP((UInt32*)address, data, length_32 * 4) ? gpHal_FlashError_VerifyFailure : gpHal_FlashError_Success;
#else
    {
        UInt32 rb[FLASH_PAGE_SIZE / 4] = {0};
        UInt8 *bp, *dp;
        bp = (UInt8*)rb;
        dp = (UInt8*)data;

        // Need to give the length in bytes to flash_read_rom -> *4
        gpHal_NVMRead((NvmPtr)address, (UInt32)length_32 * 4, (UInt8*)bp);

        if(!MEMCMP(bp, dp, length_32 * 4)) // need to compare ALL bytes !! (length in bytes)
        {
            ret = gpHal_FlashError_Success;
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("ERROR. Flash: verify failure", 0);
            UInt16 t;
            for(t = 0; t < length_32; t++)
            {
                GP_LOG_SYSTEM_PRINTF("Unexp.bp %lx", 0, (unsigned long)bp[t]);
                GP_LOG_SYSTEM_PRINTF("Unexp.dp %lx", 0, (unsigned long)dp[t]);
            }
            ret = gpHal_FlashError_VerifyFailure;
        }
    }
#endif
    return ret;
}

static void gpHal_FlashLock(void)
{
    /* no hardware lock */
}

static gpHal_FlashError_t gpHal_FlashUnlock(FlashPtr sectorAddress)
{
#if defined(GP_DIVERSITY_GPHAL_FLASH_USE_APP_ADDRESS_CHECK)
    if(gpHal_cbFlashAddressCheck(sectorAddress) != gpHal_FlashError_Success)
    {
        gpHal_FlashLock();
        return gpHal_FlashError_OutOfRange;
    }
#endif

    return gpHal_FlashError_Success;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_FlashInit(void){
#ifdef GP_COMP_CHIPEMU
#else
#endif
}

gpHal_FlashError_t gpHal_FlashBlankCheck(FlashPtr address, UInt16 numUInt32)
{
    UInt16 i;

    if(!FLASH_CHECK_WORD_ALIGNED(address))
    {
        GP_LOG_PRINTF("FlashBlankCheck unaligned address 0x%lx", 0, (unsigned long)address);
        return gpHal_FlashError_UnalignedAddress;
    }

    for(i = 0; i < numUInt32; ++i)
    {
        if(FLASH_READ_UINT32((address + (4 * i))) != 0)
        {
            GP_LOG_PRINTF("Failing erasure address %lx, data = %lx", 0, (unsigned long)(address + (4 * i)),
                          (unsigned long)FLASH_READ_UINT32((address + (4 * i))));
            return gpHal_FlashError_BlankFailure;
        }
    }

    return gpHal_FlashError_Success;
}

gpHal_FlashError_t gpHal_FlashRead(FlashPtr address, UInt16 numBytes, UInt8* data)
{
#ifdef GP_COMP_CHIPEMU
    return gpHal_NVMRead((NvmPtr)address, (UInt32)numBytes, (UInt8*)data);
#else
    MEMCPY(data, (const void*)address, numBytes);
    return gpHal_FlashError_Success;
#endif // GP_COMP_CHIPEMU
}

gpHal_FlashError_t gpHal_FlashEraseSectorNoVerify(FlashPtr sectorAddress)
{
    gpHal_FlashError_t ret = gpHal_FlashError_Success;
#if !defined(GP_COMP_CHIPEMU)
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC) /* some unit tests do not include the IPC mechanism */
#if defined(GP_COMP_GPHAL_RADIO)
    gpHal_IpcBackupRestoreFlags_t flags;
#endif // GP_COMP_GPHAL_RADIO
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#endif // !GP_COMP_CHIPEMU

    if(gpHal_FlashUnlock(sectorAddress) != gpHal_FlashError_Success)
    {
        return gpHal_FlashError_Locked;
    }

    if(!FLASH_CHECK_ADDRESS(sectorAddress))
    {
        GP_LOG_PRINTF("FlashErase addres out of range 0x%lx", 0, (unsigned long)sectorAddress);
        return gpHal_FlashError_OutOfRange;
    }

    if(sectorAddress % FLASH_SECTOR_SIZE != 0)
    {
        GP_LOG_PRINTF("FlashErase unaligned addres 0x%lx", 0, (unsigned long)sectorAddress);
        return gpHal_FlashError_UnalignedAddress;
    }

    /* acquire the mutex briefly, to make sure no other tasks are calling SE mailbox commands */
#if !defined(GP_COMP_CHIPEMU)
    hal_SEMailbox_MutexAcquire();
#endif // !GP_COMP_CHIPEMU
#ifdef GP_DIVERSITY_FREERTOS
    taskENTER_CRITICAL();
#else
    HAL_DISABLE_GLOBAL_INT();
#endif
    /* since interrupts are disabled, the mutex is no langer needed and needs to be released */
    /* because the esecure code will want to acquire it next. */
#if !defined(GP_COMP_CHIPEMU)
    hal_SEMailbox_MutexRelease();
#endif // !GP_COMP_CHIPEMU

#if !defined(GP_COMP_CHIPEMU)
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC) /* some unit tests do not include the IPC mechanism */
#if defined(GP_COMP_GPHAL_RADIO)
    gpHal_IpcStop(&flags);
#endif // GP_COMP_GPHAL_RADIO
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#endif // !GP_COMP_CHIPEMU

    if(FLASH_CHECK_ADDRESS(sectorAddress))
    {
        ret = gpHal_FlashEraseSectorInternal(sectorAddress);
    }
    else
    {
        // should already be captured by check above
        GP_ASSERT_SYSTEM(false);
    }

    // Invalidate flash data buffers.
    GP_WB_WRITE_APPUC_CACHE_FLUSH(1);
    // GP_WB_APPUC_CACHE_MAINT_CTRL_ALL_TRIG_INVALIDATE_ALL();
    // GP_WB_APPUC_CACHE_MAINT_CTRL_ALL_TRIG_CLEAN_ALL();
    __DMB();

    gpHal_FlashLock();

#if !defined(GP_COMP_CHIPEMU)
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#if defined(GP_COMP_GPHAL_RADIO)
    gpHal_IpcRestart(&flags);
#endif // GP_COMP_GPHAL_RADIO
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#endif // !GP_COMP_CHIPEMU

#ifdef GP_DIVERSITY_FREERTOS
    taskEXIT_CRITICAL();
#else
    HAL_ENABLE_GLOBAL_INT();
#endif

    return ret;
}

/*
 * For now, make sure the INF page is not erase by accident.
 * Cannot really do an ASSERT here, as we cannot execute from FLASH.
 */
gpHal_FlashError_t gpHal_FlashEraseSector(FlashPtr sectorAddress)
{
    gpHal_FlashError_t ret = gpHal_FlashEraseSectorNoVerify(sectorAddress);
    if(ret == gpHal_FlashError_Success)
    {
        ret = gpHal_FlashBlankCheck(sectorAddress, FLASH_SECTOR_SIZE / 4);
    }
    return ret;
}

gpHal_FlashError_t gpHal_FlashWriteNoVerify(FlashPtr address, UInt16 numWord, UInt32* data)
{
    gpHal_FlashError_t ret = gpHal_FlashError_Success;
#if !defined(GP_COMP_CHIPEMU)
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC) /* some unit tests do not include the IPC mechanism */
#if defined(GP_COMP_GPHAL_RADIO)
    gpHal_IpcBackupRestoreFlags_t flags;
#endif // GP_COMP_GPHAL_RADIO
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#endif // !GP_COMP_CHIPEMU

    if(gpHal_FlashUnlock(address) != gpHal_FlashError_Success)
    {
        return gpHal_FlashError_Locked;
    }

    if(!FLASH_CHECK_ADDRESS_RANGE(address, ((UInt32)numWord) * 4))
    {
        GP_LOG_PRINTF("FlashWrite addres out of range 0x%lx", 0, (unsigned long)address);
        return gpHal_FlashError_OutOfRange;
    }

    if(address % FLASH_WRITE_UNIT != 0)
    {
        GP_LOG_PRINTF("FlashWrite unaligned address 0x%lx", 0, (unsigned long)address);
        return gpHal_FlashError_UnalignedAddress;
    }

    if(numWord % (FLASH_WRITE_UNIT / 4) != 0)
    {
        GP_LOG_PRINTF("FlashWrite unaligned length 0x%x", 0, numWord);
        return gpHal_FlashError_UnalignedAddress;
    }

    if(numWord > (FLASH_PAGE_SIZE - (address % FLASH_PAGE_SIZE)) / 4)
    {
        GP_LOG_PRINTF("FlashWrite crossing page boundary 0x%lx 0x%x", 0, (unsigned long)address, numWord);
        return gpHal_FlashError_UnalignedAddress;
    }

    /* acquire the mutex briefly, to make sure no other tasks are calling SE mailbox commands */
#if !defined(GP_COMP_CHIPEMU)
    hal_SEMailbox_MutexAcquire();
#endif // !GP_COMP_CHIPEMU
#ifdef GP_DIVERSITY_FREERTOS
    taskENTER_CRITICAL();
#else
    HAL_DISABLE_GLOBAL_INT();
#endif
    /* since interrupts are disabled, the mutex is no langer needed and needs to be released */
    /* because the esecure code will want to acquire it next. */
#if !defined(GP_COMP_CHIPEMU)
    hal_SEMailbox_MutexRelease();
#endif // !GP_COMP_CHIPEMU

#if !defined(GP_COMP_CHIPEMU)
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#if defined(GP_COMP_GPHAL_RADIO)
    gpHal_IpcStop(&flags);
#endif // GP_COMP_GPHAL_RADIO
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#endif // !GP_COMP_CHIPEMU

    if(FLASH_CHECK_ADDRESS(address))
    {
#if defined(GP_COMP_CHIPEMU)
        //GP_ASSERT_DEV_INT(false);
        // chipemu needs an implementation of the function below:
        // ROM_flash_write(address, data, numWord);
        UInt32 status;
        status = gpHal_NVMWrite((NvmPtr)address, (UInt32)numWord * sizeof(data[0]), (UInt8*) data);
        if(status != gpHal_NVMError_Success)
        {
            GP_LOG_SYSTEM_PRINTF("FAIL: gpHal_NVMWrite failed w/ error %d", 0, (int)status);
            ret = gpHal_FlashError_OutOfRange;
        }
#else
        UInt32 status;
        status = hal_SEMailbox_NvmWrite((UInt32)address, data, (UInt32)numWord * sizeof(data[0]));
        if(status != HAL_SEMAILBOX_RESULT_SUCCESS)
        {
            GP_LOG_SYSTEM_PRINTF("FAIL: hal_SEMailbox_NvmWrite failed w/ error %d", 0, (int)status);
            ret = gpHal_FlashError_OutOfRange;
        }
#endif
    }
    else
    {
        GP_ASSERT_SYSTEM(false);
    }

    // Invalidate flash data buffers.
    GP_WB_WRITE_APPUC_CACHE_FLUSH(1);
    // GP_WB_APPUC_CACHE_MAINT_CTRL_ALL_TRIG_INVALIDATE_ALL();
    // GP_WB_APPUC_CACHE_MAINT_CTRL_ALL_TRIG_CLEAN_ALL();
    __DMB();

    gpHal_FlashLock();

#if !defined(GP_COMP_CHIPEMU)
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#if defined(GP_COMP_GPHAL_RADIO)
    gpHal_IpcRestart(&flags);
#endif // GP_COMP_GPHAL_RADIO
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#endif // !GP_COMP_CHIPEMU

#ifdef GP_DIVERSITY_FREERTOS
    taskEXIT_CRITICAL();
#else
    HAL_ENABLE_GLOBAL_INT();
#endif

    return ret;
}

gpHal_FlashError_t gpHal_FlashWrite(FlashPtr address, UInt16 numWord, UInt32* data)
{
    gpHal_FlashError_t ret = gpHal_FlashWriteNoVerify(address, numWord, data);

    if(ret == gpHal_FlashError_Success)
    {
        ret = gpHal_FlashVerify(address, data, numWord);
    }
    return ret;
}

gpHal_FlashError_t gpHal_FlashProgramSector(FlashPtr address, UInt16 length, UInt8* data)
{
    gpHal_FlashError_t result;
    FlashPtr sectorAligned = FLASH_ALIGN_SECTOR(address);
    UInt32 buffer[FLASH_SECTOR_SIZE / sizeof(UInt32)];

    if(!FLASH_RANGE_IN_SINGLE_SECTOR(address, length))
    {
        return gpHal_FlashError_OutOfRange;
    }

    /* read complete sector*/
    result = gpHal_FlashRead(sectorAligned, FLASH_SECTOR_SIZE, (UInt8*)buffer);

    if(result != gpHal_FlashError_Success)
    {
        return result;
    }

    /* modify */
    MEMCPY(((UInt8*)buffer) + (address - sectorAligned), data, length);
    /* erase sector */
    result = gpHal_FlashEraseSectorNoVerify(sectorAligned);

    if(result != gpHal_FlashError_Success)
    {
        return result;
    }

    /* Write Page 0 */
    result = gpHal_FlashWrite(sectorAligned, FLASH_PAGE_SIZE / 4, (UInt32*)&buffer[0]);
    if(result == gpHal_FlashError_Success)
    {
        /* Write Page 1 */
        result = gpHal_FlashWrite(sectorAligned + FLASH_PAGE_SIZE, FLASH_PAGE_SIZE / 4,
                                  (UInt32*)&buffer[FLASH_PAGE_SIZE / 4]);
    }

    return result;
}
