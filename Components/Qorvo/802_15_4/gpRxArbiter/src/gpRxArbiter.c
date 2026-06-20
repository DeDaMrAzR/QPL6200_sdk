/*
 * Copyright (c) 2013-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpRxArbiter.c
 *
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_RXARBITER

//#define GP_LOCAL_LOG

#include "hal.h"
#include "gpLog.h"
#include "gpRxArbiter.h"
#include "gpHal.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#ifdef GP_RX_ARBITER_DUTY_CYCLE
#if !defined(GP_COMP_GPHAL_ES) || !defined(GP_COMP_GPHAL_ES_ABS_EVENT) || !defined(GP_COMP_GPHAL_ES_REL_EVENT)
#error error: missing diversities for rx arbiter
#endif
#endif

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct{
    UInt8 channel;
    UInt8 rxOnCounter;
    UInt8 priority;
#ifdef GP_RX_ARBITER_DUTY_CYCLE
    Bool dutyCycleEnabled;
    UInt32 dutyCycleOnTime;
    UInt32 dutyCycleOffTime;
    UInt32 startTime;
    UInt16 recurrenceAmount;
#endif //GP_RX_ARBITER_DUTY_CYCLE
}gpRxArbiter_StackDesc_t;

/* Describes the rx state for one srcId in gpHal */
typedef struct srcIdRxState_ {
    gpRxArbiter_StackId_t stackId;
    gpRxArbiter_RadioState_t rxState;
    UInt8 rxChannel;
#ifdef GP_RX_ARBITER_DUTY_CYCLE
    UInt8 onEventIdx;
    UInt8 offEventIdx;
    UInt32 dutyCycleOnTime;
    UInt32 dutyCycleOffTime;
    UInt32 startTime;
    UInt16 recurrenceAmount;
#endif //GP_RX_ARBITER_DUTY_CYCLE
} gpSrcIdRxState_t;


/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/
static void RxArbiter_NotifyFA(gpRxArbiter_StackId_t stackId, UInt8 mode) ;
#ifdef GP_RX_ARBITER_DUTY_CYCLE
static Bool RxArbiter_DutyCycleParamsChanged(const gpRxArbiter_StackDesc_t *stackDesc, const gpSrcIdRxState_t *srcIdState);
#endif
static void RxArbiter_UpdateFA(void);

 /*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

gpRxArbiter_cbSetFaMode_t     gpRxArbiter_setFaModeCallbacks[GP_RX_ARBITER_NUMBER_OF_STACKS] ;
gpRxArbiter_cbChannelUpdate_t gpRxArbiter_channelUpdateCallbacks[GP_RX_ARBITER_NUMBER_OF_STACKS] ;

gpRxArbiter_StackDesc_t gpRxArbiter_StackDesc[GP_RX_ARBITER_NUMBER_OF_STACKS];
gpSrcIdRxState_t gpSrcIdRxState[GP_HAL_MAC_NUMBER_OF_RX_SRCIDS];

#ifdef GP_RX_ARBITER_DUTY_CYCLE
#ifndef GP_DIVERSITY_GPHAL_INTERN
UInt8 gpRxArbiter_AppSleepMode;
#endif //GP_DIVERSITY_GPHAL_INTERN
#endif //GP_RX_ARBITER_DUTY_CYCLE

#ifdef GP_RX_ARBITER_DUTY_CYCLE
#define RX_ARBITER_DUTY_CYCLE_ENABLED(stackId)  gpRxArbiter_StackDesc[ stackId ].dutyCycleEnabled
#else //GP_RX_ARBITER_DUTY_CYCLE
#define RX_ARBITER_DUTY_CYCLE_ENABLED(stackId)  false
#endif //GP_RX_ARBITER_DUTY_CYCLE

#define RX_ARBITER_RECURRENCE_INFINITE 0xFFFF

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static Bool RxArbiter_ValidStackId(gpRxArbiter_StackId_t stackId)
{
    if(stackId < GP_RX_ARBITER_NUMBER_OF_STACKS)
    {
        return true;
    }
    else
    {
        GP_LOG_PRINTF("Invalid Stack",0);
        return false;
    }
}

static void RxArbiter_SetRxOn(UIntLoop srcId, gpRxArbiter_StackId_t requestedStackId, UInt8 rxChannel, Bool rxOn)
{
    if(rxOn)
    {
        gpSrcIdRxState[srcId].rxState = gpRxArbiter_RadioStateOn;
        gpSrcIdRxState[srcId].stackId = requestedStackId;
        gpSrcIdRxState[srcId].rxChannel = rxChannel;
    }
    else
    {
        gpSrcIdRxState[srcId].stackId = gpRxArbiter_StackIdInvalid;
        gpSrcIdRxState[srcId].rxState = gpRxArbiter_RadioStateOff;
    }
    GP_LOG_PRINTF("Rx On:%u ch:%u %i==%i",0, rxOn, rxChannel, srcId, requestedStackId);
    gpHal_SetRxOnWhenIdle(srcId, rxOn, rxChannel);
}

#ifdef GP_RX_ARBITER_DUTY_CYCLE
static Bool RxArbiter_DutyCycleParamsChanged(const gpRxArbiter_StackDesc_t *stackDesc, const gpSrcIdRxState_t *srcIdState)
{
    if (stackDesc->channel != srcIdState->rxChannel)
    {
        return true;
    }
    if (stackDesc->dutyCycleOnTime != srcIdState->dutyCycleOnTime)
    {
        return true;
    }
    if (stackDesc->dutyCycleOffTime != srcIdState->dutyCycleOffTime)
    {
        return true;
    }
    if (stackDesc->startTime != srcIdState->startTime)
    {
        return true;
    }
    if (stackDesc->recurrenceAmount != srcIdState->recurrenceAmount)
    {
        return true;
    }
    return false;
}
#endif

#ifdef GP_RX_ARBITER_DUTY_CYCLE
static void RxArbiter_EnableDutyCycling(gpHal_SourceIdentifier_t srcId, gpRxArbiter_StackId_t stackId, UInt8 rxChannel)
{
    GP_LOG_PRINTF(" RxArbiter_EnableDutyCycling ",0);

    gpSrcIdRxState[srcId].stackId = stackId;
    gpSrcIdRxState[srcId].rxState = gpRxArbiter_RadioStateDutyCycle;
    gpSrcIdRxState[srcId].rxChannel = rxChannel;
    gpSrcIdRxState[srcId].dutyCycleOnTime  = gpRxArbiter_StackDesc[stackId].dutyCycleOnTime;
    gpSrcIdRxState[srcId].dutyCycleOffTime = gpRxArbiter_StackDesc[stackId].dutyCycleOffTime;
    gpSrcIdRxState[srcId].startTime        = gpRxArbiter_StackDesc[stackId].startTime;
    gpSrcIdRxState[srcId].recurrenceAmount = gpRxArbiter_StackDesc[stackId].recurrenceAmount;

    gpHal_EnableRxWindows(srcId, rxChannel,
                            gpRxArbiter_StackDesc[stackId].dutyCycleOnTime,
                            gpRxArbiter_StackDesc[stackId].dutyCycleOffTime,
                            gpRxArbiter_StackDesc[stackId].recurrenceAmount,
                            gpRxArbiter_StackDesc[stackId].startTime
                            );

}

static void RxArbiter_DisableDutyCycling(gpHal_SourceIdentifier_t srcId)
{
    GP_LOG_PRINTF(" RxArbiter_DisableDutyCycling ",0);

    gpHal_DisableRxWindows(srcId);

    gpSrcIdRxState[srcId].stackId = gpRxArbiter_StackIdInvalid;
    gpSrcIdRxState[srcId].rxState = gpRxArbiter_RadioStateOff;
    gpSrcIdRxState[srcId].dutyCycleOnTime  = 0;
    gpSrcIdRxState[srcId].dutyCycleOffTime = 0;
    gpSrcIdRxState[srcId].startTime        = 0;
    gpSrcIdRxState[srcId].recurrenceAmount = 0;
}
#endif //GP_RX_ARBITER_DUTY_CYCLE

static UIntLoop findSrcIdForStackId(gpRxArbiter_StackId_t stackId)
{
    UIntLoop idx=0;
    UInt8 macNumberOfRxSrcIds;

    macNumberOfRxSrcIds = gpHal_GetAvailableSrcIds();
    if (stackId==gpRxArbiter_StackIdInvalid)
    {
      return macNumberOfRxSrcIds;
    }
    while ((idx<macNumberOfRxSrcIds) && (gpSrcIdRxState[idx].stackId != stackId))
    {
        ++idx;
    }
    return idx;
}


static Bool IsRxOn(gpRxArbiter_StackId_t stackId)
{
    UIntLoop idx=findSrcIdForStackId(stackId);
    if (idx < gpHal_GetAvailableSrcIds())
    {
        return gpRxArbiter_RadioStateActive(gpSrcIdRxState[idx].rxState);
    }
    return false;
}

static UIntLoop GatherRxOnStackIds(gpRxArbiter_StackId_t *out)
{
    UIntLoop stackId;
    UIntLoop idx=0;
    for (stackId=0; stackId < GP_RX_ARBITER_NUMBER_OF_STACKS; ++stackId)
    {
        if(RxArbiter_ValidStackId(stackId) &&
            (gpRxArbiter_StackDesc[ stackId ].rxOnCounter || RX_ARBITER_DUTY_CYCLE_ENABLED(stackId)))
        {
            out[idx++] = stackId;
        }
    }
    return idx;
}

static Bool compareStackId(gpRxArbiter_StackId_t sId0, gpRxArbiter_StackId_t sId1, gpRxArbiter_StackId_t requestedStackId)
{
    if (gpRxArbiter_StackDesc[ sId0 ].priority < gpRxArbiter_StackDesc[ sId1 ].priority)
    {
        return true;
    }
    else if (gpRxArbiter_StackDesc[ sId0 ].priority > gpRxArbiter_StackDesc[ sId1 ].priority)
    {
        return false;
    }
    else if (sId1==requestedStackId)
    {
        return false;
    }
    else if (sId0==requestedStackId)
    {
        return true;
    }
    else
    {
#if 1       // If requestedStackId is gpRxArbiter_StackIdInvalid we also give priority to active RX stacks
        // gpRxArbiter_GetCurrentRxOnState needs this to find the highest priority active stack
        Bool isOn_0 = IsRxOn(sId0);
        Bool isOn_1 = IsRxOn(sId1);
        if (requestedStackId == gpRxArbiter_StackIdInvalid && !isOn_0 && isOn_1)
        {
            return false;
        }
        else if (requestedStackId == gpRxArbiter_StackIdInvalid && isOn_0 && !isOn_1)
        {
            return true;
        }
        else
#endif
        if (sId1<sId0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

static UIntLoop ChoosePivot(gpRxArbiter_StackId_t *A, UIntLoop lo, UIntLoop hi, gpRxArbiter_StackId_t requestedStackId)
{
    UIntLoop tmp, mid = lo+(hi-lo)/2;
    if (compareStackId(A[mid], A[lo], requestedStackId))
    {
        tmp = lo;
        lo = mid;
        mid = tmp;
    }
    if (compareStackId(A[hi], A[mid], requestedStackId))
    {
        tmp = hi;
        //hi = mid; //no effect outside function
        mid = tmp;
    }
    if (compareStackId(A[mid], A[lo], requestedStackId))
    {
        tmp = lo;
        //lo = mid; //no effect outside function
        mid = tmp;
    }
    return mid;
}

static UIntLoop PriorityQuickPartition(gpRxArbiter_StackId_t *A, UIntLoop lo, UIntLoop hi, gpRxArbiter_StackId_t requestedStackId)
{
    gpRxArbiter_StackId_t tmp, pivotValue;
    UIntLoop pivotIndex, idx, lastLowIdx = lo;

    pivotIndex = ChoosePivot(A, lo, hi, requestedStackId);
    pivotValue = A[pivotIndex];

    // move pivot to back
    tmp = A[hi];
    A[hi] = A[pivotIndex];
    A[pivotIndex] = tmp;

    for (idx=lo;idx<(hi-lo);++idx)
    {
        if (compareStackId(A[idx], pivotValue, requestedStackId))
        {
            tmp = A[idx];
            A[idx] = A[lastLowIdx];
            A[lastLowIdx++] = tmp;
        }
    }
    tmp = A[hi];
    A[hi] = A[lastLowIdx];
    A[lastLowIdx] = tmp;

    return lastLowIdx;
}

static void PriorityQuickSortStackId(gpRxArbiter_StackId_t *A, UIntLoop lo, UIntLoop hi, gpRxArbiter_StackId_t requestedStackId)
{
    if (lo < hi)
    {
        UIntLoop p = PriorityQuickPartition(A, lo, hi, requestedStackId);
        if (p) // guard against overflow
        {
          PriorityQuickSortStackId(A, lo, p-1, requestedStackId);
        }
        if (p<hi)
        {
          PriorityQuickSortStackId(A, p+1, hi, requestedStackId);
        }
    }
}

static void AssignStackIdToSrcId(gpRxArbiter_StackId_t *out, gpRxArbiter_StackId_t *sortedRxOnStacks, UIntLoop noStackIds)
{
    UIntLoop idx;
    UIntLoop u_idx_0=0;
    UIntLoop u_idx_1=0;
    UIntLoop search_idx=0;
    UInt8 macNumberOfRxSrcIds;

    gpRxArbiter_StackId_t unassigned_0[GP_HAL_MAC_NUMBER_OF_RX_SRCIDS]; // max len == noStackIds but need constant
    gpRxArbiter_StackId_t unassigned_1[GP_HAL_MAC_NUMBER_OF_RX_SRCIDS]; // max len == u_idx_0 but need constant

    MEMSET(unassigned_0, 0xFF, sizeof(unassigned_0));
    MEMSET(unassigned_1, 0xFF, sizeof(unassigned_1));

    macNumberOfRxSrcIds = gpHal_GetAvailableSrcIds();
    /* Assign those stackIds that are already assigned */
    for (idx=0;idx<noStackIds;++idx)
    {
        gpRxArbiter_StackId_t stackId = sortedRxOnStacks[idx];
        UIntLoop srcId = findSrcIdForStackId(stackId);
        if (srcId < macNumberOfRxSrcIds)
        {
            out[srcId] = stackId;
        }
        else
        {
            unassigned_0[u_idx_0++] = stackId;
        }
    }
    /* Assign those stackIds that equal an unused srcId */
    for (idx=0;idx<u_idx_0;++idx)
    {
        gpRxArbiter_StackId_t stackId = unassigned_0[idx];
        if (gpRxArbiter_StackIdInvalid==out[stackId%macNumberOfRxSrcIds])
        {
            out[ stackId % macNumberOfRxSrcIds ] = stackId;
        }
        else
        {
            unassigned_1[u_idx_1++] = stackId;
        }
    }
    /* Assign the remaining unassigned stackIDs,
     * only relevant when GP_HAL_MAC_NUMBER_OF_RX_SRCIDS < GP_RX_ARBITER_NUMBER_OF_STACKS
     */
    GP_ASSERT_DEV_INT((macNumberOfRxSrcIds < GP_RX_ARBITER_NUMBER_OF_STACKS) || (u_idx_1==0));

    for (idx=0;idx<u_idx_1;++idx)
    {
        gpRxArbiter_StackId_t stackId = unassigned_1[idx];
        while (search_idx<macNumberOfRxSrcIds &&
               gpRxArbiter_StackIdInvalid != out[search_idx])
        {
            ++search_idx;
        }
        if (search_idx<macNumberOfRxSrcIds)
        {
            out[search_idx] = stackId;
        }
        else
        {
            ++search_idx;
        }
    }
}

static void RxArbiter_UpdateRadio(gpRxArbiter_StackId_t requestedStackId)
{
    UInt8 stackIdWhichControlsRadio = gpRxArbiter_StackIdInvalid;
    gpRxArbiter_RadioState_t gpRxArbiter_CurrentRadioState;
    UInt8 gpRxArbiter_CurrentRxChannel;
    UIntLoop noStackIds;
    UIntLoop noSrcIds;
    UIntLoop idx;
    gpRxArbiter_StackId_t* sortedRxOnStacks;
    gpRxArbiter_StackId_t rxOnStacks[GP_RX_ARBITER_NUMBER_OF_STACKS];
    gpRxArbiter_StackId_t assignedStackIds[GP_HAL_MAC_NUMBER_OF_RX_SRCIDS];

    noStackIds = GatherRxOnStackIds(rxOnStacks);
    if (noStackIds)
    {
      PriorityQuickSortStackId(rxOnStacks, 0, noStackIds-1, requestedStackId);
    }
    sortedRxOnStacks = rxOnStacks;

    noSrcIds = (gpHal_GetAvailableSrcIds() < noStackIds) ? gpHal_GetAvailableSrcIds() : noStackIds;
    for (idx=0;idx<GP_HAL_MAC_NUMBER_OF_RX_SRCIDS;++idx)
    {
        assignedStackIds[idx] = gpRxArbiter_StackIdInvalid;
    }
    AssignStackIdToSrcId(assignedStackIds,sortedRxOnStacks,noSrcIds);

    // we have found the highest priority stack, now set channel and RxOn,
    for (idx=0; idx < GP_HAL_MAC_NUMBER_OF_RX_SRCIDS;++idx)
    {
        stackIdWhichControlsRadio = assignedStackIds[idx];
        if(stackIdWhichControlsRadio != gpRxArbiter_StackIdInvalid)
            continue;

        gpRxArbiter_CurrentRadioState = gpSrcIdRxState[idx].rxState;

        // no stack found => disable radio
        if(gpRxArbiter_CurrentRadioState == gpRxArbiter_RadioStateOn)
        {
            RxArbiter_SetRxOn(idx, gpRxArbiter_StackIdInvalid, GP_RX_ARBITER_INVALID_CHANNEL, false);
        }
#ifdef GP_RX_ARBITER_DUTY_CYCLE
        else if(gpRxArbiter_CurrentRadioState ==  gpRxArbiter_RadioStateDutyCycle)
        {
            RxArbiter_DisableDutyCycling(idx);
        }
#endif //GP_RX_ARBITER_DUTY_CYCLE
    }

    // set rx on/rxof/duty cycle
    for (idx=0; idx < GP_HAL_MAC_NUMBER_OF_RX_SRCIDS;++idx)
    {
        stackIdWhichControlsRadio = assignedStackIds[idx];
        if(stackIdWhichControlsRadio == gpRxArbiter_StackIdInvalid)
            continue;


        gpRxArbiter_CurrentRadioState = gpSrcIdRxState[idx].rxState;
        gpRxArbiter_CurrentRxChannel = gpRxArbiter_StackDesc[ stackIdWhichControlsRadio ].channel;

        if (GP_RX_ARBITER_INVALID_CHANNEL==gpRxArbiter_CurrentRxChannel)
        {
            gpRxArbiter_CurrentRxChannel = gpSrcIdRxState[idx].rxChannel;
        }
        if (GP_RX_ARBITER_INVALID_CHANNEL==gpRxArbiter_CurrentRxChannel)
        {
            gpRxArbiter_CurrentRxChannel = gpHal_GetRxChannel(idx);
            gpSrcIdRxState[idx].rxChannel = gpRxArbiter_CurrentRxChannel;
            gpRxArbiter_StackDesc[stackIdWhichControlsRadio].channel = gpRxArbiter_CurrentRxChannel;
        }

        switch(gpRxArbiter_CurrentRadioState)
        {
            case gpRxArbiter_RadioStateOff:
            {
                if(gpRxArbiter_StackDesc[ stackIdWhichControlsRadio ].rxOnCounter)
                {
                    RxArbiter_SetRxOn(idx, stackIdWhichControlsRadio, gpRxArbiter_CurrentRxChannel, true);
                }
#ifdef GP_RX_ARBITER_DUTY_CYCLE
                else if(RX_ARBITER_DUTY_CYCLE_ENABLED(stackIdWhichControlsRadio))
                {
                    RxArbiter_EnableDutyCycling(idx, stackIdWhichControlsRadio, gpRxArbiter_CurrentRxChannel);
                }
#endif //GP_RX_ARBITER_DUTY_CYCLE
                break;
            }
            case gpRxArbiter_RadioStateOn:
            {
                if(gpRxArbiter_StackDesc[ stackIdWhichControlsRadio ].rxOnCounter == 0)
                {
                    RxArbiter_SetRxOn(idx, gpRxArbiter_StackIdInvalid, GP_RX_ARBITER_INVALID_CHANNEL, false);
#ifdef GP_RX_ARBITER_DUTY_CYCLE
                    if(RX_ARBITER_DUTY_CYCLE_ENABLED(stackIdWhichControlsRadio))
                    {
                        RxArbiter_EnableDutyCycling(idx, stackIdWhichControlsRadio, gpRxArbiter_CurrentRxChannel);
                    }
#endif //GP_RX_ARBITER_DUTY_CYCLE
                }
                else if (gpSrcIdRxState[idx].rxChannel != gpRxArbiter_CurrentRxChannel)
                {
                    RxArbiter_SetRxOn(idx, stackIdWhichControlsRadio, gpSrcIdRxState[idx].rxChannel, false);
                    RxArbiter_SetRxOn(idx, stackIdWhichControlsRadio, gpRxArbiter_CurrentRxChannel, true);
                }
                break;
            }
#ifdef GP_RX_ARBITER_DUTY_CYCLE
            case gpRxArbiter_RadioStateDutyCycle:
            {
                if(gpRxArbiter_StackDesc[ stackIdWhichControlsRadio ].rxOnCounter)
                {
                    RxArbiter_DisableDutyCycling(idx);
                    RxArbiter_SetRxOn(idx, stackIdWhichControlsRadio, gpRxArbiter_CurrentRxChannel, true);
                }
                else if(!RX_ARBITER_DUTY_CYCLE_ENABLED(stackIdWhichControlsRadio))
                {
                    GP_LOG_PRINTF("gpRxArbiter_RadioStateDutyCycle Duty cycle disable %x ", 2,stackIdWhichControlsRadio);
                    RxArbiter_DisableDutyCycling(idx);
                }
                else if (RxArbiter_DutyCycleParamsChanged(&gpRxArbiter_StackDesc[ stackIdWhichControlsRadio ],
                                          &gpSrcIdRxState[idx]))
                {
                    RxArbiter_DisableDutyCycling(idx);
                    RxArbiter_EnableDutyCycling(idx, stackIdWhichControlsRadio, gpRxArbiter_CurrentRxChannel);
                }
                break;
            }
#endif //GP_RX_ARBITER_DUTY_CYCLE
            default:
            {
                break;
            }
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpRxArbiter_Init(void)
{
    UIntLoop srcId, stackId;
    for(srcId = 0; srcId < GP_HAL_MAC_NUMBER_OF_RX_SRCIDS ; srcId++)
    {
        gpSrcIdRxState[srcId].stackId = gpRxArbiter_StackIdInvalid;
        gpSrcIdRxState[srcId].rxState = gpRxArbiter_RadioStateOff;
        gpSrcIdRxState[srcId].rxChannel = GP_RX_ARBITER_INVALID_CHANNEL;
#ifdef GP_RX_ARBITER_DUTY_CYCLE
        gpSrcIdRxState[srcId].dutyCycleOnTime = 0;
        gpSrcIdRxState[srcId].dutyCycleOffTime = 0;
        gpSrcIdRxState[srcId].startTime        = 0;
        gpSrcIdRxState[srcId].recurrenceAmount = 0;
#endif
    }
    for(stackId = 0; stackId < GP_RX_ARBITER_NUMBER_OF_STACKS ; stackId++)
    {
        gpRxArbiter_setFaModeCallbacks[stackId] = NULL;
        gpRxArbiter_channelUpdateCallbacks[stackId] = NULL;
        gpRxArbiter_StackDesc[stackId].channel = GP_RX_ARBITER_INVALID_CHANNEL;
        gpRxArbiter_StackDesc[stackId].rxOnCounter = 0;
        gpRxArbiter_StackDesc[stackId].priority = 0xff;
#ifdef GP_RX_ARBITER_DUTY_CYCLE
        gpRxArbiter_StackDesc[stackId].dutyCycleEnabled = false;
        gpRxArbiter_StackDesc[stackId].dutyCycleOnTime = 0;
        gpRxArbiter_StackDesc[stackId].dutyCycleOffTime = 0;
        gpRxArbiter_StackDesc[stackId].startTime        = 0;
        gpRxArbiter_StackDesc[stackId].recurrenceAmount = 0;
#endif //GP_RX_ARBITER_DUTY_CYCLE
    }


    // Enable the gpFA module of the first stack. UpdateFA will make changes in enabled gpFA is necessary.
    RxArbiter_NotifyFA(0, gpRxArbiter_FaModeEnabled) ;
}

void gpRxArbiter_DeInit(void)
{
    UIntLoop srcId =0;
    gpRxArbiter_RadioState_t gpRxArbiter_CurrentRadioState;


    for (srcId =0; srcId  < GP_HAL_MAC_NUMBER_OF_RX_SRCIDS; ++srcId)
    {
        if(gpSrcIdRxState[srcId].stackId == gpRxArbiter_StackIdInvalid)
        {
            continue;
        }

        gpRxArbiter_CurrentRadioState = gpSrcIdRxState[srcId ].rxState;

        // no stack found => disable radio
        if(gpRxArbiter_CurrentRadioState == gpRxArbiter_RadioStateOn)
        {
            gpHal_SetRxOnWhenIdle(srcId , false, gpSrcIdRxState[srcId].rxChannel);
        }
#ifdef GP_RX_ARBITER_DUTY_CYCLE
        else if(gpRxArbiter_CurrentRadioState ==  gpRxArbiter_RadioStateDutyCycle)
        {
            gpHal_DisableRxWindows(srcId);
        }
#endif //GP_RX_ARBITER_DUTY_CYCLE
        gpSrcIdRxState[srcId].stackId = gpRxArbiter_StackIdInvalid;
        gpSrcIdRxState[srcId].rxState = gpRxArbiter_RadioStateOff;
    }

}

gpRxArbiter_Result_t gpRxArbiter_ResetStack(gpRxArbiter_StackId_t stackId)
{
    GP_LOG_PRINTF("Reset Stack StId %i",0,stackId);

    if(stackId < GP_RX_ARBITER_NUMBER_OF_STACKS)
    {
        gpRxArbiter_StackDesc[stackId].channel = GP_RX_ARBITER_INVALID_CHANNEL;
        gpRxArbiter_StackDesc[stackId].rxOnCounter = 0;
        gpRxArbiter_StackDesc[stackId].priority = 0xff;
#ifdef GP_RX_ARBITER_DUTY_CYCLE
        gpRxArbiter_StackDesc[stackId].dutyCycleEnabled = false;
        gpRxArbiter_StackDesc[stackId].dutyCycleOnTime  = 0;
        gpRxArbiter_StackDesc[stackId].dutyCycleOffTime = 0;
        gpRxArbiter_StackDesc[stackId].startTime        = 0;
        gpRxArbiter_StackDesc[stackId].recurrenceAmount = 0;
#endif //GP_RX_ARBITER_DUTY_CYCLE

        RxArbiter_UpdateRadio(stackId);

        return gpRxArbiter_ResultSuccess;
    }
    else
    {
        GP_LOG_PRINTF("Reset Stack Invalid StId %i",0,stackId);
        return gpRxArbiter_ResultInvalidArgument;
    }
}

gpRxArbiter_Result_t gpRxArbiter_SetStackChannel(UInt8 channel , gpRxArbiter_StackId_t stackId)
{
    GP_LOG_PRINTF("Set Ch %i, StId %i",0,channel,stackId);

    if(!GP_RXARBITER_CHECK_CHANNEL_VALID(channel) || !RxArbiter_ValidStackId(stackId))
    {
        return gpRxArbiter_ResultInvalidArgument;
    }

    if(gpRxArbiter_StackDesc[stackId].channel != channel)
    {
        gpRxArbiter_StackDesc[stackId].channel = channel;
        RxArbiter_UpdateRadio(stackId);
    }

#ifdef GP_HAL_DIVERSITY_DUTY_CYCLE
    gpHal_BackupRxChannel(findSrcIdForStackId(stackId),channel);
#endif //GP_HAL_DIVERSITY_DUTY_CYCLE

    return gpRxArbiter_ResultSuccess;
}

UInt8 gpRxArbiter_GetStackChannel(gpRxArbiter_StackId_t stackId)
{
    if(RxArbiter_ValidStackId(stackId))
    {
        return gpRxArbiter_StackDesc[stackId].channel;
    }
    else
    {
        return GP_RX_ARBITER_INVALID_CHANNEL;
    }
}

Bool gpRxArbiter_IsAnActiveChannel(gpRxArbiter_StackId_t stackId, UInt8 channel)
{

    return (gpRxArbiter_GetStackChannel(stackId) == channel);
}

gpRxArbiter_RadioState_t gpRxArbiter_GetCurrentRxOnState(void)
{
    UIntLoop noStackIds;
    gpRxArbiter_StackId_t rxOnStacks[GP_RX_ARBITER_NUMBER_OF_STACKS];
    gpRxArbiter_StackId_t *sortedRxOnStacks;

    noStackIds = GatherRxOnStackIds(rxOnStacks);
    if (noStackIds > 0)
    {
        PriorityQuickSortStackId(rxOnStacks, 0, noStackIds-1, gpRxArbiter_StackIdInvalid);
    }
    sortedRxOnStacks = rxOnStacks;

    if (noStackIds > 0)
    {
        UIntLoop srcId = findSrcIdForStackId(sortedRxOnStacks[0]);
        if(srcId < gpHal_GetAvailableSrcIds())
        {
            return gpSrcIdRxState[srcId].rxState;
        }
    }
    return gpRxArbiter_RadioStateOff;
}

UInt8 gpRxArbiter_GetCurrentRxChannel(void)
{
    UIntLoop noStackIds;
    gpRxArbiter_StackId_t rxOnStacks[GP_RX_ARBITER_NUMBER_OF_STACKS];
    gpRxArbiter_StackId_t *sortedRxOnStacks;

    noStackIds = GatherRxOnStackIds(rxOnStacks);

    if (noStackIds > 0)
    {
        PriorityQuickSortStackId(rxOnStacks, 0, noStackIds-1, gpRxArbiter_StackIdInvalid);
    }
    sortedRxOnStacks = rxOnStacks;

    if (noStackIds > 0)
    {
        UIntLoop srcId = findSrcIdForStackId(sortedRxOnStacks[0]);
        if(srcId < gpHal_GetAvailableSrcIds())
        {
            return gpSrcIdRxState[srcId].rxChannel;
        }
    }
    return GP_RX_ARBITER_INVALID_CHANNEL;
}

gpRxArbiter_Result_t gpRxArbiter_SetStackRxOn(Bool enable , gpRxArbiter_StackId_t stackId)
{
    if(RxArbiter_ValidStackId(stackId) && !RX_ARBITER_DUTY_CYCLE_ENABLED(stackId))
    {
        GP_LOG_PRINTF("RxOn:%u counter:%u StId:%u",0, enable, gpRxArbiter_StackDesc[stackId].rxOnCounter, stackId);
        if(enable)
        {
            gpRxArbiter_StackDesc[stackId].rxOnCounter++;
        }
        else
        {
            if(gpRxArbiter_StackDesc[stackId].rxOnCounter)
            {
                gpRxArbiter_StackDesc[stackId].rxOnCounter--;
            }
            else
            {
                GP_LOG_PRINTF("RxOn invalid param",0);
                return gpRxArbiter_ResultInvalidArgument;
            }
        }
        RxArbiter_UpdateRadio(stackId);
        return gpRxArbiter_ResultSuccess;
    }
    else
    {
        GP_LOG_PRINTF("RxOn:%u StId:%u invalid param",0, enable, stackId);
        return gpRxArbiter_ResultInvalidArgument;
    }
}

static void RxArbiter_UpdateChannel(gpRxArbiter_StackId_t highestPriorityStack)
{
    UInt8 stackId ;
    UInt8 gpRxArbiter_CurrentRxChannel;
    UIntLoop srcId;

    GP_LOG_PRINTF("RxArbiter_UpdateChannel stk=%i", 0, highestPriorityStack) ;

    srcId = findSrcIdForStackId(highestPriorityStack);
    if (srcId < gpHal_GetAvailableSrcIds())
    {
        gpRxArbiter_CurrentRxChannel = gpSrcIdRxState[srcId].rxChannel;
    }
    else
    {
        gpRxArbiter_CurrentRxChannel = GP_RX_ARBITER_INVALID_CHANNEL;
    }

    // inform stacks that the listening channel has changed
    for (stackId = 0 ; stackId < GP_RX_ARBITER_NUMBER_OF_STACKS ; stackId++)
    {
        /* Only stackIds which do not control the radio should be notified
         * We only pass the channel of the highest priority stack as argument,
         * TODO: should be list of all rx channels
         */
        srcId = findSrcIdForStackId(stackId);
        if (srcId >= GP_HAL_MAC_NUMBER_OF_RX_SRCIDS)
        {
            gpRxArbiter_cbChannelUpdate_t func ;

            func = gpRxArbiter_channelUpdateCallbacks[stackId] ;

            if (func && (gpRxArbiter_StackDesc[stackId].channel != GP_RX_ARBITER_INVALID_CHANNEL))
            {
                GP_LOG_PRINTF("Updating channel to %i (stk=%i)", 0, gpRxArbiter_CurrentRxChannel, stackId) ;

                func(stackId, gpRxArbiter_CurrentRxChannel) ;
            }
        }
    }
}

static void RxArbiter_NotifyFA(gpRxArbiter_StackId_t stackId, UInt8 mode)
{
    if (RxArbiter_ValidStackId(stackId))
    {
        // use function pointer to either invoke callback in current address space or in
        // server wrapper

        gpRxArbiter_cbSetFaMode_t func ;

        func = gpRxArbiter_setFaModeCallbacks[stackId] ;

        if (func && (gpRxArbiter_StackDesc[stackId].channel != GP_RX_ARBITER_INVALID_CHANNEL))
        {
            func(stackId, mode) ;
        }
    }
}

// working assumption: stackId provided to this function represents the stack that controls the radio
static void RxArbiter_UpdateFA(void)
{
    UInt8 stackId ;

    GP_LOG_PRINTF("RxArbiter_UpdateFA", 0);

    // Enable gpFA for the stacks that controls the radio, disable the other gpFAs
    for (stackId = 0 ; stackId < GP_RX_ARBITER_NUMBER_OF_STACKS ; stackId++)
    {
        UIntLoop srcId = findSrcIdForStackId(stackId);
        if (gpHal_GetAvailableSrcIds() <= srcId)
        {
            GP_LOG_PRINTF("gpFA disabled (stk %i, srcId=%i, NUM=%i)", 0, stackId, srcId, gpHal_GetAvailableSrcIds()) ;

            RxArbiter_NotifyFA(stackId, gpRxArbiter_FaModeDisabled) ;
        }
        else
        {
            GP_LOG_PRINTF("gpFa enabled (stk=%i, srcId=%i)", 0, stackId, srcId) ;

            RxArbiter_NotifyFA(stackId, gpRxArbiter_FaModeEnabled) ;
        }
    }
}

// 0 is highest priority, 0xFF is lowest priority
gpRxArbiter_Result_t gpRxArbiter_SetStackPriority(UInt8 priority , gpRxArbiter_StackId_t stackId)
{
    if(RxArbiter_ValidStackId(stackId))
    {
        UInt8 stack,
              highestPriority,
              highestPriorityStack ;

        gpRxArbiter_StackDesc[stackId].priority = priority;
        RxArbiter_UpdateRadio(stackId);

        // turn on gpFA of the stack that controls the radio; turn off gpFA of all the rest
        // prevent a storm of updates due to duty cycling by only sending out Fa mode
        // updates in case of stack priority change

        // find highest priority stack
        highestPriorityStack = gpRxArbiter_StackIdInvalid ;
        /* initialize priority to lowest value */
        highestPriority = 0xFF;
        for (stack = 0 ; stack < GP_RX_ARBITER_NUMBER_OF_STACKS ; stack++)
        {
            UIntLoop srcId = findSrcIdForStackId(stack);
            if (srcId >= gpHal_GetAvailableSrcIds())
            {
                continue;
            }
            if (highestPriorityStack == gpRxArbiter_StackIdInvalid ||
                gpRxArbiter_StackDesc[stack].priority < highestPriority)
            {
                highestPriorityStack = stack ;
                highestPriority = gpRxArbiter_StackDesc[stack].priority ;
            }
        }

        GP_LOG_PRINTF("gpRxArbiter_SetStackPriority prio=%i stk=%i", 0, priority, stackId) ;

        RxArbiter_UpdateFA() ;
        RxArbiter_UpdateChannel(highestPriorityStack) ;

        return gpRxArbiter_ResultSuccess;
    }
    else
    {
        return gpRxArbiter_ResultInvalidArgument;
    }
}

// register function to handle channel change; this could be a function in an application
// like RF4CE (when server/client wrappers are probably not needed, so that the argument points
// to a function in the address space of program linking this code) or a ZigBeePro application
// where the wrappers are probably needed, in which case the argument points to a function in the
// server wrapper.
gpRxArbiter_Result_t gpRxArbiter_RegisterChannelUpdateCallback(gpRxArbiter_StackId_t stackId, gpRxArbiter_cbChannelUpdate_t cb)
{
    gpRxArbiter_Result_t result = gpRxArbiter_ResultInvalidArgument ;

    GP_ASSERT_DEV_INT(GP_RX_ARBITER_NUMBER_OF_STACKS > stackId) ;

    if (cb && (gpRxArbiter_channelUpdateCallbacks[stackId] == NULL))
    {
        gpRxArbiter_channelUpdateCallbacks[stackId] = cb ;

        result = gpRxArbiter_ResultSuccess ;
    }

    return result ;
}

gpRxArbiter_Result_t gpRxArbiter_DeRegisterChannelUpdateCallback(gpRxArbiter_StackId_t stackId)
{
    gpRxArbiter_Result_t result = gpRxArbiter_ResultInvalidArgument ;

    GP_ASSERT_DEV_INT(GP_RX_ARBITER_NUMBER_OF_STACKS > stackId) ;

    if (gpRxArbiter_channelUpdateCallbacks[stackId] != NULL)
    {
        gpRxArbiter_channelUpdateCallbacks[stackId] = NULL ;
        result = gpRxArbiter_ResultSuccess ;
    }

    return result ;
}

// register function to handle Fa mode change; this could be a function in an application
// like RF4CE (when server/client wrappers are probably not needed, so that the argument points
// to a function in the address space of program linking this code) or a ZigBeePro application
// where the wrappers are probably needed, in which case the argument points to a function in the
// server wrapper.
gpRxArbiter_Result_t gpRxArbiter_RegisterSetFaModeCallback(gpRxArbiter_StackId_t stackId, gpRxArbiter_cbSetFaMode_t cb)
{
    gpRxArbiter_Result_t result = gpRxArbiter_ResultInvalidArgument ;

    GP_ASSERT_DEV_INT(GP_RX_ARBITER_NUMBER_OF_STACKS > stackId) ;

    if (cb && (gpRxArbiter_setFaModeCallbacks[stackId] == NULL))
    {
        gpRxArbiter_setFaModeCallbacks[stackId] = cb ;

        result = gpRxArbiter_ResultSuccess ;
    }

    return result ;

}

Bool gpRxArbiter_GetStackRxOn(gpRxArbiter_StackId_t stackId)
{
    if(RxArbiter_ValidStackId(stackId))
    {
        return gpRxArbiter_StackDesc[stackId].rxOnCounter;
     }
    else
    {
        return false;
    }
}

#ifdef GP_RX_ARBITER_DUTY_CYCLE
gpRxArbiter_Result_t gpRxArbiter_SetDutyCycleOnOffTimes(UInt32 onTimeUs , UInt32 offTimeUs , gpRxArbiter_StackId_t stackId)
{
    if(RxArbiter_ValidStackId(stackId))
    {
        UInt32 startTime;
        gpHal_GetTime(&startTime); // start time = now!

        gpRxArbiter_StackDesc[stackId].dutyCycleOnTime = onTimeUs;
        gpRxArbiter_StackDesc[stackId].dutyCycleOffTime = offTimeUs;
        gpRxArbiter_StackDesc[stackId].startTime        = startTime;
        gpRxArbiter_StackDesc[stackId].recurrenceAmount = RX_ARBITER_RECURRENCE_INFINITE;
        return gpRxArbiter_ResultSuccess;
    }
    else
    {
        return gpRxArbiter_ResultInvalidArgument;
    }
}

gpRxArbiter_Result_t gpRxArbiter_EnableDutyCycling(Bool enable , gpRxArbiter_StackId_t stackId)
{
    if(!RxArbiter_ValidStackId(stackId))
    {
        return gpRxArbiter_ResultInvalidArgument;
    }

    GP_LOG_PRINTF("gpRxArbiter_EnableDutyCycling enable %x rxCounter %x dutycycle %x  ",4, enable, gpRxArbiter_StackDesc[ stackId ].rxOnCounter, gpRxArbiter_StackDesc[ stackId ].dutyCycleEnabled);

    // radio needs to be disabled for this stackId.
    if(gpRxArbiter_StackDesc[ stackId ].rxOnCounter)
    {
        return gpRxArbiter_ResultInvalidArgument;
    }
    //only enable if not already enabled
    if(enable)
    {
        if(RX_ARBITER_DUTY_CYCLE_ENABLED(stackId))
        {
            return gpRxArbiter_ResultInvalidArgument;
        }
    }
    else
    {
        if(!RX_ARBITER_DUTY_CYCLE_ENABLED(stackId))
        {
            return gpRxArbiter_ResultInvalidArgument;
        }

    }
    gpRxArbiter_StackDesc[stackId].dutyCycleEnabled = enable;
    RxArbiter_UpdateRadio(stackId);
    return gpRxArbiter_ResultSuccess;

}
#endif //GP_RX_ARBITER_DUTY_CYCLE

Bool gpRxArbiter_GetDutyCycleEnabled(gpRxArbiter_StackId_t stackId)
{
    if(RxArbiter_ValidStackId(stackId))
    {
        return RX_ARBITER_DUTY_CYCLE_ENABLED(stackId);
    }
    else
    {
        return false;
    }
}

#ifdef GP_RX_ARBITER_DUTY_CYCLE
gpRxArbiter_Result_t gpRxArbiter_EnableRxWindows(UInt8 channel, UInt32 dutyCycleOnTime, UInt32 dutyCyclePeriod, UInt16 recurrenceAmount, UInt32 startTime, gpRxArbiter_StackId_t stackId)
{
    gpRxArbiter_Result_t res = gpRxArbiter_SetStackChannel(channel , stackId);
    if(res!=gpRxArbiter_ResultSuccess)
    {
        return res;
    }

    if(!RxArbiter_ValidStackId(stackId))
    {
        return gpRxArbiter_ResultInvalidArgument;
    }

    gpRxArbiter_StackDesc[stackId].dutyCycleOnTime  = dutyCycleOnTime;
    gpRxArbiter_StackDesc[stackId].dutyCycleOffTime = dutyCyclePeriod;
    gpRxArbiter_StackDesc[stackId].startTime        = startTime;
    gpRxArbiter_StackDesc[stackId].recurrenceAmount = recurrenceAmount;

    GP_LOG_PRINTF("gpRxArbiter_EnableRxWindows rxCounter %x dutycycle %x  ",0, gpRxArbiter_StackDesc[ stackId ].rxOnCounter, gpRxArbiter_StackDesc[ stackId ].dutyCycleEnabled);
    GP_LOG_PRINTF("gpRxArbiter_EnableRxWindows recurrenceAmount %x  ",0, gpRxArbiter_StackDesc[ stackId ].recurrenceAmount);

    // radio needs to be disabled for this stackId.
    if(gpRxArbiter_StackDesc[ stackId ].rxOnCounter)
    {
        return gpRxArbiter_ResultInvalidArgument;
    }

    gpRxArbiter_StackDesc[stackId].dutyCycleEnabled = true;
    RxArbiter_UpdateRadio(stackId);

    return gpRxArbiter_ResultSuccess;
}

gpRxArbiter_Result_t gpRxArbiter_DisableRxWindows(gpRxArbiter_StackId_t stackId)
{
    if(!RxArbiter_ValidStackId(stackId))
    {
        return gpRxArbiter_ResultInvalidArgument;
    }

    gpRxArbiter_StackDesc[stackId].dutyCycleOnTime  = 0;
    gpRxArbiter_StackDesc[stackId].dutyCycleOffTime = 0;
    gpRxArbiter_StackDesc[stackId].startTime        = 0;
    gpRxArbiter_StackDesc[stackId].recurrenceAmount = 0;

    GP_LOG_PRINTF("gpRxArbiter_DisableRxWindows rxCounter %x dutycycle %x  ",0, gpRxArbiter_StackDesc[ stackId ].rxOnCounter, gpRxArbiter_StackDesc[ stackId ].dutyCycleEnabled);

    // radio needs to be disabled for this stackId.
    if(gpRxArbiter_StackDesc[ stackId ].rxOnCounter)
    {
        return gpRxArbiter_ResultInvalidArgument;
    }

    //only disable if not already disabled
    if(!RX_ARBITER_DUTY_CYCLE_ENABLED(stackId))
    {
        return gpRxArbiter_ResultInvalidArgument;
    }

    gpRxArbiter_StackDesc[stackId].dutyCycleEnabled = false;
    RxArbiter_UpdateRadio(stackId);
    return gpRxArbiter_ResultSuccess;

}
#endif //def GP_RX_ARBITER_DUTY_CYCLE
