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
#include "qRegSysRam.h"
#include "qDrvCommon.h"
#include "qDrvDMA.h"
#include "qDrvPWMXL.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief PWMXL channel state. */
typedef struct {
    UInt16 dutyTicks; /**< Current duty cycle. */
    Bool initialized; /**< Channel initialized. */
} qDrvPWMXL_ChannelState_t;

/** @brief PWMXL instance control block */
typedef struct {
    qDrvPWMXL_Cb_t callback;                                    /**< Waveform event callback function. */
    void* arg;                                                  /**< User context variable. */
    qDrvPWMXL_ChannelState_t channels[REG_PWMXL_CHANNEL_COUNT]; /**< Channel state structures. */
    UInt16 periodTicks;                                         /**< Configured PWM period. */
    void* dmaBuffer;                                            /**< DMA buffer. */
    UInt16 dmaBufferLen;                                        /**< DMA buffer length in bytes. */
    UInt16 dmaBufferPos;                                        /**< DMA buffer write index. */
    qDrvDMA_Channel_t dmaChannel;                               /**< DMA channel. */
    Bool dmaActive;                                             /**< DMA transfer active state. */
    qDrvPWMXL_DmaMode_t dmaMode;                                /**< Waveform mode. */
    UInt8 dmaPwmChannel;                                        /**< Waveform channel. */
} qDrvPWMXL_ControlBlock_t;

Q_STATIC_ASSERT(sizeof(qDrvPWMXL_ControlBlock_t) == sizeof(qDrvPWMXL_Internal_t));

/** @brief Instance-specific constant data block. */
typedef struct {
    qRegSysRam_Retain_t retain;         /**< Bit index in SysRam retain register. */
    qRegDMA_Trigger_t dmaTriggerSelect; /**< DMA trigger number. */
} qDrvPWMXL_ConstData_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define Q_DRV_PWMXL_PRESCALER_MAX 7UL

/** @brief Macro for initializing const data block for TimerXL instance. */
#define Q_DRV_PWMXL_CONST_DATA_INIT(ID)                                                                                \
    {                                                                                                                  \
        .retain = Q_REG_SYSRAM_RETAIN_GET(PWMXLS),                     /* */                                           \
            .dmaTriggerSelect = qRegDMA_TriggerPWMXLNextBufferNotFull, /* */                                           \
    }

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief Constant data for PWMXL instances. */
static const qDrvPWMXL_ConstData_t pwmxlConstData[REG_PWMXL_INSTANCE_COUNT] = {
    Q_DRV_PWMXL_CONST_DATA_INIT(0),
};

static qDrvPWMXL_t* pInstances[REG_PWMXL_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static inline qDrvPWMXL_ControlBlock_t* qDrvPWMXL_ControlBlockGet(const qDrvPWMXL_t* const pInstance)
{
    return (qDrvPWMXL_ControlBlock_t*)&pInstance->internal;
}

static void qDrvPWMXL_BufferMapSet(const qDrvPWMXL_t* pInstance, qDrvPWMXL_BufferMap_t mapping)
{
    // Map a specific channel buffer event to general PWMXL_NEXT_BUFFER_NOT_FULL for DMA triggering.
    UInt8 selectedChannel = mapping.channel;
    if(selectedChannel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return;
    }

    for(UInt8 channel = 0; channel < REG_PWMXL_CHANNEL_COUNT; ++channel)
    {
        if(channel == selectedChannel)
        {
            qRegPWMXL_ChannelMapPhaseEmptySet(pInstance->baseAddr, channel, mapping.next_phase);
            qRegPWMXL_ChannelMapWidthEmptySet(pInstance->baseAddr, channel, mapping.next_width);
            qRegPWMXL_ChannelMapDitherEmptySet(pInstance->baseAddr, channel, mapping.next_dither);
        }
        else
        {
            qRegPWMXL_ChannelMapPhaseEmptySet(pInstance->baseAddr, channel, false);
            qRegPWMXL_ChannelMapWidthEmptySet(pInstance->baseAddr, channel, false);
            qRegPWMXL_ChannelMapDitherEmptySet(pInstance->baseAddr, channel, false);
        }
    }
}

static void qDrvPWMXL_CbDma(void* arg, qDrvDMA_EventType_t callbackType, void* pChunk, UInt16 chunkSize)
{
    const qDrvPWMXL_t* pInstance = (const qDrvPWMXL_t*)arg;
    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);

    switch(callbackType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
        {
            /* Split the buffer in half. */
            UInt16 chunkSize = pCb->dmaBufferLen / 2;

            qDrvDMA_Advance(pCb->dmaChannel, chunkSize);
            if(pCb->callback)
            {
                void* pdmaBufferOffset = (UInt32*)pCb->dmaBuffer + pCb->dmaBufferPos;
                pCb->callback(pInstance, pdmaBufferOffset, chunkSize, pCb->dmaMode, pCb->arg);
            }

            pCb->dmaBufferPos = (pCb->dmaBufferPos + chunkSize) % pCb->dmaBufferLen;
            break;
        }
        default:
        {
            break;
        }
    }

    return;
}

static qResult_t qDrvPWMXL_DMAStart(const qDrvPWMXL_t* pInstance, qDrvPWMXL_DmaMode_t mode,
                                    qDrvPWMXL_BufferMap_t mapping, void* buffer, UInt16 length)
{
    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);
    qResult_t result;
    UInt8 sampleSize;

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->dmaActive)
    {
        return Q_ERR_BUSY;
    }

    if(mapping.channel >= REG_PWMXL_CHANNEL_COUNT || !length)
    {
        return Q_ERR_INVPARAM;
    }

    if(pCb->dmaChannel == Q_NOCHANNEL)
    {
        pCb->dmaChannel = qDrvDMA_Alloc();
        if(pCb->dmaChannel == Q_NOCHANNEL)
        {
            return Q_ERR_OUTOFMEMORY;
        }
    }

    pCb->dmaBuffer = buffer;
    pCb->dmaBufferLen = length;
    pCb->dmaPwmChannel = mapping.channel;
    pCb->dmaMode = mode;
    pCb->dmaBufferPos = 0;

    qDrvDMA_ChannelConfig_t dmaConfig = {
        .callback = &qDrvPWMXL_CbDma,
        .mode = qDrvDMA_ChannelModeMemToPeri,
        .bufferMode = qDrvDMA_BufferModeCircular,
        .dmaTriggerSelect = pwmxlConstData[pInstance->id].dmaTriggerSelect,
    };

    if(mode == qDrvPWMXL_DmaMode32bitWaveform)
    {
        sampleSize = sizeof(UInt32);
        dmaConfig.wordMode = qRegDMA_WordMode32Bit;
        dmaConfig.periAddress = qRegPWMXL_ValueAddrGet(pInstance->baseAddr);
    }
    else
    {
        sampleSize = sizeof(UInt16);
        dmaConfig.wordMode = qRegDMA_WordMode16Bit;
        dmaConfig.periAddress = qRegPWMXL_ChannelNextWidthAddrGet(pInstance->baseAddr, mapping.channel);
    }

    result = qDrvDMA_ChannelInit(pCb->dmaChannel, &dmaConfig, (void*)pInstance);
    if(result != Q_OK)
    {
        return result;
    }

    result = qDrvDMA_BufferSet(pCb->dmaChannel, buffer, length);
    if(result != Q_OK)
    {
        return result;
    }

    qDrvPWMXL_BufferMapSet(pInstance, mapping);
    qDrvDMA_Advance(pCb->dmaChannel, length);

    pCb->dmaActive = true;
    result = qDrvDMA_ChannelEnable(pCb->dmaChannel, (length / 2) / sampleSize);
    if(result != Q_OK)
    {
        pCb->dmaActive = false;
        return result;
    }

    qDrvDMA_ChannelIRQEnableSet(pCb->dmaChannel, qRegDMA_ChannelIrqAlmostComplete, true);

    return Q_OK;
}

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvPWMXL_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qRegQUIC_TriggerSrc_t qDrvPWMXL_QuicTriggerSrcGet(qDrvQUIC_Internal_Instance_t* pInstance, void* pConfig)
{
    (void)pInstance;
    qDrvPWMXL_QuicTriggerConfig_t* triggerConfig = (qDrvPWMXL_QuicTriggerConfig_t*)pConfig;

    Q_ASSERT(triggerConfig->pwmChannel < REG_PWMXL_CHANNEL_COUNT);

    qRegQUIC_TriggerSrc_t triggerSrc = qRegQUIC_TriggerSrcNoTrigger;

    if(triggerConfig->srcTrigger == qDrvPWMXL_QuicSrcTriggerTopOverflow)
    {
        triggerSrc = qRegQUIC_TriggerSrcPwmxlTopOverflow;
    }

    else if(triggerConfig->srcTrigger < qDrvPWMXL_QuicSrcTriggerTopOverflow)
    {
        triggerSrc =
            qRegQUIC_TriggerSrcPwmxl0PhaseMatch + triggerConfig->srcTrigger +
            (triggerConfig->pwmChannel * (qRegQUIC_TriggerSrcPwmxl1PhaseMatch - qRegQUIC_TriggerSrcPwmxl0PhaseMatch));
    }

    return triggerSrc;
}

static qResult_t qDrvPWMXL_QuicActionConfigSet(qRegQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pInstance,
                                               void* pConfig, UInt32* pActionIdx)
{
    const qDrvPWMXL_t* pPwmxlInstance = (qDrvPWMXL_t*)pInstance;
    qDrvPWMXL_QuicActionConfig_t* const pActionConfig = (qDrvPWMXL_QuicActionConfig_t*)pConfig;

    if((channel >= REG_QUIC_CHANNEL_COUNT) || (pActionConfig->actionIdx >= REG_PWMXL_QUIC_ACTION_COUNT))
    {
        return Q_ERR_INVPARAM;
    }

    *pActionIdx = pActionConfig->actionIdx;

    qRegPWMXL_QUICActionChannelSet(pPwmxlInstance->baseAddr, pActionConfig->actionIdx, channel);
    qRegPWMXL_QUICActionPwmxlChannelSet(pPwmxlInstance->baseAddr, pActionConfig->actionIdx, pActionConfig->pwmChannel);
    qRegPWMXL_QUICActionTypeSet(pPwmxlInstance->baseAddr, pActionConfig->actionIdx, pActionConfig->actionType);

    return Q_OK;
}

static qResult_t qDrvPWMXL_QuicActionDisconnect(qRegQUIC_Channel_t channel, UInt32 actionIdx,
                                                qDrvQUIC_Internal_Instance_t* pInstance)
{
    const qDrvPWMXL_t* pPwmxlInstance = (qDrvPWMXL_t*)pInstance;

    if(qRegPWMXL_QUICActionChannelGet(pPwmxlInstance->baseAddr, actionIdx) == channel)
    {
        qRegPWMXL_QUICActionTypeSet(pPwmxlInstance->baseAddr, actionIdx, qRegPWMXL_QuicActionTypeNone);
    }

    return Q_OK;
}

static qDrvQUIC_Internal_Both_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskBoth,
    .apiSpeedGet = qDrvPWMXL_QuicSpeedGet,
    .apiTriggerSrcGet = qDrvPWMXL_QuicTriggerSrcGet,
    .apiTriggerDisconnect = NULL,
    .apiActionConfigSet = qDrvPWMXL_QuicActionConfigSet,
    .apiActionDisconnect = qDrvPWMXL_QuicActionDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvPWMXL_PinConfigSet(const qDrvPWMXL_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(&pPinConfig->pwm, 1);
    if(res != Q_OK)
    {
        return res;
    }

    if(pPinConfig->pwm.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->pwm.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    return Q_OK;
}

qResult_t qDrvPWMXL_PinConfigClear(const qDrvPWMXL_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(&pPinConfig->pwm, 1);
}

qResult_t qDrvPWMXL_Init(qDrvPWMXL_t* pInstance, const qDrvPWMXL_Config_t* const pConfig, const qDrvPWMXL_Cb_t callback,
                         void* arg)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);
    UInt8 prescaler;
    UInt16 periodTicks;

    if(qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    memset(pCb, 0, sizeof(*pCb));

    qDrvPWMXL_FrequencyCalculate(Q_DRV_PWMXL_DEFAULT_FREQUENCY, &periodTicks, &prescaler);
    qRegPWMXL_PrescalerSet(pInstance->baseAddr, pConfig->prescaler);
    qRegPWMXL_ActiveTopWrapSet(pInstance->baseAddr, pConfig->periodTicks);
    qRegPWMXL_CountModeSet(pInstance->baseAddr, pConfig->countMode);
    qSysRam_RetainEnableSet(pwmxlConstData[pInstance->id].retain, true);

    pCb->periodTicks = periodTicks;
    pCb->dmaChannel = Q_NOCHANNEL;
    pCb->callback = callback;
    pCb->arg = arg;

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvPWMXL_Deinit(const qDrvPWMXL_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_OK;
    }

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);

    if(pCb->dmaChannel != Q_NOCHANNEL)
    {
        if(pCb->dmaActive)
        {
            (void)qDrvPWMXL_WaveformStop(pInstance);
        }
        (void)qDrvDMA_ChannelFree(pCb->dmaChannel);
        pCb->dmaChannel = Q_NOCHANNEL;
    }

    for(UInt8 channel = 0; channel < REG_PWMXL_CHANNEL_COUNT; ++channel)
    {
        (void)qDrvPWMXL_ChannelDeinit(pInstance, channel);
    }

    qRegPWMXL_EnableSet(pInstance->baseAddr, false);
    qSysRam_RetainEnableSet(pwmxlConstData[pInstance->id].retain, false);

    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

Bool qDrvPWMXL_InitCheck(const qDrvPWMXL_t* pInstance)
{
    Q_ASSERT(pInstance);

    return pInstances[pInstance->id] != NULL;
}

qResult_t qDrvPWMXL_PeriodSet(const qDrvPWMXL_t* pInstance, UInt16 periodTicks, UInt8 prescaler)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegPWMXL_PrescalerSet(pInstance->baseAddr, prescaler);
    if(qRegPWMXL_EnableGet(pInstance->baseAddr))
    {
        /* Change the period in the next cycle to avoid glitching. */
        qRegPWMXL_NextTopWrapSet(pInstance->baseAddr, periodTicks);
    }
    else
    {
        qRegPWMXL_ActiveTopWrapSet(pInstance->baseAddr, periodTicks);
    }

    return Q_OK;
}

Bool qDrvPWMXL_FrequencyCalculate(UInt32 frequency, UInt16* ticks, UInt8* prescaler)
{
    Q_ASSERT(frequency);
    Q_ASSERT(ticks);
    Q_ASSERT(prescaler);

    Bool result = true;
    UInt32 ticks_u32 = Q_DRV_PWMXL_CLOCK_FREQUENCY / frequency;
    *prescaler = 0;

    // Top wrap field (period in ticks) of the peripheral is 16-bit long.
    // Larger value must be scaled using prescaler
    while(ticks_u32 > UINT16_MAX && *prescaler < Q_DRV_PWMXL_PRESCALER_MAX)
    {
        ticks_u32 >>= 1;
        ++(*prescaler);
    }
    // The actual period is equal to TOP + 1, so:
    --ticks_u32;

    // Prescaler divider has limited bit length.
    // In case of exceeding it, set the maximum allowed period.
    if(ticks_u32 > UINT16_MAX)
    {
        ticks_u32 = UINT16_MAX;
        result = false;
    }

    *ticks = (UInt16)ticks_u32;
    return result;
}

UInt32 qDrvPWMXL_TicksToUsConvert(const qDrvPWMXL_t* pInstance, UInt16 periodTicks)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return 0;
    }

    UInt8 prescaler = qRegPWMXL_PrescalerGet(pInstance->baseAddr);

    /* Unit of Q_DRV_PWMXL_CLOCK_FREQUENCY is cycle/s. To get microseconds,
       result must be multiplied by 1M or Q_DRV_PWMXL_CLOCK_FREQUENCY divided
       by 1M. 1M is split between input (ticks * 1k, to avoid overflow)
       and Q_DRV_PWMXL_CLOCK_FREQUENCY (/1k to avoid losing fractional part
       with high prescaler value).
     */
    UInt32 periodTicks32 = (UInt32)periodTicks * 1000U;
    return periodTicks32 / ((Q_DRV_PWMXL_CLOCK_FREQUENCY >> prescaler) / 1000U);
}

UInt16 qDrvPWMXL_WaveformParamsPack(const qDrvPWMXL_WaveformEntryParams_t* params)
{
    Q_ASSERT(params);

    return Q_REG_PWMXL_WAVEFORM_PARAMS_PACK(params->valueType, params->idx, params->validatePhase,
                                            params->validateWidth, params->validateDither);
}

void qDrvPWMXL_WaveformParamsUnpack(UInt16 value, qDrvPWMXL_WaveformEntryParams_t* params)
{
    Q_ASSERT(params);

    params->valueType = Q_REG_PWMXL_WAVEFORM_PARAM_VALUETYPE_GET(value);
    params->idx = Q_REG_PWMXL_WAVEFORM_PARAM_IDX_GET(value);
    params->validatePhase = Q_REG_PWMXL_WAVEFORM_PARAM_VALIDATEPHASE_GET(value);
    params->validateWidth = Q_REG_PWMXL_WAVEFORM_PARAM_VALIDATEWIDTH_GET(value);
    params->validateDither = Q_REG_PWMXL_WAVEFORM_PARAM_VALIDATEDITHER_GET(value);
}

qResult_t qDrvPWMXL_ChannelInit(const qDrvPWMXL_t* pInstance, UInt8 channel)
{
    Q_ASSERT(pInstance);

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->channels[channel].initialized)
    {
        return Q_ERR_INVSTATE;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegPWMXL_ChannelActivePhaseSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelActiveWidthSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelActiveDitherSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelNextPhaseSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelNextWidthSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelNextDitherSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelTriggerModeSet(pInstance->baseAddr, channel, qRegPWMXL_TriggerModePhaseMatch);
    qRegPWMXL_ChannelInvertSet(pInstance->baseAddr, channel, false);
    qRegPWMXL_ChannelOutputDriveSet(pInstance->baseAddr, channel, Q_DRV_PWMXL_DEFAULT_DRIVE);

    pCb->channels[channel].dutyTicks = 0;
    pCb->channels[channel].initialized = true;

    return Q_OK;
}

qResult_t qDrvPWMXL_ChannelDeinit(const qDrvPWMXL_t* pInstance, UInt8 channel)
{
    Q_ASSERT(pInstance);

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance) || !pCb->channels[channel].initialized)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegPWMXL_ChannelNextPhaseSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelNextWidthSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelNextDitherSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelActivePhaseSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelActiveWidthSet(pInstance->baseAddr, channel, 0);
    qRegPWMXL_ChannelActiveDitherSet(pInstance->baseAddr, channel, 0);

    pCb->channels[channel].initialized = false;

    return Q_OK;
}

qResult_t qDrvPWMXL_Enable(const qDrvPWMXL_t* pInstance, Bool enable)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegPWMXL_EnableGet(pInstance->baseAddr) == enable)
    {
        return Q_ERR_INVSTATE;
    }

    qRegPWMXL_EnableSet(pInstance->baseAddr, enable);

    return Q_OK;
}

qResult_t qDrvPWMXL_ChannelDutySet(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16 dutyPercent)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);
    dutyPercent = min(dutyPercent, 100UL * 100UL);

    // The actual PWM period is equal to TOP + 1, hence the adjustment
    UInt32 target_ticks = ((UInt32)pCb->periodTicks + 1) * (UInt32)dutyPercent / (100UL * 100UL);
    qRegPWMXL_ChannelNextWidthSet(pInstance->baseAddr, channel, (UInt16)target_ticks);

    return Q_OK;
}

qResult_t qDrvPWMXL_ChannelDutyRawSet(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16 ticks)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegPWMXL_ChannelNextWidthSet(pInstance->baseAddr, channel, ticks);

    return Q_OK;
}
qResult_t qDrvPWMXL_ChannelDutyExtSet(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16 ticks, UInt16 phase,
                                      UInt8 dither)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegPWMXL_ChannelNextPhaseSet(pInstance->baseAddr, channel, phase);
    qRegPWMXL_ChannelNextWidthSet(pInstance->baseAddr, channel, ticks);
    qRegPWMXL_ChannelNextDitherSet(pInstance->baseAddr, channel, dither);

    return Q_OK;
}

qResult_t qDrvPWMXL_InvertSet(const qDrvPWMXL_t* pInstance, UInt8 channel, Bool invert)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegPWMXL_ChannelInvertSet(pInstance->baseAddr, channel, invert);

    return Q_OK;
}

qResult_t qDrvPWMXL_DriveSet(const qDrvPWMXL_t* pInstance, UInt8 channel, qRegCommon_Drive_t drive)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegPWMXL_ChannelOutputDriveSet(pInstance->baseAddr, channel, drive);

    return Q_OK;
}

qResult_t qDrvPWMXL_TriggerModeSet(const qDrvPWMXL_t* pInstance, UInt8 channel, qDrvPWMXL_TriggerMode_t triggerMode)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_PWMXL_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegPWMXL_ChannelTriggerModeSet(pInstance->baseAddr, channel, triggerMode);

    return Q_OK;
}

qResult_t qDrvPWMXL_WaveformStart(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16* buffer, UInt16 sampleCount)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(buffer);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);
    UInt32 length = sizeof(buffer[0]) * sampleCount;

    /* Sample count must be an even number to split the buffer in half. */
    if(sampleCount % 2 || length > UINT16_MAX || !pCb->callback)
    {
        return Q_ERR_INVPARAM;
    }

    qDrvPWMXL_BufferMap_t mapping = {
        .channel = channel,
        .next_width = true,
    };
    return qDrvPWMXL_DMAStart(pInstance, qDrvPWMXL_DmaMode16bit, mapping, buffer, length);
}

qResult_t qDrvPWMXL_ExtWaveformStart(const qDrvPWMXL_t* pInstance, qDrvPWMXL_BufferMap_t mapping,
                                     qDrvPWMXL_WaveformEntry_t* buffer, UInt16 sampleCount)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(buffer);

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);
    UInt32 length = sizeof(qDrvPWMXL_WaveformEntry_t) * sampleCount;

    /* Sample count must be an even number to split the buffer in half. */
    if(sampleCount % 2 || length > UINT16_MAX || !pCb->callback)
    {
        return Q_ERR_INVPARAM;
    }

    return qDrvPWMXL_DMAStart(pInstance, qDrvPWMXL_DmaMode32bitWaveform, mapping, buffer, length);
}

qResult_t qDrvPWMXL_WaveformStop(const qDrvPWMXL_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvPWMXL_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvPWMXL_ControlBlock_t* pCb = qDrvPWMXL_ControlBlockGet(pInstance);

    if(!pCb->dmaActive || pCb->dmaChannel == Q_NOCHANNEL)
    {
        return Q_ERR_INVSTATE;
    }

    qDrvDMA_ChannelIRQEnableSet(pCb->dmaChannel, qRegDMA_ChannelIrqAlmostComplete, false);
    qDrvDMA_ChannelIRQEnableSet(pCb->dmaChannel, qRegDMA_ChannelIrqComplete, false);
    qDrvDMA_ChannelDisable(pCb->dmaChannel);
    qRegPWMXL_ChannelMapPhaseEmptySet(pInstance->baseAddr, pCb->dmaPwmChannel, false);
    qRegPWMXL_ChannelMapWidthEmptySet(pInstance->baseAddr, pCb->dmaPwmChannel, false);
    qRegPWMXL_ChannelMapDitherEmptySet(pInstance->baseAddr, pCb->dmaPwmChannel, false);
    qRegPWMXL_ChannelNextWidthSet(pInstance->baseAddr, pCb->dmaPwmChannel, 0);

    pCb->dmaPwmChannel = 0;

    return Q_OK;
}
