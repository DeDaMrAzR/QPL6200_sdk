/*
 * Copyright (c) 2014, 2016, GreenPeak Technologies
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpMacCore_DataPending.c
 *   This file contains the implementation of the MAC data pending.
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
//#define GP_LOCAL_LOG
// General includes
#include "gpAssert.h"
#include "gpLog.h"
#include "gpHal.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE

#define MACCORE_ADDRESS_MODE_INVALID ((gpMacCore_AddressMode_t)(-1))

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

Bool MacCore_IsDataPending(Bool dataQueued, Bool isNeighbour, gpMacCore_StackId_t stackId)
{
    Bool dataPending;
    if(MacCore_GetDataPendingModeStack(stackId) == gpMacCore_DataPendingModeForNonNeighbourDevices)
    {
        // In this mode we want FP==1 for neighbours with queued data and non-neighbours
        dataPending = !isNeighbour || dataQueued;
    }
    else
    {
        dataPending = dataQueued;
    }
    return dataPending;
}

gpMacCore_Result_t MacCore_DataPendingManipulateEntry(Bool dataQueued, Bool isNeighbour, gpMacCore_AddressInfo_t* pAddrInfo, gpMacCore_StackId_t stackId)
{
    gpHal_Result_t result;
    gpHal_AddressInfo_t addrInfo;
    Bool add;
    Bool fp;

    if (stackId >= DIVERSITY_NR_OF_STACKS)
    {
        return gpMacCore_ResultInvalidParameter;
    }

    fp = MacCore_IsDataPending(dataQueued, isNeighbour, stackId);

    if(MacCore_GetDataPendingModeStack(stackId) == gpMacCore_DataPendingModeForNonNeighbourDevices)
    {
        add = !fp; // in table => DP==0.
    }
    else
    {
        add = fp;  // in table => DP==1
    }

    //Translation to gpHal struct
    if(pAddrInfo->addressMode == gpMacCore_AddressModeExtendedAddress)
    {
        addrInfo.address.Extended = pAddrInfo->address.Extended;
    }
    else if(pAddrInfo->addressMode == gpMacCore_AddressModeShortAddress)
    {
        addrInfo.address.Short = pAddrInfo->address.Short;
    }
    else
    {
        return gpMacCore_ResultInvalidParameter;
    }
    addrInfo.panId       = pAddrInfo->panId;
    addrInfo.addressMode = pAddrInfo->addressMode;

    if(add)
    {
        result = gpHal_DpAddEntry(&addrInfo, stackId);
        // If table full => gpHal_ResultBusy
        if(result != gpHal_ResultSuccess)
        {
            return gpMacCore_ResultTransactionOverflow;
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("Not adding due to mode %i  [%i]", 0, MacCore_GetDataPendingModeStack(stackId), stackId);
        result = gpHal_DpRemoveEntry(&addrInfo, stackId);
        if (result == gpHal_ResultInvalidHandle)
        {
            GP_LOG_PRINTF("Nothing to remove",0);
        }
        else if(result != gpHal_ResultSuccess)
        {
            return gpMacCore_ResultInvalidParameter;
        }
    }

    return gpMacCore_ResultSuccess;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

gpMacCore_Result_t gpMacCore_DataPending_QueueAdd(gpMacCore_AddressInfo_t* pAddrInfo, gpMacCore_StackId_t stackId)
{
    return MacCore_DataPendingManipulateEntry(true, MacCore_CheckIfNeighbour(pAddrInfo, stackId), pAddrInfo, stackId);
}

gpMacCore_Result_t gpMacCore_DataPending_QueueRemove(gpMacCore_AddressInfo_t* pAddrInfo, gpMacCore_StackId_t stackId)
{
    return MacCore_DataPendingManipulateEntry(false, MacCore_CheckIfNeighbour(pAddrInfo, stackId), pAddrInfo, stackId);
}

gpMacCore_Result_t gpMacCore_DataPending_QueueClear(gpMacCore_StackId_t stackId)
{
    gpHal_Result_t result;

    result = gpHal_DpClearEntries(stackId);

    if(result == gpHal_ResultInvalidParameter)
    {
        return gpMacCore_ResultInvalidParameter;
    }
    return gpMacCore_ResultSuccess;
}

Bool gpMacCore_DataPendingGetAckDataPending(void)
{
    return gpHal_GetFramePendingAckDefault();
}

void MacCore_InitDataPending(void)
{
    UInt8 stackIndex, index;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    for(stackIndex = 0; stackIndex < DIVERSITY_NR_OF_STACKS; stackIndex++)
    {
        for(index = 0; index < MACCORE_MAX_NEIGHBOURS(); index++)
        {
            MEMSET(MacCore_GetNeighbour(stackIndex,index),-1, sizeof(gpMacCore_AddressInfo_t));
        }
        maccore_constglobals->MacCore_DataPendingModePerStack[stackIndex] = gpMacCore_DataPendingModeMac802154;
    }
}

Bool gpMacCore_AddNeighbour   (gpMacCore_AddressInfo_t *pAddrInfo, gpMacCore_StackId_t stackId)
{
    UInt8 index;

    GP_LOG_PRINTF("gpMacCore_AddNeighbour",0);

    // Find if already exists in list
    for(index = 0; index < MACCORE_MAX_NEIGHBOURS(); index++)
    {
        if(MacCore_CompairAddress(MacCore_GetNeighbour(stackId,index), pAddrInfo))
        {
            // already present. Do nothing
            return true;
        }
    }

    // insert new
    for(index = 0; index < MACCORE_MAX_NEIGHBOURS(); index++)
    {
        if((MacCore_GetNeighbour(stackId,index))->addressMode == MACCORE_ADDRESS_MODE_INVALID)
        {
            Bool dataQueued;

            /* If data is pending we expect poll indications. We need to redetermine what the data pending
             * bit was for TX'ed ACKs */
            dataQueued = MacCore_ExistsElemForAddress(pAddrInfo, stackId);

            MEMCPY(MacCore_GetNeighbour(stackId,index), pAddrInfo, sizeof(gpMacCore_AddressInfo_t));
            MacCore_DataPendingManipulateEntry(dataQueued, true, pAddrInfo, stackId);
            return true;
        }
    }

    return false;
}

Bool gpMacCore_RemoveNeighbour(gpMacCore_AddressInfo_t *pAddrInfo, gpMacCore_StackId_t stackId)
{
    UInt8 index;

    GP_LOG_PRINTF("gpMacCore_RemoveNeighbour",0);

    //find if exists in list
    for(index = 0; index < MACCORE_MAX_NEIGHBOURS(); index++)
    {
        if(MacCore_CompairAddress(MacCore_GetNeighbour(stackId,index), pAddrInfo))
        {
            Bool dataQueued;

            /* If data is pending we expect poll indications. We need to redetermine what the data pending
             * bit was for TX'ed ACKs */
            dataQueued = MacCore_ExistsElemForAddress(pAddrInfo, stackId);

            MEMSET(MacCore_GetNeighbour(stackId,index),-1, sizeof(gpMacCore_AddressInfo_t));
            MacCore_DataPendingManipulateEntry(dataQueued, false, pAddrInfo, stackId);
            return true;
        }
    }

    return false;
}

void gpMacCore_ClearNeighbours(gpMacCore_StackId_t stackId)
{
    UInt8 index;

    GP_LOG_PRINTF("gpMacCore_ClearNeighbours",0);

    //Clear entire list
    for(index = 0; index < MACCORE_MAX_NEIGHBOURS(); index++)
    {
        MacCore_DataPendingManipulateEntry(false, false, MacCore_GetNeighbour(stackId,index), stackId);
        MEMSET(MacCore_GetNeighbour(stackId,index),-1, sizeof(gpMacCore_AddressInfo_t));
    }
}

UInt8 MacCore_NumberOfNeighbours(gpMacCore_StackId_t stackId)
{
    UInt8 index;
    UInt8 count = 0;

    //find if exists in list
    for(index = 0; index < MACCORE_MAX_NEIGHBOURS(); index++)
    {
        if ((MacCore_GetNeighbour(stackId,index))->addressMode != MACCORE_ADDRESS_MODE_INVALID)
        {
            count++;
        }
    }
    return count;
}

gpMacCore_Result_t gpMacCore_SetDataPendingMode(gpMacCore_DataPendingMode_t dataPendingMode, gpMacCore_StackId_t stackId)
{
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    GP_ASSERT_DEV_INT(stackId < DIVERSITY_NR_OF_STACKS);

    /* We don't expect any data pending or neighbours */
    if( MacCore_IndTxBufferNbrActiveElements(stackId) > 0 ||
        MacCore_NumberOfNeighbours(stackId) > 0)
    {
        GP_LOG_SYSTEM_PRINTF("Mode setting not possible. Still have neighbours/packets",0);
        return gpMacCore_ResultTransactionOverflow;
    }

    maccore_constglobals->MacCore_DataPendingModePerStack[stackId] = dataPendingMode;
    // now change the default frame pending bit.
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    gpHal_SetFramePendingAckStack(dataPendingMode, stackId);
#else
    if(MacCore_GetDataPendingMode() == gpMacCore_DataPendingModeForNonNeighbourDevices)
    {
        // switch to DP = always on
        gpHal_SetFramePendingAckDefault(true);
    }
    else
    {
        // switch to DP = always off
        gpHal_SetFramePendingAckDefault(false);
    }
#endif
    return gpMacCore_ResultSuccess;
}

gpMacCore_DataPendingMode_t MacCore_GetDataPendingMode(void)
{
    UInt8 index;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    for(index = 0; index < DIVERSITY_NR_OF_STACKS; index++)
    {
        if(maccore_constglobals->MacCore_DataPendingModePerStack[index] == gpMacCore_DataPendingModeForNonNeighbourDevices)
        {
            // if any stack has this mode set, then use this as a global setting.
            return gpMacCore_DataPendingModeForNonNeighbourDevices;
        }
    }
    return gpMacCore_DataPendingModeMac802154;
}

gpMacCore_DataPendingMode_t MacCore_GetDataPendingModeStack(UInt8 stackId)
{
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    return maccore_constglobals->MacCore_DataPendingModePerStack[stackId];
#else
    return MacCore_GetDataPendingMode();
#endif
}

UInt8 MacCore_GetNeighbourIndex(gpMacCore_AddressInfo_t *pAddrInfo, gpMacCore_StackId_t stackId)
{
    UInt8 index;

    //find if exists in list
    for(index = 0; index < MACCORE_MAX_NEIGHBOURS(); index++)
    {
        if(MacCore_CompairAddress(MacCore_GetNeighbour(stackId,index), pAddrInfo))
        {
            // found.
            return index;
        }
    }

    return MACCORE_MAX_NEIGHBOURS();
}

Bool MacCore_CheckIfNeighbour(gpMacCore_AddressInfo_t *pAddrInfo, gpMacCore_StackId_t stackId)
{
    UInt8 index;

    //find if exists in list
    index = MacCore_GetNeighbourIndex(pAddrInfo, stackId);
    return index < MACCORE_MAX_NEIGHBOURS();
}

Bool MacCore_CheckIfDataPending(gpMacCore_AddressInfo_t *pAddrInfo, gpMacCore_StackId_t stackId)
{
    UInt8 index;
    Bool isNeighbour;
    Bool dataQueued;

    index = MacCore_GetNeighbourIndex(pAddrInfo, stackId);
    isNeighbour = index < MACCORE_MAX_NEIGHBOURS();

    dataQueued = MacCore_ExistsElemForAddress(pAddrInfo, stackId);

    return MacCore_IsDataPending(dataQueued, isNeighbour, stackId);
}
