/*
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

//#define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLELLCPPROCEDURES

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpBle.h"
#include "gpBleConfig.h"
#include "gpBleChannelMapManager.h"
#include "gpBleLlcpProcedures.h"
#include "gpBleLlcpProcedures_defs.h"
#include "gpBleLlcpProcedures_Update_defs.h"
#include "gpBle_defs.h"
#include "gpLog.h"
#include "gpSched.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define BLE_CTR_DATA_LENGTH_CHAN_MAP_IND 7

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
*****************************************************************************/

static Ble_LlcpFrameworkAction_t Ble_LlcpChannelMapUpdateStoreCtrData(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpPd_Loh_t* pPdLoh, gpBleLlcp_Opcode_t opcode);
static Ble_LlcpFrameworkAction_t Ble_LlcpChannelMapUpdatePduTransmitted(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpBleLlcp_Opcode_t txOpcode);
#ifdef GP_DIVERSITY_BLE_PERIPHERAL
static Ble_LlcpFrameworkAction_t Ble_LlcpChannelMapUpdatePduReceived(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpBleLlcp_Opcode_t rxOpcode);
#endif // GP_DIVERSITY_BLE_PERIPHERAL
static void Ble_LlcpChannelMapUpdateFinished(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, Bool notifyHost);
static void Ble_LlcpRegisterChannelMapUpdateProcedure(void);

/*****************************************************************************
 *                    Procedure descriptor
*****************************************************************************/

static const gpBleLlcpFramework_PduDescriptor_t BleLlcpProcedures_ChannelMapUpdatePduDescriptors[] =
{
    {gpBleLlcp_OpcodeChannelMapInd, 7, GPBLELLCPFRAMEWORK_PDU_ROLE_MASK_CENTRAL},
};

static const  Ble_LlcpProcedureDescriptor_t BleLlcpProcedures_ChannelMapUpdateDescriptor =
{
    .procedureFlags = GPBLELLCPFRAMEWORK_PROCEDURE_FLAGS_INSTANT_BM,
    .procedureDataLength = sizeof(gpBleLlcpFramework_ChannelMapUpdateData_t),
    .nrOfPduDescriptors = number_of_elements(BleLlcpProcedures_ChannelMapUpdatePduDescriptors),
    .pPduDescriptors = BleLlcpProcedures_ChannelMapUpdatePduDescriptors,
    .featureMask = GPBLELLCP_FEATUREMASK_NONE,
    .cbQueueingNeeded = NULL,
    .cbProcedureStart = NULL,
    .cbStoreCtrData = Ble_LlcpChannelMapUpdateStoreCtrData,
#ifdef GP_DIVERSITY_BLE_PERIPHERAL
    .cbPduReceived = Ble_LlcpChannelMapUpdatePduReceived,
#endif // GP_DIVERSITY_BLE_PERIPHERAL
    .cbUnexpectedPduReceived = NULL,
    .cbPduQueued = NULL,
    .cbPduTransmitted = Ble_LlcpChannelMapUpdatePduTransmitted,
    .cbFinished = Ble_LlcpChannelMapUpdateFinished
};

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/


Ble_LlcpFrameworkAction_t Ble_LlcpChannelMapUpdateStoreCtrData(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpPd_Loh_t* pPdLoh, gpBleLlcp_Opcode_t opcode)
{
    gpHci_ChannelMap_t newChannelMap;
    UInt16 instant;
    UInt16 pduConnEventCount;
    UIntLoop i;
    UInt8 nrOfSelectedChannels = 0;

    gpBle_ReadChannelMapFromPdu(pPdLoh, &newChannelMap);
    gpPd_ReadWithUpdate(pPdLoh, sizeof(instant), (UInt8*)&instant);

    for(i = 0; i < sizeof(newChannelMap.channels); i++)
    {
        nrOfSelectedChannels += POPCOUNT(newChannelMap.channels[i]);
    }

    if(nrOfSelectedChannels == 0)
    {
        GP_LOG_PRINTF("Expect at least 1 channel, got only 0",0);
        pProcedure->result = gpHci_ResultInvalidLMPParametersInvalidLLParameters;
        return Ble_LlcpFrameworkActionRejectWithUnknownRsp;
    }

    pduConnEventCount = Ble_LlcpGetPduConnEventCount(pContext, pPdLoh);

    if(!Ble_LlcpInstantValid(instant, pduConnEventCount))
    {
        GP_LOG_PRINTF("Instant is in the past, connection lost",0);
        // Schedule to make sure we do not terminate while procedure still running
        gpSched_ScheduleEventArg(0, Ble_LlcpInstantInvalidFollowUp, pContext);
        return Ble_LlcpFrameworkActionContinue;
    }

    pContext->channelMapHandleNext = gpBleChannelMapManager_AllocateHandle(&newChannelMap);

    if(!gpBleChannelMapManager_IsHandleInUse(pContext->channelMapHandleNext))
    {
        pProcedure->result = gpHci_ResultMemoryCapacityExceeded;
        return Ble_LlcpFrameworkActionReject;
    }

    Ble_LlcpConfigureLastScheduledConnEventAfterPassed(pProcedure, pduConnEventCount, instant);


    return Ble_LlcpFrameworkActionContinue;
}

Ble_LlcpFrameworkAction_t Ble_LlcpChannelMapUpdatePduTransmitted(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpBleLlcp_Opcode_t txOpcode)
{
    Ble_LlcpFrameworkAction_t action = Ble_LlcpFrameworkActionContinue;

    switch(txOpcode)
    {
        case gpBleLlcp_OpcodeChannelMapInd:
        {
            action = Ble_LlcpFrameworkActionPause;
            break;
        }
        case gpBleLlcp_OpcodeUnknownRsp:
        {
            action = Ble_LlcpFrameworkActionStop;
            break;
        }
        default:
        {
            // Should not happen
            GP_ASSERT_DEV_INT(false);
        }
    }

    return action;
}

Ble_LlcpFrameworkAction_t Ble_LlcpChannelMapUpdatePduReceived(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpBleLlcp_Opcode_t rxOpcode)
{
    GP_ASSERT_DEV_INT(pProcedure != NULL);

    pProcedure->currentTxPdu = gpBleLlcp_OpcodeInvalid;
    pProcedure->expectedRxPdu = gpBleLlcp_OpcodeInvalid;

    return Ble_LlcpFrameworkActionPause;
}

void Ble_LlcpChannelMapUpdateFinished(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, Bool notifyHost)
{
    if(gpBleLlcpFramework_ProcedureStateGet(pProcedure, BLE_LLCP_PROCEDURE_INTERRUPTED_BY_TERMINATION_IDX))
    {
        Ble_LlcpStopLastScheduledConnEventCount(pContext);
        gpBleLlcp_AclChannelMapPreInstantPassed(pContext, pProcedure);
    }
}


void Ble_LlcpChannelMapUpdatePreInstantPassed(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure)
{
    pProcedure->result = gpHci_ResultSuccess;

    gpBleLlcp_AclChannelMapPreInstantPassed(pContext, pProcedure);
}

void Ble_LlcpRegisterChannelMapUpdateProcedure(void)
{
    gpBleLlcpFramework_RegisterProcedure(gpBleLlcp_ProcedureIdChannelMapUpdate, &BleLlcpProcedures_ChannelMapUpdateDescriptor);

    gpBleLlcpFramework_RegisterInvalidProcedureAction(gpBleLlcp_ProcedureIdChannelMapUpdate, false);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


void gpBleLlcpProcedures_ChannelMapUpdateInit(void)
{
    Ble_LlcpRegisterChannelMapUpdateProcedure();
}

void gpBleLlcpProcedures_ChannelMapUpdateReset(Bool firstReset)
{
}

/*****************************************************************************
 *                    Public Service Function Definitions
 *****************************************************************************/
