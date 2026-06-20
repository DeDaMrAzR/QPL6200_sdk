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

#ifndef _Q_REG_HRADC_H_
#define _Q_REG_HRADC_H_

#include "hal.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get the HRADC register address. */
#define Q_REG_HRADC_FIFO_ADDRESS_GET() (UInt32)(GP_WB_HR_ADC_RESULT_ADDRESS)

/** @brief Get the HRADC action slot offset. */
#define Q_REG_HRADC_QUIC_ACTION_SLOT_OFFSET (GP_WB_HR_ADC_QUIC_CFG_1_ADDRESS - GP_WB_HR_ADC_QUIC_CFG_0_ADDRESS)

/** @enum qRegHRADC_HanningCycles_t */
//@{
#define qRegHRADC_HanningCycles32  0x0UL
#define qRegHRADC_HanningCycles64  0x1UL
#define qRegHRADC_HanningCycles128 0x2UL
#define qRegHRADC_HanningCycles256 0x3UL
/** @typedef qRegHRADC_HanningCycles_t
 *  @brief HRADC cycles that will be applied to the hanning filter before data is ready.
 */
typedef UInt8 qRegHRADC_HanningCycles_t;
//@}

/** @enum qRegHRADC_Channel_t */
//@{
#define qRegHRADC_ChannelAnIo0 GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_0
#define qRegHRADC_ChannelAnIo1 GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_1
#define qRegHRADC_ChannelAnIo2 GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_2
#define qRegHRADC_ChannelAnIo3 GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_3
#define qRegHRADC_ChannelNone  Q_NOCHANNEL
/** @typedef qRegHRADC_Channel_t
 *  @brief HRADC channel.
 */
typedef UInt8 qRegHRADC_Channel_t;
//@}

/** @enum qRegHRADC_QuicActionType_t */
//@{
/** @brief Action none. */
#define qRegHRADC_QuicActionTypeNone GP_WB_ENUM_HR_ADC_ACTION_NONE
/** @brief Action trigger single measurement. */
#define qRegHRADC_QuicActionTypeSingle GP_WB_ENUM_HR_ADC_ACTION_DO_SINGLE_MEASUREMENT
/** @brief Action trigger single measurement. */
#define qRegHRADC_QuicActionTypeAccuStart GP_WB_ENUM_HR_ADC_ACTION_START_ACCU
/** @typedef qRegHRADC_QuicActionType_t
 *  @brief HRADC action.
 */
typedef UInt8 qRegHRADC_QuicActionType_t;

/** @enum qRegHRADC_Gain_t */
//@{
#define qRegHRADC_Gain1_4 0UL
#define qRegHRADC_Gain1   1UL
#define qRegHRADC_Gain4   2UL
#define qRegHRADC_Gain8   3UL
/** @typedef qRegHRADC_Gain_t
 *  @brief HRADC gain.
 */
typedef UInt8 qRegHRADC_Gain_t;

/** @enum qRegHRADC_Irq_t */
//@{
#define qRegHRADC_IrqAccuDone     GP_WB_INT_CTRL_MASKED_APPUC_HR_ADC_ACCU_DONE_INTERRUPT_LSB
#define qRegHRADC_IrqFifoNotEmpty GP_WB_INT_CTRL_MASKED_APPUC_HR_ADC_FIFO_NOT_EMPTY_INTERRUPT_LSB
#define qRegHRADC_IrqOverVoltage  GP_WB_INT_CTRL_MASKED_APPUC_HR_ADC_OVERVOLTAGE_INTERRUPT_LSB
/** @typedef qRegHRADC_Irq_t
 *  @brief HRADC Interrupt.
 */
typedef UInt8 qRegHRADC_Irq_t;
//@}

/** @brief Set HRADC BS skip. */
static inline void qRegHRADC_BsSkipSet(UInt32 bsSkip)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_BS_SKIP_ADDRESS, GP_WB_HR_ADC_BS_SKIP_MASK, bsSkip << GP_WB_HR_ADC_BS_SKIP_LSB);
}

/** @brief Get HRADC BS skip. */
static inline UInt32 qRegHRADC_BsSkipGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_BS_SKIP_ADDRESS) & GP_WB_HR_ADC_BS_SKIP_MASK) >> GP_WB_HR_ADC_BS_SKIP_LSB;
}

/** @brief Set HRADC sample delay. */
static inline void qRegHRADC_SampleDelaySet(UInt32 sampleDelay)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_SAMPLE_DELAY_ADDRESS, GP_WB_HR_ADC_SAMPLE_DELAY_MASK,
                     sampleDelay << GP_WB_HR_ADC_SAMPLE_DELAY_LSB);
}

/** @brief Get HRADC sample delay. */
static inline UInt32 qRegHRADC_SampleDelayGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_SAMPLE_DELAY_ADDRESS) & GP_WB_HR_ADC_SAMPLE_DELAY_MASK) >>
           GP_WB_HR_ADC_SAMPLE_DELAY_LSB;
}

/** @brief Set HRADC hanning cycles. */
static inline void qRegHRADC_HanningCyclesSet(qRegHRADC_HanningCycles_t hanningCycles)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_HANNING_CYCLES_N_ADDRESS, GP_WB_HR_ADC_HANNING_CYCLES_N_MASK,
                     hanningCycles << GP_WB_HR_ADC_HANNING_CYCLES_N_LSB);
}

/** @brief Get HRADC hanning cycles. */
static inline qRegHRADC_HanningCycles_t qRegHRADC_HanningCyclesGet(void)
{
    return (qRegHRADC_HanningCycles_t)((GP_WB_READ_U32(GP_WB_HR_ADC_HANNING_CYCLES_N_ADDRESS) &
                                        GP_WB_HR_ADC_HANNING_CYCLES_N_MASK) >>
                                       GP_WB_HR_ADC_HANNING_CYCLES_N_LSB);
}

/** @brief Set HRADC result overwrite enable. */
static inline void qRegHRADC_ResultOverwriteEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_ALLOW_RESULT_OVERWRITE_ADDRESS, GP_WB_HR_ADC_ALLOW_RESULT_OVERWRITE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_ALLOW_RESULT_OVERWRITE_LSB);
}

/** @brief Get HRADC result overwrite enable. */
static inline Bool qRegHRADC_ResultOverwriteEnableGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_ALLOW_RESULT_OVERWRITE_ADDRESS) & GP_WB_HR_ADC_ALLOW_RESULT_OVERWRITE_MASK) >>
           GP_WB_HR_ADC_ALLOW_RESULT_OVERWRITE_LSB;
}

/** @brief Set HRADC accumulator measurement sequence start overwrite enable. */
static inline void qRegHRADC_AccuStartOverwriteEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_ALLOW_ACCU_START_OVERWRITE_ADDRESS, GP_WB_HR_ADC_ALLOW_ACCU_START_OVERWRITE_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_ALLOW_ACCU_START_OVERWRITE_LSB);
}

/** @brief Get HRADC accumulator measurement sequence start overwrite enable. */
static inline Bool qRegHRADC_AccuStartOverwriteEnableGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_ALLOW_ACCU_START_OVERWRITE_ADDRESS) &
            GP_WB_HR_ADC_ALLOW_ACCU_START_OVERWRITE_MASK) >>
           GP_WB_HR_ADC_ALLOW_ACCU_START_OVERWRITE_LSB;
}

/** @brief Set HRADC reset delay. */
static inline void qRegHRADC_ResetDelaySet(UInt32 resetDelay)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_RESET_DELAY_ADDRESS, GP_WB_HR_ADC_RESET_DELAY_MASK,
                     resetDelay << GP_WB_HR_ADC_RESET_DELAY_LSB);
}

/** @brief Get HRADC reset delay. */
static inline UInt32 qRegHRADC_ResetDelayGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_RESET_DELAY_ADDRESS) & GP_WB_HR_ADC_RESET_DELAY_MASK) >>
           GP_WB_HR_ADC_RESET_DELAY_LSB;
}

/** @brief Set HRADC over voltage detect threshold. */
static inline void qRegHRADC_OverVoltageDetThrSet(UInt32 threshold)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_OVERVOLTAGE_DET_THRESHOLD_ADDRESS, GP_WB_HR_ADC_OVERVOLTAGE_DET_THRESHOLD_MASK,
                     threshold << GP_WB_HR_ADC_OVERVOLTAGE_DET_THRESHOLD_LSB);
}

/** @brief Get HRADC over voltage detect threshold. */
static inline UInt32 qRegHRADC_OverVoltageDetThrGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_OVERVOLTAGE_DET_THRESHOLD_ADDRESS) &
            GP_WB_HR_ADC_OVERVOLTAGE_DET_THRESHOLD_MASK) >>
           GP_WB_HR_ADC_OVERVOLTAGE_DET_THRESHOLD_LSB;
}

/** @brief Set HRADC number of samples that will be measured and accumulated. */
static inline void qRegHRADC_AccuCountSet(UInt32 accuCount)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_ACCU_COUNT_ADDRESS, GP_WB_HR_ADC_ACCU_COUNT_MASK,
                     accuCount << GP_WB_HR_ADC_ACCU_COUNT_LSB);
}

/** @brief Get HRADC number of samples that will be measured and accumulated. */
static inline UInt32 qRegHRADC_AccuCountGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_ACCU_COUNT_ADDRESS) & GP_WB_HR_ADC_ACCU_COUNT_MASK) >>
           GP_WB_HR_ADC_ACCU_COUNT_LSB;
}

/** @brief Set HRADC continuous measurements enable. */
static inline void qRegHRADC_ContinuousMeasEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_ENABLE_CONTINUOUS_MEASUREMENT_ADDRESS,
                     GP_WB_HR_ADC_ENABLE_CONTINUOUS_MEASUREMENT_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_ENABLE_CONTINUOUS_MEASUREMENT_LSB);
}

/** @brief Get HRADC continuous measurements enable. */
static inline Bool qRegHRADC_ContinuousMeasEnableGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_ENABLE_CONTINUOUS_MEASUREMENT_ADDRESS) &
            GP_WB_HR_ADC_ENABLE_CONTINUOUS_MEASUREMENT_MASK) >>
           GP_WB_HR_ADC_ENABLE_CONTINUOUS_MEASUREMENT_LSB;
}

/** @brief Set HRADC length of clk_phi1 phase in multiples of 32 MHz. */
static inline void qRegHRADC_ClkPhi1PhaseLengthSet(UInt32 length)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_CLK_PHI1_PHASE_LENGTH_ADDRESS, GP_WB_HR_ADC_CLK_PHI1_PHASE_LENGTH_MASK,
                     length << GP_WB_HR_ADC_CLK_PHI1_PHASE_LENGTH_LSB);
}

/** @brief Get HRADC length of clk_phi1 phase in multiples of 32 MHz. */
static inline UInt32 qRegHRADC_ClkPhi1PhaseLengthGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_CLK_PHI1_PHASE_LENGTH_ADDRESS) & GP_WB_HR_ADC_CLK_PHI1_PHASE_LENGTH_MASK) >>
           GP_WB_HR_ADC_CLK_PHI1_PHASE_LENGTH_LSB;
}

/** @brief Set HRADC length of clk_phi2 phase in multiples of 32 MHz. */
static inline void qRegHRADC_ClkPhi2PhaseLengthSet(UInt32 length)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_CLK_PHI2_PHASE_LENGTH_ADDRESS, GP_WB_HR_ADC_CLK_PHI2_PHASE_LENGTH_MASK,
                     length << GP_WB_HR_ADC_CLK_PHI2_PHASE_LENGTH_LSB);
}

/** @brief Get HRADC length of clk_phi2 phase in multiples of 32 MHz. */
static inline UInt32 qRegHRADC_ClkPhi2PhaseLengthGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_CLK_PHI2_PHASE_LENGTH_ADDRESS) & GP_WB_HR_ADC_CLK_PHI2_PHASE_LENGTH_MASK) >>
           GP_WB_HR_ADC_CLK_PHI2_PHASE_LENGTH_LSB;
}

/** @brief Set HRADC length of clk_f1 phase in multiples of 32 MHz. */
static inline void qRegHRADC_ClkF1PhaseLengthSet(UInt32 length)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_CLK_F1_PHASE_LENGTH_ADDRESS, GP_WB_HR_ADC_CLK_F1_PHASE_LENGTH_MASK,
                     length << GP_WB_HR_ADC_CLK_F1_PHASE_LENGTH_LSB);
}

/** @brief Get HRADC length of clk_f1 phase in multiples of 32 MHz. */
static inline UInt32 qRegHRADC_ClkF1PhaseLengthGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_CLK_F1_PHASE_LENGTH_ADDRESS) & GP_WB_HR_ADC_CLK_F1_PHASE_LENGTH_MASK) >>
           GP_WB_HR_ADC_CLK_F1_PHASE_LENGTH_LSB;
}

/** @brief Set HRADC length of clk_f2 phase in multiples of 32 MHz. */
static inline void qRegHRADC_ClkF2PhaseLengthSet(UInt32 length)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_CLK_F2_PHASE_LENGTH_ADDRESS, GP_WB_HR_ADC_CLK_F2_PHASE_LENGTH_MASK,
                     length << GP_WB_HR_ADC_CLK_F2_PHASE_LENGTH_LSB);
}

/** @brief Get HRADC length of clk_f2 phase in multiples of 32 MHz. */
static inline UInt32 qRegHRADC_ClkF2PhaseLengthGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_CLK_F2_PHASE_LENGTH_ADDRESS) & GP_WB_HR_ADC_CLK_F2_PHASE_LENGTH_MASK) >>
           GP_WB_HR_ADC_CLK_F2_PHASE_LENGTH_LSB;
}

/** @brief Start HRADC measurements. */
static inline void qRegHRADC_AccuStart(void)
{
    GP_WB_WRITE_U32(GP_WB_HR_ADC_START_ACCU_ADDRESS, GP_WB_HR_ADC_START_ACCU_MASK);
}

/** @brief Get HRADC measurement result. */
static inline UInt32 qRegHRADC_AccuResultGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_ACCU_RESULT_ADDRESS) & GP_WB_HR_ADC_ACCU_RESULT_MASK) >>
           GP_WB_HR_ADC_ACCU_RESULT_LSB;
}

/** @brief Get HRADC valid status of measurement. */
static inline Bool qRegHRADC_AccuResultValidGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_ACCU_RESULT_VALID_ADDRESS) & GP_WB_HR_ADC_ACCU_RESULT_VALID_MASK) >>
           GP_WB_HR_ADC_ACCU_RESULT_VALID_LSB;
}

/** @brief Get HRADC ongoing status of measurement. */
static inline Bool qRegHRADC_AccuResultOngoingGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_ONGOING_MEASUREMENT_ADDRESS) & GP_WB_HR_ADC_ONGOING_MEASUREMENT_MASK) >>
           GP_WB_HR_ADC_ONGOING_MEASUREMENT_LSB;
}

/** @brief Clear HRADC measurement done interrupt. */
static inline void qRegHRADC_IRQAccuDoneClear(void)
{
    GP_WB_WRITE_U32(GP_WB_HR_ADC_CLR_ACCU_DONE_INTERRUPT_ADDRESS, GP_WB_HR_ADC_CLR_ACCU_DONE_INTERRUPT_MASK);
}

/** @brief Clear HRADC over voltage interrupt. */
static inline void qRegHRADC_IRQOverVoltageClear(void)
{
    GP_WB_WRITE_U32(GP_WB_HR_ADC_CLR_OVERVOLTAGE_INTERRUPT_ADDRESS, GP_WB_HR_ADC_CLR_OVERVOLTAGE_INTERRUPT_MASK);
}

/** @brief Check HRADC accumulation done unmasked interrupt. */
static inline Bool qRegHRADC_IRQUnmaskedAccuDoneCheck(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_UNMASKED_ACCU_DONE_INTERRUPT_ADDRESS) &
            GP_WB_HR_ADC_UNMASKED_ACCU_DONE_INTERRUPT_MASK) >>
           GP_WB_HR_ADC_UNMASKED_ACCU_DONE_INTERRUPT_LSB;
}

/** @brief Check HRADC fifo not empty unmasked interrupt. */
static inline Bool qRegHRADC_IRQUnmaskedFifoNotEmptyCheck(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_UNMASKED_FIFO_NOT_EMPTY_INTERRUPT_ADDRESS) &
            GP_WB_HR_ADC_UNMASKED_FIFO_NOT_EMPTY_INTERRUPT_MASK) >>
           GP_WB_HR_ADC_UNMASKED_FIFO_NOT_EMPTY_INTERRUPT_LSB;
}

/** @brief Check HRADC over voltage unmasked interrupt. */
static inline Bool qRegHRADC_IRQUnmaskedOverVoltageCheck(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_UNMASKED_OVERVOLTAGE_INTERRUPT_ADDRESS) &
            GP_WB_HR_ADC_UNMASKED_OVERVOLTAGE_INTERRUPT_MASK) >>
           GP_WB_HR_ADC_UNMASKED_OVERVOLTAGE_INTERRUPT_LSB;
}

/** @brief Get HRADC hanning result FIFO of the output bitstream scaled to ~16 bit code. */
static inline UInt32 qRegHRADC_ResultGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_RESULT_ADDRESS) & GP_WB_HR_ADC_RESULT_MASK) >> GP_WB_HR_ADC_RESULT_LSB;
}

/** @brief Set HRADC N channel. */
static inline void qRegHRADC_ChannelNSet(qRegHRADC_Channel_t channel)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_N_SEL_ADDRESS, GP_WB_HR_ADC_N_SEL_MASK, channel << GP_WB_HR_ADC_N_SEL_LSB);
}

/** @brief Get HRADC N channel. */
static inline qRegHRADC_Channel_t qRegHRADC_ChannelNGet(void)
{
    return (qRegHRADC_Channel_t)((GP_WB_READ_U32(GP_WB_HR_ADC_N_SEL_ADDRESS) & GP_WB_HR_ADC_N_SEL_MASK) >>
                                 GP_WB_HR_ADC_N_SEL_LSB);
}

/** @brief Set HRADC P channel. */
static inline void qRegHRADC_ChannelPSet(qRegHRADC_Channel_t channel)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_P_SEL_ADDRESS, GP_WB_HR_ADC_P_SEL_MASK, channel << GP_WB_HR_ADC_P_SEL_LSB);
}

/** @brief Get HRADC P channel. */
static inline qRegHRADC_Channel_t qRegHRADC_ChannelPGet(void)
{
    return (qRegHRADC_Channel_t)((GP_WB_READ_U32(GP_WB_HR_ADC_P_SEL_ADDRESS) & GP_WB_HR_ADC_P_SEL_MASK) >>
                                 GP_WB_HR_ADC_P_SEL_LSB);
}

/** @brief Set HRADC QUIC action channel. */
static inline void qRegHRADC_QuicActionChannelSet(UInt8 actionIndex, UInt32 channel)
{
    UInt32 address = GP_WB_HR_ADC_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_HRADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    GP_WB_MWRITE_U32(address, GP_WB_HR_ADC_QUIC_ACTION_CHANNEL_0_MASK,
                     channel << GP_WB_HR_ADC_QUIC_ACTION_CHANNEL_0_LSB);
}

/** @brief Get HRADC QUIC action channel. */
static inline UInt32 qRegHRADC_QuicActionChannelGet(UInt8 actionIndex)
{
    UInt32 address = GP_WB_HR_ADC_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_HRADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    return (GP_WB_READ_U32(address) & GP_WB_HR_ADC_QUIC_ACTION_CHANNEL_0_MASK) >>
           GP_WB_HR_ADC_QUIC_ACTION_CHANNEL_0_LSB;
}

/** @brief Set HRADC QUIC action type. */
static inline void qRegHRADC_QuicActionTypeSet(UInt8 actionIndex, qRegHRADC_QuicActionType_t quicActionType)
{
    UInt32 address = GP_WB_HR_ADC_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_HRADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    GP_WB_MWRITE_U32(address, GP_WB_HR_ADC_QUIC_ACTION_TYPE_0_MASK,
                     quicActionType << GP_WB_HR_ADC_QUIC_ACTION_TYPE_0_LSB);
}

/** @brief Get HRADC QUIC action type. */
static inline qRegHRADC_QuicActionType_t qRegHRADC_QuicActionTypeGet(UInt8 actionIndex)
{
    UInt32 address = GP_WB_HR_ADC_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_HRADC_QUIC_ACTION_SLOT_OFFSET * actionIndex);

    return (qRegHRADC_QuicActionType_t)((GP_WB_READ_U32(address) & GP_WB_HR_ADC_QUIC_ACTION_TYPE_0_MASK) >>
                                        GP_WB_HR_ADC_QUIC_ACTION_TYPE_0_LSB);
}

/** @brief Set HRADC boost. */
static inline void qRegHRADC_BoostSet(UInt32 boost)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_BOOST_ADDRESS, GP_WB_HR_ADC_BOOST_MASK, boost << GP_WB_HR_ADC_BOOST_LSB);
}

/** @brief Get HRADC boost. */
static inline UInt32 qRegHRADC_BoostGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_BOOST_ADDRESS) & GP_WB_HR_ADC_BOOST_MASK) >> GP_WB_HR_ADC_BOOST_LSB;
}

/** @brief Set HRADC gain. */
static inline void qRegHRADC_GainSet(qRegHRADC_Gain_t gain)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_GAIN_ADDRESS, GP_WB_HR_ADC_GAIN_MASK, gain << GP_WB_HR_ADC_GAIN_LSB);
}

/** @brief Get HRADC gain. */
static inline qRegHRADC_Gain_t qRegHRADC_GainGet(void)
{
    return (qRegHRADC_Gain_t)((GP_WB_READ_U32(GP_WB_HR_ADC_GAIN_ADDRESS) & GP_WB_HR_ADC_GAIN_MASK) >>
                              GP_WB_HR_ADC_GAIN_LSB);
}

/** @brief Set HRADC enable bias LDO power up. */
static inline void qRegHRADC_BiasLdoPowerUpEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_BIAS_LDO_PUP_ADDRESS, GP_WB_HR_ADC_BIAS_LDO_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_BIAS_LDO_PUP_LSB);
}

/** @brief Get HRADC enable bias LDO power up. */
static inline Bool qRegHRADC_BiasLdoPowerUpEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_BIAS_LDO_PUP_ADDRESS) & GP_WB_HR_ADC_BIAS_LDO_PUP_MASK) >>
                  GP_WB_HR_ADC_BIAS_LDO_PUP_LSB);
}

/** @brief Set HRADC enable dithering of sigma delta. */
static inline void qRegHRADC_DitherEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_DITHER_ENA_ADDRESS, GP_WB_HR_ADC_DITHER_ENA_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_DITHER_ENA_LSB);
}

/** @brief Get HRADC enable dithering of sigma delta. */
static inline Bool qRegHRADC_DitherEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_DITHER_ENA_ADDRESS) & GP_WB_HR_ADC_DITHER_ENA_MASK) >>
                  GP_WB_HR_ADC_DITHER_ENA_LSB);
}

/** @brief Set HRADC enable bleed for analog supply LDO. */
static inline void qRegHRADC_LdoBleedEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_LDO_BLEED_EN_ADDRESS, GP_WB_HR_ADC_LDO_BLEED_EN_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_LDO_BLEED_EN_LSB);
}

/** @brief Get HRADC enable bleed for analog supply LDO. */
static inline Bool qRegHRADC_LdoBleedEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_LDO_BLEED_EN_ADDRESS) & GP_WB_HR_ADC_LDO_BLEED_EN_MASK) >>
                  GP_WB_HR_ADC_LDO_BLEED_EN_LSB);
}

/** @brief Set HRADC enable current monitor HRADC LDO. */
static inline void qRegHRADC_LdoIMonEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_LDO_IMON_EN_ADDRESS, GP_WB_HR_ADC_LDO_IMON_EN_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_LDO_IMON_EN_LSB);
}

/** @brief Get HRADC enable current monitor HRADC LDO. */
static inline Bool qRegHRADC_LdoIMonEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_LDO_IMON_EN_ADDRESS) & GP_WB_HR_ADC_LDO_IMON_EN_MASK) >>
                  GP_WB_HR_ADC_LDO_IMON_EN_LSB);
}

/** @brief Set HRADC enable analog supply LDO. */
static inline void qRegHRADC_LdoAnalogSupplyEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_LDO_PUP_ADDRESS, GP_WB_HR_ADC_LDO_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_LDO_PUP_LSB);
}

/** @brief Get HRADC enable analog supply LDO. */
static inline Bool qRegHRADC_LdoAnalogSupplyEnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_LDO_PUP_ADDRESS) & GP_WB_HR_ADC_LDO_PUP_MASK) >> GP_WB_HR_ADC_LDO_PUP_LSB);
}

/** @brief Set HRADC capbank muxes enable. */
static inline void qRegHRADC_MuxPowerUpEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_MUX_PUP_ADDRESS, GP_WB_HR_ADC_MUX_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_MUX_PUP_LSB);
}

/** @brief Get HRADC capbank muxes enable. */
static inline Bool qRegHRADC_MuxPowerUpEnableGet(void)
{
    return (
        Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_MUX_PUP_ADDRESS) & GP_WB_HR_ADC_MUX_PUP_MASK) >> GP_WB_HR_ADC_MUX_PUP_LSB);
}

/** @brief Set HRADC channel N pup signal. */
static inline void qRegHRADC_ChannelNSelPupSet(Bool selPup)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_N_SEL_PUP_ADDRESS, GP_WB_HR_ADC_N_SEL_PUP_MASK, selPup << GP_WB_HR_ADC_N_SEL_PUP_LSB);
}

/** @brief Get HRADC channel N pup signal. */
static inline Bool qRegHRADC_ChannelNSelPupGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_N_SEL_PUP_ADDRESS) & GP_WB_HR_ADC_N_SEL_PUP_MASK) >>
                  GP_WB_HR_ADC_N_SEL_PUP_LSB);
}

/** @brief Set HRADC channel P pup signal. */
static inline void qRegHRADC_ChannelPSelPupSet(Bool selPup)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_P_SEL_PUP_ADDRESS, GP_WB_HR_ADC_P_SEL_PUP_MASK, selPup << GP_WB_HR_ADC_P_SEL_PUP_LSB);
}

/** @brief Get HRADC channel P pup signal. */
static inline Bool qRegHRADC_ChannelPSelPupGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_P_SEL_PUP_ADDRESS) & GP_WB_HR_ADC_P_SEL_PUP_MASK) >>
                  GP_WB_HR_ADC_P_SEL_PUP_LSB);
}

/** @brief Set HRADC enable power-up. */
static inline void qRegHRADC_PowerUpEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_PUP_ADDRESS, GP_WB_HR_ADC_PUP_MASK, (enable ? 1UL : 0UL) << GP_WB_HR_ADC_PUP_LSB);
}

/** @brief Get HRADC enable power-up. */
static inline Bool qRegHRADC_PowerUpEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_PUP_ADDRESS) & GP_WB_HR_ADC_PUP_MASK) >> GP_WB_HR_ADC_PUP_LSB);
}

/** @brief Set HRADC enable Vref buffer circuit. */
static inline void qRegHRADC_VrefBufferCircuitEnableSet(Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_REF_BUFFER_PUP_ADDRESS, GP_WB_HR_ADC_REF_BUFFER_PUP_MASK,
                     (enable ? 1UL : 0UL) << GP_WB_HR_ADC_REF_BUFFER_PUP_LSB);
}

/** @brief Get HRADC enable Vref buffer circuit. */
static inline Bool qRegHRADC_VrefBufferCircuitEnableGet(void)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_HR_ADC_REF_BUFFER_PUP_ADDRESS) & GP_WB_HR_ADC_REF_BUFFER_PUP_MASK) >>
                  GP_WB_HR_ADC_REF_BUFFER_PUP_LSB);
}

/** @brief Set HRADC reference tune bits for analog supply LDO. */
static inline void qRegHRADC_LdoRefBitsSet(UInt8 refBits)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_LDO_REFBITS_ADDRESS, GP_WB_HR_ADC_LDO_REFBITS_MASK,
                     (UInt32)refBits << GP_WB_HR_ADC_LDO_REFBITS_LSB);
}

/** @brief Get HRADC reference tune bits for analog supply LDO. */
static inline UInt8 qRegHRADC_LdoRefBitsGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_LDO_REFBITS_ADDRESS) & GP_WB_HR_ADC_LDO_REFBITS_MASK) >>
           GP_WB_HR_ADC_LDO_REFBITS_LSB;
}

/** @brief Set HRADC reference tune bits. */
static inline void qRegHRADC_RefBitsSet(UInt8 refBits)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_REF_REFBITS_ADDRESS, GP_WB_HR_ADC_REF_REFBITS_MASK,
                     (UInt32)refBits << GP_WB_HR_ADC_REF_REFBITS_LSB);
}

/** @brief Get HRADC reference tune bits. */
static inline UInt8 qRegHRADC_RefBitsGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_REF_REFBITS_ADDRESS) & GP_WB_HR_ADC_REF_REFBITS_MASK) >>
           GP_WB_HR_ADC_REF_REFBITS_LSB;
}

/** @brief Set HRADC reference tune bits for common-mode voltage. */
static inline void qRegHRADC_VcmRefBitsSet(UInt8 refBits)
{
    GP_WB_MWRITE_U32(GP_WB_HR_ADC_VCM_REFBITS_ADDRESS, GP_WB_HR_ADC_VCM_REFBITS_MASK,
                     (UInt32)refBits << GP_WB_HR_ADC_VCM_REFBITS_LSB);
}

/** @brief Get HRADC reference tune bits for common-mode voltage. */
static inline UInt8 qRegHRADC_VcmRefBitsGet(void)
{
    return (GP_WB_READ_U32(GP_WB_HR_ADC_VCM_REFBITS_ADDRESS) & GP_WB_HR_ADC_VCM_REFBITS_MASK) >>
           GP_WB_HR_ADC_VCM_REFBITS_LSB;
}

/** @brief Get HRADC calibrated gain used to convert result value into microvolt. */
static inline UInt32 qRegHRADC_CalibratedGainGet(qRegHRADC_Gain_t gain, UInt32 calPtr)
{
    switch(gain)
    {
        case qRegHRADC_Gain1_4:
            return GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN0P25_UV(calPtr);

        case qRegHRADC_Gain1:
            return GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN1_UV(calPtr);

        case qRegHRADC_Gain4:
            return GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN4_UV(calPtr);

        case qRegHRADC_Gain8:
            return GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN8_UV(calPtr);

        default:
            break;
    }
    return 0;
}

/** @brief Get HRADC calibrated offset value in microvolt. */
static inline Int32 qRegHRADC_CalibratedOffsetGet(qRegHRADC_Gain_t gain, UInt32 calPtr)
{
    switch(gain)
    {
        case qRegHRADC_Gain1_4:
            return GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN0P25_UV(calPtr);

        case qRegHRADC_Gain1:
            return GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN1_UV(calPtr);

        case qRegHRADC_Gain4:
            return GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN4_UV(calPtr);

        case qRegHRADC_Gain8:
            return GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN8_UV(calPtr);

        default:
            break;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_HRADC_H_ */
