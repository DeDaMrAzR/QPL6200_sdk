/*
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

#ifndef _HAL_TIMERXL_H_
#define _HAL_TIMERXL_H_

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name halTimerXL_timerId_t */
//@{
/** @brief Specifying timer0 */
#define halTimerXL_timer0 0
/** @brief Specifying timer1 */
#define halTimerXL_timer1 1
/** @brief Specifying timer2 */
#define halTimerXL_timer2 2
/** @brief Specifying timer3 */
#define halTimerXL_timer3 3
/** @brief Specifying timer3 */
#define halTimerXL_timer4 4
/** @brief Specifying timer3 */
#define halTimerXL_timer5 5

#define HAL_TIMERXL_COUNT 6

/* number of compare-capture registers per timer */
#define HAL_COMPARECAPTURE_COUNT 4

/** @typedef halTimerXL_timerId_t
 *  @brief Identifying the XL timer
 */
typedef UInt8 halTimerXL_timerId_t;

/** @typedef halTimerXL_countMode_t
 *  @brief Specify Set the input signal for timer
 */
typedef UInt8 halTimerXL_countMode_t;

/** @typedef halTimerXL_mode_t
 *  @brief Specify single 32-bit, dual 16-bit or disabled.
 */
typedef UInt8 halTimerXL_mode_t;

// /** @name halTimerXL_clkSel_t */
// //@{
// /** @brief Specifying internal 16 MHz clock */
// #define halTimerXL_clkSelIntClk 0
// /** @brief Specifying timer0 as input clock */
// #define halTimerXL_clkSelTmr0 1
// /** @brief Specifying timer1 as input clock */
// #define halTimerXL_clkSelTmr1 2
// /** @brief Specifying timer2 as input clock */
// #define halTimerXL_clkSelTmr2 3
// //@}

/** @name halTimerXL_countMode_t */
//@{
/** @brief Specifying internal 16 MHz clock */
#define halTimerXL_countModeIntClk GP_WB_ENUM_TIMERXL_COUNT_MODE_PRESCALED
/** @brief Specifying an external trigger */
#define halTimerXL_countModeExtTrig GP_WB_ENUM_TIMERXL_COUNT_MODE_EXTERNAL
//@}

/** @name halTimerXL_mode_t */
//@{
/** @brief Specifying internal 16 MHz clock */
#define halTimerXL_modeDisabled      GP_WB_ENUM_TIMERXL_OPERATING_MODE_DISABLED
#define halTimerXL_modeSingle32Bits  GP_WB_ENUM_TIMERXL_OPERATING_MODE_SINGLE_32BITS
#define halTimerXL_modeDual16Bits    GP_WB_ENUM_TIMERXL_OPERATING_MODE_DUAL_16BITS
//@}

/* Callback type (called on timer wrap interrupt) */
typedef void (*halTimerXL_cbTimerWrapInterruptHandler_t)(void);

/**
 * @brief Initialize  the timer HAL
 */
void halTimerXL_Init(void);

/**
 * @brief returns a free timer Id
 * Allows the applications to dynamically use a free timer, avoiding the
 * need to hard-code timer-ids in applications and in other parts of the stack.
 *
 * @return              TimerId of a timer which is not in use.
 */
halTimerXL_timerId_t halTimerXL_getFreeTimerId(void);

/**
 * @brief Initialize timer
 * Configures the timer
 * E.g., The callback on interrupt will be triggered at
 * ((1/16MHz) * (2**prescalerDiv) * threshold) when internal
 * clock is selected
 *
 * @param timerId       Identifies the timer
 * @param prescalerDiv  The prescaler value to scale down the input clock by a factor of 2**val
 * @param countMode     The input clock to the timer
 * @param threshold     Threshold value at which the timer timer should wrap
 * @param Inthandler    Callback program to be called when the interrupt fires. If
 *                      this is set to NULL, interrupt mask will not be enabled for
 *                      this timer (no interrupt fires).
 * @param isPeriodic    Set state of periodicity timer interrupt
 */
void halTimerXL_initTimer(halTimerXL_timerId_t timerId,
                          UInt8 prescalerDiv,
                          halTimerXL_countMode_t countMode,
                          UInt32 threshold,
                          halTimerXL_cbTimerWrapInterruptHandler_t Inthandler,
                          Bool isPeriodic);

/**
 * @brief Start the timer
 *
 * @param timerId       Identifies the timer
 */
void halTimerXL_startTimer(halTimerXL_timerId_t timerId);

/**
 * @brief Stop the timer
 *
 * @param timerId       Identifies the timer
 */
void halTimerXL_stopTimer(halTimerXL_timerId_t timerId);

/**
 * @brief Reset the timer
 *
 * @param timerId       Identifies the timer
 */
void halTimerXL_resetTimer(halTimerXL_timerId_t timerId);

/**
 * @brief Free the timer to be initialized and used
 * for other purposes
 *
 * @param timerId       Identifies the timer
 */
void halTimerXL_freeTimer(halTimerXL_timerId_t timerId);

/**
 * @brief Check state of timer
 *
 * @param timerId       Identifies the timer
 * @param @return       1 - start timer, 0 - stop timer
 */
UInt32 halTimerXL_isEnabledTimer(halTimerXL_timerId_t timerId);

/**
 * @brief Set the prescaler value to scale down the input clock by a factor
 * of 2**val
 *
 * @param timerId       Identifies the timer
 * @param val           prescaler value
 */
void halTimerXL_setPrescalerDiv(halTimerXL_timerId_t timerId, UInt16 val);

/**
 * @brief Set the input signal for timer with timerId
 *
 * @param timerId       Identifies the timer
 * @param val           input signal for the timer; possibilities are defined by halTimerXL_countMode_t
 */
void halTimerXL_setCountMode(halTimerXL_timerId_t timerId, halTimerXL_countMode_t countMode);

/**
 * @brief Set the threshold value at which the timer should wrap
 *
 * @param timerId       Identifies the timer
 * @param val           the threshold value
 */
void halTimerXL_setThreshold(halTimerXL_timerId_t timerId, UInt32 threshold);

/**
 * @brief Get the threshold value at which the timer should wrap
 *
 * @param timerId       Identifies the timer
 * @return              the threshold value
 */
UInt32 halTimerXL_getThreshold(halTimerXL_timerId_t timerId);

/**
 * @brief Capture the current timer value in one of the capture registers.
 *
 * @param timerId       Identifies the timer
 * @param captureIndex  Index of the capture register
 */
void halTimerXL_captureTimer(halTimerXL_timerId_t timerId, UInt8 captureIndex);

/**
 * @brief Get the captured value of the timer.
 *
 * @param timerId       Identifies the timer
 * @param captureIndex  Index of the capture register
 * @return              the timer value
 */
UInt32 halTimerXL_getCapturedValue(halTimerXL_timerId_t timerId, UInt8 captureIndex);

/**
 * @brief Read back the current timer value
 *
 * @param timerId       Identifies the timer
 * @return              current timer value
 */
UInt32 halTimerXL_getTimerValue(halTimerXL_timerId_t timerId);

/**
 * @brief Set the current timer value
 *
 * @param timerId       Identifies the timer
 * @param val           current timer value
 */
void halTimerXL_setTimerPreset(halTimerXL_timerId_t timerId, UInt32 val);

#ifdef __cplusplus
}
#endif

#endif // _HAL_TIMERXL_H_
