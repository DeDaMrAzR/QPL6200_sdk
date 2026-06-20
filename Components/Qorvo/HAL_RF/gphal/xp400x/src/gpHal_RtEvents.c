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
#include "gpHal.h"
#include "gpHal_Ble_DEFS.h"
#include "gpHal_RtEvents.h"
#include "gpHal_DEFS.h"

#include "hal_SupplyVoltageMonitor.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#define GPHAL_RT_EVENTS_MEMORY_SIZE     (GP_WB_RT_EVENT_SIZE * RT_MAX_NR_OF_EVENTS)

#define GPHAL_RT_UNTIMED_TIMED_ENTRIES_MAX GP_HAL_PBM_TYPE1_AMOUNT

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

// The TTB currently only has 16 entries, so we should also limit the RT events array
GP_COMPILE_TIME_VERIFY(RT_MAX_NR_OF_EVENTS <= 16);

#define GP_HAL_RT_EVENTS_GET_UNTIMED_TIMED_OFFSET(i)    GP_HAL_PHYSICAL_TO_ARM((UInt32)&gpHal_RtUntimedTimedMemory[i])

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    UInt32 executionTime;
    UInt8 evType;
    UInt16 infoPtr;
    UInt16 nextPtr;
} gpHal_UntimedTimedEvent_t;

/******************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// Memory for rt_event addressmap - used as main structure to share event information between NRT and RT
static COMPILER_ALIGNED(4) UInt8 gpHal_RtEventsMemory[GPHAL_RT_EVENTS_MEMORY_SIZE] LINKER_SECTION(".rt_fast_ram");

static UInt16 gpHal_RtEventsAdministration;

// (Internal) memory for RT timed / untimed queue entries
static gpHal_UntimedTimedEvent_t gpHal_RtUntimedTimedMemory[GPHAL_RT_UNTIMED_TIMED_ENTRIES_MAX] LINKER_SECTION(".rt_fast_ram");

// This will be gone when we EOL xp400x
extern gpHal_BleServiceContext_t gpHal_BleServiceEventContext[GPHAL_BLE_NR_OF_SERVICE_EVENTS];

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void gpHal_RtEventsUntimedTimedQueueInit(void)
{
    UIntLoop i;
    rt_event_handle_t eventHandle;
    gpHal_RtEvent_t rtEvent;

    MEMSET(&rtEvent, 0, sizeof(rtEvent));

    GP_HAL_REGISTER_AKRAM_REGION(gpHal_RtUntimedTimedMemory);

    GP_WB_WRITE_RT_EV_LIST_FREE_PTR(TO_RT_ADDR(GP_HAL_RT_EVENTS_GET_UNTIMED_TIMED_OFFSET(0)));

    for (i = 0; i < (GPHAL_RT_UNTIMED_TIMED_ENTRIES_MAX - 1); i++)
    {
        GP_WB_WRITE_EV_LIST_ENTRY_NEXT_PTR(GP_HAL_RT_EVENTS_GET_UNTIMED_TIMED_OFFSET(i), TO_RT_ADDR(GP_HAL_RT_EVENTS_GET_UNTIMED_TIMED_OFFSET(i + 1)));
    }

    GP_WB_WRITE_EV_LIST_ENTRY_NEXT_PTR(GP_HAL_RT_EVENTS_GET_UNTIMED_TIMED_OFFSET(GPHAL_RT_UNTIMED_TIMED_ENTRIES_MAX - 1), 0);

    eventHandle = gpHal_RtEventsAllocate();
    GP_ASSERT_DEV_EXT(eventHandle != RT_EVENT_HANDLE_INVALID);
    GP_WB_WRITE_RT_EV_UNTIMED_NR(eventHandle);

    rtEvent.type = GP_WB_ENUM_RT_STATE_ZB_UNTIMED_EV;

    gpHal_BleStartEvent(eventHandle, GP_HAL_RT_TIMESTAMP_NOT_IMPORTANT, &rtEvent);
}

static void gpHal_RtEventTimedTxInit(void)
{
    GP_WB_WRITE_RT_EV_TIMED_TX_NR(RT_EVENT_HANDLE_INVALID);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_RtEventsInit(void)
{
    GP_HAL_REGISTER_AKRAM_REGION(gpHal_RtEventsMemory);

    gpHal_RtEventsAdministration = 0;

    GP_WB_WRITE_RT_EVENT_BASE_PTR(TO_RT_ADDR(GP_HAL_PHYSICAL_TO_ARM(gpHal_RtEventsMemory)));

    gpHal_RtEventsUntimedTimedQueueInit();

    gpHal_RtEventTimedTxInit();
}

rt_event_handle_t gpHal_RtEventsAllocate(void)
{
    UIntLoop i;

    for(i = 0; i < RT_MAX_NR_OF_EVENTS; i++)
    {
        if(!BIT_TST(gpHal_RtEventsAdministration, i))
        {
            gpHal_Address_t eventAddress = RT_EVENT_HANDLE_TO_OFFSET(i);

            MEMSET((UInt8*)GP_HAL_ARM_TO_PHYSICAL(eventAddress), 0, GP_WB_RT_EVENT_SIZE);
            GP_WB_WRITE_RT_EVENT_TTB_HANDLE(eventAddress, TTB_EVENT_HANDLE_INVALID);
            BIT_SET(gpHal_RtEventsAdministration, i);
#ifdef GP_COMP_GPHAL_BLE
            // Update cleanup time before starting the event
            gpHal_BleUpdateCleanupTime(gpHal_RtEventsMultipleAllocated());
#endif //GP_COMP_GPHAL_BLE
            return i;
        }
    }

    return RT_EVENT_HANDLE_INVALID;
}

void gpHal_RtEventsFree(rt_event_handle_t event_handle)
{
    if(event_handle == RT_MAX_NR_OF_EVENTS)
    {
        return;
    }

    BIT_CLR(gpHal_RtEventsAdministration, event_handle);
#ifdef GP_COMP_GPHAL_BLE
    // Activity was successfully removed, update cleanup time.
    gpHal_BleUpdateCleanupTime(gpHal_RtEventsMultipleAllocated());
#endif //GP_COMP_GPHAL_BLE
}

Bool gpHal_RtEventsMultipleAllocated(void)
{
    return (POPCOUNT(gpHal_RtEventsAdministration) > 1);
}

gpHal_Result_t gpHal_BleStartService(gpHal_RtEvent_t* pRtEvent, UInt32 timeStamp)
{
    gpHal_Result_t result;
    gpHal_Address_t eventAddress;
    gpHal_BleServiceEventId_t serviceEvent;

    if(pRtEvent->type == GPHAL_ENUM_EVENT_TYPE_ADVERTISE)
    {
        serviceEvent = gpHal_BleServiceEventIdAdvertising;
    }
    else if(pRtEvent->type == GP_WB_ENUM_RT_STATE_BLE_SUBEVENT)
    {
        serviceEvent = gpHal_BleServiceEventIdSubEv;
    }
    else if(pRtEvent->type == GPHAL_ENUM_EVENT_TYPE_BG_SCANNING)
    {
        serviceEvent = gpHal_BleServiceEventIdBgsc;
    }
    else
    {
        GP_ASSERT_DEV_EXT(false);
        return gpHal_ResultMemCapacityExceeded;
    }

    GP_ASSERT_DEV_INT(gpHal_BleServiceEventContext[serviceEvent].eventNr == RT_EVENT_HANDLE_INVALID);
    // Allocate and schedule the absolute event
    gpHal_BleServiceEventContext[serviceEvent].eventNr = gpHal_RtEventsAllocate();

    if(gpHal_BleServiceEventContext[serviceEvent].eventNr == RT_EVENT_HANDLE_INVALID)
    {
        return gpHal_ResultMemCapacityExceeded;
    }

    eventAddress = RT_EVENT_HANDLE_TO_OFFSET(gpHal_BleServiceEventContext[serviceEvent].eventNr);

    GP_WB_WRITE_RT_EVENT_INFO_PTR(eventAddress, GP_HAL_BLE_SERVICE_TO_OFFSET_FROM_START(serviceEvent));

    result = gpHal_BleStartEvent(gpHal_BleServiceEventContext[serviceEvent].eventNr, timeStamp, pRtEvent);
    return result;
}

gpHal_Result_t gpHal_BleRestartService(gpHal_BleServiceEventId_t serviceEvent, UInt32 timeStamp, UInt8 type)
{
    return gpHal_BleRestartEvent(gpHal_BleServiceEventContext[serviceEvent].eventNr, timeStamp);
}

gpHal_Result_t gpHal_BleStopService(gpHal_BleServiceEventId_t serviceEvent)
{
    gpHal_Result_t result;

    result = gpHal_BleStopEvent(gpHal_BleServiceEventContext[serviceEvent].eventNr);

    if(result == gpHal_ResultSuccess)
    {
        gpHal_RtEventsFree(gpHal_BleServiceEventContext[serviceEvent].eventNr);
        gpHal_BleServiceEventContext[serviceEvent].eventNr = RT_EVENT_HANDLE_INVALID;
    }

    return result;
}

// Use this function when a new event is started
gpHal_Result_t gpHal_BleStartEvent(rt_event_handle_t eventHandle, UInt32 timeStamp, gpHal_RtEvent_t* pRtEvent)
{
    gpHal_Result_t result;
    gpHal_Address_t eventAddress = RT_EVENT_HANDLE_TO_OFFSET(eventHandle);

    GP_WB_WRITE_RT_EVENT_TYPE(eventAddress, pRtEvent->type);
    GP_WB_WRITE_RT_EVENT_INTERVAL_US(eventAddress, pRtEvent->intervalUs);
    GP_WB_WRITE_RT_EVENT_PRIORITY(eventAddress, pRtEvent->priority);
    GP_WB_WRITE_RT_EVENT_EXTENDED_PRIO_EN(eventAddress, pRtEvent->enableExtPriority);
    GP_WB_WRITE_RT_EVENT_NR_CONSEC_SKIPPED_EVENTS(eventAddress, pRtEvent->nrOfConsecSkippedEvents);

    result = gpHal_BleRestartEvent(eventHandle, timeStamp);

    return result;
}

// Use this function when an already existing event is restarted
gpHal_Result_t gpHal_BleRestartEvent(rt_event_handle_t eventHandle, UInt32 timeStamp)
{
    gpHal_Result_t result;
    ble_mgr_start_event_args_t eventArgs;

    // Populate IPC arguments for BLE event manager
    eventArgs.event_nr = eventHandle;
    eventArgs.schedule_time = timeStamp;

    // Let the BLE ev mgr start and validate this event
    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_START_EVENT, sizeof(ble_mgr_start_event_args_t), (UInt8*)&eventArgs);

    return result;
}

gpHal_Result_t gpHal_BleStopEvent(rt_event_handle_t eventHandle)
{
    gpHal_Result_t result;

    // trigger IPC command to BLE ev mgr
    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_STOP_EVENT, sizeof(eventHandle), &eventHandle);

    if(result == gpHal_ResultSuccess)
    {
        // If we stop an event, we should also clear any pending interrupt for this event
        GP_WB_WRITE_AK_IPC_CLR_RT2X_EVENT_PROCESSED_INTERRUPTS(1 << eventHandle);
        GP_WB_WRITE_AK_IPC_CLR_RT2X_LAST_CONN_EVENT_CNT_INTERRUPTS(1 << eventHandle);

        // All low-priority interrupts are masked-off by the interrupt handler (gpHal_RadioInterruptsProcessIpcInterrupt)
        // until the unmasked interrupts are processed by the main-loop after which they are masked-on again
        // (gpHal_RadioInterruptsRestoreMasksAfterHandling).
        // This function is also called by the high-priorty interrupt handler that handles connection
        // creation. Clearing the interrupt here prevents the normal processing
        // of the interrupt by the main-loop (hal_HandlePolledPlatformInterrupt) and the the restoring of
        // the interrupt masks.
        // This is problematic in case the APPUC goes into WFI (waiting for interrupt) state because it
        // won't wake up for masked-off interrupts.
        // Therefore we restore the masks here if there are no pending interrupts for other events.
        // Since interrupt handler (gpHal_RadioInterruptsProcessIpcInterrupt) can disable masks concurrently
        HAL_DISABLE_GLOBAL_INT();
        if ((UInt16)0 == GP_WB_READ_AK_IPC_UNMASKED_RT2X_EVENT_PROCESSED_INTERRUPTS())
        {
            GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_EVENT_PROCESSED_INTERRUPT(1);
        }
        if ((UInt16)0 == GP_WB_READ_AK_IPC_UNMASKED_RT2X_LAST_CONN_EVENT_CNT_INTERRUPTS())
        {
            GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_LAST_CONN_EVENT_CNT_INTERRUPT(1);
        }
        HAL_ENABLE_GLOBAL_INT();
    }

    return result;
}

rt_event_handle_t gpHal_RtEventTimedTxAllocate(void)
{
    rt_event_handle_t eventHandle;
    gpHal_RtEvent_t rtEvent;

    MEMSET(&rtEvent, 0, sizeof(rtEvent));
    eventHandle = gpHal_RtEventsAllocate();
    GP_ASSERT_DEV_EXT(eventHandle != RT_EVENT_HANDLE_INVALID);
    GP_WB_WRITE_RT_EV_TIMED_TX_NR(eventHandle);
    rtEvent.type = GP_WB_ENUM_RT_STATE_ZB_UNTIMED_EV; // use the untimed type to ensure RT does not schedule the event yet

    gpHal_BleStartEvent(eventHandle, GP_HAL_RT_TIMESTAMP_NOT_IMPORTANT, &rtEvent);
    return eventHandle;
}

void gpHal_RtEventTimedTxRelease(rt_event_handle_t eventHandle)
{
    GP_ASSERT_DEV_EXT(eventHandle == GP_WB_READ_RT_EV_TIMED_TX_NR());

    gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_STOP_EVENT, sizeof(rt_event_handle_t), &eventHandle);
    GP_WB_WRITE_RT_EV_TIMED_TX_NR(RT_EVENT_HANDLE_INVALID);
    gpHal_RtEventsFree(eventHandle);
}

rt_event_handle_t gpHal_RtEventCoexExtAllocate(void)
{
    rt_event_handle_t eventHandle;
    gpHal_RtEvent_t rtEvent;

    MEMSET(&rtEvent, 0, sizeof(rtEvent));
    eventHandle = gpHal_RtEventsAllocate();
    GP_ASSERT_DEV_EXT(eventHandle != RT_EVENT_HANDLE_INVALID);
    GP_WB_WRITE_RT_ZB_COEX_PACKET_RX_EXT_EV_NR(eventHandle);
    // use the untimed type to ensure RT does not schedule the event yet
    rtEvent.type = GP_WB_ENUM_RT_STATE_ZB_UNTIMED_EV;

    gpHal_BleStartEvent(eventHandle, GP_HAL_RT_TIMESTAMP_NOT_IMPORTANT, &rtEvent);
    return eventHandle;
}

void gpHal_RtEventCoexExtRelease(rt_event_handle_t eventHandle)
{
    GP_ASSERT_DEV_EXT(eventHandle == GP_WB_READ_RT_ZB_COEX_PACKET_RX_EXT_EV_NR());

    gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_STOP_EVENT, sizeof(rt_event_handle_t), &eventHandle);
    GP_WB_WRITE_RT_ZB_COEX_PACKET_RX_EXT_EV_NR(RT_EVENT_HANDLE_INVALID);
    gpHal_RtEventsFree(eventHandle);
}
