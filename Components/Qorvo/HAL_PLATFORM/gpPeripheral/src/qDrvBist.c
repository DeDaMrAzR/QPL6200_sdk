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

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef struct {
    qDrvBist_HandlerCb_t handlers[qDrvBist_HandlerCounts]; /**< @c qDrvBist_HandlerCb_t callbacks. */
    UInt32 counter;                                      /**<  Counter to track @c qDrvBist_IRQEnable(..) calls. */
} qDrvBist_ControlBlock_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static qDrvBist_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void IRQEnable(void)
{
    if(controlBlock.counter == 0)
    {
        qRegBist_IRQEnable(true);
        NVIC_EnableIRQ(BIST_IRQn);
    }

    controlBlock.counter++;
}

static void IRQDisable(void)
{
    controlBlock.counter--;
    if(controlBlock.counter == 0)
    {
        qRegBist_IRQEnable(false);
        NVIC_DisableIRQ(BIST_IRQn);
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void qDrvBist_RegisterCallback(qDrvBist_Handler_t type, const qDrvBist_HandlerCb_t pCallback)
{
    Q_ASSERT(type < qDrvBist_HandlerCounts);
    controlBlock.handlers[type] = pCallback;
}

void qDrvBist_UnRegisterCallback(qDrvBist_Handler_t type)
{
    Q_ASSERT(type < qDrvBist_HandlerCounts);
    controlBlock.handlers[type] = NULL;
}

void qDrvBist_IRQEnable(Bool enable)
{
    if(enable)
    {
        IRQEnable();
    }
    else
    {
        IRQDisable();
    }
}

void bist_handler(void)
{

    if(qDrvBist_AdcResultFifoNotEmptyMaskedIRQGet())
    {
        if(controlBlock.handlers[qDrvBist_HandlerAdc])
        {
            controlBlock.handlers[qDrvBist_HandlerAdc]();
        }
    }

    if(qDrvBist_CpCmpOutMaskedIRQGet())
    {
        if(controlBlock.handlers[qDrvBist_HandlerCp])
        {
            controlBlock.handlers[qDrvBist_HandlerCp]();
        }
    }

    if(qDrvBist_PeriodMeasDoneMaskedIRQGet() || qDrvBist_TotalMeasDoneMaskedIRQGet())
    {
        if(controlBlock.handlers[qDrvBist_HandlerObm])
        {
            controlBlock.handlers[qDrvBist_HandlerObm]();
        }
    }

    if(qDrvBist_TsensAboveMaximumMaskedIRQGet() || qDrvBist_TsensBelowMinimumMaskedIRQGet() ||
       qDrvBist_TsensResultFifoNotEmptyMaskedIRQGet())
    {
        if(controlBlock.handlers[qDrvBist_HandlerTsens])
        {
            controlBlock.handlers[qDrvBist_HandlerTsens]();
        }
    }
}
