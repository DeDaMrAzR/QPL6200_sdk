/*
 * Copyright (c) 2017, Qorvo Inc
 *
 *   Bluetooth Low Energy interface
 *   Implementation of gpBle
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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

//#define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLE

#include "gpBle.h"
#include "gpBleAddressResolver.h"
#include "gpBleConfig.h"
#include "gpBle_defs.h"
#include "gpBleActivityManager.h"
#include "gpHci_Includes.h"
#include "gpLog.h"
#include "gpPd.h"
#include "gpHal.h"
#include "gpSched.h"
#include "gpPoolMem.h"
#include "gpAssert.h"

#if defined(GP_DIVERSITY_BLE_BROADCASTER) || defined(GP_DIVERSITY_BLE_PERIPHERAL)
#include "gpBleAdvertiser.h"
#endif //GP_DIVERSITY_BLE_BROADCASTER || GP_DIVERSITY_BLE_PERIPHERAL

#if defined(GP_DIVERSITY_BLE_OBSERVER) 
#include "gpBleScanner.h"
#endif //GP_DIVERSITY_BLE_OBSERVER || GP_DIVERSITY_BLE_CENTRAL

#if defined(GP_DIVERSITY_BLE_PERIPHERAL)
#include "gpBleInitiator.h"
#include "gpBleLlcp.h"
#include "gpBleLlcpProcedures.h"
#include "gpBleDataChannelRxQueue.h"
#include "gpBleDataChannelTxQueue.h"
#include "gpBleDataCommon.h"
#include "gpBleDataRx.h"
#include "gpBleDataTx.h"
#endif //GP_DIVERSITY_BLE_CENTRAL || GP_DIVERSITY_BLE_PERIPHERAL

#ifdef GP_DIVERSITY_BLE_DIRECTTESTMODE_SUPPORTED
#include "gpBleTestMode.h"
#endif // GP_DIVERSITY_BLE_DIRECTTESTMODE_SUPPORTED


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    Ble_MasterSca_t sca;
    UInt16 ppmMin;
    UInt16 ppmMax;
} Ble_ScaPpmMapping_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// Spec mapping between sca and ppm
static const Ble_ScaPpmMapping_t Ble_ScaPpmMapping[]
#if defined(GP_DIVERSITY_ROM_CODE)
LINKER_SECTION(".rom")
#endif //defined(GP_DIVERSITY_ROM_CODE)
=
{
    {BLE_MASTER_SCA_0, 251, 500}, // max
    {BLE_MASTER_SCA_1, 151, 250},
    {BLE_MASTER_SCA_2, 101, 150},
    {BLE_MASTER_SCA_3,  76, 100},
    {BLE_MASTER_SCA_4,  51,  75},
    {BLE_MASTER_SCA_5,  31,  50},
    {BLE_MASTER_SCA_6,  21,  30},
    {BLE_MASTER_SCA_7,   0,  20}  // min
};

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void Ble_EventSpecificHandling(gpBle_EventBuffer_t* pEventBuf, gpHci_CommandOpCode_t opCode, gpHci_Result_t result)
{
    switch(pEventBuf->eventCode)
    {
        case gpHci_EventCode_CommandComplete:
        {
            pEventBuf->payload.commandCompleteParams.result = result;
            pEventBuf->payload.commandCompleteParams.numHciCmdPackets = 0x01;
            pEventBuf->payload.commandCompleteParams.opCode = opCode;
            break;
        }
        case gpHci_EventCode_CommandStatus:
        {
            pEventBuf->payload.commandStatusParams.status = result;
            pEventBuf->payload.commandStatusParams.numHciCmdPackets = 0x01;
            pEventBuf->payload.commandStatusParams.opCode = opCode;
            break;
        }
        default:
        {
            GP_LOG_PRINTF("No handling needed/implemented for eventCode: %x",0,pEventBuf->eventCode);
            break;
        }
    }
}


/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

// Old function - only kept for older SW that uses ROM
void Ble_ReadConnReqFromPd(gpPd_Loh_t* pPdLoh, Ble_ConnReqPdu_t* pConnReq)
{
    UInt8 hopAndSca;

    // Get the header (pd will point to payload)
    Ble_ParseAdvPdHeader(pPdLoh, &pConnReq->pduHeader);

    // We always expect the PDU type to be a CONN_REQ
    // This cannot be triggered by invalid RF packets ==> assert
    GP_ASSERT_DEV_INT(pConnReq->pduHeader.pduType == Ble_AdvertisingPduType_ConnectRequest);

    // Read addresses
    Ble_ReadAddressFromPd(pPdLoh, &pConnReq->initAddress);
    Ble_ReadAddressFromPd(pPdLoh, &pConnReq->advAddress);

    // Read LLData
    gpPd_ReadWithUpdate(pPdLoh, 4, (UInt8*)&pConnReq->llData.accessAddress);
    pConnReq->llData.crcInit = 0;
    gpPd_ReadWithUpdate(pPdLoh, BLE_CRC_SIZE, (UInt8*)&pConnReq->llData.crcInit);
    gpPd_ReadWithUpdate(pPdLoh, 1, &pConnReq->llData.winSize);
    gpPd_ReadWithUpdate(pPdLoh, 2, (UInt8*)&pConnReq->llData.winOffset);
    gpPd_ReadWithUpdate(pPdLoh, 2, (UInt8*)&pConnReq->llData.interval);
    gpPd_ReadWithUpdate(pPdLoh, 2, (UInt8*)&pConnReq->llData.latency);
    gpPd_ReadWithUpdate(pPdLoh, 2, (UInt8*)&pConnReq->llData.timeout);
    gpPd_ReadWithUpdate(pPdLoh, 5, pConnReq->llData.channelMap.channels);

    gpPd_ReadWithUpdate(pPdLoh, 1, &hopAndSca);

    pConnReq->llData.hopIncrement = BLE_GET_HOP_FROM_LAST_LL_DATA_BYTE(hopAndSca);
    pConnReq->llData.sleepClockAccuracy = BLE_GET_SCA_FROM_LAST_LL_DATA_BYTE(hopAndSca);
}

Bool Ble_IsSupervisionToValid(UInt16 supervisionTo, UInt16 latency, UInt16 connInterval)
{
    UInt32 supervisionToUs;
    UInt32 supervisionToLowerUs;

    // supervisionTo (in units) shall be in appropriate ranges
    if(!RANGE_CHECK(supervisionTo, BLE_INITIATOR_SUPERVISION_TO_RANGE_MIN, BLE_INITIATOR_SUPERVISION_TO_RANGE_MAX))
    {
        GP_LOG_PRINTF("timeout not in range: %x <= %x <= %x", 0, BLE_INITIATOR_SUPERVISION_TO_RANGE_MIN, supervisionTo, BLE_INITIATOR_SUPERVISION_TO_RANGE_MAX);
        return false;
    }

    supervisionToUs = BLE_TIME_UNIT_10000_TO_US(supervisionTo);
    supervisionToLowerUs = (UInt32)((1 + latency) * BLE_TIME_UNIT_1250_TO_US(connInterval) * 2);

    // supervisionTo (in us) shall be larger than (1 + Conn_Latency) * Conn_Interval_Max * 2, where Conn_Interval_Max is given in milliseconds
    if(supervisionToUs <= supervisionToLowerUs)
    {
        GP_LOG_PRINTF("Invalid value for supervision TO (%lu us) must be > %lu",0, (unsigned long)supervisionToUs, (unsigned long)supervisionToLowerUs);
        return false;
    }

    return true;
}

UInt16 Ble_ScaFieldToMaxPpm(Ble_MasterSca_t sca)
{
    UIntLoop i;

    for(i = 0; i < number_of_elements(Ble_ScaPpmMapping); i++)
    {
        if(sca == Ble_ScaPpmMapping[i].sca)
        {
            return Ble_ScaPpmMapping[i].ppmMax;
        }
    }

    GP_ASSERT_DEV_INT(false);

    return 0xFFFF;
}

Ble_MasterSca_t Ble_PpmToScaField(UInt16 ppm)
{
    UIntLoop i;

    for(i = 0; i < number_of_elements(Ble_ScaPpmMapping); i++)
    {
        if(RANGE_CHECK(ppm, Ble_ScaPpmMapping[i].ppmMin, Ble_ScaPpmMapping[i].ppmMax))
        {
            return Ble_ScaPpmMapping[i].sca;
        }
    }
    // in case of an invalid ppm - return the max value
    return Ble_ScaPpmMapping[0].sca;
}

