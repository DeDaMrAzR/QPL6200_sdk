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

#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qDrvDMADCB.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief DMA DCB channel control block data. */
typedef struct {
    qDrvDMADCB_CbComplete_t callback; /**< Channel callback function pointer. */
    void* pSrcBuf;                    /**< Source buffer pointer. */
    void* pDestBuf;                   /**< Destination buffer pointer. */
    void* arg;                        /**< User-supplied argument. */
} qDrvDMADCB_ChannelControlBlock_t;

/** @brief DMA DCB channel control block data. */
typedef struct {
    qDrvDMADCB_ChannelControlBlock_t channelControlBlock[REG_DMA_DCB_CHANNEL_COUNT]; /**< Channel control blocks. */
    qDrvDMADCB_CbError_t cbCpyErr; /**< Copy error callback function pointer. */
    UInt8 channelAlloc;            /**< Bitmask of allocated channels. */
    UInt8 channelInit;             /**< Bitmask of initialized channels. */
    Bool initialized;              /**< Initialization state. */
} qDrvDMADCB_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/* Internal DMA DCB control block data. */
static qDrvDMADCB_ControlBlock_t dmaControlBlock;

/* Check the size of the masks. */
Q_STATIC_ASSERT(sizeof(dmaControlBlock.channelAlloc) * 8 >= REG_DMA_DCB_CHANNEL_COUNT);
Q_STATIC_ASSERT(sizeof(dmaControlBlock.channelInit) * 8 >= REG_DMA_DCB_CHANNEL_COUNT);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static inline qDrvDMADCB_ChannelControlBlock_t* qDrvDMADCB_ChannelControlBlockGet(UInt8 channel)
{
    return &dmaControlBlock.channelControlBlock[channel];
}

static inline Bool qDrvDMADCB_ChannelAllocGet(qDrvDMADCB_Channel_t channel)
{
    return BIT_TST(dmaControlBlock.channelAlloc, channel);
}

static inline void qDrvDMADCB_ChannelAllocSet(qDrvDMADCB_Channel_t channel, Bool enable)
{
    if(enable)
    {
        BIT_SET(dmaControlBlock.channelAlloc, channel);
    }
    else
    {
        BIT_CLR(dmaControlBlock.channelAlloc, channel);
    }
}

static inline Bool qDrvDMADCB_ChannelInitGet(qDrvDMADCB_Channel_t channel)
{
    return BIT_TST(dmaControlBlock.channelInit, channel);
}

static inline void qDrvDMADCB_ChannelInitSet(qDrvDMADCB_Channel_t channel, Bool enable)
{
    if(enable)
    {
        BIT_SET(dmaControlBlock.channelInit, channel);
    }
    else
    {
        BIT_CLR(dmaControlBlock.channelInit, channel);
    }
}

static UInt8 qDrvDMADCB_WordModeToBytes(qRegDMADCB_WordMode_t wordMode)
{
    switch(wordMode)
    {
        case qRegDMADCB_WordMode8Bit:
            return 1;
        case qRegDMADCB_WordMode16Bit:
            return 2;
        case qRegDMADCB_WordMode32Bit:
            return 4;
        default:
            break;
    }

    return 0;
}

static inline void qDrvDMADCB_ChannelEventIRQEnableSet(qDrvDMADCB_Channel_t channel, Bool enable)
{
    UInt32 mask = 1UL << channel;
    if(enable)
    {
        qRegIntCtrl_EventIRQEnable(DMA_DCB_IRQn, mask, 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(DMA_DCB_IRQn, mask, 0);
    }
}

static qResult_t qDrvDMADCB_Advance(qDrvDMADCB_Channel_t channel, UInt16 wordCount, Bool src)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);

    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvDMADCB_ChannelAllocGet(channel) || !qDrvDMADCB_ChannelInitGet(channel) ||
       qRegDMADCB_SrcAddrGet(channel) == 0 || qRegDMADCB_DestAddrGet(channel) == 0)
    {
        return Q_ERR_INVPARAM;
    }

    HAL_DISABLE_GLOBAL_INT();
    Bool trigger = qRegDMADCB_TriggerGet(channel);

    if(trigger && !qRegDMADCB_IRQBufferCompleteGet(channel))
    {
        /* Transfer in progress. */
        HAL_ENABLE_GLOBAL_INT();

        return Q_ERR_BUSY;
    }

    qRegDMADCB_TriggerSet(channel, false);
    UInt32 buffSize, ptrOffset;
    Bool wrap;

    if(src)
    {
        buffSize = qRegDMADCB_SrcBufferSizeGet(channel) + 1UL;
        ptrOffset = qRegDMADCB_SrcWritePtrGet(channel);
        wrap = qRegDMADCB_SrcWritePtrWrapGet(channel);
    }
    else
    {
        buffSize = qRegDMADCB_DestBufferSizeGet(channel) + 1UL;
        ptrOffset = qRegDMADCB_DestReadPtrGet(channel);
        wrap = qRegDMADCB_DestReadPtrWrapGet(channel);
    }

    if(wordCount > buffSize)
    {
        /* Overflow. */
        HAL_ENABLE_GLOBAL_INT();

        return Q_ERR_OUTOFMEMORY;
    }

    ptrOffset += wordCount;

    if((ptrOffset / buffSize) > 0)
    {
        wrap = !wrap;
        ptrOffset = ptrOffset % buffSize;
    }

    Bool enableIrq = false;
    if(src)
    {
        qRegDMADCB_SrcWritePtrSet(channel, ptrOffset);
        qRegDMADCB_SrcWritePtrWrapSet(channel, wrap);

        /* Enable interrupt only if DEST is not full. */
        enableIrq = !(qRegDMADCB_DestReadPtrGet(channel) == qRegDMADCB_DestWritePtrGet(channel) &&
                      qRegDMADCB_DestReadPtrWrapGet(channel) != qRegDMADCB_DestWritePtrWrapGet(channel));
    }
    else
    {
        qRegDMADCB_DestReadPtrSet(channel, ptrOffset);
        qRegDMADCB_DestReadPtrWrapSet(channel, wrap);

        /* Enable interrupt only if SRC is not empty. */
        enableIrq = !(qRegDMADCB_SrcReadPtrGet(channel) == qRegDMADCB_SrcWritePtrGet(channel) &&
                      qRegDMADCB_SrcReadPtrWrapGet(channel) == qRegDMADCB_SrcWritePtrWrapGet(channel));
    }

    qRegDMADCB_TriggerSet(channel, trigger);
    qDrvDMADCB_ChannelEventIRQEnableSet(channel, enableIrq);

    HAL_ENABLE_GLOBAL_INT();

    return Q_OK;
}

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvDMADCB_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qResult_t qDrvDMADCB_QuicActionConfigSet(qRegQUIC_Channel_t quicChannel, qDrvQUIC_Internal_Instance_t* pInstance,
                                                void* pConfig, UInt32* pActionIdx)
{
    (void)pInstance;
    qDrvDMADCB_QuicActionConfig_t* const pActionConfig = (qDrvDMADCB_QuicActionConfig_t*)pConfig;

    if(!qDrvDMADCB_ChannelAllocGet(pActionConfig->dmaChannel) || !qDrvDMADCB_ChannelInitGet(pActionConfig->dmaChannel))
    {
        return Q_ERR_INVSTATE;
    }

    if((quicChannel >= REG_QUIC_CHANNEL_COUNT) || (pActionConfig->dmaChannel >= REG_DMA_DCB_CHANNEL_COUNT) ||
       (pActionConfig->actionIdx >= REG_DMA_DCB_QUIC_ACTION_SLOT_COUNT))
    {
        return Q_ERR_INVPARAM;
    }

    *pActionIdx = (UInt32)(((UInt32)pActionConfig->dmaChannel << 8UL) | pActionConfig->actionIdx);

    qRegDMADCB_QUICActionChannelSet(pActionConfig->dmaChannel, pActionConfig->actionIdx, quicChannel);
    qRegDMADCB_QUICActionTypeSet(pActionConfig->dmaChannel, pActionConfig->actionIdx, pActionConfig->action);

    return Q_OK;
}

static qResult_t qDrvDMADCB_QuicActionDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                                 qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;

    if(quicChannel >= REG_QUIC_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    UInt8 dmaChannel = (actionIdx >> 8UL) & 0xFFUL;
    actionIdx &= 0xFFUL;

    if(qRegDMADCB_QUICActionChannelGet(dmaChannel, actionIdx) == quicChannel)
    {
        qRegDMADCB_QUICActionTypeSet(dmaChannel, actionIdx, qDrvDMADCB_QuicActionNone);

        /* Disconnection performs successfully but driver needs to check whether DMA channel is properly used.
         */
        if(!qDrvDMADCB_ChannelAllocGet(dmaChannel) || !qDrvDMADCB_ChannelInitGet(dmaChannel))
        {
            return Q_ERR_INVSTATE;
        }
    }

    return Q_OK;
}

static const qDrvQUIC_Internal_Action_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskAction,
    .apiSpeedGet = qDrvDMADCB_QuicSpeedGet,
    .apiActionConfigSet = qDrvDMADCB_QuicActionConfigSet,
    .apiActionDisconnect = qDrvDMADCB_QuicActionDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvDMADCB_Init(qDrvDMADCB_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(dmaControlBlock.initialized)
    {
        return Q_ERR_INVSTATE;
    }

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    for(UInt8 i = 0; i < REG_DMA_DCB_CHANNEL_COUNT; i++)
    {
        qDrvDMADCB_ChannelEventIRQEnableSet(i, false);
    }

    memset(&dmaControlBlock, 0, sizeof(dmaControlBlock));
    dmaControlBlock.initialized = true;

    return Q_OK;
}

qResult_t qDrvDMADCB_Deinit(qDrvDMADCB_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!dmaControlBlock.initialized)
    {
        return Q_OK;
    }

    for(UInt8 i = 0; i < REG_DMA_DCB_CHANNEL_COUNT; i++)
    {
        (void)qDrvDMADCB_ChannelFree((qDrvDMADCB_Channel_t)i);
    }

    dmaControlBlock.initialized = false;

    return Q_OK;
}

Bool qDrvDMADCB_InitCheck(qDrvDMADCB_t* const pInstance)
{
    Q_ASSERT(pInstance);

    (void)pInstance;

    return dmaControlBlock.initialized;
}

qDrvDMADCB_Channel_t qDrvDMADCB_ChannelAlloc(void)
{
    if(!dmaControlBlock.initialized)
    {
        return Q_NOCHANNEL;
    }

    for(qDrvDMADCB_Channel_t channel = 0; channel < REG_DMA_DCB_CHANNEL_COUNT; ++channel)
    {
        if(!qDrvDMADCB_ChannelAllocGet(channel))
        {
            qDrvDMADCB_ChannelAllocSet(channel, true);
            return channel;
        }
    }

    return Q_NOCHANNEL;
}

qResult_t qDrvDMADCB_ChannelFree(qDrvDMADCB_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);

    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvDMADCB_ChannelAllocGet(channel))
    {
        return Q_ERR_INVPARAM;
    }

    if(qDrvDMADCB_ChannelInitGet(channel))
    {
        qDrvDMADCB_ChannelDeinit(channel);
    }

    qDrvDMADCB_ChannelAllocSet(channel, false);

    return Q_OK;
}

qResult_t qDrvDMADCB_ChannelInit(qDrvDMADCB_Channel_t channel, const qDrvDMADCB_ChannelConfig_t* const pChannelConfig,
                                 void* arg)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);
    Q_ASSERT(pChannelConfig);

    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvDMADCB_ChannelAllocGet(channel) || qDrvDMADCB_ChannelInitGet(channel))
    {
        return Q_ERR_INVPARAM;
    }

    qDrvDMADCB_ChannelControlBlock_t* pCb = qDrvDMADCB_ChannelControlBlockGet(channel);
    pCb->callback = pChannelConfig->callback;
    pCb->arg = arg;

    /* Reset and configure channel configuration registers. */
    qDrvDMADCB_ChannelEventIRQEnableSet(channel, false);

    qRegDMADCB_TriggerSet(channel, false);
    qRegDMADCB_SrcAddrSet(channel, 0);
    qRegDMADCB_DestAddrSet(channel, 0);
    qRegDMADCB_PtrReset(channel);
    qRegDMADCB_SrcWritePtrWrapSet(channel, 0);
    qRegDMADCB_WordModeSet(channel, pChannelConfig->wordMode);

    /* Enable global interrupt if it was disabled previously (there was no active channels). */
    if(dmaControlBlock.channelInit == 0)
    {
        HAL_DISABLE_GLOBAL_INT();

        qRegIntCtrl_EventIRQDisable(DMA_SCB_IRQn, (1UL << qRegDMADCB_IrqCpyErr), 0);
        qRegDMADCB_IRQCpyErrorClear();
        qRegDMADCB_RetainSet(true);
        NVIC_ClearPendingIRQ(DMA_DCB_IRQn);
        NVIC_EnableIRQ(DMA_DCB_IRQn);
        qRegIntCtrl_IRQEnable(DMA_DCB_IRQn);

        HAL_ENABLE_GLOBAL_INT();
    }

    qDrvDMADCB_ChannelInitSet(channel, true);
    return Q_OK;
}

qResult_t qDrvDMADCB_ChannelDeinit(qDrvDMADCB_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);

    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvDMADCB_ChannelAllocGet(channel) || !qDrvDMADCB_ChannelInitGet(channel))
    {
        return Q_ERR_INVPARAM;
    }

    if(qDrvDMADCB_ChannelEnabledCheck(channel))
    {
        qResult_t result = qDrvDMADCB_ChannelDisable(channel);
        if(result != Q_OK)
        {
            return result;
        }
    }

    qDrvDMADCB_ChannelControlBlock_t* pCb = qDrvDMADCB_ChannelControlBlockGet(channel);
    pCb->callback = NULL;

    HAL_DISABLE_GLOBAL_INT();

    /* Set active bitmask for this channel to off. */
    qDrvDMADCB_ChannelInitSet(channel, false);

    if(dmaControlBlock.channelInit == 0)
    {
        /* No active DMA channels. Global interrupt can be disabled. */
        qRegIntCtrl_IRQDisable(DMA_DCB_IRQn);
        NVIC_DisableIRQ(DMA_DCB_IRQn);
        NVIC_ClearPendingIRQ(DMA_DCB_IRQn);
        qRegDMADCB_RetainSet(false);
    }

    HAL_ENABLE_GLOBAL_INT();

    return Q_OK;
}

Bool qDrvDMADCB_ChannelInitCheck(qDrvDMADCB_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);

    if(!dmaControlBlock.initialized)
    {
        return false;
    }

    return qDrvDMADCB_ChannelInitGet(channel);
}

qResult_t qDrvDMADCB_ChannelEnable(qDrvDMADCB_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);

    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvDMADCB_ChannelAllocGet(channel) || !qDrvDMADCB_ChannelInitGet(channel) ||
       qRegDMADCB_SrcAddrGet(channel) == 0 || qRegDMADCB_DestAddrGet(channel) == 0)
    {
        return Q_ERR_INVPARAM;
    }

    qRegDMADCB_TriggerSet(channel, true);

    return Q_OK;
}

qResult_t qDrvDMADCB_ChannelDisable(qDrvDMADCB_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);

    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvDMADCB_ChannelAllocGet(channel) || !qDrvDMADCB_ChannelInitGet(channel))
    {
        return Q_ERR_INVPARAM;
    }

    qRegDMADCB_TriggerSet(channel, false);

    return Q_OK;
}

Bool qDrvDMADCB_ChannelEnabledCheck(qDrvDMADCB_Channel_t channel)
{
    return qRegDMADCB_TriggerGet(channel);
}

qResult_t qDrvDMADCB_BufferSet(qDrvDMADCB_Channel_t channel, void* pSrcBuf, UInt16 srcBufSize, void* pDestBuf,
                               UInt16 destBufSize)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);
    Q_ASSERT(pSrcBuf && srcBufSize);
    Q_ASSERT(pDestBuf && destBufSize);

    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvDMADCB_ChannelAllocGet(channel) || !qDrvDMADCB_ChannelInitGet(channel))
    {
        return Q_ERR_INVPARAM;
    }

    UInt8 bytesPerWord = qDrvDMADCB_WordModeToBytes(qRegDMADCB_WordModeGet(channel));

    if(((srcBufSize % bytesPerWord) != 0) || ((destBufSize % bytesPerWord) != 0))
    {
        /* Buffer size must be the multiplication of the word mode. */
        return Q_ERR_INVPARAM;
    }

    if((((UInt32)pSrcBuf % bytesPerWord) != 0) || (((UInt32)pDestBuf % bytesPerWord) != 0))
    {
        /* Memory address must be aligned to the word mode. */
        return Q_ERR_INVPARAM;
    }

    Bool trigger = qRegDMADCB_TriggerGet(channel);
    qDrvDMADCB_ChannelControlBlock_t* pCb = qDrvDMADCB_ChannelControlBlockGet(channel);
    pCb->pSrcBuf = pSrcBuf;
    pCb->pDestBuf = pDestBuf;

    if(trigger && !qRegDMADCB_IRQBufferCompleteGet(channel))
    {
        /* Transfer in progress. */
        return Q_ERR_BUSY;
    }

    qDrvDMADCB_ChannelEventIRQEnableSet(channel, false);
    qRegDMADCB_TriggerSet(channel, false);
    qRegDMADCB_PtrReset(channel);
    qRegDMADCB_SrcWritePtrWrapSet(channel, 0);
    qRegDMADCB_SrcBufferSizeSet(channel, (srcBufSize / bytesPerWord) - 1UL);
    qRegDMADCB_SrcAddrSet(channel, (UInt32)pSrcBuf);
    qRegDMADCB_DestBufferSizeSet(channel, (destBufSize / bytesPerWord) - 1UL);
    qRegDMADCB_DestAddrSet(channel, (UInt32)pDestBuf);
    qRegDMADCB_TriggerSet(channel, trigger);

    return Q_OK;
}

qResult_t qDrvDMADCB_SrcAdvance(qDrvDMADCB_Channel_t channel, UInt16 wordCount)
{
    return qDrvDMADCB_Advance(channel, wordCount, true);
}

qResult_t qDrvDMADCB_DestAdvance(qDrvDMADCB_Channel_t channel, UInt16 wordCount)
{
    return qDrvDMADCB_Advance(channel, wordCount, false);
}

Bool qDrvDMADCB_ChannelBusyCheck(qDrvDMADCB_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_DCB_CHANNEL_COUNT);

    if(!dmaControlBlock.initialized)
    {
        return false;
    }

    if(!qDrvDMADCB_ChannelInitGet(channel))
    {
        return false;
    }

    if(qRegDMADCB_TriggerGet(channel) && !qRegDMADCB_IRQBufferCompleteGet(channel))
    {
        return true;
    }

    return false;
}

qResult_t qDrvDMADCB_ErrorCallbackSet(qDrvDMADCB_CbError_t callback)
{
    if(!dmaControlBlock.initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    dmaControlBlock.cbCpyErr = callback;

    return Q_OK;
}

Q_REG_DMADCB_HANDLER_DEFINE()
{
    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(DMA_DCB_IRQn, &maskedEvents, NULL);

    if(maskedEvents & (1UL << qRegDMADCB_IrqCpyErr))
    {
        qRegDMADCB_IRQCpyErrorClear();
        if(dmaControlBlock.cbCpyErr)
        {
            dmaControlBlock.cbCpyErr();
        }
    }

    maskedEvents &= Q_REG_DMADCB_CHANNEL_IRQ_MASK;

    while(maskedEvents)
    {
        qDrvDMADCB_Channel_t channel = (qDrvDMADCB_Channel_t)__builtin_ctz((UInt32)maskedEvents);
        qDrvDMADCB_ChannelControlBlock_t* pCb = qDrvDMADCB_ChannelControlBlockGet(channel);
        BIT_CLR(maskedEvents, (UInt32)channel);

        /* Interrupt flag stays active until the transfer complete condition is true.
           Event interrupt will be re-enabled after advancing source pointer. */
        qDrvDMADCB_ChannelEventIRQEnableSet(channel, false);

        if(pCb->callback)
        {
            pCb->callback(pCb->arg, channel);
        }
    }
}
