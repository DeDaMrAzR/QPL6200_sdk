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

#ifndef _Q_REG_IR_H_
#define _Q_REG_IR_H_

#include "hal.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GP_WB_IR_0_BASE_ADDRESS GP_WB_IR_BASE_ADDRESS

/** @brief IR generator peripheral base address type. */
typedef size_t qRegIR_t;

/** @brief Get the IR generator peripheral base address by ID. */
#define Q_REG_IR_BASEADDR_GET(ID) ((qRegIR_t)Q_CONCAT(GP_WB_IR_, ID, _BASE_ADDRESS))

/** @brief Get the IR generator peripheral interrupt number. */
#define Q_REG_IR_IRQN_GET(ID) IR_IRQn

/** @brief Header of the IR generator interrupt handler. */
#define Q_REG_IR_HANDLER_DEFINE(ID) void ir_handler_impl(void)

/** @brief Maximum prescaler divider value. */
#define Q_REG_IR_PRESCALER_MAX 8

/** @brief Get the IR generator QUIC action slot offset. */
#define Q_REG_IR_QUIC_ACTION_SLOT_OFFSET                                                                               \
    (GP_WB_IR_QUIC_ACTION_CHANNEL_1_ADDRESS - GP_WB_IR_QUIC_ACTION_CHANNEL_0_ADDRESS)

/** @enum qDrvIR_Irq_t */
//@{
/** @brief RAM sequence index match interrupt. */
#define qRegIR_IrqIndexMatch GP_WB_INT_CTRL_MASK_APPUC_IR_INDEX_MATCH_INTERRUPT_LSB
/** @brief RAM sequence complete interrupt. */
#define qRegIR_IrqSequenceDone GP_WB_INT_CTRL_MASK_APPUC_IR_SEQUENCE_DONE_INTERRUPT_LSB
/** @brief RAM sequence repeat interrupt. */
#define qRegIR_IrqSequenceRepeat GP_WB_INT_CTRL_MASK_APPUC_IR_SEQUENCE_REPEAT_INTERRUPT_LSB
/** @brief RAM sequence start interrupt. */
#define qRegIR_IrqSequenceStart GP_WB_INT_CTRL_MASK_APPUC_IR_SEQUENCE_START_INTERRUPT_LSB
/** @typedef qDrvIR_Irq_t
 *  @brief IR generator interrupt type.
 */
typedef UInt8 qDrvIR_Irq_t;
//@}

/** @enum qRegIR_Mode_t */
//@{
#define qRegIR_ModeOff      GP_WB_ENUM_IR_MODULATION_MODE_MODULATION_OFF
#define qRegIR_ModeRegister GP_WB_ENUM_IR_MODULATION_MODE_REGISTER_BASED
#define qRegIR_ModePattern  GP_WB_ENUM_IR_MODULATION_MODE_PATTERN_BASED
#define qRegIR_ModeTime     GP_WB_ENUM_IR_MODULATION_MODE_TIME_BASED
#define qRegIR_ModeExternal GP_WB_ENUM_IR_MODULATION_MODE_EXT_BASED
/** @typedef qRegIR_Mode_t
 *  @brief IR generator modulation mode mode.
 */
typedef UInt8 qRegIR_Mode_t;
//@}

/** @enum qRegIR_TimeUnit_t */
//@{
#define qRegIR_TimeUnit500ns GP_WB_ENUM_IR_TIME_UNIT_TU_500NS
#define qRegIR_TimeUnit1us   GP_WB_ENUM_IR_TIME_UNIT_TU_1US
#define qRegIR_TimeUnit2us   GP_WB_ENUM_IR_TIME_UNIT_TU_2US
#define qRegIR_TimeUnit4us   GP_WB_ENUM_IR_TIME_UNIT_TU_4US
/** @typedef qRegIR_TimeUnit_t
 *  @brief IR generator time unit.
 */
typedef UInt8 qRegIR_TimeUnit_t;
//@}

/** @enum qRegIR_QuicActionType_t */
//@{
#define qRegIR_QuicActionTypeNone     GP_WB_ENUM_IR_ACTION_NONE
#define qRegIR_QuicActionTypeStartSeq GP_WB_ENUM_IR_ACTION_START_RAM_SEQUENCER
#define qRegIR_QuicActionTypeAltSet   GP_WB_ENUM_IR_ACTION_ALT_CARRIER_SET
#define qRegIR_QuicActionTypeAltUnset GP_WB_ENUM_IR_ACTION_ALT_CARRIER_UNSET
/** @typedef qRegIR_QuicActionType_t
 *  @brief IR generator QUIC action type.
 */
typedef UInt8 qRegIR_QuicActionType_t;
//@}

/** @brief Set the carrier frequency prescaler divider. */
static inline void qRegIR_PrescalerSet(UInt8 prescaler)
{
    GP_WB_WRITE_IR_PRESCALE_DIV(prescaler);
}

/** @brief Read the carrier frequency prescaler divider. */
static inline UInt8 qRegIR_PrescalerGet(void)
{
    return GP_WB_READ_IR_PRESCALE_DIV();
}

/** @brief Set the carrier ON phase threshold. */
static inline void qRegIR_ThresholdSet(UInt8 prescaler)
{
    GP_WB_WRITE_IR_THRESHOLD(prescaler);
}

/** @brief Read the carrier ON phase threshold. */
static inline UInt8 qRegIR_ThresholdGet(void)
{
    return GP_WB_READ_IR_THRESHOLD();
}

/** @brief Set the carrier period value. */
static inline void qRegIR_WrapSet(UInt8 prescaler)
{
    GP_WB_WRITE_IR_WRAP(prescaler);
}

/** @brief Read the carrier period value. */
static inline UInt8 qRegIR_WrapGet(void)
{
    return GP_WB_READ_IR_WRAP();
}

/** @brief Set the alternate carrier ON phase threshold. */
static inline void qRegIR_AltThresholdSet(UInt8 prescaler)
{
    GP_WB_WRITE_IR_ALT_THRESHOLD(prescaler);
}

/** @brief Read the alternate carrier ON phase threshold. */
static inline UInt8 qRegIR_AltThresholdGet(void)
{
    return GP_WB_READ_IR_ALT_THRESHOLD();
}

/** @brief Set the alternate carrier period value. */
static inline void qRegIR_AltWrapSet(UInt8 prescaler)
{
    GP_WB_WRITE_IR_ALT_WRAP(prescaler);
}

/** @brief Read the alternate carrier period value. */
static inline UInt8 qRegIR_AltWrapGet(void)
{
    return GP_WB_READ_IR_ALT_WRAP();
}

/** @brief Set the carrier time unit value. */
static inline void qRegIR_CarrierTUSet(UInt8 prescaler)
{
    GP_WB_WRITE_IR_CARRIER_TU(prescaler);
}

/** @brief Read the carrier time unit value. */
static inline UInt8 qRegIR_CarrierTUGet(void)
{
    return GP_WB_READ_IR_CARRIER_TU();
}

/** @brief Set the modulation time unit for time based mode. */
static inline void qRegIR_TimerTUSet(qRegIR_TimeUnit_t timeUnit)
{
    GP_WB_WRITE_IR_TIMER_TU(timeUnit);
}

/** @brief Read the modulation time unit for time based mode. */
static inline qRegIR_TimeUnit_t qRegIR_TimerTUGet(void)
{
    return (qRegIR_TimeUnit_t)GP_WB_READ_IR_TIMER_TU();
}

/** @brief Set the IR generator modulation mode. */
static inline void qRegIR_ModulationModeSet(qRegIR_Mode_t mode)
{
    GP_WB_WRITE_IR_MODULATION_MODE(mode);
}

/** @brief Read the IR generator modulation mode. */
static inline qRegIR_Mode_t qRegIR_ModulationModeGet(void)
{
    return (qRegIR_Mode_t)GP_WB_READ_IR_MODULATION_MODE();
}

/** @brief Set the ALIGN_ON_START flag. */
static inline void qRegIR_AlignOnStartSet(Bool align)
{
    GP_WB_WRITE_IR_ALIGN_ON_START(align);
}

/** @brief Read the ALIGN_ON_START flag. */
static inline Bool qRegIR_AlignOnStartGet(void)
{
    return (Bool)GP_WB_READ_IR_ALIGN_ON_START();
}

/** @brief Set the LATCH_ON_START flag. */
static inline void qRegIR_LatchOnStartSet(Bool align)
{
    GP_WB_WRITE_IR_MODULATION_LATCH_ON_START(align);
}

/** @brief Read the LATCH_ON_START flag. */
static inline Bool qRegIR_LatchOnStartGet(void)
{
    return (Bool)GP_WB_READ_IR_MODULATION_LATCH_ON_START();
}

/** @brief Set the direct modulation value for direct register based mode. */
static inline void qRegIR_ModulationValueSet(Bool value)
{
    GP_WB_WRITE_IR_MODULATION(value);
}

/** @brief Read the direct modulation value for direct register based mode. */
static inline Bool qRegIR_ModulationValueGet(void)
{
    return (Bool)GP_WB_READ_IR_MODULATION();
}

/** @brief Set the sequence repetition enable state. */
static inline void qRegIR_RepeatSet(Bool repeat)
{
    GP_WB_WRITE_IR_REPEAT_SEQUENCE(repeat);
}

/** @brief Read the sequence repetition enable state. */
static inline Bool qRegIR_RepeatGet(void)
{
    return (Bool)GP_WB_READ_IR_REPEAT_SEQUENCE();
}

/** @brief Set the RAM sequence pointer/length latching on start. */
static inline void qRegIR_LatchRamSeqOnStartSet(Bool enable)
{
    GP_WB_WRITE_IR_LATCH_RAM_SEQUENCE_START_PTR_AND_LEN_ON_START(enable);
}

/** @brief Read the RAM sequence pointer/length latching on start. */
static inline Bool qRegIR_LatchRamSeqOnStartGet(void)
{
    return (Bool)GP_WB_READ_IR_LATCH_RAM_SEQUENCE_START_PTR_AND_LEN_ON_START();
}

/** @brief Set the RAM sequence pointer/length latching on repeat. */
static inline void qRegIR_LatchRamSeqOnRepeatSet(Bool enable)
{
    GP_WB_WRITE_IR_LATCH_RAM_SEQUENCE_START_PTR_AND_LEN_ON_REPEAT(enable);
}

/** @brief Read the RAM sequence pointer/length latching on repeat. */
static inline Bool qRegIR_LatchRamSeqOnRepeatGet(void)
{
    return (Bool)GP_WB_READ_IR_LATCH_RAM_SEQUENCE_START_PTR_AND_LEN_ON_REPEAT();
}

/** @brief Set the alternate carrier toggling on index match enable state. */
static inline void qRegIR_ToggleAltCarrierSet(Bool enable)
{
    GP_WB_WRITE_IR_TOGGLE_ALT_CARRIER_CONFIG_ON_INDEX_MATCH(enable);
}

/** @brief Read the alternate carrier toggling on index match enable state. */
static inline Bool qRegIR_ToggleAltCarrierGet(void)
{
    return (Bool)GP_WB_READ_IR_TOGGLE_ALT_CARRIER_CONFIG_ON_INDEX_MATCH();
}

/** @brief Set the sequence start pointer. */
static inline void qRegIR_SeqStartPtrSet(UInt16 startPtr)
{
    GP_WB_WRITE_IR_RAM_SEQUENCE_START_PTR(startPtr);
}

/** @brief Read the sequence start pointer. */
static inline UInt16 qRegIR_SeqStartPtrGet(void)
{
    return GP_WB_READ_IR_RAM_SEQUENCE_START_PTR();
}

/** @brief Set the sequence lrngth. */
static inline void qRegIR_SeqLengthSet(UInt8 length)
{
    GP_WB_WRITE_IR_RAM_SEQUENCE_LEN(length);
}

/** @brief Read the sequence lrngth. */
static inline UInt8 qRegIR_SeqLengthGet(void)
{
    return GP_WB_READ_IR_RAM_SEQUENCE_LEN();
}

/** @brief Set the sequence match index. */
static inline void qRegIR_SeqMatchIndexSet(UInt8 index)
{
    GP_WB_WRITE_IR_RAM_SEQUENCE_MATCH_INDEX(index);
}

/** @brief Read the sequence match index. */
static inline UInt8 qRegIR_SeqMatchIndexGet(void)
{
    return GP_WB_READ_IR_RAM_SEQUENCE_MATCH_INDEX();
}

/** @brief Start the RAM sequence. */
static inline void qRegIR_SeqStart(void)
{
    GP_WB_IR_START_RAM_SEQUENCER();
}

/** @brief Set the alternace carrier config. */
static inline void qRegIR_AltCarrierSet(void)
{
    GP_WB_IR_ALT_CARRIER_SET();
}

/** @brief Unset the alternace carrier config. */
static inline void qRegIR_AltCarrierUnset(void)
{
    GP_WB_IR_ALT_CARRIER_UNSET();
}

/** @brief Clear the index match interrupt flag. */
static inline void qRegIR_ClrIndexMatchIrq(void)
{
    GP_WB_IR_CLR_INDEX_MATCH_INTERRUPT();
}

/** @brief Clear the sequence done interrupt flag. */
static inline void qRegIR_ClrSeqDoneIrq(void)
{
    GP_WB_IR_CLR_SEQUENCE_DONE_INTERRUPT();
}

/** @brief Clear the sequence repeat interrupt flag. */
static inline void qRegIR_ClrSeqRepeatIrq(void)
{
    GP_WB_IR_CLR_SEQUENCE_REPEAT_INTERRUPT();
}

/** @brief Clear the sequence start interrupt flag. */
static inline void qRegIR_ClrSeqStartIrq(void)
{
    GP_WB_IR_CLR_SEQUENCE_START_INTERRUPT();
}

/** @brief Read the index match interrupt state. */
static inline Bool qRegIR_IRQUnmaskedIndexMatchGet(void)
{
    return (Bool)GP_WB_READ_IR_UNMASKED_INDEX_MATCH_INTERRUPT();
}

/** @brief Read the sequence done interrupt state. */
static inline Bool qRegIR_IRQUnmaskedSeqDoneGet(void)
{
    return (Bool)GP_WB_READ_IR_UNMASKED_SEQUENCE_DONE_INTERRUPT();
}

/** @brief Read the sequence repeat interrupt state. */
static inline Bool qRegIR_IRQUnmaskedSeqRepeatGet(void)
{
    return (Bool)GP_WB_READ_IR_UNMASKED_SEQUENCE_REPEAT_INTERRUPT();
}

/** @brief Read the sequence start interrupt state. */
static inline Bool qRegIR_IRQUnmaskedSeqStartGet(void)
{
    return (Bool)GP_WB_READ_IR_UNMASKED_SEQUENCE_START_INTERRUPT();
}

/** @brief Read the busy flag state. */
static inline Bool qRegIR_BusyGet(void)
{
    return (Bool)GP_WB_READ_IR_BUSY();
}

/** @brief Read the memory error flag state. */
static inline Bool qRegIR_MemErrGet(void)
{
    return (Bool)GP_WB_READ_IR_MEM_ERR();
}

/** @brief Read the envelope flag state. */
static inline Bool qRegIR_EnvelopeGet(void)
{
    return (Bool)GP_WB_READ_IR_ENVELOPE();
}

/** @brief Read the carrier flag state. */
static inline Bool qRegIR_CarrierGet(void)
{
    return (Bool)GP_WB_READ_IR_CARRIER();
}

/** @brief Read the alternate carrier config state. */
static inline Bool qRegIR_AltGet(void)
{
    return (Bool)GP_WB_READ_IR_USE_ALT_CARRIER_CONFIG();
}

/** @brief Read the current RAM sequence index. */
static inline UInt8 qRegIR_SeqIndexGet(void)
{
    return GP_WB_READ_IR_SEQUENCE_IDX();
}

/** @brief Set the output drive. */
static inline void qRegIR_OutputDriveSet(qRegCommon_Drive_t drive)
{
    GP_WB_WRITE_IR_OUTPUT_DRIVE(drive);
}

/** @brief Read the output drive. */
static inline qRegCommon_Drive_t qRegIR_OutputDriveGet(void)
{
    return (qRegCommon_Drive_t)GP_WB_READ_IR_OUTPUT_DRIVE();
}

/** @brief Set the output inversion. */
static inline void qRegIR_OutputInvertSet(Bool invert)
{
    GP_WB_WRITE_IR_OUTPUT_INVERT(invert);
}

/** @brief Read the output inversion. */
static inline Bool qRegIR_OutputInvertGet(void)
{
    return (Bool)GP_WB_READ_IR_OUTPUT_INVERT();
}

/** @brief Set the input inversion. */
static inline void qRegIR_InputInvertSet(Bool invert)
{
    GP_WB_WRITE_IR_INPUT_INVERT(invert);
}

/** @brief Read the input inversion. */
static inline Bool qRegIR_InputInvertGet(void)
{
    return (Bool)GP_WB_READ_IR_INPUT_INVERT();
}

/** @brief Set the QUIC channel for a specified action index. */
static inline void qRegIR_QUICActionChannelSet(UInt8 actionIdx, UInt8 channel)
{
    UInt32 address = GP_WB_IR_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_IR_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_IR_QUIC_ACTION_CHANNEL_0_MASK, (channel << GP_WB_IR_QUIC_ACTION_CHANNEL_0_LSB));
}

/** @brief Read the QUIC channel for a specified action index. */
static inline UInt8 qRegIR_QUICActionChannelGet(UInt8 actionIdx)
{
    UInt32 address = GP_WB_IR_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_IR_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    return (GP_WB_READ_U32(address) & GP_WB_IR_QUIC_ACTION_CHANNEL_0_MASK) >> GP_WB_IR_QUIC_ACTION_CHANNEL_0_LSB;
}

/** @brief Set the QUIC action type for a specified action index. */
static inline void qRegIR_QUICActionTypeSet(UInt8 actionIdx, qRegIR_QuicActionType_t quicActionType)
{
    UInt32 address = GP_WB_IR_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_IR_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_IR_QUIC_ACTION_TYPE_0_MASK, (quicActionType << GP_WB_IR_QUIC_ACTION_TYPE_0_LSB));
}

/** @brief Read the QUIC action type for a specified action index. */
static inline qRegIR_QuicActionType_t qRegIR_QUICActionTypeGet(UInt8 actionIdx)
{
    UInt32 address = GP_WB_IR_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_IR_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    return (qRegIR_QuicActionType_t)((GP_WB_READ_U32(address) & GP_WB_IR_QUIC_ACTION_TYPE_0_MASK) >>
                                     GP_WB_IR_QUIC_ACTION_TYPE_0_LSB);
}

#ifdef __cplusplus
}
#endif

#endif //_Q_REG_IR_H_
