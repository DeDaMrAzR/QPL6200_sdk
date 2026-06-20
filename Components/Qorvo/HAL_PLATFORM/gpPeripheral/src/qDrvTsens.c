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
#include "qDrvBist.h"
#include "qDrvCommon.h"
#include "qDrvTsens.h"

#include "gpHal_kx_Rap_TSens.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    const qDrvTsens_Callbacks_t* callbacks; /**< Set of user callbacks. */
    const qDrvTsens_Config_t* config;       /**< Pointer to user configuration. */
    Bool initialized;                       /**< Initialization flag. */
    Int16 lastResult;                       /**< Last result read from TSENS. */
    Bool triggered;                         /**< Flag to indicate if the measurement was triggered. */
} qDrvTsens_ControlBlock_t;

#ifdef HAL_ADC_CB_GET_TEMPERATURE
/* Used for testing purpose to mimic temperature variations */
extern Int16 qDrvTsens_CbResultGetBlocking(void);
#endif

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvTsens_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static void qDrvTsens_IRQtHandler(void);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void qDrvTsens_IRQtHandler(void)
{
    if(qDrvBist_TsensAboveMaximumMaskedIRQGet())
    {
        qRegTsens_AboveMaximumIRQClear();
        if(controlBlock.callbacks && controlBlock.callbacks->aboveMaximum)
        {
            controlBlock.callbacks->aboveMaximum();
        }
    }

    if(qDrvBist_TsensBelowMinimumMaskedIRQGet())
    {
        qRegTsens_BelowMinimumIRQClear();
        if(controlBlock.callbacks && controlBlock.callbacks->belowMinimum)
        {
            controlBlock.callbacks->belowMinimum();
        }
    }

    if(qDrvBist_TsensResultFifoNotEmptyMaskedIRQGet())
    {
        /* Interrupt cleared when reading from FIFO. */
        if(controlBlock.callbacks && controlBlock.callbacks->fifoNotEmpty)
        {
            controlBlock.callbacks->fifoNotEmpty();
        }

        /* Make sure the interrupt is cleared in case application does not read the result. */
        qDrvTsens_ResultGet();
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
qResult_t qDrvTsens_Init(const qDrvTsens_Config_t* const pConfig, const qDrvTsens_Callbacks_t* const pCallbacks)
{
    Q_ASSERT(pConfig);

    if(qDrvTsens_InitCheck())
    {
        return Q_ERR_INVSTATE;
    }

    if(pConfig->maxThreshold > Q_DRV_TSENS_THRESHOLD_MAX)
    {
        return Q_ERR_INVPARAM;
    }

    if(pConfig->minThreshold > Q_DRV_TSENS_THRESHOLD_MAX)
    {
        return Q_ERR_INVPARAM;
    }

    if(pConfig->cycles > qDrvTsens_HanningCycles128)
    {
        return Q_ERR_INVPARAM;
    }

    /* Configure the OBM. */
    qRegTsens_MaxThresholdSet(pConfig->maxThreshold);
    qRegTsens_MinThresholdSet(pConfig->minThreshold);
    qRegTsens_HanningCyclesSet((UInt8)pConfig->cycles);

    rap_tsens_init();

    controlBlock.callbacks = pCallbacks;
    controlBlock.config = pConfig;

    if(pCallbacks->aboveMaximum || pCallbacks->belowMinimum || pCallbacks->fifoNotEmpty)
    {
        qDrvBist_RegisterCallback(qDrvBist_HandlerTsens, qDrvTsens_IRQtHandler);
        qDrvBist_IRQEnable(true);
    }

    controlBlock.initialized = true;

    return Q_OK;
}

qResult_t qDrvTsens_Deinit(void)
{
    if(!qDrvTsens_InitCheck())
    {
        return Q_OK;
    }

    qDrvTsens_ContinuousMeasurementEnable(false);
    rap_tsens_deinit();

    if(controlBlock.callbacks->aboveMaximum || controlBlock.callbacks->belowMinimum ||
       controlBlock.callbacks->fifoNotEmpty)
    {
        qDrvBist_UnRegisterCallback(qDrvBist_HandlerTsens);
        qDrvBist_IRQEnable(false);
    }

    controlBlock.initialized = false;

    return Q_OK;
}

Bool qDrvTsens_InitCheck(void)
{
    return controlBlock.initialized;
}

qResult_t qDrvTsens_ContinuousMeasurementEnable(Bool enable)
{
    if(!qDrvTsens_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegTsens_ContinuousMeasurementEnable(enable);

    return Q_OK;
}

Int16 qDrvTsens_ResultGetNonBlocking(void)
{
    if(!qDrvTsens_InitCheck())
    {
        return Q_DRV_TSENS_THRESHOLD_MAX;
    }

    return (Int16)controlBlock.lastResult;
}

Int16 qDrvTsens_ResultGetBlocking(void)
{
    if(!qDrvTsens_InitCheck())
    {
        return Q_DRV_TSENS_THRESHOLD_MAX;
    }

    /* Check if temperature sensor is already enabled. */
    if(!qRegTsens_BiasDisPupEnabled())
    {
        /* The TSENS registers are not retained when the device comes out of sleep. Re-configure it */
        qRegTsens_MaxThresholdSet(controlBlock.config->maxThreshold);
        qRegTsens_MinThresholdSet(controlBlock.config->minThreshold);
        qRegTsens_HanningCyclesSet((UInt8)controlBlock.config->cycles);
        rap_tsens_init();
    }

#ifndef HAL_ADC_CB_GET_TEMPERATURE
    // To avoid triggering the measurement multiple times, we need to check if the
    // measurement was already triggered and wait for the result to be available.
    // If the measurement was not triggered, we need to trigger it.
    if(controlBlock.triggered == false)
    {
        controlBlock.triggered = true;
        // Trigger measurement.
        qRegTsens_DoSingleMeasurement();
    }

    // Wait for the result to be available.
    for(UInt32 i = 0; i < 15; i++)
    {
        // Trigger flag can be cleared by another call to qDrvTsens_ResultGetBlocking() function.
        // It means that in the lastResult variable there is a valid result.
        if(controlBlock.triggered == false)
        {
            break;
        }
        else if(qRegTsens_ResultFifoNotEmptyIRQGet())
        {
            controlBlock.lastResult = rap_tsens_to_deg(qRegTsens_ResultGet());

            // Clear the triggered flag.
            controlBlock.triggered = false;
            break;
        }
        else
        {
            HAL_WAIT_US(10);
        }
    }

    // If the measurement was triggered and the result is not available after 150 us, that is an error.
    if(controlBlock.triggered)
    {
        controlBlock.triggered = false;
        controlBlock.lastResult = Q_DRV_TSENS_THRESHOLD_MAX;
    }
#else
    controlBlock.lastResult = qDrvTsens_CbResultGetBlocking();
#endif

    return controlBlock.lastResult;
}
