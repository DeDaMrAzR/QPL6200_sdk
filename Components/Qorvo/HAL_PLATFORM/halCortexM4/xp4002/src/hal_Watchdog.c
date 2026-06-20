/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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
#include "gpLog.h"
#include "qDrvWatchdog.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define HAL_WDT_TIMEOUT 0xFFFF /*in 16us*/

#ifdef HAL_DIVERSITY_WDT_DISABLE
#define HAL_WDT_ENABLED() (0)
#else
#define HAL_WDT_ENABLED() (1)
#endif

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#ifdef HAL_DIVERSITY_WATCHDOG_TIMEOUT_INTERRUPT_ENABLED
static hal_WatchdogTimeoutCallback_t hal_WatchdogTimeoutCallback = NULL;
#endif // HAL_DIVERSITY_WATCHDOG_TIMEOUT_INTERRUPT_ENABLED

/*****************************************************************************
 *                    Static function definitions
 *****************************************************************************/

static void watchdogCallback(void* arg)
{
#ifdef HAL_DIVERSITY_WATCHDOG_TIMEOUT_INTERRUPT_ENABLED
    if(hal_WatchdogTimeoutCallback != NULL)
    {
        // There is a WDT timeout callback registered. Handling implemented inside callback
        hal_WatchdogTimeoutCallback();
        return;
    }

    // We trigger an assert here so in development builds we understand that the watchdog is triggered and it provides
    // us the needed hook for debugging why the watchdog is triggered. Note that in production/release builds this
    // assert will translate in a reset of the system.
    GP_ASSERT_SYSTEM(false);
#endif
}

static void configureWatchdog(UInt16 timeout, qDrvWatchdog_Function_t watchdogFunction, Bool freeze_when_uc_asleep,
                              Bool freeze_when_uc_break_point) /*timeout in 16us*/
{
    qDrvWatchdog_Config_t config = {
        .timeout = timeout * Q_DRV_WATCHDOG_TICK_TO_US,
        .function = watchdogFunction,
        .freezeOnSleep = freeze_when_uc_asleep,
        .freezeOnDeepSleep = false,
        .freezeOnDebug = freeze_when_uc_break_point,
    };

    qDrvWatchdog_Disable();
    qDrvWatchdog_Reconfigure(&config);
    qDrvWatchdog_Enable();
}

/*****************************************************************************
 *                    Public function definitions
 *****************************************************************************/

void hal_WatchdogRestore(void)
{
    if(HAL_WDT_ENABLED())
    {
        if(qDrvWatchdog_InitCheck())
        {
            qDrvWatchdog_Enable();
            qDrvWatchdog_Feed();
        }
    }
}

void hal_WatchdogInit(void)
{
    qDrvWatchdog_Config_t config = Q_DRV_WATCHDOG_CONFIG_DEFAULT();
    config.function = qRegWatchdog_FunctionSoftPor;

    qDrvWatchdog_Init(&config, watchdogCallback, NULL, Q_DRV_IRQ_PRIO_DEFAULT);

    if(!HAL_WDT_ENABLED())
    {
        /* If watchdog needs to be disabled. */
        qDrvWatchdog_Deinit();
        return;
    }

#ifdef HAL_DIVERSITY_WATCHDOG_TIMEOUT_INTERRUPT_ENABLED
    hal_EnableWatchdogInterrupt(HAL_WDT_TIMEOUT);
#else
    hal_EnableWatchdog(HAL_WDT_TIMEOUT);
#endif
}

void hal_EnableWatchdog(UInt16 timeout) /*timeout in 16us*/
{
    if(HAL_WDT_ENABLED())
    {
        HAL_DISABLE_GLOBAL_INT();
        configureWatchdog(timeout, qDrvWatchdog_FunctionSoftPor, false, true);
        HAL_ENABLE_GLOBAL_INT();
        hal_ResetWatchdog();
    }
}

void hal_DisableWatchdog(void)
{
    if(HAL_WDT_ENABLED() && qDrvWatchdog_EnableCheck())
    {
        qDrvWatchdog_Disable();
    }
}

void hal_ResetWatchdog(void)
{
    if(HAL_WDT_ENABLED())
    {

        qDrvWatchdog_Feed();
    }
}

UInt16 hal_GetWatchdogTimeRemaining(void)
{
    return qDrvWatchdog_Read() / Q_DRV_WATCHDOG_TICK_TO_US;
}

void hal_TriggerWatchdog(void)
{
    if(qDrvWatchdog_EnableCheck())
    {
        while(1)
        {
            // Let the watchdog time out
        }
    }
}

void hal_EnableWatchdogInterrupt(UInt16 timeout) /*timeout in 16us*/
{
    if(HAL_WDT_ENABLED())
    {
        HAL_DISABLE_GLOBAL_INT();
        configureWatchdog(timeout, qDrvWatchdog_FunctionInterrupt, false, true);
        HAL_ENABLE_GLOBAL_INT();
    }
}

#ifdef HAL_DIVERSITY_WATCHDOG_TIMEOUT_INTERRUPT_ENABLED
void hal_WatchdogRegisterTimeoutCallback(hal_WatchdogTimeoutCallback_t callback)
{
    hal_WatchdogTimeoutCallback = callback;
}
#endif // HAL_DIVERSITY_WATCHDOG_TIMEOUT_INTERRUPT_ENABLED
