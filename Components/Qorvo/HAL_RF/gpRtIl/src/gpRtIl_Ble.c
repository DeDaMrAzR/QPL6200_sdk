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

// #define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_RTIL

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpRtIl.h"
#include "gpRtIl_defs.h"
#include "gpLog.h"
#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_Ble_DEFS.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Component Function Definitions
 *****************************************************************************/

// Return code!!!
gpHal_Result_t RtIl_BleEventStart(UInt8 eventNr, UInt32 timeStamp)
{
    gpHal_Result_t result;
    ble_mgr_start_event_args_t eventArgs;

    // Populate IPC arguments for BLE event manager
    eventArgs.event_nr = eventNr;
    eventArgs.schedule_time = timeStamp;

    // Let the BLE ev mgr start and validate this event
    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_START_EVENT, sizeof(ble_mgr_start_event_args_t), (UInt8*)&eventArgs);

    return result;
}

gpHal_Result_t RtIl_BleEventStop(UInt8 eventId)
{
    gpHal_Result_t result;

    // trigger IPC command to BLE ev mgr
    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_STOP_EVENT, sizeof(eventId), &eventId);

    if(result == gpHal_ResultSuccess)
    {
        // If we stop an event, we should also clear any pending interrupt for this event
        GP_WB_WRITE_AK_IPC_CLR_RT2X_EVENT_PROCESSED_INTERRUPTS(1 << eventId);
        HAL_DISABLE_GLOBAL_INT();
        if ((UInt16)0 == GP_WB_READ_AK_IPC_UNMASKED_RT2X_EVENT_PROCESSED_INTERRUPTS())
        {
            GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_EVENT_PROCESSED_INTERRUPT(1);
        }
        HAL_ENABLE_GLOBAL_INT();
        gpHal_RtEventsFree(eventId);
    }

    return result;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpRtIl_BleInit(gpHal_BleCallbacks_t* pCallbacks)
{
    RtIl_BleChannelMapInit();



}
