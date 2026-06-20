/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_MAC.c
 *   This file contains the implementation of the MAC functions
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

#include "gpPd.h"

#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_Ble.h"
#include "gpHal_Ble_DEFS.h"
#include "gpHal_Ble_Manual.h"

//GP hardware dependent register definitions
#include "gpHal_HW.h"
#include "gpHal_reg.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// Context for events
static gpHal_BleConnectionContext_t gpHal_BleHandleEventMapping[(GPHAL_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS + (GPHAL_BLE_MAX_NR_OF_PHYS - 1))];

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_BleCommonInit(void)
{
    MEMSET(gpHal_BleHandleEventMapping, 0xFF, sizeof(gpHal_BleHandleEventMapping));
}

UInt8 gpHal_BlePdToPbm(gpPd_Loh_t pdLoh, Bool TxIfTooLate)
{
    UInt8 pbmHandle = 0xFF;
    gpHal_Address_t optsBase;
    gpHal_Address_t dataBase;

    // Check that pbm length is within limits
    if(pdLoh.length == 0 || pdLoh.length > GP_HAL_PBM_MAX_SIZE)
    {
        GP_LOG_SYSTEM_PRINTF("PdLoh with invalid len: %i",0,pdLoh.length);
        GP_ASSERT_DEV_INT(false);
        return pbmHandle;
    }

    pbmHandle = gpPd_DataRequest(&pdLoh);
    if(!GP_HAL_CHECK_PBM_VALID(pbmHandle))
    {
        return pbmHandle;
    }

    optsBase = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmHandle);
    dataBase = GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(pbmHandle);

    gpHal_BlePopulateDefaultPbmOptions(pbmHandle, TxIfTooLate);

    UInt16 offset = (UInt16) pdLoh.offset;
    UInt16 length = (UInt16) pdLoh.length;

    GP_WB_WRITE_BLE_FORMAT_T_FRAME_PTR(optsBase, dataBase + offset);
    GP_WB_WRITE_BLE_FORMAT_T_FRAME_LEN(optsBase, length);

    return pbmHandle;
}

gpHal_BleConnectionContext_t* gpHal_BleAddConnIdMapping(UInt8 connId)
{
    UIntLoop i;

    for(i = 0; i < GPHAL_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS; i++)
    {
        if(gpHal_BleHandleEventMapping[i].infoPtr == GP_HAL_INFO_PTR_INVALID)
        {
            gpHal_Address_t eventAddress;

            // An invalid infoPtr means this handle is not yet allocated
            gpHal_BleHandleEventMapping[i].eventNr = gpHal_RtEventsAllocate();
            gpHal_BleHandleEventMapping[i].infoPtr = GP_HAL_BLE_CONNECTION_TO_OFFSET_FROM_START(GPHAL_VIRT_CONNID_TO_MASTER_CONNID(connId));

            eventAddress = RT_EVENT_HANDLE_TO_OFFSET(gpHal_BleHandleEventMapping[i].eventNr);

            GP_WB_WRITE_RT_EVENT_INFO_PTR(eventAddress, gpHal_BleHandleEventMapping[i].infoPtr);

            gpHal_BleHandleEventMapping[i].connId = connId;
            gpHal_BleHandleEventMapping[i].running = false;
            gpHal_BleHandleEventMapping[i].virtual = false;

            return &gpHal_BleHandleEventMapping[i];
        }
    }
    return NULL;
}

gpHal_BleConnectionContext_t* gpHal_BleGetConnMappingFromId(UInt8 connId)
{
    UIntLoop i;

    if(connId == GPHAL_BLE_CONN_ID_INVALID)
    {
        return NULL;
    }

    for(i = 0; i < GPHAL_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS; i++)
    {
        if(gpHal_BleHandleEventMapping[i].connId == connId)
        {
            return &gpHal_BleHandleEventMapping[i];
        }
    }
    return NULL;
}

gpHal_BleConnectionContext_t* gpHal_BleGetConnMappingFromEvent(UInt8 eventNr)
{
    UIntLoop i;

    if(eventNr == RT_EVENT_HANDLE_INVALID)
    {
        return NULL;
    }

    for(i = 0; i < GPHAL_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS; i++)
    {
        if(gpHal_BleHandleEventMapping[i].eventNr == eventNr)
        {
            return &gpHal_BleHandleEventMapping[i];
        }
    }
    return NULL;
}

void gpHal_BleRemoveConnIdMapping(gpHal_BleConnectionContext_t* pMapping)
{
    GP_ASSERT_DEV_INT(pMapping != NULL);

    gpHal_RtEventsFree(pMapping->eventNr);
    pMapping->eventNr = RT_EVENT_HANDLE_INVALID;
    pMapping->infoPtr = GP_HAL_INFO_PTR_INVALID;
    pMapping->connId = GPHAL_BLE_CONN_ID_INVALID;
}

