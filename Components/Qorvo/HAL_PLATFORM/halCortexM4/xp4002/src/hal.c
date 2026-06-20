/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
 *
 *   Hardware Abstraction Layer for ARM-based devices.
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

#include "hal.h"
#include "hal_interrupts.h"

#ifndef HAL_DIVERSITY_V2
#endif

#include "gpBsp.h"
#include "gpLog.h"
#include "gpHal.h"
#include "gpHal_Calibration.h"
#include "hal_defs.h"
#include "hal_timerXL.h"
#include "hal_SEMailbox.h"

#include "gpUtils.h"

#ifdef GP_DIVERSITY_FREERTOS
#include "gpSched.h"
#endif // GP_DIVERSITY_FREERTOS

#if defined(GP_DIVERSITY_FREERTOS) && defined(GP_COMP_GPHAL_BLE)
#include "hal_BleFreeRTOS.h"
#endif //GP_DIVERSITY_FREERTOS

#include "gpAppUcSubsystem_Calibrations.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

volatile Bool hal_PolledInterruptPending = false;

#ifdef HAL_DIVERSITY_TIMER_32US
static UInt8 hal_OverflowEventId = GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID;
static UInt8 hal_OverflowCounts = 0; // Only 5-bit needed for 32us resolution
#endif                               // HAL_DIVERSITY_TIMER_32US

/*****************************************************************************
 *                    static Function Definitions
 *****************************************************************************/

#ifdef HAL_DIVERSITY_TIMER_32US
static void hal_ConfigureOverflowEvent(void);
#endif // HAL_DIVERSITY_TIMER_32US

/*****************************************************************************
 *                    Linkerscript Symbols Declarations
 *****************************************************************************/
#if defined(__GNUC__)
extern unsigned long _estack;
extern unsigned long _sstack;
#endif
#if defined(__IAR_SYSTEMS_ICC__)
#pragma language = extended
#pragma segment = "CSTACK"
#endif

volatile UInt8 l_n_atomic;
static volatile UInt32 basepri_previous;

void hal__AtomicOn (void)
{
    /* global interrupt disable */
    __disable_irq();
    if(!l_n_atomic)
    {
        basepri_previous = __get_BASEPRI();
        __set_BASEPRI(1 << (8 - __NVIC_PRIO_BITS)); // Only the BOD has prio 1
    }
    l_n_atomic++;
    /* most interrupts are disabled now, except a few */
    __enable_irq();
}
void hal__AtomicOff (void)
{
    /* re-enable interrupts */
    /* disable interrupts just in case */
    __disable_irq();
    if (l_n_atomic > 1)
    {
        --l_n_atomic;
        __enable_irq();
    }
    else
    {
        l_n_atomic = 0;
        __set_BASEPRI(basepri_previous); // turn of priority-based masking
        __enable_irq();
    }
}

Bool hal_HandleRadioInterrupt(Bool execute)
{
    Bool pending = false;

    HAL_DISABLE_GLOBAL_INT();

    pending |= hal_PlatformInterruptIsPending();

#ifdef GP_COMP_GPHAL_RADIO
    /* Check if a (radio related) interrupt occured */
    pending |= gpHal_RadioInterruptsIsPending();
#endif //GP_COMP_GPHAL_RADIO

    HAL_ENABLE_GLOBAL_INT();

    if (pending && execute)
    {
#ifdef GP_COMP_GPHAL_RADIO
        gpHal_Interrupt();
#endif //def GP_COMP_GPHAL_RADIO
        hal_HandlePolledPlatformInterrupt();

        HAL_DISABLE_GLOBAL_INT();

        hal_PlatformInterruptsRestoreMasksAfterHandling();

#ifdef GP_COMP_GPHAL_RADIO
        gpHal_RadioInterruptsRestoreMasksAfterHandling();
#endif //GP_COMP_GPHAL_RADIO

        // All polled interrupts are handled, clear flag to avoid extra wakeup interation
        hal_ClrUnpolledInterruptBeforeSleep();

        HAL_ENABLE_GLOBAL_INT();
    }

#ifndef GP_DIVERSITY_FREERTOS
    // Handle periodic calibrations
    if(execute && (hal_HasSysTickInterruptPending() || gpHal_CalibrationGetFirstAfterWakeup()))
    {
        gpHal_CalibrationHandleTasks();
        hal_ClrSysTickInterruptPending();
    }
#endif // GP_DIVERSITY_FREERTOS
    return pending;
}

/*****************************************************************************
 *                    External Function Declarations
 *****************************************************************************/

/*****************************************************************************
 *                    Init
 *****************************************************************************/

void hal_Init(void)
{
   //Clr for debugging purposes
    GP_WB_STANDBY_CLR_PORD_INTERRUPT();

    //Active interrupt triggers every wakeup - unused for now, disable
    GP_WB_WRITE_INT_CTRL_MASK_STBC_ACTIVE_INTERRUPT(0);

    // Enable interrupt when vddb < 1.8V (clear any pending interrupts first)
    GP_WB_STANDBY_CLR_VLT_STATUS_INTERRUPT();
    GP_WB_WRITE_INT_CTRL_MASK_STBC_VLT_STATUS_INTERRUPT(1);

    HAL_LED_INIT();
    HAL_BTN_INIT();

    //Call macro directly - avoid inclusion of gphal related material in gpBsp.c
    GP_BSP_GENERIC_INIT();

    //Initialize unused pins
    GP_BSP_UNUSED_INIT();

    //clear retention for unused blocks.
    GP_BSP_SLEEPMODENOTRETAINEDLIST_INIT();


#ifdef GP_HALCORTEXM4_DISABLE_JTAG_IN_DEBUG
    /* Disable JTAG in debug mode to enable GPIO8 and GPIO9 as generale purpose IO. */
    /* Disable TDI signal mapping using BSP call*/
    GP_BSP_DEBUG_SWJDP_TDI_DEINIT();
    /* Disable SWV_TDO signal mapping using BSP call */
    GP_BSP_DEBUG_SWJDP_SWV_TDO_DEINIT();
#endif // GP_HALCORTEXM4_DISABLE_JTAG_IN_DEBUG

    //Get and check HW version
    hal_InitVersionInfo();


#ifndef HAL_DIVERSITY_V2
    halTimerXL_Init();
#endif /* HAL_DIVERSITY_V2 */

    // Init event allocation
    gpHal_InitEs();

#if defined(HAL_DIVERSITY_PUF)
    hal_InitPUF();
#endif //defined(HAL_DIVERSITY_PUF)

#ifndef GP_DIVERSITY_NO_SE
    hal_InitSleep();
#endif // GP_DIVERSITY_NO_SE

#if defined(GP_DIVERSITY_FREERTOS) && defined(GP_FREERTOS_DIVERSITY_SLEEP)
    hal_InitSleepFreeRTOS();
#endif //GP_DIVERSITY_FREERTOS && GP_FREERTOS_DIVERSITY_SLEEP


#ifndef HAL_DIVERSITY_V2
#if defined(HAL_DIVERSITY_UART)
    hal_InitUart();
#endif
#endif /* HAL_DIVERSITY_V2 */

    hal_WatchdogInit();

#ifdef HAL_DIVERSITY_TIMER_32US
    hal_ConfigureOverflowEvent();
#endif


#if defined(GP_DIVERSITY_FREERTOS) && defined(GP_COMP_GPHAL_BLE)
    hal_BleTaskCreate();
#endif //GP_DIVERSITY_FREERTOS && GP_COMP_GPHAL_BLE
}


/*****************************************************************************
 *                    STACK
 *****************************************************************************/

UInt8* hal_GetStackStartAddress(void)
{
#if defined(__GNUC__)
    return (UInt8*)(&_estack);
#endif
#if defined(__IAR_SYSTEMS_ICC__)
    return (UInt8*) (__sfe( "CSTACK" ));
#endif
}

UInt8* hal_GetStackEndAddress(void)
{
#if defined(__GNUC__)
    return (UInt8*)(&_sstack);
#endif
#if defined(__IAR_SYSTEMS_ICC__)
    return (UInt8*) (__sfb( "CSTACK" ));
#endif
}

/*****************************************************************************
 *                    RESET
 *****************************************************************************/

void hal_Reset(void)
{
    hal_SEMailbox_ResetRequest(GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP);
    while(1)
        ;
}

hal_WakeupReason_t hal_GetWakeupReason(void)
{
    UInt8 reason = GP_WB_READ_PMUD_WKUP_REASON();

    switch(reason)
    {
        case GP_WB_ENUM_WKUP_REASON_POR:
            return hal_WakeupReason_Reset;
        case GP_WB_ENUM_WKUP_REASON_TIMER_EVENT:
            return hal_WakeupReason_Timer;
        case GP_WB_ENUM_WKUP_REASON_LPCOMP_EVENT:
            return hal_WakeupReason_LpComp;
        case GP_WB_ENUM_WKUP_REASON_GPIO_EVENT:
            return hal_WakeupReason_Gpio;
        default:
            // Not specified separately:
            // GP_WB_ENUM_WKUP_REASON_IMMEDIATE_EVENT:
            // GP_WB_ENUM_WKUP_REASON_WAKEUP_ON_RF_EVENT:
            return hal_WakeupReason_Unspecified;
    }
}

hal_ResetReason_t hal_GetResetReason(void)
{
    UInt8 reason = GP_WB_READ_PMUD_POR_REASON();

    switch (reason)
    {
        case GP_WB_ENUM_POR_REASON_HW_POR:
            return hal_ResetReason_HWPor;
        case GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP:
            return hal_ResetReason_SWPor;
        case GP_WB_ENUM_POR_REASON_SOFT_POR_BY_WATCHDOG:
            return hal_ResetReason_Watchdog;
        case GP_WB_ENUM_POR_REASON_POR_BY_VDDB_CUTOFF:
            return hal_ResetReason_BrownOut;
        default:
            return hal_ResetReason_Unspecified;
    }
}

/*****************************************************************************
 *                    SysTick
 *****************************************************************************/
#ifndef GP_DIVERSITY_FREERTOS
Bool sysTickInitialized = false;
void hal_EnableSysTick(UInt32 ticks)
{
    GP_ASSERT_SYSTEM(sysTickInitialized == false);
    /* Disable SysTick */
    SysTick->CTRL = 0;
    /* Set reload value */
    SysTick->LOAD = ticks-1;
    /* Enable SysTick interrupts and set to lower priority */
    NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    NVIC_EnableIRQ(SysTick_IRQn);
    /* Reset the SysTick counter */
    SysTick->VAL = 0;
    /* select processor clock */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    /* Enable SysTick interrupt */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    /* Enable SysTick */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    sysTickInitialized = true;
}
#endif

/*****************************************************************************
 *                    Timer
 *****************************************************************************/

#ifdef HAL_DIVERSITY_TIMER_32US
UInt32 hal_TimerGetCurrentTime_32us(void)
{
    UInt32 timeInUs;

    HAL_TIMER_GET_CURRENT_TIME_1US(timeInUs);

    return (((UInt32)hal_OverflowCounts) << 27) + (timeInUs >> 5);
}

/**
 * Timer overflow callback
 */
void hal_cbTimerOverflow(void)
{
    // Called on half of time 32-bit us timebase
    UInt8 control = 0x0;
    UInt32 currentTimeUs;

    // Check in which half of the timeline we are
    gpHal_GetTime(&currentTimeUs);
    if(currentTimeUs < 0xFFFFFFFF / 2)
    {
        // Passsing 0 overflow
        HAL_DISABLE_GLOBAL_INT();
        // Increment overflow count
        hal_OverflowCounts++;
        HAL_ENABLE_GLOBAL_INT();

        currentTimeUs = 0xFFFFFFFF / 2;
    }
    else
    {
        // Passing half time base overflow
        currentTimeUs = 0x0;
    }

    GP_ES_SET_EVENT_RESULT(control, gpHal_EventResultInvalid);
    GP_ES_SET_EVENT_STATE(control, gpHal_EventStateScheduled);

    gpHal_RefreshAbsoluteEvent(hal_OverflowEventId, currentTimeUs, control);
}

/**
 * Configure Absolute HW event to track main counter overflow
 */
void hal_ConfigureOverflowEvent(void)
{
    gpHal_AbsoluteEventDescriptor_t ev;

    MEMSET(&ev, 0, sizeof(ev));

    if(hal_OverflowEventId == GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID) // Allow re-config
    {
        hal_OverflowEventId = gpHal_GetAbsoluteEvent();
    }
    GP_ASSERT_DEV_EXT(GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID != hal_OverflowEventId);

    ev.type = GP_WB_ENUM_EVENT_TYPE_DUMMY;
    ev.executionOptions = GP_ES_EXECUTION_OPTIONS_EXECUTE_IF_TOO_LATE;
    ev.interruptOptions = GP_ES_INTERRUPT_OPTIONS_MASK;
    gpHal_GetTime(&ev.exTime);
    ev.exTime = ev.exTime < 0xFFFFFFFF / 2 ? 0xFFFFFFFF / 2 : 0x0;
    ev.recAmount = 0xFFFF;         // Infinite
    ev.recPeriod = 0xFFFFFFFF / 2; // Trigger every half period to avoid scheduling in the past
    GP_ES_SET_EVENT_STATE(ev.control, gpHal_EventStateScheduled);

    gpHal_ScheduleAbsoluteEvent(&ev, hal_OverflowEventId);
    gpHal_RegisterAbsoluteEventCallback(hal_cbTimerOverflow, hal_OverflowEventId);
    gpHal_EnableAbsoluteEventCallbackInterrupt(hal_OverflowEventId, true);
}
#endif // HAL_DIVERSITY_TIMER_32US

/*****************************************************************************
 *                    ISRs
 *****************************************************************************/

void hal_NotifyRTOS(void)
{
#ifdef GP_DIVERSITY_FREERTOS
    gpSched_NotifySchedTask();
#endif
}

/*
 * hal_IntHandlerPrologue() is the first code executed after wake up, as we always wake up on a interrupt
 */
void hal_IntHandlerPrologue(void)
{
    // SetUnPolled is needed in case the interrupt is enabled, and some part of it is
    // handled in a polled fashion in the main loop of the scheduler.
    // We need this to avoid race conditions when we would go in sleep, while some (like ES) interrupt was disabled
    // because the interrupt handler was called (which disables itself), but the polled processing is not done yet.
    // => Needed for ES interrupt handler
    hal_SetUnpolledInterruptBeforeSleep();
}

/*
 * hal_IntHandlerEpilogue() should be called at the end of each interrupt handler
 */
void hal_IntHandlerEpilogue(void)
{
}

#if defined(GP_COMP_GPHAL_ES_ABS_EVENT)
void es_handler_impl(void)
{
    /* Disable ES interrupt - since it is a single store, it is atomic */
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_ES_INTERRUPT(0);
    hal_NotifyRTOS();
}
#endif

#ifdef HAL_DIVERSITY_V2
/* Measure chip temperature, Dummy for freeRtos */
Q8_8 halADC_MeasureTemperature(void)
{
    return false;
}
#endif
