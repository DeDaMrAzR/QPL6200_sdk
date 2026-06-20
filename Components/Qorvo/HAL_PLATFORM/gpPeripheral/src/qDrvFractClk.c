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
#include "qDrvFractClk.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/* Definition of 2^20, to avoid usage math.h library. */
#define TWO_POW_20 1048576ULL

/* Minimum frequency in Hz, maximum is defined in a register layer. */
#define MIN_FREQUENCY 100000ULL

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static const qDrvFractClk_t* pInstances[REG_FRACT_CLK_INSTANCE_COUNT];

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvFractClk_PinConfigSet(const qDrvFractClk_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pPinConfig, 1);
    if(res != Q_OK)
    {
        return res;
    }

    return Q_OK;
}

qResult_t qDrvFractClk_PinConfigClear(const qDrvFractClk_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig, 1);
}

qResult_t qDrvFractClk_Init(qDrvFractClk_t* const pInstance, const qDrvFractClk_Config_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);
    Q_ASSERT(pInstance->id < REG_FRACT_CLK_INSTANCE_COUNT);

    if(qDrvFractClk_InitCheck(pInstance))
    {
        /* Instance is already initialized. */
        return Q_ERR_INVSTATE;
    }

    if((pConfig->freq > REG_FRACT_CLK_MAX_FREQUENCY) || (pConfig->freq < MIN_FREQUENCY))
    {
        /* Invalid frequency. */
        return Q_ERR_INVPARAM;
    }

    UInt64 target = (UInt64)REG_FRACT_CLK_SRC_FREQUENCY * (UInt64)(TWO_POW_20);
    target /= (UInt64)pConfig->freq;
    target /= 2ULL;

    qRegFractClk_TTargetSet(pInstance->baseAddr, (UInt32)target);

    qSysRam_RetainEnableSet(qRegSysRam_RetainFractClkGen, true);

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvFractClk_Deinit(qDrvFractClk_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_FRACT_CLK_INSTANCE_COUNT);

    if(!qDrvFractClk_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_OK;
    }

    qRegFractClk_TTargetEnableSet(pInstance->baseAddr, false);
    qRegFractClk_TTargetSet(pInstance->baseAddr, 0);

    pInstances[pInstance->id] = NULL;

    /* It is one peripheral, divided into several instances.
     * We need to be sure, that none of them are used. */
    Bool isInitialized = false;
    for(UInt8 i = 0; i < REG_FRACT_CLK_INSTANCE_COUNT; i++)
    {
        isInitialized = pInstances[i] != NULL;
    }

    if(!isInitialized)
    {
        qSysRam_RetainEnableSet(qRegSysRam_RetainFractClkGen, false);
    }

    return Q_OK;
}

Bool qDrvFractClk_InitCheck(qDrvFractClk_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_FRACT_CLK_INSTANCE_COUNT);

    return pInstances[pInstance->id] != NULL;
}

qResult_t qDrvFractClk_EnableSet(qDrvFractClk_t* const pInstance, Bool enable)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_FRACT_CLK_INSTANCE_COUNT);

    if(!qDrvFractClk_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegFractClk_TTargetEnableGet(pInstance->baseAddr) == enable)
    {
        /* Instance is already in the requested state. */
        return Q_ERR_INVSTATE;
    }

    qRegFractClk_TTargetEnableSet(pInstance->baseAddr, enable);

    return Q_OK;
}
