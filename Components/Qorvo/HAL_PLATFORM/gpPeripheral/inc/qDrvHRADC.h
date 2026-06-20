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

#ifndef _Q_DRV_HRADC_H_
#define _Q_DRV_HRADC_H_

/**
 * @defgroup Q_DRV_HRADC_H High Resolution ADC driver
 * @{
 *
 * @brief This is a driver for HRADC.
 *
 * @file qDrvHRADC.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegHRADC.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUICInternal.h"
#endif /* defined(REG_QUIC_PRESENT) */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Pin configuration macro for HRADC instance. */
#define Q_DRV_HRADC_PIN(IOB_INDEX, ANIO_INDEX)                                                                         \
    {                                                                                                                  \
        .pin = IOB_INDEX,                                                       /* */                                  \
            .alternate = Q_DRV_IOB_ALT_GET(IOB_INDEX, ANALOG_ANIO, ANIO_INDEX), /* */                                  \
    }

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvHRADC_Channel_t */
//@{
#define qDrvHRADC_ChannelAnIo0 qRegHRADC_ChannelAnIo0
#define qDrvHRADC_ChannelAnIo1 qRegHRADC_ChannelAnIo1
#define qDrvHRADC_ChannelAnIo2 qRegHRADC_ChannelAnIo2
#define qDrvHRADC_ChannelAnIo3 qRegHRADC_ChannelAnIo3
#define qDrvHRADC_ChannelNone  qRegHRADC_ChannelNone
/** @typedef qDrvHRADC_Channel_t
 *  @brief HRADC channel.
 */
typedef qRegHRADC_Channel_t qDrvHRADC_Channel_t;

/** @enum qDrvHRADC_Gain_t */
//@{
#define qDrvHRADC_Gain1_4 qRegHRADC_Gain1_4
#define qDrvHRADC_Gain1   qRegHRADC_Gain1
#define qDrvHRADC_Gain4   qRegHRADC_Gain4
#define qDrvHRADC_Gain8   qRegHRADC_Gain8
/** @typedef qDrvHRADC_Gain_t
 *  @brief HRADC gain.
 */
typedef qRegHRADC_Gain_t qDrvHRADC_Gain_t;

/** @enum qDrvHRADC_HanningCycles_t */
//@{
#define qDrvHRADC_HanningCycles32  qRegHRADC_HanningCycles32
#define qDrvHRADC_HanningCycles64  qRegHRADC_HanningCycles64
#define qDrvHRADC_HanningCycles128 qRegHRADC_HanningCycles128
#define qDrvHRADC_HanningCycles256 qRegHRADC_HanningCycles256
/** @typedef qDrvHRADC_HanningCycles_t
 *  @brief HRADC cycles that will be applied to the hanning filter before data is ready.
 */
typedef qRegHRADC_HanningCycles_t qDrvHRADC_HanningCycles_t;
//@}

/**
 * @brief Callback triggered on accumulation done event.
 *
 * @param[in] arg    User-defined argument.
 * @param[in] result Accumulation result value.
 */
typedef void (*qDrvHRADC_CbAccuDone_t)(void* arg, UInt32 result);

/**
 * @brief Callback triggered on fifo threshold count occurred.
 *
 * @param[in] arg    User-defined argument.
 * @param[in] buffer Sample buffer pointer.
 * @param[in] length Data length in samples count.
 */
typedef void (*qDrvHRADC_CbFifo_t)(void* arg, UInt16* buffer, UInt16 length);

/**
 * @brief Callback triggered on cycle done event.
 *
 * @param[in] arg    User-defined argument.
 * @param[in] result Cycle result value.
 */
typedef void (*qDrvHRADC_CbCycleDone_t)(void* arg, UInt32 result);

/* Callback set structure definition. */
typedef struct {
    qDrvHRADC_CbAccuDone_t accuDone;   /**< Accumulation complete. */
    qDrvHRADC_CbFifo_t fifoThreshold;  /**< Fifo threshold event. */
    qDrvHRADC_CbCycleDone_t cycleDone; /**< Measurement cycle complete. */
} qDrvHRADC_Cbs_t;

/** @brief HRADC common config structure. */
typedef struct {
    qDrvHRADC_Channel_t pChannel; /**< HRADC positive channel. */
    qDrvHRADC_Channel_t nChannel; /**< HRADC negative channel. Set @p qDrvHRADC_ChannelNone if unused. */
    Bool diffMode;                /**< True if the differential mode is to be enabled, false otherwise. */
    Bool continuousOverwrite; /**< True if in the continuous mode the result is to be overwritten without reading last
                           result, false otherwise. */
    Bool accuStartOverwrite; /**< True if the ongoing accumulator measurement sequence can be restarted if a new trigger
                                arrive, false otherwise. */
    qDrvHRADC_Gain_t gain;   /**< HRADC gain. */
    qDrvHRADC_HanningCycles_t hanningCycles; /**< HRADC cycles to be applied to the hanning filter before data is ready.
                                              */
    UInt32 sampleDelay;                      /**< HRADC sample delay. */
    Bool dma;                                /**< True if the DMA mode is to be used, false otherwise. */
} qDrvHRADC_Config_t;

/** @brief HRADC output fifo config structure. */
typedef struct {
    UInt16* buffer;    /**< Buffer pointer to be used by HRADC fifo. */
    UInt16 bufferSize; /**< Buffer size in bytes. */
} qDrvHRADC_FifoConfig_t;

/** @brief HRADC voltage structure. */
typedef struct {
    Bool negative;     /**< True if the voltage is negative, false otherwise. */
    UInt8 integer;     /**< Integer part of the voltage. */
    UInt16 fractional; /**< Fractional part of the voltage. */
} qDrvHRADC_Voltage_t;

#if defined(REG_QUIC_PRESENT)
/** @brief HRADC QUIC destination configuration structure. */
typedef struct {
    UInt8 actionIdx;                       /**< QUIC action index. */
    qRegHRADC_QuicActionType_t actionType; /**< QUIC action type. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD();    /**< Required action field. */
} qDrvHRADC_QuicActionConfig_t;
#endif                                     /* REG_QUIC_PRESENT */

/** @brief HRADC instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvHRADC_t;

/** @brief Default configuration macro for HRADC instance. */
#define Q_DRV_HRADC_CONFIG_DEFAULT(P_CHANNEL, N_CHANNEL, DIFF_MODE, GAIN)                                              \
    {                                                                                                                  \
        .pChannel = P_CHANNEL,           /**< HRADC positive channel. */                                               \
            .nChannel = N_CHANNEL,       /**< HRADC negative channel. Set @p qDrvHRADC_ChannelNone if unused. */       \
            .diffMode = DIFF_MODE,       /**< True if the differential mode is to be enabled, false otherwise. */      \
            .continuousOverwrite = true, /**< Continuous mode result is overwritten without reading last result. */    \
            .accuStartOverwrite = false, /**< Ongoing accumulator measurement sequence can not be be restarted.  */    \
            .gain = GAIN,                /**< HRADC gain. */                                                           \
            .hanningCycles = qDrvHRADC_HanningCycles256, /**< 256 HRADC cycles applied to the hanning filter. */       \
            .sampleDelay = 0,                            /**< 0 sample delay. */                                       \
            .dma = false,                                /* DMA mode is not used. */                                   \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the HRADC instance pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin already in use.
 */
qResult_t qDrvHRADC_PinConfigSet(const qDrvIOB_PinAlt_t* const pPinConfig);

/**
 * @brief Deinitialize the HRADC instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 * @param[in] pinsCount  Pins count to be configured.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvHRADC_PinConfigClear(const qDrvIOB_PinAlt_t* const pPinConfig, UInt8 pinsCount);

/**
 * @brief Initialize the HRADC instance.
 *
 * @param[in] pInstance   HRADC instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] pCallbacks  Set of callback functions pointer.
 * @param[in] arg         User-supplied argument passed to the callback function.
 * @param[in] irqPriority ARM interrupt priority.

 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_OUTOFMEMORY   Unable to allocate DMA channel.
 * @retval Q_ERR_NOTSUPPORTED  Selected channel is not available on this package.
 */
qResult_t qDrvHRADC_Init(qDrvHRADC_t* const pInstance, const qDrvHRADC_Config_t* const pConfig,
                         const qDrvHRADC_Cbs_t* const pCallbacks, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the HRADC instance.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvHRADC_Deinit(qDrvHRADC_t* const pInstance);

/**
 * @brief Check whether the HRADC instance is initialised.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvHRADC_InitCheck(qDrvHRADC_t* const pInstance);

/**
 * @brief Configure HRADC fifo.
 *
 * @note DMA woks only if the fifo is properly configured.
 *
 * @param[in] pInstance HRADC instance pointer.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Fifo is already enabled.
 * @retval Q_ERR_INVPARAM      Provided configuration is not correct.
 * @retval Q_ERR_GENERAL       Fifo callback function is not set.
 */
qResult_t qDrvHRADC_FifoConfigSet(qDrvHRADC_t* const pInstance, qDrvHRADC_FifoConfig_t* const pConfig);

/**
 * @brief Enable HRADC fifo.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Fifo is already enabled.
 * @retval Q_ERR_GENERAL       Fifo buffer is not set.
 */
qResult_t qDrvHRADC_FifoEnable(qDrvHRADC_t* const pInstance);

/**
 * @brief Disable HRADC fifo.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Fifo is already disabled
 */
qResult_t qDrvHRADC_FifoDisable(qDrvHRADC_t* const pInstance);

/**
 * @brief Check whether the HRADC fifo is enabled.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval True if the fifo is enabled, false otherwise.
 */
Bool qDrvHRADC_FifoEnableCheck(qDrvHRADC_t* const pInstance);

/**
 * @brief Flush HRADC fifo.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_GENERAL       Fifo threshold callback function is not set.
 * @retval Q_ERR_INVSTATE      Fifo buffer is not set.
 * @retval Q_ERR_OUTOFMEMORY   All data is already proceed.
 */
qResult_t qDrvHRADC_FifoFlush(qDrvHRADC_t* const pInstance);

/**
 * @brief Configure HRADC accumulation.
 *
 * @param[in] pInstance HRADC instance pointer.
 * @param[in] accCount
 * @param[in] irqCycleDone
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Ongoing accumulation.
 * @retval Q_ERR_INVPARAM      Wrong accumulation count value.
 * @retval Q_ERR_GENERAL       Accumulate done callback function is not set. */
qResult_t qDrvHRADC_AccumulateConfigSet(qDrvHRADC_t* const pInstance, UInt32 accCount, Bool irqCycleDone);

/**
 * @brief Start HRADC accumulation measurements.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Accumulation count not set or ongoing accumulation and restart not enabled.
 * @retval Q_ERR_BUSY          Unable to start accumulation. Quic already used.
 */
qResult_t qDrvHRADC_AccumulateStart(qDrvHRADC_t* const pInstance);

/**
 * @brief Check whether the HRADC accumulate result is ready.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @return True if the accumulation result is ready, false otherwise.
 */
Bool qDrvHRADC_AccumulateReadyCheck(qDrvHRADC_t* const pInstance);

/**
 * @brief Configure HRADC measurements in the continuous mode.
 *
 * @param[in] pInstance    HRADC instance pointer.
 * @param[in] irqCycleDone True if the interrupt cycle done is to be enabled, false otherwise.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_GENERAL       Accumulate done callback function is not set.
 */
qResult_t qDrvHRADC_ContinuousConfigSet(qDrvHRADC_t* const pInstance, Bool irqCycleDone);

/**
 * @brief Start HRADC measurements in the continuous mode.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Continuous mode already enabled.
 * @retval Q_ERR_BUSY          Unable to start continuous mode. Quic mode already used.
 */
qResult_t qDrvHRADC_ContinuousStart(qDrvHRADC_t* const pInstance);

/**
 * @brief Stop HRADC measurements continuous mode.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Continuous mode already disabled.
 */
qResult_t qDrvHRADC_ContinuousStop(qDrvHRADC_t* const pInstance);

/**
 * @brief Check whether the HRADC continuous mode is enabled.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @return True if the continuous mode is enabled, false otherwise.
 */
Bool qDrvHRADC_ContinuousEnableGet(qDrvHRADC_t* const pInstance);

/**
 * @brief Check whether result raw value is ready.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @return True if the result raw value is ready to be read, false otherwise.
 */
Bool qDrvHRADC_ResultReadyCheck(qDrvHRADC_t* const pInstance);

/**
 * @brief Get HRADC raw result value.
 *
 * @param[in] pInstance HRADC instance pointer.
 *
 * @return Result raw value.
 */
UInt32 qDrvHRADC_RawResultGet(qDrvHRADC_t* const pInstance);

/**
 * @brief Convert raw result value to voltage value.
 *
 * @param[in] pInstance  HRADC instance pointer.
 * @param[in] raw        Buffer result raw value.
 *
 * @return Voltage value.
 */
qDrvHRADC_Voltage_t qDrvHRADC_RawToVoltageConvert(qDrvHRADC_t* const pInstance, UInt32 raw);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_HRADC_H_
