/*
 * Copyright (c) 2023, Qorvo Inc
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

#ifndef _Q_REG_WATCHDOG_H_
#define _Q_REG_WATCHDOG_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Value that should be written to Key register to reset the watchdog timer. */
#define Q_REG_WATCHDOG_KEY_VALUE_RESET 0xAA

/** @brief Value that should be written to Key register to enable control change window. */
#define Q_REG_WATCHDOG_KEY_VALUE_CONTROL 0x55

/** @enum qRegWatchdog_Event_t */
//@{
/** @brief TX buffer not full. */
#define qRegWatchdog_TimerExpired GP_WB_INT_CTRL_MASK_APPUC_WATCHDOG_TIMER_EXPIRED_INTERRUPT_LSB
/** @typedef qRegWatchdog_event_t
 *  @brief Watchdog event.
 */
typedef UInt8 qRegWatchdog_Event_t;
//@}

/** @enum qRegWatchdog_Function_t */
//@{
#define qRegWatchdog_FunctionNone       GP_WB_ENUM_WATCHDOG_FUNCTION_IDLE
#define qRegWatchdog_FunctionInterrupt  GP_WB_ENUM_WATCHDOG_FUNCTION_GEN_INTERRUPT
#define qRegWatchdog_FunctionSoftPor    GP_WB_ENUM_WATCHDOG_FUNCTION_SOFT_POR
#define qRegWatchdog_FunctionResetAppuc GP_WB_ENUM_WATCHDOG_FUNCTION_RESET_APPUC
/** @typedef qRegWatchdog_Function_t
 *  @brief Watchdog function mode.
 */
typedef UInt8 qRegWatchdog_Function_t;
//@}

/** @brief Watchdog control status value. */
typedef UInt32 qRegWatchdog_Control_t;

/** @brief Set the Watchdog timeout to the control register. */
static inline void qRegWatchdog_TimeoutSet(qRegWatchdog_Control_t* const control, UInt32 timeout)
{
    *control &= ~GP_WB_WATCHDOG_TIMEOUT_MASK;
    *control |= ((timeout << GP_WB_WATCHDOG_TIMEOUT_LSB) & GP_WB_WATCHDOG_TIMEOUT_MASK);
}

/** @brief Set the enable flag to the control register. */
static inline void qRegWatchdog_EnableSet(qRegWatchdog_Control_t* const control, Bool enable)
{
    if(enable)
    {
        *control |= GP_WB_WATCHDOG_ENABLE_MASK;
    }
    else
    {
        *control &= ~GP_WB_WATCHDOG_ENABLE_MASK;
    }
}

/** @brief Set the Watchdog function to the control register. */
static inline void qRegWatchdog_FunctionSet(qRegWatchdog_Control_t* const control, qRegWatchdog_Function_t function)
{
    *control &= ~GP_WB_WATCHDOG_WATCHDOG_FUNCTION_MASK;
    *control |= ((function << GP_WB_WATCHDOG_WATCHDOG_FUNCTION_LSB) & GP_WB_WATCHDOG_WATCHDOG_FUNCTION_MASK);
}

/** @brief Set the Watchdog freeze when asleep flag to the control register. */
static inline void qRegWatchdog_FreezeWhenAsleepEnableSet(qRegWatchdog_Control_t* const control, Bool enable)
{
    if(enable)
    {
        *control |= GP_WB_WATCHDOG_FREEZE_WHEN_APPUC_ASLEEP_MASK;
    }
    else
    {
        *control &= ~GP_WB_WATCHDOG_FREEZE_WHEN_APPUC_ASLEEP_MASK;
    }
}

/** @brief Set the Watchdog freeze when in deep sleep flag to the control register. */
static inline void qRegWatchdog_FreezeWhenInDeepSleepEnableSet(qRegWatchdog_Control_t* const control, Bool enable)
{
    if(enable)
    {
        *control |= GP_WB_WATCHDOG_FREEZE_WHEN_APPUC_INDEEPSLEEP_MASK;
    }
    else
    {
        *control &= ~GP_WB_WATCHDOG_FREEZE_WHEN_APPUC_INDEEPSLEEP_MASK;
    }
}

/** @brief Set the Watchdog freeze when breakpoint flag to the control register. */
static inline void qRegWatchdog_FreezeWhenBreakPointEnableSet(qRegWatchdog_Control_t* const control, Bool enable)
{
    if(enable)
    {
        *control |= GP_WB_WATCHDOG_FREEZE_WHEN_APPUC_BREAK_POINT_MASK;
    }
    else
    {
        *control &= ~GP_WB_WATCHDOG_FREEZE_WHEN_APPUC_BREAK_POINT_MASK;
    }
}

/** @brief Set the Watchdog control. */
static inline void qRegWatchdog_ControlSet(qRegWatchdog_Control_t control)
{
    GP_WB_WRITE_U32(GP_WB_WATCHDOG_CONTROL_ADDRESS, ((UInt32)control & GP_WB_WATCHDOG_CONTROL_MASK));
}

/** @brief Get the Watchdog enable status. */
static inline Bool qRegWatchdog_EnabledGet(void)
{
    return (GP_WB_READ_U32(GP_WB_WATCHDOG_CONTROL_ADDRESS) & GP_WB_WATCHDOG_ENABLE_MASK) ? true : false;
}

/** @brief Set the Watchdog key. */
static inline void qRegWatchdog_KeySet(UInt32 key)
{
    GP_WB_WRITE_U32(GP_WB_WATCHDOG_KEY_ADDRESS, ((key << GP_WB_WATCHDOG_KEY_LSB) & GP_WB_WATCHDOG_KEY_MASK));
}

/** @brief Clear the Watchdog timer expired interrupt. */
static inline void qRegWatchdog_IrqTimerExpiredClear(void)
{
    GP_WB_WATCHDOG_CLR_TIMER_EXPIRED_INTERRUPT();
}

/** @brief Check whether the Watchdog is able to write control register. */
static inline Bool qRegWatchdog_WindowOngoingCheck(void)
{
    return ((GP_WB_READ_U32(GP_WB_WATCHDOG_CONTROL_CHANGE_WINDOW_ONGOING_ADDRESS) &
             GP_WB_WATCHDOG_CONTROL_CHANGE_WINDOW_ONGOING_MASK) >>
            GP_WB_WATCHDOG_CONTROL_CHANGE_WINDOW_ONGOING_LSB);
}

/** @brief Get the Watchdog remaining time. */
static inline UInt32 qRegWatchdog_CurrentTimeGet(void)
{
    return ((GP_WB_READ_U32(GP_WB_WATCHDOG_CURRENT_TIME_ADDRESS) & GP_WB_WATCHDOG_CURRENT_TIME_MASK) >>
            GP_WB_WATCHDOG_CURRENT_TIME_LSB);
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_WATCHDOG_H_ */
