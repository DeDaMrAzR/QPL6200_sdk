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

#ifndef _REG_OBM_H_
#define _REG_OBM_H_

/**
 * @defgroup REG_OBM OBM driver
 * @{
 *
 * @brief This is a driver for OBM peripheral.
 *
 * @file qRegObm.h
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
#define REG_OBM_PRESCALER_MAX 12

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
//@{
/** @brief OBM(Oscillator BenchMark) input clock source off. */
#define qRegObm_ClkSrcOff GP_WB_ENUM_OBM_SRC_OFF
/** @brief Use RC64KHz as clock source. */
#define qRegObm_ClkSrcRc64k GP_WB_ENUM_OBM_SRC_RC64K_CLOCK
/** @brief Use XTL 32KHz as clock source. */
#define qRegObm_ClkSrcXt32k GP_WB_ENUM_OBM_SRC_XT32K_CLOCK
/** @brief Use DTC as clock source. */
#define qRegObm_ClkSrcDtc GP_WB_ENUM_OBM_SRC_DTC_CLOCK
/** @brief Use XTL 32MHz as clock source. */
#define qRegObm_ClkSrcXt32m GP_WB_ENUM_OBM_SRC_XT32M_CLOCK
/** @brief Use BIST Ring oscillator as clock source. */
#define qRegObm_ClkSrcBistRingOsc GP_WB_ENUM_OBM_SRC_BIST_RINGOSC
/** @brief Use XO 32MHz as clock source. */
#define qRegObm_ClkSrcBistXo32m GP_WB_ENUM_OBM_SRC_BIST_XO32M_CLOCK
/** @brief Use external clock or input as clock source. */
#define qRegObm_ClkSrcExternal GP_WB_ENUM_OBM_SRC_EXTERNAL
/** @brief Use RC measurements as clock source. */
#define qRegObm_ClkSrcBistRcmeas GP_WB_ENUM_OBM_SRC_BIST_RCMEAS
/** @brief Use RC 64MHz as clock source. */
#define qRegObm_ClkSrcRc64m GP_WB_ENUM_OBM_SRC_RC64M_CLOCK
/** @typedef qRegObm_ClkSrc_t
 *  @brief OBM input clock source.
 */
typedef UInt8 qRegObm_ClkSrc_t;
//@}

//@{
/** @brief OBM(Oscillator BenchMark) state idle. */
#define qRegObm_StateIdle GP_WB_ENUM_OBM_STATE_IDLE
/** @brief OBM state resync init 0 */
#define qRegObm_StateResyncInit0 GP_WB_ENUM_OBM_STATE_RESYNC_INIT0
/** @brief OBM state resync init 1 */
#define qRegObm_StateResyncInit1 GP_WB_ENUM_OBM_STATE_RESYNC_INIT1
/** @brief OBM state resync init 0 */
#define qRegObm_StateResyncInit2 GP_WB_ENUM_OBM_STATE_RESYNC_INIT2
/** @brief OBM state searching first edge. */
#define qRegObm_StateSearchingFirstEdge GP_WB_ENUM_OBM_STATE_SEARCHING_FIRST_EDGE
/** @brief OBM state measuring. */
#define qRegObm_StateMeasuring GP_WB_ENUM_OBM_STATE_MEASURING
/** @typedef qRegObm_State_t
 *  @brief OBM input clock source.
 */
typedef UInt8 qRegObm_State_t;
//@}

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
/** @brief Read the interrupt status bit indicating that a new single period measurement is available */
static inline Bool qRegObm_PeriodMeasDoneIRQGet(void)
{
    return GP_WB_READ_BIST_UNMASKED_OBM_PERIOD_MEAS_DONE_INTERRUPT();
}

/** @brief Read the interrupt status bit indicating that the measurement cycle has ended. */
static inline Bool qRegObm_TotalMeasDoneIRQGet(void)
{
    return GP_WB_READ_BIST_UNMASKED_OBM_TOTAL_MEAS_DONE_INTERRUPT();
}

/** @brief Clear unmasked obm period measurement done interrupt */
static inline void qRegObm_PeriodMeasDoneIRQClear(void)
{
    GP_WB_BIST_CLR_OBM_PERIOD_MEAS_DONE_INTERRUPT();
}

/** @brief Clear unmasked obm total measurement done interrupt */
static inline void qRegObm_TotalMeasDoneIRQClear(void)
{
    GP_WB_BIST_CLR_OBM_TOTAL_MEAS_DONE_INTERRUPT();
}

/** @brief Set number of periods to measure the length of */
static inline void qRegObm_EdgeCountThresholdSet(UInt16 threshold)
{
    GP_WB_WRITE_BIST_OBM_EDGE_CNT_THR(threshold - 1);
}

/** @brief Read number of periods to measure the length of */
static inline UInt16 qRegObm_EdgeCountThresholdGet(void)
{
    return GP_WB_READ_BIST_OBM_EDGE_CNT_THR() + 1;
}

/** @brief Set the prescaler divider. */
static inline void qRegObm_PrescalerSet(UInt16 prescaler)
{
    GP_WB_WRITE_BIST_OBM_PRESCALER(prescaler);
}

/** @brief Read the prescaler divider. */
static inline UInt16 qRegObm_PrescalerGet(void)
{
    return GP_WB_READ_BIST_OBM_PRESCALER();
}

/** @brief Select the input clock source. */
static inline void qRegObm_ClkSrcSet(qRegObm_ClkSrc_t clkSrc)
{
    GP_WB_WRITE_BIST_OBM_SRC((UInt8)clkSrc);
}

/** @brief Read the input clock source. */
static inline qRegObm_ClkSrc_t qRegObm_ClkSrcGet(void)
{
    return (qRegObm_ClkSrc_t)GP_WB_READ_BIST_OBM_SRC();
}

/** @brief Set the input clock source invert bit. */
static inline void qRegObm_SrcInvertSet(Bool enable)
{
    GP_WB_WRITE_BIST_OBM_SRC_INVERT((UInt8)enable);
}

/** @brief Read the input clock source invert bit. */
static inline Bool qRegObm_SrcInvertGet(void)
{
    return (Bool)GP_WB_READ_BIST_OBM_SRC_INVERT();
}

/** @brief Set minimum period threshold in 64MHz clock cycles */
static inline void qRegObm_PeriodMinThresholdSet(UInt16 threshold)
{
    GP_WB_WRITE_BIST_OBM_PERIOD_MIN_THR(threshold);
}

/** @brief Read minimum period threshold in 64MHz clock cycles */
static inline UInt16 qRegObm_PeriodMinThresholdGet(void)
{
    return GP_WB_READ_BIST_OBM_PERIOD_MIN_THR();
}

/** @brief Set maximum period threshold in 64MHz clock cycles */
static inline void qRegObm_PeriodMaxThresholdSet(UInt16 threshold)
{
    GP_WB_WRITE_BIST_OBM_PERIOD_MAX_THR(threshold);
}

/** @brief Read maximum period threshold in 64MHz clock cycles */
static inline UInt16 qRegObm_PeriodMaxThresholdGet(void)
{
    return GP_WB_READ_BIST_OBM_PERIOD_MAX_THR();
}

/** @brief Read the last measured period, in 64MHz clock cycles */
static inline UInt16 qRegObm_PeriodMeasGet(void)
{
    return GP_WB_READ_BIST_OBM_PERIOD_MEAS();
}

/** @brief Read the total length of the measurement, in 64MHz clock cycles */
static inline UInt32 qRegObm_TotalMeasGet(void)
{
    return GP_WB_READ_BIST_OBM_TOTAL_MEAS();
}

/** @brief Read the total 64MHz clock cycles where the input signal was 1 */
static inline UInt32 qRegObm_HighMeasGet(void)
{
    return GP_WB_READ_BIST_OBM_HIGH_MEAS();
}

/** @brief Read the actual number of edges seen */
static inline UInt16 qRegObm_EdgeCountGet(void)
{
    return GP_WB_READ_BIST_OBM_EDGE_CNT();
}

/** @brief Read internal state */
static inline qRegObm_State_t qRegObm_StateGet(void)
{
    return (qRegObm_State_t)GP_WB_READ_BIST_OBM_STATE();
}

/** @brief Read the status bit indicating that at least 1 period measurement was below minimum threshold */
static inline Bool qRegObm_PeriodBelowMinGet(void)
{
    return GP_WB_READ_BIST_OBM_PERIOD_BELOW_MIN();
}

/** @brief Read the status bit indicating that at least 1 period measurement was above maximum threshold */
static inline Bool qRegObm_PeriodAboveMaxGet(void)
{
    return GP_WB_READ_BIST_OBM_PERIOD_ABOVE_MAX();
}

/** @brief Start the measurement cycle */
static inline void qRegObm_Start(void)
{
    GP_WB_BIST_OBM_START();
}

/** @brief Stop the measurement cycle prematurely */
static inline void qRegObm_Stop(void)
{
    GP_WB_BIST_OBM_STOP();
}

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif //_REG_OBM_H_
