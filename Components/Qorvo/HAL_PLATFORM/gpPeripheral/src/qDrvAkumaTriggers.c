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
#include "qDrvAkumaTriggers.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    Bool initialized;
} qDrvAkumaTriggers_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvAkumaTriggers_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvAkumaTriggers_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qRegQUIC_TriggerSrc_t qDrvAkumaTriggers_QuicTriggerSrcGet(qDrvQUIC_Internal_Instance_t* pInstance, void* pConfig)
{
    (void)pInstance;
    qDrvAkumaTriggers_QuicTriggerConfig_t* triggerConfig = (qDrvAkumaTriggers_QuicTriggerConfig_t*)pConfig;

    Q_ASSERT(triggerConfig->ipcChannel < REG_AKUMATRIGGERS_IPC_CHANNEL_COUNT);

    qRegQUIC_TriggerSrc_t triggerSrc = qRegQUIC_TriggerSrcNoTrigger;

    switch(triggerConfig->srcTrigger)
    {
        case qDrvAkumaTriggers_QuicSrcTriggerPaOn:
            triggerSrc = qRegQUIC_TriggerSrcAkumaPaOn;
            break;
        case qDrvAkumaTriggers_QuicSrcTriggerPhyRx:
            triggerSrc = qRegQUIC_TriggerSrcAkumaPhyRx;
            break;
        case qDrvAkumaTriggers_QuicSrcTriggerPhyTx:
            triggerSrc = qRegQUIC_TriggerSrcAkumaPhyTx;
            break;
        case qDrvAkumaTriggers_QuicSrcTriggerRxOn:
            triggerSrc = qRegQUIC_TriggerSrcAkumaRxOn;
            break;
        case qDrvAkumaTriggers_QuicSrcTriggerTxOn:
            triggerSrc = qRegQUIC_TriggerSrcAkumaTxOn;
            break;
        case qDrvAkumaTriggers_QuicSrcTriggerIpcChannel:
            triggerSrc = qRegQUIC_TriggerSrcAkumaIpc0 +
                         (triggerConfig->ipcChannel * (qRegQUIC_TriggerSrcAkumaIpc1 - qRegQUIC_TriggerSrcAkumaIpc0));
            break;
        default:
            triggerSrc = qRegQUIC_TriggerSrcNoTrigger;
            break;
    }

    return triggerSrc;
}

static qDrvQUIC_Internal_Trigger_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskTrigger,
    .apiSpeedGet = qDrvAkumaTriggers_QuicSpeedGet,
    .apiTriggerSrcGet = qDrvAkumaTriggers_QuicTriggerSrcGet,
    .apiTriggerDisconnect = NULL,
};

#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvAkumaTriggers_Init(qDrvAkumaTriggers_t* const pInstance)
{
    if(controlBlock.initialized)
    {
        return Q_ERR_INVSTATE;
    }

    controlBlock.initialized = true;

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}

qResult_t qDrvAkumaTriggers_Deinit(qDrvAkumaTriggers_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!controlBlock.initialized)
    {
        return Q_OK;
    }

    controlBlock.initialized = false;

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = NULL;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}
