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

#ifndef _REG_LED_H_
#define _REG_LED_H_

#include "hal.h"
#include "qRegCommon.h"
#include "qRegGPIO.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief LED channel address offset. */
#define REG_LED_ADDR_OFFSET (GP_WB_LEDS_CONTROL_1_ADDRESS - GP_WB_LEDS_CONTROL_0_ADDRESS)

/** @enum qRegLED_SrcClkFreq_t */
//@{
#define qRegLED_SrcClkFreq32M GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_M32
#define qRegLED_SrcClkFreq8M  GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_M8
#define qRegLED_SrcClkFreq2M  GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_M2
#define qRegLED_SrcClkFreq62K GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_K62
/** @typedef qRegLED_SrcClkFreq_t
 *  @brief LED clock frequency.
 */
typedef UInt8 qRegLED_SrcClkFreq_t;
//@}

/** @enum qRegLED_QuicActionType_t */
//@{
#define qRegLED_QuicActionNone   GP_WB_ENUM_LED_ACTION_NONE
#define qRegLED_QuicActionSet    GP_WB_ENUM_LED_ACTION_SET
#define qRegLED_QuicActionClr    GP_WB_ENUM_LED_ACTION_CLR
#define qRegLED_QuicActionToggle GP_WB_ENUM_LED_ACTION_TOGGLE
/** @typedef qRegLED_QuicActionType_t
 *  @brief LED QUIC action type.
 */
typedef UInt8 qRegLED_QuicActionType_t;
//@}

/** @brief Enable LED channel. */
static inline void qRegLED_EnableSet(UInt8 channel, Bool enable)
{
    size_t addr = GP_WB_LEDS_LED0_ENABLE_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    GP_WB_MWRITE_U1(addr, GP_WB_LEDS_LED0_ENABLE_LSB, (UInt8)enable);
}

/** @brief Read the channel enable state. */
static inline Bool qRegLED_EnableGet(UInt8 channel)
{
    size_t addr = GP_WB_LEDS_LED0_ENABLE_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    return GP_WB_READ_U1(addr, GP_WB_LEDS_LED0_ENABLE_LSB);
}

/** @brief Enable the fading feature for a channel. */
static inline void qRegLED_FadeSet(UInt8 channel, Bool enable)
{
    size_t addr = GP_WB_LEDS_LED0_FADE_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    GP_WB_MWRITE_U1(addr, GP_WB_LEDS_LED0_FADE_LSB, (UInt8)enable);
}

/** @brief Read the fading enable state for LED channel. */
static inline Bool qRegLED_FadeGet(UInt8 channel)
{
    size_t addr = GP_WB_LEDS_LED0_FADE_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    return GP_WB_READ_U1(addr, GP_WB_LEDS_LED0_FADE_LSB);
}

/** @brief Set the threshold value, which controle the duty cycling the LED in ON state. */
static inline void qRegLED_ThresholdSet(UInt8 channel, UInt8 threshold)
{
    size_t addr = GP_WB_LEDS_LED0_THRESHOLD_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    GP_WB_WRITE_U8(addr, threshold);
}

/** @brief Read the threshold value. */
static inline Bool qRegLED_ThresholdGet(UInt8 channel)
{
    size_t addr = GP_WB_LEDS_LED0_THRESHOLD_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    return GP_WB_READ_U8(addr);
}

/** @brief Set the output drive mode. */
static inline void qRegLED_DriveSet(UInt8 channel, qRegCommon_Drive_t drive)
{
    size_t addr = GP_WB_LEDS_LED0_OUTPUT_DRIVE_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    GP_WB_MWRITE_U1(addr, GP_WB_LEDS_LED0_OUTPUT_DRIVE_LSB, (UInt8)drive);
}

/** @brief Read the output drive mode. */
static inline Bool qRegLED_DriveGet(UInt8 channel)
{
    size_t addr = GP_WB_LEDS_LED0_OUTPUT_DRIVE_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    return (qRegCommon_Drive_t)GP_WB_READ_U1(addr, GP_WB_LEDS_LED0_OUTPUT_DRIVE_LSB);
}

/** @brief Invert the LED channel output. */
static inline void qRegLED_InvertSet(UInt8 channel, Bool invert)
{
    size_t addr = GP_WB_LEDS_LED0_OUTPUT_INVERT_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    GP_WB_MWRITE_U1(addr, GP_WB_LEDS_LED0_OUTPUT_INVERT_LSB, (UInt8)invert);
}

/** @brief Read the channel invert setting. */
static inline Bool qRegLED_InvertGet(UInt8 channel)
{
    size_t addr = GP_WB_LEDS_LED0_OUTPUT_INVERT_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    return GP_WB_READ_U1(addr, GP_WB_LEDS_LED0_OUTPUT_INVERT_LSB);
}

/** @brief Allow chip to go to sleep, even if the led is driven continuously
 *         (dutycycling prevents chip from going to sleep).
 */
static inline void qRegLED_AllowSleepSet(UInt8 channel, Bool enable)
{
    size_t addr = GP_WB_LEDS_LED0_ALLOW_GOTOSLEEP_WHEN_ON_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    GP_WB_MWRITE_U1(addr, GP_WB_LEDS_LED0_ALLOW_GOTOSLEEP_WHEN_ON_LSB, (UInt8)enable);
}

/** @brief Read the state of allow-sleep setting. */
static inline Bool qRegLED_AllowSleepGet(UInt8 channel)
{
    size_t addr = GP_WB_LEDS_LED0_ALLOW_GOTOSLEEP_WHEN_ON_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    return GP_WB_READ_U1(addr, GP_WB_LEDS_LED0_ALLOW_GOTOSLEEP_WHEN_ON_LSB);
}

/** @brief Set the LED clock frequency. */
static inline void qRegLED_SrcClkFreqSet(qRegLED_SrcClkFreq_t srcClkFreq)
{
    GP_WB_WRITE_U32(GP_WB_LEDS_SRC_CLK_SPEED_ADDRESS, srcClkFreq);
}

/** @brief Get the LED clock frequency. */
static inline qRegLED_SrcClkFreq_t qRegLED_SrcClkFreqGet()
{
    return (qRegLED_SrcClkFreq_t)GP_WB_READ_U32(GP_WB_LEDS_SRC_CLK_SPEED_ADDRESS);
}

/** @brief Set the prescaler divider. */
static inline void qRegLED_PrescalerDividerSet(UInt32 prescalerDivider)
{
    GP_WB_WRITE_U32(GP_WB_LEDS_PRESCALER_DIV_ADDRESS, prescalerDivider);
}

/** @brief Set the slope time unit. */
static inline void qRegLED_SlopeTimeUnitSet(UInt32 slopeTimeUnit)
{
    GP_WB_WRITE_U32(GP_WB_LEDS_SLOPE_TIME_UNIT_ADDRESS, slopeTimeUnit);
}

/** @brief Enable/disable backup of peripheral registers before entering deep sleep mode. */
static inline void qRegLED_RetainSet(Bool enable)
{
    GP_WB_WRITE_SYSRAM_RETAIN_LEDS((UInt8)enable);
}

static inline void qRegLED_QuicChannelSet(UInt8 channel, UInt32 actionIdx, UInt32 quicChannel)
{
    size_t addr = GP_WB_LEDS_LED0_QUIC_ACTION_CHANNEL_0_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    addr += (sizeof(UInt32) * actionIdx);

    GP_WB_MWRITE_U32(addr, GP_WB_LEDS_LED0_QUIC_ACTION_CHANNEL_0_MASK,
                     quicChannel << GP_WB_LEDS_LED0_QUIC_ACTION_CHANNEL_0_LSB);
}

static inline UInt32 qRegLED_QuicChannelGet(UInt8 channel, UInt32 actionIdx)
{
    size_t addr = GP_WB_LEDS_LED0_QUIC_ACTION_CHANNEL_0_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    addr += (sizeof(UInt32) * actionIdx);

    return (GP_WB_READ_U32(addr) & GP_WB_LEDS_LED0_QUIC_ACTION_CHANNEL_0_MASK) >>
           GP_WB_LEDS_LED0_QUIC_ACTION_CHANNEL_0_LSB;
}

static inline void qRegLED_QuicActionTypeSet(UInt8 channel, UInt32 actionIdx, qRegLED_QuicActionType_t actionType)
{
    size_t addr = GP_WB_LEDS_LED0_QUIC_ACTION_TYPE_0_ADDRESS + (REG_LED_ADDR_OFFSET * channel);
    addr += (sizeof(UInt32) * actionIdx);

    GP_WB_MWRITE_U32(addr, GP_WB_LEDS_LED0_QUIC_ACTION_TYPE_0_MASK,
                     actionType << GP_WB_LEDS_LED0_QUIC_ACTION_TYPE_0_LSB);
}

#ifdef __cplusplus
}
#endif

#endif //_REG_LED_H_
