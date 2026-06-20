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

#include "qRegInternal.h"
#include "qDrvLED.h"

#include <stdlib.h>

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define THRESHOLD_MAX 0xFFU

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef struct {
    Bool initialized;
} qDrvLED_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static qDrvLED_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvLED_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qResult_t qDrvLED_QuicActionConfigSet(qRegQUIC_Channel_t quicChannel, qDrvQUIC_Internal_Instance_t* pInstance,
                                             void* pConfig, UInt32* pActionIdx)
{
    if(!qDrvLED_InitCheck((qDrvLED_t* const)pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    qDrvLED_QuicActionConfig_t* const pActionConfig = (qDrvLED_QuicActionConfig_t*)pConfig;

    if((pActionConfig->ledChannel >= REG_LED_CHANNEL_COUNT) || (quicChannel >= REG_QUIC_CHANNEL_COUNT) ||
       (pActionConfig->actionIdx >= REG_LED_QUIC_ACTION_COUNT))
    {
        return Q_ERR_INVPARAM;
    }

    *pActionIdx = (UInt32)(((UInt32)pActionConfig->ledChannel << 8UL) | pActionConfig->actionIdx);

    qRegLED_QuicChannelSet(pActionConfig->ledChannel, pActionConfig->actionIdx, quicChannel);
    qRegLED_QuicActionTypeSet(pActionConfig->ledChannel, pActionConfig->actionIdx, pActionConfig->action);

    return Q_OK;
}

static qResult_t qDrvLED_QuicTriggerDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                               qDrvQUIC_Internal_Instance_t* pInstance)
{
    if(!qDrvLED_InitCheck((qDrvLED_t* const)pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    if(quicChannel >= REG_QUIC_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    UInt8 ledChannel = (actionIdx >> 8UL) & 0xFFUL;
    actionIdx &= 0xFFUL;

    if(qRegLED_QuicChannelGet(ledChannel, actionIdx) == quicChannel)
    {
        qRegLED_QuicActionTypeSet(ledChannel, actionIdx, qRegLED_QuicActionNone);
    }

    return Q_OK;
}

static qDrvQUIC_Internal_Action_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskAction,
    .apiSpeedGet = qDrvLED_QuicSpeedGet,
    .apiActionConfigSet = qDrvLED_QuicActionConfigSet,
    .apiActionDisconnect = qDrvLED_QuicTriggerDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvLED_PinConfigSet(const qDrvLED_PinConfig_t* pPinConfig)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(&pPinConfig->led, 1);
    if(res != Q_OK)
    {
        return res;
    }

    if(pPinConfig->led.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->led.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    return Q_OK;
}

qResult_t qDrvLED_PinConfigClear(const qDrvLED_PinConfig_t* pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(&pPinConfig->led, 1);
}

qResult_t qDrvLED_Init(qDrvLED_t* const pInstance, const qDrvLED_Config_t* pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(qDrvLED_InitCheck(pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    if(pConfig->slopeTime)
    {
        UInt32 slopeTimeUnit = ((pConfig->slopeTime * 1000) / 1024U);
        if(slopeTimeUnit)
        {
            slopeTimeUnit--;
        }

        if(slopeTimeUnit > REG_LED_SLOPE_TIME_UNIT_MAX)
        {
            return Q_ERR_INVPARAM;
        }

        qRegLED_SlopeTimeUnitSet(slopeTimeUnit);
    }

    const UInt32 srcSpeeds[4] = {
        32000000,
        8000000,
        2000000,
        64000,
    };

    const qRegLED_SrcClkFreq_t srcClockFreqs[4] = {
        qRegLED_SrcClkFreq32M,
        qRegLED_SrcClkFreq8M,
        qRegLED_SrcClkFreq2M,
        qRegLED_SrcClkFreq62K,
    };

    UInt32 srcSpeedIndex = 0;
    UInt32 prescalerDivider = 0;
    UInt32 freqDiff = 0xFFFFFFFF;

    for(UInt8 i = 0; i < Q_ARRAY_SIZE(srcSpeeds); i++)
    {
        for(UInt32 prescaler = 0; prescaler < REG_LED_PRESCALER_DIV_MAX; prescaler++)
        {
            Int32 tempDiff = (srcSpeeds[i] >> prescaler) - pConfig->frequency;
            if(abs(tempDiff) < freqDiff)
            {
                freqDiff = tempDiff;
                srcSpeedIndex = i;
                prescalerDivider = prescaler;
            }
        }
    }

    qRegLED_SrcClkFreqSet(srcClockFreqs[srcSpeedIndex]);
    qRegLED_PrescalerDividerSet(prescalerDivider);

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    controlBlock.initialized = true;

    return Q_OK;
}

qResult_t qDrvLED_Deinit(qDrvLED_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvLED_InitCheck(pInstance))
    {
        return Q_OK;
    }

    for(UInt8 i = 0; i < REG_LED_CHANNEL_COUNT; i++)
    {
        qRegLED_FadeSet(i, false);
        qRegLED_EnableSet(i, false);
    }

    controlBlock.initialized = false;

    return Q_OK;
}

Bool qDrvLED_InitCheck(qDrvLED_t* const pInstance)
{
    Q_ASSERT(pInstance);

    (void)pInstance;

    return controlBlock.initialized;
}

qResult_t qDrvLED_Configure(qDrvLED_t* const pInstance, UInt8 channel, const qDrvLED_ChannelConfig_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(!qDrvLED_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_LED_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegLED_ThresholdSet(channel, THRESHOLD_MAX);
    qRegLED_DriveSet(channel, pConfig->drive);
    qRegLED_InvertSet(channel, pConfig->invert);
    qRegLED_FadeSet(channel, false);
    qRegLED_AllowSleepSet(channel, pConfig->allowSleep);

    return Q_OK;
}

qResult_t qDrvLED_Set(qDrvLED_t* const pInstance, UInt8 channel, Bool enable)
{
    Q_ASSERT(pInstance);

    if(!qDrvLED_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_LED_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegLED_FadeSet(channel, false);
    qRegLED_EnableSet(channel, enable);

    return Q_OK;
}

qResult_t qDrvLED_SetBrightness(qDrvLED_t* const pInstance, UInt8 channel, UInt8 pwm, Bool fade)
{
    Q_ASSERT(pInstance);

    if(!qDrvLED_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(channel >= REG_LED_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qRegLED_FadeSet(channel, fade);

    if(pwm)
    {
        qRegLED_ThresholdSet(channel, pwm);
        qRegLED_EnableSet(channel, true);
    }
    else
    {
        qRegLED_EnableSet(channel, false);
    }

    return Q_OK;
}
