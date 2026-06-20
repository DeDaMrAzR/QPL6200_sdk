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

#ifndef _Q_DRV_FRACT_CLK_H_
#define _Q_DRV_FRACT_CLK_H_

/**
 * @defgroup Q_DRV_FRACT_CLK_H Fractional Clock Generator driver
 * @{
 *
 * @brief This is a driver for Fract Clk (Fractional Clock Generator).
 *
 * @file qDrvFrackClk.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegFractClk.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief Fractional Clock Generator instance structure. */
typedef struct {
    const qRegFractClk_t baseAddr; /**< Peripheral base address. */
    const UInt8 id;                /**< Peripheral instance ID. */
} qDrvFractClk_t;

/** @brief Fractional Clock Generator pin definition. */
typedef qDrvIOB_PinAlt_t qDrvFractClk_PinConfig_t;

/** @brief Fractional Clock Generator config structure. */
typedef struct {
    UInt32 freq; /**< Clock frequency in Hz. */
} qDrvFractClk_Config_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro for defining Fractional Clock Generator instance. */
#define Q_DRV_FRACT_CLK_INSTANCE_DEFINE(ID)                                                                            \
    {                                                                                                                  \
        .baseAddr = Q_REG_FRACT_CLK_BASEADDR_GET(ID), /* */                                                            \
            .id = ID,                                 /* */                                                            \
    }

/**
 * @brief Pin configuration macro for Fractional Clock Generator instance in master transmitter mode.
 *
 * @param[in] INSTANCE_ID ID of the peripheral instance.
 * @param[in] OUT_PIN     Clock output pin.
 */
#define Q_DRV_FRACT_CLK_PIN_CONFIG(INSTANCE_ID, OUT_PIN)                                                               \
    {                                                                                                                  \
        .pin = OUT_PIN,                                                                          /* */                 \
            .alternate = Q_DRV_IOB_ALT_GET(OUT_PIN, FRACT_CLK_GEN_CLK_FRACT_CLKGEN, INSTANCE_ID) /* */                 \
    }

/**
 * @brief Default Fractional Clock Generator configuration macro.
 *
 * @param[in] FREQUENCY_HZ Clock frequency in Hz.
 */
#define Q_DRV_FRACT_CLK_CONFIG_DEFAULT(FREQUENCY_HZ)                                                                   \
    {                                                                                                                  \
        .freq = FREQUENCY_HZ, /* */                                                                                    \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the Fractional Clock Generator instance pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin configuration.
 * @retval Q_ERR_INVSTATE Pin already in use.
 */
qResult_t qDrvFractClk_PinConfigSet(const qDrvFractClk_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the Fractional Clock Generator instance pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin was not mapped to this peripheral.
 */
qResult_t qDrvFractClk_PinConfigClear(const qDrvFractClk_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize Fractional Clock Generator instance.
 *
 * @param[in] pInstance Fractional Clock Generator instance pointer.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid configuration.
 * @retval Q_ERR_OUTOFMEMORY   Unable to allocate DMA channel.
 */
qResult_t qDrvFractClk_Init(qDrvFractClk_t* pInstance, const qDrvFractClk_Config_t* const pConfig);

/**
 * @brief Deinitialize the Fractional Clock Generator instance.
 *
 * @param[in] pInstance Fractional Clock Generator instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvFractClk_Deinit(qDrvFractClk_t* const pInstance);

/**
 * @brief Check if Fractional Clock Generator instance is initialized.
 *
 * @param[in] pInstance Fractional Clock Generator instance pointer.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvFractClk_InitCheck(qDrvFractClk_t* const pInstance);

/**
 * @brief Enable or disable the Fractional Clock Generator instance.
 *
 * @param[in] pInstance Fractional Clock Generator instance pointer.
 * @param[in] enable    Enable or disable the Fractional Clock Generator instance.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is already in the requested state.
 */
qResult_t qDrvFractClk_EnableSet(qDrvFractClk_t* const pInstance, Bool enable);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_FRACT_CLK_H_
