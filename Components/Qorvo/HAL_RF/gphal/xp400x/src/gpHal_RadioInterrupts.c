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
#include "gpHal_RadioInterruptsMaskChecks.h"
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

static UInt32 gpHal_RadioInterruptsGetHpMask(void);
static UInt32 gpHal_RadioInterruptsGetLpMask(void);
static void gpHal_RadioInterruptsSetFullMask(void);
static UInt32 gpHal_RadioInterruptsGetSelectedIpcAndRciUnmasked(void);
static void gpHal_RadioInterruptsProcessIpcInterrupt(void);

#ifdef GP_COMP_GPHAL_RADIO
static void gpHal_RadioInterruptsHandleRtError(void);
static void gpHal_RadioInterruptsHandlePending(void);
static void gpHal_FlushRtLogging(void);
#endif //GP_COMP_GPHAL_RADIO

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

UInt32 gpHal_RadioInterruptsGetHpMask(void)
{
    UInt32 mask = 0;

#ifdef GP_COMP_GPHAL_BLE
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_CONN_REQ_TX_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_CONN_REQ_IND_INTERRUPT_MASK;
#endif //GP_COMP_GPHAL_BLE

    return mask;
}

UInt32 gpHal_RadioInterruptsGetLpMask(void)
{
    UInt32 mask = 0;

#ifdef GP_COMP_GPHAL_BLE
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_DATA_IND_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_ADV_IND_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_DATA_CNF_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_EVENT_PROCESSED_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_LAST_CONN_EVENT_CNT_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_SCAN_REQ_RX_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_STAT_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_UNEXPECTED_COND_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_ERROR_INTERRUPT_MASK;
#endif //GP_COMP_GPHAL_BLE

#ifdef GP_COMP_GPHAL_MAC
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_IND_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_0_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_1_INTERRUPT_MASK;
    mask |= GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_2_INTERRUPT_MASK;
#endif // GP_COMP_GPHAL_MAC

    return mask;
}

UInt32 gpHal_RadioInterruptsGetFullMask(void)
{
    return (gpHal_RadioInterruptsGetHpMask() | gpHal_RadioInterruptsGetLpMask());
}

void gpHal_RadioInterruptsSetFullMask(void)
{
    GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_INTERRUPTS(gpHal_RadioInterruptsGetFullMask());
}

UInt32 gpHal_RadioInterruptsGetSelectedIpcAndRciUnmasked(void)
{
    UInt32 unmaskedInterrupts;

    unmaskedInterrupts = GP_WB_READ_AK_IPC_UNMASKED_RT2X_INTERRUPTS_0()
                         /* retain only the common (lower) bits, remove bit 14,15 which are not defined */
                         & GP_WB_AK_IPC_UNMASKED_RT2X_INTERRUPTS_0_MASK
                         /* remove bit 16-31 (event processed interrupts mask) */
                         & (~GP_WB_AK_IPC_UNMASKED_RT2X_EVENT_PROCESSED_INTERRUPTS_MASK);

    if(GP_WB_READ_AK_IPC_UNMASKED_RT2X_EVENT_PROCESSED_INTERRUPTS() != 0)
    {
        BIT_SET(unmaskedInterrupts, GP_WB_AK_INT_CTRL_MASK_IPCRT2X_EVENT_PROCESSED_INTERRUPT_LSB);
    }
    if(GP_WB_READ_AK_IPC_UNMASKED_RT2X_LAST_CONN_EVENT_CNT_INTERRUPTS() != 0)
    {
        BIT_SET(unmaskedInterrupts, GP_WB_AK_INT_CTRL_MASK_IPCRT2X_LAST_CONN_EVENT_CNT_INTERRUPT_LSB);
    }
    if(GP_WB_READ_AK_IPC_UNMASKED_RT2X_EVENT_SKIPPED_INTERRUPTS() != 0)
    {
        BIT_SET(unmaskedInterrupts, GP_WB_AK_INT_CTRL_MASK_IPCRT2X_EVENT_SKIPPED_INTERRUPT_LSB);
    }

    unmaskedInterrupts |= (GP_WB_READ_AK_IPC_UNMASKED_RT2X_RCI_INTERRUPTS() << GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_IND_INTERRUPT_LSB);

    if(GP_WB_READ_AK_IPC_UNMASKED_RT2X_RX_PTR_LIST_EMPTY_INTERRUPT())
    {
         BIT_SET(unmaskedInterrupts, GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RX_PTR_LIST_EMPTY_INTERRUPT_LSB);
    }

    return unmaskedInterrupts & gpHal_RadioInterruptsGetFullMask();
}

void gpHal_RadioInterruptsProcessIpcInterrupt(void)
{
#ifdef GP_COMP_GPHAL_BLE
    UInt32 hpMask = gpHal_RadioInterruptsGetHpMask();
    UInt32 hpInterrupts = GP_WB_READ_AK_INT_CTRL_MASKED_IPCRT2X_INTERRUPTS() & hpMask;

    if(hpInterrupts != 0)
    {
        // High priority interrupt: process immediately
        gpHal_IsrHighPriorityInterrupt(hpInterrupts);
    }
    else
    {
        // Low priority interrupt: keep high priority masks and process LP interrupt polled
        GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_INTERRUPTS(hpMask);
    }
#else
    GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_IPCRT2X_INTERRUPT(0);
#endif //GP_COMP_GPHAL_BLE
}

#ifdef GP_COMP_GPHAL_RADIO
void gpHal_RadioInterruptsHandleRtError(void)
{
    if(GP_WB_READ_RT_ERROR_TYPE() == GP_WB_ENUM_RT_ERROR_TYPE_ASSERT)
    {
    }
    else
    {
        // RT logs are not automatically flushed. Doing it here will ensure we see all traces from before the crash
        gpHal_FlushRtLogging();

        // This means the RT has halted unexpectedly (e.g. because of an hardfault). Assert for now
        GP_LOG_SYSTEM_PRINTF("!!! RT error: type: %u (info: %u) - M0 halted !!!", 0, GP_WB_READ_RT_ERROR_TYPE(),
                             GP_WB_READ_RT_ERROR_INFO());
        GP_ASSERT_SYSTEM(false);
    }

    GP_WB_AK_IPC_CLR_RT2X_ERROR_INTERRUPT();
}

void gpHal_RadioInterruptsHandlePending(void)
{
    UInt32 unmaskedInterrupts = gpHal_RadioInterruptsGetSelectedIpcAndRciUnmasked();

    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_ERROR_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleRtError();
    }
#ifdef GP_COMP_GPHAL_BLE
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_DATA_CNF_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleBleDataConfBle();
        hal_SupplyVoltageMonitorAnnounceTxFinished();
    }
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_DATA_IND_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleDataIndBle();
    }
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_ADV_IND_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleAdvIndBle();
    }
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_EVENT_PROCESSED_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleEventProcessed();
    }
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_LAST_CONN_EVENT_CNT_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleLastEvent();
    }
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_STAT_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleStat();
    }
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_SCAN_REQ_RX_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleScanReqRx();
    }
#endif // GP_COMP_GPHAL_BLE

#ifdef GP_COMP_GPHAL_MAC
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_0_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleDataConf0Zb();
    }
    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_1_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleDataConf1Zb();
    }

    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_IND_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleDataIndZb();
    }

    if(unmaskedInterrupts & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_2_INTERRUPT_MASK)
    {
        gpHal_RadioInterruptsHandleTimedRxDoneZb();
    }

#endif // GP_COMP_GPHAL_MAC
}
#endif //GP_COMP_GPHAL_RADIO

#ifdef GP_COMP_GPHAL_RADIO
extern Bool gpHal_RtLog_PrintLogs(void);
#endif //GP_COMP_GPHAL_RADIO
void gpHal_FlushRtLogging(void)
{
}

/*****************************************************************************
 *                    Helper Function Definitions
 *****************************************************************************/

UInt8 gpHal_RadioInterruptsGetPbmFromQueueHead(UInt32 queuePtr)
{
    UInt32 formatPtr = gpHal_RtRciPop(queuePtr);

    return gpHal_findPbmHandle(formatPtr);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_RadioInterruptsInit(void)
{
    // Enable the individual (IPC) masks
    gpHal_RadioInterruptsSetFullMask();

    // Enable the masks on block-level
    GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_STBC_INTERRUPT(1);
    GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_IPCRT2X_INTERRUPT(1);
#ifdef GP_COMP_GPHAL_MAC
    GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_PHY_INTERRUPT(1);
#endif // GP_COMP_GPHAL_MAC

    // Enable global interrupts and make sure we can see them in NRT
    GP_WB_WRITE_AK_INT_CTRL_GLOBAL_INTERRUPTS_ENABLE(1);
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_AKUMA_INTERRUPT(1);
    // This is an extra mask level - needs to be set in order to forward all (currently only one) interrupts from Akuma
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_AKUMA_MAIN_INTERRUPT(1);
    NVIC_EnableIRQ((IRQn_Type)AKUMA_IRQn);
    NVIC_SetPriority((IRQn_Type)AKUMA_IRQn, 2);
}

Bool gpHal_RadioInterruptsIsPending(void)
{
    Bool pending;

    pending = GP_WB_READ_AK_INT_CTRL_UNMASKED_STBC_INTERRUPT() || GP_WB_READ_AK_INT_CTRL_UNMASKED_PHY_INTERRUPT();

    pending |= (gpHal_RadioInterruptsGetSelectedIpcAndRciUnmasked() != 0);

    return pending;
}

void gpHal_Interrupt(void)
{
#ifdef GP_COMP_GPHAL_RADIO
    while (gpHal_RadioInterruptsGetSelectedIpcAndRciUnmasked())
    {
        gpHal_RadioInterruptsHandlePending();
    }
#endif // GP_COMP_GPHAL_RADIO
    GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_PHY_INTERRUPT(1);
    GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_IPCRT2X_INTERRUPT(1);
}

void gpHal_RadioInterruptsRestoreMasksAfterHandling(void)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_AKUMA_INTERRUPT(1);

    // Needs to be enabled when HP interrupts are added
    gpHal_RadioInterruptsSetFullMask();
}

#ifdef GP_DIVERSITY_DEVELOPMENT
Bool gpHal_RadioInterruptsCheckConfig(void)
{
    UInt32 maskHp = gpHal_RadioInterruptsGetHpMask();
    UInt32 maskLp = gpHal_RadioInterruptsGetLpMask();

    if((maskHp & maskLp) != 0)
    {
        // A certain interrupt shall only be present in high or low priority mask
        GP_LOG_SYSTEM_PRINTF("Error: HP (0x%lx) and LP mask (0x%lx) overlap",0, (long unsigned int)maskHp, (long unsigned int)maskLp);
        return false;
    }

#ifdef GP_COMP_GPHAL_BLE
    if( (maskLp & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_DATA_IND_INTERRUPT_MASK) == 0 ||
        (maskLp & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_DATA_CNF_INTERRUPT_MASK) == 0 ||
        (maskLp & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_BLE_ADV_IND_INTERRUPT_MASK)  == 0)
    {
        GP_LOG_SYSTEM_PRINTF("Error: LP mask (0x%lx) does not contain required BLE interrupts",0, (long unsigned int)maskLp);
        return false;
    }
#endif //GP_COMP_GPHAL_BLE

#ifdef GP_COMP_GPHAL_MAC
    if( (maskLp & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_IND_INTERRUPT_MASK) == 0 ||
        (maskLp & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_0_INTERRUPT_MASK) == 0 ||
        (maskLp & GP_WB_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_1_INTERRUPT_MASK)  == 0)
    {
        GP_LOG_SYSTEM_PRINTF("Error: LP mask (0x%lx) does not contain required MAC interrupts",0, (long unsigned int)maskLp);
        return false;
    }
#endif //GP_COMP_GPHAL_MAC

    return true;
}
#endif //GP_DIVERSITY_DEVELOPMENT

/*****************************************************************************
 *                    ISR Function Definitions
 *****************************************************************************/

// This function is called directly from the ISR handler
void akuma_handler_impl(void)
{
    if(GP_WB_READ_AK_INT_CTRL_MASKED_EXT_IPCRT2X_INTERRUPT())
    {
        // Extra indirection needed here - depending on inclusion of BLE (high-priority interrupts) or not
        gpHal_RadioInterruptsProcessIpcInterrupt();
    }

    // Handling for these interrupts still needs to be implemented - but disable the masks to avoid re-entrance.
    if(GP_WB_READ_AK_INT_CTRL_MASKED_EXT_STBC_INTERRUPT())
    {
        GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_STBC_INTERRUPT(0);
    }

    if(GP_WB_READ_AK_INT_CTRL_MASKED_EXT_PHY_INTERRUPT())
    {
        GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_PHY_INTERRUPT(0);
    }

#ifdef GP_DIVERSITY_FREERTOS
    hal_NotifyRTOS();
#endif
}
