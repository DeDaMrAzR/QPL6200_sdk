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

#ifndef _Q_DRV_GPADC_H_
#define _Q_DRV_GPADC_H_

/**
 * @defgroup Q_DRV_GPADC_H General Purpose ADC driver
 * @{
 *
 * @brief This is a driver for General Purpose ADC.
 *
 * @file qDrvAkumaTriggers.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegGPADC.h"
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

/** @brief Pin configuration macro for GPADC instance. */
#define Q_DRV_GPADC_PIN(IOB_INDEX, ANIO_INDEX)                                                                         \
    {                                                                                                                  \
        .pin = IOB_INDEX,                                                       /* */                                  \
            .alternate = Q_DRV_IOB_ALT_GET(IOB_INDEX, ANALOG_ANIO, ANIO_INDEX), /* */                                  \
    }

/** @brief Macro used to disable preset value. */
#define Q_DRV_GPADC_PRESET_VALUE_UNUSED 0xFFFFUL

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvGPADC_UpdateMode_t */
//@{
/** @brief Normal update mode.*/
#define qDrvGPADC_UpdateModeNormal qRegGPADC_BufferUpdateModeNormal
/** @brief Update only when the new value is lower than the current one. */
#define qDrvGPADC_UpdateModeMinHold qRegGPADC_BufferUpdateModeMinHold
/** @brief Update only when the new value is higher than the current one. */
#define qDrvGPADC_UpdateModeMaxHold qRegGPADC_BufferUpdateModeMaxHold
/** @brief Update only when the new value is out of given bounds. */
#define qDrvGPADC_UpdateModeOutOfBounds qRegGPADC_BufferUpdateModeOutOfRange
/** @typedef qDrvGPADC_UpdateMode_t
 *  @brief ADC output update mode.
 */
typedef qRegGPADC_BufferUpdateMode_t qDrvGPADC_UpdateMode_t;
//@}

/** @enum qDrvGPADC_VoltageMode_t */
//@{
/** @brief No applied scaler to the ADC input.*/
#define qDrvGPADC_VoltageModeLow 0UL
/** @brief Scaler 1/4 is applied to the ADC input. */
#define qDrvGPADC_VoltageModeHigh 1UL
/** @typedef qDrvGPADC_VoltageMode_t
 *  @brief ADC input voltage mode.
 */
typedef UInt8 qDrvGPADC_VoltageMode_t;
//@}

/** @enum qDrvGPADC_PostBuffer_t */
//@{
/** @brief The result of the measurement will be posted to the first ADC result buffer. */
#define qDrvGPADC_PostBufferA BM(0UL)
/** @brief The result of the measurement will be posted to the second ADC result buffer. */
#define qDrvGPADC_PostBufferB BM(1UL)
/** @brief The result of the measurement will be posted to the third ADC result buffer. */
#define qDrvGPADC_PostBufferC BM(2UL)
/** @brief The result of the measurement will be posted to the ADC result FIFO. */
#define qDrvGPADC_PostBufferFifo BM(3UL)
/** @brief The result of the measurement will be posted to the Analog Watchdog. */
#define qDrvGPADC_PostBufferAwd BM(4UL)
/** @typedef qDrvGPADC_PostBuffer_t
 *  @brief ADC post buffer.
 */
typedef UInt8 qDrvGPADC_PostBuffer_t;
//@}

/** @enum qDrvGPADC_Resolution_t */
//@{
/** @brief Result stored as 8 MSBs. */
#define qDrvGPADC_Resolution8Bit 0
/** @brief 11-bit result stored as 16-bit integer. */
#define qDrvGPADC_Resolution11Bit 1
/** @typedef qDrvGPADC_Resolution_t
 *  @brief ADC result value resolution.
 */
typedef UInt8 qDrvGPADC_Resolution_t;
//@}

/** @enum qDrvGPADC_AWDEvent_t */
//@{
/** @brief Monitored input channel voltage has dropped below threshold value. */
#define qDrvGPADC_AWDEventBelow BM(0)
/** @brief Monitored input channel voltage has exceeded threshold value. */
#define qDrvGPADC_AWDEventAbove BM(1)

#define qDrvGPADC_AWDEventAll (qDrvGPADC_AWDEventBelow | qDrvGPADC_AWDEventAbove)
/** @typedef qDrvGPADC_AWDEvent_t
 *  @brief Analog Watchdog event type.
 */
typedef UInt8 qDrvGPADC_AWDEvent_t;
//@}

/**
 * @brief Analog Watchdog event callback.
 *
 * @param[in] arg   User-defined argument.
 * @param[in] event AWD event type.
 */
typedef void (*qDrvGPADC_CbAWD_t)(void* arg, qDrvGPADC_AWDEvent_t event);

/**
 * @brief Fifo transfer callback.
 *
 * @param[in] arg    User-defined argument.
 * @param[in] buffer Sample buffer pointer.
 * @param[in] length Data length in samples count.
 */
typedef void (*qDrvGPADC_CbFifo_t)(void* arg, void* buffer, UInt16 length);

/**
 * @brief Callback triggered on completion of full measurement cycle (all input slots).
 *
 * @param[in] arg User-defined argument.
 */
typedef void (*qDrvGPADC_CbCycleDone_t)(void* arg);

/**
 * @brief Callback triggered on buffer update event.
 *
 * @param[in] arg    User-defined argument.
 * @param[in] buffer Updated buffer.
 */
typedef void (*qDrvGPADC_CbBufferUpdate_t)(void* arg, qRegGPADC_Buffer_t buffer, UInt16 result);

/* Callback set structure definition. */
typedef struct {
    qDrvGPADC_CbAWD_t awd;                   /**< Analog Watchdog event. */
    qDrvGPADC_CbFifo_t fifoThreshold;        /**< Fifo threshold event. */
    qDrvGPADC_CbCycleDone_t cycleDone;       /**< Measurement cycle complete. */
    qDrvGPADC_CbBufferUpdate_t bufferUpdate; /**< Buffer update complete. */
} qDrvGPADC_Cbs_t;

/** @brief GPADC common config structure. */
typedef struct {
    Bool dma; /**< True if the DMA mode is to be used, false otherwise. */
} qDrvGPADC_Config_t;

/** @brief GPADC input slot config structure. */
typedef struct {
    qRegGPADC_Channel_t pChannel;        /**< GPADC positive channel. */
    qRegGPADC_Channel_t nChannel;        /**< GPADC negative channel. Set @p qRegGPADC_ChannelNone if unused. */
    Bool diffMode;                       /**< True if the differential mode is to be enabled, false otherwise. */
    UInt8 waitTime;                      /**< Wait time which defines the sampling frequency.
                                              The wait time between channels is equal to
                                              62.5 ns * (waitTime * 2 + 11 + 64 * m), where
                                              m = 1 when there is more than one input channel
                                              enabled, otherwise m = 0. */
    qDrvGPADC_VoltageMode_t voltageMode; /**< Measured expected voltage range. */
    Bool higherSpeed; /**< True if the high speed mode is to be enabled, false otherwise. Available only in single ended
                         mode. */
    UInt8 filterCap;  /**< Internal RC filter capacitance. 0 to disable the filter. */
    qDrvGPADC_PostBuffer_t postBuffer; /**< GPADC post buffer mask. */
} qDrvGPADC_SlotConfig_t;

/** @brief GPADC preset values config structure. */
typedef struct {
    UInt16 min; /**< Minimum preset value. Use @p Q_DRV_GPADC_PRESET_VALUE_UNUSED to disable. */
    UInt16 max; /**< Maximum preset value. Use @p Q_DRV_GPADC_PRESET_VALUE_UNUSED to disable. */
} qDrvGPADC_Preset_t;

/** @brief GPADC output buffer config structure. */
typedef struct {
    qDrvGPADC_Resolution_t resolution; /**< Measurement resolution. */
    qDrvGPADC_UpdateMode_t updateMode; /**< Value update mode.*/
    Bool irqEnable;                    /**< Trigger an interrupt after every value update. */
    qDrvGPADC_Preset_t preset;         /**< Buffer preset values. */
} qDrvGPADC_BufferConfig_t;

/** @brief GPADC output fifo config structure. */
typedef struct {
    qDrvGPADC_Resolution_t resolution; /**< Measurement resolution. */
    UInt8 subsampleRate; /**< Rate at which samples are put into the FIFO (every fifo_subsample_rate + 1’th sample is
                            used). */
    void* buffer;        /**< Buffer pointer to be used by GPADC fifo. */
    UInt16 bufferSize;   /**< Buffer size in bytes. */
} qDrvGPADC_FifoConfig_t;

/** @brief GPADC analog watchdog config structure. */
typedef struct {
    qDrvGPADC_Preset_t preset; /**< Analog watchdog preset values. */
} qDrvGPADC_AwdConfig_t;

/** @brief GPADC voltage structure. */
typedef struct {
    Bool negative;     /**< True if the voltage is negative, false otherwise. */
    UInt8 integer;     /**< Integer part of the voltage. */
    UInt16 fractional; /**< Fractional part of the voltage. */
} qDrvGPADC_Voltage_t;

#if defined(REG_QUIC_PRESENT)
/** @brief GPADC QUIC destination configuration structure. */
typedef struct {
    UInt8 actionIdx;                       /**< QUIC action index. */
    qRegGPADC_QuicActionType_t actionType; /**< QUIC action type. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD();    /**< Required action field. */
} qDrvGPADC_QuicActionConfig_t;
#endif                                     /* REG_QUIC_PRESENT */

/** @brief GPADC instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvGPADC_t;

/** @brief Default configuration macro for GPADC instance. */
#define Q_DRV_GPADC_CONFIG_DEFAULT()                                                                                   \
    {                                                                                                                  \
        .dma = false, /* DMA mode is not used. */                                                                      \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the GPADC instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 * @param[in] pinCount  Pins count to be configured.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvGPADC_PinConfigSet(const qDrvIOB_PinAlt_t* const pPinConfig, UInt8 pinCount);

/**
 * @brief Deinitialize the GPADC instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 * @param[in] pinsCount  Pins count to be configured.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvGPADC_PinConfigClear(const qDrvIOB_PinAlt_t* const pPinConfig, UInt8 pinsCount);

/**
 * @brief Initialize GPADC peripheral.
 *
 * @param[in] pInstance   GPADC instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] pCallbacks  Set of callback functions pointer.
 * @param[in] arg         User-supplied argument passed to the callback function.
 * @param[in] irqPriority ARM interrupt priority.

 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_OUTOFMEMORY   Unable to allocate DMA channel.
 * @retval Q_ERR_NOTSUPPORTED  Selected channel is not available on this package.
 */
qResult_t qDrvGPADC_Init(qDrvGPADC_t* const pInstance, const qDrvGPADC_Config_t* const pConfig,
                         const qDrvGPADC_Cbs_t* const pCallbacks, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the GPADC instance.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvGPADC_Deinit(qDrvGPADC_t* const pInstance);

/**
 * @brief Check whether the GPADC instance is initialised.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvGPADC_InitCheck(qDrvGPADC_t* const pInstance);

/**
 * @brief Enable GPADC input slot number.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] slot      ADC input slot number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Slot is already enabled or not configured.
 */
qResult_t qDrvGPADC_SlotEnable(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot);

/**
 * @brief Disable GPADC input slot number.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] slot      ADC input slot number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Slot is already disabled.
 */
qResult_t qDrvGPADC_SlotDisable(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot);

/**
 * @brief Check whether the GPADC slot is enabled.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] slot      ADC input slot number.
 *
 * @retval True if the slot is enabled, false otherwise.
 */
Bool qDrvGPADC_SlotEnableCheck(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot);

/**
 * @brief Configure GPADC slot channel.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] slot      ADC input slot number.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Provided configuration is not correct.
 * @retval Q_ERR_INVSTATE      Slot is enabled.
 */
qResult_t qDrvGPADC_SlotConfigSet(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot,
                                  qDrvGPADC_SlotConfig_t* const pConfig);

/**
 * @brief Configure GPADC buffer channel.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] buffer    ADC output buffer number.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Buffer number is not correct.
 * @retval Q_ERR_GENERAL       Buffer callback function is not set.
 */
qResult_t qDrvGPADC_BufferConfigSet(qDrvGPADC_t* const pInstance, qRegGPADC_Buffer_t buffer,
                                    qDrvGPADC_BufferConfig_t* const pConfig);

/**
 * @brief Set GPADC buffer preset values.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] buffer    ADC output buffer number.
 * @param[in] pPreset   Preset configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvGPADC_BufferPresetSet(qDrvGPADC_t* const pInstance, qRegGPADC_Buffer_t buffer,
                                    qDrvGPADC_Preset_t* const pPreset);

/**
 * @brief Get raw result value from provided buffer.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] buffer    ADC output buffer number.
 *
 * @return Buffer result raw value.
 */
UInt16 qDrvGPADC_BufferRawResultGet(qDrvGPADC_t* const pInstance, qRegGPADC_Buffer_t buffer);

/**
 * @brief Convert raw result value to voltage value.
 *
 * @param[in] pInstance  GPADC instance pointer.
 * @param[in] raw        Buffer result raw value.
 * @param[in] resolution ADC result value resolution.
 * @param[in] slot       ADC input slot number.
 *
 * @return Voltage value.
 */
qDrvGPADC_Voltage_t qDrvGPADC_RawToVoltageConvert(qDrvGPADC_t* const pInstance, UInt16 raw,
                                                  qDrvGPADC_Resolution_t resolution, qRegGPADC_Slot_t slot);

/**
 * @brief Convert voltage value to raw buffer value.
 *
 * @param[in] pInstance  GPADC instance pointer.
 * @param[in] voltage    Buffer voltage value
 * @param[in] resolution ADC result value resolution.
 * @param[in] slot       ADC input slot number.
 *
 * @return Raw buffer value.
 */
UInt16 qDrvGPADC_VoltageToRawConvert(qDrvGPADC_t* const pInstance, qDrvGPADC_Voltage_t voltage,
                                     qDrvGPADC_Resolution_t resolution, qRegGPADC_Slot_t slot);

/**
 * @brief Configure GPADC fifo.
 *
 * @note DMA woks only if the fifo is properly configured.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Fifo is already enabled.
 * @retval Q_ERR_INVPARAM      Provided configuration is not correct.
 * @retval Q_ERR_GENERAL       Fifo callback function is not set.
 */
qResult_t qDrvGPADC_FifoConfigSet(qDrvGPADC_t* const pInstance, qDrvGPADC_FifoConfig_t* const pConfig);

/**
 * @brief Enable GPADC fifo.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Fifo is already enabled.
 * @retval Q_ERR_GENERAL       Fifo buffer is not set.
 */
qResult_t qDrvGPADC_FifoEnable(qDrvGPADC_t* const pInstance);

/**
 * @brief Disable GPADC fifo.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Fifo is already disabled
 */
qResult_t qDrvGPADC_FifoDisable(qDrvGPADC_t* const pInstance);

/**
 * @brief Check whether the GPADC fifo is enabled.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval True if the fifo is enabled, false otherwise.
 */
Bool qDrvGPADC_FifoEnableCheck(qDrvGPADC_t* const pInstance);

/**
 * @brief Flush GPADC fifo.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_GENERAL       Fifo threshold callback function is not set.
 * @retval Q_ERR_INVSTATE      Fifo buffer is not set.
 * @retval Q_ERR_OUTOFMEMORY   All data is already proceed.
 */
qResult_t qDrvGPADC_FifoFlush(qDrvGPADC_t* const pInstance);

/**
 * @brief Configure GPADC analog watchdog.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Wrong AWD limit values.
 * @retval Q_ERR_INVSTATE      AWD is enabled.
 * @retval Q_ERR_GENERAL       AWD callback function is not set.
 */
qResult_t qDrvGPADC_AwdConfigSet(qDrvGPADC_t* const pInstance, qDrvGPADC_AwdConfig_t* const pConfig);

/**
 * @brief Enable GPADC analog watchdog events.
 *
 * @note All events are one-shot and need to be re-enabled after the interrupt.
 *
 * @param[in] pInstance GPADC instance pointer.
 * @param[in] event     Event mask to be enabled.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      AWD is not configured.
 */
qResult_t qDrvGPADC_AwdEnable(qDrvGPADC_t* const pInstance, qDrvGPADC_AWDEvent_t event);

/**
 * @brief Enable GPADC analog watchdog.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      AWD is already disabled.
 */
qResult_t qDrvGPADC_AwdDisable(qDrvGPADC_t* const pInstance);

/**
 * @brief Check whether the GPADC analog watchdog is enabled.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval True if the analog watchdog is enabled, false otherwise.
 */
Bool qDrvGPADC_AwdEnableCheck(qDrvGPADC_t* const pInstance);

/**
 * @brief Start GPADC measurements in the continuous mode.
 *
 * @param[in] pInstance    GPADC instance pointer.
 * @param[in] irqCycleDone True if the interrupt cycle done is to be enabled, false otherwise.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_GENERAL       Cycle done callback function is not set.
 */
qResult_t qDrvGPADC_ContinuousConfigSet(qDrvGPADC_t* const pInstance, Bool irqCycleDone);

/**
 * @brief Start GPADC measurements in the continuous mode.
 *
 * @param[in] pInstance    GPADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Slots are not enabled. At least one slot should be enabled.
 * @retval Q_ERR_BUSY          Unable to start continuous mode. Quic mode already used.
 */
qResult_t qDrvGPADC_ContinuousStart(qDrvGPADC_t* const pInstance);

/**
 * @brief Stop GPADC continuous measurements.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvGPADC_ContinuousStop(qDrvGPADC_t* const pInstance);

/**
 * @brief Start GPADC measurements in the one shoot mode.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Continuous mode is already enabled or slots are not enabled.
 *                             At least one slot should be enabled.
 * @retval Q_ERR_BUSY          Unable to start continuous mode. Quic mode already used.
 * @retval Q_ERR_GENERAL       Cycle done callback function is not set.
 */
qResult_t qDrvGPADC_OneShootStart(qDrvGPADC_t* const pInstance);

/**
 * @brief Check whether there is an ongoing measurement.
 *
 * @param[in] pInstance GPADC instance pointer.
 *
 * @return True if there is an ongoing measurement, false otherwise.
 */
Bool qDrvGPADC_BusyCheck(qDrvGPADC_t* const pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_GPADC_H_
