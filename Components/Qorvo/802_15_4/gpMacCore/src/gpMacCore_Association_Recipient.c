/*
 * Copyright (c) 2012-2014, 2016, GreenPeak Technologies
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


void MacCore_FreeAssocRespPointer(MacCore_AssocRsp_t *pAssocRsp)
{
    UInt8 loop;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    //entry cannot be NULL
    GP_ASSERT_DEV_EXT(pAssocRsp);

    //list of pointers cannot be empty
    GP_ASSERT_DEV_EXT(maccore_globals->gpMacCore_pAssocRspMultiple);

    for(loop = 0; loop < GP_MACCORE_MAX_ASSOC_RESP; loop++)
    {
        if(maccore_globals->gpMacCore_pAssocRspMultiple[loop] == pAssocRsp)
        {
            gpPoolMem_Free(maccore_globals->gpMacCore_pAssocRspMultiple[loop]);
            maccore_globals->gpMacCore_pAssocRspMultiple[loop] = NULL;
            maccore_globals->MacCore_NumberAssocRsp--;

            //all entries freed, also free the table of pointers
            if(maccore_globals->MacCore_NumberAssocRsp == 0)
            {
                gpPoolMem_Free(maccore_globals->gpMacCore_pAssocRspMultiple);
                maccore_globals->gpMacCore_pAssocRspMultiple = NULL;
            }

            return;
        }
    }

    //entry not found
    GP_ASSERT_DEV_EXT(false);
}

MacCore_AssocRsp_t *MacCore_GetFreeAssocRespPointer(void)
{
    UInt8 loop;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    //first request of association response, need to allocate the table of pointers first
    if(maccore_globals->gpMacCore_pAssocRspMultiple == NULL)
    {
        maccore_globals->gpMacCore_pAssocRspMultiple = GP_POOLMEM_MALLOC(sizeof(MacCore_AssocRsp_t*)*GP_MACCORE_MAX_ASSOC_RESP);
        if(maccore_globals->gpMacCore_pAssocRspMultiple == NULL)
        {
            GP_LOG_SYSTEM_PRINTF("AssocRespMultiple NULL",0);
            return NULL;
        }
        maccore_globals->MacCore_NumberAssocRsp = 0;
        MEMSET(maccore_globals->gpMacCore_pAssocRspMultiple, 0x0, sizeof(MacCore_AssocRsp_t*)*GP_MACCORE_MAX_ASSOC_RESP);
    }

    for(loop = 0; loop < GP_MACCORE_MAX_ASSOC_RESP; loop++)
    {
        if(maccore_globals->gpMacCore_pAssocRspMultiple[loop] == NULL)
        {
            maccore_globals->gpMacCore_pAssocRspMultiple[loop] = GP_POOLMEM_MALLOC(sizeof(MacCore_AssocRsp_t));
            if(maccore_globals->gpMacCore_pAssocRspMultiple[loop] == NULL)
            {
                GP_LOG_SYSTEM_PRINTF("AssocResp NULL",0);
                return NULL;
            }
            maccore_globals->MacCore_NumberAssocRsp++;
            return maccore_globals->gpMacCore_pAssocRspMultiple[loop];
        }
    }
    return NULL;
}

MacCore_AssocRsp_t *MacCore_GetAssocRespPointer(MACAddress_t extendedAddress)
{
    UInt8 loop;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    //not initialized yet
    if(maccore_globals->gpMacCore_pAssocRspMultiple == NULL)
    {
        return NULL;
    }

    for(loop = 0; loop < GP_MACCORE_MAX_ASSOC_RESP; loop++)
    {
        if(maccore_globals->gpMacCore_pAssocRspMultiple[loop] != NULL)
        {
            if((maccore_globals->gpMacCore_pAssocRspMultiple[loop]->deviceAddress.LSB == extendedAddress.LSB)
             &&(maccore_globals->gpMacCore_pAssocRspMultiple[loop]->deviceAddress.MSB == extendedAddress.MSB))
            {
                return maccore_globals->gpMacCore_pAssocRspMultiple[loop];
            }
        }
    }
    return NULL;
}

Bool MacCore_AssocRespPointerValid(MacCore_AssocRsp_t *pAssocRsp)
{
    UInt8 loop;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    if((pAssocRsp == NULL) || (maccore_globals->gpMacCore_pAssocRspMultiple == NULL))
    {
        return false;
    }

    for(loop = 0; loop < GP_MACCORE_MAX_ASSOC_RESP; loop++)
    {
        if(maccore_globals->gpMacCore_pAssocRspMultiple[loop] == pAssocRsp)
        {
            return true;
        }
    }
    return false;
}

void MacCore_AssocRespStopRequests(gpMacCore_StackId_t stackId)
{
    UInt8 loop;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    //not initialized yet
    if(maccore_globals->gpMacCore_pAssocRspMultiple == NULL)
    {
        return;
    }

    for(loop = 0; loop < GP_MACCORE_MAX_ASSOC_RESP; loop++)
    {
        if(maccore_globals->gpMacCore_pAssocRspMultiple[loop] != NULL)
        {
            if(maccore_globals->gpMacCore_pAssocRspMultiple[loop]->stackId == stackId)
            {
                gpMacCore_AddressInfo_t dstAddrInfo;

                dstAddrInfo.address.Extended = maccore_globals->gpMacCore_pAssocRspMultiple[loop]->deviceAddress;
                dstAddrInfo.addressMode = gpMacCore_AddressModeExtendedAddress;
                dstAddrInfo.panId = gpMacCore_GetPanId(stackId);

                gpSched_UnscheduleEventArg(MacCore_AssociateResponseTimeOut, (void*)maccore_globals->gpMacCore_pAssocRspMultiple[loop]);
                gpSched_UnscheduleEventArg(MacCore_cbAssocResp, (void*)maccore_globals->gpMacCore_pAssocRspMultiple[loop]);

                //Clear the pending AssociationResponse message
                gpMacCore_DataPending_QueueRemove(&dstAddrInfo, stackId);

                //free pointer
                MacCore_FreeAssocRespPointer(maccore_globals->gpMacCore_pAssocRspMultiple[loop]);

                //all entries freed, stop the loop, because MacCore_FreeAssocRespPointer will also free the pointer list
                if(maccore_globals->MacCore_NumberAssocRsp == 0)
                {
                    return;
                }
            }
        }
    }
}

// Triggered on assoc response confirm
void MacCore_cbAssocConfirm(gpMacCore_AddressInfo_t* pSrcAddrInfo, gpMacCore_AddressInfo_t* pDstAddrInfo, gpMacCore_Result_t status, gpMacCore_StackId_t stackId, gpPd_TimeStamp_t txTime)
{
    MacCore_AssocRsp_t *pAssocRsp = MacCore_GetAssocRespPointer(pDstAddrInfo->address.Extended);

    if(pAssocRsp == NULL)
    {
        GP_ASSERT_DEV_EXT(false);
        return;
    }

    //No longer in flight
    pAssocRsp->inFlight = false;

    //If it is a success, or if the timeout reached during in flight, we need to clear and confirm
    //If it has failed (no ACK, CCA fail) and timeout has not expired, don't do a thing, next poll will try to send again, until timeout kicks in
    if((status == gpMacCore_ResultSuccess) ||
        (!gpSched_ExistsEventArg(MacCore_AssociateResponseTimeOut, (void*)pAssocRsp)))
    {
        //un-schedulle timeout
        gpSched_UnscheduleEventArg(MacCore_AssociateResponseTimeOut, (void*)pAssocRsp);
        //remove from queue
        gpMacCore_DataPending_QueueRemove(pDstAddrInfo, stackId);
        //free pointer
        MacCore_FreeAssocRespPointer(pAssocRsp);
        //send callback
        gpMacCore_cbAssociateCommStatusIndication(pSrcAddrInfo, pDstAddrInfo, status, stackId, txTime);
    }
}

// Triggered on the poll indication
void MacCore_cbAssocResp(void *arg)
{
    gpMacCore_Result_t  result;
    UInt8 pMsdu[GP_MACCORE_ASSOCIATION_RESPONSE_CMD_LEN];
    gpMacCore_AddressInfo_t srcAddrInfo;
    gpMacCore_AddressInfo_t dstAddrInfo;
    MacCore_AssocRsp_t *pAssocRsp = arg;
    UInt8 retriesBackup;

    if(NULL == pAssocRsp)
    {
        GP_LOG_SYSTEM_PRINTF("AssocResp NULL",0);
        return;
    }

    if(!MacCore_AssocRespPointerValid(pAssocRsp))
    {
        //If the timeout runs before this one is schedulled, the pointer is no longer valid
        return;
    }

    if(pAssocRsp->inFlight)
    {
        //This request is already in flight, it means a double poll before the first confirm.
        //Don't send a new one
        return;
    }

    MacCore_InitSrcAddressInfo(&srcAddrInfo, gpMacCore_AddressModeExtendedAddress, pAssocRsp->stackId);
    dstAddrInfo.address.Extended = pAssocRsp->deviceAddress;
    dstAddrInfo.addressMode = gpMacCore_AddressModeExtendedAddress;
    dstAddrInfo.panId = srcAddrInfo.panId;
    pMsdu[0] = gpMacCore_CommandAssociationResponse;
    pMsdu[1] = (UInt8)pAssocRsp->assocShortAddr;
    pMsdu[2] = (UInt8)(pAssocRsp->assocShortAddr>>8);
    pMsdu[3] = pAssocRsp->assocStatus;

    //Assoc response packet should not retry
    retriesBackup = gpMacCore_GetNumberOfRetries(pAssocRsp->stackId);
    gpMacCore_SetNumberOfRetries(0, pAssocRsp->stackId);

    // send Association Response
    result = MacCore_SendCommand(&dstAddrInfo, &srcAddrInfo, GP_MACCORE_TX_OPT_ACK_REQ, NULL, pMsdu, GP_MACCORE_ASSOCIATION_RESPONSE_CMD_LEN , pAssocRsp->stackId , gpHal_MacDefault);

    //Re-store retries
    gpMacCore_SetNumberOfRetries(retriesBackup, pAssocRsp->stackId);

    //If the result of the request is success, set the flag that it is in flight. If it failled to request, the next poll will re-trigger, or the timeout will kick in
    if(result == gpMacCore_ResultSuccess)
    {
        pAssocRsp->inFlight = true;
    }
}

/** @fn gpMacCore_AssociateResponse(MACAddress_t *pDstAddress, UInt16 associateShortAddress ,gpMacCore_Result_t assocStatus)
 *  @brief This function collects data for the association response and puts it on the indirect transmission queue - coordinator side
 *
 *  @param gpMacCore_Address_t  *pDstAddress
 *  @param UInt16               associateShortAddress
 *  @param gpMacCore_Result_t   assocStatus
 *
 *  @return void
 *  @note only present on FFD
 */
void gpMacCore_AssociateResponse(MACAddress_t* pDeviceAddress, UInt16 associateShortAddress, gpMacCore_Result_t assocStatus, gpMacCore_StackId_t stackId)
{
    gpMacCore_AddressInfo_t srcAddrInfo;
    gpMacCore_AddressInfo_t dstAddrInfo;
    MacCore_AssocRsp_t *pAssocRsp;

    MacCore_InitSrcAddressInfo(&srcAddrInfo, gpMacCore_AddressModeExtendedAddress, stackId);
    MEMCPY( &dstAddrInfo.address.Extended , pDeviceAddress , sizeof(MACAddress_t));
    dstAddrInfo.addressMode = gpMacCore_AddressModeExtendedAddress;
    dstAddrInfo.panId = srcAddrInfo.panId;

    pAssocRsp = MacCore_GetFreeAssocRespPointer();
    if(pAssocRsp == NULL)
    {
        gpMacCore_cbAssociateCommStatusIndication(&srcAddrInfo, &dstAddrInfo, gpMacCore_ResultTransactionOverflow, stackId , 0);
        return;
    }

    pAssocRsp->assocShortAddr = associateShortAddress;
    pAssocRsp->assocStatus    = assocStatus;
    pAssocRsp->stackId        = stackId;
    pAssocRsp->inFlight       = false;
    MEMCPY( &pAssocRsp->deviceAddress , pDeviceAddress , sizeof(MACAddress_t));
    gpMacCore_DataPending_QueueAdd(&dstAddrInfo, stackId);

    // Schedule the expiration of this transaction
    gpSched_ScheduleEventArg(GP_MACCORE_TRANSACTION_PERSISTENCE_TIME_TO_US(gpMacCore_GetTransactionPersistenceTime(stackId)), MacCore_AssociateResponseTimeOut, (void*)pAssocRsp);

}

void MacCore_AssociateResponseTimeOut(void* arg)
{
    UInt8 stackId;
    gpMacCore_AddressInfo_t srcAddrInfo;
    gpMacCore_AddressInfo_t dstAddrInfo;
    MacCore_AssocRsp_t *pAssocRsp = (MacCore_AssocRsp_t*)arg;

    if(NULL == pAssocRsp)
    {
        GP_LOG_SYSTEM_PRINTF("AssocTO NULL",0);
        return;
    }

    if(!MacCore_AssocRespPointerValid(pAssocRsp))
    {
        GP_ASSERT_DEV_EXT(false);
        return;
    }

    if(pAssocRsp->inFlight)
    {
        //if it is in flight, don't do anything, send status on the confirm
        return;
    }

    MacCore_InitSrcAddressInfo(&srcAddrInfo, gpMacCore_AddressModeShortAddress, pAssocRsp->stackId);
    dstAddrInfo.address.Extended = pAssocRsp->deviceAddress;
    dstAddrInfo.addressMode = gpMacCore_AddressModeExtendedAddress;
    dstAddrInfo.panId = srcAddrInfo.panId;

    //Freeing before CB, so calback can re-trigger if wanted
    stackId = pAssocRsp->stackId;
    MacCore_FreeAssocRespPointer(pAssocRsp);

    gpMacCore_DataPending_QueueRemove(&dstAddrInfo, stackId);
    gpMacCore_cbAssociateCommStatusIndication(&srcAddrInfo, &dstAddrInfo, gpMacCore_ResultTransactionExpired, stackId, 0 );
}

