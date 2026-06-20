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

#ifndef _Q_REG_TIMERXL_H_
#define _Q_REG_TIMERXL_H_

#include "hal.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get the TIMERXL action slot offset. */
#define Q_REG_TIMERXL_QUIC_ACTION_SLOT_OFFSET                                                                          \
    (GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_1_ADDRESS - GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_0_ADDRESS)

/** @brief TimerXL base address type. */
typedef size_t qRegTimerXL_t;

/** @enum qDrvTimerXL_Irq_t */
//@{
/** @brief Compare channel 0 match 0. */
#define qRegTimerXL_IrqCC0Match0 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_0_MATCH_0_INTERRUPT_LSB
/** @brief Compare channel 0 match 1. */
#define qRegTimerXL_IrqCC0Match1 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_0_MATCH_1_INTERRUPT_LSB
/** @brief Compare channel 1 match 0. */
#define qRegTimerXL_IrqCC1Match0 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_1_MATCH_0_INTERRUPT_LSB
/** @brief Compare channel 1 match 1. */
#define qRegTimerXL_IrqCC1Match1 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_1_MATCH_1_INTERRUPT_LSB
/** @brief Compare channel 2 match 0. */
#define qRegTimerXL_IrqCC2Match0 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_2_MATCH_0_INTERRUPT_LSB
/** @brief Compare channel 2 match 1. */
#define qRegTimerXL_IrqCC2Match1 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_2_MATCH_1_INTERRUPT_LSB
/** @brief Compare channel 3 match 0. */
#define qRegTimerXL_IrqCC3Match0 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_3_MATCH_0_INTERRUPT_LSB
/** @brief Compare channel 3 match 1. */
#define qRegTimerXL_IrqCC3Match1 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_3_MATCH_1_INTERRUPT_LSB
/** @brief Compare channel 4 match 0. */
#define qRegTimerXL_IrqCC4Match0 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_4_MATCH_0_INTERRUPT_LSB
/** @brief Compare channel 4 match 1. */
#define qRegTimerXL_IrqCC4Match1 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_4_MATCH_1_INTERRUPT_LSB
/** @brief Compare channel 5 match 0. */
#define qRegTimerXL_IrqCC5Match0 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_5_MATCH_0_INTERRUPT_LSB
/** @brief Compare channel 5 match 1. */
#define qRegTimerXL_IrqCC5Match1 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_CC_5_MATCH_1_INTERRUPT_LSB
/** @brief Main counter overflow 0. */
#define qRegTimerXL_IrqOverflow0 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_OVERFLOW_0_INTERRUPT_LSB
/** @brief Main counter overflow 1. */
#define qRegTimerXL_IrqOverflow1 GP_WB_INT_CTRL_MASK_APPUC_TIMERXL_0_OVERFLOW_1_INTERRUPT_LSB
/** @typedef qDrvTimerXL_Irq_t
 *  @brief TimerXL interrupt type.
 */
typedef UInt8 qDrvTimerXL_Irq_t;
//@}

/** @brief Maximum prescaler divider value. */
#define Q_REG_TIMERXL_PRESCALER_MAX GP_WB_TIMERXL_0_TIMERXL32_PRESCALER_DIV_MASK

/** @brief Get the interrupt number for a given TimerXL block. */
#define Q_REG_TIMERXL_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(TIMERXL_, ID, _IRQn))

/** @enum qRegTimerXL_Mode_t */
//@{
#define qRegTimerXL_ModeDisabled GP_WB_ENUM_TIMERXL_OPERATING_MODE_DISABLED
#define qRegTimerXL_Mode32Bit    GP_WB_ENUM_TIMERXL_OPERATING_MODE_SINGLE_32BITS
#define qRegTimerXL_Mode16Bit    GP_WB_ENUM_TIMERXL_OPERATING_MODE_DUAL_16BITS
/** @typedef qRegTimerXL_Mode_t
 *  @brief TimerXL operating mode.
 */
typedef UInt8 qRegTimerXL_Mode_t;
//@}

/** @enum qRegTimerXL_CountMode_t */
//@{
#define qRegTimerXL_CountModePrescaled GP_WB_ENUM_TIMERXL_COUNT_MODE_PRESCALED
#define qRegTimerXL_CountModeExternal  GP_WB_ENUM_TIMERXL_COUNT_MODE_EXTERNAL
/** @typedef qRegTimerXL_CountMode_t
 *  @brief TimerXL counting mode.
 */
typedef UInt8 qRegTimerXL_CountMode_t;
//@}

/** @enum qRegTimerXL_QuicActionType_t */
//@{
#define qRegTimerXL_QuicActionTypeNone           GP_WB_ENUM_TIMERXL_ACTION_NONE
#define qRegTimerXL_QuicActionTypeIncrementTimer GP_WB_ENUM_TIMERXL_ACTION_INCREMENT_TIMER
#define qRegTimerXL_QuicActionTypeStartTimer     GP_WB_ENUM_TIMERXL_ACTION_START_TIMER
#define qRegTimerXL_QuicActionTypeStopTimer      GP_WB_ENUM_TIMERXL_ACTION_STOP_TIMER
#define qRegTimerXL_QuicActionTypeToggleTimer    GP_WB_ENUM_TIMERXL_ACTION_TOGGLE_TIMER
#define qRegTimerXL_QuicActionTypeResetTimer     GP_WB_ENUM_TIMERXL_ACTION_RESET_TIMER
#define qRegTimerXL_QuicActionTypePresetTimer    GP_WB_ENUM_TIMERXL_ACTION_PRESET_TIMER
#define qRegTimerXL_QuicActionTypeCapture0       GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_0
#define qRegTimerXL_QuicActionTypeCapture1       GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_1
#define qRegTimerXL_QuicActionTypeCapture2       GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_2
#define qRegTimerXL_QuicActionTypeCapture3       GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_3
/** @typedef qRegTimerXL_QuicActionType_t
 *  @brief TimerXL QUIC action type.
 */
typedef UInt8 qRegTimerXL_QuicActionType_t;
//@}

/** @brief Get the TimerXL block base address. */
static inline qRegTimerXL_t qRegTimerXL_BlockAddrGet(UInt8 blockId)
{
    return (qRegTimerXL_t)(GP_WB_TIMERXL_0_BASE_ADDRESS +
                           (GP_WB_TIMERXL_1_BASE_ADDRESS - GP_WB_TIMERXL_0_BASE_ADDRESS) * blockId);
}

/** @brief Set the TimerXL block mode of operation. */
static inline void qRegTimerXL_ModeSet(qRegTimerXL_t blockBaseAddr, qRegTimerXL_Mode_t mode)
{
    GP_WB_WRITE_TIMERXL_MODE(blockBaseAddr, mode);
}

/** @brief Read the TimerXL block mode of operation. */
static inline qRegTimerXL_Mode_t qRegTimerXL_ModeGet(qRegTimerXL_t blockBaseAddr)
{
    return (qRegTimerXL_Mode_t)GP_WB_READ_TIMERXL_MODE(blockBaseAddr);
}

static inline void qRegTimerXL_QUICActionChannelSet(qRegTimerXL_t blockBaseAddr, UInt8 quicActionIdx,
                                                    UInt8 quicActionChannel)
{
    UInt32 address = blockBaseAddr + GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_0_ADDRESS +
                     (Q_REG_TIMERXL_QUIC_ACTION_SLOT_OFFSET * quicActionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_0_MASK,
                     ((UInt32)quicActionChannel << GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_0_LSB));
}

static inline UInt8 qRegTimerXL_QUICActionChannelGet(qRegTimerXL_t blockBaseAddr, UInt8 quicActionIdx)
{
    UInt32 address = blockBaseAddr + GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_0_ADDRESS +
                     (Q_REG_TIMERXL_QUIC_ACTION_SLOT_OFFSET * quicActionIdx);

    return (UInt8)((GP_WB_READ_U32(address) & GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_0_MASK) >>
                   GP_WB_TIMERXL_QUIC_ACTION_CHANNEL_0_LSB);
}

static inline void qRegTimerXL_QUICActionTimerSet(qRegTimerXL_t blockBaseAddr, UInt8 quicActionIdx, UInt8 timerIdx)
{
    UInt32 address = blockBaseAddr + GP_WB_TIMERXL_QUIC_ACTION_TIMERXL_0_ADDRESS +
                     (Q_REG_TIMERXL_QUIC_ACTION_SLOT_OFFSET * quicActionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_TIMERXL_QUIC_ACTION_TIMERXL_0_MASK,
                     ((UInt32)timerIdx << GP_WB_TIMERXL_QUIC_ACTION_TIMERXL_0_LSB));
}

static inline void qRegTimerXL_QUICActionTypeSet(qRegTimerXL_t blockBaseAddr, UInt8 quicActionIdx,
                                                 qRegTimerXL_QuicActionType_t quicActionType)
{
    UInt32 address = blockBaseAddr + GP_WB_TIMERXL_QUIC_ACTION_TYPE_0_ADDRESS +
                     (Q_REG_TIMERXL_QUIC_ACTION_SLOT_OFFSET * quicActionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_TIMERXL_QUIC_ACTION_TYPE_0_MASK,
                     ((UInt32)quicActionType << GP_WB_TIMERXL_QUIC_ACTION_TYPE_0_LSB));
}

#ifdef __cplusplus
}
#endif

#endif //_Q_REG_TIMERXL_H_
