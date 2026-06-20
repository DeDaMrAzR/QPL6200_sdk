/*
 * Copyright (c) 2024, Qorvo Inc
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

#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qDrvWatchdog.h"

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief Watchdog instance control block */
typedef struct {
    qRegWatchdog_Control_t control;
    qDrvWatchdog_Cb_t cb;
    void* arg;
    Bool initialized;
} qDrvWatchdog_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvWatchdog_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static qResult_t qDrvWatchdog_ControlSet(qRegWatchdog_Control_t control)
{
    qRegWatchdog_KeySet(Q_REG_WATCHDOG_KEY_VALUE_CONTROL);

    UInt32 attemptsCount = 100;
    while(!qRegWatchdog_WindowOngoingCheck())
    {
        attemptsCount--;
        if(attemptsCount == 0)
        {
            return Q_ERR_TIMEOUT;
        }
    }

    qRegWatchdog_ControlSet(control);

    return Q_OK;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvWatchdog_Init(const qDrvWatchdog_Config_t* const pConfig, const qDrvWatchdog_Cb_t pCallback, void* arg,
                            UInt32 irqPriority)
{
    Q_ASSERT(pConfig);

    if((pConfig->function == qDrvWatchdog_FunctionInterrupt) && (pCallback == NULL))
    {
        return Q_ERR_INVPARAM;
    }

    if(qDrvWatchdog_InitCheck())
    {
        return Q_ERR_INVSTATE;
    }

    qRegWatchdog_TimeoutSet(&controlBlock.control, pConfig->timeout / Q_DRV_WATCHDOG_TICK_TO_US);
    qRegWatchdog_FunctionSet(&controlBlock.control, pConfig->function);
    qRegWatchdog_FreezeWhenAsleepEnableSet(&controlBlock.control, pConfig->freezeOnSleep);
    qRegWatchdog_FreezeWhenInDeepSleepEnableSet(&controlBlock.control, pConfig->freezeOnDeepSleep);
    qRegWatchdog_FreezeWhenBreakPointEnableSet(&controlBlock.control, pConfig->freezeOnDebug);

    qRegWatchdog_EnableSet(&controlBlock.control, false);

    qResult_t result = qDrvWatchdog_ControlSet(controlBlock.control);
    if(result != Q_OK)
    {
        return result;
    }

    if(pCallback)
    {
        controlBlock.arg = arg;
        controlBlock.cb = pCallback;

        qRegWatchdog_IrqTimerExpiredClear();

        /* Enable timer expired event interrupt. */
        qRegIntCtrl_EventIRQEnable(WATCHDOG_IRQn, (1UL << qDrvWatchdog_TimerExpired), 0);

        NVIC_SetPriority(WATCHDOG_IRQn, irqPriority);

        /* Enable global interrupts from the peripheral. */
        qRegIntCtrl_IRQEnable(WATCHDOG_IRQn);

        /* Enable NVIC interrupts. */
        NVIC_ClearPendingIRQ(WATCHDOG_IRQn);
        NVIC_EnableIRQ(WATCHDOG_IRQn);
    }

    controlBlock.initialized = true;

    return Q_OK;
}

qResult_t qDrvWatchdog_Deinit(void)
{
    if(!qDrvWatchdog_InitCheck())
    {
        return Q_OK;
    }

    if(qDrvWatchdog_EnableCheck())
    {
        qDrvWatchdog_Disable();
    }

    if(controlBlock.cb)
    {
        controlBlock.cb = NULL;

        /* Disable timer expired event interrupt. */
        qRegIntCtrl_EventIRQDisable(WATCHDOG_IRQn, (1UL << qDrvWatchdog_TimerExpired), 0);

        /* Disable NVIC interrupts. */
        NVIC_DisableIRQ(WATCHDOG_IRQn);

        /* Disable global interrupts from the peripheral. */
        qRegIntCtrl_IRQDisable(WATCHDOG_IRQn);
    }

    controlBlock.initialized = false;

    return Q_OK;
}

qResult_t qDrvWatchdog_Reconfigure(const qDrvWatchdog_Config_t* const pConfig)
{
    Q_ASSERT(pConfig);

    if(!qDrvWatchdog_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qDrvWatchdog_EnableCheck())
    {
        return Q_ERR_INVSTATE;
    }

    qRegWatchdog_TimeoutSet(&controlBlock.control, pConfig->timeout / Q_DRV_WATCHDOG_TICK_TO_US);
    qRegWatchdog_FunctionSet(&controlBlock.control, pConfig->function);
    qRegWatchdog_FreezeWhenAsleepEnableSet(&controlBlock.control, pConfig->freezeOnSleep);
    qRegWatchdog_FreezeWhenInDeepSleepEnableSet(&controlBlock.control, pConfig->freezeOnDeepSleep);
    qRegWatchdog_FreezeWhenBreakPointEnableSet(&controlBlock.control, pConfig->freezeOnDebug);

    return Q_OK;
}

Bool qDrvWatchdog_InitCheck(void)
{
    return controlBlock.initialized;
}

qResult_t qDrvWatchdog_Enable(void)
{
    if(!qDrvWatchdog_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegWatchdog_EnableSet(&controlBlock.control, true);

    return qDrvWatchdog_ControlSet(controlBlock.control);
}

qResult_t qDrvWatchdog_Disable(void)
{
    if(!qDrvWatchdog_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegWatchdog_EnableSet(&controlBlock.control, false);

    return qDrvWatchdog_ControlSet(controlBlock.control);
}

Bool qDrvWatchdog_EnableCheck(void)
{
    if(!qDrvWatchdog_InitCheck())
    {
        return false;
    }

    return qRegWatchdog_EnabledGet();
}

qResult_t qDrvWatchdog_Feed(void)
{
    if(!qDrvWatchdog_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegWatchdog_KeySet(Q_REG_WATCHDOG_KEY_VALUE_RESET);

    return Q_OK;
}

UInt32 qDrvWatchdog_Read(void)
{
    if(!qDrvWatchdog_InitCheck())
    {
        return 0;
    }

    return qRegWatchdog_CurrentTimeGet() * Q_DRV_WATCHDOG_TICK_TO_US;
}

void watchdog_handler_impl(void)
{
    qRegWatchdog_IrqTimerExpiredClear();

    if(controlBlock.initialized == false)
    {
        NVIC_DisableIRQ(WATCHDOG_IRQn);
        qRegIntCtrl_IRQDisable(WATCHDOG_IRQn);
        return;
    }

    if(controlBlock.cb)
    {
        controlBlock.cb(controlBlock.arg);
    }
}
