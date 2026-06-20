/*
 * Copyright (c) 2016, GreenPeak Technologies
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

#include "global.h"
#include "hal.h"
#include "hal_defs.h"
#include "gpHal.h"
#include "gpHal_ES.h"
#include "gpAssert.h"
#include "esec_qorvo.h"
#include "gpLog.h"

#ifdef GP_DIVERSITY_DEVELOPMENT
#include "gpStat.h"
#endif

void gpHal_cbEventTooLate(void);

/** @brief Helper function to restore the watchdog configuration after a sleep,
 *         as it is not restored by the hardware retain mechanism.
 */
void hal_WatchdogRestore(void);

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

#if defined(__GNUC__)
#define HAL_APPUCRAM_RETAIN_SIZE  ((UInt32)&__appuc_ram_retain_length)
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#error "IAR compiler is not supported"
#endif

#if defined(__SES_ARM)
#error "Segger compiler is not supported"
#endif

// Amount of RAM bytes (UC and SYS) that should be used for CRC calculation
// Total amount = 2^(HAL_CRC_MODE_CONFIG_RETENTION_SIZE_POWER + 1) bytes
#define HAL_CRC_MODE_CONFIG_RETENTION_SIZE_POWER 5

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    UInt32 disableCounter;
    UInt32 threshold;
} hal_SleepControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// event used for (timed) wake-up
static gpHal_AbsoluteEventId_t hal_wakeUpEventId;

// Indication whether the ARM is allowed to go to sleep (can be prevented by an interrupt that is not handled).
Bool hal_maySleep;

#if defined(HAL_DIVERSITY_SLEEP)
/** @brief Sleep mode control block */
static hal_SleepControlBlock_t hal_SleepControlBlock = {
    .disableCounter = 0,
    .threshold = HAL_DEFAULT_GOTOSLEEP_THRES,
};
#endif

static UInt32 hal_sleepCount;
static UInt32 hal_last_configured_wakeup_time = 0;

/*****************************************************************************
 *                    Static Function Declarations
 *****************************************************************************/

static void hal_ConfigureWakeUpEvent(void);
static void hal_RescheduleWakeUpEvent(UInt32 sleepTime);
static void hal_ConfigureRetention(void);
#ifdef GP_DIVERSITY_DEVELOPMENT
static Bool hal_CheckExternalEventConfigured(void);
#endif

/*****************************************************************************
 *                    Linkerscript Symbols Declarations
 *****************************************************************************/

#if defined(__GNUC__)
extern const UInt32 __attribute__((weak)) __appuc_ram_retain_length;
#endif

extern const UIntPtr sw_retention_begin;
#define HAL_SW_RETENTION_BEGIN ((UIntPtr)&sw_retention_begin)

/*****************************************************************************
 *                    External Function Declarations
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void hal_ConfigureWakeUpEvent(void)
{
    gpHal_AbsoluteEventDescriptor_t ev;

    // Need to wake up in 'sleeptime' * 32us. Wakeup event must be scheduled.
    MEMSET(&ev, 0, sizeof(ev));

    // Allocate event id, but never free it.
    // We keep the event info and only update execution time (performance optimization)
    hal_wakeUpEventId = gpHal_GetAbsoluteEvent();
    GP_ASSERT_DEV_EXT(GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID != hal_wakeUpEventId);

    ev.type = GP_WB_ENUM_EVENT_TYPE_DUMMY;
    /* we need ex_itl when waking up very fast */
    ev.executionOptions = GP_ES_EXECUTION_OPTIONS_EXECUTE_IF_TOO_LATE;
    ev.interruptOptions = GP_ES_INTERRUPT_OPTIONS_MASK;

    if(HAL_BSP_IO_ACTIVITY_PENDING())
    {
        ev.executionOptions |= GP_ES_EXECUTION_OPTIONS_PROHIBIT_STANDBY;
    }

    gpHal_GetTime(&ev.exTime);

    ev.exTime += HAL_SLEEP_MAX_SLEEP_TIME;

    GP_ES_SET_EVENT_STATE(ev.control, gpHal_EventStateScheduled);
    gpHal_ScheduleAbsoluteEvent(&ev, hal_wakeUpEventId);
    gpHal_UnscheduleAbsoluteEvent(hal_wakeUpEventId);
    gpHal_EnableAbsoluteEventCallbackInterrupt(hal_wakeUpEventId, true);
}

void hal_RescheduleWakeUpEvent(UInt32 sleepTime)
{
    UInt32 wakeUpTs;
    UInt8 wakeUpControl = 0;

    gpHal_GetTime(&wakeUpTs);

    if(sleepTime == HAL_SLEEP_INDEFINITE_SLEEP_TIME)
    {
        wakeUpTs +=
            HAL_SLEEP_MAX_SLEEP_TIME; /* wake anyhow to be sure we increment the overflow counter after an overflow */
    }
    else
    {
        wakeUpTs += sleepTime;
    }

    GP_ES_SET_EVENT_RESULT(wakeUpControl, gpHal_EventResultInvalid);
    GP_ES_SET_EVENT_STATE(wakeUpControl, gpHal_EventStateScheduled);

    hal_last_configured_wakeup_time = wakeUpTs;
    gpHal_RefreshAbsoluteEvent(hal_wakeUpEventId, wakeUpTs, wakeUpControl);
}

void hal_ConfigureRetention(void)
{
    // always retain the cache, and the AK RAM and it's cache
    UInt16 retain = GP_WB_PMUD_RAM_RETAIN_APPUC_CACHE_MASK;

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
    retain |= GP_WB_PMUD_RAM_RETAIN_AK_MASK |
              GP_WB_PMUD_RAM_RETAIN_AK_CACHE_MASK;
#endif // GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

    if(HAL_APPUCRAM_RETAIN_SIZE > 0)
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_0K_32K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (32 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_32K_64K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (64 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_64K_96K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (96 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_96K_128K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (128 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_128K_160K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (160 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_160K_192K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (192 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_192K_224K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (224 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_224K_256K_MASK;
    }
    if(HAL_APPUCRAM_RETAIN_SIZE >= (256 * 1024))
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_256K_288K_MASK;
    }

    // retain the top of stack
    UInt32 stack_retention = HAL_SP_RESERVED_SIZE + HAL_SP_BACKUP_SIZE + HAL_SP_BACKUP_MARGIN;
    /* the stack grows top down */
    if(stack_retention > 0)
    {
        retain |= GP_WB_PMUD_RAM_RETAIN_APPUC_256K_288K_MASK;
    }
    if(stack_retention >= (32 * 1024))
    {
        // we don't expect such a large stack when going to sleep.
        GP_ASSERT_DEV_INT(false);
    }

    esec_config_appuc_ram_retain(retain);
}

#ifdef GP_DIVERSITY_DEVELOPMENT
Bool hal_CheckExternalEventConfigured(void)
{
    return true;
}
#endif /* GP_DIVERSITY_DEVELOPMENT */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void hal_InitSleep(void)
{
    hal_ConfigureWakeUpEvent();
    hal_ConfigureRetention();

    hal_maySleep = true;
    hal_sleepCount = 0;

    hal_SleepSetGotoSleepThreshold(HAL_DEFAULT_GOTOSLEEP_THRES);

    // Enable the standby interrupt when going to deep sleep
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_STANDBY_INTERRUPT(1);
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_STANDBY_PREPARE_DEEPSLEEP_INTERRUPT(1);
    NVIC_EnableIRQ((IRQn_Type)STANDBY_IRQn);

    // initial setting for the low power mode.
    esec_set_power_mode(Q_POWER_MODE(GP_WB_ENUM_STANDBY_POWER_MODE_LPS, GP_WB_ENUM_ACTIVE_POWER_MODE_HPS,
                                     GP_WB_ENUM_RTC_TIMER_SRC_XT32M));
    esec_write_rc64k_use_vcore(0);
}

void hal_SleepIncrementCount(void) { hal_sleepCount++; }

UInt32 hal_SleepGetSleepCount(void) { return hal_sleepCount; }

void hal_sleep_uc(UInt32 sleeptime)
{
#ifdef GP_DIVERSITY_DEVELOPMENT
    UInt32 t1, t2;
#endif

    GP_ASSERT_DEV_EXT(l_n_atomic == 0);
    // UInt8 es_int = GP_WB_READ_INT_CTRL_MASK_APPUC_ES_INTERRUPT();

    if(sleeptime < 1000)
    {
        return;
    }

    if((sleeptime != HAL_SLEEP_INDEFINITE_SLEEP_TIME) && (sleeptime > HAL_SLEEP_MAX_SLEEP_TIME))
    {
        // ceil to maximum sleep time
        sleeptime = HAL_SLEEP_MAX_SLEEP_TIME;
    }

    /*
     * Interrupts are disabled again just before sleep instruction
     * such that all preparation steps have been done or none.
     */
    HAL_DISABLE_GLOBAL_INT();

    hal_RescheduleWakeUpEvent(sleeptime);

#ifdef GP_DIVERSITY_DEVELOPMENT
    if(sleeptime == HAL_SLEEP_INDEFINITE_SLEEP_TIME)
    {
        if(!hal_CheckExternalEventConfigured())
        {
            GP_LOG_SYSTEM_PRINTF("No Ext WKUP configured", 0);
            GP_ASSERT_SYSTEM(false);
        }
    }
#endif


#if defined(HAL_DIVERSITY_UART) && !defined(HAL_DIVERSITY_V2)
    hal_UartBeforeSleep();
#endif

    /* finally, everything is ready for a healthy sleep */
    HAL_ENABLE_GLOBAL_INT();

#ifdef GP_DIVERSITY_DEVELOPMENT
    gpHal_GetTime(&t1);
#endif

    /* Actual sleep */
    hal_sleep();

    /*
     * At this point, any interrupt is executed and IntHandlerPrologue() is executed
     * OR we did not go to sleep at all...
     */

#if defined(HAL_DIVERSITY_UART) && !defined(HAL_DIVERSITY_V2)
    hal_UartAfterSleep();
#endif // HAL_DIVERSITY_UART

    hal_WatchdogRestore();

    gpHal_UnscheduleAbsoluteEvent(hal_wakeUpEventId);

#ifdef GP_DIVERSITY_DEVELOPMENT
    gpHal_GetTime(&t2);

    if((t2 - t1) > 1000)
    {
        GP_LOG_PRINTF("slept %lu us(%lu->%lu)", 0, dist(t2, t1), t1, t2);
        GP_STAT_SAMPLE_TIME();

        if(HAL_TIME_COMPARE_LOWER_US(hal_last_configured_wakeup_time, t2))
        {
            gpHal_cbEventTooLate();
        }
    }
#endif
}

void hal_SleepSetGotoSleepEnable(Bool enable)
{
#if defined(HAL_DIVERSITY_SLEEP)

#ifndef GP_DIVERSITY_FREERTOS
    HAL_DISABLE_GLOBAL_INT();
#endif

    if (enable)
    {
        GP_ASSERT_DEV_EXT(hal_SleepControlBlock.disableCounter);
        hal_SleepControlBlock.disableCounter--;
    }
    else
    {
        hal_SleepControlBlock.disableCounter++;
    }

#ifndef GP_DIVERSITY_FREERTOS
    HAL_ENABLE_GLOBAL_INT();
#endif

#else
    NOT_USED(enable);
#endif
}

void hal_SleepSetGotoSleepThreshold(UInt32 threshold)
{
#if defined(HAL_DIVERSITY_SLEEP)
    hal_SleepControlBlock.threshold = threshold;
#else
    NOT_USED(threshold);
#endif
}

UInt32 hal_SleepGetGotoSleepThreshold(void)
{
#if defined(HAL_DIVERSITY_SLEEP)
    return hal_SleepControlBlock.threshold;
#else
    return 0;
#endif
}

Bool hal_SleepCheck(UInt32 expectedIdleTime)
{
#if defined(HAL_DIVERSITY_SLEEP)
    if(hal_SleepControlBlock.disableCounter)
    {
        return false;
    }
    if(expectedIdleTime <= hal_SleepControlBlock.threshold)
    {
        return false;
    }
    return true;
#else
    return false;
#endif
}
