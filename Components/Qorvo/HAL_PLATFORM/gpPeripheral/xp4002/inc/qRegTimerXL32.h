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

#ifndef _Q_REG_TIMERXL32_H_
#define _Q_REG_TIMERXL32_H_

#include "qRegTimerXL.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief 32-bit TimerXL instance pointer. */
typedef size_t qRegTimerXL32_t;

/** @brief Get the address offset of a subregister within the specific compare channel group. */
#define Q_REG_TIMERXL32_CC_REG_OFFSET_GET(CC, REGNAME)                                                                 \
    (Q_CONCAT(GP_WB_TIMERXL32_CC_0_, REGNAME, _ADDRESS) +                                                              \
     (Q_CONCAT(GP_WB_TIMERXL32_CC_1_, REGNAME, _ADDRESS) - Q_CONCAT(GP_WB_TIMERXL32_CC_0_, REGNAME, _ADDRESS)) *       \
         (CC) +                                                                                                        \
     (Q_CONCAT(GP_WB_TIMERXL32_CC_0_, REGNAME, _LSB) / 8))

/** @brief Get the bit position of a subregister within the specific compare channel group. */
#define Q_REG_TIMERXL32_CC_REG_POS_GET(CC, REGNAME) (Q_CONCAT(GP_WB_TIMERXL32_CC_0_, REGNAME, _LSB) % 8)

/** @brief Write the specific CC flag of a TimerXL instance.. */
#define Q_REG_TIMERXL32_CC_FLAG_WRITE(BASEADDR, CC, FLAGNAME, VALUE)                                                   \
    GP_WB_MWRITE_U1((BASEADDR + Q_REG_TIMERXL32_CC_REG_OFFSET_GET(CC, FLAGNAME)),                                      \
                    Q_REG_TIMERXL32_CC_REG_POS_GET(CC, FLAGNAME), VALUE)

/** @brief Read the specific CC flag of a TimerXL instance.. */
#define Q_REG_TIMERXL32_CC_FLAG_READ(BASEADDR, CC, FLAGNAME)                                                           \
    GP_WB_READ_U1((BASEADDR + Q_REG_TIMERXL32_CC_REG_OFFSET_GET(CC, FLAGNAME)),                                        \
                  Q_REG_TIMERXL32_CC_REG_POS_GET(CC, FLAGNAME))

/** @brief Get the TimerXL 32-bit isntance base address. */
static inline qRegTimerXL32_t qRegTimerXL32_BaseAddrGet(UInt8 id)
{
    return (qRegTimerXL_t)(GP_WB_TIMERXL_0_BASE_ADDRESS +
                           (GP_WB_TIMERXL_1_BASE_ADDRESS - GP_WB_TIMERXL_0_BASE_ADDRESS) * id);
}

/** @brief Set the prescaler divider. */
static inline void qRegTimerXL32_PrescalerSet(qRegTimerXL32_t baseAddr, UInt8 prescaler)
{
    GP_WB_WRITE_TIMERXL32_PRESCALER_DIV((size_t)baseAddr, prescaler);
}

/** @brief Read the prescaler divider. */
static inline UInt8 qRegTimerXL32_PrescalerGet(qRegTimerXL32_t baseAddr)
{
    return GP_WB_READ_TIMERXL32_PRESCALER_DIV((size_t)baseAddr);
}

/** @brief Set the count mode divider. */
static inline void qRegTimerXL32_CountModeSet(qRegTimerXL32_t baseAddr, qRegTimerXL_CountMode_t countMode)
{
    GP_WB_WRITE_TIMERXL32_COUNT_MODE((size_t)baseAddr, countMode);
}

/** @brief Read the count mode divider. */
static inline qRegTimerXL_CountMode_t qRegTimerXL32_CountModeGet(qRegTimerXL32_t baseAddr)
{
    return (qRegTimerXL_CountMode_t)GP_WB_READ_TIMERXL32_COUNT_MODE((size_t)baseAddr);
}

/** @brief Start the timer. */
static inline void qRegTimerXL32_Start(qRegTimerXL32_t baseAddr)
{
    GP_WB_TIMERXL32_START(baseAddr);
}

/** @brief Stop the timer. */
static inline void qRegTimerXL32_Stop(qRegTimerXL32_t baseAddr)
{
    GP_WB_TIMERXL32_STOP(baseAddr);
}

/** @brief Reset the timer value including prescaler. */
static inline void qRegTimerXL32_Reset(qRegTimerXL32_t baseAddr)
{
    GP_WB_TIMERXL32_RESET(baseAddr);
}

/** @brief Presets the timer value to the value set by the compare channel 0 register. */
static inline void qRegTimerXL32_Preset(qRegTimerXL32_t baseAddr)
{
    GP_WB_TIMERXL32_PRESET(baseAddr);
}

/** @brief Clear the overflow interrupt flag. */
static inline void qRegTimerXL32_IRQOverflowClear(qRegTimerXL32_t baseAddr)
{
    GP_WB_TIMERXL32_CLR_OVERFLOW_INTERRUPT(baseAddr);
}

/** @brief Read the overflow interrupt flag. */
static inline Bool qRegTimerXL32_IRQOverflowGet(qRegTimerXL32_t baseAddr)
{
    return (Bool)GP_WB_READ_TIMERXL32_UNMASKED_OVERFLOW_INTERRUPT(baseAddr);
}

/** @brief Get the running state of the timer. */
static inline Bool qRegTimerXL32_RunningGet(qRegTimerXL32_t baseAddr)
{
    return (Bool)GP_WB_READ_TIMERXL32_RUNNING(baseAddr);
}

/** @brief Get the current counter value. */
static inline UInt32 qRegTimerXL32_CounterGet(qRegTimerXL32_t baseAddr)
{
    return GP_WB_READ_TIMERXL32_COUNTER(baseAddr);
}

/** @brief Set the compare channel value. */
static inline void qRegTimerXL32_CCValueSet(qRegTimerXL32_t baseAddr, UInt8 cc, UInt32 value)
{
    size_t addr = baseAddr + Q_REG_TIMERXL32_CC_REG_OFFSET_GET(cc, COMPARECAPTURE);
    GP_WB_WRITE_U32(addr, value);
}

/** @brief Read the captured value for a giveen channel. */
static inline UInt32 qRegTimerXL32_CCValueGet(qRegTimerXL32_t baseAddr, UInt8 cc)
{
    size_t addr = baseAddr + Q_REG_TIMERXL32_CC_REG_OFFSET_GET(cc, COMPARECAPTURE);
    return GP_WB_READ_U32(addr);
}

/** @brief Set the clear-on-match flag. */
static inline void qRegTimerXL32_ClearOnMatchSet(qRegTimerXL32_t baseAddr, UInt8 cc, Bool enable)
{
    Q_REG_TIMERXL32_CC_FLAG_WRITE(baseAddr, cc, CLR_ON_MATCH, enable);
}

/** @brief Read the clear-on-match flag. */
static inline Bool qRegTimerXL32_ClearOnMatchGet(qRegTimerXL32_t baseAddr, UInt8 cc)
{
    return (Bool)Q_REG_TIMERXL32_CC_FLAG_READ(baseAddr, cc, CLR_ON_MATCH);
}

/** @brief Set the oneshot mode. */
static inline void qRegTimerXL32_OneshotSet(qRegTimerXL32_t baseAddr, UInt8 cc, Bool enable)
{
    Q_REG_TIMERXL32_CC_FLAG_WRITE(baseAddr, cc, ONESHOT_MODE, enable);
}

/** @brief Read the oneshot mode state. */
static inline Bool qRegTimerXL32_OneshotGet(qRegTimerXL32_t baseAddr, UInt8 cc)
{
    return (Bool)Q_REG_TIMERXL32_CC_FLAG_READ(baseAddr, cc, ONESHOT_MODE);
}

/** @brief Trigger the capture operation on a given channel. */
static inline void qRegTimerXL32_Capture(qRegTimerXL32_t baseAddr, UInt8 cc)
{
    Q_REG_TIMERXL32_CC_FLAG_WRITE(baseAddr, cc, CAPTURE, 1);
}

/** @brief Set the clear-on-match flag. */
static inline void qRegTimerXL32_IRQMatchClear(qRegTimerXL32_t baseAddr, UInt8 cc)
{
    size_t addr =
        baseAddr + GP_WB_TIMERXL32_CLR_CC_0_MATCH_INTERRUPT_ADDRESS +
        (GP_WB_TIMERXL32_CLR_CC_1_MATCH_INTERRUPT_ADDRESS - GP_WB_TIMERXL32_CLR_CC_0_MATCH_INTERRUPT_ADDRESS) * cc;

    GP_WB_MWRITE_U1(addr, GP_WB_TIMERXL32_CLR_CC_0_MATCH_INTERRUPT_LSB, 1);
}

/** @brief Read the clear-on-match flag. */
static inline Bool qRegTimerXL32_IRQMatchGet(qRegTimerXL32_t baseAddr, UInt8 cc)
{
    size_t addr = baseAddr + GP_WB_TIMERXL32_UNMASKED_CC_0_MATCH_INTERRUPT_ADDRESS +
                  (GP_WB_TIMERXL32_UNMASKED_CC_1_MATCH_INTERRUPT_ADDRESS -
                   GP_WB_TIMERXL32_UNMASKED_CC_0_MATCH_INTERRUPT_ADDRESS) *
                      cc;

    return (Bool)GP_WB_READ_U1(addr, GP_WB_TIMERXL32_UNMASKED_CC_0_MATCH_INTERRUPT_LSB);
}

#ifdef __cplusplus
}
#endif

#endif //_Q_REG_TIMERXL32_H_
