/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_DEFS.h
 *
 *  This file contains internal definitions of the GPHAL.
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

#ifndef _GPHAL_KX_RT_EVENTS_H_
#define _GPHAL_KX_RT_EVENTS_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qv_rt_nrt_common.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

// Value to indicate a timestamp is not relevant. Note this is a valid value, but the purpose is to indicate it will be ignored in further processing.
#define GP_HAL_RT_TIMESTAMP_NOT_IMPORTANT   0xFFFFFFFF

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_RtEventsInit(void);

rt_event_handle_t gpHal_RtEventsAllocate(void);
void gpHal_RtEventsFree(rt_event_handle_t event_handle);
Bool gpHal_RtEventsMultipleAllocated(void);

rt_event_handle_t gpHal_RtEventTimedTxAllocate(void);
void gpHal_RtEventTimedTxRelease(rt_event_handle_t eventHandle);

rt_event_handle_t gpHal_RtEventCoexExtAllocate(void);
void gpHal_RtEventCoexExtRelease(rt_event_handle_t eventHandle);
#endif //_GPHAL_KX_RT_EVENTS_H_
