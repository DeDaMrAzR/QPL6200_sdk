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

#include "qRegHwInfo.h"
#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qRegSysRam.h"
#include "qDrvHRADC.h"
#include "qDrvDMA.h"

#include "gpHal_kx_Rap_Sens.h"

#define Q_DRV_ADC_IRQN HR_ADC_IRQn

#define Q_DRV_GAIN_TO_REF_VOLTAGE(gain)      /* */                                                                     \
    (gain == qRegHRADC_Gain1_4 ? (800 * 4) : /* */                                                                     \
         (gain == qRegHRADC_Gain1 ? (800) :  /* */                                                                     \
              (gain == qRegHRADC_Gain4 ? (800 / 4) : (800 / 8))))

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    const qDrvHRADC_Cbs_t* pCallbacks; /**< User defined callback handler. */
    void* arg;                         /**< User defined argument to be passed to the callback. */
    qDrvDMA_Channel_t dmaChannel;      /**< DMA channel to be used by the instance driver.  */
    Bool initialized;                  /**< True if the instance is initialized, false otherwise. */
    Bool fifoEnabled;                  /**< True if the fifo is enabled, false otherwise. */
    Bool irqCycleDone;                 /**< True if the interrupt cycle done is initialized, false otherwise. */
    UInt16 fifoSize;                   /**< Fifo sample size. */
    UInt16 fifoSampleCount;            /**< Current samples count. */
    UInt16 fifoTail;                   /**< Index of the fifo tail. */
    UInt16* fifoBuffer;                /**< Pointer to the fifo buffer. */
} qDrvHRADC_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvHRADC_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void qDrvHRADC_Enable(void)
{
    qRegHRADC_BiasLdoPowerUpEnableSet(true);
    qRegHRADC_LdoAnalogSupplyEnableSet(true);
    HAL_WAIT_US(10);

    qRegHRADC_MuxPowerUpEnableSet(true);
    qRegHRADC_PowerUpEnableSet(true);
    qRegHRADC_VrefBufferCircuitEnableSet(true);
    HAL_WAIT_US(10);
}

static void qDrvHRADC_Disable(void)
{
    qRegHRADC_BiasLdoPowerUpEnableSet(false);
    qRegHRADC_LdoAnalogSupplyEnableSet(false);
    qRegHRADC_MuxPowerUpEnableSet(false);
    qRegHRADC_PowerUpEnableSet(false);
    qRegHRADC_VrefBufferCircuitEnableSet(false);
}

static inline void qDrvHRADC_FifoInterruptEnableSet(qDrvHRADC_t* const pInstance)
{
    if(controlBlock.fifoEnabled == false && controlBlock.irqCycleDone == false)
    {
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, 1UL << qRegHRADC_IrqFifoNotEmpty, 0);
    }
    else
    {
        qRegIntCtrl_EventIRQEnable(Q_DRV_ADC_IRQN, 1UL << qRegHRADC_IrqFifoNotEmpty, 0);
    }
}

/**
 * @brief Flush data from internal fifo.
 *
 * @return True if there was a data in the fifo, false otherwise.
 */
static Bool qDrvHRADC_InternalFifoFlushSafe(void)
{
    if(controlBlock.fifoTail == controlBlock.fifoSampleCount)
    {
        /* Already precessed all received data. */
        return false;
    }

    UInt16 startIndex = controlBlock.fifoTail;
    UInt16 samplesInBuffer = 0;

    if(controlBlock.fifoTail > controlBlock.fifoSampleCount)
    {
        samplesInBuffer = controlBlock.fifoSize - controlBlock.fifoTail;
        controlBlock.fifoTail = 0;
    }

    if(controlBlock.fifoSampleCount > controlBlock.fifoTail)
    {
        samplesInBuffer = controlBlock.fifoSampleCount - controlBlock.fifoTail;
        controlBlock.fifoTail = controlBlock.fifoSampleCount;
    }

    controlBlock.pCallbacks->fifoThreshold(controlBlock.arg, &controlBlock.fifoBuffer[startIndex], samplesInBuffer);

    return true;
}

/**
 * @brief Flush data from DMA fifo.
 *
 * @return True if there was a data in the fifo, false otherwise.
 */
static Bool qDrvHRADC_DmaFifoFlushSafe(void)
{
    UInt16 wordCount;
    void* pChunk = qDrvDMA_ContinuousChunkGet(controlBlock.dmaChannel, &wordCount);

    if((pChunk == NULL) || (wordCount == 0UL))
    {
        /* Already precessed all received chunks. */
        return false;
    }

    if(controlBlock.pCallbacks && controlBlock.pCallbacks->fifoThreshold)
    {
        controlBlock.pCallbacks->fifoThreshold(controlBlock.arg, (UInt16*)pChunk, wordCount);
    }

    /* Release received data. */
    qDrvDMA_Advance(controlBlock.dmaChannel, wordCount);

    return true;
}

/**
 * @brief HRADC DMA RX transmission callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Pointer to the received data chunk.
 * @param[in] wordCount Number of words in the chunk.
 */
static void qDrvHRADC_CbDmaRx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 wordCount)
{
    (void)arg;
    switch(eventType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
            controlBlock.pCallbacks->fifoThreshold(controlBlock.arg, (UInt16*)pChunk, wordCount);
            return;
        case qDrvDMA_EventTypeBufOverflow:
            /* Not supported yet. */
            Q_ASSERT(false);
            return;
        default:
            /* This should never happen with a valid DMA configuration. */
            Q_ASSERT(false);
            return;
    }
}

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvHRADC_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qResult_t qDrvHRADC_QuicActionConfigSet(qRegQUIC_Channel_t quicChannel, qDrvQUIC_Internal_Instance_t* pInstance,
                                               void* pConfig, UInt32* pActionIdx)
{
    qDrvHRADC_t* const pHRADCInstance = (qDrvHRADC_t*)pInstance;

    if(!qDrvHRADC_InitCheck(pHRADCInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvHRADC_QuicActionConfig_t* const pActionConfig = (qDrvHRADC_QuicActionConfig_t*)pConfig;

    if((quicChannel >= REG_QUIC_CHANNEL_COUNT) || (pActionConfig->actionIdx >= REG_HRADC_QUIC_ACTION_COUNT))
    {
        return Q_ERR_INVPARAM;
    }

    if(pActionConfig->actionType == qRegHRADC_QuicActionTypeNone)
    {
        return Q_ERR_INVPARAM;
    }

    if((pActionConfig->actionType == qRegHRADC_QuicActionTypeSingle) && qRegHRADC_ContinuousMeasEnableGet())
    {
        /* Continuous mode already enabled. */
        return Q_ERR_INVSTATE;
    }

    *pActionIdx = pActionConfig->actionIdx;

    qRegHRADC_QuicActionTypeSet(pActionConfig->actionIdx, pActionConfig->actionType);
    qRegHRADC_QuicActionChannelSet(pActionConfig->actionIdx, quicChannel);

    return Q_OK;
}

static qResult_t qDrvHRADC_QuicActionDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                                qDrvQUIC_Internal_Instance_t* pInstance)
{
    qDrvHRADC_t* const pHRADCInstance = (qDrvHRADC_t*)pInstance;

    if(!qDrvHRADC_InitCheck(pHRADCInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegHRADC_QuicActionChannelGet(actionIdx) == quicChannel)
    {
        qRegHRADC_QuicActionTypeSet(actionIdx, qRegHRADC_QuicActionTypeNone);
    }

    return Q_OK;
}

static qDrvQUIC_Internal_Action_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskAction,
    .apiSpeedGet = qDrvHRADC_QuicSpeedGet,
    .apiActionConfigSet = qDrvHRADC_QuicActionConfigSet,
    .apiActionDisconnect = qDrvHRADC_QuicActionDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvHRADC_PinConfigSet(const qDrvIOB_PinAlt_t* const pinConfig)
{
    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pinConfig, 1);
    if(res != Q_OK)
    {
        return res;
    }

    qDrvIOB_ConfigInputSet(pinConfig->pin, qDrvIOB_PullNone, false);

    return Q_OK;
}

qResult_t qDrvHRADC_PinConfigClear(const qDrvIOB_PinAlt_t* const pPinConfig, UInt8 pinsCount)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig, pinsCount);
}

qResult_t qDrvHRADC_Init(qDrvHRADC_t* const pInstance, const qDrvHRADC_Config_t* const pConfig,
                         const qDrvHRADC_Cbs_t* const pCallbacks, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is already initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(pConfig->pChannel == qRegHRADC_ChannelNone)
    {
        /* P channel needs to be set. */
        return Q_ERR_INVPARAM;
    }

    if((pConfig->diffMode == false) && (pConfig->nChannel != qRegHRADC_ChannelNone) &&
       (pConfig->nChannel != pConfig->pChannel))
    {
        /* Wrong single ended mode configuration. */
        return Q_ERR_INVPARAM;
    }

    if(pConfig->diffMode == true &&
       ((pConfig->nChannel == qRegHRADC_ChannelNone) || (pConfig->nChannel == pConfig->pChannel)))
    {
        /* Wrong differential mode configuration. */
        return Q_ERR_INVPARAM;
    }

    qRegHwInfo_Package_t package = qRegHwInfo_PackageGet();
    UInt8 anioMask = qRegHwInfo_AnioMaskGet(package);

    if(!(anioMask & (1UL << pConfig->pChannel)) ||
       ((pConfig->nChannel != qRegHRADC_ChannelNone) && !(anioMask & (1UL << pConfig->nChannel))))
    {
        /* Selected channel is not available on this package. */
        return Q_ERR_NOTSUPPORTED;
    }

    if(pConfig->dma)
    {
        /* DMA channel allocation. */
        controlBlock.dmaChannel = qDrvDMA_Alloc();
        if(controlBlock.dmaChannel == Q_NOCHANNEL)
        {
            /* Unable to allocate DMA channel. */
            return Q_ERR_OUTOFMEMORY;
        }
    }
    else
    {
        controlBlock.dmaChannel = Q_NOCHANNEL;
    }

    /* Power up the ADC. */
    qDrvHRADC_Enable();

    qRegHRADC_ChannelPSelPupSet(false);
    qRegHRADC_ChannelNSelPupSet(false);

    qRegHRADC_AccuStartOverwriteEnableSet(pConfig->accuStartOverwrite);

    qRegHRADC_GainSet(pConfig->gain);
    qRegHRADC_HanningCyclesSet(pConfig->hanningCycles);
    qRegHRADC_SampleDelaySet(pConfig->sampleDelay);
    qRegHRADC_ChannelPSet(pConfig->pChannel);

    qRegHRADC_ChannelPSelPupSet(true);

    if(pConfig->nChannel != qRegHRADC_ChannelNone)
    {
        /* Enable negative pin in differential mode. */
        qRegHRADC_ChannelNSet(pConfig->nChannel);
        qRegHRADC_ChannelNSelPupSet(true);
    }

    if(pCallbacks)
    {
        /* Setup interrupt configuration. */
        controlBlock.pCallbacks = pCallbacks;
        controlBlock.arg = arg;

        /* Enable global interrupts from the peripheral. */
        qRegIntCtrl_IRQEnable(Q_DRV_ADC_IRQN);

        /* Enable NVIC interrupts. */
        NVIC_ClearPendingIRQ(Q_DRV_ADC_IRQN);
        NVIC_SetPriority(Q_DRV_ADC_IRQN, irqPriority);
        NVIC_EnableIRQ(Q_DRV_ADC_IRQN);
    }

    controlBlock.initialized = true;

    qSysRam_RetainEnableSet(Q_REG_SYSRAM_RETAIN_GET(HR_ADC), true);

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}

qResult_t qDrvHRADC_Deinit(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        return Q_OK;
    }

    (void)qDrvHRADC_ContinuousStop(pInstance);
    (void)qDrvHRADC_ContinuousConfigSet(pInstance, false);

    if(controlBlock.pCallbacks)
    {
        /* Disable NVIC interrupts. */
        NVIC_DisableIRQ(Q_DRV_ADC_IRQN);

        /* Disable global interrupts from the peripheral. */
        qRegIntCtrl_IRQDisable(Q_DRV_ADC_IRQN);

        /* Disable interrupt events from the peripheral. */
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, 0xFFFFFFFF, 0);

        controlBlock.pCallbacks = NULL;
    }

    if(controlBlock.dmaChannel != Q_NOCHANNEL)
    {
        (void)qDrvDMA_ChannelFree(controlBlock.dmaChannel);
        controlBlock.dmaChannel = Q_NOCHANNEL;
    }

    qRegHRADC_ChannelPSelPupSet(false);
    qRegHRADC_ChannelNSelPupSet(false);

    /* Power down the ADC. */
    qDrvHRADC_Disable();

    controlBlock.initialized = false;

    qSysRam_RetainEnableSet(Q_REG_SYSRAM_RETAIN_GET(HR_ADC), false);

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = NULL;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}

Bool qDrvHRADC_InitCheck(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    return controlBlock.initialized;
}

qResult_t qDrvHRADC_FifoConfigSet(qDrvHRADC_t* const pInstance, qDrvHRADC_FifoConfig_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);
    Q_ASSERT(pConfig->buffer);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qDrvHRADC_FifoEnableCheck(pInstance))
    {
        /* Fifo is already enabled. */
        return Q_ERR_INVSTATE;
    }

    if(pConfig->bufferSize == 0)
    {
        /* Wrong buffer length. */
        return Q_ERR_INVPARAM;
    }

    if(pConfig->bufferSize % 2)
    {
        /* Buffer length not aligned to fifo resolution. */
        return Q_ERR_INVPARAM;
    }

    if((controlBlock.pCallbacks == NULL) || (controlBlock.pCallbacks->fifoThreshold == NULL))
    {
        /* Fifo callback function is not set. */
        return Q_ERR_GENERAL;
    }

    controlBlock.fifoBuffer = pConfig->buffer;
    controlBlock.fifoSize = pConfig->bufferSize / (sizeof(pConfig->buffer[0]));

    if(controlBlock.dmaChannel != Q_NOCHANNEL)
    {
        /* Mark interrupt mode unused. */
        controlBlock.fifoBuffer = NULL;

        qDrvDMA_ChannelConfig_t dmaConfig = {
            .callback = qDrvHRADC_CbDmaRx,
            .periAddress = Q_REG_HRADC_FIFO_ADDRESS_GET(),
            .mode = qDrvDMA_ChannelModePeriToMem,
            .bufferMode = qDrvDMA_BufferModeCircular,
            .wordMode = qRegDMA_WordMode16Bit,
            .dmaTriggerSelect = GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_HR_ADC_FIFO_NOT_EMPTY,
        };

        qResult_t result = qDrvDMA_ChannelInit(controlBlock.dmaChannel, &dmaConfig, NULL);
        if(result != Q_OK)
        {
            qDrvDMA_ChannelFree(controlBlock.dmaChannel);

            controlBlock.dmaChannel = Q_NOCHANNEL;
            return result;
        }

        result = qDrvDMA_BufferSet(controlBlock.dmaChannel, pConfig->buffer, pConfig->bufferSize);
        if(result != Q_OK)
        {
            return result;
        }
    }

    return Q_OK;
}

qResult_t qDrvHRADC_FifoEnable(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qDrvHRADC_FifoEnableCheck(pInstance))
    {
        /* Fifo is already enabled. */
        return Q_ERR_INVSTATE;
    }

    if(controlBlock.dmaChannel != Q_NOCHANNEL)
    {
        qResult_t result = qDrvDMA_ChannelEnable(controlBlock.dmaChannel, controlBlock.fifoSize / 2UL);
        if(result != Q_OK)
        {
            return result;
        }
    }
    else
    {
        if(controlBlock.fifoBuffer == NULL || controlBlock.fifoSize == 0)
        {
            /* Fifo buffer is not set. */
            return Q_ERR_GENERAL;
        }

        controlBlock.fifoSampleCount = 0;
        controlBlock.fifoTail = 0;

        controlBlock.fifoEnabled = true;

        qDrvHRADC_FifoInterruptEnableSet(pInstance);
    }

    return Q_OK;
}

qResult_t qDrvHRADC_FifoDisable(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvHRADC_FifoEnableCheck(pInstance))
    {
        /* Fifo is already disabled. */
        return Q_ERR_INVSTATE;
    }

    if(controlBlock.dmaChannel != Q_NOCHANNEL)
    {
        qResult_t result = qDrvDMA_ChannelDisable(controlBlock.dmaChannel);
        if(result != Q_OK)
        {
            return result;
        }
    }
    else
    {
        controlBlock.fifoEnabled = false;
        qDrvHRADC_FifoInterruptEnableSet(pInstance);
    }

    return Q_OK;
}

Bool qDrvHRADC_FifoEnableCheck(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    if(controlBlock.dmaChannel != Q_NOCHANNEL)
    {
        return qDrvDMA_ChannelEnabledCheck(controlBlock.dmaChannel);
    }
    else
    {
        return controlBlock.fifoEnabled;
    }

    return false;
}

qResult_t qDrvHRADC_FifoFlush(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    const qDrvHRADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

    if((pCallbacks == NULL) || (pCallbacks->fifoThreshold == NULL))
    {
        /* Fifo threshold callback function is not set. */
        return Q_ERR_GENERAL;
    }

    if((controlBlock.dmaChannel == Q_NOCHANNEL) && (controlBlock.fifoBuffer == NULL))
    {
        /* Fifo buffer is not set. */
        return Q_ERR_INVSTATE;
    }

    Bool (*flushFunction)(void) =
        (controlBlock.dmaChannel != Q_NOCHANNEL) ? qDrvHRADC_DmaFifoFlushSafe : qDrvHRADC_InternalFifoFlushSafe;

    HAL_DISABLE_GLOBAL_INT();
    if(flushFunction() == false)
    {
        /* All data is already proceed.  */
        HAL_ENABLE_GLOBAL_INT();
        return Q_ERR_OUTOFMEMORY;
    }
    HAL_ENABLE_GLOBAL_INT();

    return Q_OK;
}

qResult_t qDrvHRADC_AccumulateConfigSet(qDrvHRADC_t* const pInstance, UInt32 accCount, Bool irqCycleDone)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(accCount == 0 || accCount >= GP_WB_HR_ADC_ACCU_COUNT_MASK)
    {
        /* Wrong accumulation count value. */
        return Q_ERR_INVPARAM;
    }

    if(qRegHRADC_AccuResultOngoingGet())
    {
        /* Ongoing accumulation. */
        return Q_ERR_INVSTATE;
    }

    const qDrvHRADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

    qRegHRADC_AccuCountSet(accCount - 1);
    qRegHRADC_IRQAccuDoneClear();

    if(irqCycleDone)
    {
        if((pCallbacks == NULL) || (pCallbacks->accuDone == NULL))
        {
            /* Accumulate done callback function is not set. */
            return Q_ERR_GENERAL;
        }

        qRegIntCtrl_EventIRQEnable(Q_DRV_ADC_IRQN, 1UL << qRegHRADC_IrqAccuDone, 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, 1UL << qRegHRADC_IrqAccuDone, 0);
    }

    return Q_OK;
}

qResult_t qDrvHRADC_AccumulateStart(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegHRADC_AccuCountGet() == 0)
    {
        /* Accumulation count not set. */
        return Q_ERR_INVSTATE;
    }

    if(qRegHRADC_AccuResultOngoingGet() && !qRegHRADC_AccuStartOverwriteEnableGet())
    {
        /* Ongoing accumulation and restart not enabled. */
        return Q_ERR_INVSTATE;
    }

    for(UInt8 actionIdx = 0; actionIdx < REG_HRADC_QUIC_ACTION_COUNT; actionIdx++)
    {
        if(qRegHRADC_QuicActionTypeGet(actionIdx) == qRegHRADC_QuicActionTypeAccuStart)
        {
            /* Unable to start accumulation. Quic already used. */
            return Q_ERR_BUSY;
        }
    }

    qRegHRADC_AccuStart();

    return Q_OK;
}

Bool qDrvHRADC_AccumulateValidCheck(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    return qRegHRADC_AccuResultValidGet();
}

Bool qDrvHRADC_AccumulateReadyCheck(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    return qRegHRADC_IRQUnmaskedAccuDoneCheck();
}

qResult_t qDrvHRADC_ContinuousConfigSet(qDrvHRADC_t* const pInstance, Bool irqCycleDone)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    const qDrvHRADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

    if(irqCycleDone)
    {
        if((pCallbacks == NULL) || (pCallbacks->cycleDone == NULL))
        {
            controlBlock.irqCycleDone = false;
            /* Accumulate done callback function is not set. */
            return Q_ERR_GENERAL;
        }

        (void)qRegHRADC_ResultGet();
    }
    controlBlock.irqCycleDone = irqCycleDone;

    qDrvHRADC_FifoInterruptEnableSet(pInstance);

    return Q_OK;
}

qResult_t qDrvHRADC_ContinuousStart(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegHRADC_ContinuousMeasEnableGet())
    {
        /* Continuous mode already enabled. */
        return Q_ERR_INVSTATE;
    }

    for(UInt8 actionIdx = 0; actionIdx < REG_HRADC_QUIC_ACTION_COUNT; actionIdx++)
    {
        if(qRegHRADC_QuicActionTypeGet(actionIdx) == qRegHRADC_QuicActionTypeSingle)
        {
            /* Unable to start continuous mode. Quic mode already used. */
            return Q_ERR_BUSY;
        }
    }

    qRegHRADC_ContinuousMeasEnableSet(true);

    return Q_OK;
}

qResult_t qDrvHRADC_ContinuousStop(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(!qRegHRADC_ContinuousMeasEnableGet())
    {
        /* Continuous mode already disabled. */
        return Q_ERR_INVSTATE;
    }

    qRegHRADC_ContinuousMeasEnableSet(false);

    return Q_OK;
}

Bool qDrvHRADC_ContinuousEnableGet(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    return qRegHRADC_ContinuousMeasEnableGet();
}

Bool qDrvHRADC_ResultReadyCheck(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    return qRegHRADC_IRQUnmaskedFifoNotEmptyCheck();
}

UInt32 qDrvHRADC_RawResultGet(qDrvHRADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    if(qRegHRADC_ContinuousMeasEnableGet() == false)
    {
        /* Continuous mode already disabled. */
        return 0;
    }

    return qRegHRADC_ResultGet();
}

qDrvHRADC_Voltage_t qDrvHRADC_RawToVoltageConvert(qDrvHRADC_t* const pInstance, UInt32 raw)
{
    Q_ASSERT(pInstance);

    qDrvHRADC_Voltage_t voltage = {
        .negative = false,
        .integer = 0,
        .fractional = 0,
    };

    if(!qDrvHRADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return voltage;
    }

    Int64 outVoltage = rap_sens_hr_adc_code_to_uv(raw, qRegHRADC_GainGet());

    if(outVoltage < 0)
    {
        voltage.negative = true;
        outVoltage = -outVoltage;
    }
    outVoltage /= 1000LL; /* Convert uV to mV. */

    voltage.integer = outVoltage / 1000LL;
    voltage.fractional = outVoltage % 1000LL;

    return voltage;
}

void hr_adc_handler(void)
{
    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(Q_DRV_ADC_IRQN, &maskedEvents, NULL);

    const qDrvHRADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

    if(pCallbacks == NULL)
    {
        /* Disable NVIC interrupts. */
        NVIC_DisableIRQ(Q_DRV_ADC_IRQN);

        /* Disable global interrupts from the peripheral. */
        qRegIntCtrl_IRQDisable(Q_DRV_ADC_IRQN);
    }

    if(maskedEvents & (1UL << qRegHRADC_IrqAccuDone))
    {
        UInt32 result = qRegHRADC_AccuResultGet();

        qRegHRADC_IRQAccuDoneClear();

        pCallbacks->accuDone(controlBlock.arg, result);
    }

    if(maskedEvents & (1UL << qRegHRADC_IrqFifoNotEmpty))
    {
        UInt32 result = qRegHRADC_ResultGet();

        if(controlBlock.irqCycleDone)
        {
            pCallbacks->cycleDone(controlBlock.arg, result);
        }

        UInt16* fifoBuffer = controlBlock.fifoBuffer;

        if(fifoBuffer && controlBlock.fifoEnabled)
        {
            fifoBuffer[controlBlock.fifoSampleCount++] = result;

            if(controlBlock.fifoSampleCount == (controlBlock.fifoSize / 2UL))
            {
                (void)qDrvHRADC_InternalFifoFlushSafe();
            }

            if(controlBlock.fifoSampleCount == controlBlock.fifoSize)
            {
                controlBlock.fifoSampleCount = 0;
                (void)qDrvHRADC_InternalFifoFlushSafe();
            }
        }
    }

    if(maskedEvents & (1UL << qRegHRADC_IrqOverVoltage))
    {
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, 1UL << qRegHRADC_IrqOverVoltage, 0);

        qRegHRADC_IRQOverVoltageClear();
    }
}
