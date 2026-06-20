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

#ifndef _Q_DRV_TWIS_H_
#define _Q_DRV_TWIS_H_

/**
 * @defgroup Q_DRV_TWIS TWIS driver
 * @{
 *
 * @brief This is a driver for TWIS (I2C Slave) peripheral.
 *
 * @file qDrvTWIS.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegTWIS.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvTWIS_Event_t */
//@{
/** @brief Data received. */
#define qDrvTWIS_EventRx 0
/** @brief Data sent successfully. */
#define qDrvTWIS_EventTx 1
/** @typedef qDrvTWIS_Event_t
 *  @brief TWI Slave event type.
 */
typedef UInt8 qDrvTWIS_Event_t;
//@}

/** @brief TWIS internal data structure. */
typedef struct {
    UInt8 data[28];
} __attribute__((aligned(4))) qDrvTWIS_InternalData_t;

/** @brief TWIS instance structure */
typedef struct {
    const qRegTWIS_t baseAddr;            /**< Peripheral base address. */
    qDrvTWIS_InternalData_t internalData; /**< Internal driver data. */
    const UInt8 id;                       /**< Peripheral instance ID. */
} qDrvTWIS_t;

/** @brief TWI Slave config structure. */
typedef struct {
    UInt16 address;         /**< Slave address. */
    Bool acceptGeneralCall; /**< Accept TWI master write on general call address (0). */
} qDrvTWIS_Config_t;

/** @brief TWIS pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t sda;     /**< SDA pin/alternate pair. */
        qDrvIOB_PinAlt_t scl;     /**< SCL pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[2]; /**< Internal pin config array. */
} qDrvTWIS_PinConfig_t;

/**
 * @brief TWI Slave callback.
 *
 * @param[in] arg       User-supplied argument.
 * @param[in] event     TWIS event type.
 * @param[in] buffer    RX or TX buffer.
 * @param[in] byteCount Number of bytes transferred.
 */
typedef void (*qDrvTWIS_Callback_t)(void* arg, qDrvTWIS_Event_t event, UInt8* buffer, UInt16 byteCount);

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro for defining TWIS instance. */
#define Q_DRV_TWIS_INSTANCE_DEFINE(ID)                                                                                 \
    {                                                                                                                  \
        .baseAddr = Q_REG_TWIS_BASEADDR_GET(ID), /* */                                                                 \
            .internalData = {},                  /* */                                                                 \
            .id = ID,                            /* */                                                                 \
    }

/** @brief Default configuration macro for TWIS instance. */
#define Q_DRV_TWIS_CONFIG_DEFAULT(ADDRESS)                                                                             \
    {                                                                                                                  \
        .address = (ADDRESS),           /* */                                                                          \
            .acceptGeneralCall = false, /* */                                                                          \
    }

/** @brief Pin configuration macro for TWIS instance. */
#define Q_DRV_TWIS_PIN_CONFIG(ID, SDA_PIN, SCL_PIN)                                                                    \
    {                                                                                                                  \
        .sda = {.pin = SDA_PIN, .alternate = Q_DRV_IOB_ALT_GET(SDA_PIN, I2C_SL, SDA)},     /* */                       \
            .scl = {.pin = SCL_PIN, .alternate = Q_DRV_IOB_ALT_GET(SCL_PIN, I2C_SL, SCL)}, /* */                       \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initialize the TWIS instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvTWIS_PinConfigSet(const qDrvTWIS_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the TWIS instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvTWIS_PinConfigClear(const qDrvTWIS_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize the TWIS instance.
 *
 * @param[in] pInstance   TWI Slave instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] callback    Callback function.
 * @param[in] arg         User-supplied argument passed to the callback function.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral was already initialized.
 * @retval Q_ERR_INVPARAM Configuration is not correct.
 */
qResult_t qDrvTWIS_Init(const qDrvTWIS_t* pInstance, const qDrvTWIS_Config_t* const pConfig,
                        const qDrvTWIS_Callback_t callback, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the TWIS instance.
 *
 * @param[in] pInstance TWI Slave instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvTWIS_Deinit(const qDrvTWIS_t* pInstance);

/**
 * @brief Check if the peripheral is initialized.
 *
 * @param[in] pInstance TWI Slave instance pointer.
 *
 * @returns True if the peripheral is initialized, false otherwise.
 */
Bool qDrvTWIS_InitCheck(const qDrvTWIS_t* pInstance);

/**
 * @brief Set the RX buffer for data reception.
 * @note  Null pointer or zero-length buffer disables the reception.
 *
 * @param[in] pInstance TWI Slave instance pointer.
 * @param[in] pBuffer   RX buffer. NULL value will force the driver to discard any RX data.
 * @param[in] bufferLen RX buffer length.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_BUSY     Peripheral already enabled.
 * @retval Q_ERR_INVSTATE Peripheral not initialized.
 */
qResult_t qDrvTWIS_RxBufferSet(const qDrvTWIS_t* pInstance, UInt8* pBuffer, UInt16 bufferLen);

/**
 * @brief Enable the peripheral logic and data reception.
 * @note  User must configure the RX buffer with @p qDrvTWIS_RxBufferSet to receive any data.
 *        If the buffer is not set, the peripheral will respond on master writes,
 *        however the incoming data will be discarded.
 *
 * @param[in] pInstance TWI Slave instance pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral not initialized.
 */
qResult_t qDrvTWIS_Enable(const qDrvTWIS_t* pInstance);

/**
 * @brief Disable the peripheral logic and data reception.
 *
 * @param[in] pInstance TWI Slave instance pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral not initialized.
 */
qResult_t qDrvTWIS_Disable(const qDrvTWIS_t* pInstance);

/**
 * @brief Set the data buffer to be transferred during the next operation.
 *
 * @param[in] pInstance TWI Slave instance pointer.
 * @param[in] pBuffer   TX buffer.
 * @param[in] bufferLen TX buffer length.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_BUSY     The previous transfer is in progesss.
 * @retval Q_ERR_INVPARAM Invalid (NULL) parameters.
 * @retval Q_ERR_INVSTATE Peripheral not initialized.
 */
qResult_t qDrvTWIS_Tx(const qDrvTWIS_t* pInstance, UInt8* pBuffer, UInt16 bufferLen);

/**
 * @brief Enable or disable slave clock stretching feature.
 *
 * @param[in] pInstance     TWI Slave instance pointer.
 * @param[in] clkStretching Enable flag.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral not initialized.
 */
qResult_t qDrvTWIS_ClockStretchingSet(const qDrvTWIS_t* pInstance, Bool clkStretching);

/**
 * @brief Check if there is a transfer running.
 *
 * @param[in] pInstance TWI Slave instance pointer.
 *
 * @returns True if there is an ongoing transmission, false otherwise.
 */
Bool qDrvTWIS_BusyCheck(const qDrvTWIS_t* pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_TWIS_H_
