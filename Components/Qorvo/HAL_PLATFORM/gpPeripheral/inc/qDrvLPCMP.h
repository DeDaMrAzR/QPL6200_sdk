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

#ifndef _Q_DRV_LPCMP_H_
#define _Q_DRV_LPCMP_H_

/**
 * @defgroup Q_DRV_LPCMP_H Low Power Comparator driver
 * @{
 *
 * @brief This is a driver for LPCMP (Low Power Comparator) peripheral.
 *
 * @file qDrvLPCMP.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegLPCMP.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvLPCMP_Reference_t */
//@{
/** @brief No reference voltage.
 */
#define qDrvLPCMP_ReferenceNone qRegLPCMP_ReferenceNone
/** @brief Use ANIO pin as reference.
 */
#define qDrvLPCMP_ReferenceExternal qRegLPCMP_ReferenceExternal
/** @brief Internal reference voltage.
 */
#define qDrvLPCMP_ReferenceInternal qRegLPCMP_ReferenceInternal
/** @brief VDD (battery voltage) as reference.
 */
#define qDrvLPCMP_ReferenceVDD qRegLPCMP_ReferenceVDD
/** @typedef qDrvLPCMP_Reference_t
 *  @brief LPCMP channel reference source.
 */
typedef UInt8 qDrvLPCMP_Reference_t;
//@}

/** @enum qDrvLPCMP_Match_t */
//@{
/** @brief Match event will be triggered when input voltage is below the reference voltage (comparator value is 0).
 */
#define qDrvLPCMP_MatchBelow qRegLPCMP_MatchBelow
/** @brief Match event will be triggered when input voltage is above the reference voltage (comparator value is 1).
 */
#define qDrvLPCMP_MatchAbove qRegLPCMP_MatchAbove
/** @typedef qDrvLPCMP_Match_t
 *  @brief LPCMP compare mode.
 */
typedef UInt8 qDrvLPCMP_Match_t;
//@}

/** @brief LPCMP internal data structure. */
typedef struct {
    UInt8 data[9];
} __attribute__((aligned(4))) qDrvLPCMP_InternalData_t;

/** @brief LPCMP instance structure. */
typedef struct {
    qDrvLPCMP_InternalData_t internalData; /**< Internal driver data. */
} qDrvLPCMP_t;

/** @brief LPCMP config structure. */
typedef struct {
    UInt8 clkDiv; /**< LPCMP clock divider. */
} qDrvLPCMP_Config_t;

/** @brief LPCMP slot config structure. */
typedef struct {
    qRegLPCMP_Channel_t inputANIO;   /**< Input voltage ANIO number. */
    UInt8 inputScale;                /**< Input voltage scaling ((inputScale + 1) / 256). */
    qDrvLPCMP_Reference_t refSelect; /**< Reference voltage selector. */
    qRegLPCMP_Channel_t refANIO;     /**< Reference voltage ANIO number.
                                          Valid only if @p refSelect is @p qDrvLPCMP_ReferenceExternal.
                                      */
    UInt8 refScale;                  /**< Reference voltage scaling ((refScale + 1) / 256). */
    qDrvLPCMP_Match_t match;         /**< Matching mode (comparator value). */
} qDrvLPCMP_SlotConfig_t;

/** @brief LPCMP pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t anio;    /**< ANIOx pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[1]; /**< Internal pin config array. */
} qDrvLPCMP_PinConfig_t;

/**
 * @brief LPCMP match event callback.
 *
 * @param[in] arg  User-supplied argument
 * @param[in] slot Comparator slot number, on which match event has occured.
 */
typedef void (*qDrvLPCMP_Callback_t)(void* arg, qRegLPCMP_Slot_t slot);

/** @brief Macro for defining LPCMP instance. */
#define Q_DRV_LPCMP_INSTANCE_DEFINE(ID)                                                                                \
    {                                                                                                                  \
        .internalData = {.data = {Q_REG_LPCMP_BASEADDR_GET(ID) ? 0 : 0}}, /* */                                        \
    }

/** @brief Default configuration macro for LPCMP instance. */
#define Q_DRV_LPCMP_CONFIG_DEFAULT()                                                                                   \
    {                                                                                                                  \
        .clkDiv = 1, /* */                                                                                             \
    }

/** @brief Macro for defining LPCMP pin configuration. */
#define Q_DRV_LPCMP_PIN_CONFIG(PIN, ANIO_NUM)                                                                          \
    {                                                                                                                  \
        .anio = {.pin = PIN, .alternate = Q_DRV_IOB_ALT_GET(PIN, ANALOG_ANIO, ANIO_NUM)}, /* */                        \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initialize the LPCMP analog input pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin configuration.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvLPCMP_PinConfigSet(const qDrvLPCMP_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the LPCMP analog input pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvLPCMP_PinConfigClear(const qDrvLPCMP_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize the LPCMP instance.
 *
 * @param[in] pInstance   LPCMP instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] callback    Callback function.
 * @param[in] arg         User-supplied argument passed to the callback function.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral was already initialized.
 * @retval Q_ERR_INVPARAM Configuration is not correct.
 */
qResult_t qDrvLPCMP_Init(const qDrvLPCMP_t* pInstance, const qDrvLPCMP_Config_t* const pConfig,
                         const qDrvLPCMP_Callback_t callback, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the LPCMP instance.
 *
 * @param[in] pInstance LPCMP instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvLPCMP_Deinit(const qDrvLPCMP_t* pInstance);

/**
 * @brief Check if the LPCMP instance is initialized.
 *
 * @param[in] pInstance LPCMP instance pointer.
 *
 * @return True if the peripheral is initialized, false otherwise.
 */
Bool qDrvLPCMP_InitCheck(const qDrvLPCMP_t* pInstance);

/**
 * @brief Configure the comparator slot.
 *
 * @param[in] pInstance LPCMP instance pointer.
 * @param[in] slot      Comparator slot number.
 * @param[in] pConfig   Slot configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid configuration.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_NOTSUPPORTED  Selected channel is not available on this package.
 */
qResult_t qDrvLPCMP_SlotConfigSet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot,
                                  const qDrvLPCMP_SlotConfig_t* pConfig);

/**
 * @brief Enable the comparator peripheral.
 *
 * @note Every configured slot must be enabled additionally.
 *
 * @param[in] pInstance LPCMP instance pointer.
 * @param[in] enable    True to enable, false to disable.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvLPCMP_EnableSet(const qDrvLPCMP_t* pInstance, Bool enable);

/**
 * @brief Enable or disable the comparator slot.
 *
 * @param[in] pInstance LPCMP instance pointer.
 * @param[in] slot      Comparator slot number.
 * @param[in] enable    True to enable, false to disable.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid slot number.
 * @retval Q_ERR_INVSTATE      Slot is not configured.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvLPCMP_SlotEnableSet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot, Bool Enable);

/**
 * @brief Enable or disable the comparator slot window mode.
 *
 * @param[in] pInstance LPCMP instance pointer.
 * @param[in] slot      Comparator slot number. The window mode will be enabled
 *                      for all slots within the window group (0+1, 2+3).
 * @param[in] enable    True to enable, false to disable.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid slot number.
 * @retval Q_ERR_INVSTATE      At least one slot within the window is not configured.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvLPCMP_SlotWindowSet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot, Bool enable);

/**
 * @brief Read the compare result when callback is not set.
 * @note  This function will always return false if callback is enabled,
 *        as the channels are disabled on match event to avoid continuous interrupt triggering.
 *
 * @param[in] pInstance LPCMP instance pointer.
 * @param[in] slot      Comparator slot number.
 *
 * @return Compare result for a given slot.
 */
Bool qDrvLPCMP_SlotResultGet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_LPCMP_H_
