    /*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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

#include "gpRtIl.h"
#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_Ble_DEFS.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#define GPRTIL_BLE_CHANNELMAP_NR_OF_BYTES   (GPHAL_BLE_MAX_NR_OF_SUPPORTED_CHANNEL_MAPS * GPHAL_BLE_CHAN_MAP_INFO_SIZE)

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

GP_COMPILE_TIME_VERIFY((GPHAL_BLE_CHAN_MAP_INFO_SIZE % GP_WB_MAX_MEMBER_SIZE) == 0);
GP_COMPILE_TIME_VERIFY(GPHAL_BLE_CHAN_MAP_INFO_SIZE >= GP_WB_CONN_CH_MAP_SIZE);

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/******************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static COMPILER_ALIGNED(GP_WB_MAX_MEMBER_SIZE) UInt8 gpHal_BleChannelMapData[GPRTIL_BLE_CHANNELMAP_NR_OF_BYTES] LINKER_SECTION(".rt_fast_ram");

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static gpHal_Address_t RtIl_BleChannelMapHandleToStartAddress(gpHal_BleChannelMapHandle_t handle);
static UInt16 RtIl_BleChannelMapHandleToOffset(gpHal_BleChannelMapHandle_t handle);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

gpHal_Address_t RtIl_BleChannelMapHandleToStartAddress(gpHal_BleChannelMapHandle_t handle)
{
    return GP_HAL_PHYSICAL_TO_ARM(gpHal_BleChannelMapData) + (handle * GPHAL_BLE_CHAN_MAP_INFO_SIZE);
}

UInt16 RtIl_BleChannelMapHandleToOffset(gpHal_BleChannelMapHandle_t handle)
{
    gpHal_Address_t startAddress = RtIl_BleChannelMapHandleToStartAddress(handle);
    return TO_RT_ADDR(startAddress);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void RtIl_BleChannelMapInit(void)
{
    GP_HAL_REGISTER_AKRAM_REGION(gpHal_BleChannelMapData);

}

void gpRtIl_BleWriteChannelMap(gpHal_BleChannelMapHandle_t handle, gpHal_ChannelMap_t* pChanMap)
{
    gpHal_Address_t chanMapPtr;

    if(handle >= GPHAL_BLE_MAX_NR_OF_SUPPORTED_CHANNEL_MAPS)
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }

    chanMapPtr = RtIl_BleChannelMapHandleToStartAddress(handle);

    GP_HAL_WRITE_BYTE_STREAM(chanMapPtr, pChanMap->usedChanIds, sizeof(pChanMap->usedChanIds));
    GP_HAL_WRITE_BYTE_STREAM(chanMapPtr + GP_WB_CONN_CH_MAP_HOP_REMAP_0_ADDRESS, pChanMap->remapTable, sizeof(pChanMap->remapTable));
    GP_WB_WRITE_CONN_CH_MAP_HOP_REMAP_TABLE_LEN(chanMapPtr, pChanMap->hopRemapTableLength);
}

void gpRtIl_BleReadChannelMap(gpHal_BleChannelMapHandle_t handle, UInt8* pChanMap)
{
    gpHal_Address_t chanMapPtr;

    if(handle >= GPHAL_BLE_MAX_NR_OF_SUPPORTED_CHANNEL_MAPS)
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }

    chanMapPtr = RtIl_BleChannelMapHandleToStartAddress(handle);

    GP_HAL_READ_BYTE_STREAM(chanMapPtr, pChanMap, GP_HAL_CHANNEL_MAP_LENGTH);
}

UInt16 gpRtIl_BleChannelMapHandleToOffset(gpHal_BleChannelMapHandle_t handle)
{
    return RtIl_BleChannelMapHandleToOffset(handle);
}

// Typically called when a service wants to switch to a new channel map handle (when the old is not longer needed)
void gpRtIl_BleSwitchChannelMap(gpHal_BleChannelMapHandle_t* pHandleCurrent, gpHal_BleChannelMapHandle_t* pHandleNew, UInt8* pCompressedPtr)
{
    *pHandleCurrent = *pHandleNew;
    *pHandleNew = GP_RTIL_BLE_CHANNEL_MAP_HANDLE_INVALID;

    if(pCompressedPtr != NULL)
    {
        UInt16* pChannelMap = (UInt16*)(void*)pCompressedPtr;
        *pChannelMap = gpRtIl_BleChannelMapHandleToOffset(*pHandleCurrent);
    }
}
