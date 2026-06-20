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
#include "qDrvRTZ.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro for getting the valid register value. */
#define DRV_RTZ_VALUE_TO_REGISTER_GET(VALUE) (((VALUE) > 0) ? ((VALUE) - 1UL) : 0UL)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief Control block - RTZ driver instance local data. */
typedef struct {
    qDrvRTZ_Cb_t callback;        /**< User defined callback handler. */
    void* arg;                    /**< User defined argument to be passed to the callback. */
    void* pBuffer;                /**< Pointer to the memory buffer for pixels data. */
    UInt16 wordCount;             /** Size of the pixels buffer in words. */
    UInt16 sampleIndex;           /**< Index of the current sample. */
    qDrvDMA_Channel_t dmaChannel; /**< DMA channel to be used by the instance driver.  */
    Bool rtzTransmitting;         /**< Flag for indicating whether the RTZ instance is transmitting the sequence. */
    UInt8 wordSizeBytes;          /**< Word size in bytes. */
    qDrvRTZ_TransferMode_t transferMode; /**< Transfer mode. */
} qDrvRTZ_ControlBlock_t;

/* Control block data are stored in the internal data memory region of the instance.   */
Q_STATIC_ASSERT(sizeof(qDrvRTZ_ControlBlock_t) == sizeof(qDrvRTZ_Internal_t));

/** @brief Constant values - RTZ driver constant instance data that are unable to be calculated at runtime. */
typedef struct {
    IRQn_Type irqn;          /**< Interrupt number used by the peripheral. */
    UInt32 dmaTriggerSelect; /**< DMA source select of the peripheral. */
    UInt32 txDataAddress;    /**< TX data address register of the peripheral. */
} qDrvRTZ_ConstValues_t;

/**
 * @brief Macro for initializing const values required by a RTX driver.
 *
 *@param[in] ID Id number of the instance.
 */
#define Q_DRV_RTZ_CONST_VALUES_INIT(ID)                                                                                \
    {                                                                                                                  \
        .irqn = Q_REG_RTZ_IRQN_GET(ID),                                  /* Init IRQn. */                              \
        .dmaTriggerSelect = Q_CONCAT(qRegDMA_TriggerRTZ, ID, TxNotFull), /* Init DMA trigger select. */                \
        .txDataAddress = Q_REG_RTZ_TX_DATA_ADDRESS_GET(ID)               /* Init TX data address. */                   \
    }

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief Constant values of all RTZ instances. */
static const qDrvRTZ_ConstValues_t rtzConstValues[REG_RTZ_INSTANCE_COUNT] = {
    Q_DRV_RTZ_CONST_VALUES_INIT(0),
    Q_DRV_RTZ_CONST_VALUES_INIT(1),
};

/** @brief Array of pointers to already initialised RTZ instances. */
static qDrvRTZ_t* rtzInstances[REG_RTZ_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the interrupt number of the peripheral.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Interrupt number.
 */
static inline IRQn_Type qDrvRTZ_IRQnGet(qDrvRTZ_t* const pInstance)
{
    return rtzConstValues[pInstance->id].irqn;
}

/**
 * @brief Get the DMA source select of the peripheral.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Interrupt number.
 */
static inline UInt32 qDrvRTZ_DmaTriggerSelectGet(qDrvRTZ_t* const pInstance)
{
    return rtzConstValues[pInstance->id].dmaTriggerSelect;
}

/**
 * @brief Get the TX data address register of the peripheral. Can be used as DMA destination address.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Address register..
 */
static inline UInt32 qDrvRTZ_TxDataAddressGet(qDrvRTZ_t* const pInstance)
{
    return rtzConstValues[pInstance->id].txDataAddress;
}

/**
 * @brief Get the pointer to the control block.
 *
 * @param[in] pInstance Pointer to the driver instance structure.
 *
 * @retval Pointer to the control block
 */
static inline qDrvRTZ_ControlBlock_t* qDrvRTZ_ControlBlockGet(qDrvRTZ_t* const pInstance)
{
    return (qDrvRTZ_ControlBlock_t*)(&pInstance->internal);
}

/**
 * @brief Get the length (in bytes) of the buffer mode.
 *
 * @param[in] wordSize Word mode to be checked.
 *
 * @return Word mode length in bytes.
 */
static inline UInt8 qDrvRTZ_WordSizeToBytes(qRegRTZ_WordSize_t wordSize)
{
    switch(wordSize)
    {
        case qRegRTZ_WordSize8Bit:
            return 1;
        case qRegRTZ_WordSize16Bit:
            return 2;
        case qRegRTZ_WordSize32Bit:
            return 4;
        default:
            break;
    }

    return 0;
}

/**
 * @brief Get the DMA word mode from the buffer word mode.
 *
 * @param[in] wordSize Word mode to be checked.
 *
 * @return DMA word mode.
 */
static inline qRegDMA_WordMode_t qDrvRTZ_WordSizeToDmaWordMode(qRegRTZ_WordSize_t wordSize)
{
    switch(wordSize)
    {
        case qRegRTZ_WordSize8Bit:
            return qRegDMA_WordMode8Bit;
        case qRegRTZ_WordSize16Bit:
            return qRegDMA_WordMode16Bit;
        case qRegRTZ_WordSize32Bit:
            return qRegDMA_WordMode32Bit;
        default:
            break;
    }

    return (qRegDMA_WordMode_t)0xFF;
}

/**
 * @brief Get the symbol register value based on the time configuration.
 *
 * @param[in] timeConfig Time configuration in nanoseconds to be calculated.
 *
 * @return Calculated register value.
 */
static UInt32 qDrvRTZ_SymbolTimeToRegisterGet(Int32 timeConfig)
{
    if(timeConfig == Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED)
    {
        return 0UL;
    }

    /* Multiply time config to increase the calculation accuracy. */
    timeConfig *= 100;

    /* Divide by 31.25 ns. */
    UInt32 registerValue = (UInt32)qDrvCommon_DivideRoundClosest(timeConfig, 3125UL);
    return DRV_RTZ_VALUE_TO_REGISTER_GET(registerValue);
};

/**
 * @brief Get the reset register value based on the time configuration.
 *
 * @param[in] timeConfig Time configuration in nanoseconds to be calculated.
 *
 * @return Calculated register value.
 */
static UInt32 qDrvRTZ_ResetTimeToRegisterGet(Int32 timeConfig)
{
    if(timeConfig == Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED)
    {
        return 0UL;
    }

    /* Divide by 500 ns. */
    UInt32 registerValue = (UInt32)qDrvCommon_DivideRoundClosest(timeConfig, 500UL);
    return DRV_RTZ_VALUE_TO_REGISTER_GET(registerValue);
};

static inline void qDrvRTZ_TimingsConfigSet(qDrvRTZ_t* const pInstance, const qDrvRTZ_TimingsConfig_t* pTimingsConfig)
{
    qRegRTZ_t baseAddr = pInstance->baseAddr;

    qRegRTZ_HighTimeZeroSet(baseAddr, qDrvRTZ_SymbolTimeToRegisterGet(pTimingsConfig->t0H));
    qRegRTZ_LowTimeZeroSet(baseAddr, qDrvRTZ_SymbolTimeToRegisterGet(pTimingsConfig->t0L));
    qRegRTZ_HighTimeOneSet(baseAddr, qDrvRTZ_SymbolTimeToRegisterGet(pTimingsConfig->t1H));
    qRegRTZ_LowTimeOneSet(baseAddr, qDrvRTZ_SymbolTimeToRegisterGet(pTimingsConfig->t1L));
    qRegRTZ_HighTimeStopSet(baseAddr, qDrvRTZ_SymbolTimeToRegisterGet(pTimingsConfig->tStopH));
    qRegRTZ_LowTimeStopSet(baseAddr, qDrvRTZ_SymbolTimeToRegisterGet(pTimingsConfig->tStopL));
    qRegRTZ_HighTimeResetSet(baseAddr, qDrvRTZ_ResetTimeToRegisterGet(pTimingsConfig->tResetH));
    qRegRTZ_LowTimeResetSet(baseAddr, qDrvRTZ_ResetTimeToRegisterGet(pTimingsConfig->tResetL));

    /* Enable stop symbol if high and low time value are set. */
    Bool stopSymbolEnable = ((pTimingsConfig->tStopH != Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED) &&
                             (pTimingsConfig->tStopL != Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED));
    qRegRTZ_StopSymbolAfterTrailingDataEnableSet(baseAddr, stopSymbolEnable);
}

static inline void qDrvRTZ_FrameConfigSet(qDrvRTZ_t* const pInstance, const qDrvRTZ_FrameConfig_t* pFrameConfig)
{
    qRegRTZ_t baseAddr = pInstance->baseAddr;

    qRegRTZ_PixelsCountSet(baseAddr, DRV_RTZ_VALUE_TO_REGISTER_GET(pFrameConfig->pixelsCount));
    qRegRTZ_DataBitsSet(baseAddr, DRV_RTZ_VALUE_TO_REGISTER_GET(pFrameConfig->dataBits));
    qRegRTZ_TrailingDataBitsSet(baseAddr, DRV_RTZ_VALUE_TO_REGISTER_GET(pFrameConfig->trailingDataBits));

    qRegRTZ_LsbFirstEnableSet(baseAddr, pFrameConfig->bitOrder == qDrvRTZ_BitOrderLSBFirst ? true : false);
    qRegRTZ_WordSizeSet(baseAddr, pFrameConfig->wordSize);

    Bool trailingDataBitsEnable = pFrameConfig->trailingDataBits > 0UL;
    qRegRTZ_TrailingDataEnableSet(baseAddr, trailingDataBitsEnable);

    qRegRTZ_AlignPixelOnNextWordEnableSet(baseAddr, pFrameConfig->wordAlign);
}

static inline void qDrvRTZ_InvertConfigSet(qDrvRTZ_t* const pInstance, const qDrvRTZ_InvertConfig_t* pInvertConfig)
{
    qRegRTZ_t baseAddr = pInstance->baseAddr;

    qRegRTZ_ZeroOutputInvertEnableSet(baseAddr, pInvertConfig->zero);
    qRegRTZ_OneOutputInvertEnableSet(baseAddr, pInvertConfig->one);
    qRegRTZ_StopOutputInvertEnableSet(baseAddr, pInvertConfig->stop);
    qRegRTZ_ResetOutputInvertEnableSet(baseAddr, pInvertConfig->reset);
    qRegRTZ_PhysicalOutputInvertEnableSet(baseAddr, pInvertConfig->physical);
}

static inline void qDrvRTZ_JitterConfigSet(qDrvRTZ_t* const pInstance, const qDrvRTZ_JitterConfig_t* pJitterConfig)
{
    qRegRTZ_t baseAddr = pInstance->baseAddr;

    qRegRTZ_JitterMagnitudeBitsSet(baseAddr, pJitterConfig->magnitudeBits);
    qRegRTZ_TLowJitterEnableSet(baseAddr, pJitterConfig->tLow);
    qRegRTZ_THighJitterEnableSet(baseAddr, pJitterConfig->tHigh);
}

static void qDrvRTZ_UpdateTxData(qDrvRTZ_ControlBlock_t* controlBlock, qRegRTZ_t baseAddr)
{
    switch(controlBlock->wordSizeBytes)
    {
        case 1:
        {
            UInt8* pBuffer8 = (UInt8*)controlBlock->pBuffer;
            qRegRTZ_TxDataSet(baseAddr, pBuffer8[controlBlock->sampleIndex]);
            break;
        }
        case 2:
        {
            UInt16* pBuffer16 = (UInt16*)controlBlock->pBuffer;
            qRegRTZ_TxDataSet(baseAddr, pBuffer16[controlBlock->sampleIndex]);
            break;
        }
        case 4:
        {
            UInt32* pBuffer32 = (UInt32*)controlBlock->pBuffer;
            qRegRTZ_TxDataSet(baseAddr, pBuffer32[controlBlock->sampleIndex]);
            break;
        }
        default:
        {
            break;
        }
    }

    controlBlock->sampleIndex++;
}

static UInt16 qDrvRTZ_ExpectedBufferSizeGet(qDrvRTZ_t* const pInstance)
{
    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);
    qRegRTZ_t baseAddr = pInstance->baseAddr;

    UInt32 dataBytes = (qRegRTZ_DataBitsGet(baseAddr) + 1UL) / 8UL;
    UInt32 pixelCount = qRegRTZ_PixelsCountGet(baseAddr) + 1UL;

    if(qRegRTZ_AlignPixelOnNextWordEnableCheck(baseAddr) &&
       (qRegRTZ_BufferWordModeGet(baseAddr) != qRegRTZ_WordSize8Bit))
    {
        /* Alignment is important only for qRegRTZ_WordSize16Bit and qRegRTZ_WordSize32Bit word mode. */
        dataBytes += (dataBytes % 2UL);
    }

    UInt32 expectedBufferSize = pixelCount * dataBytes;

    /* Buffer size should be aligned to the word length. */
    expectedBufferSize += expectedBufferSize % controlBlock->wordSizeBytes;

    return expectedBufferSize;
}

static UInt16 qDrvRTZ_TransferSequencesGet(qDrvRTZ_t* const pInstance)
{
    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);

    return qDrvRTZ_ExpectedBufferSizeGet(pInstance) / controlBlock->wordSizeBytes;
}

static void qDrvRTZ_CbDmaTx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    (void)pChunk;
    (void)chunkSize;

    switch(eventType)
    {
        case qDrvDMA_EventTypeBufEmpty:
        {
            qDrvRTZ_ControlBlock_t* pControlBlock = (qDrvRTZ_ControlBlock_t*)arg;
            (void)qDrvDMA_ChannelDisable(pControlBlock->dmaChannel);
            pControlBlock->rtzTransmitting = false;

            if(pControlBlock->callback)
            {
                pControlBlock->callback(pControlBlock->arg, qDrvRTZ_EventComplete);
            }
            break;
        }
        default:
        {
            /* This should never happen with a valid DMA configuration. */
            Q_ASSERT(false);
            break;
        }
    }
}

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvRTZ_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qResult_t qDrvRTZ_QuicActionConfigSet(qRegQUIC_Channel_t quicChannel, qDrvQUIC_Internal_Instance_t* pInstance,
                                             void* pConfig, UInt32* pActionIdx)
{
    (void)pActionIdx;
    qDrvRTZ_t* const pRtzInstance = (qDrvRTZ_t*)pInstance;

    if(!qDrvRTZ_InitCheck(pRtzInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pRtzInstance);

    if(controlBlock->transferMode == qDrvRTZ_TransferModePolling)
    {
        /* Polling mode is not supported for QUIC action. */
        return Q_ERR_INVSTATE;
    }

    qDrvRTZ_QuicActionConfig_t* const pActionConfig = (qDrvRTZ_QuicActionConfig_t*)pConfig;

    if(pActionConfig->actionType != qDrvRTZ_QuicActionTypeTriggerSequence)
    {
        return Q_ERR_INVPARAM;
    }

    qRegRTZ_QuicActionTypeSet(pRtzInstance->baseAddr, qRegRTZ_QuicActionTypeTriggerSequence);
    qRegRTZ_QuicActionChannelSet(pRtzInstance->baseAddr, quicChannel);

    return Q_OK;
}

static qResult_t qDrvRTZ_QuicActionDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                              qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)actionIdx;
    qDrvRTZ_t* const pRtzInstance = (qDrvRTZ_t*)pInstance;

    if(!qDrvRTZ_InitCheck(pRtzInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_INVSTATE;
    }

    qRegRTZ_QuicActionTypeSet(pRtzInstance->baseAddr, qRegRTZ_QuicActionTypeNone);

    return Q_OK;
}

static qDrvQUIC_Internal_Action_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskAction,
    .apiSpeedGet = qDrvRTZ_QuicSpeedGet,
    .apiActionConfigSet = qDrvRTZ_QuicActionConfigSet,
    .apiActionDisconnect = qDrvRTZ_QuicActionDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvRTZ_PinConfigSet(const qDrvRTZ_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(&pPinConfig->tx, 1);
    if(res != Q_OK)
    {
        return res;
    }

    if(pPinConfig->tx.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->tx.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    return Q_OK;
}

qResult_t qDrvRTZ_PinConfigClear(const qDrvRTZ_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(&pPinConfig->tx, 1);
}

qResult_t qDrvRTZ_Init(qDrvRTZ_t* const pInstance, const qDrvRTZ_Config_t* const pConfig, const qDrvRTZ_Cb_t callback,
                       void* arg, UInt32 interruptPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_RTZ_INSTANCE_COUNT);
    Q_ASSERT(pConfig);

    if(qDrvRTZ_InitCheck(pInstance))
    {
        /* Instance is already initialized. */
        return Q_ERR_INVSTATE;
    }

    if((pConfig->frame.wordSize != qRegRTZ_WordSize8Bit) && (pConfig->frame.wordSize != qRegRTZ_WordSize16Bit) &&
       (pConfig->frame.wordSize != qRegRTZ_WordSize32Bit))
    {
        /* Wrong frame word size. */
        return Q_ERR_INVPARAM;
    }

    /* Clear the control block data. */
    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);
    memset((void*)controlBlock, 0, sizeof(qDrvRTZ_ControlBlock_t));

    /* Setup peripheral configuration. */
    qDrvRTZ_TimingsConfigSet(pInstance, &pConfig->timings);
    qDrvRTZ_FrameConfigSet(pInstance, &pConfig->frame);
    qDrvRTZ_InvertConfigSet(pInstance, &pConfig->invert);
    qDrvRTZ_JitterConfigSet(pInstance, &pConfig->jitter);

    /* Mark DMA channel as not allocated. */
    controlBlock->dmaChannel = Q_NOCHANNEL;

    if(pConfig->transferMode == qDrvRTZ_TransferModeDMA)
    {
        /* DMA channel allocation. */
        controlBlock->dmaChannel = qDrvDMA_Alloc();
        if(controlBlock->dmaChannel == Q_NOCHANNEL)
        {
            /* Unable to allocate DMA channel. */
            return Q_ERR_OUTOFMEMORY;
        }

        /* Initialize DMA channel. */
        qDrvDMA_ChannelConfig_t channelConfig = {
            .callback = &qDrvRTZ_CbDmaTx,
            .periAddress = qDrvRTZ_TxDataAddressGet(pInstance),
            .mode = qDrvDMA_ChannelModeMemToPeri,
            .bufferMode = qDrvDMA_BufferModeSingleShot,
            .wordMode = qDrvRTZ_WordSizeToDmaWordMode(pConfig->frame.wordSize),
            .dmaTriggerSelect = qDrvRTZ_DmaTriggerSelectGet(pInstance),
        };

        qResult_t result = qDrvDMA_ChannelInit(controlBlock->dmaChannel, &channelConfig, (void*)controlBlock);
        if(result != Q_OK)
        {
            /* Unable to initialize DMA channel. */
            return result;
        }
    }
    else if(pConfig->transferMode == qDrvRTZ_TransferModeInterrupt)
    {
        /* Enable global interrupts from the peripheral. */
        qRegIntCtrl_IRQEnable(qDrvRTZ_IRQnGet(pInstance));

        /* Clear event complete event. */
        qRegRTZ_IRQSequenceDoneClear(pInstance->baseAddr);

        /* Enable NVIC interrupts. */
        NVIC_ClearPendingIRQ(qDrvRTZ_IRQnGet(pInstance));
        NVIC_SetPriority(qDrvRTZ_IRQnGet(pInstance), interruptPriority);
        NVIC_EnableIRQ(qDrvRTZ_IRQnGet(pInstance));
    }

    controlBlock->transferMode = pConfig->transferMode;
    controlBlock->wordSizeBytes = qDrvRTZ_WordSizeToBytes(pConfig->frame.wordSize);

    /* Setup interrupt configuration. */
    controlBlock->callback = callback;
    controlBlock->arg = arg;

    rtzInstances[pInstance->id] = pInstance;

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}

qResult_t qDrvRTZ_Reconfigure(qDrvRTZ_t* const pInstance, const qDrvRTZ_Config_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_RTZ_INSTANCE_COUNT);
    Q_ASSERT(pConfig);

    if(!qDrvRTZ_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qRegRTZ_WordSize_t oldWordSize = qRegRTZ_BufferWordModeGet(pInstance->baseAddr);

    /* Setup peripheral configuration. */
    qDrvRTZ_TimingsConfigSet(pInstance, &pConfig->timings);
    qDrvRTZ_FrameConfigSet(pInstance, &pConfig->frame);
    qDrvRTZ_InvertConfigSet(pInstance, &pConfig->invert);
    qDrvRTZ_JitterConfigSet(pInstance, &pConfig->jitter);

    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);

    controlBlock->transferMode = pConfig->transferMode;
    controlBlock->wordSizeBytes = qDrvRTZ_WordSizeToBytes(pConfig->frame.wordSize);

    if((pConfig->transferMode == qDrvRTZ_TransferModeDMA) && (oldWordSize != pConfig->frame.wordSize))
    {
        qResult_t result = qDrvDMA_ChannelDeinit(controlBlock->dmaChannel);
        if(result != Q_OK)
        {
            /* Unable to deinitialize DMA channel. */
            return result;
        }

        /* Initialize DMA channel. */
        qDrvDMA_ChannelConfig_t channelConfig = {
            .callback = &qDrvRTZ_CbDmaTx,
            .periAddress = qDrvRTZ_TxDataAddressGet(pInstance),
            .mode = qDrvDMA_ChannelModeMemToPeri,
            .bufferMode = qDrvDMA_BufferModeSingleShot,
            .wordMode = qDrvRTZ_WordSizeToDmaWordMode(pConfig->frame.wordSize),
            .dmaTriggerSelect = qDrvRTZ_DmaTriggerSelectGet(pInstance),
        };

        result = qDrvDMA_ChannelInit(controlBlock->dmaChannel, &channelConfig, (void*)controlBlock);
        if(result != Q_OK)
        {
            /* Unable to initialize DMA channel. */
            return result;
        }
    }

    /* After reconfiguration, the new buffer should be applied. */
    controlBlock->pBuffer = NULL;

    return Q_OK;
}

qResult_t qDrvRTZ_Deinit(qDrvRTZ_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvRTZ_InitCheck(pInstance))
    {
        return Q_OK;
    }

    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);

    if(controlBlock->dmaChannel != Q_NOCHANNEL)
    {
        qDrvDMA_ChannelFree(controlBlock->dmaChannel);
        controlBlock->dmaChannel = Q_NOCHANNEL;
    }

    /* Disable NVIC interrupts. */
    NVIC_DisableIRQ(qDrvRTZ_IRQnGet(pInstance));

    /* Disable global interrupts from the peripheral. */
    qRegIntCtrl_IRQDisable(qDrvRTZ_IRQnGet(pInstance));

    /* Disable interrupt events from the peripheral. */
    qRegIntCtrl_EventIRQDisable(
        qDrvRTZ_IRQnGet(pInstance),
        (1UL << qDrvRTZ_EventTxNotFull) | (1UL << qDrvRTZ_EventComplete) | (1UL << qDrvRTZ_EventUnderrunError), 0);

    controlBlock->callback = NULL;

    rtzInstances[pInstance->id] = NULL;

    return Q_OK;
}

Bool qDrvRTZ_InitCheck(qDrvRTZ_t* const pInstance)
{
    return rtzInstances[pInstance->id] != NULL;
}

qResult_t qDrvRTZ_BufferSet(qDrvRTZ_t* const pInstance, void* buffer, UInt16 bufferSize)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(buffer);

    if(!qDrvRTZ_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);

    if(bufferSize == 0)
    {
        controlBlock->pBuffer = NULL;
        return Q_ERR_INVPARAM;
    }

    if(((UInt32)buffer % controlBlock->wordSizeBytes) != 0)
    {
        controlBlock->pBuffer = NULL;
        /* Memory address should be aligned to the word mode. */
        return Q_ERR_INVPARAM;
    }

    UInt32 expectedBufferSize = qDrvRTZ_ExpectedBufferSizeGet(pInstance);

    if((expectedBufferSize == 0) || (expectedBufferSize != bufferSize))
    {
        /* Expected buffer size is not equal to buffer size. */

        return Q_ERR_INVPARAM;
    }

    controlBlock->pBuffer = buffer;
    controlBlock->wordCount = bufferSize / controlBlock->wordSizeBytes;

    if(controlBlock->transferMode == qDrvRTZ_TransferModeDMA)
    {
        return qDrvDMA_BufferSet(controlBlock->dmaChannel, controlBlock->pBuffer, bufferSize);
    }

    return Q_OK;
}

qResult_t qDrvRTZ_PixelSet(qDrvRTZ_t* const pInstance, const void* const pPixelBuffer, UInt16 startPixelIndex,
                           UInt16 pixelCount)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pPixelBuffer);

    if(!qDrvRTZ_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qRegRTZ_t baseAddr = pInstance->baseAddr;
    UInt32 pixelsCount = qRegRTZ_PixelsCountGet(baseAddr) + 1UL;

    if(startPixelIndex >= pixelsCount)
    {
        /* Start pixel index is too large. */
        return Q_ERR_INVPARAM;
    }

    if((startPixelIndex + pixelCount) > pixelsCount)
    {
        /* Too many pixels to update. */
        return Q_ERR_INVPARAM;
    }

    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);
    if((controlBlock->pBuffer == NULL) || (controlBlock->wordCount == 0))
    {
        /* Memory buffer is not set. */
        return Q_ERR_GENERAL;
    }

    UInt32 dataBytes = (qRegRTZ_DataBitsGet(baseAddr) + 1UL) / 8UL;

    if(qRegRTZ_AlignPixelOnNextWordEnableCheck(baseAddr) &&
       (qRegRTZ_BufferWordModeGet(baseAddr) != qRegRTZ_WordSize8Bit))
    {
        /* Alignment is important only for qRegRTZ_WordSize16Bit and qRegRTZ_WordSize32Bit word mode. */
        dataBytes += (dataBytes % 2UL);
    }

    UInt8* pBuffer = (UInt8*)controlBlock->pBuffer;
    UInt8* pStartBuffer = &pBuffer[startPixelIndex * dataBytes];

    UInt16 byteCount = pixelCount * dataBytes;
    memcpy(pStartBuffer, pPixelBuffer, byteCount);

    return Q_OK;
}

qResult_t qDrvRTZ_Update(qDrvRTZ_t* const pInstance, Bool trigger)
{
    Q_ASSERT(pInstance);

    if(!qDrvRTZ_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);
    if((controlBlock->pBuffer == NULL) || (controlBlock->wordCount == 0))
    {
        /* Memory buffer is not set. */
        return Q_ERR_GENERAL;
    }

    if(qDrvRTZ_BusyCheck(pInstance) || (controlBlock->rtzTransmitting))
    {
        /* Instance is already transmitting the previous sequence. */
        return Q_ERR_BUSY;
    }

    qRegRTZ_t baseAddr = pInstance->baseAddr;
    qRegRTZ_TxDataBufferClear(baseAddr);
    qRegRTZ_IRQSequenceDoneClear(baseAddr);

    qResult_t result;
    if(controlBlock->transferMode == qDrvRTZ_TransferModeDMA)
    {
        result = qDrvDMA_Advance(controlBlock->dmaChannel, controlBlock->wordCount);
        if(result != Q_OK)
        {
            return result;
        }

        controlBlock->rtzTransmitting = true;

        result = qDrvDMA_ChannelEnable(controlBlock->dmaChannel, 0);
        if(result != Q_OK)
        {
            return result;
        }
    }

    if(controlBlock->transferMode == qDrvRTZ_TransferModeInterrupt)
    {
        controlBlock->sampleIndex = 0;
        controlBlock->rtzTransmitting = true;

        qDrvRTZ_UpdateTxData(controlBlock, baseAddr);

        /* Enable event complete event. */
        qRegIntCtrl_EventIRQEnable(qDrvRTZ_IRQnGet(pInstance),
                                   (1UL << qDrvRTZ_EventComplete) | (1UL << qDrvRTZ_EventTxNotFull), 0);
    }

    if(trigger)
    {
        UInt32 expectedTransferSequences = 0;
        if(controlBlock->transferMode == qDrvRTZ_TransferModePolling)
        {
            expectedTransferSequences = qDrvRTZ_TransferSequencesGet(pInstance);
            controlBlock->rtzTransmitting = true;
            qDrvRTZ_UpdateTxData(controlBlock, baseAddr);
        }

        qRegRTZ_SequenceTrigger(baseAddr);

        if(controlBlock->transferMode == qDrvRTZ_TransferModePolling)
        {
            for(UInt32 i = 1; i < expectedTransferSequences; i++)
            {
                while(!qRegRTZ_IRQUnmaskedTxNotFullCheck(baseAddr))
                {
                    ;
                }

                qDrvRTZ_UpdateTxData(controlBlock, baseAddr);
            }

            while(!qRegRTZ_IRQUnmaskedSequenceDoneCheck(baseAddr))
            {
                ;
            }
            controlBlock->rtzTransmitting = false;
        }
    }

    return Q_OK;
}

Bool qDrvRTZ_BusyCheck(qDrvRTZ_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvRTZ_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    qRegRTZ_t baseAddr = pInstance->baseAddr;

    return (Bool)(qRegRTZ_SequenceBusyCheck(baseAddr));
}

static void qDrvRTZ_InterruptHandler(UInt8 id)
{
    qDrvRTZ_t* pInstance = rtzInstances[id];
    qDrvRTZ_ControlBlock_t* controlBlock = qDrvRTZ_ControlBlockGet(pInstance);
    qRegRTZ_t baseAddr = pInstance->baseAddr;

    Q_ASSERT(pInstance);

    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(qDrvRTZ_IRQnGet(pInstance), &maskedEvents, NULL);

    if(maskedEvents & (1UL << qDrvRTZ_EventTxNotFull))
    {
        qDrvRTZ_UpdateTxData(controlBlock, baseAddr);
    }

    if(maskedEvents & (1UL << qDrvRTZ_EventComplete))
    {
        qRegIntCtrl_EventIRQDisable(qDrvRTZ_IRQnGet(pInstance), (1UL << qDrvRTZ_EventComplete), 0);
        qRegRTZ_IRQSequenceDoneClear(baseAddr);

        controlBlock->rtzTransmitting = false;
        if(controlBlock->callback)
        {
            controlBlock->callback(controlBlock->arg, qDrvRTZ_EventComplete);
        }
    }

    if(maskedEvents & (1UL << qDrvRTZ_EventUnderrunError))
    {
        qRegIntCtrl_EventIRQDisable(qDrvRTZ_IRQnGet(pInstance), (1UL << qDrvRTZ_EventUnderrunError), 0);
        qRegRTZ_IRQBufferUnderrunClear(baseAddr);
    }
}

void rtz_0_handler(void)
{
    qDrvRTZ_InterruptHandler(0);
}

void rtz_1_handler(void)
{
    qDrvRTZ_InterruptHandler(1);
}
