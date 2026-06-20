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

#ifndef _Q_DRV_SPIS_H_
#define _Q_DRV_SPIS_H_

/**
 * @defgroup Q_DRV_SPIS SPIS driver
 * @{
 *
 * @brief This is a driver for SPIS (SPI Slave) peripheral.
 *
 * @file qDrvSPIS.h
 *
 */

/*****************************************************************************
 *                    Include Definitions
 *****************************************************************************/

#include "qRegSPIS.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvSPIS_Error_t */
//@{
/** @brief RX overrun error. Triggered when on RX buffer overflow.
 */
#define qDrvSPIS_ErrorRxOverrun 0
/** @typedef qDrvSPIS_Error_t
 *  @brief SPI Slave error type.
 */
typedef UInt8 qDrvSPIS_Error_t;
//@}

/** @brief SPIS internal data structure. */
typedef struct {
    UInt8 data[26];
} __attribute__((aligned(4))) qDrvSPIS_InternalData_t;

/** @brief SPIS instance structure */
typedef struct {
    const qRegSPIS_t baseAddr;            /**< Peripheral base address. */
    qDrvSPIS_InternalData_t internalData; /**< Internal driver data. */
    const UInt8 id;                       /**< Peripheral instance ID. */
} qDrvSPIS_t;

/** @brief SPI Slave config structure. */
typedef struct {
    Bool halfDuplex;        /**< Half-duplex mode. When enabled, MOSI pin will be bi-directional. */
    Bool clearTxOnDeselect; /**< Clear Tx FIFO. When enabled, the tx data byte buffer will be cleared when the ssn pin
                               is inactive. */
    Bool clearRxOnDeselect; /**< Clear Rx FIFO. When enabled, the rx data byte buffer will be cleared when the ssn pin
                               is inactive. */
} qDrvSPIS_Config_t;

/** @brief SPIS pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t mosi;    /**< MOSI pin/alternate pair. */
        qDrvIOB_PinAlt_t miso;    /**< MISO pin/alternate pair. */
        qDrvIOB_PinAlt_t sck;     /**< SCK pin/alternate pair. */
        qDrvIOB_PinAlt_t ss;      /**< SS pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[4]; /**< Internal pin config array. */
} qDrvSPIS_PinConfig_t;

/**
 * @brief SPIS data reception callback.
 *
 * @param[in] pInstance SPIS instance pointer.
 * @param[in] pBuf      Received data buffer pointer.
 * @param[in] len       Number of bytes received.
 */
typedef void (*qDrvSPIS_CbRx_t)(void* arg, UInt8* pBuf, UInt8 len);

/**
 * @brief SPIS data transmission complete callback.
 *
 * @param[in] pInstance SPIS instance pointer.
 * @param[in] pBuf      Transmitted data buffer pointer.
 * @param[in] len       Number of transmitted received.
 */
typedef void (*qDrvSPIS_CbTx_t)(void* arg, UInt8* pBuf, UInt8 len);

/**
 * @brief SPIS error callback.
 *
 * @param[in] pInstance SPIS instance pointer.
 * @param[in] error     Error type.
 */
typedef void (*qDrvSPIS_CbError_t)(void* arg, qDrvSPIS_Error_t error);

/* Callback set structure definition. */
typedef struct {
    qDrvSPIS_CbRx_t rx;
    qDrvSPIS_CbTx_t tx;
    qDrvSPIS_CbError_t error;
} qDrvSPIS_Callbacks_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/**
 * @brief Macro for defining SPIS instance. */
#define Q_DRV_SPIS_INSTANCE_DEFINE(ID)                                                                                 \
    {                                                                                                                  \
        .baseAddr = Q_REG_SPIS_BASEADDR_GET(ID), /* */                                                                 \
            .internalData = {},                  /* */                                                                 \
            .id = ID,                            /* */                                                                 \
    }

/** @brief Default configuration macro for SPIS instance. */
#define Q_DRV_SPIS_CONFIG_DEFAULT()                                                                                    \
    {                                                                                                                  \
        .halfDuplex = false,           /* */                                                                           \
            .clearTxOnDeselect = true, /* */                                                                           \
            .clearRxOnDeselect = true, /* */                                                                           \
    }

/** @brief Pin configuration macro for SPIS instance. */
#define Q_DRV_SPIS_PIN_CONFIG(ID, MOSI_PIN, MISO_PIN, SCK_PIN, SS_PIN)                                                 \
    {                                                                                                                  \
        .mosi = {.pin = MOSI_PIN, .alternate = Q_DRV_IOB_ALT_GET(MOSI_PIN, SPI_SL, MOSI)},     /* */                   \
            .miso = {.pin = MISO_PIN, .alternate = Q_DRV_IOB_ALT_GET(MISO_PIN, SPI_SL, MISO)}, /* */                   \
            .sck = {.pin = SCK_PIN, .alternate = Q_DRV_IOB_ALT_GET(SCK_PIN, SPI_SL, SCLK)},    /* */                   \
            .ss = {.pin = SS_PIN, .alternate = Q_DRV_IOB_ALT_GET(SS_PIN, SPI_SL, SSN)},        /* */                   \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the SPIS instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvSPIS_PinConfigSet(const qDrvSPIS_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the SPIS instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvSPIS_PinConfigClear(const qDrvSPIS_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize SPI Slave instance.
 *
 * @param[in] pInstance   SPIS instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] pCallbacks  Callback function pointer set.
 * @param[in] arg         User-supplied argument passed to the callback functions.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK              Success.
 * @retval Q_ERR_INVSTATE    Peripheral already initialized.
 * @retval Q_ERR_INVPARAM    Invalid configuration.
 * @retval Q_ERR_OUTOFMEMORY Unable to allocate DMA channels.
 */
qResult_t qDrvSPIS_Init(const qDrvSPIS_t* pInstance, const qDrvSPIS_Config_t* const pConfig,
                        const qDrvSPIS_Callbacks_t* const pCallbacks, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the SPI Slave instance.
 *
 * @param[in] pInstance SPIS instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvSPIS_Deinit(const qDrvSPIS_t* pInstance);

/**
 * @brief Set the reception buffer. Required for peripheral operation.
 *
 * @param[in] pInstance SPIS instance pointer.
 * @param[in] pBuf      RX buffer pointer.
 * @param[in] bufLen    RX buffer length.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          RX already enabled.
 * @retval Q_ERR_INVPARAM      Buffer size too short or DMA channel is not allocated.
 */
qResult_t qDrvSPIS_RxBufferSet(qDrvSPIS_t* const pInstance, UInt8* pBuf, UInt16 bufLen);

/**
 * @brief Enable the SPI Slave peripheral.
 *        User must set the RX buffer with @p qDrvSPIS_RxBufferSet call
 *        prior to using this function.
 *
 * @param[in] pInstance SPIS instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      Peripheral already enabled.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Buffer not set or DMA channel not allocated.
 */
qResult_t qDrvSPIS_Enable(const qDrvSPIS_t* pInstance);

/**
 * @brief Disable the peripheral and stop data reception.
 *
 * @param[in] pInstance SPIS instance pointer.
 *
 * @retval Q_OK                On success.
 * @retval Q_ERR_INVSTATE      Peripheral not enabled.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvSPIS_Disable(const qDrvSPIS_t* pInstance);

/**
 * @brief Transmit the buffer during the next SPI transaction.
 *
 * @param[in] pInstance SPIS instance pointer.
 * @param[in] pBuf      TX buffer pointer.
 * @param[in] bufLen    TX buffer length.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid arguments.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Transfer already in progress.
 * @retval Q_ERR_INVSTATE      Transmit buffer not set.
 */
qResult_t qDrvSPIS_Tx(qDrvSPIS_t* const pInstance, UInt8* pBuf, UInt16 bufLen);

/**
 * @brief Cancel the scheduled transmit operation.
 *
 * @param[in] pInstance SPIS instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Peripheral not transmitting data.
 */
qResult_t qDrvSPIS_TxAbort(const qDrvSPIS_t* pInstance);

/**
 * @brief Set the SPI slave TX latency.
 *        By setting a non-zero value, the peripheral will start transmitting data
 *        exactly after a desired number of bytes. This feature assures valid timing
 *        in a protocol, where the SPI master request and slave response is transmitted
 *        within the same frame (slave select signal asserted).
 *
 * @param[in] pInstance SPIS instance pointer.
 * @param[in] txLatency New TX latency in bytes.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral not initialized.
 */
qResult_t qDrvSPIS_TxLatencySet(const qDrvSPIS_t* pInstance, UInt8 txLatency);

/**
 * @brief Check if there is a TX operation scheduled.
 *
 * @param[in] pInstance SPIS instance pointer.
 *
 * @return True if TX operation is in progress, false otherwise.
 */
Bool qDrvSPIS_TxBusyCheck(const qDrvSPIS_t* pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_SPIS_H_
