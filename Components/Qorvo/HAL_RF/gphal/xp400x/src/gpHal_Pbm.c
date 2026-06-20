/*
 * Copyright (c) 2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
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

//#define GP_LOCAL_LOG

#include "global.h"
#include "gpHal.h"
#include "gpHal_Pbm.h"
#include "gpHal_DEFS.h"
#include "gpHal_kx_Ipc.h"
#include "gpHal_kx_format_t_checks.h"
#include "gpHal_RtEvents.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

// The maximum amount of pbms supported by the hardware
#define GP_HAL_MAX_NR_OF_PBMS GP_MM_RAM_PBM_OPTS_NR_OF

// The number of pbms that are used in total (user specified + reserved)
#define GP_HAL_NR_OF_PBMS_CONFIGURED (GP_HAL_NR_OF_PBMS_SPECIFIED_BY_USER + GP_HAL_MAX_NR_OF_RESERVED_PBMS)

// Check that the configured amount of PBMs does not exceed HW amount when PD is using PBM
#if defined(GP_DIVERSITY_PD_USE_PBM_VARIANT)
GP_COMPILE_TIME_VERIFY(GP_HAL_NR_OF_PBMS_CONFIGURED <= GP_HAL_MAX_NR_OF_PBMS);
#endif // GP_DIVERSITY_PD_USE_PBM_VARIANT

#define GP_HAL_PBM_USER_MIN_SIZE 16

#define GP_HAL_TOTAL_RESERVED_PBM_MEMORY (GP_HAL_MAX_NR_OF_RESERVED_PBMS * GP_HAL_RESERVED_PBM_SIZE)

#define GP_HAL_TOTAL_PBM_MEMORY                                                                                        \
    (GP_HAL_TOTAL_RESERVED_PBM_MEMORY + (GP_HAL_PBM_TYPE1_AMOUNT * GP_HAL_PBM_TYPE1_SIZE) +                            \
     (GP_HAL_PBM_TYPE2_AMOUNT * GP_HAL_PBM_TYPE2_SIZE))

#define GP_HAL_FREQOFFSET_HZPERUNIT 244

#define GP_HAL_SENSITIVITY_LEVEL_HS  (-101)
#define GP_HAL_SENSITIVITY_LEVEL_LPL (-98)

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

#define GP_HAL_CALCULATE_START_PBM_ADDRESS(absPBMAddress)                                                              \
    ((absPBMAddress) - ((absPBMAddress) % GP_MM_RAM_PBM_ZIGBEE_DATA_SIZE)) /*per 0x80*/
#define GP_HAL_REMAP_REL_ADDRESS_TO_DATA_SEGMENT(relAddr) ((relAddr)&0x7F)

#define GP_HAL_WRITE_PBM_START_TO_MM_ENTRY(index, startAddress)                                                        \
    GP_WB_WRITE_U16(GP_WB_MM_PBM_0_DATA_BASE_ADDRESS_ADDRESS + 2 * index,                                              \
                    startAddress) /* each addr takes up 16 bytes */

// Compile time verification of pbm options size (restricted by HW to 0x20 per pbm)
GP_COMPILE_TIME_VERIFY(GP_WB_BLE_FORMAT_T_SIZE <= GP_MM_RAM_PBM_OPTS_SIZE);
GP_COMPILE_TIME_VERIFY(GP_WB_FORMAT_R_SIZE <= GP_MM_RAM_PBM_OPTS_SIZE);
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_SIZE <= GP_MM_RAM_PBM_OPTS_SIZE);

GP_COMPILE_TIME_VERIFY(GP_HAL_PBM_MAX_SIZE % 4 == 0);

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
GP_COMPILE_TIME_VERIFY((GP_MM_RAM_PBM_OPTS_SIZE % 4) == 0);
typedef struct {
    Bool inUse;
    Bool txPbm;
    UInt16 pbmLength;
    UInt32 opts[GP_MM_RAM_PBM_OPTS_SIZE >> 2]; // needs to be 4 byte aligned to support any access as UInt32
    UInt8* pData;
} gpHal_PbmDescriptor_t;

typedef struct {
    UInt16 size;
    UInt8 amount;
} gpHal_PbmResource_t;

typedef struct {
    UInt16 formatPtr; // Contains format_r/t opts buffer address
    UInt16 nextPtr;   // Points to next entry in queue
} gpHal_RCIEntry_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static Bool gpHal_PbmInitialized = false;
static UInt32 gpHal_PbmMemoryAllocated;

static const gpHal_PbmResource_t gpHal_PbmResources[] = {
    // The first entry is for reserved pbms
    {GP_HAL_RESERVED_PBM_SIZE, GP_HAL_MAX_NR_OF_RESERVED_PBMS},
    // All these entries are configured by the user
    {GP_HAL_PBM_TYPE1_SIZE, GP_HAL_PBM_TYPE1_AMOUNT},
    {GP_HAL_PBM_TYPE2_SIZE, GP_HAL_PBM_TYPE2_AMOUNT}};

// Context for PBMs
static gpHal_PbmDescriptor_t gpHal_PbmDescriptors[GP_HAL_MAX_NR_OF_RESERVED_PBMS + GP_HAL_PBM_TYPE1_AMOUNT +
                                                  GP_HAL_PBM_TYPE2_AMOUNT] LINKER_SECTION(".rt_fast_ram");

// PBM data buffers (currently stored in SYSRAM to be able to perform encryption on PBMS from both NRT and RT)
static COMPILER_ALIGNED(4) UInt8 gpHal_PbmDataMemory[GP_HAL_TOTAL_PBM_MEMORY] LINKER_SECTION(".sysram");

// Format R list (list of available format r opts buffer pointers). For Rx, RT pops an entry from format R list.
static UInt16 gpHal_formatRList[GP_HAL_NR_OF_RX_PBMS] LINKER_SECTION(".rt_fast_ram");

// RCI queue - RT queues format ptrs to this queue for confirm/indication
static gpHal_RCIEntry_t gpHal_RCIEntryList[GP_HAL_NR_OF_RCI_ENTRIES] LINKER_SECTION(".rt_fast_ram");

// pbm handle claimed for ack
static UInt8 ackPbmHandle;
/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

// pbm configuration
#ifdef GP_DIVERSITY_DEVELOPMENT
static void gpHal_PbmDumpConfiguration(void);
#endif // GP_DIVERSITY_DEVELOPMENT

static UInt16 gpHal_PbmGetMaxResourceSize(void);
static void gpHal_PbmCheckConfiguration(void);
static UInt8 gpHal_GetPbmWithMemory(UInt16 size);
static void gpHal_PbmConfigureRxPbms(void);
static void gpHal_PbmConfigureTxPbms(void);

// pbm descriptor handling
static UInt8 gpHal_PbmDescriptorAllocate(Bool txPbm, UInt16 pbmLength);
static gpHal_PbmDescriptor_t* gpHal_PbmDescriptorGet(UInt8 pbmHandle);
static void gpHal_PbmDescriptorMallocMemory(gpHal_PbmDescriptor_t* pDesc);
static void gpHal_PbmDescriptorFreeMemory(gpHal_PbmDescriptor_t* pDesc);
static void gpHal_PbmDescriptorFree(gpHal_PbmDescriptor_t* pDesc);

// Various
static void gpHal_PbmSetDefaultOptions(UInt8 pbmHandle);
static void gpHal_PbmGetMemoryIndexRange(UInt16 size, UInt8* pStart, UInt8* pEnd, UInt16* pAccumulatedSize);
static Int8 gpHal_GetContinuousModeRSSI(UInt8 PBMentry);
static UInt8 gpHal_GetContinuousModeLQI(UInt8 PBMentry);

#ifdef GP_COMP_GPHAL_BLE
gpHal_BleTxPhy_t gpHal_PhyModeToBleTxPhyMode(UInt8 phyMode);
#endif // GP_COMP_GPHAL_BLE

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_DEVELOPMENT
void gpHal_PbmDumpConfiguration(void)
{
    UIntLoop i;

    GP_LOG_SYSTEM_PRINTF("--- PBM Config ---", 0);

    for(i = 0; i < number_of_elements(gpHal_PbmResources); i++)
    {
        GP_LOG_SYSTEM_PRINTF("%i) %i x %i bytes", 0, i, gpHal_PbmResources[i].amount, gpHal_PbmResources[i].size);
    }
    GP_LOG_SYSTEM_PRINTF("Total # PBMs: %i", 0, GP_HAL_NR_OF_PBMS_CONFIGURED);
    GP_LOG_SYSTEM_PRINTF("# Reserved: %i, Tx: %i, Rx: %i", 0, (GP_HAL_MAX_NR_OF_RESERVED_PBMS),
                         (GP_HAL_NR_OF_PBMS_SPECIFIED_BY_USER - GP_HAL_NR_OF_RX_PBMS), (GP_HAL_NR_OF_RX_PBMS));
}
#endif // GP_DIVERSITY_DEVELOPMENT

void gpHal_PbmCheckConfiguration(void)
{
    UIntLoop i;
    UInt16 totalMemoryRequested = 0;

    for(i = 0; i < number_of_elements(gpHal_PbmResources); i++)
    {
        // Check if all sizes are aligned on 4 bytes
        if(gpHal_PbmResources[i].size % 4 != 0)
        {
            GP_LOG_SYSTEM_PRINTF("PBM size %i is no multiple of 4", 0, gpHal_PbmResources[i].size);
            GP_ASSERT_SYSTEM(false);
        }

        if(i != 0)
        {
            // Check if sizes are specified in ascending order
            if(gpHal_PbmResources[i - 1].size >= gpHal_PbmResources[i].size)
            {
                GP_LOG_SYSTEM_PRINTF("PBM sizes should be specified in ascending order", 0);
                GP_ASSERT_SYSTEM(false);
            }

            if(gpHal_PbmResources[i].size < GP_HAL_PBM_USER_MIN_SIZE)
            {
                GP_LOG_SYSTEM_PRINTF("PBM min sizes should be %i", 0, GP_HAL_PBM_USER_MIN_SIZE);
                GP_ASSERT_SYSTEM(false);
            }
        }

        totalMemoryRequested += (gpHal_PbmResources[i].amount * gpHal_PbmResources[i].size);
    }

    if(totalMemoryRequested != sizeof(gpHal_PbmDataMemory))
    {
        GP_LOG_SYSTEM_PRINTF("memory mismatch, bytes req: %i, alloc: %i", 0, totalMemoryRequested,
                             (int)sizeof(gpHal_PbmDataMemory));
        GP_ASSERT_DEV_INT(false);
    }
}

UInt32 gpHal_getOptsBase(UInt8 pbmEntry)
{
    GP_LOG_PRINTF("getoptbase h %d, PBMs allocated = 0x%x", 0, pbmEntry, gpHal_PbmMemoryAllocated);
    GP_ASSERT_DEV_EXT(pbmEntry < GP_HAL_NR_OF_PBMS_CONFIGURED);
    return (UInt32)GP_HAL_PHYSICAL_TO_ARM(&gpHal_PbmDescriptors[pbmEntry].opts[0]);
}

UInt32 gpHal_getDataBase(UInt8 pbmEntry)
{
    GP_ASSERT_DEV_EXT(pbmEntry < GP_HAL_NR_OF_PBMS_CONFIGURED);
    return (UInt32)GP_HAL_PHYSICAL_TO_ARM(gpHal_PbmDescriptors[pbmEntry].pData);
}

Bool gpHal_isPbmAllocated(UInt8 pbmEntry) { return gpHal_PbmDescriptors[pbmEntry].inUse; }

UInt32 gpHal_getAckOptsBase(void) { return GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(ackPbmHandle); }

UInt16 gpHal_PbmToFormatPtr(UInt8 pbmEntry)
{
    if(!GP_HAL_CHECK_PBM_VALID(pbmEntry))
    {
        return GP_HAL_FORMAT_PTR_INVALID;
    }

    return TO_RT_ADDR(GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmEntry));
}

UInt8 gpHal_GetPbmWithMemory(UInt16 size)
{
    UInt8 handle;

    handle = gpHal_PbmDescriptorAllocate(true, size);

    return handle;
}

UInt16 gpHal_PbmGetMaxResourceSize(void)
{
    UIntLoop i;
    UInt16 maxSize;

    maxSize = 0;
    for(i = 0; i < number_of_elements(gpHal_PbmResources); i++)
    {
        if(gpHal_PbmResources[i].amount)
        {
            maxSize = max(maxSize, gpHal_PbmResources[i].size);
        }
    }

    return maxSize;
}

void gpHal_PbmConfigureRxPbms(void)
{
    UIntLoop i;

    // Pre-configure RX PBMs
    GP_WB_WRITE_RT_FORMAT_R_PTRS_NR(GP_HAL_NR_OF_RX_PBMS);
    GP_WB_WRITE_RT_FORMAT_R_MAX_FRAME_LEN(gpHal_PbmGetMaxResourceSize());
    GP_WB_WRITE_RT_FORMAT_R_PTRS_BASE_PTR(TO_RT_ADDR(GP_HAL_PHYSICAL_TO_ARM((UInt32)gpHal_formatRList)));

    for(i = 0; i < GP_HAL_NR_OF_RX_PBMS; i++)
    {
        UInt8 pbmHandle;

        // Rx wants largest PBM's
        pbmHandle = gpHal_PbmDescriptorAllocate(false, gpHal_PbmGetMaxResourceSize());
        GP_ASSERT_DEV_EXT(pbmHandle != GP_PBM_INVALID_HANDLE);

        UInt32 optsbase;
        // format r buffer
        optsbase = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmHandle);

        // store data frame ptr in format r buffer
        GP_WB_WRITE_FORMAT_R_FRAME_PTR(optsbase, GP_HAL_PHYSICAL_TO_ARM((UInt32)gpHal_PbmDescriptors[i].pData));

        /* adds format r buffer to the list of buffers that RT can use */
        // gpHal_RtAddFrPtr(optsbase);
        gpHal_formatRList[i] = TO_RT_ADDR(optsbase);
    }

    gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_RENEW_ACTIVE_FORMAT_R_PTR, 0, NULL);
}

void gpHal_PbmConfigureTxPbms(void) {}
UInt8 gpHal_PbmDescriptorAllocate(Bool txPbm, UInt16 pbmLength)
{
    UIntLoop i;
    UInt8 pbmHandle = GP_PBM_INVALID_HANDLE;
    gpHal_PbmDescriptor_t* pDesc = NULL;

    /* find a free descriptor */
    for(i = 0; i < GP_HAL_NR_OF_PBMS_CONFIGURED; i++)
    {
        if(!gpHal_PbmDescriptors[i].inUse)
        {
            // gpHal_PbmDescriptors[i].pbmHandle = i;
            gpHal_PbmDescriptors[i].inUse = true;
            gpHal_PbmDescriptors[i].pbmLength = pbmLength;
            gpHal_PbmDescriptors[i].txPbm = txPbm;
            pDesc = &gpHal_PbmDescriptors[i];
            pbmHandle = i;
            break;
        }
    }
    /* not sufficient number of PBMs */
    GP_ASSERT_DEV_EXT(pbmHandle < GP_HAL_NR_OF_PBMS_CONFIGURED);

    if(pDesc != NULL)
    {
        gpHal_PbmDescriptorMallocMemory(pDesc);

        if(pDesc->pData == NULL)
        {
            GP_LOG_SYSTEM_PRINTF("Unable to claim pbm memory for handle: %i (len: %i)", 0, pbmHandle, pbmLength);
            // Unable to configure the memory, free descriptor again
            gpHal_PbmDescriptors[i].inUse = false;
            return GP_PBM_INVALID_HANDLE;
        }

        if(txPbm)
        {
            // To avoid any garbage data from previous action, make sure to default the complete pbm options memory
            // Can only do this for TX, as RX pbms are not claimed yet when we configure them
            gpHal_PbmSetDefaultOptions(pbmHandle);
        }
    }

    return pbmHandle;
}

gpHal_PbmDescriptor_t* gpHal_PbmDescriptorGet(UInt8 pbmHandle) { return &gpHal_PbmDescriptors[pbmHandle]; }

void gpHal_PbmDescriptorMallocMemory(gpHal_PbmDescriptor_t* pDesc)
{
    UIntLoop i;
    UInt8 start;
    UInt8 end;
    UInt16 accumulatedSize;

    GP_ASSERT_DEV_EXT(pDesc->pData == NULL);

    gpHal_PbmGetMemoryIndexRange(pDesc->pbmLength, &start, &end, &accumulatedSize);

    for(i = start; i <= end; i++)
    {
        if(BIT_TST(gpHal_PbmMemoryAllocated, i) == 0)
        {
            BIT_SET(gpHal_PbmMemoryAllocated, i);
            // GP_LOG_SYSTEM_PRINTF("accum = %x en tot = %x",0,accumulatedSize,(i-start)*(pDesc->pbmLength));
            pDesc->pData = &gpHal_PbmDataMemory[accumulatedSize + (i - start) * (pDesc->pbmLength)];
            // check if data buffer is 32 - bit aligned to avoid any unaligned access
            GP_ASSERT_DEV_EXT((((UInt32)pDesc->pData) % 4) == 0);
            // GP_LOG_SYSTEM_PRINTF("start mem = 0x%x, dataP = 0x%x",0, (unsigned int)gpHal_PbmDataMemory, (unsigned
            // int)pDesc->pData);
            break;
        }
    }
}

void gpHal_PbmDescriptorFreeMemory(gpHal_PbmDescriptor_t* pDesc)
{
    UIntLoop i;
    UInt8 start;
    UInt8 end;
    UInt16 accumulatedSize;

    GP_ASSERT_DEV_EXT(pDesc->pData != NULL);

    gpHal_PbmGetMemoryIndexRange(pDesc->pbmLength, &start, &end, &accumulatedSize);

    for(i = start; i <= end; i++)
    {
        if(pDesc->pData == &gpHal_PbmDataMemory[accumulatedSize + (i - start) * pDesc->pbmLength])
        {
            BIT_CLR(gpHal_PbmMemoryAllocated, i);
            pDesc->pData = NULL;
            break;
        }
    }

    // Data should be NULL now
    GP_ASSERT_DEV_EXT(pDesc->pData == NULL);
}

void gpHal_PbmDescriptorFree(gpHal_PbmDescriptor_t* pDesc)
{
    gpHal_PbmDescriptorFreeMemory(pDesc);
    pDesc->inUse = false;
}

void gpHal_PbmSetDefaultOptions(UInt8 pbmHandle)
{
    gpHal_Address_t pbmOptAddress;
    UInt8 tempBuffer[GP_MM_RAM_PBM_OPTS_SIZE];

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(pbmHandle) && GP_HAL_IS_PBM_ALLOCATED(pbmHandle));

    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmHandle);

    MEMSET((UInt8*)tempBuffer, 0, GP_MM_RAM_PBM_OPTS_SIZE);
    GP_HAL_WRITE_BYTE_STREAM(pbmOptAddress, tempBuffer, GP_MM_RAM_PBM_OPTS_SIZE);
}

void gpHal_PbmGetMemoryIndexRange(UInt16 size, UInt8* pStart, UInt8* pEnd, UInt16* pAccumulatedSize)
{
    UIntLoop i;
    UInt8 start = 0;
    UInt8 end = 0;
    UInt16 accumulatedSize = 0;

    for(i = 0; i < number_of_elements(gpHal_PbmResources); i++)
    {
        if(i != 0)
        {
            start += gpHal_PbmResources[i - 1].amount;
            accumulatedSize += (gpHal_PbmResources[i - 1].amount * gpHal_PbmResources[i - 1].size);
        }

        end += gpHal_PbmResources[i].amount;

        if(gpHal_PbmResources[i].size == size)
        {
            break;
        }
    }

    *pStart = start;
    *pEnd = end - 1;
    *pAccumulatedSize = accumulatedSize;
}

#ifdef GP_COMP_GPHAL_BLE
gpHal_BleTxPhy_t gpHal_PhyModeToBleTxPhyMode(UInt8 phyMode)
{
    // Map phy_mode (zb + ble type) to a gpHal_BleTxPhy_t mode
    // Note that phy_mode is used in FORMAT_R_PHY_MODE and PAR_PHY_MODE
    GP_ASSERT_DEV_INT(GP_WB_ENUM_AK_PHY_MODE_BLE_LR500 >= phyMode);
    GP_ASSERT_DEV_INT(GP_WB_ENUM_AK_PHY_MODE_ZB != phyMode);

    if((GP_WB_ENUM_AK_PHY_MODE_BLE_LR500 < phyMode) || (GP_WB_ENUM_AK_PHY_MODE_ZB == phyMode))
    {
        return gpHal_BleTxPhyInvalid;
    }

    return (gpHal_BleTxPhy_t)(phyMode - 1);
}
#endif // GP_COMP_GPHAL_BLE

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_PbmInit(void)
{
    // Check calculation if value changes.
    COMPILE_TIME_ASSERT(GP_HAL_NR_OF_RCI_QUEUES == 10);

    GP_HAL_REGISTER_AKRAM_REGION(gpHal_PbmDescriptors);
    GP_HAL_REGISTER_AKRAM_REGION(gpHal_PbmDataMemory);
    GP_HAL_REGISTER_AKRAM_REGION(gpHal_formatRList);
    GP_HAL_REGISTER_AKRAM_REGION(gpHal_RCIEntryList);

    if(!gpHal_PbmInitialized)
    {
        UIntLoop i;

        // Check if all parameters are correctly configured
        gpHal_PbmCheckConfiguration();

#ifdef GP_DIVERSITY_DEVELOPMENT
        gpHal_PbmDumpConfiguration();
#endif // GP_DIVERSITY_DEVELOPMENT

        // Default initialization
        for(i = 0; i < GP_HAL_NR_OF_PBMS_CONFIGURED; i++)
        {
            gpHal_PbmDescriptors[i].inUse = false;
            gpHal_PbmDescriptors[i].txPbm = false;
            gpHal_PbmDescriptors[i].pbmLength = 0;
            gpHal_PbmDescriptors[i].pData = NULL;
            memset(gpHal_PbmDescriptors[i].opts, 0, GP_MM_RAM_PBM_OPTS_SIZE);
        }

        gpHal_PbmMemoryAllocated = 0;

        // Configure RX pbms - pbms that can be claimed by HW
        gpHal_PbmConfigureRxPbms();

        // Configure TX pbms - pbms that can be claimed by UC
        gpHal_PbmConfigureTxPbms();
        gpHal_PbmInitialized = true;

        // setup RCI queue
        GP_WB_WRITE_RT_RCI_ENTRIES_BASE_PTR(TO_RT_ADDR(GP_HAL_PHYSICAL_TO_ARM((UInt32)gpHal_RCIEntryList)));
        GP_WB_WRITE_RT_RCI_ENTRIES_NR(GP_HAL_NR_OF_RCI_ENTRIES);
    }
}

UInt8 gpHal_GetHandle(UInt16 size)
{
    UIntLoop i;
    UInt8 pbmHandle;

    for(i = 0; i < number_of_elements(gpHal_PbmResources); i++)
    {
        // Check the size is supported
        if(size == gpHal_PbmResources[i].size)
        {
            break;
        }
    }

    if(i == number_of_elements(gpHal_PbmResources))
    {
        // Unsupported size specified
        return GP_PBM_INVALID_HANDLE;
    }

    pbmHandle = gpHal_GetPbmWithMemory(size);
    GP_LOG_PRINTF("allocPbm: h %d", 0, pbmHandle);
    return pbmHandle;
}

UInt8 gpHal_findPbmHandle(UInt32 optsbase)
{
    UInt8 i;
    /* find a free descriptor */
    for(i = 0; i < GP_HAL_NR_OF_PBMS_CONFIGURED; i++)
    {
        if(gpHal_PbmDescriptors[i].inUse &&
           (UInt32)GP_HAL_PHYSICAL_TO_ARM(gpHal_PbmDescriptors[i].opts) == optsbase)
        {
            return i;
        }
    }
    return GP_PBM_INVALID_HANDLE;
}

void gpHal_FreeHandle(UInt8 handle)
{
    gpHal_PbmDescriptor_t* pDesc;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(handle) && GP_HAL_IS_PBM_ALLOCATED(handle));

    pDesc = gpHal_PbmDescriptorGet(handle);

    GP_ASSERT_DEV_EXT(pDesc != NULL);

    if(pDesc->txPbm)
    {
        // The descriptor should only be freed for TX PBMs, an RX PBM should have its descriptor available at all times
        gpHal_PbmDescriptorFree(pDesc);
    }
    else
    {
        // add the format r ptr back to format r ptrs queue to be used by RT system again
        gpHal_RtAddFrPtr(GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(handle));
    }

    GP_LOG_PRINTF("freePbm: h %d, PBMs allocated = 0x%x", 0, handle, gpHal_PbmMemoryAllocated);
}

// Lqi for K7 pbm is invalid, we calculate the lqi from the rssi value
UInt8 gpHal_GetContinuousModeLQI(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    {
        Int8 rssi;
        rssi = gpHal_CalculateRSSI(GP_WB_READ_FORMAT_R_RSSI(pbmOptAddress));
        return gpHal_CalculateLQIfromRSSI(rssi);
    }
}

Int8 gpHal_GetRSSI(UInt8 PBMentry)
{
    return gpHal_GetContinuousModeRSSI(PBMentry);
}

UInt8 gpHal_GetLQI(UInt8 PBMentry)
{
    return gpHal_GetContinuousModeLQI(PBMentry);
}

Int8 gpHal_GetSensitivityLevel(void)
{
#if defined(GP_COMP_GPHAL_MAC)
    Bool enableMultiStandard, enableMultiChannel, enableHighSensitivity;
    gpHal_GetMacRxMode(&enableMultiStandard, &enableMultiChannel, &enableHighSensitivity);
    if(enableHighSensitivity)
    {
        return GP_HAL_SENSITIVITY_LEVEL_HS;
    }
    else
    {
        return GP_HAL_SENSITIVITY_LEVEL_LPL;
    }
#else
    GP_ASSERT_DEV_INT(false);
    return 0;
#endif
}

Int8 gpHal_GetContinuousModeRSSI(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    return (Int8)gpHal_CalculateRSSI(GP_WB_READ_FORMAT_R_RSSI(pbmOptAddress));
}

void gpHal_GetRxTimestamp(UInt8 PBMentry, UInt32* pTimeStamp)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    *pTimeStamp = GP_WB_READ_FORMAT_R_TIMESTAMP(pbmOptAddress);
}

UInt16* gpHal_GetPhaseSamples(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    UInt32 addr;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);

    addr = FROM_RT_ADDR(GP_WB_READ_FORMAT_R_RAW_PHASE_SAMPLE_PTR(pbmOptAddress));

    return (UInt16*)(addr);
}

UInt8 gpHal_GetRxedChannel(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    return GP_WB_READ_FORMAT_R_CHANNEL(pbmOptAddress) + 11;
}

Int32 gpHal_GetRxedFreqOffset(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    return (GP_HAL_FREQOFFSET_HZPERUNIT * GP_WB_READ_FORMAT_R_FOFFSET(pbmOptAddress));
}

UInt16 gpHal_GetRxedAntenna(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    return GP_WB_READ_FORMAT_R_ANTENNA(pbmOptAddress);
}

void gpHal_GetTxTimestamp(UInt8 PBMentry, UInt32* pTimeStamp)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    *pTimeStamp = GP_WB_READ_ZB_FORMAT_T_TIMESTAMP(pbmOptAddress);
}

UInt8 gpHal_GetTxAckLQI(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    UInt8 lqi = 0;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);

    if(GP_WB_READ_ZB_FORMAT_T_RETURN_CODE(pbmOptAddress) == GP_WB_ENUM_PBM_RETURN_CODE_TX_SUCCESS &&
       GP_WB_READ_ZB_FORMAT_T_ACKED_MODE(pbmOptAddress))
    {
        Int8 rssi = gpHal_CalculateRSSI(GP_WB_READ_ZB_FORMAT_T_ACK_RSSI(pbmOptAddress));
        lqi = gpHal_CalculateLQIfromRSSI(rssi);
    }

    return lqi;
}

Int8 gpHal_GetTxAckRssi(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    Int8 rssi = 0;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);

    if(GP_WB_READ_ZB_FORMAT_T_RETURN_CODE(pbmOptAddress) == GP_WB_ENUM_PBM_RETURN_CODE_TX_SUCCESS &&
       GP_WB_READ_ZB_FORMAT_T_ACKED_MODE(pbmOptAddress))
    {
        rssi = gpHal_CalculateRSSI(GP_WB_READ_ZB_FORMAT_T_ACK_RSSI(pbmOptAddress));
    }

    return rssi;
}

UInt8 gpHal_GetTxCCACntr(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    return GP_WB_READ_ZB_FORMAT_T_CSMA_CA_ENABLE(pbmOptAddress) ? GP_WB_READ_ZB_FORMAT_T_CCA_CNT(pbmOptAddress) : 0;
}

UInt8 gpHal_GetTxRetryCntr(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    return GP_WB_READ_ZB_FORMAT_T_RETRY_CNT(pbmOptAddress);
}

UInt8 gpHal_GetFramePendingFromTxPbm(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    return GP_WB_READ_ZB_FORMAT_T_ACK_FP(pbmOptAddress);
}

Bool gpHal_GetRxEnhancedAckFromTxPbm(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    GP_LOG_PRINTF("sec: 0x%x%04x fv: 0x%x", 0, GP_LOG_SPLIT_UINT32(GP_WB_READ_ZB_FORMAT_T_ACK_SEC(pbmOptAddress)),
                  GP_WB_READ_ZB_FORMAT_T_ACK_FRAME_VERS_MASK(pbmOptAddress));
    return (
        (GP_WB_READ_ZB_FORMAT_T_ACK_SEC(pbmOptAddress)) &&
        (GP_WB_GET_ZBFILT_FV_2015_FROM_FRAME_VERSION_MASK(GP_WB_READ_ZB_FORMAT_T_ACK_FRAME_VERS_MASK(pbmOptAddress))));
}

UInt16 gpHal_GetFrameControlFromTxAckAfterRx(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
    UInt16 framecontrol = 0x00;
    UInt16 counter = 0;
    UInt16 break_loop = (UInt16)-1;
    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);

    do
    {
        framecontrol = GP_WB_READ_FORMAT_R_ACK_FRAME_CTRL(pbmOptAddress);
        counter++;
        GP_ASSERT_DEV_EXT(counter < break_loop);
    } while(framecontrol == 0xFC);
#ifdef GP_DIVERSITY_DEVELOPMENT
    GP_LOG_PRINTF("FC for Tx Ack: 0x%x fp: %d | %lu", 0, framecontrol,
                  GP_WB_GET_FORMAT_R_ACK_FP_FROM_ACK_FRAME_CTRL(framecontrol), (unsigned long)counter);
#else
    GP_LOG_PRINTF("FC for Tx Ack: 0x%x fp: %d", 0, framecontrol,
                  GP_WB_GET_FORMAT_R_ACK_FP_FROM_ACK_FRAME_CTRL(framecontrol));
#endif // GP_DIVERSITY_DEVELOPMENT
    return framecontrol;
}

UInt32 gpHal_GetFrameCounterFromTxAckAfterRx(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
#ifndef GP_COMP_CHIPEMU
    const UInt32 maxWaitTimeForAckInfo = 100;
#endif // not GP_COMP_CHIPEMU

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
#ifndef GP_COMP_CHIPEMU
    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_FORMAT_R_ACK_INFO_VALID(pbmOptAddress) == 0, maxWaitTimeForAckInfo);
#endif // not GP_COMP_CHIPEMU
    GP_LOG_PRINTF("Ack PBM: %d fcount:  0x%08x", 0, PBMentry,
                  (UInt16)GP_WB_READ_FORMAT_R_ACK_FRAMECOUNTER(pbmOptAddress));
    return GP_WB_READ_FORMAT_R_ACK_FRAMECOUNTER(pbmOptAddress);
}

UInt8 gpHal_GetKeyIdFromTxAckAfterRx(UInt8 PBMentry)
{
    gpHal_Address_t pbmOptAddress;
#ifndef GP_COMP_CHIPEMU
    const UInt32 maxWaitTimeForAckInfo = 100;
#endif // not GP_COMP_CHIPEMU

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(PBMentry) && GP_HAL_IS_PBM_ALLOCATED(PBMentry));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
#ifndef GP_COMP_CHIPEMU
    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_FORMAT_R_ACK_INFO_VALID(pbmOptAddress) == 0, maxWaitTimeForAckInfo);
#endif // not GP_COMP_CHIPEMU
    GP_LOG_PRINTF("Ack PBM: %d keyid:  0x%02x", 0, PBMentry, (UInt16)GP_WB_READ_FORMAT_R_ACK_KEYID(pbmOptAddress));
    return GP_WB_READ_FORMAT_R_ACK_KEYID(pbmOptAddress);
}

Int8 gpHal_CalculateRSSI(UInt8 protoRSSI)
{
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    return (Int8)ak_rap_rx_rssi_to_dbm(protoRSSI);
#else
    // RSSI(proto)->RSSI(dBm)
    Int32 temp = -115 + (protoRSSI / 2);

    if(temp > INT8_MAX)
    {
        temp = INT8_MAX;
    }
    if(temp < INT8_MIN)
    {
        temp = INT8_MIN;
    }


    return (Int8)temp;
#endif
}

UInt8 gpHal_CalculateLQIfromRSSI(Int8 RSSI)
{
    UInt16 temp;

    // Floor
    if(RSSI < -92) // Minimum dB value for LQI 0x00
    {
        temp = 0;
    }
    // Ceil
    else if(RSSI > -62) // Maximum dB value for LQI 0xFF
    {
        temp = 255;
    }
    else
    {
        // Let result start on zero and multiply with 8.5 to map the 40 dB range to 0 -> 0xFF (255/30)
        temp = ((Int16)RSSI + 92) * 85;
        temp /= 10;
    }

    return (UInt8)temp;
}

UInt8 gpHal_CalculateProtoRSSI(Int8 rssi)
{
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    UInt8 protoRSSI = 0;
    protoRSSI = ak_rap_rx_dbm_to_rssi(rssi);
    return (protoRSSI);
#else
    // RSSI (dBm) -> RSSI (proto)
    // Int32 protoRSSI = (rssi + 115) * 2;
    Int32 protoRSSI = (rssi + 124) * 2;


    if(protoRSSI > UINT8_MAX)
    {
        protoRSSI = UINT8_MAX;
    }

    if(protoRSSI < 0)
    {
        protoRSSI = 0;
    }

    return (UInt8)protoRSSI;
#endif
}

void gpHal_WriteDataInPBMCyclic(gpHal_Address_t pbmAddr, UInt8 pbmOffset, UInt8* pData, UInt8 length)
{
    UInt8 splitlength = 0;
    if(length == 0)
    {
        return;
    }
    if((pbmOffset + (length)) > GP_MM_RAM_PBM_ZIGBEE_DATA_SIZE)
    {
        // split in 2
        splitlength = pbmOffset + (length)-GP_MM_RAM_PBM_ZIGBEE_DATA_SIZE;
        // write second part
        GP_HAL_WRITE_DATA_IN_PBM(pbmAddr, (UInt8*)((pData) + (length)-splitlength), splitlength, 0);
    }
    // write first part or full part
    GP_HAL_WRITE_DATA_IN_PBM(pbmAddr, (pData), ((length)-splitlength), pbmOffset);
}
void gpHal_ReadDataInPBMCyclic(gpHal_Address_t pbmAddr, UInt8 pbmOffset, UInt8* pData, UInt8 length)
{
    UInt8 splitlength = 0;
    if((pbmOffset + (length)) > GP_MM_RAM_PBM_ZIGBEE_DATA_SIZE)
    {
        // split in 2
        splitlength = pbmOffset + (length)-GP_MM_RAM_PBM_ZIGBEE_DATA_SIZE;
        GP_HAL_READ_DATA_IN_PBM(pbmAddr, (UInt8*)((pData) + (length)-splitlength), splitlength, 0);
    }
    // write first part or full part
    GP_HAL_READ_DATA_IN_PBM(pbmAddr, (pData), ((length)-splitlength), pbmOffset);
}

void gpHal_WriteByteInPBMCyclic(gpHal_Address_t pbmAddr, UInt8 pbmOffset, UInt8 byte)
{
    gpHal_WriteDataInPBMCyclic(pbmAddr, pbmOffset, &byte, 1);
}

UInt8 gpHal_ReadByteInPBMCyclic(gpHal_Address_t pbmAddr, UInt8 pbmOffset)
{
    UInt8 byte;
    gpHal_ReadDataInPBMCyclic(pbmAddr, pbmOffset, &byte, 1);
    return byte;
}

void gpHal_WriteDataInPBM(gpHal_Address_t pbmAddr, UInt16 pbmOffset, UInt8* pData, UInt8 length)
{
    // GP_LOG_SYSTEM_PRINTF("wr: addr: %lx, offset: %x len: %x",0,pbmAddr,pbmOffset, length);
    // gpLog_PrintBuffer(length,pData);
    GP_HAL_WRITE_DATA_IN_PBM(pbmAddr, pData, length, pbmOffset);
}

void gpHal_ReadDataInPBM(gpHal_Address_t pbmAddr, UInt16 pbmOffset, UInt8* pData, UInt8 length)
{
    GP_HAL_READ_DATA_IN_PBM(pbmAddr, pData, length, pbmOffset);
}

void gpHal_WriteByteInPBM(gpHal_Address_t pbmAddr, UInt16 pbmOffset, UInt8 byte)
{
    // GP_LOG_SYSTEM_PRINTF("wr: addr: %lx, offset: %x len: %x en byte: %x",0,pbmAddr,pbmOffset, 1, byte);
    GP_HAL_WRITE_BYTE_IN_PBM(pbmAddr, byte, pbmOffset);
}

UInt8 gpHal_ReadByteInPBM(gpHal_Address_t pbmAddr, UInt16 pbmOffset)
{
    return GP_HAL_READ_BYTE_IN_PBM(pbmAddr, pbmOffset);
}

void gpHal_MakeBareCopyPBM(UInt8 PBMentryOrig, UInt8 PBMentryDst)
{
    // // not implemented
    // GP_ASSERT_SYSTEM(false);
    gpHal_Address_t tempCopyAddress;
    UInt8 tempBuffer[0x44];

    // copy PBM options - GP_MM_RAM_PBM_OPTS_SIZE
    COMPILE_TIME_ASSERT(0x44 == GP_MM_RAM_PBM_OPTS_SIZE); // Assumption here
    GP_HAL_READ_BYTE_STREAM(GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentryOrig), tempBuffer, GP_MM_RAM_PBM_OPTS_SIZE);
    /*
        GP_LOG_SYSTEM_PRINTF("CP_: addr:%lx -> %lx : %x %x %x %x",0,GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentryOrig),
                                                                   GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentryDst),
                                                                   tempBuffer[0], tempBuffer[1],
                                                                   tempBuffer[2], tempBuffer[3]);
    */
    GP_HAL_WRITE_BYTE_STREAM(GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentryDst), tempBuffer, GP_MM_RAM_PBM_OPTS_SIZE);

    // copy PBM data - GP_MM_RAM_PBM_DATA_SIZE
    COMPILE_TIME_ASSERT((GP_MM_RAM_PBM_ZIGBEE_DATA_SIZE % 32) == 0); // Assumption here
    for(tempCopyAddress = 0; tempCopyAddress < GP_MM_RAM_PBM_ZIGBEE_DATA_SIZE; tempCopyAddress += 32)
    {
        GP_HAL_READ_BYTE_STREAM(GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(PBMentryOrig) + tempCopyAddress, tempBuffer, 32);
        /*
                GP_LOG_SYSTEM_PRINTF("CP: addr:%lx -> %lx : %x %x %x %x",0,
           GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(PBMentryOrig) + tempCopyAddress, GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(PBMentryDst)
           + tempCopyAddress, tempBuffer[0], tempBuffer[1], tempBuffer[2], tempBuffer[3]);
        */
        GP_HAL_WRITE_BYTE_STREAM(GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(PBMentryDst) + tempCopyAddress, tempBuffer, 32);
    }
}

#ifdef GP_COMP_GPHAL_BLE
gpHal_Result_t gpHal_PbmGetEventCounter(UInt8 pbmHandle, UInt16* pEventCount)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(pbmHandle) && GP_HAL_IS_PBM_ALLOCATED(pbmHandle));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmHandle);

    *pEventCount = GP_WB_READ_FORMAT_R_EVENT_COUNTER(pbmOptAddress);

    return gpHal_ResultSuccess;
}

void gpHal_PbmSetCteLengthUs(UInt8 pbmHandle, UInt8 cteLengthUs)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(pbmHandle) && GP_HAL_IS_PBM_ALLOCATED(pbmHandle));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmHandle);

    GP_WB_WRITE_BLE_FORMAT_T_SUPPLEMENTAL_LEN(pbmOptAddress, cteLengthUs);
    GP_ASSERT_SYSTEM(false);
}

UInt8 gpHal_PbmGetBleRxPhy(UInt8 pbmHandle)
{
    gpHal_Address_t pbmOptAddress;

    GP_ASSERT_DEV_EXT(GP_HAL_CHECK_PBM_VALID(pbmHandle) && GP_HAL_IS_PBM_ALLOCATED(pbmHandle));
    pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmHandle);

    return gpHal_PhyModeToBleTxPhyMode(GP_WB_READ_FORMAT_R_PHY_MODE(pbmOptAddress));
}
#endif // GP_COMP_GPHAL_BLE
