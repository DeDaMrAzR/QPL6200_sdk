/*
 * Copyright (c) 2012-2014, GreenPeak Technologies
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpMacCore_Association.c
 *   This file contains the association related functionality of the IEEE 802.15.4 MAC protocol.
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

// General includes
#include "gpLog.h"
#include "gpHal.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"
#include "gpPoolMem.h"
#include "gpSched.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
void MacCore_AssociateTimeout(void)
{
    //currently nothing to do here, scheduled task is used as a flag to know if timeout has been reached
}

void MacCore_AssociateSendCommandDataRequest(void)
{
    gpMacCore_Result_t result;

    //null pointer protection
    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs == NULL)
    {
        GP_ASSERT_DEV_EXT(false);
        return;
    }

    result = MacCore_SendCommandDataRequest(&GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo, true, NULL, GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId);

    if (result != gpMacCore_ResultSuccess)
    {
        //if we were not able to send a data request, and timeout has not been reach yet, re-schedule
        if(gpSched_ExistsEvent(MacCore_AssociateTimeout))
        {
            gpSched_ScheduleEvent((UInt32)((UInt32)GP_MACCORE_RESPONSE_WAIT_TIME * (UInt32)GP_MACCORE_SYMBOL_DURATION), MacCore_AssociateSendCommandDataRequest);
        }
        else//if we were not able to send data request, and timeout has been reached, confirm fail state
        {
            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = result;
            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->txTimestamp = 0;
            MacCore_HandleAssocConf();
        }
    }
}

void MacCore_HandleAssocConf(void)
{
    /*
     *   According IEEE-802.15.4 paragraph 5.1.3.1 Association
     *   If the value of the Association Status field of the command is not “Association successful,” if there were a
     *   communication failure during the association process due to a missed acknowledgment, or if the association
     *   response command frame were not received, the device shall set macPANId to the default value (0xffff).
     */
    UInt16 assocShortAddress = GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->responseShortAddr;
    gpMacCore_Result_t status = GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result;
    UInt32 txTime = GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->txTimestamp;

    //if the status of the last poll was not a success and timeout occurred, send timeout status
    if(!gpSched_ExistsEvent(MacCore_AssociateTimeout) && (status != gpMacCore_ResultSuccess))
    {
        status = gpMacCore_ResultTransactionExpired;
    }

    GP_LOG_PRINTF("handle assocconf",0);

    if(status != gpMacCore_ResultSuccess)
    {
        gpMacCore_SetPanId(GP_MACCORE_PANID_BROADCAST, GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId);
    }
    gpPoolMem_Free(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs);
    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs = NULL;

    gpSched_UnscheduleEvent(MacCore_AssociateSendCommandDataRequest);   // Added SW-9861
    gpSched_UnscheduleEvent(MacCore_AssociateTimeout);
    gpMacCore_cbAssociateConfirm(assocShortAddress, status, txTime);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
/** @fn gpMacCore_AssociateRequest(UInt8 logicalChannel, gpMacCore_AddressInfo_t* pCoordAddrInfo, UInt8 capabilityInformation)
 *  @brief This function constructs and sends the association request - device side
 *
 *  @param UInt8 logicalChannel
 *  @param gpMacCore_AddressInfo_t* pCoordAddrInfo
 *  @param UInt8 capabilityInformation
 *
 *  @return void
 *  @note only present on a RFD
 */
void gpMacCore_AssociateRequest(UInt8 logicalChannel , gpMacCore_AddressInfo_t* pCoordAddrInfo , UInt8 capabilityInformation , gpMacCore_StackId_t stackId )
{
    gpMacCore_AddressInfo_t srcAddrInfo;
    gpMacCore_Result_t result;
    UInt8 pMsdu[GP_MACCORE_ASSOCIATION_REQUEST_CMD_LEN]={gpMacCore_CommandAssociationRequest,capabilityInformation};

    if(GP_MACCORE_CHECK_IF_ADDRESSINFO_INVALID(pCoordAddrInfo))
    {
        gpMacCore_cbAssociateConfirm(GP_MACCORE_SHORT_ADDR_BROADCAST, gpMacCore_ResultInvalidParameter, 0);
        return;
    }

    if(GP_MACCORE_CHECK_IF_ADDRESSMODE_NOT_PRESENT(pCoordAddrInfo->addressMode))
    {
        gpMacCore_cbAssociateConfirm(GP_MACCORE_SHORT_ADDR_BROADCAST, gpMacCore_ResultInvalidParameter, 0);
        return;
    }

    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs != NULL)
    {
        gpMacCore_cbAssociateConfirm(GP_MACCORE_SHORT_ADDR_BROADCAST, gpMacCore_ResultTransactionOverflow, 0 );
        return;
    }

    gpRxArbiter_SetStackChannel(logicalChannel, stackId);
    gpMacCore_SetPanId(pCoordAddrInfo->panId, stackId);

    if(pCoordAddrInfo->addressMode == gpMacCore_AddressModeShortAddress)
    {
        gpMacCore_SetCoordShortAddress(pCoordAddrInfo->address.Short, stackId);
    }
    else
    {
        gpMacCore_SetCoordExtendedAddress(&(pCoordAddrInfo->address.Extended), stackId);
    }

    // fill in assoc args
    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs = (MacCore_PollReqArgs_t*)GP_POOLMEM_MALLOC(sizeof(MacCore_PollReqArgs_t));
    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId = stackId;
    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo = *pCoordAddrInfo;
    //Init response result
    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = gpMacCore_ResultNoData;
    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->responseShortAddr = GP_MACCORE_SHORT_ADDR_BROADCAST;
    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->assoc = true;

    // prepare src address info
    MacCore_InitSrcAddressInfo(&srcAddrInfo, gpMacCore_AddressModeExtendedAddress, stackId);
    // overrule pan ID
    srcAddrInfo.panId = GP_MACCORE_PANID_BROADCAST;

    // send Association Request
    result = MacCore_SendCommand(pCoordAddrInfo, &srcAddrInfo, GP_MACCORE_TX_OPT_ACK_REQ , NULL, pMsdu, GP_MACCORE_ASSOCIATION_REQUEST_CMD_LEN , stackId, gpHal_MacDefault);

    //need to schedule timeout here, otherwise MacCore_HandleAssocConf will overwrite the info from the assoc request status to timeout
    //timeout is around 7 seconds if gpMacCore_GetTransactionPersistenceTime is GP_MACCORE_DEFAULT_TRANSACTION_PERSISTENCE_TIME
    gpSched_ScheduleEvent((UInt32)((UInt32)gpMacCore_GetTransactionPersistenceTime(stackId) * (UInt32)GP_MACCORE_BASE_SUPERFRAME_DURATION*GP_MACCORE_SYMBOL_DURATION),
                                MacCore_AssociateTimeout);

    // if success continue in dataconfirm
    // if not success, end association attempt.
    if(result != gpMacCore_ResultSuccess)
    {
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = result;
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->txTimestamp = 0;
        MacCore_HandleAssocConf();
    }
}
