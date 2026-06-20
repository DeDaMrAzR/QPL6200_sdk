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
#include "qDrvBistAdc.h"

#include "gpHal_kx_Rap_BistAdc.h"
#include "gpHal_kx_Rap_Cp.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    Bool initialized;
    Bool enabled;
    qDrvBistAdc_Config_t config;
} qDrvBistAdc_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvBistAdc_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvBistAdc_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qResult_t qDrvBistAdc_QuicActionConfigSet(
    qRegQUIC_Channel_t quicChannel, qDrvQUIC_Internal_Instance_t* pInstance, void* pConfig, UInt32* pActionIdx)
{
    if(!qDrvBistAdc_InitCheck((qDrvBistAdc_t* const)pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvBistAdc_QuicActionConfig_t* const pActionConfig = (qDrvBistAdc_QuicActionConfig_t*)pConfig;

    if((quicChannel >= REG_QUIC_CHANNEL_COUNT) || (pActionConfig->actionIdx >= REG_BISTADC_QUIC_ACTION_COUNT))
    {
        return Q_ERR_INVPARAM;
    }

    *pActionIdx = pActionConfig->actionIdx;

    qRegBistAdc_QuicChannelSet(pActionConfig->actionIdx, quicChannel);
    qRegBistAdc_QuicActionTypeSet(pActionConfig->actionIdx, pActionConfig->action);

    return Q_OK;
}

static qResult_t qDrvBistAdc_QuicTriggerDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                                   qDrvQUIC_Internal_Instance_t* pInstance)
{
    if(!qDrvBistAdc_InitCheck((qDrvBistAdc_t* const)pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(quicChannel >= REG_QUIC_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    if(qRegBistAdc_QuicChannelGet(actionIdx) == quicChannel)
    {
        qRegBistAdc_QuicActionTypeSet(actionIdx, qRegBistAdc_QuicActionNone);
    }

    return Q_OK;
}

static qDrvQUIC_Internal_Action_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskAction,
    .apiSpeedGet = qDrvBistAdc_QuicSpeedGet,
    .apiActionConfigSet = qDrvBistAdc_QuicActionConfigSet,
    .apiActionDisconnect = qDrvBistAdc_QuicTriggerDisconnect,
};

#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvBistAdc_Init(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(qDrvBistAdc_InitCheck(pInstance))
    {
        return Q_ERR_INVSTATE;
    }

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    controlBlock.initialized = true;
    controlBlock.enabled = false;

    return Q_OK;
}

qResult_t qDrvBistAdc_Deinit(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return Q_OK;
    }

    if(controlBlock.enabled)
    {
        return Q_ERR_INVSTATE;
    }

    controlBlock.initialized = false;
    return Q_OK;
}

Bool qDrvBistAdc_InitCheck(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    return controlBlock.initialized;
}

qResult_t qDrvBistAdc_Enable(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(controlBlock.enabled)
    {
        return Q_ERR_INVSTATE;
    }

    // The RAP code for initializing and configuring the Bist Adc is called here
    // and not in the qDrvBistAdc_Init, to avoid current consumption of the Bist Adc
    // when not enabled.
    rap_bist_adc_init();
    rap_bist_adc_batmon_init();
    rap_bist_adc_config(controlBlock.config.diffMode, controlBlock.config.lowVoltageRange,
                        controlBlock.config.configTSampleUs, controlBlock.config.configNrAvgExp);
    rap_bist_adc_config_mux(controlBlock.config.muxPSettingVBat, controlBlock.config.muxNSettingVBat);
    rap_bist_adc_enable();

    controlBlock.enabled = true;
    return Q_OK;
}

qResult_t qDrvBistAdc_Disable(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!controlBlock.enabled)
    {
        return Q_ERR_INVSTATE;
    }

    // The RAP code for de-initializing the Bist Adc is called here
    // and not in the qDrvBistAdc_UnInit, to avoid current consumption of the Bist Adc
    // when not enabled.
    rap_bist_adc_disable();
    rap_bist_adc_batmon_deinit();
    rap_bist_adc_deinit();
    controlBlock.enabled = false;
    return Q_OK;
}

qResult_t qDrvBistAdc_Trigger(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!controlBlock.enabled)
    {
        return Q_ERR_INVSTATE;
    }

    // Start the measurement from the software.
    qRegBistAdc_Trigger();
    return Q_OK;
}

Bool qDrvBistAdc_ReadyCheck(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return false;
    }

    if(!controlBlock.enabled)
    {
        return false;
    }

    return qRegBistAdc_ReadyCheck();
}

Int32 qDrvBistAdc_ResultGet(qDrvBistAdc_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return 0;
    }

    return rap_bist_adc_to_uv(qRegBistAdc_ResultGet() * (256 / (1 << controlBlock.config.configNrAvgExp)),
                              controlBlock.config.diffMode, controlBlock.config.lowVoltageRange);
}

qResult_t qDrvBistAdc_ConfigSet(qDrvBistAdc_t* const pInstance, const qDrvBistAdc_Config_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    controlBlock.config = *pConfig;

    return Q_OK;
}

qResult_t qDrvBistAdc_ConfigGet(qDrvBistAdc_t* const pInstance, qDrvBistAdc_Config_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(!qDrvBistAdc_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    *pConfig = controlBlock.config;

    return Q_OK;
}
