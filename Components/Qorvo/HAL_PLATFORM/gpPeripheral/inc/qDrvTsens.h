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

#ifndef _Q_DRV_TSENS_H_
#define _Q_DRV_TSENS_H_

/**
 * @defgroup Q_DRV_TSENS temperature sensor driver
 * @{
 *
 * @brief This is a driver for TSENS peripheral.
 *
 * @file qDrvTsens.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegTsens.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define Q_DRV_TSENS_THRESHOLD_MAX 0x1FFF

/** @brief Default configuration macro for TSENS. */
#define Q_DRV_TSENS_CONFIG_DEFAULT(MAX_THRESHOLD, MIN_THRESHOLD, CYCLES)                                               \
    {                                                                                                                  \
        .maxThreshold = MAX_THRESHOLD,     /* */                                                                       \
            .minThreshold = MIN_THRESHOLD, /* */                                                                       \
            .cycles = CYCLES,              /* */                                                                       \
    }

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
//@{
/** @brief TSENS (Temperature sensor) hanning filter cycles 32. */
#define qDrvTsens_HanningCycles32 qRegTsens_HanningCycles32
/** @brief Hanning filter cycles 64. */
#define qDrvTsens_HanningCycles64 qRegTsens_HanningCycles64
/** @brief Hanning filter cycles 128. */
#define qDrvTsens_HanningCycles128 qRegTsens_HanningCycles128

/**
 * @typedef qRegTsens_Hanning_t
 *  @brief TSENS Hanning filter cycles.
 */
typedef UInt8 qDrvTsens_HanningCycles_t;
//@}

/** @brief TSENS config structure. */
typedef struct {
    UInt16 maxThreshold;              /**< Maximum temperature threshold.*/
    UInt16 minThreshold;              /**< Minimum temperature threshold. */
    qDrvTsens_HanningCycles_t cycles; /**< Hanning filter cycles. */
} qDrvTsens_Config_t;

/**@brief  Temp sensor result is above the maximum threshold callback.*/
typedef void (*qDrvTsens_CbAboveMaximum_t)(void);

/**@brief  Temp sensor result is below the minimum threshold callback.*/
typedef void (*qDrvTsens_CbBelowMinimum_t)(void);

/**@brief  Temp sensor fifo holds a measurement result callback.*/
typedef void (*qDrvTsens_CbFifoNotEmpty_t)(void);

/** @brief Callback set structure definition. */
typedef struct {
    qDrvTsens_CbAboveMaximum_t aboveMaximum; /**< Temp sensor result is above the maximum threshold. */
    qDrvTsens_CbBelowMinimum_t belowMinimum; /**< Temp sensor result is below the minimum threshold .*/
    qDrvTsens_CbFifoNotEmpty_t fifoNotEmpty; /**< Temp sensor fifo holds a measurement result.*/
} qDrvTsens_Callbacks_t;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/** @brief Read the Hanning result FIFO of the temp sensor output bit stream scaled to ~13 bit code */
static inline UInt16 qDrvTsens_ResultGet(void)
{
    return qRegTsens_ResultGet();
}

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

/**
 * @brief Initialize TSENS instance.
 *
 * @param[in] pConfig   Pointer to configuration structure.
 * @param[in] pCallbacks Pointer to callback functions.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is not initialized.
 * @retval Q_ERR_INVPARAM Invalid parameters.
 */
qResult_t qDrvTsens_Init(const qDrvTsens_Config_t* const pConfig, const qDrvTsens_Callbacks_t* const pCallbacks);

/**
 * @brief UnInitialize the TSENS instance.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvTsens_Deinit(void);

/**
 * @brief Check if TSENS instance is initialized.
 *
 * @return  True if initialized, false otherwise.
 */
Bool qDrvTsens_InitCheck(void);

/**
 * @brief Set the control part of the temp sensor measurement.
 *
 * @param[in] enable  Enable or disable continuous temperature measurement.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTsens_ContinuousMeasurementEnable(Bool enable);

/**
 * @brief Read the last temperature sensor result FIFO of the temp sensor output bit stream scaled to ~13 bit code.
 *
 * @return Temperature sensor measurement result in degree celsius.
 */
Int16 qDrvTsens_ResultGetNonBlocking(void);

/**
 * @brief Trigger and read the temperature sensor result FIFO of the temp sensor output bit stream scaled to ~13 bit
 * code.
 *
 * @return Temperature sensor measurement result in degree celsius.
 */
Int16 qDrvTsens_ResultGetBlocking(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_TSENS_H_
