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
 * $Header$
 * $Change$
 * $DateTime$
 *
 * General code regarding channel maps - available for all devices where channel maps are used
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

// #define GP_LOCAL_LOG

#include "gpBleChannelMapManager.h"
#include "gpBleChannelMapManager_defs.h"
#include "gpRtIl.h"
#include "gpLog.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLECHANNELMAPMANAGER

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    gpBleChannelMapHandle_t handle;
    UInt8 nrOfUsers;
} BleChannelMapManager_MapContext_t;

/******************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// Context per channel map
BleChannelMapManager_MapContext_t BleChannelMapManager_MapContext[GPHAL_BLE_MAX_NR_OF_SUPPORTED_CHANNEL_MAPS];

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

void BleChannelMapManager_PopulateRemappingTable(gpHal_ChannelMap_t* pChanMap, gpHci_ChannelMap_t* pChannelMapHci);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void BleChannelMapManager_PopulateRemappingTable(gpHal_ChannelMap_t* pChanMap, gpHci_ChannelMap_t* pChannelMapHci)
{
    UIntLoop i;

    // Copy info to the channel map
    MEMCPY(pChanMap->usedChanIds, pChannelMapHci->channels, sizeof(pChanMap->usedChanIds));
    MEMSET(pChanMap->remapTable, 0, sizeof(pChanMap->remapTable));

    pChanMap->hopRemapTableLength = 0;
    for(i = 0; i < BLE_DATA_NUMBER_OF_CHANNELS; i++)
    {
        UInt8 arrayIndex = (i >> 3);
        UInt8 arrayBitIndex = i - (arrayIndex << 3);

        if((pChanMap->usedChanIds[arrayIndex] & (1 << arrayBitIndex)) != 0)
        {
            // channel is used ==> add to remap table
            pChanMap->remapTable[pChanMap->hopRemapTableLength] = i;
            pChanMap->hopRemapTableLength++;
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpBleChannelMapManager_Init(void)
{
    UIntLoop i;

    for(i = 0; i < number_of_elements(BleChannelMapManager_MapContext); i++)
    {
        BleChannelMapManager_MapContext[i].handle = GPBLECHANNELMAPMANAGER_HANDLE_INVALID;
        BleChannelMapManager_MapContext[i].nrOfUsers = 0;
    }

}

void gpBleChannelMapManager_Reset(Bool firstReset)
{
}

gpBleChannelMapHandle_t gpBleChannelMapManager_AllocateHandle(gpHci_ChannelMap_t* pChannelMap)
{
    UIntLoop i;

    for(i = 0; i < number_of_elements(BleChannelMapManager_MapContext); i++)
    {
        if(BleChannelMapManager_MapContext[i].handle == GPBLECHANNELMAPMANAGER_HANDLE_INVALID)
        {
            BleChannelMapManager_MapContext[i].handle = i;
            BleChannelMapManager_MapContext[i].nrOfUsers = 0;

            GP_LOG_PRINTF("[ChMap %u] Alloc",0, BleChannelMapManager_MapContext[i].handle);
            gpBleChannelMapManager_AddUser(BleChannelMapManager_MapContext[i].handle);

            if(pChannelMap != NULL)
            {
                gpBleChannelMapManager_PopulateMap(BleChannelMapManager_MapContext[i].handle, pChannelMap);
            }

            return BleChannelMapManager_MapContext[i].handle;
        }
    }

    GP_ASSERT_DEV_INT(false);
    return GPBLECHANNELMAPMANAGER_HANDLE_INVALID;
}

Bool gpBleChannelMapManager_IsHandleInUse(gpBleChannelMapHandle_t handle)
{
    UIntLoop i;

    if(handle == GPBLECHANNELMAPMANAGER_HANDLE_INVALID)
    {
        return false;
    }

    for(i = 0; i < number_of_elements(BleChannelMapManager_MapContext); i++)
    {
        if(BleChannelMapManager_MapContext[i].handle == handle)
        {
            return true;
        }
    }

    return false;
}

Bool gpBleChannelMapManager_AddUser(gpBleChannelMapHandle_t handle)
{
    UIntLoop i;

    if(handle == GPBLECHANNELMAPMANAGER_HANDLE_INVALID)
    {
        GP_LOG_PRINTF("[ChMap %u] AddUser f1",0, handle);
        return false;
    }

    for(i = 0; i < number_of_elements(BleChannelMapManager_MapContext); i++)
    {
        if(BleChannelMapManager_MapContext[i].handle == handle)
        {
            if(BleChannelMapManager_MapContext[i].nrOfUsers == 0xFF)
            {
                GP_LOG_PRINTF("[ChMap %u] AddUser f2",0, handle);
                return false;
            }

            GP_LOG_PRINTF("[ChMap %u] AddUser success %u -> %u",0, handle, BleChannelMapManager_MapContext[i].nrOfUsers, BleChannelMapManager_MapContext[i].nrOfUsers+1);
            BleChannelMapManager_MapContext[i].nrOfUsers++;
            return true;
        }
    }

    return false;
}

Bool gpBleChannelMapManager_RemoveUser(gpBleChannelMapHandle_t handle)
{
    UIntLoop i;

    if(handle == GPBLECHANNELMAPMANAGER_HANDLE_INVALID)
    {
        GP_LOG_PRINTF("[ChMap %u] RemoveUser f1",0, handle);
        return false;
    }

    for(i = 0; i < number_of_elements(BleChannelMapManager_MapContext); i++)
    {
        if(BleChannelMapManager_MapContext[i].handle == handle)
        {
            if(BleChannelMapManager_MapContext[i].nrOfUsers == 0)
            {
                GP_LOG_PRINTF("[ChMap %u] RemoveUser f2",0, handle);
                return false;
            }

            BleChannelMapManager_MapContext[i].nrOfUsers--;
            GP_LOG_PRINTF("[ChMap %u] RemoveUser success (new count %u)",0, handle, BleChannelMapManager_MapContext[i].nrOfUsers);

            // Auto free handle in case there are no users left
            if(BleChannelMapManager_MapContext[i].nrOfUsers == 0)
            {
                gpBleChannelMapManager_FreeHandle(handle);
            }

            return true;
        }
    }

    return false;
}

Bool gpBleChannelMapManager_FreeHandle(gpBleChannelMapHandle_t handle)
{
    UIntLoop i;

    if(handle == GPBLECHANNELMAPMANAGER_HANDLE_INVALID)
    {
        GP_LOG_PRINTF("[ChMap %u] Free f1",0, handle);
        return false;
    }

    for(i = 0; i < number_of_elements(BleChannelMapManager_MapContext); i++)
    {
        if(BleChannelMapManager_MapContext[i].handle == handle)
        {
            if(BleChannelMapManager_MapContext[i].nrOfUsers == 0)
            {
                GP_LOG_PRINTF("[ChMap %u] Free success",0, handle);
                BleChannelMapManager_MapContext[i].handle = GPBLECHANNELMAPMANAGER_HANDLE_INVALID;


                return true;
            }

            GP_LOG_PRINTF("[ChMap %u] Free f2",0, handle);
            return false;
        }
    }

    GP_LOG_PRINTF("[ChMap %u] Free f3",0, handle);
    return false;
}

Bool gpBleChannelMapManager_PopulateMap(gpBleChannelMapHandle_t handle, gpHci_ChannelMap_t* pChannelMap)
{
    gpHal_ChannelMap_t channelMap;

    if(!gpBleChannelMapManager_IsHandleInUse(handle))
    {
        GP_ASSERT_DEV_INT(false);
        return false;
    }

    // 3 msbs are reserved (cfr Vol 4 Part E $ 7.8.19) - must be ignored on reception
    pChannelMap->channels[4] &= GP_BLE_CHANNEL_MAP_MSB_MASK;

    BleChannelMapManager_PopulateRemappingTable(&channelMap, pChannelMap);

    gpRtIl_BleWriteChannelMap(handle, &channelMap);

    return true;
}

void gpBleChannelMapManager_GetChannelMap(gpBleChannelMapHandle_t handle, UInt8* pChannels)
{
    if(!gpBleChannelMapManager_IsHandleInUse(handle))
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }

    gpRtIl_BleReadChannelMap(handle, pChannels);
}

// Typically called when a service wants to switch to a new channel map handle (when the old is not longer needed)
void gpBleChannelMapManager_SwitchChannelMap(gpBleChannelMapHandle_t* pHandleCurrent, gpBleChannelMapHandle_t* pHandleNew, UInt8* pCompressedPtr)
{
    gpBleChannelMapManager_RemoveUser(*pHandleCurrent);

    gpRtIl_BleSwitchChannelMap(pHandleCurrent, pHandleNew, pCompressedPtr);
}

/*****************************************************************************
 *                    Service Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_BLE_ACL_CONNECTIONS_SUPPORTED
gpHci_Result_t gpBle_LeReadChannelMap(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;
    gpHci_ConnectionHandle_t connHandle = pParams->LeReadChannelMap.connectionHandle;

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result = gpBleLlcp_IsHostConnectionHandleValid(connHandle);

    if(result == gpHci_ResultSuccess)
    {
        pEventBuf->payload.commandCompleteParams.returnParams.leReadChannelMap.connectionHandle = BLE_CONN_HANDLE_HANDLE_GET(connHandle);
        gpBleLlcp_GetConnectionChannelMap(gpBleLlcp_HciHandleToIntHandle(connHandle), &pEventBuf->payload.commandCompleteParams.returnParams.leReadChannelMap.channelMap);
    }

    return result;
}
#endif //GP_DIVERSITY_BLE_ACL_CONNECTIONS_SUPPORTED
