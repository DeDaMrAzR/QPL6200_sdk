/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2024, Qorvo Inc
 *
 * gpHal_MAC.c
 *   This file contains the implementation of the MAC functions
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
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "gpPd.h"
#include "gpSched.h"

#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_Pbm.h"
#include "gpHal_MAC.h"
#include "gpHal_kx_Phy.h"
#include "gpHal_kx_Ipc.h"
// GP hardware dependent register definitions
#include "gpHal_HW.h"
#include "gpHal_reg.h"

// Debug
#include "hal.h"
#include "gpBsp.h"
#include "gpAssert.h"
#include "gpLog.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#ifdef GP_HAL_DIVERSITY_DUTY_CYCLE
typedef struct {
    gpHal_AbsoluteEventId_t on;
    gpHal_AbsoluteEventId_t off;
} gpHal_DutyCyclingEvent_t;
#endif // GP_HAL_DIVERSITY_DUTY_CYCLE

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#ifdef GP_HAL_DIVERSITY_DUTY_CYCLE
gpHal_DutyCyclingEvent_t gpHal_DutyCyclingEvents[GP_HAL_MAC_NUMBER_OF_RX_SRCIDS];
const UInt8 gpHal_RxUncertainty = 1; // 10 us
#endif // GP_HAL_DIVERSITY_DUTY_CYCLE

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

#ifdef GP_HAL_DIVERSITY_DUTY_CYCLE
static void gpHalMac_InitDutyCycling(void);
#endif // GP_HAL_DIVERSITY_DUTY_CYCLE

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  INIT
//-------------------------------------------------------------------------------------------------------
void gpHal_InitMAC_RxWindows(void)
{
#ifdef GP_HAL_DIVERSITY_DUTY_CYCLE
    gpHalMac_InitDutyCycling();
#endif // GP_HAL_DIVERSITY_DUTY_CYCLE
}

//-------------------------------------------------------------------------------------------------------
//  MAC RX WINDOW FUNCTIONS
//-------------------------------------------------------------------------------------------------------

#ifdef GP_HAL_DIVERSITY_DUTY_CYCLE
void gpHalMac_InitDutyCycling(void)
{
    UIntLoop i;

    for(i = 0; i < GP_HAL_MAC_NUMBER_OF_RX_SRCIDS; i++)
    {
        gpHal_DutyCyclingEvents[i].on = GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID;
        gpHal_DutyCyclingEvents[i].off = GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID;
    }
}

void gpHal_EnableCsl(UInt16 dutyCyclePeriod)
{
    GP_WB_WRITE_ZBFILT_CSL_PERIOD(dutyCyclePeriod);
    if(dutyCyclePeriod > 0)
    {
        // Assume that the Csl sample time will be set before any rx window is started!
        GP_WB_WRITE_ZBFILT_ENH_ACK_INSERT_CSL_IE(1);
    }
    else
    {
        // Stop CSL IE insertion
        GP_WB_WRITE_ZBFILT_ENH_ACK_INSERT_CSL_IE(0);
    }
}

void gpHal_UpdateCslSampleTime(UInt32 nextCslSampleTime)
{
    GP_WB_WRITE_ZBFILT_CSL_SAMPLETIME(nextCslSampleTime);
}

void gpHal_EnableRxWindows(gpHal_SourceIdentifier_t srcId, UInt8 channel, UInt32 dutyCycleOnTime,
                           UInt32 dutyCyclePeriod, UInt16 recurrenceAmount, UInt32 startTime)
{
    UInt8 checkChannel;

    GP_ASSERT_DEV_INT(srcId < GP_HAL_MAC_NUMBER_OF_RX_SRCIDS);
    /* only 1-shot rx windows are supported */
    GP_ASSERT_DEV_EXT(dutyCycleOnTime == dutyCyclePeriod);
    GP_ASSERT_DEV_EXT(recurrenceAmount == 0);

    // Set Ack transmit power
    if(gpHalMac_GetChannel_LowTXPower(&checkChannel))
    {
        gpHalMac_ApplyAckTransmitSettings(srcId, checkChannel);
    }
    else
    {
        gpHalMac_ApplyAckTransmitSettings(srcId, channel);
    }

    UInt8 pbmHandle;
    // selecting the minimum PBM size.
    // We don't need the PBM data itself, only the info struct
    // but there's a 1-on-1 mapping between both, so we need to reserve a PBM.
    pbmHandle = gpHal_GetHandle(GPHAL_PBM_15_4_MAX_SIZE);

    GP_ASSERT_DEV_INT(pbmHandle != GP_PBM_INVALID_HANDLE);
    // or return busy ???

    gpHal_Address_t optsbase = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(pbmHandle);
    GP_WB_WRITE_ZB_FORMAT_TIMED_RX_RX_WD_STARTTIME(optsbase, startTime);
    GP_WB_WRITE_ZB_FORMAT_TIMED_RX_RX_WD_DURATION(optsbase, dutyCycleOnTime);
    GP_WB_WRITE_ZB_FORMAT_TIMED_RX_CHANNEL_IDX(optsbase, srcId);
    GP_WB_WRITE_ZB_FORMAT_TIMED_RX_PRIORITY_FIELDS(optsbase, 0);
    UInt16 optsOffset = ak_rap_rt_to_ptr(optsbase);
    gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_ZB_TIMED_RX, sizeof(optsOffset), (UInt8*)&optsOffset);
    gpHal_IncrementMacQueueUsage();
    GP_LOG_PRINTF("Rx win call h=%d", 0, pbmHandle);
}

void gpHal_DisableRxWindows(gpHal_SourceIdentifier_t srcId)
{
    // Check if busy
    GP_ASSERT_DEV_INT(srcId < GP_HAL_MAC_NUMBER_OF_RX_SRCIDS);

    // Only single-shots windows supported ATM
    // No need to do anything in this function.
}

void gpHal_BackupRxChannel(gpHal_SourceIdentifier_t srcId, UInt8 channel)
{
}
#endif // GP_HAL_DIVERSITY_DUTY_CYCLE

// callback from RT after rx window completion.
void gpHal_HandleTimedRxDoneZb(UInt8 PBMentry)
{
    GP_LOG_PRINTF("Rx win h=%d returned %x", 0, PBMentry,
                  GP_WB_READ_ZB_FORMAT_TIMED_RX_RETURN_CODE(GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry)));

    gpHal_DecrementMacQueueUsage();
    gpHal_FreeHandle(PBMentry);
}

#ifdef GP_HAL_DIVERSITY_DUTY_CYCLE
UInt8 gpHal_GetRxUncertainty(void)
{
    return gpHal_RxUncertainty;
}
#endif // GP_HAL_DIVERSITY_DUTY_CYCLE
