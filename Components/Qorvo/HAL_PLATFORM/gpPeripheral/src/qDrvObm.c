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
#include "qDrvBist.h"
#include "qDrvCommon.h"
#include "qDrvObm.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define Q_DRV_OBM_MEASUREMENT_INVALID 0xFFFFFFFF

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    const qDrvObm_Callbacks_t* callbacks; /**< Set of user callbacks. */
    Bool initialized;                     /**< Initialization flag. */
} qDrvObm_Context_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvObm_Context_t obmState;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static inline Bool qDrvObm_Initialized(void)
{
    return obmState.initialized;
}

static void qDrvObm_IRQtHandler(void)
{
    if(qDrvBist_PeriodMeasDoneMaskedIRQGet())
    {
        qRegObm_PeriodMeasDoneIRQClear();
        if(obmState.callbacks && obmState.callbacks->periodMeasDone)
        {
            obmState.callbacks->periodMeasDone();
        }
    }

    if(qDrvBist_TotalMeasDoneMaskedIRQGet())
    {
        qDrvObm_TotalMeasDoneIRQClear();
        if(obmState.callbacks && obmState.callbacks->totalMeasDone)
        {
            obmState.callbacks->totalMeasDone();
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

UInt16 qDrvObm_PeriodMeasGet(void)
{
    if(!qDrvObm_Initialized())
    {
        return 0;
    }

    return qRegObm_PeriodMeasGet();
}

UInt32 qDrvObm_TotalMeasGet(void)
{
    if(!qDrvObm_Initialized())
    {
        return Q_DRV_OBM_MEASUREMENT_INVALID;
    }

    return qRegObm_TotalMeasGet();
}

UInt32 qDrvObm_HighMeasGet(void)
{
    if(!qDrvObm_Initialized())
    {
        return 0;
    }

    if(qRegObm_PrescalerGet())
    {
        /* When the prescaler (BIST_OBM_PRESCALER field higher than 0), the high
         * time measurement will be invalid.
         */
        return Q_DRV_OBM_MEASUREMENT_INVALID;
    }

    return qRegObm_HighMeasGet();
}

UInt16 qDrvObm_EdgeCountGet(void)
{
    if(!qDrvObm_Initialized())
    {
        return 0;
    }

    return qRegObm_EdgeCountGet();
}

qDrvObm_State_t qDrvObm_StateGet(void)
{
    return (qDrvObm_State_t) qRegObm_StateGet();
}

Bool qDrvObm_IsPeriodBelowMin(void)
{
    if(!qDrvObm_Initialized())
    {
        return false;
    }

    return qRegObm_PeriodBelowMinGet();
}

Bool qDrvObm_IsPeriodAboveMax(void)
{
    if(!qDrvObm_Initialized())
    {
        return false;
    }

    return qRegObm_PeriodAboveMaxGet();
}

qResult_t qDrvObm_Start(void)
{
    qRegObm_Start();

    return Q_OK;
}

qResult_t qDrvObm_Stop(void)
{
    qRegObm_Stop();

    return Q_OK;
}

qResult_t qDrvObm_DeInit(void)
{
    if(!qDrvObm_Initialized())
    {
        return Q_OK;
    }

    if(obmState.callbacks->totalMeasDone || obmState.callbacks->periodMeasDone)
    {
        qDrvBist_UnRegisterCallback(qDrvBist_HandlerObm);
        qDrvBist_IRQEnable(false);
    }

    return Q_OK;
}

qResult_t qDrvObm_Init(const qDrvObm_Config_t* const pConfig, const qDrvObm_Callbacks_t* const pCallbacks)
{
    Q_ASSERT(pConfig);

    if(qDrvObm_Initialized())
    {
        return Q_ERR_INVSTATE;
    }

    if(pConfig->prescaler > Q_DRV_OBM_PRESCALEAR_MAX)
    {
        return Q_ERR_NOTSUPPORTED;
    }

    if(pConfig->clkSrc > qRegObm_ClkSrcRc64m)
    {
        return Q_ERR_INVPARAM;
    }

    if(pConfig->periodMinThreshold > pConfig->periodMaxThreshold)
    {
        return Q_ERR_INVPARAM;
    }

    /* Configure the OBM. */
    qRegObm_EdgeCountThresholdSet(pConfig->edgeCountThreshold);
    qRegObm_PrescalerSet(pConfig->prescaler);
    qRegObm_ClkSrcSet(pConfig->clkSrc);
    qRegObm_SrcInvertSet(pConfig->srcInvert);
    qRegObm_PeriodMinThresholdSet(pConfig->periodMinThreshold);
    qRegObm_PeriodMaxThresholdSet(pConfig->periodMaxThreshold);

    obmState.callbacks = pCallbacks;

    if(pCallbacks->totalMeasDone || pCallbacks->periodMeasDone)
    {
        qDrvBist_RegisterCallback(qDrvBist_HandlerObm, qDrvObm_IRQtHandler);
        qDrvBist_IRQEnable(true);
    }

    obmState.initialized = true;

    return Q_OK;
}
