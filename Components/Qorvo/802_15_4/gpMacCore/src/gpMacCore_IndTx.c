/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpMacCore_IndTx.c
 *   This file contains indirect transmission functionality from the MAC protocol.
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
#include "gpMacCore.h"
#include "gpAssert.h"
#include "gpLog.h"
#include "gpSched.h"
#include "gpPd.h"
#include "gpStat.h"
#include "gpPoolMem.h"
#include "gpHal.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE

/* used to identify index returned by MacCore_IndTxBufferFindElemForTx */
#define MACCORE_ADDRESS_HAS_IN_FLIGHT_PACKET 0xFE
#define MACCORE_ADDRESS_HAS_NO_PENDING_DATA  0xFF

#define GP_MACCORE_INITIAL_FRESHNESS_CNTR     9

//Adding bit to indicate packet in flight status to freshnessCntr. Space optimization
#define MACCORE_INDTX_IN_FLIGHT_IDX 7

#define MACCORE_INDTX_FRESHNESS_COUNTER_SET(pElement, value) {(pElement)->freshnessCntr = (value) & ~BM(MACCORE_INDTX_IN_FLIGHT_IDX); }
#define MACCORE_INDTX_FRESHNESS_COUNTER_GET(pElement) ((pElement)->freshnessCntr & ~BM(MACCORE_INDTX_IN_FLIGHT_IDX))
#define MACCORE_INDTX_FRESHNESS_COUNTER_DEC(pElement) {(pElement)->freshnessCntr--; } //value ranges from 9-1

#define MACCORE_INDTX_IN_FLIGHT_GET(pElement)   BIT_TST((pElement)->freshnessCntr, MACCORE_INDTX_IN_FLIGHT_IDX)
#define MACCORE_INDTX_IN_FLIGHT_SET(pElement)   BIT_SET((pElement)->freshnessCntr, MACCORE_INDTX_IN_FLIGHT_IDX)
#define MACCORE_INDTX_IN_FLIGHT_CLR(pElement)   BIT_CLR((pElement)->freshnessCntr, MACCORE_INDTX_IN_FLIGHT_IDX)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void MacCore_IndTxInit(void)
{
    UIntLoop i;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    maccore_globals->MacCore_IndTxEmptyPacketPdHandle = GP_PD_INVALID_HANDLE;

    for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        maccore_constglobals->MacCore_IndTxBuffer[i] = NULL;
    }
    for(i = 0; i < DIVERSITY_NR_OF_STACKS; i++)
    {
        maccore_constglobals->pStackId[i] = i;
    }
}

void MacCore_IndTxBufferCleanUp(gpMacCore_StackId_t stackId)
{
    UInt8 i;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        MacCore_IndTxElement_t* pElement;
        pElement = maccore_constglobals->MacCore_IndTxBuffer[i];

        if (pElement && (pElement->pdLoh.handle != GP_PD_INVALID_HANDLE && pElement->stackId == stackId))
        {
            //If packet is in flight, don't free PD, pd will be freed on MAC dispatcher when not finding a valid callback
            if((!MACCORE_INDTX_IN_FLIGHT_GET(pElement)))
            {
                // free Pd
                gpPd_FreePd(pElement->pdLoh.handle);
            }

            // free memory
            gpPoolMem_Free(pElement);

            // make available for other frames
            maccore_constglobals->MacCore_IndTxBuffer[i] = NULL;
            GP_LOG_PRINTF("Remove(i) IndTx entry %d",0,i);
        }
    }
    // Clear full data pending info for this stack - irrespective of data pending mode
    gpMacCore_DataPending_QueueClear(stackId);

    gpSched_UnscheduleEventArg(MacCore_IndTxBufferRemoveExpiredPacket, (void*) &(maccore_constglobals->pStackId[stackId]));
}

UInt8 MacCore_IndTxBufferNbrActiveElements(UInt8 stackId)
{
    UInt8 i;
    UInt8 activeElements = 0;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        MacCore_IndTxElement_t *pElement = maccore_constglobals->MacCore_IndTxBuffer[i];

        if (pElement
            && (pElement->pdLoh.handle != GP_PD_INVALID_HANDLE)
            && stackId == pElement->stackId)
        {
            activeElements++;
        }
    }
    return activeElements;
}

Bool MacCore_IndTxAddElement(gpMacCore_AddressMode_t srcAddrMode, gpMacCore_AddressInfo_t* pDstAddrInfo, UInt8 txOptions, gpMacCore_Security_t *pSecOptions, gpMacCore_MultiChannelOptions_t multiChannelOptions, gpPd_Loh_t pdLoh, gpMacCore_StackId_t stackId)
{
    UInt8 i;
    MacCore_IndTxElement_t *pElement;
    UInt8 highestFreshnessCounter = 0;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    Bool indexAddedElem = MACCORE_INDTX_ENTRIES;

    Bool existsElem = MacCore_ExistsElemForAddress(pDstAddrInfo,stackId);
    for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        pElement = maccore_constglobals->MacCore_IndTxBuffer[i];
        // find empty spot and add it

        // slots that have never been used or freed are set to point to NULL
        if (pElement == NULL)
        {
            //only add elem once :)
            if(indexAddedElem == MACCORE_INDTX_ENTRIES)
            {
                indexAddedElem = i;

                maccore_constglobals->MacCore_IndTxBuffer[i] = GP_POOLMEM_MALLOC(sizeof(MacCore_IndTxElement_t));
                GP_LOG_PRINTF("Add IndTx entry %u",0,i);

                if (maccore_constglobals->MacCore_IndTxBuffer[i] == NULL)
                {
                    //No Poolmem free !
                    return false;
                }
            }
        }
        else
        {
            //look if a packet for the device already exists for same stackId
            if(MacCore_CompairAddress(&pElement->dstAddrInfo , pDstAddrInfo)
                && pElement->stackId == stackId)
            {
                if(MACCORE_INDTX_FRESHNESS_COUNTER_GET(pElement) > highestFreshnessCounter)
                {
                    highestFreshnessCounter = MACCORE_INDTX_FRESHNESS_COUNTER_GET(pElement);
                }
            }
        }
    }
    //no free element found in IndTxBuffer
    if (indexAddedElem == MACCORE_INDTX_ENTRIES)
    {
        return false;
    }
    else
    {
        pElement = maccore_constglobals->MacCore_IndTxBuffer[indexAddedElem];
        pElement->srcAddrMode = srcAddrMode;
        MEMCPY(&pElement->dstAddrInfo, pDstAddrInfo, sizeof(gpMacCore_AddressInfo_t));
        pElement->txOptions = txOptions;
#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
        if(pSecOptions)
        {
            MEMCPY(&pElement->secOptions, pSecOptions, sizeof(gpMacCore_Security_t));
        }
        else
        {
            pElement->secOptions.securityLevel = 0xFF;
        }
#endif //GP_MACCORE_DIVERSITY_SECURITY_ENABLED
        MEMCPY(&pElement->multiChannelOptions, &multiChannelOptions, sizeof(gpMacCore_MultiChannelOptions_t));
        MEMCPY(&pElement->pdLoh, &pdLoh, sizeof(gpPd_Loh_t));
        pElement->stackId = stackId;
        MACCORE_INDTX_IN_FLIGHT_CLR(pElement);

        //set freshness counter
        if(highestFreshnessCounter < GP_MACCORE_INITIAL_FRESHNESS_CNTR)
        {
            MACCORE_INDTX_FRESHNESS_COUNTER_SET(pElement, GP_MACCORE_INITIAL_FRESHNESS_CNTR);
        }
        else
        {
            //freshness counter for a new device is always the highest
            MACCORE_INDTX_FRESHNESS_COUNTER_SET(pElement, highestFreshnessCounter + 1);
        }
        if(!existsElem)
        {
            if(gpMacCore_DataPending_QueueAdd(pDstAddrInfo, stackId) != gpMacCore_ResultSuccess)
            {
                // free slot
                gpPoolMem_Free(maccore_constglobals->MacCore_IndTxBuffer[indexAddedElem]);
                maccore_constglobals->MacCore_IndTxBuffer[indexAddedElem] = NULL;

                GP_LOG_PRINTF("Remove(a) IndTx entry %u",0,indexAddedElem);
                return false;
            }
        }
        if(gpMacCore_GetTransactionPersistenceTime(stackId))
        {
            if(!gpSched_ExistsEventArg(MacCore_IndTxBufferRemoveExpiredPacket,(void*) &(maccore_constglobals->pStackId[stackId])))
            {
                gpSched_ScheduleEventArg((UInt32)((UInt32)gpMacCore_GetTransactionPersistenceTime(stackId) *
                                           (UInt32)GP_MACCORE_BASE_SUPERFRAME_DURATION*GP_MACCORE_SYMBOL_DURATION) /
                                           (GP_MACCORE_INITIAL_FRESHNESS_CNTR-1),
                                         MacCore_IndTxBufferRemoveExpiredPacket,(void*) &(maccore_constglobals->pStackId[stackId]));
            }
            GP_LOG_PRINTF("TransPersist for st:%u = %lu",0,gpMacCore_GetTransactionPersistenceTime(stackId),
                          (unsigned long int)((gpMacCore_GetTransactionPersistenceTime(stackId)*(UInt32)GP_MACCORE_BASE_SUPERFRAME_DURATION*GP_MACCORE_SYMBOL_DURATION) /
                          (GP_MACCORE_INITIAL_FRESHNESS_CNTR-1)));
        }
        else
        {
            /* TransactionPersistenceTime == 0: we use this as "infinite" timeout */
            /* so in this case don't schedule "MacCore_IndTxBufferRemoveExpiredPacket" */
            GP_LOG_PRINTF("Infinite Persistence",0);
        }
        return true;
    }
}
#ifdef GP_MACCORE_DIVERSITY_POLL_RECIPIENT
UInt8 MacCore_IndTxBufferFindElemForTx(gpMacCore_AddressInfo_t *pAddressInfo, gpMacCore_StackId_t stackId)
{
    UInt8 i;
    MacCore_IndTxElement_t* pElement;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    UInt8 foundElemIndex = MACCORE_INDTX_ENTRIES;
    Bool multiple = false;
    Bool hasInFlight = false;

    // look for packet with lowest freshness counter
    // this packet is the oldest -> tx first
    // if more then on packet exists -> set frame pending bit to 1.
    for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        pElement = maccore_constglobals->MacCore_IndTxBuffer[i];
        if (pElement
        && (pElement->pdLoh.handle != GP_PD_INVALID_HANDLE)
        && (MACCORE_INDTX_FRESHNESS_COUNTER_GET(pElement) > 0))
        {
            if(MacCore_CompairAddress(&pElement->dstAddrInfo , pAddressInfo)
                && pElement->stackId == stackId)
            {
                if (MACCORE_INDTX_IN_FLIGHT_GET(pElement))
                {
                    hasInFlight = true;
                }

                // this is the first one we found
                if(foundElemIndex == MACCORE_INDTX_ENTRIES)
                {
                    foundElemIndex = i;
                }
                else
                {
                    // aha , multple packets exists,
                    if(MACCORE_INDTX_FRESHNESS_COUNTER_GET(maccore_constglobals->MacCore_IndTxBuffer[foundElemIndex]) \
                        > MACCORE_INDTX_FRESHNESS_COUNTER_GET(pElement))
                    {
                        // current packet has the lowest fresness counter (so far)
                        foundElemIndex = i;
                    }
                    multiple = true;
                }
            }
        }
    }
    //If a packet is still in flight for that address, don't send any packet
    if(hasInFlight)
    {
        return MACCORE_ADDRESS_HAS_IN_FLIGHT_PACKET;
    }
    if(foundElemIndex == MACCORE_INDTX_ENTRIES)
    {
        return MACCORE_ADDRESS_HAS_NO_PENDING_DATA;
    }
    else
    {
        if(multiple)
        {
            maccore_constglobals->MacCore_IndTxBuffer[foundElemIndex]->txOptions |= GP_MACCORE_TX_OPT_MORE_DATA_PENDING;
        }
        return foundElemIndex;
    }
}
#endif // GP_MACCORE_DIVERSITY_POLL_RECIPIENT

STATIC_FUNC void MacCore_IndTxBufferRemoveExpiredPacket(void* pStackId)
{
    UIntLoop i;
    MacCore_IndTxElement_t* pElement;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        pElement = maccore_constglobals->MacCore_IndTxBuffer[i];
        if (pElement
            && (pElement->pdLoh.handle != GP_PD_INVALID_HANDLE)
            && (*(UInt8*)pStackId == pElement->stackId)
            && !MACCORE_INDTX_IN_FLIGHT_GET(pElement))
        {
            if(MACCORE_INDTX_FRESHNESS_COUNTER_GET(pElement) == 1)
            {
                //Eligible for removal
                UInt8 pdHandle = pElement->pdLoh.handle;

                pElement->pdLoh.handle = GP_PD_INVALID_HANDLE;
                if(!MacCore_ExistsElemForAddress(&pElement->dstAddrInfo, *(UInt8*)pStackId))
                {
                    gpMacCore_DataPending_QueueRemove(&pElement->dstAddrInfo, pElement->stackId);
                }

                // free memory
                gpPoolMem_Free(pElement);

                // make available for other frames
                maccore_constglobals->MacCore_IndTxBuffer[i] = NULL;
                GP_LOG_PRINTF("Remove(b) IndTx entry %u",0,i);

                gpMacCore_cbDataConfirm(gpMacCore_ResultTransactionExpired, pdHandle);
            }
            else
            {
                // decrement freshness - remove next time
                MACCORE_INDTX_FRESHNESS_COUNTER_DEC(pElement);
            }
        }
    }

    if(MacCore_IndTxBufferNbrActiveElements(*(UInt8*)pStackId) > 0)
    {
        gpSched_ScheduleEventArg(
        (UInt32)((UInt32)gpMacCore_GetTransactionPersistenceTime(*(UInt8*)pStackId)*(UInt32)GP_MACCORE_BASE_SUPERFRAME_DURATION*GP_MACCORE_SYMBOL_DURATION)/(GP_MACCORE_INITIAL_FRESHNESS_CNTR-1)
        , MacCore_IndTxBufferRemoveExpiredPacket,pStackId);
    }
}

STATIC_FUNC Bool MacCore_CompairAddress(gpMacCore_AddressInfo_t* pAddressA , gpMacCore_AddressInfo_t* pAddressB)
{
    if(pAddressA->addressMode == pAddressB->addressMode)
    {
        if(pAddressA->addressMode == gpMacCore_AddressModeShortAddress)
        {
            if(   pAddressA->address.Short == pAddressB->address.Short
                && pAddressA->panId == pAddressB->panId)
            {
                return true;
            }
        }
        else if(pAddressA->addressMode == gpMacCore_AddressModeExtendedAddress)
        {
            if(   pAddressA->address.Extended.LSB == pAddressB->address.Extended.LSB
                && pAddressA->address.Extended.MSB == pAddressB->address.Extended.MSB)
            {
                return true;
            }
        }
    }
    return false;
}

STATIC_FUNC Bool MacCore_ExistsElemForAddress(gpMacCore_AddressInfo_t* pAddressInfo, gpMacCore_StackId_t stackId)
{
    MacCore_IndTxElement_t* pElement;
    UInt8 i;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        pElement = maccore_constglobals->MacCore_IndTxBuffer[i];

        if (pElement
        && (pElement->pdLoh.handle != GP_PD_INVALID_HANDLE)
        && (MACCORE_INDTX_FRESHNESS_COUNTER_GET(pElement) > 0)
        && (pElement->stackId == stackId)
        && (!MACCORE_INDTX_IN_FLIGHT_GET(pElement)))
        {
            if(MacCore_CompairAddress(&pElement->dstAddrInfo , pAddressInfo))
            {
                return true;
            }
        }
    }
    return false;
}

#ifdef GP_MACCORE_DIVERSITY_POLL_RECIPIENT
void MacCore_cbPollIndication(gpMacCore_AddressInfo_t* pAddrInfo, UInt8 stackId, gpPd_TimeStamp_t rxTime, Bool dataPending)
{
    gpMacCore_GlobalVars_t*      maccore_globals      = GP_MACCORE_GET_GLOBALS();
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    if(gpMacCore_GetForwardPollIndications(stackId))
    {
        gpMacCore_cbPollIndication(pAddrInfo,stackId,rxTime);
    }
    else
    {
        // Check Indirect Tx queue
        UInt8 index = MacCore_IndTxBufferFindElemForTx(pAddrInfo, stackId);
        Bool fromNeighbour = MacCore_CheckIfNeighbour(pAddrInfo, stackId);

        //There is still a pending in flight packet for this device, could be related to the device polling too fast or crowded networks, don't send a dummy packet
        if(index == MACCORE_ADDRESS_HAS_IN_FLIGHT_PACKET)
        {
            gpMacCore_cbPollNotify(pAddrInfo,stackId,rxTime, GP_PD_INVALID_HANDLE, fromNeighbour);
        }

        //There is no pending data for this device
        else if(index == MACCORE_ADDRESS_HAS_NO_PENDING_DATA)
        {
            /* for Non Neighbour devices, when we have no data, we don't send an empty frame */
            /* but we rely on upper layers to send a 'LEAVE' message */
            if(((MacCore_GetDataPendingModeStack(stackId) != gpMacCore_DataPendingModeForNonNeighbourDevices) ||
                fromNeighbour))
            {
                // Be polite and prepare empty data frame - for cheating on FP always set in Ack frame
                if(dataPending && (maccore_globals->MacCore_IndTxEmptyPacketPdHandle == GP_PD_INVALID_HANDLE))  //check if default FP = 1, then we should send empty packet, in other cases we can trust HW
                {
                    gpPd_Loh_t pdLoh;
                    gpMacCore_MultiChannelOptions_t mco;
                    UInt8 txOptions;

                    maccore_globals->MacCore_IndTxEmptyPacketPdHandle = gpPd_GetPd();
                    //If PD is valid, send dummy data
                    if(gpPd_CheckPdValid(maccore_globals->MacCore_IndTxEmptyPacketPdHandle) == gpPd_ResultValidHandle)
                    {
                        txOptions = 0; //dummy data packet should be transmitted without acknowledgment request
                        pdLoh.handle = maccore_globals->MacCore_IndTxEmptyPacketPdHandle;
                        pdLoh.length = 0;
                        pdLoh.offset = GP_MACCORE_MAX_MAC_HEADER_SIZE;
                        GP_MACCORE_SET_DEFAULT_MULTICHANNELOPTIONS(mco);

                        gpMacCore_DataRequest(gpMacCore_AddressModeShortAddress, pAddrInfo, txOptions, NULL , mco, pdLoh, stackId);
                    }
                }
            }

            //When there is no pending data to send, always notify with invalid PD handle, even when dummy packet is sent
            gpMacCore_cbPollNotify(pAddrInfo,stackId,rxTime, GP_PD_INVALID_HANDLE, fromNeighbour);
        }

        //Send pending data for this device
        else
        {
            MacCore_IndTxElement_t* pElement = maccore_constglobals->MacCore_IndTxBuffer[index];
            UInt8 retriesBackup;

            GP_STAT_SAMPLE_TIME();

            //Block any new attempts of transmitting/removing this packet
            MACCORE_INDTX_IN_FLIGHT_SET(pElement);

            //Indirect packet should not retry
            retriesBackup = gpMacCore_GetNumberOfRetries(stackId);
            gpMacCore_SetNumberOfRetries(0, stackId);

            gpMacCore_DataRequest(pElement->srcAddrMode,
                                  &pElement->dstAddrInfo,
                                  pElement->txOptions,
#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
                                  (pElement->secOptions.securityLevel == 0xFF) ? NULL : &pElement->secOptions,
#else
                                  NULL,
#endif //GP_MACCORE_DIVERSITY_SECURITY_ENABLED
                                  pElement->multiChannelOptions,
                                  pElement->pdLoh,
                                  pElement->stackId);

            //Re-store retries
            gpMacCore_SetNumberOfRetries(retriesBackup, stackId);

            // pollNotify after the DataRequest to optimize response time
            gpMacCore_cbPollNotify(pAddrInfo,stackId,rxTime, pElement->pdLoh.handle, fromNeighbour);
        }
    }
}

Bool MacCore_cbIndDataConfirm(gpMacCore_Result_t result,gpPd_Handle_t pdHandle)
{
    UIntLoop i;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    /* Empty frame failures should not be called to higher layer */
    if(maccore_globals->MacCore_IndTxEmptyPacketPdHandle == pdHandle)
    {
        gpPd_FreePd(maccore_globals->MacCore_IndTxEmptyPacketPdHandle);
        maccore_globals->MacCore_IndTxEmptyPacketPdHandle = GP_PD_INVALID_HANDLE;
        return true;
    }

    for(i=0; i < MACCORE_INDTX_ENTRIES; i++)
    {
        MacCore_IndTxElement_t* pElement = maccore_constglobals->MacCore_IndTxBuffer[i];

        if((NULL != pElement) && (pElement->pdLoh.handle == pdHandle))
        {
            if(gpMacCore_ResultSuccess == result)
            {
                const UInt8 stackId = pElement->stackId;
                //Remove packet from queue
                pElement->pdLoh.handle = GP_PD_INVALID_HANDLE;
                if(!MacCore_ExistsElemForAddress(&pElement->dstAddrInfo, stackId))
                {
                    gpMacCore_DataPending_QueueRemove(&pElement->dstAddrInfo, stackId);
                }
                // Free memory
                gpPoolMem_Free(pElement);

                // Make available for other frames
                maccore_constglobals->MacCore_IndTxBuffer[i] = NULL;
                GP_LOG_PRINTF("Remove(c) IndTx entry %d",0,i);
                if(MacCore_IndTxBufferNbrActiveElements(stackId) == 0)
                {
                    gpSched_UnscheduleEventArg(MacCore_IndTxBufferRemoveExpiredPacket, (void*) &(maccore_constglobals->pStackId[stackId]));
                }
                //Continue with normal flow
                return false;
            }
            else
            {
                //Keep data in queue until expiry
                MACCORE_INDTX_IN_FLIGHT_CLR(pElement);
                return true;
            }
        }
    }

    return false;
}

#endif // GP_MACCORE_DIVERSITY_POLL_RECIPIENT


/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpMacCore_PurgeRequest(gpPd_Handle_t pdHandle, gpMacCore_StackId_t stackId)
{
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();
    if ( GP_PD_INVALID_HANDLE != pdHandle )
    {
        UIntLoop i;
        MacCore_IndTxElement_t* pElement;
        for(i = 0; i < MACCORE_INDTX_ENTRIES; i++)
        {
            pElement = maccore_constglobals->MacCore_IndTxBuffer[i];

            if (pElement && (pElement->pdLoh.handle == pdHandle)
                && pElement->stackId == stackId)
            {
                pElement->pdLoh.handle = GP_PD_INVALID_HANDLE;
                if(!MacCore_ExistsElemForAddress(&pElement->dstAddrInfo, stackId))
                {
                    gpMacCore_DataPending_QueueRemove(&pElement->dstAddrInfo, pElement->stackId);
                }

                // free memory
                gpPoolMem_Free(pElement);

                // make available for other frames
                maccore_constglobals->MacCore_IndTxBuffer[i] = NULL;
                GP_LOG_PRINTF("Remove(d) IndTx entry %d",0,i);

                // clean up event
                if(MacCore_IndTxBufferNbrActiveElements(stackId) == 0)
                {
                    gpSched_UnscheduleEventArg(MacCore_IndTxBufferRemoveExpiredPacket, (void*) &(maccore_constglobals->pStackId[stackId]));
                }

                gpMacCore_cbPurgeConfirm(gpMacCore_ResultSuccess, pdHandle);
                return;
            }
        }
    }

    gpMacCore_cbPurgeConfirm(gpMacCore_ResultInvalidHandle, pdHandle);
}
