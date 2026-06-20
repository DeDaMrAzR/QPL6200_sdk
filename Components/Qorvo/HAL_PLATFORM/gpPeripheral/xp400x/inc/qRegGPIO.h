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

#ifndef _REG_GPIO_H_
#define _REG_GPIO_H_

#include "hal.h"
#include "qRegCommon.h"
#include "qRegDMA.h"
#include "qRegStandby.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get the GPIO action slot offset. */
#define Q_REG_GPIO_QUIC_ACTION_SLOT_OFFSET                                                                             \
    (GP_WB_GPIO_QUIC_ACTION_GPIO_1_ADDRESS - GP_WB_GPIO_QUIC_ACTION_GPIO_0_ADDRESS)

/** @enum qRegGPIO_Direction_t */
//@{
#define qRegGPIO_DirectionInput  0
#define qRegGPIO_DirectionOutput 1
/** @typedef qRegGPIO_Direction_t
 *  @brief GPIO Direction.
 */
typedef UInt8 qRegGPIO_Direction_t;
//@}

/** @enum qRegGPIO_IrqLevel_t */
//@{
#define qRegGPIO_IrqLevelLowOrRising   0
#define qRegGPIO_IrqLevelHighOrFalling 1
/** @typedef qRegGPIO_IrqLevel_t
 *  @brief GPIO interrupt level.
 */
typedef UInt8 qRegGPIO_IrqLevel_t;
//@}

/** @enum qRegGPIO_IrqType_t */
//@{
#define qRegGPIO_IrqTypeEdge  GP_WB_ENUM_GPIO_INTERRUPT_TYPE_EDGE_DETECT
#define qRegGPIO_IrqTypeLevel GP_WB_ENUM_GPIO_INTERRUPT_TYPE_LEVEL
/** @typedef qRegGPIO_IrqType_t
 *  @brief GPIO interrupt type.
 */
typedef UInt8 qRegGPIO_IrqType_t;
//@}

/** @enum qRegGPIO_SampleClk_t */
//@{
#define qRegGPIO_SampleClk32M GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M32
#define qRegGPIO_SampleClk8M  GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M8
#define qRegGPIO_SampleClk4M  GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M4
#define qRegGPIO_SampleClk2M  GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M2
/** @typedef qRegGPIO_SampleClk_t
 *  @brief GPIO input sampling clock speed.
 */
typedef UInt8 qRegGPIO_SampleClk_t;
//@}

/** @enum qRegGPIO_Wakeup_t */
//@{
#define qRegGPIO_WakeupNone    GP_WB_ENUM_WAKEUP_PIN_MODE_NO_EDGE
#define qRegGPIO_WakeupRising  GP_WB_ENUM_WAKEUP_PIN_MODE_RISING_EDGE
#define qRegGPIO_WakeupFalling GP_WB_ENUM_WAKEUP_PIN_MODE_FALLING_EDGE
#define qRegGPIO_WakeupBoth    GP_WB_ENUM_WAKEUP_PIN_MODE_BOTH_EDGES
/** @typedef qRegGPIO_Wakeup_t
 *  @brief GPIO pin wakeup edge.
 */
typedef UInt8 qRegGPIO_Wakeup_t;
//@}

/** @enum qRegGPIO_QuicActionType_t */
//@{
#define qRegGPIO_QuicActionTypeNone      GP_WB_ENUM_GPIO_ACTION_NONE
#define qRegGPIO_QuicActionTypePinSet    GP_WB_ENUM_GPIO_ACTION_SET
#define qRegGPIO_QuicActionTypePinClr    GP_WB_ENUM_GPIO_ACTION_CLR
#define qRegGPIO_QuicActionTypePinToggle GP_WB_ENUM_GPIO_ACTION_TOGGLE
/** @typedef qRegGPIO_QuicActionType_t
 *  @brief GPIO QUIC action type.
 */
typedef UInt8 qRegGPIO_QuicActionType_t;
//@}

#define Q_REG_GPIO_WAKEUP_MASK (GP_WB_PMUD_GPIO_RISING_EDGE_EVENTS_0_MASK | GP_WB_PMUD_GPIO_FALLING_EDGE_EVENTS_0_MASK)

/** @brief Enable/disable input sampling clock. */
static inline void qRegGPIO_MainClkEnableSet(Bool en)
{
    GP_WB_WRITE_GPIO_MAIN_CLOCK_ENABLE(en);
}

/** @brief Enable/disable GPIO interrupt clock. */
static inline void qRegGPIO_IRQClkEnableSet(Bool en)
{
    GP_WB_WRITE_GPIO_INTERRUPTS_CLOCK_ENABLE(en);
}

/** @brief Set input sampling clock frequency. */
static inline void qRegGPIO_MainClkSet(qRegGPIO_SampleClk_t clkFreq)
{
    GP_WB_WRITE_GPIO_SAMPLE_CLOCK_SPEED(clkFreq);
}

/** @brief Set GPIO direction. */
static inline void qRegGPIO_DirectionSet(UInt8 gpio, qRegGPIO_Direction_t direction)
{
    GP_WB_MWRITE_U32(GP_WB_GPIO_GPIO_DIRECTIONS_ADDRESS, (1UL << gpio), (direction << gpio));
}

/** @brief Get GPIO direction. */
static inline qRegGPIO_Direction_t qRegGPIO_DirectionGet(UInt8 gpio)
{
    return (qRegGPIO_Direction_t)((GP_WB_READ_U32(GP_WB_GPIO_GPIO_DIRECTIONS_ADDRESS) >> (UInt32)gpio) &
                                  GP_WB_GPIO_GPIO_0_DIRECTION_MASK);
}

/** @brief Read GPIO input state. */
static inline Bool qRegGPIO_InputGet(UInt8 gpio)
{
    return (
        (GP_WB_READ_U32(GP_WB_GPIO_GPIO_INPUT_VALUES_ADDRESS) >> (UInt32)gpio) & GP_WB_GPIO_GPIO_0_INPUT_VALUE_MASK);
}

/** @brief Read all GPIO inputs. */
static inline UInt32 qRegGPIO_InputMaskGet(void)
{
    return GP_WB_READ_U32(GP_WB_GPIO_GPIO_INPUT_VALUES_ADDRESS);
}

/** @brief Write GPIO output value. */
static inline void qRegGPIO_OutputSet(UInt8 gpio, Bool value)
{
    GP_WB_MWRITE_U32(GP_WB_GPIO_GPIO_OUTPUT_VALUES_ADDRESS, (1UL << gpio), (value << gpio));
}

/** @brief Write GPIO output value to all pins. */
static inline void qRegGPIO_OutputMaskSet(UInt32 mask)
{
    GP_WB_WRITE_U32(GP_WB_GPIO_GPIO_OUTPUT_VALUES_ADDRESS, mask & GP_WB_GPIO_GPIO_OUTPUT_VALUES_MASK);
}

/** @brief Get current GPIO output value. */
static inline Bool qRegGPIO_OutputGet(UInt8 gpio)
{
    return (
        (GP_WB_READ_U32(GP_WB_GPIO_GPIO_OUTPUT_VALUES_ADDRESS) >> (UInt32)gpio) & GP_WB_GPIO_GPIO_0_OUTPUT_VALUE_MASK);
}

/** @brief Get current GPIO output value of all pins. */
static inline UInt32 qRegGPIO_OutputMaskGet(void)
{
    return GP_WB_READ_U32(GP_WB_GPIO_GPIO_OUTPUT_VALUES_ADDRESS);
}

/** @brief Clear GPIO interrupt. */
static inline void qRegGPIO_EventIRQClear(UInt8 gpio)
{
    GP_WB_WRITE_U32(GP_WB_GPIO_CLEAR_INTERRUPTS_ADDRESS, (1UL << (UInt32)gpio));
}

/** @brief Clear all GPIO interrupts. */
static inline void qRegGPIO_EventIRQsClear(UInt32 mask)
{
    GP_WB_WRITE_GPIO_CLEAR_INTERRUPTS(mask);
}

/** @brief Set GPIO interrupt level/edge. */
static inline void qRegGPIO_EventIRQLevelSet(UInt8 gpio, qRegGPIO_IrqLevel_t irqLevel)
{
    GP_WB_MWRITE_U32(GP_WB_GPIO_INTERRUPT_LEVELS_ADDRESS, (1UL << (UInt32)gpio), (irqLevel << (UInt32)gpio));
}

/** @brief Get GPIO interrupt level/edge. */
static inline qRegGPIO_IrqLevel_t qRegGPIO_EventIRQLevelGet(UInt8 gpio)
{
    return (qRegGPIO_IrqLevel_t)((GP_WB_READ_U32(GP_WB_GPIO_INTERRUPT_LEVELS_ADDRESS) >> (UInt32)gpio) &
                                 GP_WB_GPIO_GPIO_0_INTERRUPT_LEVEL_MASK);
}

/** @brief Set GPIO interrupt type (level or edge). */
static inline void qRegGPIO_EventIRQTypeSet(UInt8 gpio, qRegGPIO_IrqType_t irqType)
{
    GP_WB_MWRITE_U32(GP_WB_GPIO_INTERRUPT_TYPES_ADDRESS, (1UL << (UInt32)gpio), (irqType << (UInt32)gpio));
}

/** @brief Get GPIO interrupt type (level or edge). */
static inline qRegGPIO_IrqType_t qRegGPIO_EventIRQTypeGet(UInt8 gpio)
{
    return (qRegGPIO_IrqType_t)((GP_WB_READ_U32(GP_WB_GPIO_INTERRUPT_TYPES_ADDRESS) >> (UInt32)gpio) &
                                GP_WB_GPIO_GPIO_0_INTERRUPT_TYPE_MASK);
}

/** @brief Set GPIO interrupt type (level or edge). */
static inline void qRegGPIO_IrqSet(UInt8 gpio)
{
    GP_WB_WRITE_U32(GP_WB_GPIO_SET_INTERRUPTS_ADDRESS, (1UL << (UInt32)gpio));
}

/** @brief Get timestamp 0 value. */
static inline UInt32 qRegGPIO_Timestamp0Get(void)
{
    return GP_WB_READ_GPIO_TIMESTAMP_0();
}

/** @brief Set timestamp 0 source. */
static inline void qRegGPIO_Timestamp0SrcSet(UInt8 gpio)
{
    GP_WB_WRITE_GPIO_TIMESTAMP_0_GPIO_SRC(gpio);
}

/** @brief Get timestamp 1 value. */
static inline UInt32 qRegGPIO_Timestamp1Get(void)
{
    return GP_WB_READ_GPIO_TIMESTAMP_1();
}

/** @brief Set timestamp 1 source. */
static inline void qRegGPIO_Timestamp1SrcSet(UInt8 gpio)
{
    GP_WB_WRITE_GPIO_TIMESTAMP_1_GPIO_SRC(gpio);
}

/** @brief Get timestamp 0 fifo not empty status. */
static inline Bool qRegGPIO_TimestampFifo0NotEmpty(void)
{
    return GP_WB_READ_GPIO_TIMESTAMP_FIFO_0_NOT_EMPTY();
}

/** @brief Get timestamp 1 fifo not empty status. */
static inline Bool qRegGPIO_TimestampFifo1NotEmpty(void)
{
    return GP_WB_READ_GPIO_TIMESTAMP_FIFO_1_NOT_EMPTY();
}

/** @brief Set pin wakeup mode. */
static inline void qRegGPIO_WakeupSet(UInt8 gpio, qRegGPIO_Wakeup_t wakeup)
{
    UInt8 lsb = gpio * (GP_WB_PMUD_WAKEUP_PIN_MODE_0_NBITS) + GP_WB_PMUD_WAKEUP_PIN_MODE_0_LSB;
    UInt8 idx = (lsb >= 32) ? 1 : 0;
    UInt32 wakeup_reg[2];

    wakeup_reg[0] = GP_WB_READ_PMUD_WAKEUP_PIN_MODES_0();
    wakeup_reg[1] = GP_WB_READ_PMUD_WAKEUP_PIN_MODES_1();
    lsb = lsb & 0x1F;
    wakeup = wakeup & GP_WB_PMUD_WAKEUP_PIN_MODE_0_MASK;
    wakeup_reg[idx] = (wakeup_reg[idx] & ~(GP_WB_PMUD_WAKEUP_PIN_MODE_0_MASK << lsb)) | (wakeup << lsb);

    qRegStandby_WakeupPinsConfigure(wakeup_reg[0], wakeup_reg[1]);
}

static inline void qRegGPIO_QuicTriggerGPIOSet(UInt8 gpioTriggerIdx, UInt8 gpio)
{
    UInt32 address = GP_WB_GPIO_QUIC_TRIGGER_GPIO_0_ADDRESS + (Q_REG_GPIO_QUIC_ACTION_SLOT_OFFSET * gpioTriggerIdx);
    GP_WB_WRITE_U32(address, gpio);
}

static inline void qRegGPIO_QuicActionChannelSet(UInt8 gpioActionIdx, UInt32 channel)
{
    UInt32 address = GP_WB_GPIO_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_GPIO_QUIC_ACTION_SLOT_OFFSET * gpioActionIdx);
    GP_WB_MWRITE_U32(address, GP_WB_GPIO_QUIC_ACTION_CHANNEL_0_MASK, channel << GP_WB_GPIO_QUIC_ACTION_CHANNEL_0_LSB);
}

static inline UInt32 qRegGPIO_QuicActionChannelGet(UInt8 gpioActionIdx)
{
    UInt32 address = GP_WB_GPIO_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_GPIO_QUIC_ACTION_SLOT_OFFSET * gpioActionIdx);
    return (GP_WB_READ_U32(address) & GP_WB_GPIO_QUIC_ACTION_CHANNEL_0_MASK) >> GP_WB_GPIO_QUIC_ACTION_CHANNEL_0_LSB;
}

static inline void qRegGPIO_QuicActionGPIOSet(UInt8 gpioActionIdx, UInt32 gpio)
{
    UInt32 address = GP_WB_GPIO_QUIC_ACTION_GPIO_0_ADDRESS + (Q_REG_GPIO_QUIC_ACTION_SLOT_OFFSET * gpioActionIdx);
    GP_WB_MWRITE_U32(address, GP_WB_GPIO_QUIC_ACTION_GPIO_0_MASK, gpio << GP_WB_GPIO_QUIC_ACTION_GPIO_0_LSB);
}

static inline void qRegGPIO_QuicActionTypeSet(UInt8 gpioActionIdx, qRegGPIO_QuicActionType_t actionType)
{
    UInt32 address = GP_WB_GPIO_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_GPIO_QUIC_ACTION_SLOT_OFFSET * gpioActionIdx);
    GP_WB_MWRITE_U32(address, GP_WB_GPIO_QUIC_ACTION_TYPE_0_MASK, actionType << GP_WB_GPIO_QUIC_ACTION_TYPE_0_LSB);
}

#ifdef __cplusplus
}
#endif

#endif /* _REG_GPIO_H_ */
