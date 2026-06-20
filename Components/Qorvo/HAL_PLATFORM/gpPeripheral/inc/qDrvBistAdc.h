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

#ifndef _Q_DRV_BIST_ADC_H_
#define _Q_DRV_BIST_ADC_H_

/**
 * @defgroup Q_DRV_BIST_ADC Bist ADC driver
 * @{
 *
 * @brief This is a driver for bist-adc. The only usage of this driver should be in the BatteryMonitor and the TxMonitor
 * components.
 *
 * @file qDrvBistAdc.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegBistAdc.h"
#include "qDrvCommon.h"

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUICInternal.h"
#endif /* defined(REG_QUIC_PRESENT) */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvBistAdc_QuicAction_t */
//@{
#define qDrvBistAdc_QuicActionTypeNone      qRegBistAdc_QuicActionNone
#define qDrvBistAdc_QuicActionTypeStart     qRegBistAdc_QuicActionStart
#define qDrvBistAdc_QuicActionTypeContStart qRegBistAdc_QuicActionContStart
#define qDrvBistAdc_QuicActionTypeContStop  qRegBistAdc_QuicActionContStop
/** @typedef qDrvBistAdc_QuicAction_t
 *  @brief BistAdc quic action.
 */
typedef qRegBistAdc_QuicActionType_t qDrvBistAdc_QuicActionType_t;
//@}

/** @brief Bist ADC QUIC action configuration structure. */
typedef struct {
    UInt8 actionIdx;                     /**< QUIC action index. */
    qRegBistAdc_QuicActionType_t action; /**< QUIC action to be applied. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD();  /**< Required action field. */
} qDrvBistAdc_QuicActionConfig_t;
#endif                                   /* REG_QUIC_PRESENT */

/** @brief Bist ADC configuration structure. */
typedef struct {
    UInt8 configNrAvgExp;   /**< Exponent of the number of samples which are averaged. */
    UInt8 muxPSettingVBat;  /**< Index of the Analog signal connected to the P pin of the ADC. */
    UInt8 muxNSettingVBat;  /**< Index of the Analog signal connected to the N pin of the ADC. */
    Bool diffMode;          /**< True if the differential mode is to be enabled, false otherwise. */
    Bool lowVoltageRange;   /**< True if the low-voltage range is selected, false if not. */
    UInt32 configTSampleUs; /**< Sample period in microseconds. */
} qDrvBistAdc_Config_t;

/** @brief Bist ADC instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvBistAdc_t;

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initializes the state variables for the Bist ADC driver.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is already initialized.
 */
qResult_t qDrvBistAdc_Init(qDrvBistAdc_t* const pInstance);

/**
 * @brief Deinitialize the Bist ADC instance.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Bist ADC is enabled.
 */
qResult_t qDrvBistAdc_Deinit(qDrvBistAdc_t* const pInstance);

/**
 * @brief Check if the Bist ADC driver is initialized.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 * @return True if the Bist ADC driver is initialized, false otherwise.
 */
Bool qDrvBistAdc_InitCheck(qDrvBistAdc_t* const pInstance);

/** @brief Configures and enables the Bist ADC before a voltage measurement.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Bist ADC already enabled.
 */
qResult_t qDrvBistAdc_Enable(qDrvBistAdc_t* const pInstance);

/** @brief Disables the Bist ADC.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Bist ADC already disabled.
 */
qResult_t qDrvBistAdc_Disable(qDrvBistAdc_t* const pInstance);

/** @brief directly triggers a voltage measurement.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Measurement is already enabled.
 */
qResult_t qDrvBistAdc_Trigger(qDrvBistAdc_t* const pInstance);

/** @brief Indicated if the voltage measurement has completed.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 * @return True if there the measurement has completed.
 */
Bool qDrvBistAdc_ReadyCheck(qDrvBistAdc_t* const pInstance);

/** @brief reads out the value of the voltage measurement.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 *
 *  @return the voltage value in uV.
 */
Int32 qDrvBistAdc_ResultGet(qDrvBistAdc_t* const pInstance);

/** @brief Sets the configuration the Bist ADC.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 * @param[in] pConfig   Pointer to the configuration structure.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is not yet initialized or measurement is not yet enabled.
 */
qResult_t qDrvBistAdc_ConfigSet(qDrvBistAdc_t* const pInstance, const qDrvBistAdc_Config_t* const pConfig);

/** @brief Retrieves the configuration for the Bist ADC.
 *
 * @param[in] pInstance Bist ADC instance pointer.
 * @param[in] pConfig   Pointer to the configuration structure.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is not yet initialized.
 */
qResult_t qDrvBistAdc_ConfigGet(qDrvBistAdc_t* const pInstance, qDrvBistAdc_Config_t* const pConfig);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_BIST_ADC_H_
