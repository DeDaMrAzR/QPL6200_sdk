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

#ifndef _Q_DRV_KEYPAD_H_
#define _Q_DRV_KEYPAD_H_

/**
 * @defgroup Q_DRV_KEYPAD_H Keypad driver
 * @{
 *
 * @brief This is a driver for Keypad peripheral.
 *
 * @file qDrvKeypad.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *
 *****************************************************************************/
#include "qRegKeypad.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#define Q_DRV_KEYPAD_ROWS_COUNT    8
#define Q_DRV_KEYPAD_COLUMNS_COUNT 8

/** @enum qDrvKeypad_Mode_t */
//@{
/** @brief Keypad in basic mode. */
#define qDrvKeypad_ModeBasic 0
/** @brief Keypad in passive column mode. */
#define qDrvKeypad_ModePassiveColumn 1
/** @brief Keypad in active column mode. */
#define qDrvKeypad_ModeActiveColumn 2
/** @typedef qDrvKeypad_Mode_t
 *  @brief Keypad peripheral mode configuration.
 */
typedef UInt8 qDrvKeypad_Mode_t;
//@}

/** @brief KEYPAD internal data structure. */
typedef struct {
    UInt32 data[6]; /**< Internal data. */
} __attribute__((aligned(4))) qDrvKeypad_InternalData_t;

/** @brief KEYPAD Master/Slave instance structure */
typedef struct {
    const qRegKeypad_t baseAddr;            /**< Peripheral base address. */
    qDrvKeypad_InternalData_t internalData; /**< Internal driver data. */
    const UInt8 id;                         /**< Peripheral instance ID. */
} qDrvKeypad_t;

/** @brief KEYPAD pin definition structure. */
typedef struct {
    qDrvIOB_PinAlt_t row[Q_DRV_KEYPAD_ROWS_COUNT];
    UInt8 rowCount;
    qDrvIOB_PinAlt_t column[Q_DRV_KEYPAD_COLUMNS_COUNT];
    UInt8 columnCount;
} qDrvKeypad_PinConfig_t;

/** @brief Keypad config structure. */
typedef struct {
    qDrvKeypad_Mode_t mode; /**< Keypad mode. */
    UInt32 rdp;             /**< Row drive period in multiplication of 250ns. Values 0-250 are used as 250ns. */
    UInt32 cdp;             /**< Column drive period in multiplication of 250ns. Set 0 to disable.
                                If @p mode is qDrvKeypad_ModePassiveColumn cdp is used as passive column drive period.
                                If @p mode is qDrvKeypad_ModeActiveColumn cdp is used as active column drive period.
                                */
} qDrvKeypad_Config_t;

/**
 * @brief Keypad callback function type called after transmitting a bunch of data.
 *
 * @param[in] arg    User supplied argument.
 * @param[in] button
 */
typedef void (*qDrvKeypad_Cb_t)(void* arg, UInt8 row, UInt8 column, Bool pressed);

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/**
 * @brief Macro for defining KEYPAD row pin configuration.
 *
 * @param[in] PIN       IOB pin index.
 * @param[in] ROW_INDEX Row index.
 */
#define Q_DRV_KEYPAD_PIN_IOB_CONFIG_ROW_DEFINE(PIN, ROW_INDEX)                                                         \
    {                                                                                                                  \
        .pin = PIN,                                                     /* */                                          \
            .alternate = REG_IOB_PIN_ALT(PIN, KEYPAD_SCANS, ROW_INDEX), /* */                                          \
    }

/**
 * @brief Macro for defining KEYPAD column pin configuration.
 *
 * @param[in] PIN          IOB pin index.
 * @param[in] COLUMN_INDEX Column index.
 */
#define Q_DRV_KEYPAD_PIN_IOB_CONFIG_COLUMN_DEFINE(PIN, COLUMN_INDEX)                                                   \
    {                                                                                                                  \
        .pin = PIN,                                                         /* */                                      \
            .alternate = REG_IOB_PIN_ALT(PIN, KEYPAD_SENSES, COLUMN_INDEX), /* */                                      \
    }

#define _Q_DRV_KEYPAD_ROW_DEFINE2(PIN, ROW_INDEX) Q_DRV_KEYPAD_PIN_IOB_CONFIG_ROW_DEFINE(PIN, ROW_INDEX)
#define _Q_DRV_KEYPAD_ROW_DEFINE1(...)            _Q_DRV_KEYPAD_ROW_DEFINE2(__VA_ARGS__)
#define _Q_DRV_KEYPAD_ROW_DEFINE(val)             _Q_DRV_KEYPAD_ROW_DEFINE1(Q_DEBRACKET val),

#define _Q_DRV_KEYPAD_PIN_IOB_CONFIG_ROWS_DEFINE(...)                                                                  \
    {                                                                                                                  \
        Q_FOREACH(_Q_DRV_KEYPAD_ROW_DEFINE, __VA_ARGS__) /* */                                                         \
    }

/**
 * @brief Macro for defining KEYPAD multiple rows pin configuration.
 *
 * @param[in] ... List of row pin definitions. eg. ((1, 2), (3, 4))
 */
#define Q_DRV_KEYPAD_PIN_CONFIG_ROWS_DEFINE(...)                                                                       \
    .row = _Q_DRV_KEYPAD_PIN_IOB_CONFIG_ROWS_DEFINE(__VA_ARGS__), /* */                                                \
        .rowCount = Q_COUNT_ARGS(__VA_ARGS__)                     /* */

#define _Q_DRV_KEYPAD_COLUMN_DEFINE2(PIN, COLUMN_INDEX) Q_DRV_KEYPAD_PIN_IOB_CONFIG_COLUMN_DEFINE(PIN, COLUMN_INDEX)
#define _Q_DRV_KEYPAD_COLUMN_DEFINE1(...)               _Q_DRV_KEYPAD_COLUMN_DEFINE2(__VA_ARGS__)
#define _Q_DRV_KEYPAD_COLUMN_DEFINE(val)                _Q_DRV_KEYPAD_COLUMN_DEFINE1(Q_DEBRACKET val),

#define _Q_DRV_KEYPAD_PIN_IOB_CONFIG_COLUMNS_DEFINE(...)                                                               \
    {                                                                                                                  \
        Q_FOREACH(_Q_DRV_KEYPAD_COLUMN_DEFINE, __VA_ARGS__) /* */                                                      \
    }

/**
 * @brief Macro for defining KEYPAD multiple columns pin configuration.
 *
 * @param[in] ... List of column pin definitions. eg. ((1, 2), (3, 4))
 */
#define Q_DRV_KEYPAD_PIN_CONFIG_COLUMNS_DEFINE(...)                                                                    \
    .column = _Q_DRV_KEYPAD_PIN_IOB_CONFIG_COLUMNS_DEFINE(__VA_ARGS__), /* */                                          \
        .columnCount = Q_COUNT_ARGS(__VA_ARGS__)                        /* */

/** @brief Macro for defining KEYPAD instance. */
#define Q_DRV_KEYPAD_INSTANCE_DEFINE(ID)                                                                               \
    {                                                                                                                  \
        .baseAddr = Q_REG_KEYPAD_BASEADDR_GET(ID), /* */                                                               \
            .internalData = {},                    /* */                                                               \
            .id = ID,                              /* */                                                               \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the Keypad instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin configuration.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvKeypad_PinConfigSet(const qDrvKeypad_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the Keypad instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvKeypad_PinConfigClear(const qDrvKeypad_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize Keypad instance.
 *
 * @param[in] pInstance   Keypad instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] pCallbacks  Callbacks function set. Required for interrupt and DMA mode.
 * @param[in] arg         User-supplied argument passed to the callback functions.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid configuration.
 */
qResult_t qDrvKeypad_Init(qDrvKeypad_t* pInstance, const qDrvKeypad_Config_t* const pConfig, qDrvKeypad_Cb_t pCallback,
                          void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the Keypad instance.
 *
 * @param[in] pInstance Keypad instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvKeypad_Deinit(qDrvKeypad_t* const pInstance);

/**
 * @brief Check if Keypad instance is initialized.
 *
 * @param[in] pInstance Keypad instance pointer.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvKeypad_InitCheck(qDrvKeypad_t* const pInstance);

/**
 * @brief Check if Keypad instance is busy.
 *
 * @param[in] pInstance Keypad instance pointer.
 *
 * @retval True if the instance is busy, false otherwise.
 */
Bool qDrvKeypad_BusyCheck(qDrvKeypad_t* const pInstance);

/**
 * @brief Enable Keypad instance.
 *
 * @param[in] pInstance Keypad instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Keypad is already enabled.
 */
qResult_t qDrvKeypad_Enable(qDrvKeypad_t* const pInstance);

/**
 * @brief Disable Keypad instance.
 *
 * @param[in] pInstance Keypad instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Keypad is already disabled.
 */
qResult_t qDrvKeypad_Disable(qDrvKeypad_t* const pInstance);

/**
 * @brief Check if Keypad instance is enabled.
 *
 * @param[in] pInstance Keypad instance pointer.
 *
 * @retval True if the instance is enabled, false otherwise.
 */
Bool qDrvKeypad_EnableCheck(qDrvKeypad_t* const pInstance);

/**
 * @brief Trigger Keypad scan.
 *
 * @param[in] pInstance Keypad instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Keypad is not enabled.
 */
qResult_t qDrvKeypad_Trigger(qDrvKeypad_t* const pInstance);

/**
 * @brief Set Keypad debounce.
 *
 * @param[in] pInstance     Keypad instance pointer.
 * @param[in] enable        True if debounce is to be enabled, false otherwise.
 * @param[in] debounceCount Scan debounce count.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Keypad is enabled.
 * @retval Q_ERR_INVPARAM      Debounce count must be greater than 0.
 */
qResult_t qDrvKeypad_DebounceSet(qDrvKeypad_t* const pInstance, Bool enable, UInt8 debounceCount);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_KEYPAD_H_
