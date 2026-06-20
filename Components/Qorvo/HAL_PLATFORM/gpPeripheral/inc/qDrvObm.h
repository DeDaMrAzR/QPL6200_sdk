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

#ifndef _Q_DRV_OBM_H_
#define _Q_DRV_OBM_H_

/**
 * @defgroup Q_DRV_OBM_H OBM driver
 * @{
 *
 * @brief This is a driver for OBM peripheral.
 *
 * @file qDrvObm.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegObm.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

//@{
/** @brief OBM(Oscillator BenchMark) input clock source off. */
#define qDrvObm_ClkSrcOff qRegObm_ClkSrcOff
/** @brief Use RC64KHz as clock source. */
#define qDrvObm_ClkSrcRc64k qRegObm_ClkSrcRc64k
/** @brief Use XTL 32KHz as clock source. */
#define qDrvObm_ClkSrcXt32k qRegObm_ClkSrcXt32k
/** @brief Use DTC as clock source. */
#define qDrvObm_ClkSrcDtc qRegObm_ClkSrcDtc
/** @brief Use XTL 32MHz as clock source. */
#define qDrvObm_ClkSrcXt32m qRegObm_ClkSrcXt32m
/** @brief Use BIST Ring oscillator as clock source. */
#define qDrvObm_ClkSrcBistRingOsc qRegObm_ClkSrcBistRingOsc
/** @brief Use XO 32MHz as clock source. */
#define qDrvObm_ClkSrcBistXo32m qRegObm_ClkSrcBistXo32m
/** @brief Use external clock or input as clock source. */
#define qDrvObm_ClkSrcExternal qRegObm_ClkSrcExternal
/** @brief Use RC measurements as clock source. */
#define qDrvObm_ClkSrcBistRcmeas qRegObm_ClkSrcBistRcmeas
/** @brief Use RC 64MHz as clock source. */
#define qDrvObm_ClkSrcRc64m qRegObm_ClkSrcRc64m
/** @typedef qDrvObm_ClkSrc_t
 *  @brief OBM input clock source.
 */
typedef UInt8 qDrvObm_ClkSrc_t;
//@}

//@{
/** @brief OBM(Oscillator BenchMark) state idle. */
#define qDrvObm_StateIdle qRegObm_StateIdle
/** @brief OBM state resync init 0 */
#define qDrvObm_StateResyncInit0 qRegObm_StateResyncInit0
/** @brief OBM state resync init 1 */
#define qDrvObm_StateResyncInit1 qRegObm_StateResyncInit1
/** @brief OBM state resync init 0 */
#define qDrvObm_StateResyncInit2 qRegObm_StateResyncInit2
/** @brief OBM state searching first edge. */
#define qDrvObm_StateSearchingFirstEdge qRegObm_StateSearchingFirstEdge
/** @brief OBM state measuring. */
#define qDrvObm_StateMeasuring qRegObm_StateMeasuring
/** @typedef qDrvObm_State_t
 *  @brief OBM input clock source.
 */
typedef UInt8 qDrvObm_State_t;
//@}

/** @brief Obm config structure. */
typedef struct {
    UInt16 edgeCountThreshold; /**< Number of periods to measure the length of. */
    UInt16 prescaler;          /**< Prescaler divider. range 0..12 */
    qDrvObm_ClkSrc_t clkSrc;   /**< Clock source for the OBM input signal. */
    Bool srcInvert;            /**< Invert the signal before searching the rising edges. */
    UInt16 periodMinThreshold; /**< Minimum period threshold in 64MHz clock cycles. */
    UInt16 periodMaxThreshold; /**< Maximum period threshold in 64MHz clock cycles. */
} qDrvObm_Config_t;

/** @brief Obm period measurement done callback. */
typedef void (*qDrvObm_CbPeriodMeasDone_t)(void);

/** @brief Obm total measurement done callback. */
typedef void (*qDrvObm_CbTotalMeasDone_t)(void);

/** @brief Callback set structure definition. */
typedef struct {
    qDrvObm_CbPeriodMeasDone_t periodMeasDone; /**< Single period measurement complete. */
    qDrvObm_CbTotalMeasDone_t totalMeasDone;   /**< Total measurement cycle complete.*/
} qDrvObm_Callbacks_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define Q_DRV_OBM_PRESCALEAR_MAX REG_OBM_PRESCALER_MAX

/** @brief Default configuration macro for OBM. */
#define Q_DRV_OBM_CONFIG_DEFAULT(CLKSRC, PRESCALE, EDGECOUNT)                                                          \
    {                                                                                                                  \
        .edgeCountThreshold = EDGECOUNT, /* */                                                                         \
            .prescaler = PRESCALE,       /* */                                                                         \
            .clkSrc = CLKSRC,            /* */                                                                         \
            .srcInvert = false,          /* */                                                                         \
            .periodMinThreshold = 0,     /* */                                                                         \
            .periodMaxThreshold = 0      /* */                                                                         \
    }

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/** @brief Clear unmasked obm total measurement done interrupt */
static inline void qDrvObm_TotalMeasDoneIRQClear(void)
{
    qRegObm_TotalMeasDoneIRQClear();
}

/** @brief Set number of periods to measure the length of */
static inline void qDrvObm_EdgeCountThresholdSet(UInt16 threshold)
{
    qRegObm_EdgeCountThresholdSet(threshold);
}

/** @brief Select the input clock source. */
static inline void qDrvObm_ClkSrcSet(qDrvObm_ClkSrc_t clkSrc)
{
    qRegObm_ClkSrcSet((qRegObm_ClkSrc_t)clkSrc);
}

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

/**
 * @brief Initialize Obm instance.
 *
 * @param[in] pConfig   Pointer to configuration structure.
 * @param[in] pCallbacks Pointer to callback functions.
 *
 * @return  Error code detailing any errors, Q_OK if successful. see @c qResult_t
 */
qResult_t qDrvObm_Init(const qDrvObm_Config_t* const pConfig, const qDrvObm_Callbacks_t* const pCallbacks);

/**
 * @brief Read the last measured period, in 64MHz clock cycles.
 *
 * @return Number of 64MHz clock cycles.
 */
UInt16 qDrvObm_PeriodMeasGet(void);

/**
 * @brief Read the total length of the measurement, in 64MHz clock cycles.
 *
 * @return Number of 64MHz clock cycles.
 * */
UInt32 qDrvObm_TotalMeasGet(void);

/**
 * @brief Read the total 64MHz clock cycles where the input signal was 1.
 *
 * @return Number of 64MHz clock cycles.
 */
UInt32 qDrvObm_HighMeasGet(void);

/**
 * @brief Read the actual number of edges seen.
 *
 * @return Number of edges counted.
 */
UInt16 qDrvObm_EdgeCountGet(void);

/**
 * @brief Read internal state.
 *
 * @return @qDrvObm_State_t OBM state.
 */
qDrvObm_State_t qDrvObm_StateGet(void);

/**
 * @brief Read the status bit indicating that at least 1 period measurement was below minimum threshold.
 *
 * @return
 *        @c true At least 1 period measurement was below obm_period_min_thr.
 *        @c false  otherwise
 */
Bool qDrvObm_IsPeriodBelowMin(void);

/**
 * @brief Read the status bit indicating that at least 1 period measurement was above maximum threshold.
 *
 * @return Number of edges counted.
 *        @c true At least 1 period measurement was above obm_period_max_thr.
 *        @c false otherwise
 */
Bool qDrvObm_IsPeriodAboveMax(void);

/**
 * @brief Starts the measurement cycle.
 *
 * @return  Error code detailing any errors, Q_OK if successful. see @c qResult_t
 */
qResult_t qDrvObm_Start(void);

/**
 * @brief Stops the measurement cycle prematurely.
 *
 * @return  Error code detailing any errors, Q_OK if successful. see @c qResult_t
 */
qResult_t qDrvObm_Stop(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_OBM_H_
