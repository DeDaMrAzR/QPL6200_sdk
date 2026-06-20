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

#ifndef _Q_DRV_GPIO_H_
#define _Q_DRV_GPIO_H_

/**
 * @defgroup Q_DRV_GPIO_H General Purpose IO driver
 * @{
 *
 * @brief This is a driver for GPIO.
 *
 * @file qDrvGPIO.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegGPIO.h"
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

/** @enum qDrvGPIO_Direction_t */
//@{
#define qDrvGPIO_DirectionInput  qRegGPIO_DirectionInput
#define qDrvGPIO_DirectionOutput qRegGPIO_DirectionOutput
/** @typedef qDrvGPIO_Direction_t
 *  @brief GPIO Direction.
 */
typedef UInt8 qDrvGPIO_Direction_t;
//@}

/** @enum qDrvGPIO_IrqType_t */
//@{
/** @brief No interrupt. */
#define qDrvGPIO_IrqTypeNone 0
/** @brief Level-triggered, high. */
#define qDrvGPIO_IrqTypeHighLevel 1
/** @brief Level-triggered, low. */
#define qDrvGPIO_IrqTypeLowLevel 2
/** @brief Edge-triggered, rising. */
#define qDrvGPIO_IrqTypeRisingEdge 3
/** @brief Edge-triggered, falling. */
#define qDrvGPIO_IrqTypeFallingEdge 4
/** @typedef qDrvGPIO_IrqType_t
 *  @brief GPIO input interrupt type.
 */
typedef UInt8 qDrvGPIO_IrqType_t;
//@}

/** @enum qDrvGPIO_TimestampMode_t */
//@{
/** @brief No timestamp. */
#define qDrvGPIO_TimestampModeNone 0
/** @brief Timestamp on interrupt. */
#define qDrvGPIO_TimestampModeInterrupt 1
/** @brief Timestamp on DMA. */
#define qDrvGPIO_TimestampModeDma 2
/** @typedef qDrvGPIO_TimestampMode_t
 *  @brief GPIO timestamp mode.
 */
typedef UInt8 qDrvGPIO_TimestampMode_t;
//@}

/** @enum qDrvGPIO_Wakeup_t */
//@{
/** @brief Pin wakeup disabled. */
#define qDrvGPIO_WakeupNone qRegGPIO_WakeupNone
/** @brief Wakeup from sleep on rising edge. */
#define qDrvGPIO_WakeupRising qRegGPIO_WakeupRising
/** @brief Wakeup from sleep on falling edge. */
#define qDrvGPIO_WakeupFalling qRegGPIO_WakeupFalling
/** @brief Wakeup from sleep on both edges. */
#define qDrvGPIO_WakeupBoth qRegGPIO_WakeupBoth
/** @typedef qDrvGPIO_Wakeup_t
 *  @brief GPIO pin wakeup mode/edge.
 */
typedef UInt8 qDrvGPIO_Wakeup_t;
//@}

/** @enum qDrvGPIO_SampleClock_t */
//@{
/** @brief 32 MHz. */
#define qDrvGPIO_SampleClock32M qRegGPIO_SampleClk32M
/** @brief 8 MHz. */
#define qDrvGPIO_SampleClock8M qRegGPIO_SampleClk8M
/** @brief 4 MHz. */
#define qDrvGPIO_SampleClock4M qRegGPIO_SampleClk4M
/** @brief 2 MHz. */
#define qDrvGPIO_SampleClock2M qRegGPIO_SampleClk2M
/** @typedef qDrvGPIO_SampleClock_t
 *  @brief GPIO input sample clock.
 */
typedef UInt8 qDrvGPIO_SampleClock_t;
//@}

/** @brief GPIO peripheral config structure. */
typedef struct {
    qDrvGPIO_SampleClock_t sampleClock; /**< Input sample clock. */
} qDrvGPIO_Config_t;

/** @brief Default configuration macro for GPIO peripheral. */
#define Q_DRV_GPIO_CONFIG_DEFAULT                                                                                      \
    {                                                                                                                  \
        .sampleClock = qDrvGPIO_SampleClock32M,                                                                        \
    }

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvGPIO_QuicConfigMask_t */
//@{
/** @brief Configure GPIO direction as output. */
#define qDrvGPIO_QuicConfigMaskDirection BM(0)
/** @brief Disable alternate functions for pin. */
#define qDrvGPIO_QuicConfigMaskAlternate BM(1)
/** @brief Set default IOB configuration for pin. */
#define qDrvGPIO_QuicConfigMaskDefault BM(2)
/** @typedef qDrvGPIO_QuicConfigMask_t
 *  @brief GPIO QUIC action configuration mask.
 */
typedef UInt8 qDrvGPIO_QuicConfigMask_t;
//@}

/** @brief GPIO QUIC trigger configuration structure. */
typedef struct {
    UInt8 triggerIdx;                    /**< QUIC trigger index */
    UInt8 pinIdx;                        /**< QUIC pin index. */
    qDrvGPIO_QuicConfigMask_t mask;      /**< QUIC trigger configuration mask. */
    Q_DRV_QUIC_INTERNAL_TRIGGER_FIELD(); /**< Required trigger field. */
} qDrvGPIO_QuicTriggerConfig_t;

/** @brief GPIO QUIC action configuration structure. */
typedef struct {
    UInt8 actionIdx;                      /**< QUIC action index. */
    qRegGPIO_QuicActionType_t actionType; /**< QUIC action type. */
    UInt8 pinIdx;                         /**< QUIC pin index. */
    qDrvGPIO_QuicConfigMask_t mask;       /**< QUIC action configuration mask. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD();   /**< Required action field. */
} qDrvGPIO_QuicActionConfig_t;
#endif                                    /* REG_QUIC_PRESENT */

/** @brief GPIO instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvGPIO_t;

/**
 * @brief Timestamp stream callback.
 *
 * @param[in] arg          User-supplied argument.
 * @param[in] buffer       Timestamps buffer pointer.
 * @param[in] bufferLength Number of timestamp words ready to process.
 */
typedef void (*qDrvGPIO_CbTimestamp_t)(void* arg, UInt8 tsNum, UInt32* buffer, UInt16 bufferLength);

/**
 * @brief GPIO input interrupt.
 *
 * @param[in] arg  User-supplied argument.
 * @param[in] gpio GPIO pin number.
 */
typedef void (*qDrvGPIO_CbInput_t)(void* arg, UInt8 gpio);

/** @brief Callback set structure definition. */
typedef struct {
    qDrvGPIO_CbInput_t gpio;          /**< Global GPIO input interrupt callback. */
#ifdef REG_GPIO_HAS_TIMESTAMP
    qDrvGPIO_CbTimestamp_t timestamp; /**< GPIO timestamp callback. */
#endif                                /* REG_GPIO_HAS_TIMESTAMP */
} qDrvGPIO_Callbacks_t;

/** @brief GPIO input configuration structure. */
typedef struct {
    qDrvIOB_Pull_t pull;         /**< IOB input pull mode. */
    Bool schmittTrigger;         /**< True if the IOB Schmitt trigger is to be enabled, false otherwise. */
    qDrvGPIO_IrqType_t irqType;  /**< GPIO input interrupt type. */
    Bool highPriority;           /**< Use higher or low interrupt priority. */
    qDrvGPIO_Wakeup_t wakeup;    /**< GPIO pin wakeup mode/edge. */
    qDrvGPIO_CbInput_t callback; /**< GPIO callback. If set to NULL, global callback is used with global argument. */
    void* arg;                   /**< User argument. */
} qDrvGPIO_InputConfig_t;

#define Q_DRV_GPIO_INPUT_CONFIG_DEFAULT                                                                                \
    {                                                                                                                  \
        .pull = qDrvIOB_PullNone,            /* */                                                                     \
            .schmittTrigger = false,         /* */                                                                     \
            .irqType = qDrvGPIO_IrqTypeNone, /* */                                                                     \
            .highPriority = false,           /* */                                                                     \
            .wakeup = qDrvGPIO_WakeupNone,   /* */                                                                     \
            .callback = NULL,                /* */                                                                     \
            .arg = NULL,                     /* */                                                                     \
    }

/** @brief GPIO output configuration structure. */
typedef struct {
    qDrvIOB_Drive_t drive;       /**< IOB output drive strength. */
    qDrvIOB_SlewRate_t slewRate; /**< IOB output slew rate. */
} qDrvGPIO_OutputConfig_t;

#define Q_DRV_GPIO_OUTPUT_CONFIG_DEFAULT                                                                               \
    {                                                                                                                  \
        .drive = qDrvIOB_Drive2mA,            /* */                                                                    \
            .slewRate = qDrvIOB_SlewRateSlow, /* */                                                                    \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initialize GPIO peripheral.
 *
 * @param[in] pInstance   GPIO instance structure.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] pCallbacks  Set of global callback functions pointer.
 * @param[in] arg         User-supplied argument passed to the global callback function.
 * @param[in] irqPriority ARM interrupt priority.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral instance is already initialized.
 * @retval Q_ERR_INVPARAM Provided configuration is not correct.
 */
qResult_t qDrvGPIO_Init(qDrvGPIO_t* const pInstance, const qDrvGPIO_Config_t* pConfig,
                        const qDrvGPIO_Callbacks_t* const pCallbacks, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the GPIO instance.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvGPIO_Deinit(void);

/**
 * @brief Configure GPIO pin in input mode.
 *
 * @param[in] gpio    GPIO pin number.
 * @param[in] pConfig GPIO configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin number.
 * @retval Q_ERR_BUSY          GPIO pin is already used by another peripheral.
 * @retval Q_ERR_NOTSUPPORTED  GPIO pin is not available on this package.
 */
qResult_t qDrvGPIO_InputConfigSet(UInt8 gpio, qDrvGPIO_InputConfig_t* pConfig);

/**
 * @brief Configure GPIO pin in output mode.
 *
 * @param[in] gpio    GPIO pin number.
 * @param[in] pConfig GPIO configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin number.
 * @retval Q_ERR_BUSY          GPIO pin is already used by another peripheral.
 * @retval Q_ERR_NOTSUPPORTED  GPIO pin is not available on this package.
 */
qResult_t qDrvGPIO_OutputConfigSet(UInt8 gpio, qDrvGPIO_OutputConfig_t* pConfig);

/**
 * @brief Enable/disable pin type interrupt.
 *        GPIO input interrupt can be split between two priorities.
 *        To increase the specific pin priority over the other ones,
 *        set the priority to high.
 *
 * @param[in] gpio         GPIO pin number.
 * @param[in] irqType      Interrupt type to be applied.
 * @param[in] highPriority Use higher or low interrupt priority.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin number.
 * @retval Q_ERR_INVSTATE      GPIO pin is not configured as input.
 * @retval Q_ERR_BUSY          Interrupt for specified pin is already enabled.
 */
qResult_t qDrvGPIO_IrqConfigSet(UInt8 gpio, qDrvGPIO_IrqType_t irqType, Bool highPriority);

/**
 * @brief Enable GPIO input interrupt.
 *
 * @param[in] gpio GPIO pin number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin number.
 * @retval Q_ERR_INVSTATE      GPIO pin is not configured as input or callbacks are not set.
 * @retval Q_ERR_BUSY          Interrupt for specified pin is already enabled.
 */
qResult_t qDrvGPIO_IrqEnable(UInt8 gpio);

/**
 * @brief Disable GPIO input interrupt.
 *
 * @param[in] gpio GPIO pin number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin number.
 */
qResult_t qDrvGPIO_IrqDisable(UInt8 gpio);

/**
 * @brief Check if GPIO input interrupt is enabled.
 *
 * @param[in] gpio GPIO pin number.
 *
 * @returns True if the GPIO input interrupt is enabled, false otherwise.
 */
Bool qDrvGPIO_IrqEnabledCheck(UInt8 gpio);

/**
 * @brief Configure pin wakeup feature.
 *
 * @param[in] gpio   GPIO pin number.
 * @param[in] wakeup Wakeup mode.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin number or wring wakeup pin.
 */
qResult_t qDrvGPIO_WakeupSet(UInt8 gpio, qDrvGPIO_Wakeup_t wakeup);

/**
 * @brief Read GPIO input status.
 *
 * @param[in] gpio GPIO pin number.
 *
 * @returns True if the GPIO input is in the high state, false otherwise.
 */
Bool qDrvGPIO_Read(UInt8 gpio);

/**
 * @brief Read multiple GPIO inputs.
 *
 * @returns GPIO input state mask.
 */
UInt32 qDrvGPIO_MaskRead(void);

/**
 * @brief Set GPIO output status.
 *
 * @param[in] gpio  GPIO pin number.
 * @param[in] value Output value
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      GPIO pin is not configured as output.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvGPIO_Write(UInt8 gpio, Bool value);

/**
 * @brief Get GPIO output value.
 *
 * @param[in] gpio GPIO pin number.
 *
 * @returns True if the GPIO output is in the high state, false otherwise.
 */
Bool qDrvGPIO_OutputGet(UInt8 gpio);

/**
 * @brief Get all output GPIOs.
 *
 * @returns GPIO output state mask.
 */
UInt32 qDrvGPIO_MaskOutputGet(void);

/**
 * @brief Set all output GPIOs.
 *
 * @param[in] mask Bit mask of GPIO outputs values.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      GPIO pin is not configured as output.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvGPIO_MaskWrite(UInt32 mask);

/**
 * @brief Configure timestamp feature.
 *        The timestamp is captured every time GPIO interrupt is generated.
 *        Application must first configure the GPIO as input.
 *
 * @param[in] tsNum Timestamp slot number.
 * @param[in] gpio  GPIO pin number.
 * @param[in] mode  Timestamp mode.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin or timestamp index.
 * @retval Q_ERR_INVSTATE      Timestamp can be used only with edge interrupt.
 * @retval Q_ERR_OUTOFMEMORY   No free DMA channels left.
 * @retval Q_ERR_BUSY          Timestamp is enabled.
 */
qResult_t qDrvGPIO_TimestampConfigSet(UInt8 tsNum, UInt8 gpio, qDrvGPIO_TimestampMode_t mode);

/**
 * @brief Set the timestamp buffer.
 *
 * Buffer can be set to NULL to disable the buffered mode and call the callback function on every interrupt.
 * Non-buffered mode is acceptable only if the timestamp is configured in the interrupt mode.
 * The buffer can be set only when the timestamp is configured and disabled.
 *
 * @param[in] tsNum        Timestamp slot number.
 * @param[in] buffer       Buffer sample pointer.
 * @param[in] samplesCount Samples count.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid GPIO pin number or timestamp slot number. Wrong buffer configuration.
 * @retval Q_ERR_INVSTATE      Timestamp is not configured.
 * @retval Q_ERR_BUSY          Timestamp is enabled.
 * @retval Q_ERR_OUTOFMEMORY   No DMA channel configured.
 */
qResult_t qDrvGPIO_TimestampBufferSet(UInt8 tsNum, UInt32* buffer, UInt16 samplesCount);

/**
 * @brief Enable timestamp feature.
 *
 * @param[in] tsNum Timestamp slot number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid timestamp slot number.
 * @retval Q_ERR_INVSTATE      Timestamp is not configured or DMA channel is not allocated.
 * @retval Q_ERR_BUSY          Timestamp is already enabled.
 */
qResult_t qDrvGPIO_TimestampEnable(UInt8 tsNum);

/**
 * @brief Disable timestamp feature.
 *
 * @param[in] tsNum Timestamp slot number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Invalid timestamp slot number.
 * @retval Q_ERR_INVSTATE      Timestamp is already disabled or DMA channel is not allocated.
 */
qResult_t qDrvGPIO_TimestampDisable(UInt8 tsNum);

/**
 * @brief Check if timestamp is enabled.
 *
 * @param[in] tsNum Timestamp slot number.
 *
 * @returns True if the timestamp is enabled, false otherwise.
 */
Bool qDrvGPIO_TimestampEnabledCheck(UInt8 tsNum);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_GPIO_H_
