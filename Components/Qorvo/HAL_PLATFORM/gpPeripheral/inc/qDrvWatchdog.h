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

#ifndef _Q_DRV_WATCHDOG_H_
#define _Q_DRV_WATCHDOG_H_

/**
 * @defgroup Q_DRV_WATCHDOG Watchdog driver
 * @{
 *
 * @brief This is a driver for Watchdog peripheral.
 *
 * @file qDrvWatchdog.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegWatchdog.h"
#include "qDrvCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief One watchdog tick to time in microseconds. */
#define Q_DRV_WATCHDOG_TICK_TO_US 16UL

/** @enum qDrvWatchdog_Event_t */
//@{
/** @brief TX buffer not full. */
#define qDrvWatchdog_TimerExpired qRegWatchdog_TimerExpired
/** @typedef qDrvWatchdog_event_t
 *  @brief Watchdog event.
 */
typedef qRegWatchdog_Event_t qDrvWatchdog_Event_t;
//@}

/** @enum qDrvWatchdog_Function_t */
//@{
/** @brief A watchdog timeout will not trigger anything.
 *         No change in the state of the chip.
 */
#define qDrvWatchdog_FunctionNone qRegWatchdog_FunctionNone
/** @brief A watchdog timeout will generate an interrupt.
 *         No change in the state of the chip.
 */
#define qDrvWatchdog_FunctionInterrupt qRegWatchdog_FunctionInterrupt
/** @brief A watchdog timeout will trigger a complete system reset.
 *         This is equivalent to a Power-on-Reset.
 *         Everything in the chip will be in initial state.
 */
#define qDrvWatchdog_FunctionSoftPor qRegWatchdog_FunctionSoftPor
/** @brief A watchdog timeout will trigger a reset of the APPUC.
 *         All other areas of the chip remain in the same state.
 *         This is not recommended for normal usage, but may be useful
 *         in very specific use cases.
 */
#define qDrvWatchdog_FunctionResetAppuc qRegWatchdog_FunctionResetAppuc
/** @typedef qDrvWatchdog_Function_t
 *  @brief Watchdog mode.
 */
typedef qRegWatchdog_Function_t qDrvWatchdog_Function_t;
//@}

/** @brief Watchdog configuration structure. */
typedef struct {
    UInt32 timeout;                   /**< Watchdog timeout in microseconds. */
    qDrvWatchdog_Function_t function; /**< Watchdog action when triggered. */
    Bool freezeOnSleep;               /**< Freeze WDT when APPUC is in the sleep mode. */
    Bool freezeOnDeepSleep;           /**< Freeze WDT when APPUC is in the deep sleep mode. */
    Bool freezeOnDebug;               /**< Freeze WDT when APPUC hit a breakpoint. */
} qDrvWatchdog_Config_t;

/** @brief Callback triggered when watchdog is set to IRQ mode. */
typedef void (*qDrvWatchdog_Cb_t)(void* arg);

/** @brief Default configuration macro for Watchdog peripheral. */
#define Q_DRV_WATCHDOG_CONFIG_DEFAULT()                                                                                \
    {                                                                                                                  \
        .timeout = 10000,                          /* 10 milliseconds. */                                              \
            .function = qDrvWatchdog_FunctionNone, /* Do nothing. */                                                   \
            .freezeOnSleep = true,                 /* Freeze on sleep. */                                              \
            .freezeOnDeepSleep = true,             /* Freeze on deep sleep. */                                         \
            .freezeOnDebug = true,                 /* Freeze on debug. */                                              \
    }

/**
 * @brief Initialize Watchdog peripheral.
 *
 * @param[in] pConfig     Configuration structure.
 * @param[in] pCallback   Watchdog callback function.
 * @param[in] arg         Pointer to user defined argument.
 * @param[in] irqPriority ARM Interrupt priority.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral is already initialized.
 * @retval Q_ERR_INVPARAM Incorrect configuration.
 */
qResult_t qDrvWatchdog_Init(const qDrvWatchdog_Config_t* const pConfig, const qDrvWatchdog_Cb_t pCallback, void* arg,
                            UInt32 irqPriority);

/**
 * @brief Deinitialize the Watchdog instance.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvWatchdog_Deinit(void);

/**
 * @brief Reconfigure Watchdog peripheral.
 *
 * @note Before reconfiguration, Watchdog needs to be disabled.
 *       After reconfiguration, Watchdog needs to be reenabled.
 *
 *       Watchdog function is not reconfigured, can be skipped in the configuration structure.
 *
 * @param[in] pConfig Configuration structure.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Watchdog is enabled.
 */
qResult_t qDrvWatchdog_Reconfigure(const qDrvWatchdog_Config_t* const pConfig);

/**
 * @brief Check whether the Watchdog instance is initialised.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvWatchdog_InitCheck(void);

/**
 * @brief Enable Watchdog timer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_TIMEOUT       Unable to set control register.
 */
qResult_t qDrvWatchdog_Enable(void);

/**
 * @brief Disable Watchdog timer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_TIMEOUT       Unable to set control register.
 */
qResult_t qDrvWatchdog_Disable(void);

/**
 * @brief Check whether the Watchdog instance is enabled.
 *
 * @retval True if the instance is enabled, false otherwise.
 */
Bool qDrvWatchdog_EnableCheck(void);

/**
 * @brief Reset the Watchdog counter.
 *
 * @note This function should be called periodically before the timer expires.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvWatchdog_Feed(void);

/**
 * @brief Read the current Watchdog time to expire in microseconds.
 *
 * @return Time to expire in microseconds.
 */
UInt32 qDrvWatchdog_Read(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_WATCHDOG_H_
