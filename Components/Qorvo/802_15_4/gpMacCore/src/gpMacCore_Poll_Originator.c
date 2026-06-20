/*
 * Copyright (c) 2012-2014, GreenPeak Technologies
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpMacCore_Poll_Originator.c
 *   This file contains the implementation of the non beaconed MAC protocol.
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
#include "gpMacCore.h"
#include "gpHal.h"
#include "gpMacCore_defs.h"
#include "gpPoolMem.h"

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

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpMacCore_PollRequest(gpMacCore_AddressInfo_t* pCoordAddrInfo, gpMacCore_Security_t *pSecOptions, gpMacCore_StackId_t stackId)
{
    gpMacCore_Result_t result;

    if(GP_MACCORE_CHECK_IF_ADDRESSINFO_INVALID(pCoordAddrInfo))
    {
        gpMacCore_cbPollConfirm(gpMacCore_ResultInvalidParameter, pCoordAddrInfo, 0);
        return;
    }
    if(GP_MACCORE_CHECK_IF_ADDRESSMODE_NOT_PRESENT(pCoordAddrInfo->addressMode))
    {
        gpMacCore_cbPollConfirm(gpMacCore_ResultInvalidParameter, pCoordAddrInfo, 0);
        return;
    }

    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs)
    {
        gpMacCore_cbPollConfirm( gpMacCore_ResultTransactionOverflow ,pCoordAddrInfo,0);
        return;
    }

    GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs = (MacCore_PollReqArgs_t*)GP_POOLMEM_MALLOC(sizeof(MacCore_PollReqArgs_t));
    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs == NULL)
    {
        gpMacCore_cbPollConfirm( gpMacCore_ResultTransactionOverflow ,pCoordAddrInfo,0);
        return;
    }
    result = MacCore_SendCommandDataRequest(pCoordAddrInfo, false, pSecOptions, stackId);
    if ( result == gpMacCore_ResultSuccess )
    {
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId = stackId;
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo = *pCoordAddrInfo;
        // K5 always gives succes, might be optimistic
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = gpMacCore_ResultNoData;
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->assoc = false;
    }
    else
    {
        gpPoolMem_Free(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs);
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs = NULL;
        gpMacCore_cbPollConfirm( result ,pCoordAddrInfo,0);
    }
}

