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

#ifndef _Q_DRV_QUIC_INTERNAL_H_
#define _Q_DRV_QUIC_INTERNAL_H_

/**
 * @defgroup Q_DRV_QUIC_INTERNAL_H QUIC API
 * @{
 *
 * @brief This is a API for QUIC (Qorvo Unified InterConnect) peripheral.
 *
 * @file qApiQUIC.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegQUIC.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief Required trigger field to be added in each Quic trigger configuration structure. */
#define Q_DRV_QUIC_INTERNAL_TRIGGER_FIELD() UInt8 thisIsTriggerConfig[]

/** @brief Required action field to be added in each Quic action configuration structure. */
#define Q_DRV_QUIC_INTERNAL_ACTION_FIELD() UInt8 thisIsActionConfig[]

/** @enum qDrvQUIC_Internal_SupportedApiMask_t */
//@{
/** @brief Mask used to determine whether the instance supports the trigger API. */
#define qDrvQUIC_Internal_SupportedApiMaskTrigger BM(0)
/** @brief Mask used to determine whether the instance supports the action API. */
#define qDrvQUIC_Internal_SupportedApiMaskAction BM(1)
/** @brief Mask used to determine whether the instance supports the trigger and the action API. */
#define qDrvQUIC_Internal_SupportedApiMaskBoth                                                                         \
    (qDrvQUIC_Internal_SupportedApiMaskTrigger | qDrvQUIC_Internal_SupportedApiMaskAction)
/** @typedef qDrvQUIC_Internal_SupportedApiMask_t
 *  @brief QUIC API supported mask.
 */
typedef UInt8 qDrvQUIC_Internal_SupportedApiMask_t;
//@}

/** @brief Generic QUIC API definition. */
typedef void qDrvQUIC_Internal_t;

/**
 * @brief Generic QUIC API instance definition.
 * All peripherals that supports QUIC needs to fulfil this API.
 */
typedef struct {
    qDrvQUIC_Internal_t* quicApi; /**< Required QUIC API pointer. */
} qDrvQUIC_Internal_Instance_t;

/**
 * @brief QUIC function API for getting the instance speed.
 *
 * @param[in] pInstance Peripheral instance pointer.
 */
typedef qRegQUIC_Speed_t (*qDrvQUIC_Internal_SpeedGet_t)(qDrvQUIC_Internal_Instance_t* pInstance);

/**
 * @brief QUIC function API for configuring the trigger instance and getting the @p qRegQUIC_TriggerSrc_t.
 * If provided configuration is not valid, function should returns @p qRegQUIC_TriggerSrcNoTrigger.
 *
 * @param[in] pInstance Peripheral instance pointer.
 * @param[in] pConfig   Peripheral configuration pointer.
 */
typedef qRegQUIC_TriggerSrc_t (*qDrvQUIC_Internal_TriggerSrcGet_t)(qDrvQUIC_Internal_Instance_t* pInstance,
                                                                   void* pConfig);

/**
 * @brief QUIC function API for configuring the action instance.
 *
 * @param[in]  channel    QUIC channel to be used.
 * @param[in]  pInstance  Peripheral instance pointer.
 * @param[in]  pConfig    Peripheral configuration pointer.
 * @param[out] pActionIdx Action index pointer to be filled if peripheral has multiple actions.
 */
typedef qResult_t (*qDrvQUIC_Internal_ActionConfigSet_t)(
    qRegQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pInstance, void* pConfig, UInt32* pActionIdx);

/**
 * @brief QUIC function API called on trigger instance when @p qDrvQUIC_Disconnect() function is called.
 *
 * @param[in] channel   QUIC channel to be used.
 * @param[in] pInstance Peripheral instance pointer.
 */
typedef qResult_t (*qDrvQUIC_Internal_TriggerDisconnect_t)(qRegQUIC_Channel_t channel,
                                                           qDrvQUIC_Internal_Instance_t* pInstance);

/**
 * @brief QUIC function API called on action instance when @p qDrvQUIC_Disconnect() function is called.
 *
 * @param[in] channel   QUIC channel to be used.
 * @param[in] actionIdx QUIC action index within specified instance.
 * @param[in] pInstance Peripheral instance pointer.
 */
typedef qResult_t (*qDrvQUIC_Internal_ActionDisconnect_t)(qRegQUIC_Channel_t channel, UInt32 actionIdx,
                                                          qDrvQUIC_Internal_Instance_t* pInstance);

/** @brief QUIC trigger API. */
typedef struct {
    /* Common API. */
    qDrvQUIC_Internal_SupportedApiMask_t apiSupportedMask; /**< Supported API mask. Should be @p
                                                            qDrvQUIC_Internal_SupportedApiMaskTrigger. */
    qDrvQUIC_Internal_SpeedGet_t apiSpeedGet;              /**< Function called to get the trigger instance speed. */

    /* Trigger API. */
    qDrvQUIC_Internal_TriggerSrcGet_t apiTriggerSrcGet; /**< Function called to configure and get the trigger source of
                                                            trigger instance. */
    qDrvQUIC_Internal_TriggerDisconnect_t apiTriggerDisconnect; /**< Function called on trigger api disconnection. */
} qDrvQUIC_Internal_Trigger_t;

/** @brief QUIC action API. */
typedef struct {
    /* Common API. */
    qDrvQUIC_Internal_SupportedApiMask_t apiSupportedMask; /**< Supported API mask. Should be @p
                                                            qDrvQUIC_Internal_SupportedApiMaskAction. */
    qDrvQUIC_Internal_SpeedGet_t apiSpeedGet;              /**< Function called to get the trigger instance speed. */

    /* Action API. */
    qDrvQUIC_Internal_ActionConfigSet_t apiActionConfigSet;   /**< Function called to configure the action instance. */
    qDrvQUIC_Internal_ActionDisconnect_t apiActionDisconnect; /**< Function called on action instance disconnection. */
} qDrvQUIC_Internal_Action_t;

/** @brief QUIC trigger and action API. */
typedef struct {
    /* Common API. */
    qDrvQUIC_Internal_SupportedApiMask_t apiSupportedMask; /**< Supported API mask. Should be @p
                                                            qDrvQUIC_Internal_SupportedApiMaskBoth. */
    qDrvQUIC_Internal_SpeedGet_t apiSpeedGet;              /**< Function called to get the trigger instance speed. */

    /* Trigger API. */
    qDrvQUIC_Internal_TriggerSrcGet_t apiTriggerSrcGet; /**< Function called to configure and get the trigger source of
                                                            trigger instance. */
    qDrvQUIC_Internal_TriggerDisconnect_t apiTriggerDisconnect; /**< Function called on trigger api disconnection. */

    /* Action API. */
    qDrvQUIC_Internal_ActionConfigSet_t apiActionConfigSet;   /**< Function called to configure the action instance. */
    qDrvQUIC_Internal_ActionDisconnect_t apiActionDisconnect; /**< Function called on action instance disconnection. */
} qDrvQUIC_Internal_Both_t;

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_QUIC_INTERNAL_H_
