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

#ifndef _Q_DRV_TIMERXL_H_
#define _Q_DRV_TIMERXL_H_

/**
 * @defgroup Q_DRV_TIMERXL_H TimerXL driver
 * @{
 *
 * @brief This is a driver for TimerXL peripheral.
 *
 * @file qDrvTimerXL.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegTimerXL.h"
#include "qDrvCommon.h"

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUICInternal.h"
#endif /* defined(REG_QUIC_PRESENT) */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvTimerXL_Mode_t */
//@{
/** @brief TimerXL Block mode not set.
 */
#define qDrvTimerXL_ModeNone 0
/** @brief 16-bit mode. One TimerXL block can be used as
 *         two independent 16-bit timers.
 */
#define qDrvTimerXL_Mode16Bit 1
/** @brief 32-bit mode. Uses the whole TimerXL block.
 */
#define qDrvTimerXL_Mode32Bit 2
/** @typedef qDrvTimerXL_Mode_t
 *  @brief TimerXL mode of operation. 16- or 32-bit.
 */
typedef UInt8 qDrvTimerXL_Mode_t;
//@}

/** @enum qDrvTimerXL_CountMode_t */
//@{
/** @brief Input clock prescaled from system clock.
 */
#define qDrvTimerXL_CountModePrescaled 0
/** @brief Counting triggered externally by QUIC.
 */
#define qDrvTimerXL_CountModeExternal 1
/** @typedef qDrvTimerXL_CountMode_t
 *  @brief TimerXL count mode.
 */
typedef UInt8 qDrvTimerXL_CountMode_t;
//@}

/** @enum qDrvTimerXL_EventType_t */
//@{
/** @brief Timer overflow event.
 */
#define qDrvTimerXL_EventTypeOverflow_t 0
/** @brief Compare event.
 */
#define qDrvTimerXL_EventTypeCompare_t 1
/** @typedef qDrvTimerXL_EventType_t
 *  @brief TimerXL event type.
 */
typedef UInt8 qDrvTimerXL_EventType_t;
//@}

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvTimerXL_QUICSrcTrigger_t */
//@{
#define qDrvTimerXL_QUICSrcTriggerCC0Match 0
#define qDrvTimerXL_QUICSrcTriggerCC1Match 1
#define qDrvTimerXL_QUICSrcTriggerCC2Match 2
#define qDrvTimerXL_QUICSrcTriggerCC3Match 3
#define qDrvTimerXL_QUICSrcTriggerOverflow 4
/** @typedef qDrvTimerXL_QUICSrcTrigger_t
 *  @brief TimerXL Quic source trigger.
 */
typedef UInt8 qDrvTimerXL_QUICSrcTrigger_t;
//@}

/** @brief TimerXL QUIC trigger configuration structure. */
typedef struct {
    qDrvTimerXL_QUICSrcTrigger_t srcTrigger; /**< Trigger source. */
    Q_DRV_QUIC_INTERNAL_TRIGGER_FIELD();     /**< Required trigger field. */
} qDrvTimerXL_QuicTriggerConfig_t;

/** @brief TimerXL QUIC destination configuration structure. */
typedef struct {
    UInt8 actionIdx;                         /**< QUIC action index. */
    qRegTimerXL_QuicActionType_t actionType; /**< QUIC action type. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD();      /**< Required action field. */
} qDrvTimerXL_QuicActionConfig_t;
#endif                                       /* REG_QUIC_PRESENT */

/** @brief TimerXL event structure. */
typedef struct {
    qDrvTimerXL_EventType_t type; /**< Event type. Overflow or compare. */
    UInt8 cc;                     /**< Compare channel. Valid only for compare event. */
} qDrvTimerXL_Event_t;

/**
 * @brief TimerXL event callback.
 *
 * @param[in] arg   User context variable.
 * @param[in] event Timer event. Overflow or compare.
 */
typedef void (*qDrvTimerXL_Cb_t)(void* arg, qDrvTimerXL_Event_t event);

/** @brief TimerXL configuration structure. */
typedef struct {
    qDrvTimerXL_CountMode_t countMode; /**< Timer counting mode. Prescaled from system
                                           clock or external.
                                        */
    UInt8 prescaler;                   /**< Prescaler divider (0-15). Clock for counter
                                            is prescaled by 2^(prescaler).
                                        */
} qDrvTimerXL_Config_t;

/** @brief TimerXL internal control block placeholder. */
typedef struct {
    UInt8 data[10];
} __attribute__((aligned(4))) qDrvTimerXL_Internal_t;

/** @brief TimerXL instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi;    /**< Quic API pointer implementation. */
#endif                               /* REG_QUIC_PRESENT */
    qDrvTimerXL_Internal_t internal; /**< Internal data buffer. Should not be modified by the user. */
} qDrvTimerXL_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Default config for TimerXL instance. */
#define Q_DRV_TIMERXL_CONFIG_DEFAULT()                                                                                 \
    {                                                                                                                  \
        .countMode = qDrvTimerXL_CountModePrescaled, /* Count mode. */                                                 \
            .prescaler = 0                           /* Prescaler divider. */                                          \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initialize the TimerXL instance.
 *
 * TimerXL instance must be initialized after allocation.
 *
 * @param[in] pInstance   Timer instance pointer. Must be allocated first.
 * @param[in] pConfig     Configuration structure.
 * @param[in] callback    Timer wrap callback function.
 * @param[in] arg         User argument passed to the callback function.
 * @param[in] irqPriority ARM Interrupt priority.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is already initialized.
 * @retval Q_ERR_INVPARAM Incorrect configuration.
 */
qResult_t qDrvTimerXL_Init(qDrvTimerXL_t* const pInstance, const qDrvTimerXL_Config_t* const pConfig,
                           const qDrvTimerXL_Cb_t callback, void* arg, UInt32 irqPriority);

/**
 * @brief Deinitialize the TimerXL instance.
 *
 * @param[in] pInstance Timer instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvTimerXL_Deinit(qDrvTimerXL_t* const pInstance);

/**
 * @brief Check if the TimerXL instance is initialized.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @returns True if the instance is initialized, false otherwise.
 */
Bool qDrvTimerXL_InitCheck(qDrvTimerXL_t* const pInstance);

/**
 * @brief Allocate TimerXL instance.
 *
 * @param[in] pInstance Not initialized TimerXL instance pointer.
 * @param[in] mode      Desired timer type to allocate. 16-bit or 32-bit mode.
 *
 * @retval Q_OK              Success.
 * @retval Q_ERR_OUTOFMEMORY No timers of a given type left to allocate.
 */
qResult_t qDrvTimerXL_Alloc(qDrvTimerXL_t* const pInstance, qDrvTimerXL_Mode_t mode);

/**
 * @brief Disable and free the previously allocated TimerXL instance.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_Free(qDrvTimerXL_t* const pInstance);

/**
 * @brief Start the timer.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_Start(const qDrvTimerXL_t* const pInstance);

/**
 * @brief Stop the timer.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_Stop(const qDrvTimerXL_t* const pInstance);

/**
 * @brief Check if the given timer is currently running.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @return True if running, false otherwise.
 */
Bool qDrvTimerXL_RunningCheck(const qDrvTimerXL_t* const pInstance);

/**
 * @brief Reset the timer current counter and prescaler value.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_Reset(const qDrvTimerXL_t* const pInstance);

/**
 * @brief Preset the counter value to the value set by compare channel 0 (CC0).
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_Preset(const qDrvTimerXL_t* const pInstance);

/**
 * @brief Read the current counter value.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @returns Current counter value.
 */
UInt32 qDrvTimerXL_CounterGet(const qDrvTimerXL_t* const pInstance);

/**
 * @brief Change the prescaler divider value.
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] prescaler New prescaler value.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_PrescalerSet(const qDrvTimerXL_t* const pInstance, UInt8 prescaler);

/**
 * @brief Capture the current counter value and store it in CC channel value.
 *        The captured value can be read with @p qDrvTimerXL_CCGet.
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] cc        Compare/capture channel number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      CC number is incorrect.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_Capture(const qDrvTimerXL_t* const pInstance, UInt8 cc);

/**
 * @brief Set the value for compare channel
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] cc        Compare/capture channel number.
 * @param[in] value     New CC value.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      CC number is incorrect.
 * @retval Q_ERR_OUTOFRANGE    Value is out of range.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_CCSet(const qDrvTimerXL_t* const pInstance, UInt8 cc, UInt32 value);

/**
 * @brief Read the value for compare channel
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] cc        Compare/capture channel number.
 *
 * @returns Current compare channel value or 0 if channel number is incorrect.
 */
UInt32 qDrvTimerXL_CCGet(const qDrvTimerXL_t* const pInstance, UInt8 cc);

/**
 * @brief Enable or disable single shot mode.
 *        When enabled, the compare will operate in oneshot mode.
 *        This means that an event will be generated on the first time the timer matches
 *        the compare level, after it was armed. To generate another event,
 *        CC value must be set again.
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] cc        Compare/capture channel number.
 * @param[in] enable    Enable or disable single shot mode.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      CC number is incorrect.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_CCSingleShotSet(const qDrvTimerXL_t* const pInstance, UInt8 cc, Bool enable);

/**
 * @brief Enable or disable clearing the counter value on compare event.
 *        This is equivalent to overflow event and the overflow interrupt
 *        is also generated.
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] cc        Compare/capture channel number.
 * @param[in] enable    Enable or disable clearing.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      CC number is incorrect.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_CCClrOnMatchSet(const qDrvTimerXL_t* const pInstance, UInt8 cc, Bool enable);

/**
 * @brief Enable or disable the interrupt triggered on CC match event
 *        The callback function must be set during initialization.
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] cc        Compare/capture channel number.
 * @param[in] enable    Enable or disable event interrupt.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      CC number is incorrect.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_CCIrqEnable(const qDrvTimerXL_t* const pInstance, UInt8 cc, Bool enable);

/**
 * @brief Enable or disable the interrupt triggered on counter overflow.
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] enable    Enable or disable interrupt.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvTimerXL_OverflowIrqEnable(const qDrvTimerXL_t* const pInstance, Bool enable);

/**
 * @brief Convert microseconds to timer wrap/prescaler values when using internal clock source.
 *
 * @param[in]  pInstance  TimerXL instance pointer.
 * @param[in]  used       Number of microseconds.
 * @param[out] pTicks     32-bit value where the calculated tick count will be stored.
 * @param[out] pPrescaler Variable for calculated prescaler value. If NULL,
 *                        current prescaler value will be used.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_OUTOFRANGE    Can not calculate ticks and prescaler for expected time.
 */
qResult_t qDrvTimerXL_UsecToTicksConvert(const qDrvTimerXL_t* const pInstance, UInt32 usec, UInt32* pTicks,
                                         UInt8* pPrescaler);

/**
 * @brief Convert frequency to timer wrap/prescaler values when using internal clock source.
 *
 * @param[in]  pInstance  TimerXL instance pointer.
 * @param[in]  frequency  Frequency in Hz.
 * @param[out] pTicks     32-bit value where the calculated tick count will be stored.
 * @param[out] pPrescaler Variable for calculated prescaler value. If NULL,
 *                        current prescaler value will be used.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_OUTOFRANGE    Can not calculate ticks and prescaler for expected frequency.
 */
qResult_t qDrvTimerXL_HzToTicksConvert(const qDrvTimerXL_t* const pInstance, UInt32 frequency, UInt32* pTicks,
                                       UInt8* pPrescaler);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_TIMERXL_H_
