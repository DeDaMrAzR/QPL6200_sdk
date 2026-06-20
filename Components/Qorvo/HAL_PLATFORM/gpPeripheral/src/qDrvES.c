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
#include "qDrvES.h"
#include "qRegInternal.h"
#include "gpLog.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    Bool initialized;
} qDrvES_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvES_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvES_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    /**
     * This is a SW workaround for the HW issue described in KRON-4326
     * Wrong edge detection for slow src clock and fast dest clock.
     */
    return qRegQUIC_Speed32M;
#else
    return qRegQUIC_Speed16M;
#endif
}

static qRegQUIC_TriggerSrc_t qDrvES_QuicTriggerSrcGet(qDrvQUIC_Internal_Instance_t* pInstance, void* pConfig)
{
    (void)pInstance;
    qDrvES_QuicTriggerConfig_t* triggerConfig = (qDrvES_QuicTriggerConfig_t*)pConfig;

    Q_ASSERT(triggerConfig->esEventId < REG_ES_EVENT_COUNT);

    qRegQUIC_TriggerSrc_t triggerSrc = qRegQUIC_TriggerSrcNoTrigger;

    triggerSrc = qRegQUIC_TriggerSrcEsEvent0 +
                 (triggerConfig->esEventId * (qRegQUIC_TriggerSrcEsEvent1 - qRegQUIC_TriggerSrcEsEvent0));

    return triggerSrc;
}

static qDrvQUIC_Internal_Trigger_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskTrigger,
    .apiSpeedGet = qDrvES_QuicSpeedGet,
    .apiTriggerSrcGet = qDrvES_QuicTriggerSrcGet,
    .apiTriggerDisconnect = NULL,
};

#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvES_Init(qDrvES_t* const pInstance)
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

qResult_t qDrvES_Deinit(qDrvES_t* const pInstance)
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
