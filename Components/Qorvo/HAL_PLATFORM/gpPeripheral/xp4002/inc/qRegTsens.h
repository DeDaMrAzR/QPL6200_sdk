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

#ifndef _QREG_TSENS_H_
#define _QREG_TSENS_H_

/**
 * @defgroup QREG_TSENS temprature sensor driver
 * @{
 *
 * @brief This is a driver for TSENS peripheral.
 *
 * @file qRegTsens.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#ifdef GP_COMP_CHIPEMU
#include "gpHal_reg.h"
#else
#include "hal.h"
#endif // GP_COMP_CHIPEMU
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
//@{
/** @brief TSENS (Temprature sensor) hanning filter cycles 32. */
#define qRegTsens_HanningCycles32 GP_WB_ENUM_TSENS_HANNING_CYCLES_N_32
/** @brief Hanning filter cycles 64. */
#define qRegTsens_HanningCycles64 GP_WB_ENUM_TSENS_HANNING_CYCLES_N_64
/** @brief Hanning filter cycles 128. */
#define qRegTsens_HanningCycles128 GP_WB_ENUM_TSENS_HANNING_CYCLES_N_128

/**
 * @typedef qRegTsens_Hanning_t
 *  @brief TSENS Hanning filter cycles.
 */
typedef UInt8 qRegTsens_HanningCycles_t;
//@}

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/** @brief Set the control part of the tempsensor continuous measurement. */
static inline void qRegTsens_ContinuousMeasurementEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_ENABLE_CONTINUOUS_MEASUREMENT(enable);
}

/** @brief Read the control part of the tempsensor continuous measurement. */
static inline Bool qRegTsens_ContinuousMeasurementEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_ENABLE_CONTINUOUS_MEASUREMENT();
}

/** @brief Set the amount of initial skips BSs results form the analog before starting to sample it on a start of a
 * measurement cycle.*/
static inline void qRegTsens_BsSkipSet(UInt8 delay)
{
    GP_WB_WRITE_BIST_TSENS_BS_SKIP(delay);
}

/** @brief Read the amount of initial skips BSs results form the analog before starting to sample it on a start of a
 * measurement cycle.*/
static inline UInt8 qRegTsens_BsSkipGet(void)
{
    return GP_WB_READ_BIST_TSENS_BS_SKIP();
}

/** @brief Set sample delay after tsens clk edge in multiples of 32MHZ period at which the tempsensor analog bitstream
 * is sampled. */
static inline void qRegTsens_SampleDelaySet(UInt8 delay)
{
    GP_WB_WRITE_BIST_TSENS_SAMPLE_DELAY(delay);
}

/** @brief Read sample delay after tsens clk edge in multiples of 32MHZ period at which the tempsensor analog bitstream
 * is sampled. */
static inline UInt8 qRegTsens_SampleDelayGet(void)
{
    return GP_WB_READ_BIST_TSENS_SAMPLE_DELAY();
}

/** @brief Set Hanning filter number of cycles */
static inline void qRegTsens_HanningCyclesSet(qRegTsens_HanningCycles_t cycles)
{
    GP_WB_WRITE_BIST_TSENS_HANNING_CYCLES_N((UInt8)cycles);
}

/** @brief Read Hanning filter number of cycles */
static inline qRegTsens_HanningCycles_t qRegTsens_HanningCyclesGet(void)
{
    return (qRegTsens_HanningCycles_t)GP_WB_READ_BIST_TSENS_HANNING_CYCLES_N();
}

/** @brief Set the tempsensor result overwrite. when set, the tempsensor result fifo will be overwritten with a new
 * available result, even if the FIFO is full */
static inline void qRegTsens_ResultOverWriteSet(Bool enable)
{
    GP_WB_WRITE_BIST_ALLOW_TSENS_RESULT_OVERWRITE((UInt8)enable);
}

/** @brief Read the tempsensor result overwrite bit. */
static inline Bool qRegTsens_ResultOverWriteGet(void)
{
    return (Bool)GP_WB_READ_BIST_ALLOW_TSENS_RESULT_OVERWRITE();
}

/** @brief Perform a single tsens measurement cycle. */
static inline void qRegTsens_DoSingleMeasurement(void)
{
    GP_WB_BIST_TSENS_DO_SINGLE_MEASUREMENT();
}

/** @brief Indicates that a tsens single measurement is in progress. It is not triggered when
 * tsens_continuous_measurement is set. */
static inline UInt8 qRegTsens_IsMeasurementInProgress(void)
{
    return GP_WB_READ_BIST_TSENS_MEASUREMENT_IN_PROGRESS();
}

/** @brief Set the length ot the tsens clk_phi1 phase in multiples of 32 MHz (length = tsens_clk_phi1_phase_length+1
 * * 31.25 ns). */
static inline void qRegTsens_ClkPhi1PhaseLengthSet(UInt8 length)
{
    GP_WB_WRITE_BIST_TSENS_CLK_PHI1_PHASE_LENGTH(length);
}

/** @brief Read the length ot the tsens clk_phi2 phase in multiples of 32 MHz (length = tsens_clk_phi1_phase_length+1
 * * 31.25 ns). */
static inline UInt8 qRegTsens_ClkPhi1PhaseLengthGet(void)
{
    return GP_WB_READ_BIST_TSENS_CLK_PHI1_PHASE_LENGTH();
}

/** @brief Set the length ot the tsens clk_phi2 phase in multiples of 32 MHz (length = tsens_clk_phi2_phase_length+1
 * * 31.25 ns). */
static inline void qRegTsens_ClkPhi2PhaseLengthSet(UInt8 length)
{
    GP_WB_WRITE_BIST_TSENS_CLK_PHI2_PHASE_LENGTH(length);
}

/** @brief Read the length ot the tsens clk_phi2 phase in multiples of 32 MHz (length = tsens_clk_phi2_phase_length+1
 * * 31.25 ns). */
static inline UInt8 qRegTsens_ClkPhi2PhaseLengthGet(void)
{
    return GP_WB_READ_BIST_TSENS_CLK_PHI2_PHASE_LENGTH();
}

/** @brief Set the length ot the tsens clk_f1 phase in multiples of 32 MHz (length = tsens_clk_f1_phase_length+1 * 31.25
 * ns). */
static inline void qRegTsens_ClkF1PhaseLengthSet(UInt8 length)
{
    GP_WB_WRITE_BIST_TSENS_CLK_F1_PHASE_LENGTH(length);
}

/** @brief Read the length ot the tsens clk_phi2 phase in multiples of 32 MHz (length = tsens_clk_f1_phase_length+1
 * * 31.25 ns). */
static inline UInt8 qRegTsens_ClkF1PhaseLengthGet(void)
{
    return GP_WB_READ_BIST_TSENS_CLK_F1_PHASE_LENGTH();
}

/** @brief Set the maximum threshold above which an interrupt will be triggered if the tsens_result is higher than this
 * threshold */
static inline void qRegTsens_MaxThresholdSet(UInt16 threshold)
{
    GP_WB_WRITE_BIST_TSENS_MAX_THRESHOLD(threshold);
}

/** @brief Read the maximum threshold above which an interrupt will be triggered if the tsens_result is higher than this
 * threshold */
static inline UInt16 qRegTsens_MaxThresholdGet(void)
{
    return GP_WB_READ_BIST_TSENS_MAX_THRESHOLD();
}

/** @brief Set the minimum threshold below which an interrupt will be triggered if thetsens_result is lower than this
 * threshold */
static inline void qRegTsens_MinThresholdSet(UInt16 threshold)
{
    GP_WB_WRITE_BIST_TSENS_MIN_THRESHOLD(threshold);
}

/** @brief Read the minimum threshold below which an interrupt will be triggered if thetsens_result is lower than this
 * threshold */
static inline UInt16 qRegTsens_MinThresholdGet(void)
{
    return GP_WB_READ_BIST_TSENS_MIN_THRESHOLD();
}

/** @brief Read the Hanning result FIFO of the tempsensor output bitstream scaled to ~13 bit code */
static inline UInt16 qRegTsens_ResultGet(void)
{
    return GP_WB_READ_BIST_TSENS_RESULT();
}

/** Read the interrupt status bit indicating if the tempsensor result fifo holds a measurement result. Interrupt cleared
 * when reading from FIFO */
static inline Bool qRegTsens_ResultFifoNotEmptyIRQGet(void)
{
    return GP_WB_READ_BIST_UNMASKED_TSENS_RESULT_FIFO_NOT_EMPTY_INTERRUPT();
}

/** @brief Read the interrupt status bit indicating the tempsensor result is below the minimum threshold set by @c
 * qRegTsens_MinThresholdSet() */
static inline Bool qRegTsens_BelowMinimumIRQGet(void)
{
    return GP_WB_READ_BIST_UNMASKED_TSENS_BELOW_MINIMUM_INTERRUPT();
}

/** @brief Read the interrupt status bit indicating the tempsensor result is above the maximum threshold set by @c
 * qRegTsens_MaxThresholdSet() */
static inline Bool qRegTsens_AboveMaximumIRQGet(void)
{
    return GP_WB_READ_BIST_UNMASKED_TSENS_ABOVE_MAXIMUM_INTERRUPT();
}

/** @brief Clear unmasked TSENS result below minimum interrupt */
static inline void qRegTsens_BelowMinimumIRQClear(void)
{
    GP_WB_BIST_CLR_TSENS_BELOW_MINIMUM_INTERRUPT();
}

/** @brief Clear unmasked TSENS result above maximum interrupt */
static inline void qRegTsens_AboveMaximumIRQClear(void)
{
    GP_WB_BIST_CLR_TSENS_ABOVE_MAXIMUM_INTERRUPT();
}

/** @brief Set the DEM overrules setting of the dvbe generation */
static inline void qRegTsens_DemOverruleSet(UInt8 setting)
{
    GP_WB_WRITE_BIST_TSENS_DEM_OVERRULE(setting);
}

/** @brief Read the DEM overrules setting of the dvbe generation */
static inline UInt8 qRegTsens_DemOverruleGet(void)
{
    return GP_WB_READ_BIST_TSENS_DEM_OVERRULE();
}

/** @brief Set the vcm voltage trim bits. */
static inline void qRegTsens_VcmRefBitsSet(UInt8 bits)
{
    GP_WB_WRITE_BIST_TSENS_VCM_REFBITS(bits);
}

/** @brief Read the vcm voltage trim bits. */
static inline UInt8 qRegTsens_VcmRefBitsGet(void)
{
    return GP_WB_READ_BIST_TSENS_VCM_REFBITS();
}

/** @brief Set the LDO tune word. */
static inline void qRegTsens_LdoRefBitsSet(UInt8 bits)
{
    GP_WB_WRITE_BIST_TSENS_LDO_REFBITS(bits);
}

/** @brief Read the LDO tune word. */
static inline UInt8 qRegTsens_LdoRefBitsGet(void)
{
    return GP_WB_READ_BIST_TSENS_LDO_REFBITS();
}

/** @brief Set the vbe voltage trim bits. */
static inline void qRegTsens_VbeRefBitsSet(UInt8 bits)
{
    GP_WB_WRITE_BIST_TSENS_VBE_REFBITS(bits);
}

/** @brief Read the vbe voltage trim bits. */
static inline UInt8 qRegTsens_VbeRefBitsGet(void)
{
    return GP_WB_READ_BIST_TSENS_VBE_REFBITS();
}

/** @brief Set the bias tune current. */
static inline void qRegTsens_BiasTuneSet(UInt8 current)
{
    GP_WB_WRITE_BIST_TSENS_BIAS_TUNE(current);
}

/** @brief Read the bias tune current. */
static inline UInt8 qRegTsens_BiasTuneGet(void)
{
    return GP_WB_READ_BIST_TSENS_BIAS_TUNE();
}

/** @brief Set the DEM counter range counter. */
static inline void qRegTsens_DemRangeSet(UInt8 range)
{
    GP_WB_WRITE_BIST_TSENS_DEM_RANGE(range);
}

/** @brief Read the DEM counter range counter. */
static inline UInt8 qRegTsens_DemRangeGet(void)
{
    return GP_WB_READ_BIST_TSENS_DEM_RANGE();
}

/** @brief Set the input to use for DEM counter. */
static inline void qRegTsens_DemSelInputSet(UInt8 input)
{
    GP_WB_WRITE_BIST_TSENS_DEM_SEL_INPUT(input);
}

/** @brief Read the input to used for DEM counter. */
static inline UInt8 qRegTsens_DemSelInputGet(void)
{
    return GP_WB_READ_BIST_TSENS_DEM_SEL_INPUT();
}

/** @brief Set the spare bits for temperature sensor. */
static inline void qRegTsens_SpareSet(UInt8 bits)
{
    GP_WB_WRITE_BIST_TSENS_SPARE(bits);
}

/** @brief Read the spare bits of temperature sensor. */
static inline UInt8 qRegTsens_SpareGet(void)
{
    return GP_WB_READ_BIST_TSENS_SPARE();
}

/** @brief Set bias current distribution. */
static inline void qRegTsens_BiasDisPupEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_BIAS_DIST_PUP((UInt8)enable);
}

/** @brief Read bias current distribution. */
static inline Bool qRegTsens_BiasDisPupEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_BIAS_DIST_PUP();
}

/** @brief Set dvbe biasing power up. */
static inline void qRegTsens_BiasDvbePupEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_BIAS_DVBE_PUP((UInt8)enable);
}

/** @brief Read dvbe biasing power up. */
static inline Bool qRegTsens_BiasDvbePupEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_BIAS_DVBE_PUP();
}

/** @brief Set vbe biasing power up. */
static inline void qRegTsens_BiasVbePupEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_BIAS_VBE_PUP((UInt8)enable);
}

/** @brief Read vbe biasing power up. */
static inline Bool qRegTsens_BiasVbePupEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_BIAS_VBE_PUP();
}

/** @brief Set overrule dynamic chop control signals of vbe and dvbe. */
static inline void qRegTsens_ChopOverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_CHOP_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read overrules dynamic chop control signals of vbe and dvbe. */
static inline Bool qRegTsens_ChopOverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CHOP_OVERRULE_ENA();
}

/** @brief Set the overrule of the DEM setting for debug. */
static inline void qRegTsens_DemOverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_DEM_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read the overrule of the DEM setting for debug. */
static inline Bool qRegTsens_DemOverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_DEM_OVERRULE_ENA();
}

/** @brief Set overrule for dvbe chop. */
static inline void qRegTsens_DvbeChopOverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_DVBE_CHOP_OVERRULE((UInt8)enable);
}

/** @brief Read overrule for dvbe chop. */
static inline Bool qRegTsens_DvbChopOverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_DVBE_CHOP_OVERRULE();
}

/** @brief Set tsens to adc mode. */
static inline void qRegTsens_AdcModeEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_EN_ADC_MODE((UInt8)enable);
}

/** @brief Read tsens to adc mode. */
static inline Bool qRegTsens_AdcModeEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_EN_ADC_MODE();
}

/** @brief Set temperature sensor LDO bleed enable signal. */
static inline void qRegTsens_LdoBleedEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_LDO_BLEED((UInt8)enable);
}

/** @brief Read temperature sensor LDO bleed enable signal. */
static inline Bool qRegTsens_LdoBleedEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_LDO_BLEED();
}

/** @brief Set temperature sensor LDO current monitor enable signal. */
static inline void qRegTsens_LdoIMonEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_LDO_IMON_EN((UInt8)enable);
}

/** @brief Read temperature sensor LDO current monitor enable signal. */
static inline Bool qRegTsens_LdoIMonEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_LDO_IMON_EN();
}

/** @brief Set temperature sensor LDO power up signal enable. */
static inline void qRegTsens_LdoPupEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_LDO_PUP((UInt8)enable);
}

/** @brief Read temperature sensor LDO power up signal enable. */
static inline Bool qRegTsens_LdoPupEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_LDO_PUP();
}

/** @brief Set temperature sensor power up signal enable. */
static inline void qRegTsens_PupEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_PUP((UInt8)enable);
}

/** @brief Read temperature sensor power up signal enable. */
static inline Bool qRegTsens_PupEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_PUP();
}

/** @brief Set test input on dvbe. */
static inline void qRegTsens_TestDvbeEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_TEST_DVBE_EN((UInt8)enable);
}

/** @brief Read test input on dvbe. */
static inline Bool qRegTsens_TestDvbeEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_TEST_DVBE_EN();
}

/** @brief Set test input on vbe. */
static inline void qRegTsens_TestVbeEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_TEST_VBE_EN((UInt8)enable);
}

/** @brief Read test input on vbe. */
static inline Bool qRegTsens_TestVbeEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_TEST_VBE_EN();
}

/** @brief Set overrule for vbe chop. */
static inline void qRegTsens_VbeChopOverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_VBE_CHOP_OVERRULE((UInt8)enable);
}

/** @brief Read overrule for vbe chop. */
static inline Bool qRegTsens_VbChopOverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_VBE_CHOP_OVERRULE();
}

/** @brief Read the temperature sensor ouput. This is a bitstream. Convert in digital to temperature */
static inline Bool qRegTsens_BsEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_BS();
}

/** @brief Set overrule value for tsens_clk_eval, Clock (dynamic control signal) for temperature sensor */
static inline void qRegTsens_ClkEvalOverruleSet(Bool value)
{
    GP_WB_WRITE_BIST_TSENS_CLK_EVAL_OVERRULE((UInt8)value);
}

/** @brief Read overruled value for tsens_clk_eval, Clock (dynamic control signal) for temperature sensor */
static inline Bool qRegTsens_ClkEvalOverruleGet(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_EVAL_OVERRULE();
}

/** @brief Set overrule value for tsens_clk_f1, Clock (dynamic control signal) for temperature sensor */
static inline void qRegTsens_ClkF1OverruleSet(Bool value)
{
    GP_WB_WRITE_BIST_TSENS_CLK_F1_OVERRULE((UInt8)value);
}

/** @brief Read overruled value for tsens_clk_f1, Clock (dynamic control signal) for temperature sensor */
static inline Bool qRegTsens_ClkF1OverruleGet(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_F1_OVERRULE();
}

/** @brief Set overrule value for tsens_clk_f2, Clock (dynamic control signal) for temperature sensor */
static inline void qRegTsens_ClkF2OverruleSet(Bool value)
{
    GP_WB_WRITE_BIST_TSENS_CLK_F2_OVERRULE((UInt8)value);
}

/** @brief Read overruled value for tsens_clk_f2, Clock (dynamic control signal) for temperature sensor */
static inline Bool qRegTsens_ClkF2OverruleGet(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_F2_OVERRULE();
}

/** @brief Set overrule value for tsens_clk_phi1, Clock (dynamic control signal) for temperature sensor */
static inline void qRegTsens_ClkPhi1OverruleSet(Bool value)
{
    GP_WB_WRITE_BIST_TSENS_CLK_PHI1_OVERRULE((UInt8)value);
}

/** @brief Read overruled value for tsens_clk_phi1, Clock (dynamic control signal) for temperature sensor */
static inline Bool qRegTsens_ClkPhi1OverruleGet(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_PHI1_OVERRULE();
}

/** @brief Set overrule value for tsens_clk_phi2, Clock (dynamic control signal) for temperature sensor */
static inline void qRegTsens_ClkPhi2OverruleSet(Bool value)
{
    GP_WB_WRITE_BIST_TSENS_CLK_PHI2_OVERRULE((UInt8)value);
}

/** @brief Read overruled value for tsens_clk_phi2, Clock (dynamic control signal) for temperature sensor */
static inline Bool qRegTsens_ClkPhi2OverruleGet(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_PHI2_OVERRULE();
}

/** @brief Set overrule value for tsens_clk_reset, Clock (dynamic control signal) for temperature sensor */
static inline void qRegTsens_ClkResetOverruleSet(Bool value)
{
    GP_WB_WRITE_BIST_TSENS_CLK_RESET_OVERRULE((UInt8)value);
}

/** @brief Read overruled value for tsens_clk_reset, Clock (dynamic control signal) for temperature sensor */
static inline Bool qRegTsens_ClkResetOverruleGet(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_RESET_OVERRULE();
}

/** @brief Set tsens_clk_eval overrule */
static inline void qRegTsens_ClkEvalOverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_CLK_EVAL_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read tsens_clk_eval overrule */
static inline Bool qRegTsens_ClkEvalOverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_EVAL_OVERRULE_ENA();
}

/** @brief Set tsens_clk_f1 overrule */
static inline void qRegTsens_ClkF1OverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_CLK_F1_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read tsens_clk_f1 overrule */
static inline Bool qRegTsens_ClkF1OverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_F1_OVERRULE_ENA();
}

/** @brief Set tsens_clk_f2 overrule */
static inline void qRegTsens_ClkF2OverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_CLK_F2_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read tsens_clk_f2 overrule */
static inline Bool qRegTsens_ClkF2OverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_F2_OVERRULE_ENA();
}

/** @brief Set tsens_clk_phi1 overrule */
static inline void qRegTsens_ClkPhi1OverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_CLK_PHI1_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read tsens_clk_phi1 overrule */
static inline Bool qRegTsens_ClkPhi1OverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_PHI1_OVERRULE_ENA();
}

/** @brief Set tsens_clk_phi2 overrule */
static inline void qRegTsens_ClkPhi2OverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_CLK_PHI2_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read tsens_clk_phi2 overrule */
static inline Bool qRegTsens_ClkPhi2OverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_PHI2_OVERRULE_ENA();
}

/** @brief Set tsens_clk_reset overrule */
static inline void qRegTsens_ClkResetOverruleEnable(Bool enable)
{
    GP_WB_WRITE_BIST_TSENS_CLK_RESET_OVERRULE_ENA((UInt8)enable);
}

/** @brief Read tsens_clk_reset overrule */
static inline Bool qRegTsens_ClkResetOverruleEnabled(void)
{
    return (Bool)GP_WB_READ_BIST_TSENS_CLK_RESET_OVERRULE_ENA();
}

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif //_QREG_TSENS_H_
