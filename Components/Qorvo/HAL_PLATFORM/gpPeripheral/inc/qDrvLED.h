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

#ifndef _Q_DRV_LED_H_
#define _Q_DRV_LED_H_

/**
 * @defgroup Q_DRV_LED_H LED driver
 * @{
 *
 * @brief This is a driver for LED peripheral.
 *        It provides the functionality for simple LED control as well as brightness and fading features.
 *
 * @file qDrvLED.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegLED.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUICInternal.h"
#endif /* defined(REG_QUIC_PRESENT) */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvLED_SrcClkFreq_t */
//@{
#define qDrvLED_SrcClkFreq32M qRegLED_SrcClkFreq32M
#define qDrvLED_SrcClkFreq8M  qRegLED_SrcClkFreq8M
#define qDrvLED_SrcClkFreq2M  qRegLED_SrcClkFreq2M
#define qDrvLED_SrcClkFreq62K qRegLED_SrcClkFreq62K
/** @typedef qDrvLED_SrcClkFreq_t
 *  @brief LED clock frequency.
 */
typedef qRegLED_SrcClkFreq_t qDrvLED_SrcClkFreq_t;
//@}

/** @brief LED peripheral config structure. */
typedef struct {
    UInt32 frequency; /**< PWM Frequency time in Hertz. */
    UInt32 slopeTime; /**< Slope time in milliseconds. */
} qDrvLED_Config_t;

/** @brief LED channel config structure. */
typedef struct {
    qRegCommon_Drive_t drive; /**< GPIO drive mode. */
    Bool invert;              /**< Invert output. */
    Bool allowSleep;          /**< Allow going to sleep when duty cycling. */
} qDrvLED_ChannelConfig_t;

/** @brief LED pin definition structure. */
typedef struct {
    qDrvIOB_PinAlt_t led; /**< LED output pin/alternate pair. */
} qDrvLED_PinConfig_t;

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvLED_QuicAction_t */
//@{
#define qDrvLED_QuicActionTypeNone   qRegLED_QuicActionNone
#define qDrvLED_QuicActionTypeSet    qRegLED_QuicActionSet
#define qDrvLED_QuicActionTypeClr    qRegLED_QuicActionClr
#define qDrvLED_QuicActionTypeToggle qRegLED_QuicActionToggle
/** @typedef qDrvLED_QuicAction_t
 *  @brief LED clock frequency.
 */
typedef qRegLED_QuicActionType_t qDrvLED_QuicActionType_t;
//@}

/** @brief LED QUIC action configuration structure. */
typedef struct {
    UInt8 ledChannel;                   /**< Led channel to be used. */
    UInt8 actionIdx;                    /**< QUIC action index. */
    qRegLED_QuicActionType_t action;    /**< QUIC action to be applied. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD(); /**< Required action field. */
} qDrvLED_QuicActionConfig_t;
#endif                                  /* REG_QUIC_PRESENT */

/** @brief LED instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvLED_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Default configuration macro for LED peripheral using PWM without fading. */
#define Q_DRV_LED_CONFIG_DEFAULT                                                                                       \
    {                                                                                                                  \
        .frequency = 1000,  /* */                                                                                      \
            .slopeTime = 0, /* */                                                                                      \
    }

/**
 * @brief Macro for defining instance pin configuration.
 *
 * @param[in] CHANNEL     LED channel.
 * @param[in] CHANNEL_PIN LED pin.
 */
#define Q_DRV_LED_PIN_CONFIG(CHANNEL, CHANNEL_PIN)                                                                     \
    {                                                                                                                  \
        .led = {.pin = CHANNEL_PIN, .alternate = Q_DRV_IOB_ALT_GET(CHANNEL_PIN, LED_LED, CHANNEL)}, /* */              \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the LED output pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin already in use.
 */
qResult_t qDrvLED_PinConfigSet(const qDrvLED_PinConfig_t* pPinConfig);

/**
 * @brief Deinitialize the LED output pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin was not mapped to this peripheral.
 */
qResult_t qDrvLED_PinConfigClear(const qDrvLED_PinConfig_t* pPinConfig);

/**
 * @brief Initialize LED peripheral.
 *
 * This drivers allocates up to two timer instances for LED duty cycling
 * and fading feature. User can choose PWM only mode (without fading)
 * by setting pConfig->fadeTickUS to 0 to save one timer instance or disable
 * PWM completely by setting pConfig->pwm to qDrvLED_PwmNone.
 * The frequency of PWM is a coarse value (multiple of 1000/1.024 Hz).
 * PWM is required when using fading.
 * Fading speed is defined as number of microseconds per 1/256 unit of duty cycle change.
 *
 * @param[in] pInstance LED instance pointer.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral not initialized.
 * @retval Q_ERR_INVPARAM Provided configuration is not correct.
 */
qResult_t qDrvLED_Init(qDrvLED_t* const pInstance, const qDrvLED_Config_t* pConfig);

/**
 * @brief Deinitialize the LED instance.
 *
 * @param[in] pInstance LED instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvLED_Deinit(qDrvLED_t* const pInstance);

/**
 * @brief Check if the LED peripheral is initialized.
 *
 * @param[in] pInstance LED instance pointer.
 *
 * @return True if the peripheral is initialized, false otherwise.
 */
Bool qDrvLED_InitCheck(qDrvLED_t* const pInstance);

/**
 * @brief Configure LED output channel.
 *
 * @param[in] pInstance LED instance pointer.
 * @param[in] channel   Channel number.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Incorrect channel number.
 */
qResult_t qDrvLED_Configure(qDrvLED_t* const pInstance, UInt8 channel, const qDrvLED_ChannelConfig_t* const pConfig);

/**
 * @brief Simple LED on/off switch with 100% duty cycle.
 *
 * @param[in] pInstance LED instance pointer.
 * @param[in] channel   Channel number.
 * @param[in] enable    True to enable, false to disable.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Incorrect channel number.
 */
qResult_t qDrvLED_Set(qDrvLED_t* const pInstance, UInt8 channel, Bool enable);

/**
 * @brief Set the LED brightness state.
 *
 * @param[in] pInstance  LED instance pointer.
 * @param[in] channel    Channel number.
 * @param[in] brightness Duty cycle (0-255). The led will be cycled only if PWM timer was enabled during initialization.
 * @param[in] fade       Use fade-in/fade-out. Ignored when peripheral was initialized without fading enabled.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Incorrect channel number.
 */
qResult_t qDrvLED_SetBrightness(qDrvLED_t* const pInstance, UInt8 channel, UInt8 brightness, Bool fade);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_LED_H_
