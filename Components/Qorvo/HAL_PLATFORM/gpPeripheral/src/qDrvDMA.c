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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qDrvDMA.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief DMA channel control block data. */
typedef struct {
    qDrvDMA_CbChannel_t callback;
    UInt32 dmaTriggerSelect;
    void* pMemBuf;
    void* arg;
    qDrvDMA_ChannelMode_t mode;
    qDrvDMA_BufferMode_t bufferMode;
    Bool writePtrWrap;
    Bool noAdvance;
    UInt16 writePtr;
    UInt16 thresholdWordIndex;
} qDrvDMA_ChannelControlBlock_t;

/* Verify that the size do not increase. */
Q_STATIC_ASSERT(sizeof(qDrvDMA_ChannelControlBlock_t) == (6 * 4));

/** @brief DMA channel control block data. */
typedef struct {
    qDrvDMA_ChannelControlBlock_t channelControlBlock[REG_DMA_CHANNEL_COUNT];
    qDrvDMA_CbError_t cbCpyErr;
    UInt16 channelAlloc;
    UInt16 channelInit;
} qDrvDMA_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/* Internal DMA control block data. */
static qDrvDMA_ControlBlock_t dmaControlBlock;

/* Check the size of the masks. */
Q_STATIC_ASSERT(sizeof(dmaControlBlock.channelAlloc) * 8 >= REG_DMA_CHANNEL_COUNT);
Q_STATIC_ASSERT(sizeof(dmaControlBlock.channelInit) * 8 >= REG_DMA_CHANNEL_COUNT);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static inline qDrvDMA_ChannelControlBlock_t* qDrvDMA_ChannelControlBlockGet(UInt8 channel)
{
    return &dmaControlBlock.channelControlBlock[channel];
}

static inline Bool qDrvDMA_ChannelAllocGet(qDrvDMA_Channel_t channel)
{
    return BIT_TST(dmaControlBlock.channelAlloc, channel);
}

static inline void qDrvDMA_ChannelAllocSet(qDrvDMA_Channel_t channel, Bool enable)
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

static inline Bool qDrvDMA_ChannelInitGet(qDrvDMA_Channel_t channel)
{
    return BIT_TST(dmaControlBlock.channelInit, channel);
}

static inline void qDrvDMA_ChannelInitSet(qDrvDMA_Channel_t channel, Bool enable)
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

static UInt8 qDrvDMA_WordModeToBytes(qRegDMA_WordMode_t wordMode)
{
    switch(wordMode)
    {
        case qRegDMA_WordMode8Bit:
            return 1;
        case qRegDMA_WordMode16Bit:
            return 2;
        case qRegDMA_WordMode32Bit:
            return 4;
        default:
            break;
    }

    return 0;
}

static inline void qDrvDMA_ChannelEventIRQEnableSet(qDrvDMA_Channel_t channel, qRegDMA_ChannelIrq_t irq, Bool enable)
{
    UInt32 shift = irq == qRegDMA_ChannelIrqComplete ? 16UL : 0UL;
    shift += (UInt32)channel;

    UInt32 mask = 1UL << shift;
    if(enable)
    {
        qRegIntCtrl_EventIRQEnable(DMA_SCB_IRQn, mask, 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(DMA_SCB_IRQn, mask, 0);
    }
}

static void* qDrvDMA_ContinuousWordChunkGet(qDrvDMA_Channel_t channel, UInt16* pWordCount)
{
    UInt32 bufferPtr = qRegDMA_BufferPtrValueGet(channel);
    Bool bufferPtrWrap = qRegDMA_BufferPtrWrapValueGet(channel);

    // internalPtr and internalPtrWrap should be read in one shot to avoid race conditions.
    UInt32 internalPtrStatus = qRegDMA_InternalPtrStatusGet(channel);
    UInt32 internalPtr = qRegDMA_InternalPtrFromInternalPtrStatusGet(internalPtrStatus);
    Bool internalPtrWrap = qRegDMA_InternalPtrWrapFromInternalPtrStatusGet(internalPtrStatus);

    UInt32 bufferSize = qRegDMA_BufferSizeGet(channel) + 1UL;

    qRegDMA_WordMode_t wordMode = qRegDMA_WordModeGet(channel);
    UInt32 startWordIndex = 0; /* Last read word index. */

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

    if(cb->mode == qDrvDMA_ChannelModeMemToPeri)
    {
        if(internalPtrWrap != bufferPtrWrap)
        {
            *pWordCount = internalPtr - bufferPtr;
        }
        else
        {
            *pWordCount = bufferSize - bufferPtr;
        }
        startWordIndex = bufferPtr;
    }

    if(cb->mode == qDrvDMA_ChannelModePeriToMem)
    {
        if(internalPtrWrap == bufferPtrWrap)
        {
            *pWordCount = internalPtr - bufferPtr;
        }
        else
        {
            *pWordCount = bufferSize - bufferPtr;
        }
        startWordIndex = bufferPtr;
    }

    if(*pWordCount == 0)
    {
        return NULL;
    }

    return (void*)((UInt8*)(cb->pMemBuf) + (startWordIndex * qDrvDMA_WordModeToBytes(wordMode)));
}

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvDMA_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qResult_t qDrvDMA_QuicActionConfigSet(qRegQUIC_Channel_t quicChannel, qDrvQUIC_Internal_Instance_t* pInstance,
                                             void* pConfig, UInt32* pActionIdx)
{
    (void)pInstance;
    qDrvDMA_QuicActionConfig_t* const pActionConfig = (qDrvDMA_QuicActionConfig_t*)pConfig;

    if(!qDrvDMA_ChannelAllocGet(pActionConfig->dmaChannel))
    {
        /* Channel is not allocated.*/
        return Q_ERR_INVPARAM;
    }

    if(!qDrvDMA_ChannelInitGet(pActionConfig->dmaChannel))
    {
        /* Channel is not initialized. */
        return Q_ERR_INVSTATE;
    }

    if((quicChannel >= REG_QUIC_CHANNEL_COUNT) || (pActionConfig->dmaChannel >= REG_DMA_CHANNEL_COUNT))
    {
        return Q_ERR_INVPARAM;
    }

    *pActionIdx = (UInt32)pActionConfig->dmaChannel;

    qRegDMA_QuicChannelSet(pActionConfig->dmaChannel, quicChannel);
    qRegDMA_QuicActionSet(pActionConfig->dmaChannel, pActionConfig->action);

    return Q_OK;
}

static qResult_t qDrvDMA_QuicActionDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                              qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;

    if(quicChannel >= REG_QUIC_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    UInt8 dmaChannel = actionIdx;

    if(qRegDMA_QuicChannelGet(dmaChannel) == quicChannel)
    {
        qRegDMA_QuicActionSet(dmaChannel, qDrvDMA_QuicActionNone);

        /* Disconnection performs successfully but driver needs to check whether DMA channel is properly used. */
        if(!qDrvDMA_ChannelAllocGet(dmaChannel))
        {
            /* Channel is not allocated.*/
            return Q_ERR_INVPARAM;
        }

        if(!qDrvDMA_ChannelInitGet(dmaChannel))
        {
            /* Channel is not initialized. */
            return Q_ERR_INVSTATE;
        }
    }

    return Q_OK;
}

static qDrvQUIC_Internal_Action_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskAction,
    .apiSpeedGet = qDrvDMA_QuicSpeedGet,
    .apiActionConfigSet = qDrvDMA_QuicActionConfigSet,
    .apiActionDisconnect = qDrvDMA_QuicActionDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvDMA_Init(qDrvDMA_t* const pInstance)
{
    Q_ASSERT(pInstance);
#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */
    return Q_OK;
}

qResult_t qDrvDMA_ChannelInit(qDrvDMA_Channel_t channel, const qDrvDMA_ChannelConfig_t* const pChannelConfig, void* arg)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);
    Q_ASSERT(pChannelConfig);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated.*/
        return Q_ERR_INVPARAM;
    }

    if(qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel is already initialized. */
        return Q_ERR_INVSTATE;
    }

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

    /* Clear the control block data. */
    memset(cb, 0, sizeof(qDrvDMA_ChannelControlBlock_t));

    cb->callback = pChannelConfig->callback;
    cb->dmaTriggerSelect = pChannelConfig->dmaTriggerSelect;
    cb->arg = arg;
    cb->noAdvance = pChannelConfig->noAdvance;

    if(dmaControlBlock.channelInit == 0)
    {
        /* DMA enable. */

        /* Disable all interrupts from the previous configuration. */
        for(UInt8 i = 0; i < REG_DMA_CHANNEL_COUNT; i++)
        {
            qDrvDMA_ChannelEventIRQEnableSet(i, qRegDMA_ChannelIrqComplete, false);
            qDrvDMA_ChannelEventIRQEnableSet(i, qRegDMA_ChannelIrqAlmostComplete, false);
        }
    }

    /* Disable events form the previous channel configuration. */
    qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqComplete, false);
    qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqAlmostComplete, false);

    /* Clear events from the previous channel configuration. */
    qRegDMA_ChannelEventIRQClear(channel, qRegDMA_ChannelIrqComplete);

    /* Disable the channel. */
    qRegDMA_TriggerSet(channel, qRegDMA_TriggerNone);

    /* Set up the write mode: Byte, Half Word or Word. */
    qRegDMA_WordModeSet(channel, pChannelConfig->wordMode);

    /* Config the buffer complete interrupt mode. */
    if(pChannelConfig->bufferMode == qDrvDMA_BufferModeSingleShot)
    {
        /* In static buffer mode, complete event is just indicator that the buffer is full or empty. */
        qRegDMA_CompleteModeSet(channel, qRegDMA_CompleteModeStatus);
    }
    else if(pChannelConfig->bufferMode == qDrvDMA_BufferModeCircular)
    {
        /* In dynamic buffer mode, complete event is an buffer error. */
        qRegDMA_CompleteModeSet(channel, qRegDMA_CompleteModeError);
    }
    else
    {
        /* Invalid buffer mode. */
        return Q_ERR_INVPARAM;
    }

    cb->bufferMode = pChannelConfig->bufferMode;

    if(pChannelConfig->mode == qDrvDMA_ChannelModePeriToMem)
    {
        /* Memory is the destination. Memory should be in the circular buffer mode. */
        qRegDMA_BufferSelectSet(channel, qRegDMA_CircularBufferDest);

        /* Memory address should be set with the qDrvDMA_BufferSet() function. */
        qRegDMA_DestAddrSet(channel, 0);

        /* Setup source address. */
        qRegDMA_SrcAddrSet(channel, pChannelConfig->periAddress);
    }
    else if(pChannelConfig->mode == qDrvDMA_ChannelModeMemToPeri)
    {
        /* Memory is the source. Memory should be in the circular buffer mode. */
        qRegDMA_BufferSelectSet(channel, qRegDMA_CircularBufferSrc);

        /* Memory address should be set with the qDrvDMA_BufferSet() function. */
        qRegDMA_SrcAddrSet(channel, 0);

        /* Setup destination address. */
        qRegDMA_DestAddrSet(channel, pChannelConfig->periAddress);
    }
    else
    {
        /* Invalid channel mode. */
        return Q_ERR_INVPARAM;
    }

    cb->mode = pChannelConfig->mode;

    /* Reset internal pointers. */
    qRegDMA_PtrReset(channel);

    cb->writePtr = 0;
    cb->writePtrWrap = false;

    /* Reset work pointer. */
    qRegDMA_BufferPtrValueSet(channel, 0);
    qRegDMA_BufferPtrWrapValueSet(channel, false);

    /* Set pointer wrap to be the same as an internal pointer wrap. */
    qRegDMA_BufferPtrWrapValueSet(channel, qRegDMA_InternalPtrWrapGet(channel));

    /* Check whether there are already active DMA channels, if not, start the NVIC and enable the global DMA interrupt.
     */
    if(dmaControlBlock.channelInit == 0)
    {
        /* First DMA to start, enable global INT */
        qRegDMA_IRQCpyErrorClear();
        NVIC_ClearPendingIRQ(DMA_SCB_IRQn);
        NVIC_SetPriority(DMA_SCB_IRQn, Q_DRV_IRQ_PRIO_MID);
        NVIC_EnableIRQ(DMA_SCB_IRQn);
        qRegIntCtrl_IRQEnable(DMA_SCB_IRQn);
    }

    /* Always enable CPY ERR Interrupt. */
    qRegIntCtrl_EventIRQEnable(DMA_SCB_IRQn, 0, (1UL << qRegDMA_IrqCpyErr));

    /* Set active bitmask for this channel to on. */
    qDrvDMA_ChannelInitSet(channel, true);

    return Q_OK;
}

qResult_t qDrvDMA_ChannelDeinit(qDrvDMA_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVPARAM;
    }

    if(!qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel is not initialized. */
        return Q_ERR_INVSTATE;
    }

    if(qDrvDMA_ChannelEnabledCheck(channel))
    {
        qResult_t result = qDrvDMA_ChannelDisable(channel);
        if(result != Q_OK)
        {
            return result;
        }
    }

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);
    cb->callback = NULL;
    cb->dmaTriggerSelect = qRegDMA_TriggerNone;
    cb->pMemBuf = NULL;
    qRegDMA_DestAddrSet(channel, 0);
    qRegDMA_SrcAddrSet(channel, 0);

    /* Set active bitmask for this channel to off. */
    qDrvDMA_ChannelInitSet(channel, false);

    if(dmaControlBlock.channelInit == 0)
    {
        /* No more DMA transfers running at the moment. */
        qRegIntCtrl_IRQDisable(DMA_SCB_IRQn);
        NVIC_DisableIRQ(DMA_SCB_IRQn);
        NVIC_ClearPendingIRQ(DMA_SCB_IRQn);
    }

    return Q_OK;
}

qResult_t qDrvDMA_ChannelEnable(qDrvDMA_Channel_t channel, UInt16 almostCompleteThreshold)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVPARAM;
    }

    if(!qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel is not initialized. */
        return Q_ERR_INVSTATE;
    }

    if((qRegDMA_SrcAddrGet(channel) == 0) || (qRegDMA_DestAddrGet(channel) == 0))
    {
        /* Addresses can not be 0. */
        return Q_ERR_INVSTATE;
    }

    if(qRegDMA_TriggerGet(channel) != qRegDMA_TriggerNone)
    {
        /* Channel is already enabled. */
        return Q_ERR_INVSTATE;
    }

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

    cb->thresholdWordIndex = almostCompleteThreshold;

    if(cb->mode == qDrvDMA_ChannelModeMemToPeri)
    {
        qRegDMA_AlmostCompleteThresholdSet(channel, qRegDMA_BufferSizeGet(channel) - almostCompleteThreshold);
    }

    if(cb->mode == qDrvDMA_ChannelModePeriToMem && almostCompleteThreshold > 0)
    {
        qRegDMA_AlmostCompleteThresholdSet(channel, almostCompleteThreshold - 1);
    }

    if(cb->callback)
    {
        /* Enable channel interrupts. */
        if(almostCompleteThreshold > 0)
        {
            qRegDMA_ChannelEventIRQClear(channel, qRegDMA_ChannelIrqAlmostComplete);
            qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqAlmostComplete, true);
        }
        else
        {
            qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqAlmostComplete, false);
            qRegDMA_ChannelEventIRQClear(channel, qRegDMA_ChannelIrqAlmostComplete);
        }

        qRegDMA_ChannelEventIRQClear(channel, qRegDMA_ChannelIrqComplete);
        qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqComplete, true);
    }
    else
    {
        qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqAlmostComplete, false);
        qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqComplete, false);

        qRegDMA_ChannelEventIRQClear(channel, qRegDMA_ChannelIrqAlmostComplete);
        qRegDMA_ChannelEventIRQClear(channel, qRegDMA_ChannelIrqComplete);
    }

    qRegDMA_TriggerSet(channel, cb->dmaTriggerSelect);

    return Q_OK;
}

qResult_t qDrvDMA_ChannelDisable(qDrvDMA_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVPARAM;
    }

    if(!qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel is not initialized. */
        return Q_ERR_INVSTATE;
    }

    /* Disable DMA channel. */
    qRegDMA_TriggerSet(channel, qRegDMA_TriggerNone);

    /* Disable channel interrupts. */
    qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqAlmostComplete, false);
    qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqComplete, false);

    return Q_OK;
}

Bool qDrvDMA_ChannelEnabledCheck(qDrvDMA_Channel_t channel)
{
    return qRegDMA_TriggerGet(channel) != qRegDMA_TriggerNone;
}

qResult_t qDrvDMA_BufferSet(qDrvDMA_Channel_t channel, void* buffer, UInt16 bufferSize)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);
    Q_ASSERT(buffer);
    Q_ASSERT(bufferSize);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVPARAM;
    }

    if(!qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel is not initialized. */
        return Q_ERR_INVSTATE;
    }

    if(qDrvDMA_ChannelEnabledCheck(channel))
    {
        /* Channel is enabled. */
        return Q_ERR_INVSTATE;
    }

    qRegDMA_WordMode_t wordMode = qRegDMA_WordModeGet(channel);
    UInt8 bytesWordCount = qDrvDMA_WordModeToBytes(wordMode);

    if((bufferSize % bytesWordCount) != 0)
    {
        /* Buffer size should be the multiplication of the word mode. */
        return Q_ERR_INVPARAM;
    }

    if(((UInt32)buffer % bytesWordCount) != 0)
    {
        /* Memory address should be aligned to the word mode. */
        return Q_ERR_INVPARAM;
    }

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

    /* Reset internal pointers. */
    qRegDMA_PtrReset(channel);

    /* Reset work pointer. */
    qRegDMA_BufferPtrValueSet(channel, 0);
    qRegDMA_BufferPtrWrapValueSet(channel, 0);

    cb->writePtr = 0;
    cb->writePtrWrap = false;
    cb->pMemBuf = buffer;

    qRegDMA_BufferSizeSet(channel, (bufferSize / bytesWordCount) - 1UL);

    if(cb->mode == qDrvDMA_ChannelModePeriToMem)
    {
        qRegDMA_DestAddrSet(channel, (UInt32)buffer);
    }
    else if(cb->mode == qDrvDMA_ChannelModeMemToPeri)
    {
        qRegDMA_SrcAddrSet(channel, (UInt32)buffer);
    }

    return Q_OK;
}

void* qDrvDMA_ContinuousChunkGet(qDrvDMA_Channel_t channel, UInt16* pWordCount)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);
    Q_ASSERT(pWordCount);

    if(!qDrvDMA_ChannelAllocGet(channel) || !qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel not allocated or channel is inactive. */
        *pWordCount = 0;
        return NULL;
    }

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

    if(cb->pMemBuf == NULL)
    {
        /* Memory buffer is not set. */
        *pWordCount = 0;
        return NULL;
    }

    return qDrvDMA_ContinuousWordChunkGet(channel, pWordCount);
}

static qResult_t qDrvDMA_WordAdvance(qDrvDMA_Channel_t channel, UInt16 wordCount)
{
    UInt32 offsetToBeChanged = wordCount;

    UInt32 buffSize = qRegDMA_BufferSizeGet(channel) + 1UL;

    if(offsetToBeChanged > buffSize)
    {
        /* Too much data to be changed. */
        return Q_ERR_INVPARAM;
    }

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

    cb->writePtr += offsetToBeChanged;

    if((cb->writePtr / buffSize) > 0)
    {
        cb->writePtrWrap = !cb->writePtrWrap;
        cb->writePtr = cb->writePtr % buffSize;
    }

    HAL_DISABLE_GLOBAL_INT();
    qRegDMA_BufferPtrValueSet(channel, cb->writePtr);
    qRegDMA_BufferPtrWrapValueSet(channel, cb->writePtrWrap);

    if(cb->mode == qDrvDMA_ChannelModeMemToPeri)
    {
        qRegDMA_WritePtrSet(channel);
    }
    else if(cb->mode == qDrvDMA_ChannelModePeriToMem)
    {
        qRegDMA_ReadPtrSet(channel);
    }
    HAL_ENABLE_GLOBAL_INT();

    return Q_OK;
}

qResult_t qDrvDMA_Advance(qDrvDMA_Channel_t channel, UInt16 wordCount)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVPARAM;
    }

    if(!qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel is not initialized. */
        return Q_ERR_INVSTATE;
    }

    qResult_t result = qDrvDMA_WordAdvance(channel, wordCount);

    qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

    if(cb->noAdvance)
    {
        qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqAlmostComplete, true);
    }

    return result;
}

qDrvDMA_Channel_t qDrvDMA_Alloc()
{
    for(qDrvDMA_Channel_t channel = 0; channel < REG_DMA_CHANNEL_COUNT; ++channel)
    {
        if(!qDrvDMA_ChannelAllocGet(channel))
        {
            if(!dmaControlBlock.channelAlloc)
            {
                qRegDMA_RetainSet(true);
            }
            qDrvDMA_ChannelAllocSet(channel, true);
            return channel;
        }
    }

    return Q_NOCHANNEL;
}

qResult_t qDrvDMA_ChannelFree(qDrvDMA_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVSTATE;
    }

    if(qDrvDMA_ChannelInitGet(channel))
    {
        qDrvDMA_ChannelDeinit(channel);
    }

    qDrvDMA_ChannelAllocSet(channel, false);

    if(!dmaControlBlock.channelAlloc)
    {
        qRegDMA_RetainSet(false);
    }

    return Q_OK;
}

qResult_t qDrvDMA_ChannelIRQEnableSet(qDrvDMA_Channel_t channel, qRegDMA_ChannelIrq_t irq, Bool enable)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);

    if(!qDrvDMA_ChannelAllocGet(channel))
    {
        /* Channel is not allocated. */
        return Q_ERR_INVPARAM;
    }

    if(!qDrvDMA_ChannelInitGet(channel))
    {
        /* Channel is inactive. */
        return Q_ERR_INVSTATE;
    }

    qDrvDMA_ChannelEventIRQEnableSet(channel, irq, enable);

    return Q_OK;
}

qResult_t qDrvDMA_ErrorCallbackSet(qDrvDMA_CbError_t callback)
{
    dmaControlBlock.cbCpyErr = callback;

    return Q_OK;
}

Bool qDrvDMA_ChannelCompeteCheck(qDrvDMA_Channel_t channel)
{
    Q_ASSERT(channel < REG_DMA_CHANNEL_COUNT);

    if(!qDrvDMA_ChannelAllocGet(channel) || !qDrvDMA_ChannelInitGet(channel))
    {
        return false;
    }

    return qRegDMA_StatusCompleteGet(channel);
}

void dma_scb_handler_impl(void)
{
    UInt32 completeMaskedEvents;
    UInt32 errorMaskedEvents;

    HAL_DISABLE_GLOBAL_INT();

    qRegIntCtrl_EventsIRQMaskedGet(DMA_SCB_IRQn, &completeMaskedEvents, &errorMaskedEvents);

    if(errorMaskedEvents & (1UL << qRegDMA_IrqCpyErr))
    {
        if(dmaControlBlock.cbCpyErr)
        {
            dmaControlBlock.cbCpyErr();
        }
        qRegDMA_IRQCpyErrorClear();
    }

    qRegDMA_IrqMask_t almostCompleteMask = completeMaskedEvents & 0x0000FFFFUL;

    while(almostCompleteMask != 0)
    {
        qDrvDMA_Channel_t channel = (qDrvDMA_Channel_t)__builtin_ctz((UInt32)almostCompleteMask);
        BIT_CLR(almostCompleteMask, (UInt32)channel);

        qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

        if(cb->noAdvance || cb->bufferMode == qDrvDMA_BufferModeSingleShot)
        {
            /* If buffer mode is static, it is only "one-shot interrupt". */
            qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqAlmostComplete, false);
        }

        UInt16 chunkSizeInWord = 0;
        UInt16 wordsToBeProceed = cb->thresholdWordIndex;

        void* pChunk = qDrvDMA_ContinuousWordChunkGet(channel, &chunkSizeInWord);

        if(cb->noAdvance)
        {
            if(cb->callback)
            {
                cb->callback(cb->arg, qDrvDMA_EventTypeAlmostComplete, pChunk, chunkSizeInWord);
            }
        }
        else
        {
            if(chunkSizeInWord < wordsToBeProceed)
            {
                if(cb->callback)
                {
                    cb->callback(cb->arg, qDrvDMA_EventTypeAlmostComplete, pChunk, chunkSizeInWord);
                }

                qDrvDMA_WordAdvance(channel, chunkSizeInWord);
                wordsToBeProceed -= chunkSizeInWord;

                pChunk = qDrvDMA_ContinuousWordChunkGet(channel, &chunkSizeInWord);
            }

            if(cb->callback)
            {
                cb->callback(cb->arg, qDrvDMA_EventTypeAlmostComplete, pChunk, wordsToBeProceed);
            }

            qDrvDMA_WordAdvance(channel, wordsToBeProceed);
        }
    }

    qRegDMA_IrqMask_t completeMask = (completeMaskedEvents & 0xFFFF0000UL) >> 16UL;

    while(completeMask != 0)
    {
        volatile qDrvDMA_Channel_t channel = (qDrvDMA_Channel_t)__builtin_ctz((UInt32)completeMask);
        BIT_CLR(completeMask, (UInt32)channel);

        qDrvDMA_ChannelControlBlock_t* cb = qDrvDMA_ChannelControlBlockGet(channel);

        if(cb->bufferMode == qDrvDMA_BufferModeSingleShot)
        {
            /* Disable DMA channel. */
            qRegDMA_TriggerSet(channel, qRegDMA_TriggerNone);

            /* For single shot mode, disable next expected transfer complete event. */
            qDrvDMA_ChannelEventIRQEnableSet(channel, qRegDMA_ChannelIrqComplete, false);
        }

        qDrvDMA_EventType_t eventType = qDrvDMA_EventTypeChannelError;

        if(cb->mode == qDrvDMA_ChannelModeMemToPeri)
        {
            eventType = (cb->bufferMode == qDrvDMA_BufferModeSingleShot) ? qDrvDMA_EventTypeBufEmpty
                        : (cb->bufferMode == qDrvDMA_BufferModeCircular) ? qDrvDMA_EventTypeBufUnderrun
                                                                         : qDrvDMA_EventTypeChannelError;
        }
        else if(cb->mode == qDrvDMA_ChannelModePeriToMem)
        {
            eventType = (cb->bufferMode == qDrvDMA_BufferModeSingleShot) ? qDrvDMA_EventTypeBufFull
                        : (cb->bufferMode == qDrvDMA_BufferModeCircular) ? qDrvDMA_EventTypeBufOverflow
                                                                         : qDrvDMA_EventTypeChannelError;
        }

        UInt16 chunkSizeInWord;
        void* pChunk = qDrvDMA_ContinuousWordChunkGet(channel, &chunkSizeInWord);

        if(cb->callback && (pChunk != NULL) && (chunkSizeInWord > 0))
        {
            cb->callback(cb->arg, eventType, pChunk, chunkSizeInWord);
        }

        if(eventType == qDrvDMA_EventTypeChannelError)
        {
            /* Disable the channel if configuration error occurs. */
            qDrvDMA_ChannelDisable(channel);
        }

        /* Clear the complete event interrupt. */
        qRegDMA_ChannelEventIRQClear(channel, qRegDMA_ChannelIrqComplete);
    }

    HAL_ENABLE_GLOBAL_INT();
}
