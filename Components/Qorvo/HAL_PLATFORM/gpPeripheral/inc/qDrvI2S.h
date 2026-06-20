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

#ifndef _Q_DRV_I2S_H_
#define _Q_DRV_I2S_H_

/**
 * @defgroup Q_DRV_I2S_H General Inter-IC Sound driver
 * @{
 *
 * @brief This is a driver for I2S (Inter-IC Sound).
 *
 * @file qDrvI2S.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegI2S.h"
#include "qDrvCommon.h"
#include "qDrvDMA.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#define Q_DRV_I2S_PRESCALER_NONE 0xFFFFUL
#define Q_DRV_I2S_PRESCALER_MAX  0xFFUL

#define Q_DRV_I2S_MODE_MASTER_MASK BM(0)
#define Q_DRV_I2S_MODE_SLAVE_MASK  BM(1)
#define Q_DRV_I2S_MODE_TX_MASK     BM(2)
#define Q_DRV_I2S_MODE_RX_MASK     BM(3)

/** @enum qDrvI2S_Mode_t */
//@{
/** @brief I2S in master configuration mode with transmitter. */
#define qDrvI2S_ModeMasterTx (Q_DRV_I2S_MODE_MASTER_MASK | Q_DRV_I2S_MODE_TX_MASK)
/** @brief I2S in master configuration mode with receiver. */
#define qDrvI2S_ModeMasterRx (Q_DRV_I2S_MODE_MASTER_MASK | Q_DRV_I2S_MODE_RX_MASK)
/** @brief I2S in master configuration mode with transmitter and receiver. */
#define qDrvI2S_ModeMasterTxRx (Q_DRV_I2S_MODE_MASTER_MASK | Q_DRV_I2S_MODE_TX_MASK | Q_DRV_I2S_MODE_RX_MASK)
/** @brief I2S in slave configuration mode with transmitter. */
#define qDrvI2S_ModeSlaveTx (Q_DRV_I2S_MODE_SLAVE_MASK | Q_DRV_I2S_MODE_TX_MASK)
/** @brief I2S in slave configuration mode with receiver. */
#define qDrvI2S_ModeSlaveRx (Q_DRV_I2S_MODE_SLAVE_MASK | Q_DRV_I2S_MODE_RX_MASK)
/** @brief I2S in slave configuration mode with transmitter and receiver. */
#define qDrvI2S_ModeSlaveTxRx (Q_DRV_I2S_MODE_SLAVE_MASK | Q_DRV_I2S_MODE_TX_MASK | Q_DRV_I2S_MODE_RX_MASK)
/** @typedef qDrvI2S_Mode_t
 *  @brief I2S peripheral mode configuration.
 */
typedef UInt8 qDrvI2S_Mode_t;
//@}

/** @enum qDrvI2S_TransferMode_t */
//@{
/** @brief Pooling configuration. */
#define qDrvI2S_TransferModePolling 1
/** @brief Interrupt configuration. */
#define qDrvI2S_TransferModeInterrupt 2
/** @brief DMA configuration. */
#define qDrvI2S_TransferModeDMA 3
/** @typedef qDrvI2S_TransferMode_t
 *  @brief I2S transmitter and receiver mode configuration.
 */
typedef UInt8 qDrvI2S_TransferMode_t;
//@}

/** @enum qDrvI2S_ClkSrc_t */
//@{
#define qDrvI2S_ClkSrcMain   qRegI2S_ClkSelMain
#define qDrvI2S_ClkSrcFract0 qRegI2S_ClkSelFract0
#define qDrvI2S_ClkSrcFract1 qRegI2S_ClkSelFract1
#define qDrvI2S_ClkSrcIob    qRegI2S_ClkSelIob
/** @typedef qDrvI2S_ClkSrc_t
 *  @brief I2S clock frequency.
 */
typedef qRegI2S_ClkSel_t qDrvI2S_ClkSrc_t;
//@}

/** @brief I2S internal data structure. */
typedef struct {
    UInt32 data[7];
} __attribute__((aligned(4))) qDrvI2S_InternalData_t;

/** @brief I2S Master/Slave instance structure */
typedef struct {
    const qRegI2S_t baseAddr;            /**< Peripheral base address. */
    qDrvI2S_InternalData_t internalData; /**< Internal driver data. */
    const UInt8 id;                      /**< Peripheral instance ID. */
} qDrvI2S_t;

/** @brief I2S pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t sdo;     /**< SDO pin/alternate pair. */
        qDrvIOB_PinAlt_t sdi;     /**< SDI pin/alternate pair. */
        qDrvIOB_PinAlt_t sck;     /**< SCK pin/alternate pair. */
        qDrvIOB_PinAlt_t ws;      /**< WS pin/alternate pair. */
        qDrvIOB_PinAlt_t refIn;   /**< Reference input clock pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[5]; /**< Internal pin config array. */
} qDrvI2S_PinConfig_t;

/** @brief I2C master / slave config structure. */
typedef struct {
    qDrvI2S_Mode_t mode;                 /**< I2S mode. */
    qDrvI2S_TransferMode_t transferMode; /**< I2S transfer mode. */
    qDrvI2S_ClkSrc_t clkSrc;             /**< Peripheral input clock (F_CLK) source. */
    UInt16 prescaler; /**< SCK prescaler. F_SCK = F_CLK / 2 * (prescaler + 1). Use @p Q_DRV_I2S_PRESCALER_NONE to
                         disable. */
    UInt8 wordLen;    /**< Number of bits per one word (single channel). */
    UInt8 wsOffset;   /**< Offset in clock cycles between WS signal toggle and the beginning of the next word. */
    struct {
        UInt8 left;   /**< Left channel sample (TX) in buffer (bytes). */
        UInt8 right;  /**< Right channel sample (TX) in buffer (bytes). */
    } txBytes;
    struct {
        UInt8 left;  /**< Left channel sample (RX) in buffer (bytes). */
        UInt8 right; /**< Right channel sample (RX) in buffer (bytes). */
    } rxBytes;
} qDrvI2S_Config_t;

/**
 * @brief I2S callback function type called after transmitting a bunch of data.
 *
 * @param[in] arg        User supplied argument.
 * @param[in] pBuffer    Pointer to the already transmitted buffer. This buffer should be updated with a new data.
 * @param[in] bufferSize Size of the buffer in bytes.
 */
typedef void (*qDrvI2S_CbTx_t)(void* arg, UInt8* pBuffer, UInt16 bufferSize);

/**
 * @brief I2S callback function type called after receiving a bunch of data.
 *
 * @param[in] arg        User supplied argument.
 * @param[in] pBuffer    Pointer to the already received data.
 * @param[in] bufferSize Size of the buffer in bytes.
 */
typedef void (*qDrvI2S_CbRx_t)(void* arg, UInt8* pBuffer, UInt16 bufferSize);

/** @brief Callback structure definition. */
typedef struct {
    qDrvI2S_CbTx_t tx; /**< Data transmission event handler. Within this callback, user needs to update the buffer with
                          new data to be transferred. In case of no buffer update, the driver will transfer the data
                          that is already in the buffer. */
    qDrvI2S_CbRx_t rx; /**< Data reception event handler. */
} qDrvI2S_Callbacks_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro for defining I2S instance. */
#define Q_DRV_I2S_INSTANCE_DEFINE(ID)                                                                                  \
    {                                                                                                                  \
        .baseAddr = Q_REG_I2S_BASEADDR_GET(ID), /* */                                                                  \
            .internalData = {},                 /* */                                                                  \
            .id = ID,                           /* */                                                                  \
    }

#define _Q_DRV_I2S_REF_IN_PIN_CONFIG_2(INSTANCE_ID, SCK_PIN, WS_PIN) .refIn = {.pin = Q_NOPIN}

#define _Q_DRV_I2S_REF_IN_PIN_CONFIG_3(INSTANCE_ID, SCK_PIN, WS_PIN, REF_IN_PIN)                                       \
    .refIn = {.pin = REF_IN_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(REF_IN_PIN, I2S, INSTANCE_ID, REF_IN)}

#define _Q_DRV_I2S_REF_IN_PIN_CONFIG(INSTANCE_ID, ...)                                                                 \
    Q_CONCAT(_Q_DRV_I2S_REF_IN_PIN_CONFIG_, Q_COUNT_ARGS(__VA_ARGS__))(INSTANCE_ID, __VA_ARGS__)

#define _Q_DRV_I2S_TX_PIN_CONFIG(INSTANCE_ID, SDO_PIN)                                                                 \
    .sdo = {.pin = SDO_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(SDO_PIN, I2S, INSTANCE_ID, SDO)}

#define _Q_DRV_I2S_RX_PIN_CONFIG(INSTANCE_ID, SDI_PIN)                                                                 \
    .sdi = {.pin = SDI_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(SDI_PIN, I2S, INSTANCE_ID, SDI)}

#define _Q_DRV_I2S_MASTER_PIN_CONFIG(INSTANCE_ID, SCK_PIN, WS_PIN)                                                     \
    .sck = {.pin = SCK_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(SCK_PIN, I2S, INSTANCE_ID, SCK)}, /* */               \
        .ws = {.pin = WS_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(WS_PIN, I2S, INSTANCE_ID, WS)}  /* */

#define _Q_DRV_I2S_SLAVE_PIN_CONFIG(INSTANCE_ID, SCK_PIN, WS_PIN)                                                      \
    .sck = {.pin = SCK_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(SCK_PIN, I2S, INSTANCE_ID, SCK_IN)}, /* */            \
        .ws = {.pin = WS_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(WS_PIN, I2S, INSTANCE_ID, WS_IN)}  /* */

/**
 * @brief Pin configuration macro for I2S instance in master transmitter mode.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] SDO_PIN     Serial data output pin.
 * @param[in] SCK_PIN     Serial clock pin.
 * @param[in] WS_PIN      Word select pin.
 * @param[in] REF_IN_PIN  Reference input pin (optional).
 */
#define Q_DRV_I2S_MASTER_TX_PIN_CONFIG(INSTANCE_ID, SDO_PIN, ...)                                                      \
    {                                                                                                                  \
        _Q_DRV_I2S_TX_PIN_CONFIG(INSTANCE_ID, SDO_PIN),                                              /* SDO */         \
            .sdi = {.pin = Q_NOPIN},                                                                 /* */             \
            _Q_DRV_I2S_MASTER_PIN_CONFIG(INSTANCE_ID, Q_ARG(0, __VA_ARGS__), Q_ARG(1, __VA_ARGS__)), /* SCK & WS */    \
            _Q_DRV_I2S_REF_IN_PIN_CONFIG(INSTANCE_ID, __VA_ARGS__),                                  /* REF_IN */      \
    }

/**
 * @brief Pin configuration macro for I2S instance in master receiver mode.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] SDI_PIN     Serial data input pin.
 * @param[in] SCK_PIN     Serial clock pin.
 * @param[in] WS_PIN      Word select pin.
 * @param[in] REF_IN_PIN  Reference input pin (optional).
 */
#define Q_DRV_I2S_MASTER_RX_PIN_CONFIG(INSTANCE_ID, SDI_PIN, ...)                                                      \
    {                                                                                                                  \
        .sdo = {.pin = Q_NOPIN},                                                                     /* */             \
            _Q_DRV_I2S_RX_PIN_CONFIG(INSTANCE_ID, SDI_PIN),                                          /* SDI */         \
            _Q_DRV_I2S_MASTER_PIN_CONFIG(INSTANCE_ID, Q_ARG(0, __VA_ARGS__), Q_ARG(1, __VA_ARGS__)), /* SCK & WS */    \
            _Q_DRV_I2S_REF_IN_PIN_CONFIG(INSTANCE_ID, __VA_ARGS__),                                  /* REF_IN */      \
    }

/**
 * @brief Pin configuration macro for I2S instance in master transmitter and receiver mode.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] SDO_PIN     Serial data output pin.
 * @param[in] SDI_PIN     Serial data input pin.
 * @param[in] SCK_PIN     Serial clock pin.
 * @param[in] WS_PIN      Word select pin.
 * @param[in] REF_IN_PIN  Reference input pin (optional).
 */
#define Q_DRV_I2S_MASTER_TX_RX_PIN_CONFIG(INSTANCE_ID, SDO_PIN, SDI_PIN, ...)                                          \
    {                                                                                                                  \
        _Q_DRV_I2S_TX_PIN_CONFIG(INSTANCE_ID, SDO_PIN),                                              /* SDO */         \
            _Q_DRV_I2S_RX_PIN_CONFIG(INSTANCE_ID, SDI_PIN),                                          /* SDI */         \
            _Q_DRV_I2S_MASTER_PIN_CONFIG(INSTANCE_ID, Q_ARG(0, __VA_ARGS__), Q_ARG(1, __VA_ARGS__)), /* SCK & WS */    \
            _Q_DRV_I2S_REF_IN_PIN_CONFIG(INSTANCE_ID, __VA_ARGS__),                                  /* REF_IN */      \
    }

/**
 * @brief Pin configuration macro for I2S instance in slave receiver mode.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] SDO_PIN     Serial data output pin.
 * @param[in] SCK_PIN     Serial clock pin.
 * @param[in] WS_PIN      Word select pin.
 * @param[in] REF_IN_PIN  Reference input pin (optional).
 */
#define Q_DRV_I2S_SLAVE_TX_PIN_CONFIG(INSTANCE_ID, SDO_PIN, ...)                                                       \
    {                                                                                                                  \
        _Q_DRV_I2S_TX_PIN_CONFIG(INSTANCE_ID, SDO_PIN),                                             /* SDO */          \
            .sdi = {.pin = Q_NOPIN},                                                                /* */              \
            _Q_DRV_I2S_SLAVE_PIN_CONFIG(INSTANCE_ID, Q_ARG(0, __VA_ARGS__), Q_ARG(1, __VA_ARGS__)), /* SCK & WS */     \
            _Q_DRV_I2S_REF_IN_PIN_CONFIG(INSTANCE_ID, __VA_ARGS__),                                 /* REF_IN */       \
    }

/**
 * @brief Pin configuration macro for I2S instance in slave transmitter mode.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] SDI_PIN     Serial data input pin.
 * @param[in] SCK_PIN     Serial clock pin.
 * @param[in] WS_PIN      Word select pin.
 * @param[in] REF_IN_PIN  Reference input pin (optional).
 */
#define Q_DRV_I2S_SLAVE_RX_PIN_CONFIG(INSTANCE_ID, SDI_PIN, ...)                                                       \
    {                                                                                                                  \
        .sdo = {.pin = Q_NOPIN},                                                                    /* */              \
            _Q_DRV_I2S_RX_PIN_CONFIG(INSTANCE_ID, SDI_PIN),                                         /* SDI */          \
            _Q_DRV_I2S_SLAVE_PIN_CONFIG(INSTANCE_ID, Q_ARG(0, __VA_ARGS__), Q_ARG(1, __VA_ARGS__)), /* SCK & WS */     \
            _Q_DRV_I2S_REF_IN_PIN_CONFIG(INSTANCE_ID, __VA_ARGS__),                                 /* REF_IN */       \
    }

/**
 * @brief Pin configuration macro for I2S instance in slave transmitter and receiver mode.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] SDO_PIN     Serial data output pin.
 * @param[in] SDI_PIN     Serial data input pin.
 * @param[in] SCK_PIN     Serial clock pin.
 * @param[in] WS_PIN      Word select pin.
 * @param[in] REF_IN_PIN  Reference input pin (optional).
 */
#define Q_DRV_I2S_SLAVE_TX_RX_PIN_CONFIG(INSTANCE_ID, SDO_PIN, SDI_PIN, ...)                                           \
    {                                                                                                                  \
        _Q_DRV_I2S_TX_PIN_CONFIG(INSTANCE_ID, SDO_PIN),                                             /* SDO */          \
            _Q_DRV_I2S_RX_PIN_CONFIG(INSTANCE_ID, SDI_PIN),                                         /* SDI */          \
            _Q_DRV_I2S_SLAVE_PIN_CONFIG(INSTANCE_ID, Q_ARG(0, __VA_ARGS__), Q_ARG(1, __VA_ARGS__)), /* SCK & WS */     \
            _Q_DRV_I2S_REF_IN_PIN_CONFIG(INSTANCE_ID, __VA_ARGS__),                                 /* REF_IN */       \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the I2S instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin configuration.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvI2S_PinConfigSet(const qDrvI2S_PinConfig_t* const pPinConfig, qDrvI2S_Mode_t mode);

/**
 * @brief Deinitialize the I2S instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvI2S_PinConfigClear(const qDrvI2S_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize I2S instance.
 *
 * @param[in] pInstance   I2S instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] pCallbacks  Callbacks function set. Required for interrupt and DMA mode.
 * @param[in] arg         User-supplied argument passed to the callback functions.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid configuration.
 * @retval Q_ERR_OUTOFMEMORY   Unable to allocate DMA channel.
 */
qResult_t qDrvI2S_Init(qDrvI2S_t* pInstance, const qDrvI2S_Config_t* const pConfig, qDrvI2S_Callbacks_t* pCallbacks,
                       void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the I2S instance.
 *
 * @param[in] pInstance I2S instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvI2S_Deinit(qDrvI2S_t* const pInstance);

/**
 * @brief Check if I2S instance is initialized.
 *
 * @param[in] pInstance I2S instance pointer.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvI2S_InitCheck(qDrvI2S_t* const pInstance);

/**
 * @brief Set the buffer for data transmission.
 *
 * @param[in] pInstance  I2S instance pointer.
 * @param[in] pBuffer    Buffer pointer.
 * @param[in] bufferSize Buffer size in bytes.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid buffer size.
 * @retval Q_ERR_BUSY          Instance is already transmitting data.
 * @retval Q_ERR_INVSTATE      No TX mode configured.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvI2S_TxBufferSet(qDrvI2S_t* const pInstance, UInt8* pBuffer, UInt16 bufferSize);

/**
 * @brief Set the buffer for data reception.
 *
 * @param[in] pInstance  I2S instance pointer.
 * @param[in] pBuffer    Buffer pointer.
 * @param[in] bufferSize Buffer size in bytes.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid buffer size.
 * @retval Q_ERR_BUSY          Instance is already receiving data.
 * @retval Q_ERR_INVSTATE      No RX mode configured.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvI2S_RxBufferSet(qDrvI2S_t* const pInstance, UInt8* pBuffer, UInt16 bufferSize);

/**
 * @brief Start I2S instance.
 *
 * @param[in] pInstance I2S instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_BUSY          Instance is already transmitting or receiving data.
 * @retval Q_ERR_GENERAL       Missing buffers for TX or RX mode.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvI2S_Start(qDrvI2S_t* const pInstance);

/**
 * @brief Stop I2S instance.
 *
 * @param[in] pInstance I2S instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      Instance already stopped.
 * @retval Q_ERR_NOTSUPPORTED  Can not stop polling mode.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvI2S_Stop(qDrvI2S_t* const pInstance);

/**
 * @brief Check if I2S instance is busy.
 *
 * @param[in] pInstance I2S instance pointer.
 *
 * @retval True if the instance is busy, false otherwise.
 */
Bool qDrvI2S_BusyCheck(qDrvI2S_t* const pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_I2S_H_
