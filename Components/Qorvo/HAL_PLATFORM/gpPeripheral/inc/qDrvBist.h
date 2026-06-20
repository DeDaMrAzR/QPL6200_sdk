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

#ifndef _Q_DRV_BIST_H_
#define _Q_DRV_BIST_H_

/**
 * @defgroup Q_DRV_BIST BIST driver
 * @{
 *
 * @brief This is a driver for BIST peripheral.
 *
 * @file qDrvBist.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegBist.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief BIST interrupt callback registration type. */
typedef enum {
    qDrvBist_HandlerAdc = 0, /**< ADC interrupt callback */
    qDrvBist_HandlerCp,      /**< CP COMP interrupt callback */
    qDrvBist_HandlerObm,     /**< OBM interrupt callback */
    qDrvBist_HandlerTsens,   /**< TSENS interrupt callback */
    qDrvBist_HandlerCounts   /**< Maximum number of interrupt callbacks */
} qDrvBist_Handler_t;

/** @brief BIST interrupt handler callback. */
typedef void (*qDrvBist_HandlerCb_t)(void);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/** @brief Read the global bist unmasked interrupt for appuc. */
static inline Bool qDrvBist_IRQGet(void)
{
    return qRegBist_IRQGet();
}

/** @brief Read the global bist masked interrupt status for appuc. */
static inline Bool qDrvBist_MaskedIRQGet(void)
{
    return qRegBist_MaskedIRQGet();
}

/** @brief Read the global BIST interrupt enable state. */
static inline Bool qDrvBist_IRQEnabled(void)
{
    return qRegBist_IRQEnabled();
}

/** @brief Read masked interrupt status bit for appuc ADC result fifo not empty. */
static inline Bool qDrvBist_AdcResultFifoNotEmptyMaskedIRQGet(void)
{
    return qRegBist_AdcResultFifoNotEmptyMaskedIRQGet();
}

/** @brief Read masked interrupt status bit for appuc cp comp out. */
static inline Bool qDrvBist_CpCmpOutMaskedIRQGet(void)
{
    return qRegBist_CpCmpOutMaskedIRQGet();
}

/** @brief Read the masked interrupt status bit for appuc indicating a new single period measurement is available. */
static inline Bool qDrvBist_PeriodMeasDoneMaskedIRQGet(void)
{
    return qRegBist_PeriodMeasDoneMaskedIRQGet();
}

/** @brief Read the masked interrupt status bit for appuc indicating the measurement cycle has ended. */
static inline Bool qDrvBist_TotalMeasDoneMaskedIRQGet(void)
{
    return qRegBist_TotalMeasDoneMaskedIRQGet();
}

/** @brief Read the masked interrupt status bit indicating tsen above maximum. */
static inline Bool qDrvBist_TsensAboveMaximumMaskedIRQGet(void)
{
    return qRegBist_TsensAboveMaximumMaskedIRQGet();
}

/** @brief Read the masked interrupt status bit indicating tsens below minimum. */
static inline Bool qDrvBist_TsensBelowMinimumMaskedIRQGet(void)
{
    return qRegBist_TsensBelowMinimumMaskedIRQGet();
}

/** @brief Read the masked interrupt status bit indicating tsens result fifo not empty.  */
static inline Bool qDrvBist_TsensResultFifoNotEmptyMaskedIRQGet(void)
{
    return qRegBist_TsensResultFifoNotEmptyMaskedIRQGet();
}

/** @brief Set mask for appuc ADC result fifo not empty interrupt. */
static inline void qDrvBist_AdcResultFifoNotEmptyIRQEnable(Bool enable)
{
    qRegBist_AdcResultFifoNotEmptyIRQEnable(enable);
}

/** @brief Read mask for appuc ADC result fifo not empty interrupt. */
static inline Bool qDrvBist_AdcResultFifoNotEmptyIRQEnabled(void)
{
    return qRegBist_AdcResultFifoNotEmptyIRQEnabled();
}

/** @brief Set mask for appuc cp comp out interrupt. */
static inline void qDrvBist_CpCmpOutIRQEnable(Bool enable)
{
    qRegBist_CpCmpOutIRQEnable(enable);
}

/** @brief Read mask for appuc cp comp out interrupt. */
static inline Bool qDrvBist_CpCmpOutIRQEnabled(void)
{
    return qRegBist_CpCmpOutIRQEnabled();
}

/** @brief Set mask for appuc bist period measurement done interrupt. */
static inline void qDrvBist_PeriodMeasDoneIRQEnable(Bool enable)
{
    qRegBist_PeriodMeasDoneIRQEnable(enable);
}

/** @brief Read mask for appuc bist period measurement done interrupt. */
static inline Bool qDrvBist_PeriodMeasDoneIRQEnabled(void)
{
    return qRegBist_PeriodMeasDoneIRQEnabled();
}

/** @brief Set mask for appuc bist total measurement done interrupt. */
static inline void qDrvBist_TotalMeasDoneIRQEnable(Bool enable)
{
    qRegBist_TotalMeasDoneIRQEnable(enable);
}

/** @brief Read mask for appuc bist total measurement done interrupt. */
static inline Bool qDrvBist_TotalMeasDoneIRQEnabled(void)
{
    return qRegBist_TotalMeasDoneIRQEnabled();
}

/** @brief Set mask for appuc tsens above maximum interrupt. */
static inline void qDrvBist_TsensAboveMaximumIRQEnable(Bool enable)
{
    qRegBist_TsensAboveMaximumIRQEnable(enable);
}

/** @brief Read mask for appuc tsens above maximum interrupt. */
static inline Bool qDrvBist_TsensAboveMaximumIRQEnabled(void)
{
    return qRegBist_TsensAboveMaximumIRQEnabled();
}

/** @brief Set mask for appuc tsens below minimum interrupt. */
static inline void qDrvBist_TsensBelowMinimumIRQEnable(Bool enable)
{
    qRegBist_TsensBelowMinimumIRQEnable(enable);
}

/** @brief Read mask for appuc tsens below minimum interrupt. */
static inline Bool qDrvBist_TsensBelowMinimumIRQEnabled(void)
{
    return qRegBist_TsensBelowMinimumIRQEnabled();
}

/** @brief Set mask for appuc tsens result fifo not empty interrupt. */
static inline void qDrvBist_TsensResultFifoNotEmptyIRQEnable(Bool enable)
{
    qRegBist_TsensResultFifoNotEmptyIRQEnable(enable);
}

/** @brief Read mask for appuc tsens result fifo not empty interrupt. */
static inline Bool qDrvBist_TsensResultFifoNotEmptyIRQEnabled(void)
{
    return qRegBist_TsensResultFifoNotEmptyIRQEnabled();
}

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

/**
 * @brief Register interrupt callback function.
 *
 * @param[in] type  Type of interrupt @c qDrvBist_Handler_t
 * @param[in] pCallbacks Pointer to callback function.
 *
 */
void qDrvBist_RegisterCallback(qDrvBist_Handler_t type, const qDrvBist_HandlerCb_t pCallback);

/**
 * @brief Unregister interrupt callback function.
 *
 * @param[in] type  Type of interrupt @c qDrvBist_Handler_t
 *
 */
void qDrvBist_UnRegisterCallback(qDrvBist_Handler_t type);

/**
 * @brief Enable or disable NVIC BIST interrupt.
 *
 * @param[in] enable Enable/disable flag.
 *            @c true Enable
 *            #c false Disable
 */
void qDrvBist_IRQEnable(Bool enable);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_BIST_H_
