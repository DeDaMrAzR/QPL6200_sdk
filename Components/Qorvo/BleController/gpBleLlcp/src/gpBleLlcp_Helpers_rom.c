/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
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

#ifdef GP_DIVERSITY_JUMPTABLES
#define GP_DIVERSITY_ROM_CODE
#endif //def GP_DIVERSITY_JUMPTABLES

//#define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLELLCP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpBle.h"
#include "gpBleConfig.h"
#include "gpBleComps.h"
#include "gpBleDataChannelRxQueue.h"
#include "gpBleDataChannelTxQueue.h"
#include "gpBleDataCommon.h"
#include "gpBleDataRx.h"
#include "gpBleDataTx.h"
#include "gpBleLlcp.h"
#include "gpBleLlcpFramework.h"
#include "gpBleLlcpProcedures.h"
#include "gpBle_defs.h"
#include "gpPoolMem.h"
#include "gpSched.h"
#include "gpRandom.h"
#include "gpLog.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

// various

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

Bool Ble_IsConnectionIntervalValid(UInt16 connIntervalMin, UInt16 connIntervalMax)
{
    if(connIntervalMin > connIntervalMax)
    {
        GP_LOG_PRINTF("connIntervalMin (%x) > connIntervalMax (%x)", 0,connIntervalMin,connIntervalMax);
        return false;
    }

    if(!RANGE_CHECK(connIntervalMin, BLE_INITIATOR_CONN_INTERVAL_RANGE_MIN, BLE_INITIATOR_CONN_INTERVAL_RANGE_MAX))
    {
        GP_LOG_PRINTF("connIntervalMin not in range: %x <= %x <= %x", 0, BLE_INITIATOR_CONN_INTERVAL_RANGE_MIN, connIntervalMin, BLE_INITIATOR_CONN_INTERVAL_RANGE_MAX);
        return false;
    }

    if(!RANGE_CHECK(connIntervalMax, BLE_INITIATOR_CONN_INTERVAL_RANGE_MIN, BLE_INITIATOR_CONN_INTERVAL_RANGE_MAX))
    {
        GP_LOG_PRINTF("connIntervalMax not in range: %x <= %x <= %x", 0, BLE_INITIATOR_CONN_INTERVAL_RANGE_MIN, connIntervalMax, BLE_INITIATOR_CONN_INTERVAL_RANGE_MAX);
        return false;
    }

    return true;
}
