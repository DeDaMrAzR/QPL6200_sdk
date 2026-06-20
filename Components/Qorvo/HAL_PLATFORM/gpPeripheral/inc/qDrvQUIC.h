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

#ifndef _Q_DRV_QUIC_H_
#define _Q_DRV_QUIC_H_

/**
 * @defgroup Q_DRV_QUIC QUIC driver
 * @{
 *
 * @brief This is a driver for QUIC (Qorvo Unified InterConnect) peripheral.
 *
 * @file qDrvQUIC.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegQUIC.h"
#include "qDrvCommon.h"
#include "qDrvQUICInternal.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(Q_STATIC_ASSERT)
/** @brief Macro for checking whether the peripheral instance supports a valid QUIC API. */
#define Q_DRV_QUIC_STATIC_API_CHECK(pInstance)                                                                         \
    (void)((pInstance)->quicApi); /* Build time checker for quicApi pointer. */                                        \
    Q_STATIC_ASSERT(offsetof(typeof(*(pInstance)), quicApi) == offsetof(qDrvQUIC_Internal_Instance_t, quicApi))
#else
/** @brief Macro for checking whether the peripheral instance supports a valid QUIC API. */
#define Q_DRV_QUIC_STATIC_API_CHECK(pInstance)                                                                         \
    (void)((pInstance)->quicApi); /* Build time checker for quicApi pointer. */
#endif

/** @brief Macro for checking whether the configuration structure has @p thisIsTriggerConfig member, what indicated the
 * valid trigger configuration structure. */
#define Q_DRV_QUIC_STATIC_TRIGGER_CONFIG_CHECK(pConfig)                                                                \
    (void)((pConfig)->thisIsTriggerConfig); /* Build time checker for thisIsTriggerConfig. */

/** @brief Macro for checking whether the configuration structure has @p thisIsActionConfig member, what indicated the
 * valid action configuration structure. */
#define Q_DRV_QUIC_STATIC_ACTION_CONFIG_CHECK(pConfig)                                                                 \
    (void)((pConfig)->thisIsActionConfig); /* Build time checker for thisIsActionConfig. */

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief QUIC channel definition. */
typedef qRegQUIC_Channel_t qDrvQUIC_Channel_t;

/** @brief QUIC edge definition. */
typedef qRegQUIC_Edge_t qDrvQUIC_Edge_t;

/** @brief QUIC attach instance structure. */
typedef struct qDrvQUIC_AttachInstance {
    qDrvQUIC_Channel_t channel;                    /**< Used channel. */
    qDrvQUIC_Internal_Instance_t* pActionInstance; /**< Driver instance pointer used as action instance. */
    UInt32 actionIdx;                              /**< Action index within specified instance. */
    struct qDrvQUIC_AttachInstance* prev;          /**< Previous attach instance pointer. */
    struct qDrvQUIC_AttachInstance* next;          /**< Next attach instance pointer. */
} qDrvQUIC_AttachInstance_t;

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Allocate QUIC channel.
 *
 * @param[in] aggregation True if the requested channel requires the aggregation support, false otherwise.
 *
 * @retval Q_NOCHANNEL If there is no available QUIC channel.
 * @retval Channel     If channel is available and properly allocated.
 */
qDrvQUIC_Channel_t qDrvQUIC_Alloc(Bool aggregation);

/**
 * @brief Free QUIC channel.
 *
 * @param[in] channel Channel to be freed.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Channel is not allocated.
 */
qResult_t qDrvQUIC_Free(qDrvQUIC_Channel_t channel);

/**
 * @brief Set the QUIC edge for the provided channel.
 *
 * @param[in] channel Used channel.
 * @param[in] edge    QUIC edge selection.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Channel is not allocated.
 */
qResult_t qDrvQUIC_EdgeSet(qDrvQUIC_Channel_t channel, qDrvQUIC_Edge_t edge);

/**
 * @brief Connect QUIC API trigger instance to the channel.
 *
 * @note Trigger instance should has a proper QUIC API.
 *
 * @param[in] channel          Used channel.
 * @param[in] pTriggerInstance Driver instance pointer used as trigger instance - with valid QUIC API.
 * @param[in] pTriggerConfig   Trigger configuration structure pointer.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVSTATE     Channel is not allocated or Connection already exists.
 * @retval Q_ERR_NOTSUPPORTED Trigger instance does not support trigger API.
 * @retval Q_ERR_INVPARAM     Missing required API functions.
 */
qResult_t qDrvQUIC_TriggerConnectUnsafe(qDrvQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pTriggerInstance,
                                        void* pTriggerConfig);

/**
 * @brief Macro used for safe connect QUIC API trigger instance to the channel with provided valid configuration.
 * Should be used instead of @p qDrvQUIC_TriggerConnectUnsafe() function.
 *
 * See @p qDrvQUIC_TriggerConnectUnsafe() function for argument descriptions.
 */
#define qDrvQUIC_TriggerConnect(channel, pTriggerInstance, pTriggerConfig)                                             \
    qDrvQUIC_TriggerConnectUnsafe((channel), ((qDrvQUIC_Internal_Instance_t*)(pTriggerInstance)), (pTriggerConfig));   \
    Q_DRV_QUIC_STATIC_API_CHECK((pTriggerInstance));                                                                   \
    Q_DRV_QUIC_STATIC_TRIGGER_CONFIG_CHECK((pTriggerConfig))

/**
 * @brief Connect two QUIC API instances together with provided valid configurations.
 *
 * @note Trigger and action instance should have a proper QUIC API.
 *
 * @param[in] channel          Used channel.
 * @param[in] pTriggerInstance Driver instance pointer used as trigger instance - with valid QUIC API.
 * @param[in] pTriggerConfig   Trigger configuration structure pointer.
 * @param[in] pActionInstance  Driver instance pointer used as action instance - with valid QUIC API.
 * @param[in] pActionConfig    Action configuration structure pointer.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVSTATE     Channel is not allocated or Connection already exists.
 * @retval Q_ERR_NOTSUPPORTED Trigger instance does not support trigger API or action instance does not support action
 *                            API.
 * @retval Q_ERR_INVPARAM     Missing required API functions.
 */
qResult_t qDrvQUIC_ConnectUnsafe(qDrvQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pTriggerInstance,
                                 void* pTriggerConfig, qDrvQUIC_Internal_Instance_t* pActionInstance,
                                 void* pActionConfig);

/**
 * @brief Macro used for safe connect two QUIC API instances together with provided valid configurations.
 * Should be used instead of @p qDrvQUIC_ConnectUnsafe() function.
 *
 * See @p qDrvQUIC_ConnectUnsafe() function for argument descriptions.
 */
#define qDrvQUIC_Connect(channel, pTriggerInstance, pTriggerConfig, pActionInstance, pActionConfig)                    \
    qDrvQUIC_ConnectUnsafe((channel), ((qDrvQUIC_Internal_Instance_t*)(pTriggerInstance)), (pTriggerConfig),           \
                           ((qDrvQUIC_Internal_Instance_t*)(pActionInstance)), (pActionConfig));                       \
    Q_DRV_QUIC_STATIC_API_CHECK((pTriggerInstance));                                                                   \
    Q_DRV_QUIC_STATIC_TRIGGER_CONFIG_CHECK((pTriggerConfig));                                                          \
    Q_DRV_QUIC_STATIC_API_CHECK((pActionInstance));                                                                    \
    Q_DRV_QUIC_STATIC_ACTION_CONFIG_CHECK((pActionConfig))

/**
 * @brief Disconnect QUIC channel.
 *
 * @param[in] channel Used channel.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvQUIC_Disconnect(qDrvQUIC_Channel_t channel);

/**
 * @brief Check whether channel is already properly connected.
 *
 * @param[in] channel Used channel.
 *
 * @retval True if the channel is properly connected, false otherwise.
 */
Bool qDrvQUIC_ConnectCheck(qDrvQUIC_Channel_t channel);

/**
 * @brief Attach QUIC action to already connected channel.
 *
 * @param[in] attach          Attach instance pointer.
 * @param[in] channel         Used channel.
 * @param[in] pActionInstance Driver instance pointer used as action instance - with valid QUIC API.
 * @param[in] pActionConfig   Action configuration structure pointer.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVSTATE     Channel is not connected.
 * @retval Q_ERR_NOTSUPPORTED Action instance does not support action API.
 * @retval Q_ERR_INVPARAM     Missing required API functions.
 */
qResult_t qDrvQUIC_AttachUnsafe(qDrvQUIC_AttachInstance_t* attach, qDrvQUIC_Channel_t channel,
                                qDrvQUIC_Internal_Instance_t* pActionInstance, void* pActionConfig);

/**
 * @brief Macro used for safe attach QUIC action to already connected channel.
 * Should be used instead of @p qDrvQUIC_AttachUnsafe() function.
 *
 * See @p qDrvQUIC_AttachUnsafe() function for argument descriptions.
 */
#define qDrvQUIC_Attach(attach, channel, pActionInstance, pActionConfig)                                               \
    qDrvQUIC_AttachUnsafe(attach, (channel), ((qDrvQUIC_Internal_Instance_t*)(pActionInstance)), (pActionConfig));     \
    Q_DRV_QUIC_STATIC_API_CHECK((pActionInstance));                                                                    \
    Q_DRV_QUIC_STATIC_ACTION_CONFIG_CHECK((pActionConfig))

/**
 * @brief Detach QUIC action from already connected channel.
 *
 * @param[in] attach Attach instance pointer.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVSTATE     Channel is not allocated or connection does not exist.
 * @retval Q_ERR_NOTSUPPORTED Action instance does not support action API.
 */
qResult_t qDrvQUIC_Detach(qDrvQUIC_AttachInstance_t* pAttach);

/**
 * @brief Aggregate child QUIC trigger channel to parent trigger channel.
 *
 * @note Parent channel needs to have a support for the aggregation.
 *
 * @param[in] parentChannel Parent channel with supported aggregation.
 * @param[in] childChannel  Child channel to be aggregated.
 *
 * @retval Q_OK               Success.
 * @retval Q_ERR_INVSTATE     Channels are not allocated or channels are not connected.
 * @retval Q_ERR_INVPARAM     Parent channel can not be the same as child channel
 *                            or missing required API functions.
 * @retval Q_ERR_NOTSUPPORTED Parent and child instances speeds do not match.
 */
qResult_t qDrvQUIC_ChannelAggregate(qDrvQUIC_Channel_t parentChannel, qDrvQUIC_Channel_t childChannel);

/**
 * @brief Deaggregate child QUIC trigger channel from parent trigger channel.
 *
 * @param[in] parentChannel Parent channel with supported aggregation.
 * @param[in] childChannel  Child channel to be aggregated.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Parent channel is not allocated.
 * @retval Q_ERR_GENERAL  Aggregation does not exist.
 */
qResult_t qDrvQUIC_ChannelDeaggregate(qDrvQUIC_Channel_t parentChannel, qDrvQUIC_Channel_t childChannel);

/**
 * @brief Check if channels are aggregated.
 *
 * @param[in] parentChannel Parent channel with supported aggregation.
 * @param[in] childChannel  Child channel to be aggregated.
 *
 * @retval True if channels are aggregated, false otherwise.
 */
Bool qDrvQUIC_ChannelAggregateCheck(qDrvQUIC_Channel_t parentChannel, qDrvQUIC_Channel_t childChannel);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_QUIC_H_
