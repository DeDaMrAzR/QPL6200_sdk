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

#ifndef _Q_DRV_TWIM_H_
#define _Q_DRV_TWIM_H_

/**
 * @defgroup Q_DRV_TWIM TWIM driver
 * @{
 *
 * @brief This is a driver for TWIM (I2C Master) peripheral.
 *
 * @file qDrvTWIM.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegTWIM.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvTWIM_Freq_t */
//@{
/** @brief 400kHz. */
#define qDrvTWIM_Freq400k 1
/** @brief 200kHz. */
#define qDrvTWIM_Freq200k 3
/** @brief 100kHz. */
#define qDrvTWIM_Freq100k 7
/** @brief 50kHz. */
#define qDrvTWIM_Freq50k 15
/** @brief 25kHz. */
#define qDrvTWIM_Freq25k 31
/** @brief 12.5 kHz. */
#define qDrvTWIM_Freq12k5 63
/** @typedef qDrvTWIM_Freq_t
 *  @brief TWI Master clock frequency.
 */
typedef UInt8 qDrvTWIM_Freq_t;
//@}

/** @enum qDrvTWIM_Event_t */
//@{
/** @brief Transfer complete. */
#define qDrvTWIM_EventDone 0
/** @brief Slave device is not responding. */
#define qDrvTWIM_EventNoACK 1
/** @brief Arbitration lost. */
#define qDrvTWIM_EventArbitrationLost 2
/** @brief Bus clear operation complete. */
#define qDrvTWIM_EventBusClearDone 3
/** @typedef qDrvTWIM_Event_t
 *  @brief TWI Master event type.
 */
typedef UInt8 qDrvTWIM_Event_t;
//@}

/** @brief TWIM internal data structure. */
typedef struct {
    UInt8 data[30];
} __attribute__((aligned(4))) qDrvTWIM_InternalData_t;

/** @brief TWIM instance structure */
typedef struct {
    const qRegTWIM_t baseAddr;            /**< Peripheral base address. */
    qDrvTWIM_InternalData_t internalData; /**< Internal driver data. */
    const UInt8 id;                       /**< Peripheral instance ID. */
} qDrvTWIM_t;

/** @brief TWI transfer structure */
typedef struct {
    const UInt8* tx; /**< TX buffer. */
    UInt8* rx;       /**< RX buffer. */
    UInt16 txLen;    /**< TX buffer length in bytes. */
    UInt16 rxLen;    /**< RX buffer length in bytes. */
    UInt8 address;   /**< 7-bit slave device address. */
} qDrvTWIM_Transfer_t;

/** @brief TWI Master config structure. */
typedef struct {
    qDrvTWIM_Freq_t freq; /**< TWI Master frequency. */
    Bool clockStretching; /**< Allow clock stretching when slave device is not ready. */
} qDrvTWIM_Config_t;

/** @brief TWIM pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t sda;     /**< SDA pin/alternate pair. */
        qDrvIOB_PinAlt_t scl;     /**< SCL pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[2]; /**< Internal pin config array. */
} qDrvTWIM_PinConfig_t;

/**
 * @brief Transfer complete callback.
 *
 * @param[in] arg       User-supplied argument.
 * @param[in] event     TWIM event type.
 * @param[in] pTransfer Transfer descriptor.
 */
typedef void (*qDrvTWIM_Callback_t)(void* arg, qDrvTWIM_Event_t event, qDrvTWIM_Transfer_t* const pTransfer);

/** @brief Macro for defining TWIM instance. */
#define Q_DRV_TWIM_INSTANCE_DEFINE(ID)                                                                                 \
    {                                                                                                                  \
        .baseAddr = Q_REG_TWIM_BASEADDR_GET(ID), /* */                                                                 \
            .internalData = {},                  /* */                                                                 \
            .id = ID,                            /* */                                                                 \
    }

/** @brief Default configuration macro for TWIM instance. */
#define Q_DRV_TWIM_CONFIG_DEFAULT()                                                                                    \
    {                                                                                                                  \
        .freq = qDrvTWIM_Freq200k,    /* */                                                                            \
            .clockStretching = false, /* */                                                                            \
    }

/** @brief Pin configuration macro for TWIM instance. */
#define Q_DRV_TWIM_PIN_CONFIG(ID, SDA_PIN, SCL_PIN)                                                                    \
    {                                                                                                                  \
        .sda = {.pin = SDA_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(SDA_PIN, I2C_M, ID, SDA)},     /* */              \
            .scl = {.pin = SCL_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(SCL_PIN, I2C_M, ID, SCL)}, /* */              \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initialize the TWIM instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin configuration. Both SDA and SCL must be set.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvTWIM_PinConfigSet(const qDrvTWIM_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the TWIM instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvTWIM_PinConfigClear(const qDrvTWIM_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize the TWIM instance.
 *
 * @param[in] pInstance   TWI Master instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] callback    Callback function.
 * @param[in] arg         User-supplied argument passed to the callback function.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral was already initialized.
 * @retval Q_ERR_INVPARAM Configuration is not correct.
 */
qResult_t qDrvTWIM_Init(const qDrvTWIM_t* pInstance, const qDrvTWIM_Config_t* const pConfig,
                        const qDrvTWIM_Callback_t callback, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the TWIM instance.
 *
 * @param[in] pInstance TWI Master instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvTWIM_Deinit(const qDrvTWIM_t* pInstance);

/**
 * @brief Check if the peripheral is initialized.
 *
 * @param[in] pInstance TWI Master instance pointer.
 *
 * @returns True if the peripheral is initialized, false otherwise.
 */
Bool qDrvTWIM_InitCheck(const qDrvTWIM_t* pInstance);

/**
 * @brief Start the TWIM transfer.
 *
 * @param[in] pInstance TWI Master instance pointer.
 * @param[in] pTransfer Transfer descriptor pointer.
 *
 * @retval Q_OK           Transmission has been started.
 * @retval Q_ERR_INVSTATE Peripheral is not initialized or not enabled.
 * @retval Q_ERR_INVPARAM Invalid parameters.
 * @retval Q_ERR_BUSY     Transmission is already in progress.
 */
qResult_t qDrvTWIM_Transfer(const qDrvTWIM_t* pInstance, const qDrvTWIM_Transfer_t* const pTransfer);

/**
 * @brief Perform bus clear operation.
 *        Driver will generate qDrvTWIM_EventBusClearDone event after completion.
 *
 * @param[in] pInstance TWI Master instance pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_BUSY     Transmission is already in progress.
 * @retval Q_ERR_INVSTATE Instance is not initialized.
 */
qResult_t qDrvTWIM_BusClear(const qDrvTWIM_t* pInstance);

/**
 * @brief Stop the ongoing transfer.
 *        If there is an ongoing transaction, no event will be generated.
 *
 * @param[in] pInstance TWI Master instance pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is not initialized.
 */
qResult_t qDrvTWIM_Abort(const qDrvTWIM_t* pInstance);

/**
 * @brief Check if there is a transfer running.
 *
 * @param[in] pInstance TWI Master instance pointer.
 *
 * @returns Transfer status.
 */
Bool qDrvTWIM_BusyCheck(const qDrvTWIM_t* pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_TWIM_H_
