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

#ifndef _REG_BIST_H_
#define _REG_BIST_H_

/**
 * @defgroup REG_BIST BIST driver
 * @{
 *
 * @brief This is a driver for BIST peripheral.
 *
 * @file qRegBist.h
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
/** @brief BIST action, none */
#define qRegBist_ActionNone GP_WB_ENUM_BIST_ACTION_NONE
/** @brief BIST action, start a single temperature sensor measurement. */
#define qRegBist_ActionTsensDoSingle GP_WB_ENUM_BIST_ACTION_TSENS_DO_SINGLE
/** @brief BIST action, start temperature sensor continuous measurement. */
#define qRegBist_ActionTsensContStart GP_WB_ENUM_BIST_ACTION_TSENS_CONT_START
/** @brief BIST action, stop temperature sensor continuous measurement. */
#define qRegBist_ActionTsensContStop GP_WB_ENUM_BIST_ACTION_TSENS_CONT_STOP
/** @brief BIST action, start BIST ADC measurement - same as firing bist_adc_start. */
#define qRegBist_ActionAdcStart GP_WB_ENUM_BIST_ACTION_ADC_START
/** @brief BIST action, start BIST ADC continuous measurement. */
#define qRegBist_ActionAdcContStart GP_WB_ENUM_BIST_ACTION_ADC_CONT_START
/** @brief BIST action, stop BIST ADC continuous measurement. */
#define qRegBist_ActionAdcContStop GP_WB_ENUM_BIST_ACTION_ADC_CONT_STOP

/** @typedef qRegBist_Action_t
 *  @brief BIST action.
 */
typedef UInt8 qRegBist_Action_t;
//@}

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
/** @brief Read the global bist unmasked interrupt for appuc. */
static inline Bool qRegBist_IRQGet(void)
{
    return GP_WB_READ_INT_CTRL_UNMASKED_APPUC_BIST_INTERRUPT();
}

/** @brief Read the global bist masked interrupt staus for appuc. */
static inline Bool qRegBist_MaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_INTERRUPT();
}

/** @brief Enable or disable the global BIST interupt. */
static inline void qRegBist_IRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_INTERRUPT(enable);
}

/** @brief Read the global BIST interrupt enable state. */
static inline Bool qRegBist_IRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_INTERRUPT();
}

/** @brief Read masked interupt status bit for appuc ADC eoc timeout. */
static inline Bool qRegBist_AdcEocTimeoutMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_ADC_EOC_TIMEOUT_INTERRUPT();
}

/** @brief Read masked interupt status bit for appuc ADC result fifo not empty. */
static inline Bool qRegBist_AdcResultFifoNotEmptyMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_ADC_RESULT_FIFO_NOT_EMPTY_INTERRUPT();
}

/** @brief Read masked interupt status bit for appuc cp comp out. */
static inline Bool qRegBist_CpCmpOutMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_CP_COMP_OUT_INTERRUPT();
}

/** @brief Read the masked interrupt stauts bit for appuc indicating a new single period measurement is available. */
static inline Bool qRegBist_PeriodMeasDoneMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_OBM_PERIOD_MEAS_DONE_INTERRUPT();
}

/** @brief Read the masked interrupt status bit for appuc indicating the measurement cycle has ended. */
static inline Bool qRegBist_TotalMeasDoneMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_OBM_TOTAL_MEAS_DONE_INTERRUPT();
}

/** @brief Read the masked interrupt status bit indicating tsen above maximum. */
static inline Bool qRegBist_TsensAboveMaximumMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_TSENS_ABOVE_MAXIMUM_INTERRUPT();
}

/** @brief Read the masked interrupt status bit indicating tsens below minimum. */
static inline Bool qRegBist_TsensBelowMinimumMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_TSENS_BELOW_MINIMUM_INTERRUPT();
}

/** @brief Read the masked interrupt status bit indicating tsens result fifo not empty.  */
static inline Bool qRegBist_TsensResultFifoNotEmptyMaskedIRQGet(void)
{
    return GP_WB_READ_INT_CTRL_MASKED_APPUC_BIST_TSENS_RESULT_FIFO_NOT_EMPTY_INTERRUPT();
}

/** @brief Set mask for appuc ADC eoc timeout interrupt. */
static inline void qRegBist_AdcEocTimeoutIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_ADC_EOC_TIMEOUT_INTERRUPT(enable);
}

/** @brief Read mask for appuc ADC eoc timeout interrupt. */
static inline Bool qRegBist_AdcEocTimeoutIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_ADC_EOC_TIMEOUT_INTERRUPT();
}

/** @brief Set mask for appuc ADC result fifo not empty interrupt. */
static inline void qRegBist_AdcResultFifoNotEmptyIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_ADC_RESULT_FIFO_NOT_EMPTY_INTERRUPT(enable);
}

/** @brief Read mask for appuc ADC result fifo not empty interrupt. */
static inline Bool qRegBist_AdcResultFifoNotEmptyIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_ADC_RESULT_FIFO_NOT_EMPTY_INTERRUPT();
}

/** @brief Set mask for appuc cp comp out interrupt. */
static inline void qRegBist_CpCmpOutIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_CP_COMP_OUT_INTERRUPT(enable);
}

/** @brief Read mask for appuc cp comp out interrupt. */
static inline Bool qRegBist_CpCmpOutIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_CP_COMP_OUT_INTERRUPT();
}

/** @brief Set mask for appuc bist period measurement done interrupt. */
static inline void qRegBist_PeriodMeasDoneIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_OBM_PERIOD_MEAS_DONE_INTERRUPT(enable);
}

/** @brief Read mask for appuc bist period measurement done interrupt. */
static inline Bool qRegBist_PeriodMeasDoneIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_OBM_PERIOD_MEAS_DONE_INTERRUPT();
}

/** @brief Set mask for appuc bist total measurement done interrupt. */
static inline void qRegBist_TotalMeasDoneIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_OBM_TOTAL_MEAS_DONE_INTERRUPT(enable);
}

/** @brief Read mask for appuc bist total measurement done interrupt. */
static inline Bool qRegBist_TotalMeasDoneIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_OBM_TOTAL_MEAS_DONE_INTERRUPT();
}

/** @brief Set mask for appuc tsen above maximum interrupt. */
static inline void qRegBist_TsensAboveMaximumIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_TSENS_ABOVE_MAXIMUM_INTERRUPT(enable);
}

/** @brief Read mask for appuc tsen above maximum interrupt. */
static inline Bool qRegBist_TsensAboveMaximumIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_TSENS_ABOVE_MAXIMUM_INTERRUPT();
}

/** @brief Set mask for appuc tsens below minimum interrupt. */
static inline void qRegBist_TsensBelowMinimumIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_TSENS_BELOW_MINIMUM_INTERRUPT(enable);
}

/** @brief Read mask for appuc tsens below minimum interrupt. */
static inline Bool qRegBist_TsensBelowMinimumIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_TSENS_BELOW_MINIMUM_INTERRUPT();
}

/** @brief Set mask for appuc tsens result fifo not empty interrupt. */
static inline void qRegBist_TsensResultFifoNotEmptyIRQEnable(Bool enable)
{
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_BIST_TSENS_RESULT_FIFO_NOT_EMPTY_INTERRUPT(enable);
}

/** @brief Read mask for appuc tsens result fifo not empty interrupt. */
static inline Bool qRegBist_TsensResultFifoNotEmptyIRQEnabled(void)
{
    return GP_WB_READ_INT_CTRL_MASK_APPUC_BIST_TSENS_RESULT_FIFO_NOT_EMPTY_INTERRUPT();
}

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif //_REG_BIST_H_
