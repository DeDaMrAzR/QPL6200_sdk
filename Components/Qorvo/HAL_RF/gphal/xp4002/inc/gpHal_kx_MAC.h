/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
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
 *
 */

#ifndef _GPHAL_KX_MAC_H_
#define _GPHAL_KX_MAC_H_


#include "gpHal_Statistics.h"
#include "gpHal_RtEvents.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GPHAL_CCA_THRESHOLD_VALUE       (-75)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern gpHal_StatisticsMacCounter_t gpHal_StatisticsMacCounters;
extern UInt8 gpHal_MacState;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

void gpHal_DataRequest_base(UInt8 pbmHandle);
void gpHal_DataRequest_PrepareTimedTx(UInt8 pbmHandle);
void gpHal_DataRequest_ScheduleTimedTx(rt_event_handle_t eventId, UInt8 pbmHandle, UInt32 exec_time);

void gpHal_InitMAC(void);

#ifdef __cplusplus
}
#endif

#endif //_GPHAL_KX_MAC_H_
