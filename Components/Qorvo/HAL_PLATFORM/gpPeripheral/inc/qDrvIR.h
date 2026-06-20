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

#ifndef _Q_DRV_IR_H_
#define _Q_DRV_IR_H_

/**
 * @defgroup Q_DRV_IR_H General Infrared generator driver
 * @{
 *
 * @brief This is a driver for IR (Infrared generator).
 *
 * @file qDrvIR.h
 *
 */

/*****************************************************************************
 *                    Include Definitions
 *****************************************************************************/

#include "qRegIR.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvIR_Drive_t */
//@{
/** @brief Push-pull. */
#define qDrvIR_DrivePushPull 0
/** @brief Open drain. */
#define qDrvIR_DriveOpenDrain 1
/** @brief High-drive sink. */
#define qDrvIR_DriveHighSink 2
/** @typedef qDrvIR_Drive_t
 *  @brief IR generator output drive mode.
 */
typedef UInt8 qDrvIR_Drive_t;
//@}

/** @enum qDrvIR_Mode_t */
//@{
/** @brief Direct register mode. */
#define qDrvIR_ModeDirect 0
/** @brief RAM pattern mode. */
#define qDrvIR_ModePattern 1
/** @brief RAM time-based mode. */
#define qDrvIR_ModeTime 2
/** @typedef qDrvIR_Mode_t
 *  @brief IR generator mode of operation.
 */
typedef UInt8 qDrvIR_Mode_t;
//@}

/** @enum qDrvIR_TimeUnit_t */
//@{
/** @brief 500ns. */
#define qDrvIR_TimeUnit500ns qRegIR_TimeUnit500ns
/** @brief 1us. */
#define qDrvIR_TimeUnit1us qRegIR_TimeUnit1us
/** @brief 2us. */
#define qDrvIR_TimeUnit2us qRegIR_TimeUnit2us
/** @brief 4us. */
#define qDrvIR_TimeUnit4us qRegIR_TimeUnit4us
/** @typedef qDrvIR_TimeUnit_t
 *  @brief Time unit for time-based pattern.
 */
typedef qRegIR_TimeUnit_t qDrvIR_TimeUnit_t;
//@}

/** @enum qDrvIR_Event_t */
//@{
/** @brief Pattern start event. */
#define qDrvIR_EventStart qRegIR_IrqSequenceStart
/** @brief Index match event. */
#define qDrvIR_EventMatch qRegIR_IrqIndexMatch
/** @brief Pattern repetition event. */
#define qDrvIR_EventRepeat qRegIR_IrqSequenceRepeat
/** @brief Pattern complete event. */
#define qDrvIR_EventDone qRegIR_IrqSequenceDone
/** @typedef qDrvIR_Event_t
 *  @brief IR generator driver event type.
 */
typedef UInt8 qDrvIR_Event_t;
//@}

/** @brief IR driver internal data structure. */
typedef struct {
    UInt8 data[9];
} __attribute__((aligned(4))) qDrvIR_InternalData_t;

/** @brief IR generator instance structure. */
typedef struct {
    qDrvIR_InternalData_t internalData; /**< Internal driver data. */
} qDrvIR_t;

/** @brief IR config structure. */
typedef struct {
    qDrvIR_Drive_t outputDrive; /**< Output pin drive. The output cannot be inverted when using high drive sink mode. */
    Bool outputInvert;          /**<  Invert the output value. */
    Bool inputInvert;           /**<  Invert the input value when using external modulation source. */
} qDrvIR_Config_t;

/** @brief IR pin definition structure. */
typedef union {
    struct {
        qDrvIOB_PinAlt_t output;  /**< IR output pin/alternate pair. */
        qDrvIOB_PinAlt_t input;   /**< IR input pin/alternate pair. */
    };
    qDrvIOB_PinAlt_t pinArray[2]; /**< Internal pin config array. */
} qDrvIR_PinConfig_t;

/**
 * @brief IR event callback.
 *
 * @param[in] arg   User-supplied argument.
 * @param[in] event IR driver event.
 */
typedef void (*qDrvIR_Callback_t)(void* arg, qDrvIR_Event_t event);

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro for defining IR instance. */
#define Q_DRV_IR_INSTANCE_DEFINE(ID)                                                                                   \
    {                                                                                                                  \
        .internalData = {.data = {Q_REG_IR_BASEADDR_GET(ID) ? 0 : 0}}, /* */                                           \
    }

/** @brief Default configuration macro for IR instance. */
#define Q_DRV_IR_CONFIG_DEFAULT()                                                                                      \
    {                                                                                                                  \
        .outputDrive = qDrvIR_DrivePushPull, /* */                                                                     \
            .outputInvert = false,           /* */                                                                     \
            .inputInvert = false,            /* */                                                                     \
    }

/** @brief IR output-only pin configuration. */
#define Q_DRV_IR_PIN_CONFIG_OUT(OUT_PIN)                                                                               \
    {                                                                                                                  \
        .output = {.pin = OUT_PIN, .alternate = Q_DRV_IOB_ALT_GET(OUT_PIN, IR, DOUT)}, /* */                           \
            .input = {.pin = Q_NOPIN, .alternate = 0},                                 /* */                           \
    }

/** @brief IR input + output pin configuration. */
#define Q_DRV_IR_PIN_CONFIG_INOUT(OUT_PIN, IN_PIN)                                                                     \
    {                                                                                                                  \
        .output = {.pin = OUT_PIN, .alternate = Q_DRV_IOB_ALT_GET(OUT_PIN, IR, DOUT)}, /* */                           \
            .input = {.pin = IN_PIN, .alternate = Q_DRV_IOB_ALT_GET(IN_PIN, IR, DIN)}, /* */                           \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the IR generator instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Pin(s) already in use.
 */
qResult_t qDrvIR_PinConfigSet(const qDrvIR_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the IR generator instance pins.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE At least one pin was not mapped to this peripheral.
 */
qResult_t qDrvIR_PinConfigClear(const qDrvIR_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize the IR generator instance.
 *
 * @param[in] pInstance   IR generator instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] callback    Callback function.
 * @param[in] arg         User-supplied argumend passed to the callback function.
 * @param[in] irqPriority Interrupt priority.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVSTATE     Peripheral already initialized.
 * @retval Q_ERR_INVPARAM     Invalid configuration.
 * @retval Q_ERR_NOTSUPPORTED Provided configuration is not supported.
 */
qResult_t qDrvIR_Init(const qDrvIR_t* pInstance, const qDrvIR_Config_t* const pConfig, const qDrvIR_Callback_t callback,
                      void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the IR generator instance.
 *
 * @param[in] pInstance IR generator instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvIR_Deinit(const qDrvIR_t* pInstance);

/**
 * @brief Stop the ongoing RAM sequence immediately.
 *
 * @param[in] pInstance IR generator instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvIR_Abort(const qDrvIR_t* pInstance);

/**
 * @brief Stop the ongoing RAM sequence after the current repetition.
 * @note  This function returns immediately. User may check for pattern completion with
 *        @p qDrvIR_BusyCheck.
 *
 * @param[in] pInstance IR generator instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvIR_Stop(const qDrvIR_t* pInstance);

/**
 * @brief Check the peripheral busy state.
 *
 * @param[in] pInstance IR generator instance pointer.
 *
 * @returns IR pattern status.
 */
Bool qDrvIR_BusyCheck(const qDrvIR_t* pInstance);

/**
 * @brief Calculate carrier parameters.
 *
 * @param[in]  pInstance     IR generator instance pointer.
 * @param[in]  periodNs      Carrier period in nanoseconds.
 * @param[in]  dutyCycle     Carrier duty cycle.
 * @param[out] pPrescalerDiv Calculated prescaler value pointer.
 * @param[out] pWrap         Calculated wrap value pointer.
 * @param[out] pThreshold    Calculated threshold value pointer.
 */
void qDrvIR_CarrierParamCalculate(const qDrvIR_t* pInstance, UInt32 periodNs, UInt8 dutyCycle, UInt8* pPrescalerDiv,
                                  UInt8* pWrap, UInt8* pThreshold);

/**
 * @brief Set the carrier prescaler divider value.
 *
 * @param[in] pInstance    IR generator instance pointer.
 * @param[in] prescalerDiv Carrier prescaler divider.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 * @retval Q_ERR_INVPARAM      Invalid parameters.
 */
qResult_t qDrvIR_CarrierPrescalerSet(const qDrvIR_t* pInstance, UInt8 prescalerDiv);

/**
 * @brief Set the carrier parameters.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] wrap      Carrier period.
 * @param[in] threshold Carrier threshold (duty cycle) value.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 */
qResult_t qDrvIR_CarrierTimingSet(const qDrvIR_t* pInstance, UInt8 wrap, UInt8 threshold);

/**
 * @brief Set the alternate carrier parameters.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] wrap      Carrier period.
 * @param[in] threshold Carrier threshold (duty cycle) value.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 */
qResult_t qDrvIR_AltCarrierTimingSet(const qDrvIR_t* pInstance, UInt8 wrap, UInt8 threshold);

/**
 * @brief Switch between primary and alternate carrier parameters.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] alt       Use alternate carrier.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 */
qResult_t qDrvIR_AltCarrierSet(const qDrvIR_t* pInstance, Bool alt);

/**
 * @brief Configure the index match event.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] index     Sample index on which the event is triggered.
 * @param[in] altToggle Enable toggling between the primary and the alternate
 *                      carrier parameters on match event.
 * @param[in] irqEnable Enable interrupt on match event.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 */
qResult_t qDrvIR_IndexMatchSet(const qDrvIR_t* pInstance, UInt8 index, Bool altToggle, Bool irqEnable);

/**
 * @brief Start the pattern generation in timed mode.
 * @note Time unit for this mode can be set with @p qDrvIR_PatternTimeUnitSet.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] pPattern  Sample buffer pointer. Sample represents 16 consecutive output bits.
 * @param[in] length    Number of samples in the buffer. Must be greater than 0.
 * @param[in] repeat    Repeat pattern. Must stopped with @p qDrvIR_Stop or @p qDrvIR_Abort.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is not initialized.
 * @retval Q_ERR_BUSY     Peripheral is busy.
 * @retval Q_ERR_INVPARAM Invalid parameters.
 */
qResult_t qDrvIR_PatternStart(const qDrvIR_t* pInstance, UInt16* pPattern, UInt8 length, Bool repeat);

/**
 * @brief Set the time unit for pattern mode.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] tu        Number of carrier periods per one bit in the sample buffer.
 *                      Must be greater than 0.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 * @retval Q_ERR_INVPARAM      Invalid parameters.
 */
qResult_t qDrvIR_PatternTimeUnitSet(const qDrvIR_t* pInstance, UInt8 tu);

/**
 * @brief Start the pattern generation in timed mode.
 * @note Time unit for this mode can be set with @p qDrvIR_TimeUnitSet.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] pPattern  Sample buffer containing on and off times.
 * @param[in] length    Number of samples in the buffer. Must be an even number.
 * @param[in] repeat    Repeat pattern. Must stopped with @p qDrvIR_Stop or @p qDrvIR_Abort.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 * @retval Q_ERR_INVPARAM      Invalid parameters.
 */
qResult_t qDrvIR_TimePatternStart(const qDrvIR_t* pInstance, UInt16* pPattern, UInt8 length, Bool repeat);

/**
 * @brief Set the time unit for time-based pattern mode.
 *
 * @param[in] pInstance IR generator instance pointer.
 * @param[in] tu        Time unit. 500ns to 4us.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_BUSY          Peripheral is busy.
 */
qResult_t qDrvIR_TimeUnitSet(const qDrvIR_t* pInstance, qDrvIR_TimeUnit_t tu);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_IR_H_
