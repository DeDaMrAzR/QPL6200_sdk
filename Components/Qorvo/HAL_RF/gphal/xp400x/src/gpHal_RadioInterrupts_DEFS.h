/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpHal.h"
#include "gpHal_DEFS.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

UInt8 gpHal_RadioInterruptsGetPbmFromQueueHead(UInt32 queuePtr);

// Individual interrupt handling functions
void gpHal_RadioInterruptsHandleBleDataConfBle(void);
void gpHal_RadioInterruptsHandleDataIndBle(void);
Bool gpHal_RadioInterruptsHandleAdvIndBle(void);
void gpHal_RadioInterruptsHandleSubevDone(void);
void gpHal_RadioInterruptsHandleEventProcessed(void);
void gpHal_RadioInterruptsHandleLastEvent(void);
void gpHal_RadioInterruptsHandleStat(void);
void gpHal_RadioInterruptsHandleScanReqRx(void);
void gpHal_RadioInterruptsHandleIsoRx(void);
void gpHal_RadioInterruptsHandleDataConf0Zb(void);
void gpHal_RadioInterruptsHandleDataConf1Zb(void);
void gpHal_RadioInterruptsHandleDataIndZb(void);
void gpHal_RadioInterruptsHandleTimedRxDoneZb(void);

void gpHal_IsrHighPriorityInterrupt(UInt32 hpIntMask);
