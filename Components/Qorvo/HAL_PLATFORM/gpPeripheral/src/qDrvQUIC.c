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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegInternal.h"
#include "qDrvQUIC.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define QUIC_API_GET_FROM_TRIGGER_INSTANCE(pInstance, apiTriggerFunctionName)                                          \
    (((qDrvQUIC_Internal_Trigger_t*)(pInstance)->quicApi)->apiSupportedMask &                                          \
     qDrvQUIC_Internal_SupportedApiMaskAction)                                                                         \
        ? (((qDrvQUIC_Internal_Both_t*)(pInstance)->quicApi)->apiTriggerFunctionName)                                  \
        : (((qDrvQUIC_Internal_Trigger_t*)(pInstance)->quicApi)->apiTriggerFunctionName)

#define QUIC_API_GET_FROM_ACTION_INSTANCE(pInstance, apiTriggerFunctionName)                                           \
    (((qDrvQUIC_Internal_Action_t*)(pInstance)->quicApi)->apiSupportedMask &                                           \
     qDrvQUIC_Internal_SupportedApiMaskTrigger)                                                                        \
        ? (((qDrvQUIC_Internal_Both_t*)(pInstance)->quicApi)->apiTriggerFunctionName)                                  \
        : (((qDrvQUIC_Internal_Action_t*)(pInstance)->quicApi)->apiTriggerFunctionName)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    qDrvQUIC_Internal_Instance_t* pTriggerInstance;
    qDrvQUIC_Internal_Instance_t* pActionInstance;
    UInt32 actionIdx;
    qDrvQUIC_AttachInstance_t* pAttachInstance;
} qDrvQUIC_Connection_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static UInt32 channelAlloc;
static qDrvQUIC_Connection_t connection[REG_QUIC_CHANNEL_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static inline Bool qDrvQUIC_ChannelAllocGet(qDrvQUIC_Channel_t channel)
{
    return BIT_TST(channelAlloc, channel);
}

static inline void qDrvQUIC_ChannelAllocSet(qDrvQUIC_Channel_t channel, Bool enable)
{
    if(enable)
    {
        BIT_SET(channelAlloc, channel);
    }
    else
    {
        BIT_CLR(channelAlloc, channel);
    }
}

static qResult_t qDrvQUIC_ActionDisconnect(qDrvQUIC_Channel_t channel, UInt32 actionIdx,
                                           qDrvQUIC_Internal_Instance_t* pActionInstance)
{
    /* Action instance can be set to NULL if channel is used only as a trigger to be aggregated. */
    qDrvQUIC_Internal_ActionDisconnect_t apiActionDisconnect =
        QUIC_API_GET_FROM_ACTION_INSTANCE(pActionInstance, apiActionDisconnect);

    if(apiActionDisconnect == NULL)
    {
        return Q_ERR_NOTSUPPORTED;
    }

    return apiActionDisconnect(channel, actionIdx, pActionInstance);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qDrvQUIC_Channel_t qDrvQUIC_Alloc(Bool aggregation)
{
#if REG_QUIC_AGGREGATION_CHANNEL_COUNT
    UInt8 channelStartCount = aggregation ? 0UL : REG_QUIC_AGGREGATION_CHANNEL_COUNT;
    UInt8 channelStopCount = aggregation ? REG_QUIC_AGGREGATION_CHANNEL_COUNT : REG_QUIC_CHANNEL_COUNT;
#else
    if(aggregation)
    {
        return Q_NOCHANNEL;
    }
    UInt8 channelStartCount = 0UL;
    UInt8 channelStopCount = REG_QUIC_CHANNEL_COUNT;
#endif

    for(qDrvQUIC_Channel_t channel = channelStartCount; channel < channelStopCount; channel++)
    {
        if(!qDrvQUIC_ChannelAllocGet(channel))
        {
            qDrvQUIC_ChannelAllocSet(channel, true);
            return channel;
        }
    }
    return Q_NOCHANNEL;
}

qResult_t qDrvQUIC_Free(qDrvQUIC_Channel_t channel)
{
    Q_ASSERT(channel < REG_QUIC_CHANNEL_COUNT);

    if(!qDrvQUIC_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(qDrvQUIC_ConnectCheck(channel))
    {
        /* Perform a disconnection if connection exists. */
        qResult_t result = qDrvQUIC_Disconnect(channel);
        if(result != Q_OK)
        {
            return result;
        }
    }

    qDrvQUIC_ChannelAllocSet(channel, false);

    return Q_OK;
}

qResult_t qDrvQUIC_EdgeSet(qDrvQUIC_Channel_t channel, qDrvQUIC_Edge_t edge)
{
    Q_ASSERT(channel < REG_QUIC_CHANNEL_COUNT);

    if(!qDrvQUIC_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    qRegQUIC_EdgeSet((qRegQUIC_Channel_t)channel, (qRegQUIC_Edge_t)edge);

    return Q_OK;
}

qResult_t qDrvQUIC_TriggerConnectUnsafe(qDrvQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pTriggerInstance,
                                        void* pTriggerConfig)
{
    Q_ASSERT(channel < REG_QUIC_CHANNEL_COUNT);
    Q_ASSERT(pTriggerInstance);
    Q_ASSERT(pTriggerInstance->quicApi);

    if(!qDrvQUIC_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(qDrvQUIC_ConnectCheck(channel))
    {
        /* Connection already exists. */
        return Q_ERR_INVSTATE;
    }

    qDrvQUIC_Internal_Trigger_t* triggerApi = (qDrvQUIC_Internal_Trigger_t*)pTriggerInstance->quicApi;

    if(!(triggerApi->apiSupportedMask & qDrvQUIC_Internal_SupportedApiMaskTrigger))
    {
        return Q_ERR_NOTSUPPORTED;
    }

    /* Get the source speed getter function. */
    qDrvQUIC_Internal_SpeedGet_t triggerApiSpeedGet = QUIC_API_GET_FROM_TRIGGER_INSTANCE(pTriggerInstance, apiSpeedGet);

    if(triggerApiSpeedGet == NULL)
    {
        /* Source speed getter is a required API. */
        return Q_ERR_INVPARAM;
    }

    /* Get the trigger select getter function. */
    qDrvQUIC_Internal_TriggerSrcGet_t apiTriggerSrcGet =
        QUIC_API_GET_FROM_TRIGGER_INSTANCE(pTriggerInstance, apiTriggerSrcGet);

    if(apiTriggerSrcGet == NULL)
    {
        /* Source trigger getter is a required API. */
        return Q_ERR_INVPARAM;
    }

    /* Set source peripheral speed. */
    qRegQUIC_SrcSpeedSet((qRegQUIC_Channel_t)channel, triggerApiSpeedGet((void*)pTriggerInstance));

    qRegQUIC_TriggerSrc_t triggerSrc = apiTriggerSrcGet((void*)pTriggerInstance, pTriggerConfig);
    if(triggerSrc == qRegQUIC_TriggerSrcNoTrigger)
    {
        /* Unable to get the valid trigger source for provided configuration. */
        return Q_ERR_INVPARAM;
    }

    /* Connect the channel. Enable trigger selection. */
    qRegQUIC_TriggerSrcSet((qRegQUIC_Channel_t)channel, triggerSrc);

    connection[channel].pTriggerInstance = pTriggerInstance;
    connection[channel].pActionInstance = NULL;
    connection[channel].pAttachInstance = NULL;

    return Q_OK;
}

qResult_t qDrvQUIC_ConnectUnsafe(qDrvQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pTriggerInstance,
                                 void* pTriggerConfig, qDrvQUIC_Internal_Instance_t* pActionInstance,
                                 void* pActionConfig)
{
    Q_ASSERT(channel < REG_QUIC_CHANNEL_COUNT);
    Q_ASSERT(pTriggerInstance);
    Q_ASSERT(pActionInstance);
    Q_ASSERT(pTriggerInstance->quicApi);
    Q_ASSERT(pActionInstance->quicApi);

    qResult_t result = qDrvQUIC_TriggerConnectUnsafe(channel, pTriggerInstance, pTriggerConfig);
    if(result != Q_OK)
    {
        return result;
    }

    qDrvQUIC_Internal_Action_t* actionApi = (qDrvQUIC_Internal_Action_t*)pActionInstance->quicApi;
    if(!(actionApi->apiSupportedMask & qDrvQUIC_Internal_SupportedApiMaskAction))
    {
        qRegQUIC_TriggerSrcSet((qRegQUIC_Channel_t)channel, qRegQUIC_TriggerSrcNoTrigger);
        connection[channel].pTriggerInstance = NULL;
        return Q_ERR_NOTSUPPORTED;
    }

    /* Get the destination speed getter function. */
    qDrvQUIC_Internal_SpeedGet_t actionApiSpeedGet = QUIC_API_GET_FROM_ACTION_INSTANCE(pActionInstance, apiSpeedGet);
    if(actionApiSpeedGet == NULL)
    {
        /* Destination speed getter is a required API. */
        qRegQUIC_TriggerSrcSet((qRegQUIC_Channel_t)channel, qRegQUIC_TriggerSrcNoTrigger);
        connection[channel].pTriggerInstance = NULL;
        return Q_ERR_INVPARAM;
    }

    /* Set destination peripheral speed. */
    qRegQUIC_DstSpeedSet((qRegQUIC_Channel_t)channel, actionApiSpeedGet((void*)pActionInstance));

    /* Get the destination config setter function. */
    qDrvQUIC_Internal_ActionConfigSet_t apiActionConfigSet =
        QUIC_API_GET_FROM_ACTION_INSTANCE(pActionInstance, apiActionConfigSet);

    if(apiActionConfigSet != NULL)
    {
        result = apiActionConfigSet(channel, (void*)pActionInstance, pActionConfig, &connection[channel].actionIdx);
        if(result != Q_OK)
        {
            qRegQUIC_TriggerSrcSet((qRegQUIC_Channel_t)channel, qRegQUIC_TriggerSrcNoTrigger);
            connection[channel].pTriggerInstance = NULL;
            return result;
        }
    }

    connection[channel].pActionInstance = pActionInstance;

    return Q_OK;
}

qResult_t qDrvQUIC_Disconnect(qDrvQUIC_Channel_t channel)
{
    Q_ASSERT(channel < REG_QUIC_CHANNEL_COUNT);

    if(!qDrvQUIC_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(!qDrvQUIC_ConnectCheck(channel))
    {
        /* Connection does not exist. */
        return Q_ERR_INVSTATE;
    }

    qResult_t result = Q_OK;

    /* Disconnect the channel. */
    qRegQUIC_TriggerSrcSet((qRegQUIC_Channel_t)channel, qRegQUIC_TriggerSrcNoTrigger);

    qDrvQUIC_Internal_Instance_t* pTriggerInstance = connection[channel].pTriggerInstance;
    qDrvQUIC_Internal_TriggerDisconnect_t apiTriggerDisconnect =
        QUIC_API_GET_FROM_TRIGGER_INSTANCE(pTriggerInstance, apiTriggerDisconnect);

    if(apiTriggerDisconnect)
    {
        result = apiTriggerDisconnect(channel, pTriggerInstance);
        if(result != Q_OK)
        {
            return result;
        }
    }

    qDrvQUIC_Internal_Instance_t* pActionInstance = connection[channel].pActionInstance;
    if(pActionInstance)
    {
        /* Action instance can be set to NULL if channel is used only as a trigger to be aggregated. */
        result = qDrvQUIC_ActionDisconnect(channel, connection[channel].actionIdx, pActionInstance);
        if(result != Q_OK)
        {
            return result;
        }
        connection[channel].pActionInstance = NULL;
    }

    qDrvQUIC_AttachInstance_t* pAttach = connection[channel].pAttachInstance;
    while(pAttach)
    {
        qDrvQUIC_AttachInstance_t* pNextAttach = pAttach->next;
        (void)qDrvQUIC_Detach(pAttach);
        pAttach = pNextAttach;
    }

    connection[channel].pAttachInstance = NULL;
    connection[channel].pTriggerInstance = NULL;

    return Q_OK;
}

Bool qDrvQUIC_ConnectCheck(qDrvQUIC_Channel_t channel)
{
    Q_ASSERT(channel < REG_QUIC_CHANNEL_COUNT);

    return qRegQUIC_TriggerSrcGet((qRegQUIC_Channel_t)channel) != qRegQUIC_TriggerSrcNoTrigger;
}

qResult_t qDrvQUIC_AttachUnsafe(qDrvQUIC_AttachInstance_t* attach, qDrvQUIC_Channel_t channel,
                                qDrvQUIC_Internal_Instance_t* pActionInstance, void* pActionConfig)
{
    Q_ASSERT(attach);
    Q_ASSERT(channel < REG_QUIC_CHANNEL_COUNT);
    Q_ASSERT(pActionInstance);
    Q_ASSERT(pActionInstance->quicApi);

    if(!qDrvQUIC_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(!qDrvQUIC_ConnectCheck(channel))
    {
        /* Connection does not exist. */
        return Q_ERR_INVSTATE;
    }

    /* Get the destination speed getter function. */
    qDrvQUIC_Internal_SpeedGet_t actionApiSpeedGet = QUIC_API_GET_FROM_ACTION_INSTANCE(pActionInstance, apiSpeedGet);
    if(actionApiSpeedGet == NULL)
    {
        /* Destination speed getter is a required API. */
        return Q_ERR_INVPARAM;
    }

    /* Get the source channel speed. */
    qRegQUIC_Speed_t speed = qRegQUIC_SrcSpeedGet(channel);

    if(speed != actionApiSpeedGet((void*)pActionInstance))
    {
        /* Speeds do not match. */
        return Q_ERR_NOTSUPPORTED;
    }

    /* Get the destination config setter function. */
    qDrvQUIC_Internal_ActionConfigSet_t apiActionConfigSet =
        QUIC_API_GET_FROM_ACTION_INSTANCE(pActionInstance, apiActionConfigSet);

    if(apiActionConfigSet == NULL)
    {
        /* Action config setter is a required API. */
        return Q_ERR_INVPARAM;
    }

    qResult_t result = apiActionConfigSet(channel, (void*)pActionInstance, pActionConfig, &attach->actionIdx);
    if(result != Q_OK)
    {
        return result;
    }

    attach->channel = channel;
    attach->pActionInstance = pActionInstance;
    attach->prev = NULL;
    attach->next = NULL;

    qDrvQUIC_AttachInstance_t* pConnAttach = connection[channel].pAttachInstance;

    if(pConnAttach)
    {
        while(pConnAttach->next)
        {
            pConnAttach = pConnAttach->next;
        }
        pConnAttach->next = attach;
        attach->prev = pConnAttach;
    }
    else
    {
        connection[channel].pAttachInstance = attach;
    }

    return Q_OK;
}

qResult_t qDrvQUIC_Detach(qDrvQUIC_AttachInstance_t* pAttach)
{
    Q_ASSERT(pAttach);
    Q_ASSERT(pAttach->pActionInstance);
    Q_ASSERT(pAttach->channel < REG_QUIC_CHANNEL_COUNT);

    if(!qDrvQUIC_ChannelAllocGet(pAttach->channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(!qDrvQUIC_ConnectCheck(pAttach->channel))
    {
        /* Connection does not exist. */
        return Q_ERR_INVSTATE;
    }

    qResult_t result = qDrvQUIC_ActionDisconnect(pAttach->channel, pAttach->actionIdx, pAttach->pActionInstance);
    if(result != Q_OK)
    {
        return result;
    }

    qDrvQUIC_AttachInstance_t* pPrevAttach = pAttach->prev;
    qDrvQUIC_AttachInstance_t* pNextAttach = pAttach->next;

    if(pPrevAttach)
    {
        pPrevAttach->next = pNextAttach;
    }
    else
    {
        connection[pAttach->channel].pAttachInstance = pNextAttach;
    }

    if(pNextAttach)
    {
        pNextAttach->prev = pPrevAttach;
    }
    else
    {
        connection[pAttach->channel].pAttachInstance = pPrevAttach;
    }

    return Q_OK;
}

#if REG_QUIC_AGGREGATION_CHANNEL_COUNT
qResult_t qDrvQUIC_ChannelAggregate(qDrvQUIC_Channel_t parentChannel, qDrvQUIC_Channel_t childChannel)
{
    Q_ASSERT(parentChannel < REG_QUIC_AGGREGATION_CHANNEL_COUNT);
    Q_ASSERT(childChannel < REG_QUIC_CHANNEL_COUNT);

    if(!qDrvQUIC_ChannelAllocGet(parentChannel) || !qDrvQUIC_ChannelAllocGet(childChannel))
    {
        /* Channels are not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(!qDrvQUIC_ConnectCheck(parentChannel) || !qDrvQUIC_ConnectCheck(childChannel))
    {
        /* Channels are not connected. */
        return Q_ERR_INVSTATE;
    }

    if(parentChannel == childChannel)
    {
        /* Unable to aggregate parent channel to itself. */
        return Q_ERR_INVPARAM;
    }

    qDrvQUIC_Internal_Instance_t* pParentInstance = connection[parentChannel].pTriggerInstance;
    qDrvQUIC_Internal_Instance_t* pChildInstance = connection[childChannel].pTriggerInstance;

    /* Get speed getter functions. */
    qDrvQUIC_Internal_SpeedGet_t parentApiSpeedGet = QUIC_API_GET_FROM_TRIGGER_INSTANCE(pParentInstance, apiSpeedGet);
    qDrvQUIC_Internal_SpeedGet_t childApiSpeedGet = QUIC_API_GET_FROM_TRIGGER_INSTANCE(pChildInstance, apiSpeedGet);

    if((parentApiSpeedGet == NULL) || (childApiSpeedGet == NULL))
    {
        /* Speed getters are a required API. */
        return Q_ERR_INVPARAM;
    }

    if(parentApiSpeedGet(pParentInstance) != childApiSpeedGet(pChildInstance))
    {
        /* Instances' speed do not match. */
        return Q_ERR_NOTSUPPORTED;
    }

    qRegQUIC_AggregationMaskSet(parentChannel, (1UL << childChannel));

    return Q_OK;
}

qResult_t qDrvQUIC_ChannelDeaggregate(qDrvQUIC_Channel_t parentChannel, qDrvQUIC_Channel_t childChannel)
{
    Q_ASSERT(parentChannel < REG_QUIC_AGGREGATION_CHANNEL_COUNT);
    Q_ASSERT(childChannel < REG_QUIC_CHANNEL_COUNT);

    if(!qDrvQUIC_ChannelAllocGet(parentChannel))
    {
        /* Parent channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(parentChannel == childChannel)
    {
        /* Unable to deaggregate parent channel from itself. */
        return Q_ERR_INVPARAM;
    }

    if(!(qRegQUIC_AggregationMaskGet(parentChannel) & (1UL << childChannel)))
    {
        /* Aggregation does not exist. */
        return Q_ERR_GENERAL;
    }

    qRegQUIC_AggregationMaskClr(parentChannel, (1UL << childChannel));

    return Q_OK;
}

Bool qDrvQUIC_ChannelAggregateCheck(qDrvQUIC_Channel_t parentChannel, qDrvQUIC_Channel_t childChannel)
{
    Q_ASSERT(parentChannel < REG_QUIC_AGGREGATION_CHANNEL_COUNT);
    Q_ASSERT(childChannel < REG_QUIC_CHANNEL_COUNT);

    return (qRegQUIC_AggregationMaskGet(parentChannel) & (1UL << childChannel)) ? true : false;
}
#endif
