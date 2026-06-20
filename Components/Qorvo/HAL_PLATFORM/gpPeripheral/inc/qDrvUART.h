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

#ifndef _Q_DRV_UART_H_
#define _Q_DRV_UART_H_

/**
 * @defgroup Q_DRV_UART UART driver
 * @{
 *
 * @brief This is a driver for UART peripheral.
 *        It provides an API for RX/TX transfers with optional use of the DMA.
 *
 * To use the driver, user must create a driver instance using predefined macro,
 * providing user-defined name and physical instance number:
 *
 * @code
 *
 * qDrvUART_t uart0 = Q_DRV_UART_INSTANCE_DEFINE(0);
 *
 * @endcode
 *
 * An instance must be initialized with @p qDrvUART_Init call, which requires
 * user to provide instance configuration (@p qDrvUART_Config_t), a set
 * of callbacks (@p qDrvUART_Callbacks_t), user defined argument and the interrupt priority.
 * Optional default configuration is provided with @p Q_DRV_UART_CONFIG_DEFAULT macro.
 * If @p qDrvUART_Init succeeds, rest of the API functions become available.
 *
 * @file qDrvUART.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegUART.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvUART_ErrMask_t */
//@{
/** @brief Buffer overrun error. */
#define qDrvUART_ErrMaskRxOverrun (1UL << 0UL)
/** @brief Parity error. */
#define qDrvUART_ErrMaskRxParity (1UL << 1UL)
/** @brief Framing error. */
#define qDrvUART_ErrMaskRxFraming (1UL << 2UL)
/** @typedef qDrvUART_ErrMask_t
 *  @brief Error type mask.
 */
typedef UInt8 qDrvUART_ErrMask_t;
//@}

/** @brief UART config structure. */
typedef struct {
    UInt32 baudrate;                /**< Baudrate. */
    qRegUART_Bitlength_t bitlength; /**< Data length in bits. */
    qRegUART_Parity_t parity;       /**< Use parity bit. */
    qRegUART_Stopbits_t stopbits;   /**< Number of stop bits. */
    Bool txDma;                     /**< True if the TX DMA is to be enabled, false otherwise. */
    Bool rxDma;                     /**< True if the RX DMA is to be enabled, false otherwise. */
    Bool hwfc;                      /**< True if the hardware flow control is to be enabled, false otherwise. */
    Bool noRxAdvance;               /**< True if the RX buffer should not be advanced, false otherwise. */
} qDrvUART_Config_t;

/**
 * @brief TX complete callback.
 *
 * @param[in] arg    User defined argument set with the @p qDrvUART_Init() function.
 * @param[in] pData  Pointer to the transferred data buffer.
 * @param[in] length Buffer size in bytes.
 */
typedef void (*qDrvUART_CbTx_t)(void* arg, UInt8* pData, UInt16 length);

/**
 * @brief RX complete callback.
 *
 * @param[in] arg    User defined argument set with the @p qDrvUART_Init() function.
 * @param[in] pData  Pointer to the received data buffer.
 * @param[in] length Buffer size in bytes.
 */
typedef void (*qDrvUART_CbRx_t)(void* arg, UInt8* pData, UInt16 length);

/**
 * @brief Transmission error callback.
 *
 * @param[in] arg     User defined argument set with the @p qDrvUART_Init() function.
 * @param[in] errMask Mask of detected errors since the last callback call.
 */
typedef void (*qDrvUART_CbError_t)(void* arg, qDrvUART_ErrMask_t errMask);

/** @brief Callback structure definition. */
typedef struct {
    qDrvUART_CbTx_t tx;       /**< Data transmission complete event handler. */
    qDrvUART_CbRx_t rx;       /**< Data reception event handler. */
    qDrvUART_CbError_t error; /**< Data error event handler. */
} qDrvUART_Callbacks_t;

/** @brief Internal data buffer array. */
typedef struct {
    UInt32 buffer[9];
} qDrvUART_InternalData_t;

/** @brief UART instance structure */
typedef struct {
    const UInt8 id;                       /**< Peripheral instance ID. */
    const qRegUART_t baseAddr;            /**< Base address of the peripheral. */
    qDrvUART_InternalData_t internalData; /**< Internal data buffer. Should not be modified by the user. */
} qDrvUART_t;

/** @brief UART pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t tx;  /**< TX pin/alternate pair. */
        qDrvIOB_PinAlt_t rx;  /**< RX pin/alternate pair. */
        qDrvIOB_PinAlt_t cts; /**< CTS pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[3]; /**< Internal pin config array. */
} qDrvUart_PinConfig_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/** @brief Macro for defining UART instance. */
#define Q_DRV_UART_INSTANCE_DEFINE(ID)                                                                                 \
    {                                                                                                                  \
        .id = ID,                                     /* Initialize the ID of the peripheral instance. */              \
            .baseAddr = Q_REG_UART_BASE_ADDR_GET(ID), /* Initialize the base address of the peripheral instance. */    \
    }

/** @brief Default configuration macro for UART instance. */
#define Q_DRV_UART_CONFIG_DEFAULT(BAUDRATE)                                                                            \
    {                                                                                                                  \
        .baudrate = BAUDRATE,                 /* Baudrate. */                                                          \
            .bitlength = qRegUART_Bitlength8, /* Bit length. */                                                        \
            .parity = qRegUART_ParityNone,    /* Frame parity. */                                                      \
            .stopbits = qRegUART_StopbitsOne, /* Number of frame bit stops. */                                         \
            .txDma = true,                    /* Transmitter DMA mode enabled. */                                      \
            .rxDma = false,                   /* Receiver DMA mode disabled. */                                        \
            .hwfc = false,                    /* Hardware flow control. */                                             \
    }

#define _Q_DRV_UART_PIN_CONFIG1(INSTANCE_ID, TX_PIN)                                                                   \
    {                                                                                                                  \
        .tx = {.pin = TX_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(TX_PIN, UART, INSTANCE_ID, TX)}, /* */              \
            .rx = {.pin = Q_NOPIN},                                                                 /* */              \
            .cts = {.pin = Q_NOPIN},                                                                /* */              \
    }

#define _Q_DRV_UART_PIN_CONFIG2(INSTANCE_ID, TX_PIN, RX_PIN)                                                           \
    {                                                                                                                  \
        .tx = {.pin = TX_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(TX_PIN, UART, INSTANCE_ID, TX)},     /* */          \
            .rx = {.pin = RX_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(RX_PIN, UART, INSTANCE_ID, RX)}, /* */          \
            .cts = {.pin = Q_NOPIN},                                                                    /* */          \
    }

#define _Q_DRV_UART_PIN_CONFIG3(INSTANCE_ID, TX_PIN, RX_PIN, CTS_PIN)                                                  \
    {                                                                                                                  \
        .tx = {.pin = TX_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(TX_PIN, UART, INSTANCE_ID, TX)},         /* */      \
            .rx = {.pin = RX_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(RX_PIN, UART, INSTANCE_ID, RX)},     /* */      \
            .cts = {.pin = CTS_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(CTS_PIN, UART, INSTANCE_ID, CTS)}, /* */      \
    }

/**
 * @brief Macro for defining instance pin configuration.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] ...         Pins to be configured. In order: TX pin, RX pin (optional), CTS pin (optional).
 */
#define Q_DRV_UART_PIN_CONFIG(INSTANCE_ID, ...)                                                                        \
    Q_CONCAT_2(_Q_DRV_UART_PIN_CONFIG, Q_COUNT_ARGS(__VA_ARGS__))(INSTANCE_ID, __VA_ARGS__)

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the UART instance pins.
 *
 * @param[in] pPinConfig Pointer to the pin configuration structure.
 *
 * @retval Q_OK            Success.
 * @retval Q_ERR_INVSTATE  Pin(s) already in use.
 */
qResult_t qDrvUART_PinConfigSet(qDrvUart_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the UART instance pins.
 *
 * @param[in] pPinConfig Pointer to the pin configuration structure.
 *
 * @retval Q_OK            Success.
 * @retval Q_ERR_INVSTATE  At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvUART_PinConfigClear(qDrvUart_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize the UART instance.
 *
 * @param[in] pInstance         UART instance pointer.
 * @param[in] pConfig           Pointer to the configuration structure.
 * @param[in] pCallbacks        Pointer to set of callback functions.
 * @param[in] arg               Pointer to user defined argument.
 * @param[in] interruptPriority Priority of the interrupt.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Provided configuration is not correct.
 * @retval Q_ERR_INVSTATE      Instance is already initialized.
 * @retval Q_ERR_OUTOFMEMORY   Unable to allocate DMA channel.
 * @retval Q_ERR_NOTSUPPORTED  Configuration is not supported.
 */
qResult_t qDrvUART_Init(qDrvUART_t* const pInstance, const qDrvUART_Config_t* const pConfig,
                        const qDrvUART_Callbacks_t* const pCallbacks, void* arg, UInt32 interruptPriority);

/**
 * @brief Deinitialize the UART instance.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval Q_OK       Success.
 * @retval Q_ERR_BUSY Uart is already transmitting or receiving.
 */
qResult_t qDrvUART_Deinit(qDrvUART_t* const pInstance);

/**
 * @brief Check whether the UART instance is initialised.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvUART_InitCheck(qDrvUART_t* const pInstance);

/**
 * @brief Helper set the UART instance callbacks. This function overrides the previous callbacks.
 *
 * @note Calling this function is not required if the callbacks are set during the initialization.
 *
 * @param[in] pInstance  UART instance pointer.
 * @param[in] pCallbacks Pointer to the callback functions.
 * @param[in] arg        Pointer to user defined argument.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Uart is already transmitting or receiving.
 */
qResult_t qDrvUART_CallbackSet(qDrvUART_t* const pInstance, const qDrvUART_Callbacks_t* const pCallbacks, void* arg);

/**
 * @brief Enable the UART instance interrupts.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_IrqEnable(qDrvUART_t* const pInstance);

/**
 * @brief Disable the UART instance interrupts.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_IrqDisable(qDrvUART_t* const pInstance);

/**
 * @brief Check whether the UART instance interrupts are enabled.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval True if the interrupts are enabled, false otherwise.
 */
Bool qDrvUART_IrqEnabledCheck(qDrvUART_t* const pInstance);

/**
 * @brief Start the data transmission in the polling mode.
 *
 * @param[in] pInstance UART instance pointer.
 * @param[in] pData     Pointer to the transmission data buffer.
 * @param[in] length    Data buffer size in bytes.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Wrong buffer size.
 * @retval Q_ERR_BUSY          Previous transmission is already in progress.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_Tx(qDrvUART_t* const pInstance, const UInt8* const pData, UInt16 length);

/**
 * @brief Start the data transmission in interrupt mode.
 *
 * @note This function needs to be called only if interrupts are enabled with @p qDrvUART_IrqEnable function.
 *       The transmission is done in the UART interrupt context.
 *
 * @param[in] pInstance UART instance pointer.
 * @param[in] pData     Pointer to the transmission data buffer.
 * @param[in] length    Data buffer size in bytes.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_BUSY          Previous transmission is already in progress.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_TxIrq(qDrvUART_t* const pInstance, const UInt8* const pData, UInt16 length);

/**
 * @brief Start the data transmission in DMA mode.
 *
 * @note This function needs to be called only if the DMA is enabled with @p qDrvUART_Init function and interrupts
 *       are enabled with @p qDrvUART_IrqEnable function.
 *       The transmission is done in the DMA or UART interrupt context.
 *
 * @param[in] pInstance UART instance pointer.
 * @param[in] pData     Pointer to the transmission data buffer.
 * @param[in] length    Data buffer size in bytes.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Wrong buffer size.
 * @retval Q_ERR_INVSTATE      DMA is not initialized.
 * @retval Q_ERR_BUSY          Previous transmission is already in progress.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_TxDMA(qDrvUART_t* const pInstance, const UInt8* const pData, UInt16 length);

/**
 * @brief Check whether there is an ongoing transmission.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @returns True if there is an ongoing transmission, false otherwise.
 */
Bool qDrvUART_TxBusyCheck(qDrvUART_t* const pInstance);

/**
 * @brief Check whether the transmission buffer is empty.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @returns True if the buffer is empty, false otherwise.
 */
Bool qDrvUART_TxFifoEmptyCheck(qDrvUART_t* const pInstance);

/**
 * @brief Abort the ongoing interrupt transmission.
 *
 * @param[in] pInstance    UART instance pointer.
 * @param[in] pAlreadySend Pointer to the variable where the number of already sent bytes will be written, can be NULL.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      There is no transmission to be aborted.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_TxIrqAbort(qDrvUART_t* const pInstance, UInt16* pAlreadySend);

/**
 * @brief Abort the ongoing DMA transmission.
 *
 * @param[in] pInstance    UART instance pointer.
 * @param[in] pAlreadySend Pointer to the variable where the number of already sent bytes will be written, can be NULL.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      There is no transmission to be aborted.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_TxDMAAbort(qDrvUART_t* const pInstance, UInt16* pAlreadySend);

/**
 * @brief Get the pointer to the available chunk data in the buffer since the last transmission start.
 *
 * @param[in]  pInstance  UART instance pointer.
 * @param[out] pByteCount Pointer to the variable where the available chunk size in bytes will be written.
 *
 * @return Pointer to the available buffer chunk. NULL if there is no remaining data.
 */
UInt8* qDrvUART_TxRemainingChunkGet(qDrvUART_t* const pInstance, UInt16* pByteCount);

#ifndef REG_UART_NO_RX

/**
 * @brief Enable the receiver.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_BUSY          There is the ongoing reception.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_RxEnable(qDrvUART_t* const pInstance);

/**
 * @brief Disable the receiver.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      There is no ongoing reception.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_RxDisable(qDrvUART_t* const pInstance);

/**
 * @brief Check whether the receiver is enabled.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @return True if the receiver is enabled, false otherwise.
 */
Bool qDrvUART_RxEnableCheck(qDrvUART_t* const pInstance);

/**
 * @brief Get last received data from the RX FIFO in the pooling mode.
 *
 * @note This function needs to be called only if the receiver is enabled with @p qDrvUART_RxEnable function.
 *
 * @param[in]  pInstance UART instance pointer.
 * @param[out] pData     Pointer to the received data.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      No data in the RX FIFO.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_Rx(qDrvUART_t* const pInstance, UInt8* const pData);

/**
 * @brief Enable the receiver of the UART instance with the interrupt.
 *
 * @note This function needs to be called only if the receiver is enabled with @p qDrvUART_RxEnable function.
 *
 * @param[in] pInstance UART instance pointer.
 * @param[in] pData     Pointer to the data receiver buffer.
 *                      If NULL, the buffer is not used and each received byte will trigger the RX callback.
 * @param[in] length    Data buffer size in bytes. If the buffer is not used, this parameter is ignored.
 * @param[in] threshold Received bytes threshold value for calling the RX callback.
 *                      If the buffer is not used, this parameter is ignored.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_BUSY          There is the ongoing reception.
 * @retval Q_ERR_INVPARAM      Wrong threshold value.
 * @retval Q_ERR_INVSTATE      Async mode is not set.
 * @retval Q_ERR_OUTOFMEMORY   RX buffer is not set.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_RxIrq(qDrvUART_t* const pInstance, UInt8* const pData, UInt16 length, UInt16 threshold);

/**
 * @brief Enable the receiver of the UART instance with the DMA.
 *
 * @note This function needs to be called only if the receiver is enabled with @p qDrvUART_RxEnable function.
 *
 * @param[in] pInstance UART instance pointer.
 * @param[in] pData     Pointer to the data receiver buffer. Can not be NULL.
 * @param[in] length    Data buffer size in bytes.
 * @param[in] threshold Received bytes threshold value for calling the RX callback.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Wrong buffer size.
 * @retval Q_ERR_INVSTATE      Async mode is not set.
 * @retval Q_ERR_OUTOFMEMORY   RX buffer is not set.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_RxDMA(qDrvUART_t* const pInstance, UInt8* const pData, UInt16 length, UInt16 threshold);

/**
 * @brief Disable the ongoing reception.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      There is no ongoing reception.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_RxAbort(qDrvUART_t* const pInstance);

/**
 * @brief Flush already received data from the buffer.
 *
 * @param[in] pInstance UART instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      RX callback is not set.
 * @retval Q_ERR_INVSTATE      Interrupt or DMA mode is not set.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_RxFifoFlush(qDrvUART_t* const pInstance);

/**
 * @brief Check whether there is new data in the RX buffer and the threshold was not triggered yet.
 *        For interrupt and DMA mode this function checks number of unread bytes in the buffer.
 *        For the polling mode, it checks if the RX FIFO register is not empty.
 *
 * @param pInstance UART instance pointer.
 *
 * @return True if in the buffer there is new data, false otherwise.
 */
Bool qDrvUART_RxNewDataCheck(qDrvUART_t* const pInstance);

/**
 * @brief Get the pointer to the available chunk data in the buffer.
 *
 * @param[in]  pInstance  UART instance pointer.
 * @param[out] pByteCount Pointer to variable where available chunk size in bytes will be written.
 *
 * @retval Pointer to the available buffer chunk. NULL if there is no new data to be read.
 */
UInt8* qDrvUART_RxContinuousChunkGet(qDrvUART_t* const pInstance, UInt16* pByteCount);

/**
 * @brief Advance the RX buffer.
 *
 * @param[in] pInstance UART instance pointer.
 * @param[in] byteCount Number of bytes to be advanced.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      Instance initialized with the no advance RX buffer mode or receiver not in async mode.
 * @retval Q_ERR_INVPARAM      Wrong buffer size.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUart_RxAdvance(qDrvUART_t* const pInstance, UInt16 byteCount);

/**
 * @brief Enable or disable the receiver error interrupt.
 *
 * @note This function needs to be called only if interrupts are enabled with @p qDrvUART_IrqEnable function.
 *
 * @param[in] pInstance UART instance pointer.
 * @param[in] enable    True if the error interrupt is to be enabled, false otherwise.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvUART_RxErrorIrqEnableSet(qDrvUART_t* const pInstance, Bool enable);

#endif /* REG_UART_NO_RX */

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_UART_H_
