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

#ifndef _Q_REG_GPADC_H_
#define _Q_REG_GPADC_H_

#include "hal.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get the GPADC Fifo register address. */
#define Q_REG_GPADC_FIFO_ADDRESS_GET() (UInt32)(GP_WB_GP_ADC_FIFO_RESULT_ADDRESS)

/** @brief Get the GPADC slot offset. */
#define Q_REG_GPADC_SLOT_OFFSET (GP_WB_GP_ADC_SLOT_B_CONFIG_ADDRESS - GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS)

/** @brief Get the GPADC action slot offset. */
#define Q_REG_GPADC_QUIC_ACTION_SLOT_OFFSET                                                                            \
    (GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_1_ADDRESS - GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_0_ADDRESS)

/** @brief Get the GPADC preset max value. */
#define Q_REG_GPADC_PRESET_MAX GP_WB_GP_ADC_PRESET_MAX_VALUE_MASK

#define Q_REG_GPADC_FIFO_SUBSAMPLE_RATE_MAX (1UL << GP_WB_GP_ADC_FIFO_SUBSAMPLE_RATE_NBITS)

/** @enum qRegGPADC_TriggerMode_t */
//@{
#define qRegGPADC_TriggerModeNoneOrQuic GP_WB_ENUM_ADC_TRIGGER_MODE_NEVER_OR_QUIC
#define qRegGPADC_TriggerModeContinuous GP_WB_ENUM_ADC_TRIGGER_MODE_CONTINUOUS
/** @typedef qRegGPADC_TriggerMode_t
 *  @brief GPADC trigger mode.
 */
typedef UInt8 qRegGPADC_TriggerMode_t;
//@}

/** @enum qRegGPADC_Slot_t */
//@{
#define qRegGPADC_SlotA 0UL
#define qRegGPADC_SlotB 1UL
#define qRegGPADC_SlotC 2UL
#define qRegGPADC_SlotD 3UL
/** @typedef qRegGPADC_Slot_t
 *  @brief GPADC slot.
 */
typedef UInt8 qRegGPADC_Slot_t;
//@}

/** @enum qRegGPADC_Channel_t */
//@{
#define qRegGPADC_ChannelAnIo0 GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_0
#define qRegGPADC_ChannelAnIo1 GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_1
#define qRegGPADC_ChannelAnIo2 GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_2
#define qRegGPADC_ChannelAnIo3 GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_3
#define qRegGPADC_ChannelNone  Q_NOCHANNEL
/** @typedef qRegGPADC_Channel_t
 *  @brief GPADC channel.
 */
typedef UInt8 qRegGPADC_Channel_t;
//@}

/** @enum qRegGPADC_Buffer_t */
//@{
#define qRegGPADC_BufferA 0x0UL
#define qRegGPADC_BufferB 0x1UL
#define qRegGPADC_BufferC 0x2UL
/** @typedef qRegGPADC_Buffer_t
 *  @brief GPADC buffer.
 */
typedef UInt8 qRegGPADC_Buffer_t;
//@}

/** @enum qRegGPADC_BufferUpdateMode_t */
//@{
#define qRegGPADC_BufferUpdateModeNormal     GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_NORMAL
#define qRegGPADC_BufferUpdateModeMinHold    GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_MIN_HOLD
#define qRegGPADC_BufferUpdateModeMaxHold    GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_MAX_HOLD
#define qRegGPADC_BufferUpdateModeOutOfRange GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_OUT_OF_RANGE
/** @typedef qRegGPADC_BufferUpdateMode_t
 *  @brief GPADC buffer update mode.
 */
typedef UInt8 qRegGPADC_BufferUpdateMode_t;
//@}

/** @enum qRegGPADC_Irq_t */
//@{
#define qRegGPADC_IrqAwdAboveMaximum GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_AWD_ABOVE_MAXIMUM_INTERRUPT_LSB
#define qRegGPADC_IrqAwdBelowMinimum GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_AWD_BELOW_MINIMUM_INTERRUPT_LSB
#define qRegGPADC_IrqBufferAUpdated  GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_BUFFER_A_UPDATED_INTERRUPT_LSB
#define qRegGPADC_IrqBufferBUpdated  GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_BUFFER_B_UPDATED_INTERRUPT_LSB
#define qRegGPADC_IrqBufferCUpdated  GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_BUFFER_C_UPDATED_INTERRUPT_LSB
#define qRegGPADC_IrqCycleDone       GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_CYCLE_DONE_INTERRUPT_LSB
#define qRegGPADC_IrqFifoNotEmpty    GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_FIFO_NOT_EMPTY_INTERRUPT_LSB
#define qRegGPADC_IrqFifoOverrun     GP_WB_INT_CTRL_MASKED_APPUC_GP_ADC_FIFO_OVERRUN_INTERRUPT_LSB
/** @typedef qRegGPADC_Irq_t
 *  @brief GPADC Interrupt.
 */
typedef UInt8 qRegGPADC_Irq_t;
//@}

#if defined(GP_WB_ENUM_ADC_ACTION_NONE)
/** @enum qRegGPADC_QuicActionType_t */
//@{
/** @brief Action none. */
#define qRegGPADC_QuicActionTypeNone GP_WB_ENUM_ADC_ACTION_NONE
/** @brief Action trigger measurement. */
#define qRegGPADC_QuicActionTypeDoMeasurement GP_WB_ENUM_ADC_ACTION_DO_MEASUREMENT
/** @typedef qRegGPADC_QuicActionType_t
 *  @brief GPADC action.
 */
typedef UInt8 qRegGPADC_QuicActionType_t;
//@}
#else
/** @enum qRegGPADC_QuicActionType_t */
//@{
/** @brief Action none. */
#define qRegGPADC_QuicActionTypeNone          GP_WB_ENUM_GP_ADC_ACTION_NONE
/** @brief Action trigger measurement. */
#define qRegGPADC_QuicActionTypeDoMeasurement GP_WB_ENUM_GP_ADC_ACTION_DO_MEASUREMENT
/** @typedef qRegGPADC_QuicActionType_t
 *  @brief GPADC action.
 */
typedef UInt8 qRegGPADC_QuicActionType_t;
//@}
#endif

/** @enum qRegGPADC_ClockDivide_t */
//@{
#define qRegGPADC_ClockDivideBy8 0x00UL
#define qRegGPADC_ClockDivideBy6 0x01UL
#define qRegGPADC_ClockDivideBy5 0x02UL
#define qRegGPADC_ClockDivideBy4 0x03UL
/** @typedef qRegGPADC_ClockDivide_t
 *  @brief GPADC clock divide.
 */
typedef UInt8 qRegGPADC_ClockDivide_t;
//@}

/** @enum qRegGPADC_ClockSelect_t */
//@{
#define qRegGPADC_ClockSelectFractional 0x00UL
#define qRegGPADC_ClockSelectXtal       0x01UL
/** @typedef qRegGPADC_ClockSelect_t
 *  @brief GPADC clock select.
 */
typedef UInt8 qRegGPADC_ClockSelect_t;
//@}

/** @enum qRegGPADC_CFilt_t */
//@{
#define qRegGPADC_CFiltHighest   0x00UL
#define qRegGPADC_CFiltLarge     0x01UL
#define qRegGPADC_CFiltMedium    0x02UL
#define qRegGPADC_CFiltNarrowest 0x03UL
/** @typedef qRegGPADC_CFilt_t
 *  @brief GPADC CFILT bandwidth.
 */
typedef UInt8 qRegGPADC_CFilt_t;
//@}

/** @enum qRegGPADC_OverruleMask_t */
//@{
/** @brief Overruled value for cfilt_sel. */
#define qRegGPADC_OverruleMaskCFiltSel BM(0)
/** @brief Overruled value for mux_smpl1. */
#define qRegGPADC_OverruleMaskMuxSmpl1 BM(1)
/** @brief Overruled value for n_sel. */
#define qRegGPADC_OverruleMaskNSel BM(2)
/** @brief Overruled value for n_sel_pup. */
#define qRegGPADC_OverruleMaskNSelPup BM(3)
/** @brief Overruled value for p_sel. */
#define qRegGPADC_OverruleMaskPSel BM(4)
/** @brief Overruled value for p_sel_pup. */
#define qRegGPADC_OverruleMaskPSelPup BM(5)
/** @brief Overruled value for rc_r_bypass. */
#define qRegGPADC_OverruleMaskRCRBypass BM(6)
/** @brief Overruled value for reset. */
#define qRegGPADC_OverruleMaskReset BM(7)
/** @brief Overruled value for scaler_att1. */
#define qRegGPADC_OverruleMaskScalerAtt1 BM(8)
/** @brief Overruled value for scaler_att2. */
#define qRegGPADC_OverruleMaskScalerAtt2 BM(9)
/** @brief Overruled value for scaler_low_voltage_range. */
#define qRegGPADC_OverruleMaskScalerLowVoltageRange BM(10)
/** @brief Overruled value for scaler_reset. */
#define qRegGPADC_OverruleMaskScalerReset BM(11)
/** @brief Overruled value for scaler_smpl2. */
#define qRegGPADC_OverruleMaskScalerSmpl2 BM(12)
/** @brief Overruled value for scaler_to_vcm. */
#define qRegGPADC_OverruleMaskScalerToVcm BM(13)
/** @brief Overruled value for se_to_de_mode. */
#define qRegGPADC_OverruleMaskSeToDeMode BM(14)
/** @brief Overruled value for smpl. */
#define qRegGPADC_OverruleMaskSmpl BM(15)
/** @typedef qRegGPADC_OverruleMask_t
 *  @brief GPADC overrule mask.
 */
typedef UInt32 qRegGPADC_OverruleMask_t;
//@}

/** @enum qRegGPADC_Mode_t */
//@{
#define qRegGPADC_ModeHighVoltageDifferential 0
#define qRegGPADC_ModeHighVoltageSingleEnded  1
#define qRegGPADC_ModeLowVoltageDifferential  2
#define qRegGPADC_ModeLowVoltageSingleEnded   3
/** @typedef qRegGPADC_Mode_t
 *  @brief GP ADC mode.
 */
typedef UInt8 qRegGPADC_Mode_t;
//@}

/** @brief Set GPADC trigger mode. */
static inline void qRegGPADC_TriggerModeSet(qRegGPADC_TriggerMode_t triggerMode)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_TRIGGER_MODE_ADDRESS, GP_WB_GP_ADC_TRIGGER_MODE_MASK,
                     triggerMode << GP_WB_GP_ADC_TRIGGER_MODE_LSB);
}

/** @brief Get GPADC trigger mode. */
static inline qRegGPADC_TriggerMode_t qRegGPADC_TriggerModeGet(void)
{
    return (
        qRegGPADC_TriggerMode_t)((GP_WB_READ_U32(GP_WB_GP_ADC_TRIGGER_MODE_ADDRESS) & GP_WB_GP_ADC_TRIGGER_MODE_MASK) >>
                                 GP_WB_GP_ADC_TRIGGER_MODE_LSB);
}

/** @brief Set GPADC input mux settle time. */
static inline void qRegGPADC_InputMuxSettleTimeSet(UInt32 settleTime)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_INPUT_MUX_SETTLE_TIME_ADDRESS, GP_WB_GP_ADC_INPUT_MUX_SETTLE_TIME_MASK,
                     settleTime << GP_WB_GP_ADC_INPUT_MUX_SETTLE_TIME_LSB);
}

/** @brief Get GPADC input mux settle time. */
static inline UInt32 qRegGPADC_InputMuxSettleTimeGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_INPUT_MUX_SETTLE_TIME_ADDRESS) & GP_WB_GP_ADC_INPUT_MUX_SETTLE_TIME_MASK) >>
           GP_WB_GP_ADC_INPUT_MUX_SETTLE_TIME_LSB;
}

/** @brief Set GPADC input mux reset time. */
static inline void qRegGPADC_InputMuxResetTimeSet(UInt32 resetTime)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_INPUT_MUX_RESET_TIME_ADDRESS, GP_WB_GP_ADC_INPUT_MUX_RESET_TIME_MASK,
                     resetTime << GP_WB_GP_ADC_INPUT_MUX_RESET_TIME_LSB);
}

/** @brief Get GPADC input mux reset time. */
static inline UInt32 qRegGPADC_InputMuxResetTimeGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_INPUT_MUX_RESET_TIME_ADDRESS) & GP_WB_GP_ADC_INPUT_MUX_RESET_TIME_MASK) >>
           GP_WB_GP_ADC_INPUT_MUX_RESET_TIME_LSB;
}

/** @brief Set GPADC slot mask enable. */
static inline void qRegGPADC_SlotMaskEnableSet(UInt32 slotMask, Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SLOT_ENABLE_ADDRESS, slotMask, (enable ? slotMask : 0UL));
}

/** @brief Get GPADC slot mask enable. */
static inline UInt32 qRegGPADC_SlotMaskEnableGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_SLOT_ENABLE_ADDRESS) & GP_WB_GP_ADC_SLOT_ENABLE_MASK) >>
           GP_WB_GP_ADC_SLOT_ENABLE_LSB;
}

/** @brief Set GPADC slot P channel. */
static inline void qRegGPADC_SlotChannelPSet(qRegGPADC_Slot_t slot, qRegGPADC_Channel_t channel)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_CHANNEL_P_MASK,
                     channel << GP_WB_GP_ADC_SLOT_A_CHANNEL_P_LSB);
}

/** @brief Get GPADC slot P channel. */
static inline qRegGPADC_Channel_t qRegGPADC_SlotChannelPGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (qRegGPADC_Channel_t)((GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_CHANNEL_P_MASK) >>
                                 GP_WB_GP_ADC_SLOT_A_CHANNEL_P_LSB);
}

/** @brief Set GPADC slot P channel pup signal. */
static inline void qRegGPADC_SlotChannelPSelPupSet(qRegGPADC_Slot_t slot, Bool selPup)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_CHANNEL_P_SEL_PUP_MASK,
                     selPup << GP_WB_GP_ADC_SLOT_A_CHANNEL_P_SEL_PUP_LSB);
}

/** @brief Get GPADC slot P channel pup signal. */
static inline Bool qRegGPADC_SlotChannelPSelPupGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (Bool)((GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_CHANNEL_P_SEL_PUP_MASK) >>
                  GP_WB_GP_ADC_SLOT_A_CHANNEL_P_SEL_PUP_LSB);
}

/** @brief Set GPADC slot N channel. */
static inline void qRegGPADC_SlotChannelNSet(qRegGPADC_Slot_t slot, qRegGPADC_Channel_t channel)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_CHANNEL_N_MASK,
                     channel << GP_WB_GP_ADC_SLOT_A_CHANNEL_N_LSB);
}

/** @brief Get GPADC slot N channel. */
static inline qRegGPADC_Channel_t qRegGPADC_SlotChannelNGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (qRegGPADC_Channel_t)((GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_CHANNEL_N_MASK) >>
                                 GP_WB_GP_ADC_SLOT_A_CHANNEL_N_LSB);
}

/** @brief Set GPADC slot channel N pup signal. */
static inline void qRegGPADC_SlotChannelNSelPupSet(qRegGPADC_Slot_t slot, Bool selPup)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_CHANNEL_N_SEL_PUP_MASK,
                     selPup << GP_WB_GP_ADC_SLOT_A_CHANNEL_N_SEL_PUP_LSB);
}

/** @brief Get GPADC slot channel N pup signal. */
static inline Bool qRegGPADC_SlotChannelNSelPupGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (Bool)((GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_CHANNEL_N_SEL_PUP_MASK) >>
                  GP_WB_GP_ADC_SLOT_A_CHANNEL_N_SEL_PUP_LSB);
}

/** @brief Set GPADC slot wait time. */
static inline void qRegGPADC_SlotWaitTimeSet(qRegGPADC_Slot_t slot, UInt32 waitTime)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_SCALER_WAIT_TIME_MASK,
                     waitTime << GP_WB_GP_ADC_SLOT_A_SCALER_WAIT_TIME_LSB);
}

/** @brief Get GPADC slot wait time. */
static inline UInt32 qRegGPADC_SlotWaitTimeGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_SCALER_WAIT_TIME_MASK) >>
           GP_WB_GP_ADC_SLOT_A_SCALER_WAIT_TIME_LSB;
}

/** @brief Set GPADC slot scaler low voltage mode enable. */
static inline void qRegGPADC_SlotLowVoltageModeEnableSet(qRegGPADC_Slot_t slot, Bool enable)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_SCALER_LOW_VOLTAGE_MODE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SLOT_A_SCALER_LOW_VOLTAGE_MODE_LSB);
}

/** @brief Get GPADC slot scaler low voltage mode enable. */
static inline UInt32 qRegGPADC_SlotLowVoltageModeEnableGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_SCALER_LOW_VOLTAGE_MODE_MASK) >>
           GP_WB_GP_ADC_SLOT_A_SCALER_LOW_VOLTAGE_MODE_LSB;
}

/** @brief Set GPADC slot single-ended (SE) mode to differential-ended (DE) mode. */
static inline void qRegGPADC_SlotSeToDeModeEnableSet(qRegGPADC_Slot_t slot, Bool enable)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_SE_TO_DE_MODE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SLOT_A_SE_TO_DE_MODE_LSB);
}

/** @brief Get GPADC slot single-ended (SE) mode to differential-ended (DE) mode. */
static inline UInt32 qRegGPADC_SlotSeToDeModeEnableGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_SE_TO_DE_MODE_MASK) >>
           GP_WB_GP_ADC_SLOT_A_SE_TO_DE_MODE_LSB;
}

/** @brief Set GPADC slot bypass resistor in RC filter. */
static inline void qRegGPADC_SlotBypassResistorEnableSet(qRegGPADC_Slot_t slot, Bool enable)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_RC_R_BYPASS_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SLOT_A_RC_R_BYPASS_LSB);
}

/** @brief Get GPADC slot bypass resistor in RC filter. */
static inline UInt32 qRegGPADC_SlotBypassResistorEnableGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_RC_R_BYPASS_MASK) >>
           GP_WB_GP_ADC_SLOT_A_RC_R_BYPASS_LSB;
}

/** @brief Set GPADC slot input filtering. */
static inline void qRegGPADC_SlotInputFilteringEnableSet(qRegGPADC_Slot_t slot, Bool enable)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_CFILT_SEL_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SLOT_A_CFILT_SEL_LSB);
}

/** @brief Get GPADC slot input filtering. */
static inline UInt32 qRegGPADC_SlotInputFilteringEnableGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_CFILT_SEL_MASK) >>
           GP_WB_GP_ADC_SLOT_A_CFILT_SEL_LSB;
}

/** @brief Set GPADC slot post result to result buffer. */
static inline void qRegGPADC_SlotPostResultToBufEnableSet(qRegGPADC_Slot_t slot, qRegGPADC_Buffer_t buffer, Bool enable)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);
    UInt32 mask = GP_WB_GP_ADC_SLOT_A_POST_TO_BUFFER_A_MASK << (UInt32)buffer;
    UInt32 lsb = GP_WB_GP_ADC_SLOT_A_POST_TO_BUFFER_A_LSB + (UInt32)buffer;

    GP_WB_MWRITE_U32(slotConfigAddress, mask, ((enable ? 1UL : 0UL) << lsb));
}

/** @brief Get GPADC slot post result to result buffer. */
static inline Bool qRegGPADC_SlotPostResultToBufAEnableGet(qRegGPADC_Slot_t slot, qRegGPADC_Buffer_t buffer)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);
    UInt32 mask = GP_WB_GP_ADC_SLOT_A_POST_TO_BUFFER_A_MASK << (UInt32)buffer;
    UInt32 lsb = GP_WB_GP_ADC_SLOT_A_POST_TO_BUFFER_A_LSB + (UInt32)buffer;

    return (Bool)((GP_WB_READ_U32(slotConfigAddress) & mask) >> lsb);
}

/** @brief Set GPADC slot post result to fifo result buffer. */
static inline void qRegGPADC_SlotPostResultToFifoEnableSet(qRegGPADC_Slot_t slot, Bool enable)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_POST_TO_FIFO_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SLOT_A_POST_TO_FIFO_LSB);
}

/** @brief Get GPADC slot post result to fifo result buffer. */
static inline Bool qRegGPADC_SlotPostResultToFifoEnableGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (Bool)((GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_POST_TO_FIFO_MASK) >>
                  GP_WB_GP_ADC_SLOT_A_POST_TO_FIFO_LSB);
}

/** @brief Set GPADC slot post result to analog watchdog. */
static inline void qRegGPADC_SlotPostResultToWdgEnableSet(qRegGPADC_Slot_t slot, Bool enable)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    GP_WB_MWRITE_U32(slotConfigAddress, GP_WB_GP_ADC_SLOT_A_POST_TO_AWD_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SLOT_A_POST_TO_AWD_LSB);
}

/** @brief Set GPADC slot post result to analog watchdog. */
static inline Bool qRegGPADC_SlotPostResultToWdgEnableGet(qRegGPADC_Slot_t slot)
{
    UInt32 slotConfigAddress = GP_WB_GP_ADC_SLOT_A_CONFIG_ADDRESS + ((UInt32)slot * Q_REG_GPADC_SLOT_OFFSET);

    return (Bool)((GP_WB_READ_U32(slotConfigAddress) & GP_WB_GP_ADC_SLOT_A_POST_TO_AWD_MASK) >>
                  GP_WB_GP_ADC_SLOT_A_POST_TO_AWD_LSB);
}

/** @brief Set GPADC preset max value. */
static inline void qRegGPADC_PresetMaxValueSet(UInt32 value)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_PRESET_MAX_VALUE_ADDRESS, GP_WB_GP_ADC_PRESET_MAX_VALUE_MASK,
                     value << GP_WB_GP_ADC_PRESET_MAX_VALUE_LSB);
}

/** @brief Get GPADC preset max value. */
static inline UInt32 qRegGPADC_PresetMaxValueGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_PRESET_MAX_VALUE_ADDRESS) & GP_WB_GP_ADC_PRESET_MAX_VALUE_MASK) >>
           GP_WB_GP_ADC_PRESET_MAX_VALUE_LSB;
}

/** @brief Set GPADC preset min value. */
static inline void qRegGPADC_PresetMinValueSet(UInt32 value)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_PRESET_MIN_VALUE_ADDRESS, GP_WB_GP_ADC_PRESET_MIN_VALUE_MASK,
                     value << GP_WB_GP_ADC_PRESET_MIN_VALUE_LSB);
}

/** @brief Get GPADC preset min value. */
static inline UInt32 qRegGPADC_PresetMinValueGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_PRESET_MIN_VALUE_ADDRESS) & GP_WB_GP_ADC_PRESET_MIN_VALUE_MASK) >>
           GP_WB_GP_ADC_PRESET_MIN_VALUE_LSB;
}

/** @brief Set GPADC fifo subsample rate. */
static inline void qRegGPADC_FifoSubsampleRateSet(UInt32 rate)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_FIFO_SUBSAMPLE_RATE_ADDRESS, GP_WB_GP_ADC_FIFO_SUBSAMPLE_RATE_MASK,
                     rate << GP_WB_GP_ADC_FIFO_SUBSAMPLE_RATE_LSB);
}

/** @brief Get GPADC fifo subsample rate. */
static inline UInt32 qRegGPADC_FifoSubsampleRateGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_FIFO_SUBSAMPLE_RATE_ADDRESS) & GP_WB_GP_ADC_FIFO_SUBSAMPLE_RATE_MASK) >>
           GP_WB_GP_ADC_FIFO_SUBSAMPLE_RATE_LSB;
}

/** @brief Set GPADC fifo 8 bits mode enable. */
static inline void qRegGPADC_FifoMode8BitsEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_FIFO_MODE8BITS_ADDRESS, GP_WB_GP_ADC_FIFO_MODE8BITS_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_FIFO_MODE8BITS_LSB);
}

/** @brief Get GPADC fifo 8 bits mode enable. */
static inline UInt32 qRegGPADC_FifoMode8BitsEnableGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_FIFO_MODE8BITS_ADDRESS) & GP_WB_GP_ADC_FIFO_MODE8BITS_MASK) >>
           GP_WB_GP_ADC_FIFO_MODE8BITS_LSB;
}

/** @brief Set GPADC buffer 8 bits mode enable. */
static inline void qRegGPADC_BufferMode8BitsEnableSet(qRegGPADC_Buffer_t buffer, Bool enable)
{
    UInt32 mask = GP_WB_GP_ADC_BUFFER_A_MODE8BITS_MASK << (3UL * (UInt32)buffer);
    UInt32 lsb = GP_WB_GP_ADC_BUFFER_A_MODE8BITS_LSB + (3UL * (UInt32)buffer);

    GP_WB_MWRITE_U32(GP_WB_GP_ADC_BUFFER_A_MODE8BITS_ADDRESS, mask, ((enable ? 1UL : 0UL) << lsb));
}

/** @brief Get GPADC buffer 8 bits mode enable. */
static inline Bool qRegGPADC_BufferMode8BitsEnableGet(qRegGPADC_Buffer_t buffer)
{
    UInt32 mask = GP_WB_GP_ADC_BUFFER_A_MODE8BITS_MASK << (3UL * (UInt32)buffer);
    UInt32 lsb = GP_WB_GP_ADC_BUFFER_A_MODE8BITS_LSB + (3UL * (UInt32)buffer);

    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_BUFFER_A_MODE8BITS_ADDRESS) & mask) >> lsb);
}

/** @brief Set GPADC buffer update mode. */
static inline void qRegGPADC_BufferUpdateModeSet(qRegGPADC_Buffer_t buffer, qRegGPADC_BufferUpdateMode_t updateMode)
{
    UInt32 mask = GP_WB_GP_ADC_BUFFER_A_UPDATE_MODE_MASK << (3UL * (UInt32)buffer);
    UInt32 lsb = GP_WB_GP_ADC_BUFFER_A_UPDATE_MODE_LSB + (3UL * (UInt32)buffer);

    GP_WB_MWRITE_U32(GP_WB_GP_ADC_BUFFER_A_UPDATE_MODE_ADDRESS, mask, (updateMode << lsb));
}

/** @brief Get GPADC buffer update mode. */
static inline qRegGPADC_BufferUpdateMode_t qRegGPADC_BufferUpdateModeGet(qRegGPADC_Buffer_t buffer)
{
    UInt32 mask = GP_WB_GP_ADC_BUFFER_A_UPDATE_MODE_MASK << (3UL * (UInt32)buffer);
    UInt32 lsb = GP_WB_GP_ADC_BUFFER_A_UPDATE_MODE_LSB + (3UL * (UInt32)buffer);

    return (qRegGPADC_BufferUpdateMode_t)((GP_WB_READ_U32(GP_WB_GP_ADC_BUFFER_A_UPDATE_MODE_ADDRESS) & mask) >> lsb);
}

/** @brief Set GPADC buffer preset. */
static inline void qRegGPADC_BufferPresetSet(qRegGPADC_Buffer_t buffer)
{
    UInt32 mask = GP_WB_GP_ADC_BUFFER_A_PRESET_MASK << (UInt32)buffer;

    GP_WB_WRITE_U32(GP_WB_GP_ADC_BUFFER_A_PRESET_ADDRESS, mask);
}

/** @brief Set GPADC ADW preset. */
static inline void qRegGPADC_ADWPresetSet(void)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_AWD_PRESET_ADDRESS, GP_WB_GP_ADC_AWD_PRESET_MASK, GP_WB_GP_ADC_AWD_PRESET_MASK);
}

/** @brief Clear GPADC fifo subsample counter. */
static inline void qRegGPADC_FifoSubsampleCounterClear(void)
{
    GP_WB_WRITE_U32(GP_WB_GP_ADC_CLR_FIFO_SUBSAMPLE_CNT_ADDRESS, GP_WB_GP_ADC_CLR_FIFO_SUBSAMPLE_CNT_MASK);
}

/** @brief Clear GPADC buffer updated interrupt. */
static inline void qRegGPADC_IRQBufferUpdatedClear(qRegGPADC_Buffer_t buffer)
{
    UInt32 mask = GP_WB_GP_ADC_CLR_BUFFER_A_UPDATED_INTERRUPT_MASK << (UInt32)buffer;

    GP_WB_WRITE_U32(GP_WB_GP_ADC_CLR_INTERRUPTS_ADDRESS, mask);
}

/** @brief Clear GPADC fifo overrun interrupt. */
static inline void qRegGPADC_IRQFifoOverrunClear(void)
{
    GP_WB_WRITE_U32(GP_WB_GP_ADC_CLR_INTERRUPTS_ADDRESS, GP_WB_GP_ADC_CLR_FIFO_OVERRUN_INTERRUPT_MASK);
}

/** @brief Clear GPADC ADW below minimum interrupt. */
static inline void qRegGPADC_IRQAdwBelowMinimumClear(void)
{
    GP_WB_WRITE_U32(GP_WB_GP_ADC_CLR_INTERRUPTS_ADDRESS, GP_WB_GP_ADC_CLR_AWD_BELOW_MINIMUM_INTERRUPT_MASK);
}

/** @brief Clear GPADC ADW above maximum interrupt. */
static inline void qRegGPADC_IRQAdwAboveMaximumClear(void)
{
    GP_WB_WRITE_U32(GP_WB_GP_ADC_CLR_INTERRUPTS_ADDRESS, GP_WB_GP_ADC_CLR_AWD_ABOVE_MAXIMUM_INTERRUPT_MASK);
}

/** @brief Clear GPADC cycle done interrupt. */
static inline void qRegGPADC_IRQCycleDoneClear(void)
{
    GP_WB_WRITE_U32(GP_WB_GP_ADC_CLR_INTERRUPTS_ADDRESS, GP_WB_GP_ADC_CLR_CYCLE_DONE_INTERRUPT_MASK);
}

/** @brief Get GPADC fifo result. */
static inline UInt16 qRegGPADC_FifoResultGet(void)
{
    return GP_WB_READ_U32(GP_WB_GP_ADC_FIFO_RESULT_ADDRESS);
}

/** @brief Get GPADC buffer result. */
static inline UInt16 qRegGPADC_BufferResultGet(qRegGPADC_Buffer_t buffer)
{
    UInt32 address = GP_WB_GP_ADC_BUFFER_A_RESULT_ADDRESS + (sizeof(UInt32) * (UInt32)buffer);

    return GP_WB_READ_U32(address);
}

/** @brief Get GPADC active slot. */
static inline qRegGPADC_Slot_t qRegGPADC_ActiveSlotGet(void)
{
    return (
        qRegGPADC_Slot_t)((GP_WB_READ_U32(GP_WB_GP_ADC_ACTIVE_SLOT_IDX_ADDRESS) & GP_WB_GP_ADC_ACTIVE_SLOT_IDX_MASK) >>
                          GP_WB_GP_ADC_ACTIVE_SLOT_IDX_LSB);
}

/** @brief Get GPADC busy status. */
static inline Bool qRegGPADC_BusyGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_CONVERSION_CYCLE_BUSY_ADDRESS) & GP_WB_GP_ADC_CONVERSION_CYCLE_BUSY_MASK) >>
              GP_WB_GP_ADC_CONVERSION_CYCLE_BUSY_LSB);
}

/** @brief Set GPADC QUIC action channel. */
static inline void qRegGPADC_QuicActionChannelSet(UInt8 actionIndex, UInt32 channel)
{
    UInt32 address = GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_GPADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    GP_WB_MWRITE_U32(address, GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_0_MASK,
                     channel << GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_0_LSB);
}

/** @brief Get GPADC QUIC action channel. */
static inline UInt32 qRegGPADC_QuicActionChannelGet(UInt8 actionIndex)
{
    UInt32 address = GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_GPADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    return (GP_WB_READ_U32(address) & GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_0_MASK) >>
           GP_WB_GP_ADC_QUIC_ACTION_CHANNEL_0_LSB;
}

/** @brief Set GPADC QUIC action type. */
static inline void qRegGPADC_QuicActionTypeSet(UInt8 actionIndex, qRegGPADC_QuicActionType_t quicActionType)
{
    UInt32 address = GP_WB_GP_ADC_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_GPADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    GP_WB_MWRITE_U32(address, GP_WB_GP_ADC_QUIC_ACTION_TYPE_0_MASK,
                     quicActionType << GP_WB_GP_ADC_QUIC_ACTION_TYPE_0_LSB);
}

/** @brief Get GPADC QUIC action type. */
static inline qRegGPADC_QuicActionType_t qRegGPADC_QuicActionTypeGet(UInt8 actionIndex)
{
    UInt32 address = GP_WB_GP_ADC_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_GPADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    return (qRegGPADC_QuicActionType_t)((GP_WB_READ_U32(address) & GP_WB_GP_ADC_QUIC_ACTION_TYPE_0_MASK) >>
                                        GP_WB_GP_ADC_QUIC_ACTION_TYPE_0_LSB);
}

/** @brief Set GPADC reference tune bits for analog supply LDO. */
static inline void qRegGPADC_LdoRefBitsSet(UInt8 refBits)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_LDO_REFBITS_ADDRESS, GP_WB_GP_ADC_LDO_REFBITS_MASK,
                     (UInt32)refBits << GP_WB_GP_ADC_LDO_REFBITS_LSB);
}

/** @brief Get GPADC reference tune bits for analog supply LDO. */
static inline UInt8 qRegGPADC_LdoRefBitsGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_LDO_REFBITS_ADDRESS) & GP_WB_GP_ADC_LDO_REFBITS_MASK) >>
           GP_WB_GP_ADC_LDO_REFBITS_LSB;
}

/** @brief Set GPADC clock divider. */
static inline void qRegGPADC_ClockDivideSet(qRegGPADC_ClockDivide_t clockDivide)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_CLKDIV_SD_ADC_SPEED_ADDRESS, GP_WB_GP_ADC_CLKDIV_SD_ADC_SPEED_MASK,
                     (UInt32)clockDivide << GP_WB_GP_ADC_CLKDIV_SD_ADC_SPEED_LSB);
}

/** @brief Get GPADC clock divider. */
static inline qRegGPADC_ClockDivide_t qRegGPADC_ClockDivideGet(void)
{
    return (qRegGPADC_ClockDivide_t)((GP_WB_READ_U32(GP_WB_GP_ADC_CLKDIV_SD_ADC_SPEED_ADDRESS) &
                                      GP_WB_GP_ADC_CLKDIV_SD_ADC_SPEED_MASK) >>
                                     GP_WB_GP_ADC_CLKDIV_SD_ADC_SPEED_LSB);
}

/** @brief Set GPADC fractional clock from digital for audio ADC. */
static inline void qRegGPADC_FracClockEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_CLK_FRAC_SD_ADC_ADDRESS, GP_WB_GP_ADC_CLK_FRAC_SD_ADC_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_CLK_FRAC_SD_ADC_LSB);
}

/** @brief Get GPADC fractional clock from digital for audio ADC. */
static inline Bool qRegGPADC_FracClockEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_CLK_FRAC_SD_ADC_ADDRESS) & GP_WB_GP_ADC_CLK_FRAC_SD_ADC_MASK) >>
                  GP_WB_GP_ADC_CLK_FRAC_SD_ADC_LSB);
}

/** @brief Set GPADC reference tune bits. */
static inline void qRegGPADC_RefBitsSet(UInt8 refBits)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_REF_REFBITS_ADDRESS, GP_WB_GP_ADC_REF_REFBITS_MASK,
                     (UInt32)refBits << GP_WB_GP_ADC_REF_REFBITS_LSB);
}

/** @brief Get GPADC reference tune bits. */
static inline UInt8 qRegGPADC_RefBitsGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_REF_REFBITS_ADDRESS) & GP_WB_GP_ADC_REF_REFBITS_MASK) >>
           GP_WB_GP_ADC_REF_REFBITS_LSB;
}

/** @brief Set GPADC adjust delay in comparator for settling time. */
static inline void qRegGPADC_AdjDelayCompSet(UInt8 delayComp)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_COMP_DELAY_SEL_ADDRESS, GP_WB_GP_ADC_COMP_DELAY_SEL_MASK,
                     (UInt32)delayComp << GP_WB_GP_ADC_COMP_DELAY_SEL_LSB);
}

/** @brief Get GPADC adjust delay in comparator for settling time. */
static inline UInt8 qRegGPADC_AdjDelayCompGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_COMP_DELAY_SEL_ADDRESS) & GP_WB_GP_ADC_COMP_DELAY_SEL_MASK) >>
           GP_WB_GP_ADC_COMP_DELAY_SEL_LSB;
}

/** @brief Set GPADC enable clock divider for SD-ADCs. */
static inline void qRegGPADC_ClockDividerEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_CLKDIV_SD_ADC_PUP_ADDRESS, GP_WB_GP_ADC_CLKDIV_SD_ADC_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_CLKDIV_SD_ADC_PUP_LSB);
}

/** @brief Get GPADC enable clock divider for SD-ADCs. */
static inline Bool qRegGPADC_ClockDividerEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_CLKDIV_SD_ADC_PUP_ADDRESS) & GP_WB_GP_ADC_CLKDIV_SD_ADC_PUP_MASK) >>
                  GP_WB_GP_ADC_CLKDIV_SD_ADC_PUP_LSB);
}

/** @brief Set GPADC current reference reference tune bits. */
static inline void qRegGPADC_IrefBitsSet(UInt8 refBits)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_IREF_REFBITS_ADDRESS, GP_WB_GP_ADC_IREF_REFBITS_MASK,
                     (UInt32)refBits << GP_WB_GP_ADC_IREF_REFBITS_LSB);
}

/** @brief Get GPADC current reference reference tune bits. */
static inline UInt8 qRegGPADC_IrefBitsGHet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_IREF_REFBITS_ADDRESS) & GP_WB_GP_ADC_IREF_REFBITS_MASK) >>
           GP_WB_GP_ADC_IREF_REFBITS_LSB;
}

/** @brief Set GPADC different mappings for error estimation. */
static inline void qRegGPADC_ErrorEstMapSet(UInt8 estMap)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_ERROR_EST_MAP_SEL_ADDRESS, GP_WB_GP_ADC_ERROR_EST_MAP_SEL_MASK,
                     (UInt32)estMap << GP_WB_GP_ADC_ERROR_EST_MAP_SEL_LSB);
}

/** @brief Get GPADC different mappings for error estimation. */
static inline UInt8 qRegGPADC_ErrorEstMapGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_ERROR_EST_MAP_SEL_ADDRESS) & GP_WB_GP_ADC_ERROR_EST_MAP_SEL_MASK) >>
           GP_WB_GP_ADC_ERROR_EST_MAP_SEL_LSB;
}

/** @brief Set GPADC clock select. */
static inline void qRegGPADC_ClockSelectSet(qRegGPADC_ClockSelect_t clockSelect)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_CLKDIV_SD_ADC_SELECT_ADDRESS, GP_WB_GP_ADC_CLKDIV_SD_ADC_SELECT_MASK,
                     (UInt32)clockSelect << GP_WB_GP_ADC_CLKDIV_SD_ADC_SELECT_LSB);
}

/** @brief Get GPADC clock select. */
static inline qRegGPADC_ClockSelect_t qRegGPADC_ClockSelectGet(void)
{
    return (qRegGPADC_ClockSelect_t)((GP_WB_READ_U32(GP_WB_GP_ADC_CLKDIV_SD_ADC_SELECT_ADDRESS) &
                                      GP_WB_GP_ADC_CLKDIV_SD_ADC_SELECT_MASK) >>
                                     GP_WB_GP_ADC_CLKDIV_SD_ADC_SELECT_LSB);
}

/** @brief Set GPADC sensor macro level reference voltage calibration. Calibrated top voltage to 800 mV. */
static inline void qRegGPADC_ResCalSet(UInt8 estMap)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_RES_CAL_ADDRESS, GP_WB_GP_ADC_RES_CAL_MASK,
                     (UInt32)estMap << GP_WB_GP_ADC_RES_CAL_LSB);
}

/** @brief Get GPADC sensor macro level reference voltage calibration. Calibrated top voltage to 800 mV. */
static inline UInt8 qRegGPADC_ResCalGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_RES_CAL_ADDRESS) & GP_WB_GP_ADC_RES_CAL_MASK) >> GP_WB_GP_ADC_RES_CAL_LSB;
}

/** @brief Set GPADC enable error estimation for reduced noise. */
static inline void qRegGPADC_ErrorEstEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_ERROR_EST_EN_ADDRESS, GP_WB_GP_ADC_ERROR_EST_EN_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_ERROR_EST_EN_LSB);
}

/** @brief Get GPADC enable error estimation for reduced noise. */
static inline Bool qRegGPADC_ErrorEstEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_ERROR_EST_EN_ADDRESS) & GP_WB_GP_ADC_ERROR_EST_EN_MASK) >>
                  GP_WB_GP_ADC_ERROR_EST_EN_LSB);
}

/** @brief Set GPADC enable bleed for analog supply LDO. */
static inline void qRegGPADC_LdoBleedEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_LDO_BLEED_ADDRESS, GP_WB_GP_ADC_LDO_BLEED_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_LDO_BLEED_LSB);
}

/** @brief Get GPADC enable bleed for analog supply LDO. */
static inline Bool qRegGPADC_LdoBleedEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_LDO_BLEED_ADDRESS) & GP_WB_GP_ADC_LDO_BLEED_MASK) >>
                  GP_WB_GP_ADC_LDO_BLEED_LSB);
}

/** @brief Set GPADC enable current monitor GPADC LDO. */
static inline void qRegGPADC_LdoIMonEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_LDO_IMON_EN_ADDRESS, GP_WB_GP_ADC_LDO_IMON_EN_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_LDO_IMON_EN_LSB);
}

/** @brief Get GPADC enable current monitor GPADC LDO. */
static inline Bool qRegGPADC_LdoIMonEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_LDO_IMON_EN_ADDRESS) & GP_WB_GP_ADC_LDO_IMON_EN_MASK) >>
                  GP_WB_GP_ADC_LDO_IMON_EN_LSB);
}

/** @brief Set GPADC enable analog supply LDO. */
static inline void qRegGPADC_LdoAnalogSupplyEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_LDO_PUP_ADDRESS, GP_WB_GP_ADC_LDO_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_LDO_PUP_LSB);
}

/** @brief Get GPADC enable analog supply LDO. */
static inline Bool qRegGPADC_LdoAnalogSupplyEnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_LDO_PUP_ADDRESS) & GP_WB_GP_ADC_LDO_PUP_MASK) >> GP_WB_GP_ADC_LDO_PUP_LSB);
}

/** @brief Set GPADC enable power-up. */
static inline void qRegGPADC_PowerUpEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_PUP_ADDRESS, GP_WB_GP_ADC_PUP_MASK, (enable ? 1UL : 0UL) << GP_WB_GP_ADC_PUP_LSB);
}

/** @brief Get GPADC enable power-up. */
static inline Bool qRegGPADC_PowerUpEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_PUP_ADDRESS) & GP_WB_GP_ADC_PUP_MASK) >> GP_WB_GP_ADC_PUP_LSB);
}

/** @brief Set GPADC enable VCM buffer power-up. */
static inline void qRegGPADC_VcmBufferPowerUpEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_VCMBUF_PUP_ADDRESS, GP_WB_GP_ADC_VCMBUF_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_VCMBUF_PUP_LSB);
}

/** @brief Get GPADC enable VCM buffer power-up. */
static inline Bool qRegGPADC_VcmBufferPowerUpEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_VCMBUF_PUP_ADDRESS) & GP_WB_GP_ADC_VCMBUF_PUP_MASK) >>
                  GP_WB_GP_ADC_VCMBUF_PUP_LSB);
}

/** @brief Set GPADC enable Vref buffer circuit. */
static inline void qRegGPADC_VrefBufferCircuitEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_VREF_PUP_ADDRESS, GP_WB_GP_ADC_VREF_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_VREF_PUP_LSB);
}

/** @brief Get GPADC enable Vref buffer circuit. */
static inline Bool qRegGPADC_VrefBufferCircuitEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_VREF_PUP_ADDRESS) & GP_WB_GP_ADC_VREF_PUP_MASK) >>
                  GP_WB_GP_ADC_VREF_PUP_LSB);
}

/** @brief Set GPADC enable current reference block power-up. */
static inline void qRegGPADC_IRefPowerUpEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_IREF_PUP_ADDRESS, GP_WB_GP_ADC_IREF_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_IREF_PUP_LSB);
}

/** @brief Get GPADC enable current reference block power-up. */
static inline Bool qRegGPADC_IRefPowerUpEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_IREF_PUP_ADDRESS) & GP_WB_GP_ADC_IREF_PUP_MASK) >>
                  GP_WB_GP_ADC_IREF_PUP_LSB);
}

/** @brief Set GPADC enable increases reference for HTOL usage. */
static inline void qRegGPADC_IncRefHtolEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_MACRO_HTOL_ADDRESS, GP_WB_GP_ADC_MACRO_HTOL_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_MACRO_HTOL_LSB);
}

/** @brief Get GPADC enable increases reference for HTOL usage. */
static inline Bool qRegGPADC_IncRefHtolEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_MACRO_HTOL_ADDRESS) & GP_WB_GP_ADC_MACRO_HTOL_MASK) >>
                  GP_WB_GP_ADC_MACRO_HTOL_LSB);
}

/** @brief Set GPADC enable bleed current for macro LDO. */
static inline void qRegGPADC_LdoBleedCurrentEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_MACRO_LDO_BLEED_ADDRESS, GP_WB_GP_ADC_MACRO_LDO_BLEED_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_MACRO_LDO_BLEED_LSB);
}

/** @brief Get GPADC enable bleed current for macro LDO. */
static inline Bool qRegGPADC_LdoBleedCurrentEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_MACRO_LDO_BLEED_ADDRESS) & GP_WB_GP_ADC_MACRO_LDO_BLEED_MASK) >>
                  GP_WB_GP_ADC_MACRO_LDO_BLEED_LSB);
}

/** @brief Set GPADC enable current monitor macro LDO. */
static inline void qRegGPADC_LdoCurrentMonitorEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_MACRO_LDO_IMON_EN_ADDRESS, GP_WB_GP_ADC_MACRO_LDO_IMON_EN_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_MACRO_LDO_IMON_EN_LSB);
}

/** @brief Get GPADC enable current monitor macro LDO. */
static inline Bool qRegGPADC_LdoCurrentMonitorEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_MACRO_LDO_IMON_EN_ADDRESS) & GP_WB_GP_ADC_MACRO_LDO_IMON_EN_MASK) >>
                  GP_WB_GP_ADC_MACRO_LDO_IMON_EN_LSB);
}

/** @brief Set GPADC enable LDO power-up. */
static inline void qRegGPADC_LdoPowerUpEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_MACRO_LDO_PUP_ADDRESS, GP_WB_GP_ADC_MACRO_LDO_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_MACRO_LDO_PUP_LSB);
}

/** @brief Get GPADC enable LDO power-up. */
static inline Bool qRegGPADC_LdoPowerUpEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_MACRO_LDO_PUP_ADDRESS) & GP_WB_GP_ADC_MACRO_LDO_PUP_MASK) >>
                  GP_WB_GP_ADC_MACRO_LDO_PUP_LSB);
}

/** @brief Get GPADC result. */
static inline UInt16 qRegGPADC_ResultGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_RESULT_ADDRESS) & GP_WB_GP_ADC_RESULT_MASK) >> GP_WB_GP_ADC_RESULT_LSB;
}

/** @brief Get GPADC end of conversion status. */
static inline Bool qRegGPADC_EndOfConversionGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_EOC_ADDRESS) & GP_WB_GP_ADC_EOC_MASK) >> GP_WB_GP_ADC_EOC_LSB);
}

/** @brief Set GPADC overruled negative input pin. */
static inline void qRegGPADC_OverruleNPinSet(UInt8 nSel)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_N_SEL_OVERRULE_ADDRESS, GP_WB_GP_ADC_N_SEL_OVERRULE_MASK,
                     (UInt32)nSel << GP_WB_GP_ADC_N_SEL_OVERRULE_LSB);
}

/** @brief Get GPADC overruled negative input pin. */
static inline UInt8 qRegGPADC_OverruleNPinGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_N_SEL_OVERRULE_ADDRESS) & GP_WB_GP_ADC_N_SEL_OVERRULE_MASK) >>
           GP_WB_GP_ADC_N_SEL_OVERRULE_LSB;
}

/** @brief Set GPADC overruled positive input pin. */
static inline void qRegGPADC_OverrulePPinSet(UInt8 pSel)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_P_SEL_OVERRULE_ADDRESS, GP_WB_GP_ADC_P_SEL_OVERRULE_MASK,
                     (UInt32)pSel << GP_WB_GP_ADC_P_SEL_OVERRULE_LSB);
}

/** @brief Get GPADC overruled positive input pin. */
static inline UInt8 qRegGPADC_OverrulePPinGet(void)
{
    return (GP_WB_READ_U32(GP_WB_GP_ADC_P_SEL_OVERRULE_ADDRESS) & GP_WB_GP_ADC_P_SEL_OVERRULE_MASK) >>
           GP_WB_GP_ADC_P_SEL_OVERRULE_LSB;
}

/** @brief Set GPADC overruled value for CFILT. */
static inline void qRegGPADC_OverruleCFiltSet(qRegGPADC_CFilt_t cFilt)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_CFILT_SEL_OVERRULE_ADDRESS, GP_WB_GP_ADC_CFILT_SEL_OVERRULE_MASK,
                     (UInt32)cFilt << GP_WB_GP_ADC_CFILT_SEL_OVERRULE_LSB);
}

/** @brief Get GPADC overruled value for CFILT. */
static inline qRegGPADC_CFilt_t qRegGPADC_OverruleCFiltGet(void)
{
    return (qRegGPADC_CFilt_t)((GP_WB_READ_U32(GP_WB_GP_ADC_CFILT_SEL_OVERRULE_ADDRESS) &
                                GP_WB_GP_ADC_CFILT_SEL_OVERRULE_MASK) >>
                               GP_WB_GP_ADC_CFILT_SEL_OVERRULE_LSB);
}

/** @brief Set GPADC overruled value for MUX SMPL1 enable. */
static inline void qRegGPADC_OverruleMuxSmpl1EnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_MUX_SMPL1_OVERRULE_ADDRESS, GP_WB_GP_ADC_MUX_SMPL1_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_MUX_SMPL1_OVERRULE_LSB);
}

/** @brief Get GPADC overruled value for MUX SMPL1 enable. */
static inline Bool qRegGPADC_OverruleMuxSmpl1EnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_MUX_SMPL1_OVERRULE_ADDRESS) & GP_WB_GP_ADC_MUX_SMPL1_OVERRULE_MASK) >>
                  GP_WB_GP_ADC_MUX_SMPL1_OVERRULE_LSB);
}

/** @brief Set GPADC overruled enable for negative input selection. */
static inline void qRegGPADC_OverruleNSelEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_N_SEL_PUP_OVERRULE_ADDRESS, GP_WB_GP_ADC_N_SEL_PUP_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_N_SEL_PUP_OVERRULE_LSB);
}

/** @brief Get GPADC overruled enable for negative input selection. */
static inline Bool qRegGPADC_OverruleNSelEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_N_SEL_PUP_OVERRULE_ADDRESS) & GP_WB_GP_ADC_N_SEL_PUP_OVERRULE_MASK) >>
                  GP_WB_GP_ADC_N_SEL_PUP_OVERRULE_LSB);
}

/** @brief Set GPADC overruled enable for positive input selection. */
static inline void qRegGPADC_OverrulePSelEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_P_SEL_PUP_OVERRULE_ADDRESS, GP_WB_GP_ADC_P_SEL_PUP_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_P_SEL_PUP_OVERRULE_LSB);
}

/** @brief Get GPADC overruled enable for positive input selection. */
static inline Bool qRegGPADC_OverrulePSelEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_P_SEL_PUP_OVERRULE_ADDRESS) & GP_WB_GP_ADC_P_SEL_PUP_OVERRULE_MASK) >>
                  GP_WB_GP_ADC_P_SEL_PUP_OVERRULE_LSB);
}

/** @brief Set GPADC overruled RC bypass enable. */
static inline void qRegGPADC_OverruleRCBypassEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_RC_R_BYPASS_OVERRULE_ADDRESS, GP_WB_GP_ADC_RC_R_BYPASS_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_RC_R_BYPASS_OVERRULE_LSB);
}

/** @brief Get GPADC overruled RC bypass enable. */
static inline Bool qRegGPADC_OverruleRCBypassEnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_RC_R_BYPASS_OVERRULE_ADDRESS) & GP_WB_GP_ADC_RC_R_BYPASS_OVERRULE_MASK) >>
              GP_WB_GP_ADC_RC_R_BYPASS_OVERRULE_LSB);
}

/** @brief Set GPADC overruled value for reset enable. */
static inline void qRegGPADC_OverruleResetEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_RESET_OVERRULE_ADDRESS, GP_WB_GP_ADC_RESET_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_RESET_OVERRULE_LSB);
}

/** @brief Get GPADC overruled value for reset enable. */
static inline Bool qRegGPADC_OverruleResetEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_RESET_OVERRULE_ADDRESS) & GP_WB_GP_ADC_RESET_OVERRULE_MASK) >>
                  GP_WB_GP_ADC_RESET_OVERRULE_LSB);
}

/** @brief Set GPADC overruled first attenuation cap of scaler enable. */
static inline void qRegGPADC_OverruleAtt1EnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SCALER_ATT1_OVERRULE_ADDRESS, GP_WB_GP_ADC_SCALER_ATT1_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SCALER_ATT1_OVERRULE_LSB);
}

/** @brief Get GPADC overruled first attenuation cap of scaler enable. */
static inline Bool qRegGPADC_OverruleAtt1EnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_SCALER_ATT1_OVERRULE_ADDRESS) & GP_WB_GP_ADC_SCALER_ATT1_OVERRULE_MASK) >>
              GP_WB_GP_ADC_SCALER_ATT1_OVERRULE_LSB);
}

/** @brief Set GPADC overruled second attenuation cap of scaler enable. */
static inline void qRegGPADC_OverruleAtt2EnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SCALER_ATT2_OVERRULE_ADDRESS, GP_WB_GP_ADC_SCALER_ATT2_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SCALER_ATT2_OVERRULE_LSB);
}

/** @brief Get GPADC overruled second attenuation cap of scaler enable. */
static inline Bool qRegGPADC_OverruleAtt2EnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_SCALER_ATT2_OVERRULE_ADDRESS) & GP_WB_GP_ADC_SCALER_ATT2_OVERRULE_MASK) >>
              GP_WB_GP_ADC_SCALER_ATT2_OVERRULE_LSB);
}

/** @brief Set GPADC overruled low voltage range for scaler enable. */
static inline void qRegGPADC_OverruleLowVoltageRangeEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SCALER_LOW_VOLTAGE_RANGE_OVERRULE_ADDRESS,
                     GP_WB_GP_ADC_SCALER_LOW_VOLTAGE_RANGE_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SCALER_LOW_VOLTAGE_RANGE_OVERRULE_LSB);
}

/** @brief Get GPADC overruled low voltage range for scaler enable. */
static inline Bool qRegGPADC_OverruleLowVoltageRangeEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_SCALER_LOW_VOLTAGE_RANGE_OVERRULE_ADDRESS) &
                   GP_WB_GP_ADC_SCALER_LOW_VOLTAGE_RANGE_OVERRULE_MASK) >>
                  GP_WB_GP_ADC_SCALER_LOW_VOLTAGE_RANGE_OVERRULE_LSB);
}

/** @brief Set GPADC overruled reset caps before new sample enable. */
static inline void qRegGPADC_OverruleResetCapsEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SCALER_RESET_OVERRULE_ADDRESS, GP_WB_GP_ADC_SCALER_RESET_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SCALER_RESET_OVERRULE_LSB);
}

/** @brief Get GPADC overruled reset caps before new sample enable. */
static inline Bool qRegGPADC_OverruleResetCapsEnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_SCALER_RESET_OVERRULE_ADDRESS) & GP_WB_GP_ADC_SCALER_RESET_OVERRULE_MASK) >>
              GP_WB_GP_ADC_SCALER_RESET_OVERRULE_LSB);
}

/** @brief Set GPADC overruled value of last sample stage before new sample enable. */
static inline void qRegGPADC_OverruleSmpl2EnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SCALER_SMPL2_OVERRULE_ADDRESS, GP_WB_GP_ADC_SCALER_SMPL2_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SCALER_SMPL2_OVERRULE_LSB);
}

/** @brief Get GPADC overruled value of last sample stage before new sample enable. */
static inline Bool qRegGPADC_OverruleSmpl2EnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_SCALER_SMPL2_OVERRULE_ADDRESS) & GP_WB_GP_ADC_SCALER_SMPL2_OVERRULE_MASK) >>
              GP_WB_GP_ADC_SCALER_SMPL2_OVERRULE_LSB);
}

/** @brief Set GPADC overruled reset last scaler cap to VCM enable. */
static inline void qRegGPADC_OverruleVCMEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SCALER_TO_VCM_OVERRULE_ADDRESS, GP_WB_GP_ADC_SCALER_TO_VCM_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SCALER_TO_VCM_OVERRULE_LSB);
}

/** @brief Get GPADC overruled reset last scaler cap to VCM enable. */
static inline Bool qRegGPADC_OverruleVCMEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_SCALER_TO_VCM_OVERRULE_ADDRESS) &
                   GP_WB_GP_ADC_SCALER_TO_VCM_OVERRULE_MASK) >>
                  GP_WB_GP_ADC_SCALER_TO_VCM_OVERRULE_LSB);
}

/** @brief Set GPADC overruled single-ended (SE) mode to differential-ended (DE) mode. */
static inline void qRegGPADC_OverruleSeToDeModeEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_SE_TO_DE_MODE_OVERRULE_ADDRESS, GP_WB_GP_ADC_SE_TO_DE_MODE_OVERRULE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_GP_ADC_SE_TO_DE_MODE_OVERRULE_LSB);
}

/** @brief Get GPADC overruled single-ended (SE) mode to differential-ended (DE) mode. */
static inline Bool qRegGPADC_OverruleSeToDeModeEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_GP_ADC_SE_TO_DE_MODE_OVERRULE_ADDRESS) &
                   GP_WB_GP_ADC_SE_TO_DE_MODE_OVERRULE_MASK) >>
                  GP_WB_GP_ADC_SE_TO_DE_MODE_OVERRULE_LSB);
}

/** @brief Set GPADC enable status of overrule. */
static inline void qRegGPADC_OverruleEnableSet(qRegGPADC_OverruleMask_t overruleMask, Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_GP_ADC_OVERRULE_ENA_0_ADDRESS, overruleMask & GP_WB_GP_ADC_OVERRULE_ENA_0_MASK,
                     enable ? GP_WB_GP_ADC_OVERRULE_ENA_0_MASK : 0);
}

/** @brief Get GPADC enable status of overrule. */
static inline qRegGPADC_OverruleMask_t qRegGPADC_OverruleEnableGet(void)
{
    return (qRegGPADC_OverruleMask_t)(GP_WB_READ_U32(GP_WB_GP_ADC_OVERRULE_ENA_0_ADDRESS) &
                                      GP_WB_GP_ADC_OVERRULE_ENA_0_MASK);
}

/** @brief Get GPADC calibrated gain used to convert result value into microvolt. */
static inline UInt32 qRegGPADC_CalibratedGainGet(qRegGPADC_Mode_t mode, UInt32 calPtr)
{
    switch(mode)
    {
        case qRegGPADC_ModeHighVoltageDifferential:
            return GP_WB_READ_CAL_NVM_GP_ADC_LSB_HIGH_DIFF_UV(calPtr);
        case qRegGPADC_ModeHighVoltageSingleEnded:
            return GP_WB_READ_CAL_NVM_GP_ADC_LSB_HIGH_SE_UV(calPtr);
        case qRegGPADC_ModeLowVoltageDifferential:
            return GP_WB_READ_CAL_NVM_GP_ADC_LSB_LOW_DIFF_UV(calPtr);
        case qRegGPADC_ModeLowVoltageSingleEnded:
            return GP_WB_READ_CAL_NVM_GP_ADC_LSB_LOW_SE_UV(calPtr);
        default:
            break;
    }
    return GP_WB_READ_CAL_NVM_GP_ADC_LSB_HIGH_DIFF_UV(calPtr);
}

/** @brief Get GPADC calibrated offset value in microvolt. */
static inline Int32 qRegGPADC_CalibratedOffsetGet(qRegGPADC_Mode_t mode, UInt32 calPtr)
{
    switch(mode)
    {
        case qRegGPADC_ModeHighVoltageDifferential:
            return (Int32)GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_HIGH_DIFF_UV(calPtr);
        case qRegGPADC_ModeHighVoltageSingleEnded:
            return (Int32)GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_HIGH_SE_UV(calPtr);
        case qRegGPADC_ModeLowVoltageDifferential:
            return (Int32)GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_LOW_DIFF_UV(calPtr);
        case qRegGPADC_ModeLowVoltageSingleEnded:
            return (Int32)GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_LOW_SE_UV(calPtr);
        default:
            break;
    }
    return (Int32)GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_HIGH_DIFF_UV(calPtr);
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_GPADC_H_ */
