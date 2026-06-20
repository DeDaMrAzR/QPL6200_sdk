/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_RadioInterrupts_DEFS.h"

#if defined(GP_DIVERSITY_FREERTOS)
#include "hal_BleFreeRTOS.h"
#endif //(GP_DIVERSITY_FREERTOS)
#include "hal_SupplyVoltageMonitor.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_RadioInterruptsHandleBleDataConfBle(void)
{
    gpHal_cbBleDataConfirm(gpHal_RadioInterruptsGetPbmFromQueueHead(GP_WB_RT_RCI_BLE_DATA_CNF_HEAD_PTR_ADDRESS));
}

void gpHal_RadioInterruptsHandleDataIndBle(void)
{
    gpHal_cbBleDataIndication(gpHal_RadioInterruptsGetPbmFromQueueHead(GP_WB_RT_RCI_BLE_DATA_IND_HEAD_PTR_ADDRESS));
}

Bool gpHal_RadioInterruptsHandleAdvIndBle(void)
{
    UInt16 unmaskedRci = GP_WB_READ_AK_IPC_UNMASKED_RT2X_RCI_INTERRUPTS();

    if(unmaskedRci & GP_WB_AK_IPC_UNMASKED_RT2X_RCI_BLE_ADV_IND_INTERRUPT_MASK)
    {
        gpHal_cbBleAdvertisingIndication(gpHal_RadioInterruptsGetPbmFromQueueHead(GP_WB_RT_RCI_BLE_ADV_IND_HEAD_PTR_ADDRESS));
        return true;
    }

    return false;
}


void gpHal_RadioInterruptsHandleEventProcessed(void)
{
    UIntLoop i;
    UInt16 events = GP_WB_READ_AK_IPC_UNMASKED_RT2X_EVENT_PROCESSED_INTERRUPTS();

    for(i = 0; i < 16; i++)
    {
        if(BIT_TST(events,i))
        {
            //GP_LOG_SYSTEM_PRINTF("Event %i interrupt",0,i);
            GP_WB_WRITE_AK_IPC_CLR_RT2X_EVENT_PROCESSED_INTERRUPTS(1 << i);
            gpHal_cbBleEventProcessed(i);
        }
    }
}

void gpHal_RadioInterruptsHandleLastEvent(void)
{
    UIntLoop i;
    UInt16 events = GP_WB_READ_AK_IPC_UNMASKED_RT2X_LAST_CONN_EVENT_CNT_INTERRUPTS();

    for(i = 0; i < 16; i++)
    {
        if(BIT_TST(events,i))
        {
            GP_WB_WRITE_AK_IPC_CLR_RT2X_LAST_CONN_EVENT_CNT_INTERRUPTS(1 << i);
            gpHal_cbBleLastConnEventCountReached(i);
        }
    }
}

void gpHal_RadioInterruptsHandleStat(void)
{
    if(GP_WB_READ_RT_STAT_TYPE() == GP_WB_ENUM_RT_STAT_TYPE_ES_TRIGGER_TOO_LATE)
    {
        // The ble ev mgr reported an event that was handled too late
        gpHal_cbEventTooLate();
        GP_WB_AK_IPC_CLR_RT2X_STAT_INTERRUPT();
    }
}

void gpHal_RadioInterruptsHandleScanReqRx(void)
{
    GP_WB_AK_IPC_CLR_RT2X_SCAN_REQ_RX_INTERRUPT();
    gpHal_cbBleScanRequestReceived();
}


// High-priority interrupt - called from ISR directly
void gpHal_IsrHighPriorityInterrupt(UInt32 hpIntMask)
{
    if(hpIntMask & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_CONN_REQ_IND_INTERRUPT_MASK)
    {
#ifndef GP_DIVERSITY_FREERTOS
        gpHal_cbBleConnectionRequestIndication(gpHal_RadioInterruptsGetPbmFromQueueHead(GP_WB_RT_RCI_BLE_CONN_REQ_IND_HEAD_PTR_ADDRESS));
#else
        hal_BleIsrRciDefer(GP_BLE_TASK_EVENT_RCI_CONN_REQ, gpHal_RadioInterruptsGetPbmFromQueueHead(GP_WB_RT_RCI_BLE_CONN_REQ_IND_HEAD_PTR_ADDRESS));
#endif
    }

    if(hpIntMask & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_CONN_REQ_TX_INTERRUPT_MASK)
    {
        // Clear interrupt
        GP_WB_AK_IPC_CLR_RT2X_CONN_REQ_TX_INTERRUPT();
#ifndef GP_DIVERSITY_FREERTOS
        gpHal_cbBleConnectionRequestConfirm();
#else
        hal_BleIsrIPCGPMDefer();
#endif
    }
}
