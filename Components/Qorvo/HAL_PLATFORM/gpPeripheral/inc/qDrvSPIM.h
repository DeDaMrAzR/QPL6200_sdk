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

#ifndef _Q_DRV_SPIM_H_
#define _Q_DRV_SPIM_H_

/**
 * @defgroup Q_DRV_SPIM SPIM driver
 * @{
 *
 * @brief This is a driver for SPIM (SPI Master) peripheral.
 *
 * @file qDrvSPIM.h
 *
 */

/*****************************************************************************
 *                    Include Definitions
 *****************************************************************************/

#include "qRegSPIM.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvSPIM_ClkFreq_t */
//@{
#define qDrvSPIM_ClkFreq32M  qRegSPIM_ClkFreq32M
#define qDrvSPIM_ClkFreq16M  qRegSPIM_ClkFreq16M
#define qDrvSPIM_ClkFreq8M   qRegSPIM_ClkFreq8M
#define qDrvSPIM_ClkFreq4M   qRegSPIM_ClkFreq4M
#define qDrvSPIM_ClkFreq2M   qRegSPIM_ClkFreq2M
#define qDrvSPIM_ClkFreq1M   qRegSPIM_ClkFreq1M
#define qDrvSPIM_ClkFreq500k qRegSPIM_ClkFreq500k
#define qDrvSPIM_ClkFreq250k qRegSPIM_ClkFreq250k
/** @typedef qDrvSPIM_ClkFreq_t
 *  @brief SPIM clock frequency.
 */
typedef UInt8 qDrvSPIM_ClkFreq_t;
//@}

/** @enum qDrvSPIM_BitOrder_t */
//@{
/** @brief Most significant bit first.
 */
#define qDrvSPIM_BitOrderMSBFirst 0
/** @brief Least significant bit first.
 */
#define qDrvSPIM_BitOrderLSBFirst 1
/** @typedef qDrvSPIM_BitOrder_t
 *  @brief SPI Master bit order.
 */
typedef UInt8 qDrvSPIM_BitOrder_t;
//@}

/** @enum qDrvSPIM_Mode_t */
//@{
/** @brief CPOL=0,  CPHA=0: clock idle state is low, data shifted on
 *         falling (inactive) clock edge / sampled on rising edge.
 */
#define qDrvSPIM_Mode0 qRegSPIM_Mode0
/** @brief CPOL=0,  CPHA=1: clock idle state is low, data shifted on
 *         rising (inactive) clock edge / sampled on falling edge.
 */
#define qDrvSPIM_Mode1 qRegSPIM_Mode1
/** @brief CPOL=1,  CPHA=0: clock idle state is high, data shifted on
 *         rising (active) clock edge / sampled on falling edge.
 */
#define qDrvSPIM_Mode2 qRegSPIM_Mode2
/** @brief CPOL=1,  CPHA=1: clock idle state is high, data shifted on
 *         falling (active) clock edge / sampled on rising edge.
 */
#define qDrvSPIM_Mode3 qRegSPIM_Mode3
/** @typedef qDrvSPIM_Mode_t
 *  @brief SPIM mode.
 */
typedef UInt8 qDrvSPIM_Mode_t;
//@}

/** @enum qDrvSPIM_TransferMode_t */
//@{
/** @brief Interrupt configuration. */
#define qDrvSPIM_TransferModeInterrupt 1
/** @brief DMA configuration. */
#define qDrvSPIM_TransferModeDMA 2
/** @typedef qDrvSPIM_TransferMode_t
 *  @brief SPIM transmitter mode configuration.
 */
typedef UInt8 qDrvSPIM_TransferMode_t;
//@}

/** @enum qDrvSPIM_DataBits_t */
//@{
#define qDrvSPIM_DataBits8  qRegSPIM_DataBits8
#define qDrvSPIM_DataBits16 qRegSPIM_DataBits16
#define qDrvSPIM_DataBits32 qRegSPIM_DataBits32
/** @typedef qDrvSPIM_DataBits_t
 *  @brief SPI frame size.
 */
typedef qRegSPIM_DataBits_t qDrvSPIM_DataBits_t;
//@}

/** @enum qDrvSPIM_Mode_t */
//@{
/** @brief CPOL=0,  CPHA=0: clock idle state is low, data shifted on
 *         falling (inactive) clock edge / sampled on rising edge.
 */
#define qDrvSPIM_Mode0 qRegSPIM_Mode0
/** @brief CPOL=0,  CPHA=1: clock idle state is low, data shifted on
 *         rising (inactive) clock edge / sampled on falling edge.
 */
#define qDrvSPIM_Mode1 qRegSPIM_Mode1
/** @brief CPOL=1,  CPHA=0: clock idle state is high, data shifted on
 *         rising (active) clock edge / sampled on falling edge.
 */
#define qDrvSPIM_Mode2 qRegSPIM_Mode2
/** @brief CPOL=1,  CPHA=1: clock idle state is high, data shifted on
 *         falling (active) clock edge / sampled on rising edge.
 */
#define qDrvSPIM_Mode3 qRegSPIM_Mode3
/** @typedef qDrvSPIM_Mode_t
 *  @brief SPIM mode.
 */
typedef UInt8 qDrvSPIM_Mode_t;
//@}

/** @brief SPIM internal data structure. */
typedef struct {
    UInt8 data[30];
} __attribute__((aligned(4))) qDrvSPIM_InternalData_t;

/** @brief SPIM instance structure */
typedef struct {
    const qRegSPIM_t baseAddr;            /**< Peripheral base address. */
    qDrvSPIM_InternalData_t internalData; /**< Internal driver data. */
    const UInt8 id;                       /**< Peripheral instance ID. */
} qDrvSPIM_t;

/** @brief SPIM transfer configuration structure */
typedef struct {
    qDrvSPIM_ClkFreq_t clkFreq;   /**< SPI Master clock frequency divider; f = SYS_CLK / freqDiv.*/
    qDrvSPIM_Mode_t mode;         /**< SPI mode. */
    qDrvSPIM_BitOrder_t bitOrder; /**< Bit order within a SPI frame. MSB or LSB first. */
    qDrvSPIM_DataBits_t dataBits; /**< SPI frame size. */
} qDrvSPIM_TransferConfig_t;

/** @brief SPIM transfer structure */
typedef struct {
    const UInt8* txBuf; /**< TX buffer pointer. */
    UInt8* rxBuf;       /**< RX buffer pointer. */
    UInt16 txLen;       /**< TX buffer length. */
    UInt16 rxLen;       /**< RX buffer length. */
} qDrvSPIM_Transfer_t;

/** @brief SPIM config structure. */
typedef struct {
    qDrvSPIM_TransferMode_t transferMode;     /**< Transfer mode. */
    qDrvSPIM_TransferConfig_t transferConfig; /**< Transfer configuration. */
} qDrvSPIM_Config_t;

/** @brief SPIM pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t mosi;    /**< MOSI pin/alternate pair. */
        qDrvIOB_PinAlt_t miso;    /**< MISO pin/alternate pair. */
        qDrvIOB_PinAlt_t sck;     /**< SCK pin/alternate pair. */
        qDrvIOB_PinAlt_t nss;     /**< NSS pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[4]; /**< Internal pin config array. */
} qDrvSPIM_PinConfig_t;

/**
 * @brief Transfer complete callback.
 *
 * @param[in] pInstance SPIM instance pointer.
 * @param[in] pTransfer RX/TX buffers.
 */
typedef void (*qDrvSPIM_Callback_t)(void* arg, qDrvSPIM_Transfer_t* const pTransfer);

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/**
 * @brief Macro for defining SPIM instance. */
#define Q_DRV_SPIM_INSTANCE_DEFINE(ID)                                                                                 \
    {                                                                                                                  \
        .baseAddr = Q_REG_SPIM_BASEADDR_GET(ID), /* */                                                                 \
            .internalData = {},                  /* */                                                                 \
            .id = ID,                            /* */                                                                 \
    }

/** @brief Default configuration macro for SPIM instance. */
#define Q_DRV_SPIM_CONFIG_DEFAULT()                                                                                    \
    {                                                                                                                  \
        .transferMode = qDrvSPIM_TransferModeDMA,      /* */                                                           \
            .transferConfig = {                                                                                        \
                .clkFreq = qDrvSPIM_ClkFreq250k,       /* */                                                           \
                .mode = qDrvSPIM_Mode0,                /* */                                                           \
                .bitOrder = qDrvSPIM_BitOrderMSBFirst, /* */                                                           \
                .dataBits = qDrvSPIM_DataBits8,        /* */                                                           \
            }                                                                                                          \
    }

#define _Q_DRV_SPIM_PIN_CONFIG3(ID, MOSI_PIN, MISO_PIN, SCK_PIN)                                                       \
    {                                                                                                                  \
        .mosi = {.pin = MOSI_PIN, .alternate = Q_DRV_IOB_ALT_GET(MOSI_PIN, SPI_M, MOSI)},     /* */                    \
            .miso = {.pin = MISO_PIN, .alternate = Q_DRV_IOB_ALT_GET(MISO_PIN, SPI_M, MISO)}, /* */                    \
            .sck = {.pin = SCK_PIN, .alternate = Q_DRV_IOB_ALT_GET(SCK_PIN, SPI_M, SCLK)},    /* */                    \
            .nss = {.pin = Q_NOPIN},                                                          /* */                    \
    }

#define _Q_DRV_SPIM_PIN_CONFIG4(ID, MOSI_PIN, MISO_PIN, SCK_PIN, SSN_PIN)                                              \
    {                                                                                                                  \
        .mosi = {.pin = MOSI_PIN, .alternate = Q_DRV_IOB_ALT_GET(MOSI_PIN, SPI_M, MOSI)},     /* */                    \
            .miso = {.pin = MISO_PIN, .alternate = Q_DRV_IOB_ALT_GET(MISO_PIN, SPI_M, MISO)}, /* */                    \
            .sck = {.pin = SCK_PIN, .alternate = Q_DRV_IOB_ALT_GET(SCK_PIN, SPI_M, SCLK)},    /* */                    \
            .nss = {.pin = SSN_PIN, .alternate = Q_DRV_IOB_ALT_GET(SSN_PIN, SPI_M, SSN)},     /* */                    \
    }

/**
 * @brief Macro for defining SPIM pin configuration.
 *
 * @param[in] ID       SPIM instance ID.
 * @param[in] MOSI_PIN MOSI pin index.
 * @param[in] MISO_PIN MISO pin index.
 * @param[in] SCK_PIN  SCK pin index.
 * @param[in] SSN_PIN  Optional NSS pin index in case of hardware select.
 */
#define Q_DRV_SPIM_PIN_CONFIG(ID, ...) Q_CONCAT(_Q_DRV_SPIM_PIN_CONFIG, Q_COUNT_ARGS(__VA_ARGS__))(ID, __VA_ARGS__)

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the SPIM instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvSPIM_PinConfigSet(const qDrvSPIM_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the SPIM instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvSPIM_PinConfigClear(const qDrvSPIM_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize the SPIM instance.
 *
 * @param[in] pInstance   SPIM instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] callback    Callback function.
 * @param[in] arg         User-supplied argument passed to the callback function.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK              Success.
 * @retval Q_ERR_INVSTATE    Peripheral already initialized.
 * @retval Q_ERR_OUTOFMEMORY DMA channel allocation failed.
 */
qResult_t qDrvSPIM_Init(const qDrvSPIM_t* pInstance, const qDrvSPIM_Config_t* const pConfig,
                        const qDrvSPIM_Callback_t callback, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the SPI Master instance.
 *
 * @param[in] pInstance SPIM instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvSPIM_Deinit(const qDrvSPIM_t* pInstance);

/**
 * @brief COnfigure SPI master transfer.
 *
 * @param[in] pInstance SPIM instance pointer.
 * @param[in] pConfig   Transfer configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Transmission already in progress.
 * @retval Q_ERR_INVPARAM      Invalid config parameters.
 */
qResult_t qDrvSPIM_TransferConfigSet(const qDrvSPIM_t* pInstance, const qDrvSPIM_TransferConfig_t* const pConfig);

/**
 * @brief Start SPI transfer.
 *
 * @param[in] pInstance SPIM instance pointer.
 * @param[in] pTransfer Set of buffers.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Transmission already in progress.
 * @retval Q_ERR_INVPARAM      Invalid transfer parameters.
 */
qResult_t qDrvSPIM_Transfer(const qDrvSPIM_t* pInstance, const qDrvSPIM_Transfer_t* const pTransfer);

/**
 * @brief Stop ongoing transfer.
 *
 * @param[in] pInstance SPIM instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Peripheral not transmitting data.
 */
qResult_t qDrvSPIM_Abort(const qDrvSPIM_t* pInstance);
/**
 * @brief Check if there is a transfer running.
 *
 * @param[in] pInstance SPIM instance pointer.
 *
 * @returns Transfer status.
 */
Bool qDrvSPIM_BusyCheck(const qDrvSPIM_t* pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_SPIM_H_
