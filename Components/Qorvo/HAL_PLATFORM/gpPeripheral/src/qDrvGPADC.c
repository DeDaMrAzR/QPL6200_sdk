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
#include "qDrvDMA.h"
#include "qDrvGPADC.h"

#include "gpHal_kx_Rap_Sens.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define Q_DRV_ADC_IRQN GP_ADC_IRQn

#define Q_DRV_GP_ADC_SLOT_MASK ((1UL << REG_GPADC_SLOT_COUNT) - 1UL)

#define Q_DRV_GP_ADC_REF_MV (970UL)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qRegGPADC_ConfiguredMask_t */
//@{
#define qRegGPADC_ConfiguredMaskSlotA    BM(0UL)
#define qRegGPADC_ConfiguredMaskSlotB    BM(1UL)
#define qRegGPADC_ConfiguredMaskSlotC    BM(2UL)
#define qRegGPADC_ConfiguredMaskSlotD    BM(3UL)
#define qRegGPADC_ConfiguredMaskAwdAbove BM(4UL)
#define qRegGPADC_ConfiguredMaskAwdBelow BM(5UL)
/** @typedef qRegGPADC_ConfiguredMask_t
 *  @brief GPADC driver configured mask.
 */
typedef UInt8 qRegGPADC_ConfiguredMask_t;
//@}

/** @brief GPADC instance control block data. */
typedef struct {
    const qDrvGPADC_Cbs_t* pCallbacks;         /**< User defined callback handler. */
    void* arg;                                 /**< User defined argument to be passed to the callback. */
    qDrvDMA_Channel_t dmaChannel;              /**< DMA channel to be used by the instance driver.  */
    Bool initialized;                          /**< True if the instance is initialized, false otherwise. */
    qRegGPADC_ConfiguredMask_t configuredMask; /**< GPADC driver configured mask. */
    UInt8 fifoSampleSize;                      /**< Byte size of one sample. */
    UInt16 fifoSize;                           /**< Fifo sample size. */
    UInt16 fifoSampleCount;                    /**< Current samples count. */
    UInt16 fifoTail;                           /**< Index of the fifo tail. */
    void* fifoBuffer;                          /**< Pointer to the fifo buffer. */
    qDrvGPADC_PostBuffer_t slotPostBufferMask[REG_GPADC_SLOT_COUNT]; /**< Saved post buffer mask. */
} qDrvGPADC_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvGPADC_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Set the slot post buffer configuration.
 *
 * @param[in] slot       ADC input slot number.
 * @param[in] postBuffer ADC post buffer.
 */
static void qDrvGPADC_SlotConfigPostBuffer(qRegGPADC_Slot_t slot, qDrvGPADC_PostBuffer_t postBuffer)
{
    qRegGPADC_SlotPostResultToBufEnableSet(slot, qRegGPADC_BufferA, (postBuffer & qDrvGPADC_PostBufferA));
    qRegGPADC_SlotPostResultToBufEnableSet(slot, qRegGPADC_BufferB, (postBuffer & qDrvGPADC_PostBufferB));
    qRegGPADC_SlotPostResultToBufEnableSet(slot, qRegGPADC_BufferC, (postBuffer & qDrvGPADC_PostBufferC));
    qRegGPADC_SlotPostResultToFifoEnableSet(slot, (postBuffer & qDrvGPADC_PostBufferFifo));
    qRegGPADC_SlotPostResultToWdgEnableSet(slot, (postBuffer & qDrvGPADC_PostBufferAwd));
}

/**
 * @brief Flush data from internal fifo.
 *
 * @return True if there was a data in the fifo, false otherwise.
 */
static Bool qDrvGPADC_InternalFifoFlushSafe(void)
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

    void* fifoBuffer = (void*)(&((UInt8*)controlBlock.fifoBuffer)[startIndex * controlBlock.fifoSampleSize]);
    controlBlock.pCallbacks->fifoThreshold(controlBlock.arg, fifoBuffer, samplesInBuffer);

    return true;
}

/**
 * @brief Flush data from DMA fifo.
 *
 * @return True if there was a data in the fifo, false otherwise.
 */
static Bool qDrvGPADC_DmaFifoFlushSafe(void)
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
        controlBlock.pCallbacks->fifoThreshold(controlBlock.arg, pChunk, wordCount);
    }

    /* Release received data. */
    qDrvDMA_Advance(controlBlock.dmaChannel, wordCount);

    return true;
}

/**
 * @brief GPADC DMA RX transmission callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Pointer to the already received chunk of data.
 * @param[in] wordCount Number of words in the chunk.
 */
static void qDrvGPADC_CbDmaRx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 wordCount)
{
    (void)arg;
    switch(eventType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
            controlBlock.pCallbacks->fifoThreshold(controlBlock.arg, pChunk, wordCount);
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

/** @brief Enable GPADC peripheral. */
static void qDrvGPADC_Enable(void)
{
    qRegGPADC_LdoAnalogSupplyEnableSet(true);
    qRegGPADC_LdoBleedEnableSet(true);
    qRegGPADC_VcmBufferPowerUpEnableSet(true);
    HAL_WAIT_US(10);

    qRegGPADC_VrefBufferCircuitEnableSet(true);
    HAL_WAIT_US(10);

    qRegGPADC_PowerUpEnableSet(true);
}

/** @brief Disable GPADC peripheral. */
static void qDrvGPADC_Disable(void)
{
    qRegGPADC_PowerUpEnableSet(false);
    qRegGPADC_VrefBufferCircuitEnableSet(false);
    qRegGPADC_VcmBufferPowerUpEnableSet(false);
    qRegGPADC_LdoBleedEnableSet(false);
    qRegGPADC_LdoAnalogSupplyEnableSet(false);
}

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvGPADC_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qResult_t qDrvGPADC_QuicActionConfigSet(qRegQUIC_Channel_t quicChannel, qDrvQUIC_Internal_Instance_t* pInstance,
                                               void* pConfig, UInt32* pActionIdx)
{
    qDrvGPADC_t* const pGpAdcInstance = (qDrvGPADC_t*)pInstance;

    if(!qDrvGPADC_InitCheck(pGpAdcInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvGPADC_QuicActionConfig_t* const pActionConfig = (qDrvGPADC_QuicActionConfig_t*)pConfig;

    if((quicChannel >= REG_QUIC_CHANNEL_COUNT) || (pActionConfig->actionIdx >= REG_GPADC_QUIC_ACTION_COUNT))
    {
        return Q_ERR_INVPARAM;
    }

    if(pActionConfig->actionType != qRegGPADC_QuicActionTypeDoMeasurement)
    {
        return Q_ERR_INVPARAM;
    }

    *pActionIdx = pActionConfig->actionIdx;

    qRegGPADC_QuicActionTypeSet(pActionConfig->actionIdx, qRegGPADC_QuicActionTypeDoMeasurement);
    qRegGPADC_QuicActionChannelSet(pActionConfig->actionIdx, quicChannel);

    qRegGPADC_TriggerModeSet(qRegGPADC_TriggerModeNoneOrQuic);

    return Q_OK;
}

static qResult_t qDrvGPADC_QuicActionDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                                qDrvQUIC_Internal_Instance_t* pInstance)
{
    qDrvGPADC_t* const pGpAdcInstance = (qDrvGPADC_t*)pInstance;

    if(!qDrvGPADC_InitCheck(pGpAdcInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegGPADC_QuicActionChannelGet(actionIdx) == quicChannel)
    {
        qRegGPADC_QuicActionTypeSet(actionIdx, qRegGPADC_QuicActionTypeNone);
    }

    return Q_OK;
}

static qDrvQUIC_Internal_Action_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskAction,
    .apiSpeedGet = qDrvGPADC_QuicSpeedGet,
    .apiActionConfigSet = qDrvGPADC_QuicActionConfigSet,
    .apiActionDisconnect = qDrvGPADC_QuicActionDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvGPADC_PinConfigSet(const qDrvIOB_PinAlt_t* const pPinConfig, UInt8 pinCount)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pPinConfig, pinCount);
    if(res != Q_OK)
    {
        return res;
    }

    for(UInt8 i = 0; i < pinCount; i++)
    {
        qDrvIOB_ConfigInputSet(pPinConfig[i].pin, qDrvIOB_PullNone, false);
    }

    return Q_OK;
}

qResult_t qDrvGPADC_PinConfigClear(const qDrvIOB_PinAlt_t* const pPinConfig, UInt8 pinsCount)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig, pinsCount);
}

qResult_t qDrvGPADC_Init(qDrvGPADC_t* const pInstance, const qDrvGPADC_Config_t* const pConfig,
                         const qDrvGPADC_Cbs_t* const pCallbacks, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is already initialized. */
        return Q_ERR_UNINITIALIZED;
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

    /* Power up the ADC. */
    qDrvGPADC_Enable();

    qRegGPADC_SlotMaskEnableSet(Q_DRV_GP_ADC_SLOT_MASK, false);
    memset(controlBlock.slotPostBufferMask, 0, sizeof(qDrvGPADC_PostBuffer_t) * REG_GPADC_BUFFER_COUNT);

    controlBlock.initialized = true;

    qSysRam_RetainEnableSet(qRegSysRam_RetainGPADC, true);

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}

qResult_t qDrvGPADC_Deinit(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        return Q_OK;
    }

    (void)qDrvGPADC_ContinuousStop(pInstance);

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

    (void)qDrvGPADC_SlotDisable(pInstance, qRegGPADC_SlotA);
    (void)qDrvGPADC_SlotDisable(pInstance, qRegGPADC_SlotB);
    (void)qDrvGPADC_SlotDisable(pInstance, qRegGPADC_SlotC);
    (void)qDrvGPADC_SlotDisable(pInstance, qRegGPADC_SlotD);

    (void)qDrvGPADC_AwdDisable(pInstance);
    (void)qDrvGPADC_FifoDisable(pInstance);

    if(controlBlock.dmaChannel != Q_NOCHANNEL)
    {
        (void)qDrvDMA_ChannelFree(controlBlock.dmaChannel);
        controlBlock.dmaChannel = Q_NOCHANNEL;
    }

    controlBlock.configuredMask = 0;

    controlBlock.fifoBuffer = NULL;
    controlBlock.fifoSize = 0;
    controlBlock.fifoSampleSize = 0;
    controlBlock.fifoTail = 0;

    /* Power down the ADC. */
    qDrvGPADC_Disable();

    controlBlock.initialized = false;

    qSysRam_RetainEnableSet(qRegSysRam_RetainGPADC, false);

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = NULL;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}

Bool qDrvGPADC_InitCheck(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    return controlBlock.initialized;
}

qResult_t qDrvGPADC_SlotEnable(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(slot < REG_GPADC_SLOT_COUNT);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    UInt32 slotMask = 1UL << (UInt32)slot;

    if(qRegGPADC_SlotMaskEnableGet() & slotMask)
    {
        /* Slot is already enabled. */
        return Q_ERR_INVSTATE;
    }

    if(!(controlBlock.configuredMask & slotMask))
    {
        /* Slot is not configured. */
        return Q_ERR_INVSTATE;
    }

    qRegGPADC_SlotMaskEnableSet(slotMask, true);

    qDrvGPADC_PostBuffer_t postBuffer = controlBlock.slotPostBufferMask[(UInt8)slot];
    if(postBuffer)
    {
        qDrvGPADC_SlotConfigPostBuffer(slot, postBuffer);
    }

    return Q_OK;
}

qResult_t qDrvGPADC_SlotDisable(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(slot < REG_GPADC_SLOT_COUNT);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    UInt32 slotMask = 1UL << (UInt32)slot;

    if((qRegGPADC_SlotMaskEnableGet() & slotMask) == 0UL)
    {
        /* Slot is already disabled. */
        return Q_ERR_INVSTATE;
    }

    qDrvGPADC_SlotConfigPostBuffer(slot, (qDrvGPADC_PostBuffer_t)0UL);

    qRegGPADC_SlotMaskEnableSet(slotMask, false);

    return Q_OK;
}

Bool qDrvGPADC_SlotEnableCheck(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot)
{
    (void)pInstance;

    return (qRegGPADC_SlotMaskEnableGet() & (1UL << (UInt32)slot)) ? true : false;
}

qResult_t qDrvGPADC_SlotConfigSet(qDrvGPADC_t* const pInstance, qRegGPADC_Slot_t slot,
                                  qDrvGPADC_SlotConfig_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(slot < REG_GPADC_SLOT_COUNT);
    Q_ASSERT(pConfig);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(pConfig->pChannel == qRegGPADC_ChannelNone)
    {
        /* P channel needs to be set. */
        controlBlock.slotPostBufferMask[(UInt8)slot] = 0UL;
        return Q_ERR_INVPARAM;
    }

    Bool singleEndedMode = (pConfig->pChannel == pConfig->nChannel) || (pConfig->nChannel == qRegGPADC_ChannelNone);

    if(pConfig->diffMode && singleEndedMode)
    {
        /* Wrong differential mode configuration.  */
        return Q_ERR_INVPARAM;
    }

    if((singleEndedMode == false) && pConfig->higherSpeed)
    {
        /* High speed config is possible only in single ended mode. */
        return Q_ERR_INVPARAM;
    }

    qRegHwInfo_Package_t package = qRegHwInfo_PackageGet();
    UInt8 anioMask = qRegHwInfo_AnioMaskGet(package);

    if(!(anioMask & (1UL << pConfig->pChannel)) ||
       ((singleEndedMode == false) && !(anioMask & (1UL << pConfig->nChannel))))
    {
        /* Channel is not available in the package. */
        return Q_ERR_INVPARAM;
    }

    if(qDrvGPADC_SlotEnableCheck(pInstance, slot) == true)
    {
        /* Slot needs to be disabled before configuration. */
        return Q_ERR_INVSTATE;
    }

    qRegGPADC_SlotChannelPSet(slot, pConfig->pChannel);
    qRegGPADC_SlotChannelNSet(slot, singleEndedMode ? pConfig->pChannel : pConfig->nChannel);
    qRegGPADC_SlotChannelPSelPupSet(slot, true);
    qRegGPADC_SlotChannelNSelPupSet(slot, true);

    qRegGPADC_SlotWaitTimeSet(slot, pConfig->waitTime);
    qRegGPADC_SlotLowVoltageModeEnableSet(slot, (pConfig->voltageMode == qDrvGPADC_VoltageModeLow));
    qRegGPADC_SlotSeToDeModeEnableSet(slot, singleEndedMode);
    qRegGPADC_SlotBypassResistorEnableSet(slot, pConfig->higherSpeed);
    qDrvGPADC_SlotConfigPostBuffer(slot, pConfig->postBuffer);

    controlBlock.slotPostBufferMask[(UInt8)slot] = pConfig->postBuffer;
    controlBlock.configuredMask |= 1UL << (UInt32)slot;

    return Q_OK;
}

qResult_t qDrvGPADC_BufferConfigSet(qDrvGPADC_t* const pInstance, qRegGPADC_Buffer_t buffer,
                                    qDrvGPADC_BufferConfig_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(buffer < REG_GPADC_BUFFER_COUNT);
    Q_ASSERT(pConfig);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    UInt32 irq;
    switch(buffer)
    {
        case qRegGPADC_BufferA:
            irq = qRegGPADC_IrqBufferAUpdated;
            break;
        case qRegGPADC_BufferB:
            irq = qRegGPADC_IrqBufferBUpdated;
            break;
        case qRegGPADC_BufferC:
            irq = qRegGPADC_IrqBufferCUpdated;
            break;
        default:
            /* Unsupported buffer. */
            return Q_ERR_INVPARAM;
    }

    qRegGPADC_BufferMode8BitsEnableSet(buffer, (pConfig->resolution == qDrvGPADC_Resolution8Bit));
    qRegGPADC_BufferUpdateModeSet(buffer, (qRegGPADC_BufferUpdateMode_t)pConfig->updateMode);

    (void)qDrvGPADC_BufferPresetSet(pInstance, buffer, &pConfig->preset);

    if(pConfig->irqEnable)
    {
        const qDrvGPADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

        if((pCallbacks == NULL) || (pCallbacks->bufferUpdate == NULL))
        {
            /* Buffer callback function is not set. */
            return Q_ERR_GENERAL;
        }

        qRegGPADC_IRQBufferUpdatedClear(buffer);
        qRegIntCtrl_EventIRQEnable(Q_DRV_ADC_IRQN, (1UL << irq), 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, (1UL << irq), 0);
    }

    return Q_OK;
}

qResult_t qDrvGPADC_BufferPresetSet(qDrvGPADC_t* const pInstance, qRegGPADC_Buffer_t buffer,
                                    qDrvGPADC_Preset_t* const pPreset)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(buffer < REG_GPADC_BUFFER_COUNT);
    Q_ASSERT(pPreset);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qRegGPADC_PresetMinValueSet((pPreset->min == Q_DRV_GPADC_PRESET_VALUE_UNUSED) ? 0UL : pPreset->min);

    qRegGPADC_PresetMaxValueSet(
        (pPreset->max == Q_DRV_GPADC_PRESET_VALUE_UNUSED) ? Q_REG_GPADC_PRESET_MAX : pPreset->max);

    qRegGPADC_BufferPresetSet(buffer);

    return Q_OK;
}

UInt16 qDrvGPADC_BufferRawResultGet(qDrvGPADC_t* const pInstance, qRegGPADC_Buffer_t buffer)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return 0;
    }

    return qRegGPADC_BufferResultGet(buffer);
}

qDrvGPADC_Voltage_t qDrvGPADC_RawToVoltageConvert(qDrvGPADC_t* const pInstance, UInt16 raw,
                                                  qDrvGPADC_Resolution_t resolution, qRegGPADC_Slot_t slot)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(slot < REG_GPADC_SLOT_COUNT);

    qDrvGPADC_Voltage_t voltage = {
        .negative = false,
        .integer = 0,
        .fractional = 0,
    };

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return voltage;
    }

    Int32 outVoltage = rap_sens_gp_adc_code_to_uv(raw, resolution == qDrvGPADC_Resolution8Bit,
                                                  qRegGPADC_SlotSeToDeModeEnableGet(slot) == true,
                                                  qRegGPADC_SlotLowVoltageModeEnableGet(slot) == false);

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

UInt16 qDrvGPADC_VoltageToRawConvert(qDrvGPADC_t* const pInstance, qDrvGPADC_Voltage_t voltage,
                                     qDrvGPADC_Resolution_t resolution, qRegGPADC_Slot_t slot)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(slot < REG_GPADC_SLOT_COUNT);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return 0UL;
    }

    if(qRegGPADC_SlotSeToDeModeEnableGet(slot) && voltage.negative)
    {
        /*  */
        return 0UL;
    }

    Int16 rawVoltage = (voltage.integer * 1000) + voltage.fractional;

    if(voltage.negative)
    {
        rawVoltage *= -1;
    }

    Int16 vRef = qRegGPADC_SlotLowVoltageModeEnableGet(slot) ? Q_DRV_GP_ADC_REF_MV : (Q_DRV_GP_ADC_REF_MV * 4);
    Int16 outRaw;

    if(qRegGPADC_SlotSeToDeModeEnableGet(slot))
    {
        outRaw = (rawVoltage * 2047) / vRef;
    }
    else
    {
        outRaw = 1024 + ((rawVoltage * 1024) / vRef);
        if(outRaw < 0)
        {
            outRaw = 0;
        }
    }

    if(resolution == qDrvGPADC_Resolution8Bit)
    {
        outRaw >>= 3UL;
    }

    return (UInt16)outRaw;
}

qResult_t qDrvGPADC_FifoConfigSet(qDrvGPADC_t* const pInstance, qDrvGPADC_FifoConfig_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);
    Q_ASSERT(pConfig->buffer);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qDrvGPADC_FifoEnableCheck(pInstance))
    {
        /* Fifo is already enabled. */
        return Q_ERR_INVSTATE;
    }

    if(pConfig->bufferSize == 0)
    {
        /* Wrong buffer length. */
        return Q_ERR_INVPARAM;
    }

    if(pConfig->subsampleRate >= Q_REG_GPADC_FIFO_SUBSAMPLE_RATE_MAX)
    {
        /* Wrong subsample fifo value. */
        return Q_ERR_INVPARAM;
    }

    Bool resolution11bit = pConfig->resolution == qDrvGPADC_Resolution11Bit;

    if(resolution11bit && (pConfig->bufferSize % 2))
    {
        /* Buffer length not aligned to fifo resolution. */
        return Q_ERR_INVPARAM;
    }

    qRegGPADC_FifoSubsampleRateSet(pConfig->subsampleRate);

    /* Set the fifo sample size. */
    controlBlock.fifoSampleSize = resolution11bit ? 2UL : 1UL;

    if((controlBlock.pCallbacks == NULL) || (controlBlock.pCallbacks->fifoThreshold == NULL))
    {
        /* Fifo callback function is not set. */
        return Q_ERR_GENERAL;
    }

    controlBlock.fifoBuffer = pConfig->buffer;
    controlBlock.fifoSize = pConfig->bufferSize / controlBlock.fifoSampleSize;

    if(controlBlock.dmaChannel != Q_NOCHANNEL)
    {
        /* Mark interrupt mode unused. */
        controlBlock.fifoBuffer = NULL;

        qDrvDMA_ChannelConfig_t dmaConfig = {
            .callback = qDrvGPADC_CbDmaRx,
            .periAddress = Q_REG_GPADC_FIFO_ADDRESS_GET(),
            .mode = qDrvDMA_ChannelModePeriToMem,
            .bufferMode = qDrvDMA_BufferModeCircular,
            .wordMode = resolution11bit ? qRegDMA_WordMode16Bit : qRegDMA_WordMode8Bit,
            .dmaTriggerSelect = qRegDMA_TriggerGPADCFifoNotEmpty,
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

qResult_t qDrvGPADC_FifoEnable(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qDrvGPADC_FifoEnableCheck(pInstance))
    {
        /* Fifo is already enabled. */
        return Q_ERR_INVSTATE;
    }

    (void)qRegGPADC_FifoResultGet();
    qRegGPADC_FifoSubsampleCounterClear();

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
        if(controlBlock.fifoBuffer == NULL || controlBlock.fifoSize == 0 || controlBlock.fifoSampleSize == 0)
        {
            /* Fifo buffer is not set. */
            return Q_ERR_GENERAL;
        }

        controlBlock.fifoSampleCount = 0;
        controlBlock.fifoTail = 0;

        qRegIntCtrl_EventIRQEnable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqFifoNotEmpty), 0);
    }

    qRegGPADC_IRQFifoOverrunClear();
    qRegIntCtrl_EventIRQEnable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqFifoOverrun), 0);

    return Q_OK;
}

qResult_t qDrvGPADC_FifoDisable(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvGPADC_FifoEnableCheck(pInstance))
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
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqFifoNotEmpty), 0);
    }

    qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqFifoOverrun), 0);

    return Q_OK;
}

Bool qDrvGPADC_FifoEnableCheck(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
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
        UInt32 enabledMask;
        qRegIntCtrl_EventIRQEnableGet(Q_DRV_ADC_IRQN, &enabledMask, NULL);

        return (enabledMask & (1UL << qRegGPADC_IrqFifoOverrun)) > 0UL;
    }

    return false;
}

qResult_t qDrvGPADC_FifoFlush(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    const qDrvGPADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

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
        (controlBlock.dmaChannel != Q_NOCHANNEL) ? qDrvGPADC_DmaFifoFlushSafe : qDrvGPADC_InternalFifoFlushSafe;

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

qResult_t qDrvGPADC_AwdConfigSet(qDrvGPADC_t* const pInstance, qDrvGPADC_AwdConfig_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if((pConfig->preset.min > pConfig->preset.max) ||
       ((pConfig->preset.max != Q_DRV_GPADC_PRESET_VALUE_UNUSED) && (pConfig->preset.min == pConfig->preset.max)))
    {
        /* Wrong AWD limit values. */
        return Q_ERR_INVPARAM;
    }

    if(qDrvGPADC_AwdEnableCheck(pInstance))
    {
        /* AWD is enabled. */
        return Q_ERR_INVSTATE;
    }

    if((controlBlock.pCallbacks == NULL) || (controlBlock.pCallbacks->awd == NULL))
    {
        /* AWD callback function is not set. */
        return Q_ERR_GENERAL;
    }

    if(pConfig->preset.min != Q_DRV_GPADC_PRESET_VALUE_UNUSED)
    {
        qRegGPADC_PresetMinValueSet((UInt32)pConfig->preset.min);
        controlBlock.configuredMask |= qRegGPADC_ConfiguredMaskAwdBelow;
    }
    else
    {
        qRegGPADC_PresetMinValueSet(0UL);
        controlBlock.configuredMask &= ~qRegGPADC_ConfiguredMaskAwdBelow;
    }

    if(pConfig->preset.max != Q_DRV_GPADC_PRESET_VALUE_UNUSED)
    {
        qRegGPADC_PresetMaxValueSet((UInt32)pConfig->preset.max);
        controlBlock.configuredMask |= qRegGPADC_ConfiguredMaskAwdAbove;
    }
    else
    {
        qRegGPADC_PresetMaxValueSet(Q_REG_GPADC_PRESET_MAX);
        controlBlock.configuredMask &= ~qRegGPADC_ConfiguredMaskAwdAbove;
    }

    qRegGPADC_ADWPresetSet();

    return Q_OK;
}

qResult_t qDrvGPADC_AwdEnable(qDrvGPADC_t* const pInstance, qDrvGPADC_AWDEvent_t event)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if((controlBlock.configuredMask & (qRegGPADC_ConfiguredMaskAwdBelow | qRegGPADC_ConfiguredMaskAwdAbove)) == 0UL)
    {
        /* AWD is not configured. */
        return Q_ERR_INVSTATE;
    }

    qRegGPADC_IRQAdwBelowMinimumClear();
    qRegGPADC_IRQAdwAboveMaximumClear();

    UInt32 mask = 0UL;
    if((event & qDrvGPADC_AWDEventBelow) && (controlBlock.configuredMask & qRegGPADC_ConfiguredMaskAwdBelow))
    {
        mask |= (1UL << qRegGPADC_IrqAwdBelowMinimum);
    }

    if((event & qDrvGPADC_AWDEventAbove) && (controlBlock.configuredMask & qRegGPADC_ConfiguredMaskAwdAbove))
    {
        mask |= (1UL << qRegGPADC_IrqAwdAboveMaximum);
    }

    qRegIntCtrl_EventIRQEnable(Q_DRV_ADC_IRQN, mask, 0);

    return Q_OK;
}

qResult_t qDrvGPADC_AwdDisable(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(!qDrvGPADC_AwdEnableCheck(pInstance))
    {
        /* AWD is already disabled. */
        return Q_ERR_INVSTATE;
    }

    qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN,
                                ((1UL << qRegGPADC_IrqAwdAboveMaximum) | (1UL << qRegGPADC_IrqAwdBelowMinimum)), 0);

    return Q_OK;
}

Bool qDrvGPADC_AwdEnableCheck(qDrvGPADC_t* const pInstance)
{
    (void)pInstance;

    UInt32 enabledMask;
    qRegIntCtrl_EventIRQEnableGet(Q_DRV_ADC_IRQN, &enabledMask, NULL);

    return (enabledMask & ((1UL << qRegGPADC_IrqAwdAboveMaximum) | (1UL << qRegGPADC_IrqAwdBelowMinimum))) > 0UL;
}

qResult_t qDrvGPADC_ContinuousConfigSet(qDrvGPADC_t* const pInstance, Bool irqCycleDone)
{
    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(irqCycleDone)
    {
        const qDrvGPADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

        if((pCallbacks == NULL) || (pCallbacks->cycleDone == NULL))
        {
            /* Cycle done callback function is not set. */
            return Q_ERR_GENERAL;
        }

        qRegGPADC_IRQCycleDoneClear();
        qRegIntCtrl_EventIRQEnable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqCycleDone), 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqCycleDone), 0);
    }

    return Q_OK;
}

qResult_t qDrvGPADC_ContinuousStart(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegGPADC_SlotMaskEnableGet() == 0UL)
    {
        /* Slots are not enabled. At least one slot should be enabled. */
        return Q_ERR_INVSTATE;
    }

#if defined(REG_QUIC_PRESENT)
    for(UInt8 actionIdx = 0; actionIdx < REG_GPADC_QUIC_ACTION_COUNT; actionIdx++)
    {
        if(qRegGPADC_QuicActionTypeGet(actionIdx) != qRegGPADC_QuicActionTypeNone)
        {
            /* Unable to start continuous mode. Quic mode already used. */
            return Q_ERR_BUSY;
        }
    }
#endif /* defined(REG_QUIC_PRESENT) */

    qRegGPADC_TriggerModeSet(qRegGPADC_TriggerModeContinuous);

    return Q_OK;
}

qResult_t qDrvGPADC_ContinuousStop(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqCycleDone), 0);

    qRegGPADC_TriggerModeSet(qRegGPADC_TriggerModeNoneOrQuic);

    return Q_OK;
}

qResult_t qDrvGPADC_OneShootStart(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(qRegGPADC_TriggerModeGet() == qRegGPADC_TriggerModeContinuous)
    {
        /* Continuous mode is already enabled. */
        return Q_ERR_INVSTATE;
    }

    qResult_t res = qDrvGPADC_ContinuousStart(pInstance);
    if(res != Q_OK)
    {
        return res;
    }

    qRegGPADC_TriggerModeSet(qRegGPADC_TriggerModeNoneOrQuic);

    return Q_OK;
}

Bool qDrvGPADC_BusyCheck(qDrvGPADC_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvGPADC_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return false;
    }

    return qRegGPADC_BusyGet();
}

void gp_adc_handler(void)
{
    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(Q_DRV_ADC_IRQN, &maskedEvents, NULL);

    const qDrvGPADC_Cbs_t* pCallbacks = controlBlock.pCallbacks;

    if(pCallbacks == NULL)
    {
        /* Disable NVIC interrupts. */
        NVIC_DisableIRQ(Q_DRV_ADC_IRQN);

        /* Disable global interrupts from the peripheral. */
        qRegIntCtrl_IRQDisable(Q_DRV_ADC_IRQN);
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqFifoNotEmpty))
    {
        UInt16 fifoResult = qRegGPADC_FifoResultGet();

        void* fifoBuffer = controlBlock.fifoBuffer;

        if(fifoBuffer)
        {
            if(controlBlock.fifoSampleSize == 1UL)
            {
                ((UInt8*)fifoBuffer)[controlBlock.fifoSampleCount++] = (UInt8)fifoResult;
            }
            else
            {
                ((UInt16*)fifoBuffer)[controlBlock.fifoSampleCount++] = fifoResult;
            }

            if(controlBlock.fifoSampleCount == (controlBlock.fifoSize / 2UL))
            {
                (void)qDrvGPADC_InternalFifoFlushSafe();
            }

            if(controlBlock.fifoSampleCount == controlBlock.fifoSize)
            {
                controlBlock.fifoSampleCount = 0;
                (void)qDrvGPADC_InternalFifoFlushSafe();
            }
        }
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqFifoOverrun))
    {
        qRegGPADC_IRQFifoOverrunClear();
        ; // TODO SW-12086
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqAwdAboveMaximum))
    {
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqAwdAboveMaximum), 0);
        qRegGPADC_IRQAdwAboveMaximumClear();

        pCallbacks->awd(controlBlock.arg, qDrvGPADC_AWDEventAbove);
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqAwdBelowMinimum))
    {
        qRegIntCtrl_EventIRQDisable(Q_DRV_ADC_IRQN, (1UL << qRegGPADC_IrqAwdBelowMinimum), 0);
        qRegGPADC_IRQAdwBelowMinimumClear();

        pCallbacks->awd(controlBlock.arg, qDrvGPADC_AWDEventBelow);
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqCycleDone))
    {
        qRegGPADC_IRQCycleDoneClear();

        pCallbacks->cycleDone(controlBlock.arg);
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqBufferAUpdated))
    {
        qRegGPADC_IRQBufferUpdatedClear(qRegGPADC_BufferA);

        pCallbacks->bufferUpdate(controlBlock.arg, qRegGPADC_BufferA, qRegGPADC_BufferResultGet(qRegGPADC_BufferA));
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqBufferBUpdated))
    {
        qRegGPADC_IRQBufferUpdatedClear(qRegGPADC_BufferB);

        pCallbacks->bufferUpdate(controlBlock.arg, qRegGPADC_BufferB, qRegGPADC_BufferResultGet(qRegGPADC_BufferB));
    }

    if(maskedEvents & (1UL << qRegGPADC_IrqBufferCUpdated))
    {
        qRegGPADC_IRQBufferUpdatedClear(qRegGPADC_BufferC);

        pCallbacks->bufferUpdate(controlBlock.arg, qRegGPADC_BufferC, qRegGPADC_BufferResultGet(qRegGPADC_BufferC));
    }
}
