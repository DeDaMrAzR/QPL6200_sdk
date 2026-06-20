/*
 * Copyright (c) 2012-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpMacDispatcher.c
 *   This file contains the implementation of the MAC dispatcher.
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
// #define GP_LOCAL_LOG

// General includes
#include "gpAssert.h"
#include "gpLog.h"
#include "gpSched.h"
#include "gpStat.h"

#include "gpMacDispatcher.h"
#include "gpMacDispatcher_def.h"
#include "gpMacDispatcher_callbacks.h"
#include "gpMacDispatcher_clientServerCmdId.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_MACDISPATCHER

#define MACDISPATCHER_MAX_QUEUE_SIZE 5

#define MAC_DISPATCHER_DEFAULT_BOOL_VALUE   false
#define MAC_DISPATCHER_DEFAULT_INT8_VALUE   0x7F
#define MAC_DISPATCHER_DEFAULT_UINT8_VALUE  0xFF
#define MAC_DISPATCHER_DEFAULT_UINT16_VALUE 0xFFFF
#define MAC_DISPATCHER_DEFAULT_UINT32_VALUE 0xFFFFFFFF

#define MAC_DISPATCHER_STACK_ID_IN_VALID_RANGE(stackId)     (stackId < GP_DIVERSITY_NR_OF_STACKS)
#define MAC_DISPATCHER_STACK_REGISTERED(stackId)            (gpMacDispatcher_StackMapping[stackId].numericId != GP_MACCORE_STACK_UNDEFINED)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    gpMacCore_ScanType_t scanType;
    UInt32 scanChannels;
    UInt8 scanDuration;
    UInt8 resultListSize;
    UInt8* pEdScanResultList; // !!! we only keep the pointer itself. (There's no data yet when it's called)
} MacDispatcher_ScanRequest_Parameter_struct_t;

typedef struct {
    gpMacCore_AddressMode_t srcAddrMode;
    struct{ gpMacCore_AddressInfo_t data; Bool isNull;} pDstAddressInfo;
    UInt8 txOptions;
    struct{ gpMacCore_Security_t data; Bool isNull;} pSecOptions;
    gpMacCore_MultiChannelOptions_t multiChannelOptions;
    gpPd_Loh_t pdLoh;
} MacDispatcher_DataRequest_Parameter_struct_t;

typedef struct {
    struct{ gpMacCore_AddressInfo_t data; Bool isNull;} coordAddrInfo;
    struct{ gpMacCore_Security_t data; Bool isNull;} pSecOptions;
} MacDispatcher_PollRequest_Parameter_struct_t;

typedef struct {
    UInt8 logicalChannel;
    struct{ gpMacCore_AddressInfo_t data; Bool isNull;} coordAddrInfo;
    UInt8 capabilityInformation;
} MacDispatcher_AssociateRequest_Parameter_struct_t;
typedef struct {
    struct{ MACAddress_t data; Bool isNull;} pDeviceAddress;
    UInt16 associateShortAddress;
    gpMacDispatcher_Result_t status;
} MacDispatcher_AssociateResponse_Parameter_struct_t;

typedef struct {
    struct{ MACAddress_t data; Bool isNull;} pOrphanAddress;
    UInt16 shortAddress;
    Bool associatedMember;
} MacDispatcher_OrphanResponse_Parameter_struct_t;

typedef struct {
    gpPd_Handle_t pdHandle;
} MacDispatcher_PurgeRequest_Parameter_struct_t;

typedef struct {
    UInt8 MacDispatcher_CmdId;
    gpMacDispatcher_StackId_t stackId;
    union {
        MacDispatcher_ScanRequest_Parameter_struct_t MacDispatcher_ScanRequest_Parameter;
        MacDispatcher_DataRequest_Parameter_struct_t MacDispatcher_DataRequest_Parameter;
#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
        MacDispatcher_PollRequest_Parameter_struct_t MacDispatcher_PollRequest_Parameter;
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
        MacDispatcher_AssociateRequest_Parameter_struct_t MacDispatcher_AssociateRequest_Parameter;
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
        MacDispatcher_AssociateResponse_Parameter_struct_t MacDispatcher_AssociateResponse_Parameter;
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
        MacDispatcher_OrphanResponse_Parameter_struct_t MacDispatcher_OrphanResponse_Parameter;
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
        MacDispatcher_PurgeRequest_Parameter_struct_t MacDispatcher_PurgeRequest_Parameter;
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
    } Parameters;

} MacDispatcher_Generic_Parameter_struct_t;

typedef struct {
    gpMacDispatcher_StackId_t stackId;
    Bool directTx;
} MacDispatcher_StackInfo_t;

typedef struct {
    gpPd_Handle_t pdHandle;
    MacDispatcher_StackInfo_t info;
} MacDispatcher_StackPdHandleMapping_t;

struct MacDispatcher_QueuePointers_t {
    UInt8 inIndex;
    UInt8 outIndex;
    UInt8 nrEntries;
} MacDispatcher_QueuePointers;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

STATIC MacDispatcher_StackPdHandleMapping_t     MacDispatcher_StackPdMapping[GP_PD_NR_OF_HANDLES];
STATIC MacDispatcher_Generic_Parameter_struct_t MacDispatcher_Queue[MACDISPATCHER_MAX_QUEUE_SIZE];
STATIC gpMacDispatcher_StackId_t MacDispatcher_LockStack;
STATIC UInt8 MacDispatcher_LockCntr;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

// Pd linking functions
static void MacDispatcher_InitStackIdPdHandleMapping(void);
static Bool MacDispatcher_SetStackIdForPdHandle(gpMacDispatcher_StackId_t stackId, gpPd_Handle_t pdHandle,
                                                Bool directTx);
static Bool MacDispatcher_GetStackInfoForPdHandle(gpPd_Handle_t pdHandle, MacDispatcher_StackInfo_t* pInfo);
static void MacDispatcher_ClearStackIdPdHandleMapping(gpMacDispatcher_StackId_t stackId);

// general queueing functions
static void MacDispatcher_QueueInit(void);
static UInt8 MacDispatcher_QueueGetEntries(void);
static void MacDispatcher_dumpQueueEntry(UInt8 entry);
static void MacDispatcher_dumpQueue(void);
static MacDispatcher_Generic_Parameter_struct_t* MacDispatcher_QueueGetInPtr(void);
static MacDispatcher_Generic_Parameter_struct_t* MacDispatcher_QueueGetOutPtr(void);
static void MacDispatcher_QueueCheckAndCall(void);

// specific pack/unpack functions
static Bool MacDispatcher_ScanRequestPackAndQueue(gpMacCore_ScanType_t scanType, UInt32 scanChannels,
                                                  UInt8 scanDuration, UInt8 resultListSize, UInt8* pEdScanResultList,
                                                  gpMacDispatcher_StackId_t stackId);
static void MacDispatcher_ScanRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr);
static Bool MacDispatcher_DataRequestPackAndQueue(
    gpMacCore_AddressMode_t srcAddrMode, gpMacCore_AddressInfo_t* pDstAddrInfo, UInt8 txOptions,
    gpMacCore_Security_t* pSecOptions, gpMacCore_MultiChannelOptions_t multiChannelOptions, gpPd_Loh_t pdLoh,
    gpMacDispatcher_StackId_t stackId);
static void MacDispatcher_DataRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr);
#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
static Bool MacDispatcher_PollRequestPackAndQueue(gpMacCore_AddressInfo_t* pCoordAddrInfo,
                                                  gpMacCore_Security_t* pSecOptions, gpMacDispatcher_StackId_t stackId);
static void MacDispatcher_PollRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr);
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
static Bool MacDispatcher_AssociateRequestPackAndQueue(UInt8 logicalChannel, gpMacCore_AddressInfo_t* pCoordAddrInfo,
                                                       UInt8 capabilityInformation, gpMacDispatcher_StackId_t stackId);
static void MacDispatcher_AssociateRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr);
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
static Bool MacDispatcher_AssociateResponsePackAndQueue(MACAddress_t* pDeviceAddress, UInt16 associateShortAddress,
                                                        gpMacDispatcher_Result_t status,
                                                        gpMacDispatcher_StackId_t stackId);
static void MacDispatcher_AssociateResponseUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr);
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
static Bool MacDispatcher_OrphanResponsePackAndQueue(MACAddress_t* pOrphanAddress, UInt16 shortAddress,
                                                     Bool associatedMember, gpMacDispatcher_StackId_t stackId);
static void MacDispatcher_OrphanResponseUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr);
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
static Bool MacDispatcher_PurgeRequestPackAndQueue(gpPd_Handle_t pdHandle, gpMacDispatcher_StackId_t stackId);
static void MacDispatcher_PurgeRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr);
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

// Define static functions MacDispatcher_cbXXX() to call registered API callbacks.
#include "gpMacDispatcher_callbacks.c"

static void MacDispatcher_InitStackIdPdHandleMapping(void)
{
    UIntLoop initIndex;

    for (initIndex = 0; initIndex < GP_PD_NR_OF_HANDLES; initIndex++)
    {
        MacDispatcher_StackPdMapping[initIndex].pdHandle = GP_PD_INVALID_HANDLE;
        MacDispatcher_StackPdMapping[initIndex].info.stackId = GP_MAC_DISPATCHER_INVALID_STACK_ID;
    }
}

static Bool MacDispatcher_SetStackIdForPdHandle(gpMacDispatcher_StackId_t stackId, gpPd_Handle_t pdHandle,
                                                Bool directTx)
{
    UIntLoop searchIndex=0;

    for(searchIndex=0; searchIndex < GP_PD_NR_OF_HANDLES; searchIndex++)
    {
        if( GP_PD_INVALID_HANDLE == MacDispatcher_StackPdMapping[searchIndex].pdHandle )
        {
            MacDispatcher_StackPdMapping[searchIndex].pdHandle = pdHandle;
            MacDispatcher_StackPdMapping[searchIndex].info.stackId = stackId;
            MacDispatcher_StackPdMapping[searchIndex].info.directTx = directTx;
            return true;
        }
    }
    return false;
}

static Bool MacDispatcher_GetStackInfoForPdHandle(gpPd_Handle_t pdHandle, MacDispatcher_StackInfo_t* pInfo)
{
    UIntLoop searchIndex;
    for(searchIndex=0; searchIndex < GP_PD_NR_OF_HANDLES; searchIndex++)
    {
        if(MacDispatcher_StackPdMapping[searchIndex].pdHandle == pdHandle)
        {
            MacDispatcher_StackPdMapping[searchIndex].pdHandle = GP_PD_INVALID_HANDLE;
            if(pInfo)
            {
                MEMCPY(pInfo, &MacDispatcher_StackPdMapping[searchIndex].info, sizeof(MacDispatcher_StackInfo_t));
            }
            MacDispatcher_StackPdMapping[searchIndex].info.stackId = GP_MAC_DISPATCHER_INVALID_STACK_ID;
            return true;
        }
    }
    return false;
}

static void MacDispatcher_ClearStackIdPdHandleMapping(gpMacDispatcher_StackId_t stackId)
{
    UIntLoop searchIndex;
    for(searchIndex=0; searchIndex < GP_PD_NR_OF_HANDLES; searchIndex++)
    {
        if(MacDispatcher_StackPdMapping[searchIndex].info.stackId == stackId)
        {

            {
                MacDispatcher_StackPdMapping[searchIndex].pdHandle = GP_PD_INVALID_HANDLE;
                MacDispatcher_StackPdMapping[searchIndex].info.stackId  = GP_MAC_DISPATCHER_INVALID_STACK_ID;

                //If it was a direct TX, release the LOCK
                if(MacDispatcher_StackPdMapping[searchIndex].info.directTx)
                {
                    if(stackId == MacDispatcher_LockStack)
                    {
                        gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
                    }
                }
            }
        }
    }
}

void MacDispatcher_QueueInit(void)
{
    MacDispatcher_QueuePointers.inIndex=0;
    MacDispatcher_QueuePointers.outIndex=0;
    MacDispatcher_QueuePointers.nrEntries=0;
}

UInt8 MacDispatcher_QueueGetEntries(void)
{
    return MacDispatcher_QueuePointers.nrEntries;
}

void MacDispatcher_dumpQueueEntry(UInt8 entry)
{
    UInt8 index;

    index = (MacDispatcher_QueuePointers.inIndex + entry) % MACDISPATCHER_MAX_QUEUE_SIZE;
    NOT_USED(index);
    GP_LOG_PRINTF("entry %i command %i",0, entry, MacDispatcher_Queue[index].MacDispatcher_CmdId);
}

void MacDispatcher_dumpQueue(void)
{
    UIntLoop i;
    // GP_LOG_PRINTF("gpMacDispatcher queue entries: %i",0, MacDispatcher_QueueGetEntries());
    for(i=0; i< MacDispatcher_QueueGetEntries(); i++)
    {
        MacDispatcher_dumpQueueEntry(i);
    }
}
MacDispatcher_Generic_Parameter_struct_t* MacDispatcher_QueueGetInPtr(void)
{
    MacDispatcher_Generic_Parameter_struct_t* ptr;
    if(MacDispatcher_QueuePointers.nrEntries == MACDISPATCHER_MAX_QUEUE_SIZE)
    {
        ptr = NULL;
    }
    else
    {
        ptr = (&(MacDispatcher_Queue[MacDispatcher_QueuePointers.inIndex]));
        MacDispatcher_QueuePointers.inIndex = (MacDispatcher_QueuePointers.inIndex+1)%MACDISPATCHER_MAX_QUEUE_SIZE;
        MacDispatcher_QueuePointers.nrEntries++;
    }
    return ptr;
}

MacDispatcher_Generic_Parameter_struct_t* MacDispatcher_QueueGetOutPtr(void)
{
    MacDispatcher_Generic_Parameter_struct_t* ptr;
    if(MacDispatcher_QueuePointers.nrEntries == 0)
    {
        ptr = NULL;
    }
    else
    {
        ptr = (&(MacDispatcher_Queue[MacDispatcher_QueuePointers.outIndex]));
        MacDispatcher_QueuePointers.outIndex = (MacDispatcher_QueuePointers.outIndex+1)%MACDISPATCHER_MAX_QUEUE_SIZE;
        MacDispatcher_QueuePointers.nrEntries--;
    }
    return ptr;
}

#if defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
Bool MacDispatcher_ScanRequestPackAndQueue(gpMacCore_ScanType_t scanType, UInt32 scanChannels, UInt8 scanDuration,
                                           UInt8 resultListSize, UInt8* pEdScanResultList,
                                           gpMacDispatcher_StackId_t stackId)
{
    MacDispatcher_Generic_Parameter_struct_t* paramStruct;
    paramStruct = MacDispatcher_QueueGetInPtr();
    if(paramStruct == NULL)
    {
        GP_LOG_PRINTF("ScanReq: No place in Queue",0);
        MacDispatcher_dumpQueue();
        return false;
    }

    paramStruct->MacDispatcher_CmdId = gpMacDispatcher_ScanRequest_CmdId;
    paramStruct->stackId = stackId;

    paramStruct->Parameters.MacDispatcher_ScanRequest_Parameter.scanType                     = scanType;
    paramStruct->Parameters.MacDispatcher_ScanRequest_Parameter.scanChannels                 = scanChannels;
    paramStruct->Parameters.MacDispatcher_ScanRequest_Parameter.scanDuration                 = scanDuration;
    paramStruct->Parameters.MacDispatcher_ScanRequest_Parameter.resultListSize               = resultListSize;
    paramStruct->Parameters.MacDispatcher_ScanRequest_Parameter.pEdScanResultList            = pEdScanResultList;

    GP_LOG_PRINTF("gpMacDispatcher_ScanRequest adding to queue",0);
    //MacDispatcher_dumpQueue();

    return true;
}

void MacDispatcher_ScanRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr)
{
    gpMacDispatcher_ScanRequest(
        ptr->Parameters.MacDispatcher_ScanRequest_Parameter.scanType,
        ptr->Parameters.MacDispatcher_ScanRequest_Parameter.scanChannels,
        ptr->Parameters.MacDispatcher_ScanRequest_Parameter.scanDuration,
        ptr->Parameters.MacDispatcher_ScanRequest_Parameter.resultListSize,
        ptr->Parameters.MacDispatcher_ScanRequest_Parameter.pEdScanResultList,
        ptr->stackId
        );
}
#endif
Bool MacDispatcher_DataRequestPackAndQueue(gpMacCore_AddressMode_t srcAddrMode, gpMacCore_AddressInfo_t* pDstAddrInfo,
                                           UInt8 txOptions, gpMacCore_Security_t* pSecOptions,
                                           gpMacCore_MultiChannelOptions_t multiChannelOptions, gpPd_Loh_t pdLoh,
                                           gpMacDispatcher_StackId_t stackId)
{
    MacDispatcher_Generic_Parameter_struct_t* paramStruct;
    paramStruct = MacDispatcher_QueueGetInPtr();
    if(paramStruct == NULL)
    {
        GP_LOG_PRINTF("DataReq: No place in Queue",0);
        MacDispatcher_dumpQueue();
        return false;
    }

    paramStruct->MacDispatcher_CmdId = gpMacDispatcher_DataRequest_CmdId;
    paramStruct->stackId = stackId;

    paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.srcAddrMode                = srcAddrMode;
    paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.pDstAddressInfo.isNull     = true;
    if(pDstAddrInfo)
    {
        MEMCPY ( &paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.pDstAddressInfo.data, pDstAddrInfo, sizeof(gpMacCore_AddressInfo_t) );
        paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.pDstAddressInfo.isNull = false;
    }
    paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.txOptions                  = txOptions;
    paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.pSecOptions.isNull         = true;
    if(pSecOptions)
    {
        MEMCPY ( &paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.pSecOptions.data, pSecOptions, sizeof(gpMacCore_Security_t) );
        paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.pSecOptions.isNull     = false;
    }
    paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.pdLoh                      = pdLoh;
    paramStruct->Parameters.MacDispatcher_DataRequest_Parameter.multiChannelOptions        = multiChannelOptions;

    GP_LOG_PRINTF("gpMacDispatcher_DataRequest adding to queue", 0);
    //MacDispatcher_dumpQueue();

    return true;
}

void MacDispatcher_DataRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr)
{
    gpMacCore_AddressInfo_t* addressInfoPtr = NULL;
    gpMacCore_Security_t*    securityPtr    = NULL;

    if(ptr->Parameters.MacDispatcher_DataRequest_Parameter.pDstAddressInfo.isNull == false)
    {
        addressInfoPtr = &(ptr->Parameters.MacDispatcher_DataRequest_Parameter.pDstAddressInfo.data);
    }
    if(ptr->Parameters.MacDispatcher_DataRequest_Parameter.pSecOptions.isNull == false)
    {
        securityPtr = &(ptr->Parameters.MacDispatcher_DataRequest_Parameter.pSecOptions.data);
    }

    gpMacDispatcher_DataRequest(ptr->Parameters.MacDispatcher_DataRequest_Parameter.srcAddrMode, addressInfoPtr,
                                ptr->Parameters.MacDispatcher_DataRequest_Parameter.txOptions, securityPtr,
                                ptr->Parameters.MacDispatcher_DataRequest_Parameter.multiChannelOptions,
                                ptr->Parameters.MacDispatcher_DataRequest_Parameter.pdLoh, ptr->stackId);
}

#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
Bool MacDispatcher_PollRequestPackAndQueue(gpMacCore_AddressInfo_t* pCoordAddrInfo, gpMacCore_Security_t* pSecOptions,
                                           gpMacDispatcher_StackId_t stackId)
{
    MacDispatcher_Generic_Parameter_struct_t* paramStruct;
    paramStruct = MacDispatcher_QueueGetInPtr();
    if(paramStruct == NULL)
    {
        GP_LOG_PRINTF("PollReq: No place in Queue",0);
        MacDispatcher_dumpQueue();
        return false;
    }

    paramStruct->MacDispatcher_CmdId = gpMacDispatcher_PollRequest_CmdId;
    paramStruct->stackId = stackId;

    paramStruct->Parameters.MacDispatcher_PollRequest_Parameter.coordAddrInfo.isNull     = true;
    if(pCoordAddrInfo)
    {
        MEMCPY ( &paramStruct->Parameters.MacDispatcher_PollRequest_Parameter.coordAddrInfo.data, pCoordAddrInfo, sizeof(gpMacCore_AddressInfo_t) );
        paramStruct->Parameters.MacDispatcher_PollRequest_Parameter.coordAddrInfo.isNull = false;
    }
    paramStruct->Parameters.MacDispatcher_PollRequest_Parameter.pSecOptions.isNull     = true;
    if(pSecOptions)
    {
        MEMCPY ( &paramStruct->Parameters.MacDispatcher_PollRequest_Parameter.pSecOptions.data, pSecOptions, sizeof(gpMacCore_Security_t) );
        paramStruct->Parameters.MacDispatcher_PollRequest_Parameter.pSecOptions.isNull = false;
    }
    GP_LOG_PRINTF("gpMacDispatcher_PollRequest adding to queue",0);
    //MacDispatcher_dumpQueue();

    return true;
}

void MacDispatcher_PollRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr)
{
    gpMacCore_AddressInfo_t* addressInfoPtr = NULL;
    gpMacCore_Security_t *secOptionsPtr = NULL;

    if(ptr->Parameters.MacDispatcher_PollRequest_Parameter.coordAddrInfo.isNull == false)
    {
        addressInfoPtr = &(ptr->Parameters.MacDispatcher_PollRequest_Parameter.coordAddrInfo.data);
    }
    if(ptr->Parameters.MacDispatcher_PollRequest_Parameter.pSecOptions.isNull == false)
    {
        secOptionsPtr = &(ptr->Parameters.MacDispatcher_PollRequest_Parameter.pSecOptions.data);
    }

    gpMacDispatcher_PollRequest(addressInfoPtr, secOptionsPtr, ptr->stackId);
}
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
Bool MacDispatcher_AssociateRequestPackAndQueue(UInt8 logicalChannel, gpMacCore_AddressInfo_t* pCoordAddrInfo,
                                                UInt8 capabilityInformation, gpMacDispatcher_StackId_t stackId)
{
    MacDispatcher_Generic_Parameter_struct_t* paramStruct;
    paramStruct = MacDispatcher_QueueGetInPtr();
    if(paramStruct == NULL)
    {
        GP_LOG_PRINTF("AssocReq: No place in Queue",0);
        MacDispatcher_dumpQueue();
        return false;
    }
    paramStruct->MacDispatcher_CmdId = gpMacDispatcher_AssociateRequest_CmdId;
    paramStruct->stackId = stackId;

    paramStruct->Parameters.MacDispatcher_AssociateRequest_Parameter.logicalChannel           = logicalChannel;
    paramStruct->Parameters.MacDispatcher_AssociateRequest_Parameter.coordAddrInfo.isNull     = true;
    if(pCoordAddrInfo)
    {
        MEMCPY ( &paramStruct->Parameters.MacDispatcher_AssociateRequest_Parameter.coordAddrInfo.data, pCoordAddrInfo, sizeof(gpMacCore_AddressInfo_t) );
        paramStruct->Parameters.MacDispatcher_AssociateRequest_Parameter.coordAddrInfo.isNull = false;
    }
    paramStruct->Parameters.MacDispatcher_AssociateRequest_Parameter.capabilityInformation    = capabilityInformation;

    GP_LOG_PRINTF("gpMacDispatcher_AssociateRequest adding to queue", 0);
    //MacDispatcher_dumpQueue();

    return true;
}

void MacDispatcher_AssociateRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr)
{
    gpMacCore_AddressInfo_t* coordAddrInfo = NULL;

    if(ptr->Parameters.MacDispatcher_AssociateRequest_Parameter.coordAddrInfo.isNull == false)
    {
        coordAddrInfo = &(ptr->Parameters.MacDispatcher_AssociateRequest_Parameter.coordAddrInfo.data);
    }

    gpMacDispatcher_AssociateRequest(
        ptr->Parameters.MacDispatcher_AssociateRequest_Parameter.logicalChannel,
        coordAddrInfo,
        ptr->Parameters.MacDispatcher_AssociateRequest_Parameter.capabilityInformation,
        ptr->stackId
        );
}
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
Bool MacDispatcher_AssociateResponsePackAndQueue(MACAddress_t* pDeviceAddress, UInt16 associateShortAddress,
                                                 gpMacDispatcher_Result_t status, gpMacDispatcher_StackId_t stackId)
{
    MacDispatcher_Generic_Parameter_struct_t* paramStruct;
    paramStruct = MacDispatcher_QueueGetInPtr();
    if(paramStruct == NULL)
    {
        GP_LOG_PRINTF("AssocResp: No place in Queue",0);
        MacDispatcher_dumpQueue();
        return false;
    }
    paramStruct->MacDispatcher_CmdId = gpMacDispatcher_AssociateResponse_CmdId;
    paramStruct->stackId = stackId;

    paramStruct->Parameters.MacDispatcher_AssociateResponse_Parameter.pDeviceAddress.isNull     = true;
    if(pDeviceAddress)
    {
        MEMCPY ( &paramStruct->Parameters.MacDispatcher_AssociateResponse_Parameter.pDeviceAddress.data, pDeviceAddress, sizeof(MACAddress_t) );
        paramStruct->Parameters.MacDispatcher_AssociateResponse_Parameter.pDeviceAddress.isNull = false;
    }
    paramStruct->Parameters.MacDispatcher_AssociateResponse_Parameter.associateShortAddress    = associateShortAddress;
    paramStruct->Parameters.MacDispatcher_AssociateResponse_Parameter.status                   = status;

    GP_LOG_PRINTF("gpMacDispatcher_AssociateResponse adding to queue",0);
    //MacDispatcher_dumpQueue();

    return true;
}

void MacDispatcher_AssociateResponseUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr)
{
    MACAddress_t* pDeviceAddress = NULL;

    if(ptr->Parameters.MacDispatcher_AssociateResponse_Parameter.pDeviceAddress.isNull == false)
    {
        pDeviceAddress = &(ptr->Parameters.MacDispatcher_AssociateResponse_Parameter.pDeviceAddress.data);
    }

    gpMacDispatcher_AssociateResponse(
        pDeviceAddress,
        ptr->Parameters.MacDispatcher_AssociateResponse_Parameter.associateShortAddress,
        ptr->Parameters.MacDispatcher_AssociateResponse_Parameter.status,
        ptr->stackId
        );
}
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
Bool MacDispatcher_OrphanResponsePackAndQueue(MACAddress_t* pOrphanAddress, UInt16 shortAddress, Bool associatedMember,
                                              gpMacDispatcher_StackId_t stackId)
{
    MacDispatcher_Generic_Parameter_struct_t* paramStruct;
    paramStruct = MacDispatcher_QueueGetInPtr();
    if(paramStruct == NULL)
    {
        GP_LOG_PRINTF("OrphanResp: No place in Queue",0);
        MacDispatcher_dumpQueue();
        return false;
    }
    paramStruct->MacDispatcher_CmdId = gpMacDispatcher_OrphanResponse_CmdId;
    paramStruct->stackId = stackId;

    paramStruct->Parameters.MacDispatcher_OrphanResponse_Parameter.pOrphanAddress.isNull       = true;
    if(pOrphanAddress)
    {
        MEMCPY ( &paramStruct->Parameters.MacDispatcher_OrphanResponse_Parameter.pOrphanAddress.data, pOrphanAddress, sizeof(MACAddress_t) );
        paramStruct->Parameters.MacDispatcher_OrphanResponse_Parameter.pOrphanAddress.isNull   = false;
    }
    paramStruct->Parameters.MacDispatcher_OrphanResponse_Parameter.shortAddress                = shortAddress;
    paramStruct->Parameters.MacDispatcher_OrphanResponse_Parameter.associatedMember            = associatedMember;

    GP_LOG_PRINTF("gpMacDispatcher_OrphanResponse adding to queue",0);
    //MacDispatcher_dumpQueue();

    return true;
}

void MacDispatcher_OrphanResponseUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr)
{
    MACAddress_t* pOrphanAddress = NULL;

    if(ptr->Parameters.MacDispatcher_OrphanResponse_Parameter.pOrphanAddress.isNull == false)
    {
        pOrphanAddress = &(ptr->Parameters.MacDispatcher_OrphanResponse_Parameter.pOrphanAddress.data);
    }

    gpMacDispatcher_OrphanResponse(
        pOrphanAddress,
        ptr->Parameters.MacDispatcher_OrphanResponse_Parameter.shortAddress,
        ptr->Parameters.MacDispatcher_OrphanResponse_Parameter.associatedMember,
        ptr->stackId
        );
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
Bool MacDispatcher_PurgeRequestPackAndQueue(gpPd_Handle_t pdHandle, gpMacDispatcher_StackId_t stackId)
{
    MacDispatcher_Generic_Parameter_struct_t* paramStruct;
    paramStruct = MacDispatcher_QueueGetInPtr();
    if(paramStruct == NULL)
    {
        GP_LOG_PRINTF("PurgeRequest: No place in Queue",0);
        MacDispatcher_dumpQueue();
        return false;
    }
    paramStruct->MacDispatcher_CmdId = gpMacDispatcher_PurgeRequest_CmdId;
    paramStruct->stackId = stackId;
    paramStruct->Parameters.MacDispatcher_PurgeRequest_Parameter.pdHandle = pdHandle;

    GP_LOG_PRINTF("gpMacDispatcher_PurgeRequest adding to queue",0);
    //MacDispatcher_dumpQueue();

    return true;
}

void MacDispatcher_PurgeRequestUnpackAndCall(MacDispatcher_Generic_Parameter_struct_t* ptr)
{
    gpMacDispatcher_PurgeRequest(
        ptr->Parameters.MacDispatcher_PurgeRequest_Parameter.pdHandle,
        ptr->stackId
        );
}
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION

void MacDispatcher_QueueCheckAndCall(void)
{
    MacDispatcher_Generic_Parameter_struct_t* ptr;

    // GP_LOG_PRINTF("MacDispatcher_QueueCheckAndCall", 0);
    // MacDispatcher_dumpQueue();
    ptr = MacDispatcher_QueueGetOutPtr();
    if(ptr != NULL)
    {
        switch(ptr->MacDispatcher_CmdId)
        {
            case gpMacDispatcher_DataRequest_CmdId:
            {
                MacDispatcher_DataRequestUnpackAndCall(ptr);
                break;
            }
#if defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
            case gpMacDispatcher_ScanRequest_CmdId:
            {
                MacDispatcher_ScanRequestUnpackAndCall(ptr);
                break;
            }
#endif // defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
            case gpMacDispatcher_AssociateRequest_CmdId:
            {
                MacDispatcher_AssociateRequestUnpackAndCall(ptr);
                break;
            }
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
            case gpMacDispatcher_AssociateResponse_CmdId:
            {
                MacDispatcher_AssociateResponseUnpackAndCall(ptr);
                break;
            }
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
            case gpMacDispatcher_OrphanResponse_CmdId:
             {
                 MacDispatcher_OrphanResponseUnpackAndCall(ptr);
                 break;
             }
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
            case gpMacDispatcher_PollRequest_CmdId:
            {
                MacDispatcher_PollRequestUnpackAndCall(ptr);
                break;
            }
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
            case gpMacDispatcher_PurgeRequest_CmdId:
            {
                MacDispatcher_PurgeRequestUnpackAndCall(ptr);
                break;
            }
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
            default:
            {
                GP_ASSERT_DEV_INT(true);
                break;
            }
        }
    }
}

/*****************************************************************************
 *                    MacCore Callback Function Definitions
 *****************************************************************************/

void gpMacCore_cbDataConfirm(gpMacCore_Result_t status, gpPd_Handle_t pdHandle)
{
    MacDispatcher_StackInfo_t stackInfo;
    gpMacDispatcher_StackId_t stackId;
    if(MacDispatcher_GetStackInfoForPdHandle(pdHandle, &stackInfo))
    {
        stackId = stackInfo.stackId;
    }
    else
    {
        stackId = GP_MAC_DISPATCHER_INVALID_STACK_ID;
    }

    GP_LOG_PRINTF("gpMacCore_cbDataConfirm stack %u", 0, (gpMacCore_StackId_t)stackId);
    GP_STAT_SAMPLE_TIME();
    if(stackId != GP_MAC_DISPATCHER_INVALID_STACK_ID)
    {
        if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
        {
            // If stack is invalid, it has been unregistered with a packet in the fly
            MacDispatcher_HandleAutoTxAntennaToggling((gpMacDispatcher_Result_t)status,
                                                      (gpMacDispatcher_StackId_t)stackId);
        }
        MacDispatcher_cbDataConfirm((gpMacDispatcher_Result_t)status, pdHandle, (gpMacDispatcher_StackId_t)stackId);

        // If it is a direct TX, release the Lock
        if(stackInfo.directTx)
        {
            if(MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID)
            {
                gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
            }
        }
    }
    else
    {
        // Catch any incoming data confirm that was in flight during stack de-registration.
        gpPd_FreePd(pdHandle);
    }
}

void gpMacCore_cbDataIndication(gpMacCore_AddressInfo_t* pSrcAddrInfo, gpMacCore_AddressInfo_t* pDstAddrInfo, UInt8 dsn,
                                gpMacCore_Security_t* pSecOptions, gpPd_Loh_t pdLoh, gpMacCore_StackId_t stackId)
{
    GP_ASSERT_DEV_INT((gpMacDispatcher_StackId_t)stackId != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbDataIndication stack %u", 0, stackId);
    GP_STAT_SAMPLE_TIME();
    MacDispatcher_cbDataIndication(pSrcAddrInfo, pDstAddrInfo, dsn, pSecOptions, pdLoh,
                                   (gpMacDispatcher_StackId_t)stackId);
}

#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
void gpMacCore_cbPollConfirm(gpMacCore_Result_t status, gpMacCore_AddressInfo_t* pCoordAddrInfo,
                             gpPd_TimeStamp_t txTime)
{
    GP_ASSERT_DEV_INT(MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbPollConfirm", 0);
    GP_STAT_SAMPLE_TIME();
    MacDispatcher_cbPollConfirm((gpMacDispatcher_Result_t)status, pCoordAddrInfo, txTime, MacDispatcher_LockStack);
    gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
}
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_POLL_RECIPIENT
void gpMacCore_cbPollIndication(gpMacCore_AddressInfo_t* pAddrInfo, gpMacCore_StackId_t stackId,
                                gpPd_TimeStamp_t rxTime)
{
    // GP_LOG_SYSTEM_PRINTF("gpMacCore_cbPollIndication stack %x ", 2, stackId);
    GP_ASSERT_DEV_INT((gpMacDispatcher_StackId_t)stackId != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    MacDispatcher_cbPollIndication(pAddrInfo, rxTime, (gpMacDispatcher_StackId_t)stackId);
}

void gpMacCore_cbPollNotify(gpMacCore_AddressInfo_t* pAddrInfo, gpMacCore_StackId_t stackId, gpPd_TimeStamp_t rxTime,
                            gpPd_Handle_t pdHandle, Bool fromNeighbour)
{
    // GP_LOG_SYSTEM_PRINTF("gpMacCore_cbPollNotify stack %x ", 2, stackId);
    GP_ASSERT_DEV_INT((gpMacDispatcher_StackId_t)stackId != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    MacDispatcher_cbPollNotify(pAddrInfo, rxTime, pdHandle, fromNeighbour, (gpMacDispatcher_StackId_t)stackId);
}
#endif // GP_MACCORE_DIVERSITY_POLL_RECIPIENT

#if(defined(GP_MACCORE_DIVERSITY_SCAN_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR))
void gpMacCore_cbBeaconNotifyIndication(UInt8 bsn, gpMacCore_PanDescriptor_t* pPanDescriptor,
                                        gpMacCore_StackId_t stackId, UInt8 beaconPayloadLength, UInt8* pBeaconPayload)
{
    GP_LOG_PRINTF("gpMacCore_cbBeaconNotifyIndication stack %u", 0, stackId);
    MacDispatcher_cbBeaconNotifyIndication(bsn, pPanDescriptor, beaconPayloadLength, pBeaconPayload,
                                           (gpMacDispatcher_StackId_t)stackId);
}
#endif
#if(defined(GP_MACCORE_DIVERSITY_SCAN_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR) ||               \
    defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR))
void gpMacCore_cbScanConfirm(gpMacCore_Result_t status, gpMacCore_ScanType_t scanType, UInt32 unscannedChannels,
                             UInt8 resultListSize, UInt8* pEdScanResultList)
{
    GP_ASSERT_DEV_INT(MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbScanConfirm", 0);
    MacDispatcher_cbScanConfirm((gpMacDispatcher_Result_t)status, scanType, unscannedChannels, resultListSize,
                                pEdScanResultList, MacDispatcher_LockStack);
    gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
void gpMacCore_cbAssociateConfirm(UInt16 assocShortAddress, gpMacCore_AssocStatus_t status, gpPd_TimeStamp_t txTime)
{
    GP_ASSERT_DEV_INT(MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbAssociateConfirm", 0);
    MacDispatcher_cbAssociateConfirm(assocShortAddress, (gpMacDispatcher_Result_t)status, txTime,
                                     MacDispatcher_LockStack);
    gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
}
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
void gpMacCore_cbAssociateIndication(gpMacCore_Address_t* pDeviceAddress, UInt8 capabilityInformation,
                                     gpMacCore_StackId_t stackId, gpPd_TimeStamp_t rxTime)
{
    GP_ASSERT_DEV_INT((gpMacDispatcher_StackId_t)stackId != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbAssociateIndication stack %u", 0, stackId);
    MacDispatcher_cbAssociateIndication(pDeviceAddress, capabilityInformation, rxTime,
                                        (gpMacDispatcher_StackId_t)stackId);
}

void gpMacCore_cbAssociateCommStatusIndication(gpMacCore_AddressInfo_t* pSrcAddrInfo,
                                               gpMacCore_AddressInfo_t* pDstAddrInfo, gpMacCore_Result_t status,
                                               gpMacCore_StackId_t stackId, gpPd_TimeStamp_t txTime)
{
    NOT_USED(stackId);
    GP_ASSERT_DEV_INT(MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbAssociateCommStatusIndication stack %u", 0, stackId);
    MacDispatcher_cbAssociateCommStatusIndication(pSrcAddrInfo, pDstAddrInfo, (gpMacDispatcher_Result_t)status, txTime,
                                                  MacDispatcher_LockStack);
    gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
}
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
void gpMacCore_cbOrphanCommStatusIndication(gpMacCore_AddressInfo_t* pSrcAddrInfo,
                                            gpMacCore_AddressInfo_t* pDstAddrInfo, gpMacCore_Result_t status,
                                            gpMacCore_StackId_t stackId, gpPd_TimeStamp_t txTime)
{
    NOT_USED(stackId);
    GP_ASSERT_DEV_INT(MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbOrphanCommStatusIndication stack %u", 0, stackId);
    MacDispatcher_cbOrphanCommStatusIndication(pSrcAddrInfo, pDstAddrInfo, (gpMacDispatcher_Result_t)status, txTime,
                                               MacDispatcher_LockStack);
    gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
void gpMacCore_cbPurgeConfirm(gpMacCore_Result_t status, gpPd_Handle_t pdHandle)
{
    GP_ASSERT_DEV_INT(MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID);
    GP_LOG_PRINTF("gpMacCore_cbPurgeConfirm", 0);
    if(status == gpMacCore_ResultSuccess)
    {
        // clear the stackid <-> pd mapping
        MacDispatcher_StackInfo_t stackInfo;
        MacDispatcher_GetStackInfoForPdHandle(pdHandle, &stackInfo);
    }
    MacDispatcher_cbPurgeConfirm((gpMacDispatcher_Result_t)status, pdHandle, MacDispatcher_LockStack);
    gpMacDispatcher_LockRelease(MacDispatcher_LockStack);
}
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
void gpMacCore_cbOrphanIndication(MACAddress_t* pOrphanAddress, gpMacCore_StackId_t stackId, gpPd_TimeStamp_t rxTime)
{
    GP_LOG_PRINTF("gpMacCore_cbOrphanIndication stack %u", 0, stackId);
    MacDispatcher_cbOrphanIndication(pOrphanAddress, rxTime, (gpMacDispatcher_StackId_t)stackId);
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT

void gpMacCore_cbSecurityFailureCommStatusIndication(
    gpMacCore_AddressInfo_t* pSrcAddrInfo, gpMacCore_AddressInfo_t* pDstAddrInfo, gpMacDispatcher_Result_t status,
    gpMacCore_StackId_t stackId, gpPd_TimeStamp_t time)
{
    GP_LOG_PRINTF("gpMacCore_cbSecurityFailureCommStatusIndication stack %u", 0, stackId);
    MacDispatcher_cbSecurityFailureCommStatusIndication(pSrcAddrInfo, pDstAddrInfo, (gpMacDispatcher_Result_t)status,
                                                        time, (gpMacDispatcher_StackId_t)stackId);
}

void gpMacCore_cbSecurityFrameCounterIndication(UInt32 frameCounter, gpMacCore_StackId_t stackId)
{
    NOT_USED(frameCounter);
    NOT_USED(stackId);
#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
    GP_LOG_PRINTF("gpMacCore_cbSecurityFrameCounterIndication stack %u", 0, stackId);
    MacDispatcher_cbSecurityFrameCounterIndication(frameCounter, (gpMacDispatcher_StackId_t)stackId);
#endif // GP_MACCORE_DIVERSITY_SECURITY_ENABLED
}

Bool gpMacCore_cbValidStack(gpMacCore_StackId_t stackId)
{
    return ((MAC_DISPATCHER_STACK_ID_IN_VALID_RANGE(stackId) && MAC_DISPATCHER_STACK_REGISTERED(stackId)));
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpMacDispatcher_Init(void)
{
    MacDispatcher_LockStack = GP_MAC_DISPATCHER_INVALID_STACK_ID;
    MacDispatcher_LockCntr = 0;

    MacDispatcher_QueueInit();

    MacDispatcher_InitStackMapping();
    MacDispatcher_InitStackIdPdHandleMapping();
}

void gpMacDispatcher_DeInit(void)
{

    MacDispatcher_InitStackIdPdHandleMapping();
    MacDispatcher_InitStackMapping();
    MacDispatcher_QueueInit();

    MacDispatcher_LockStack = GP_MAC_DISPATCHER_INVALID_STACK_ID;
    MacDispatcher_LockCntr = 0;
}

Bool gpMacDispatcher_LockClaim(gpMacDispatcher_StackId_t stackId)
{
    Bool rv = false;

    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        if (MacDispatcher_LockStack == GP_MAC_DISPATCHER_INVALID_STACK_ID)
        {
            GP_ASSERT_DEV_INT(MacDispatcher_LockCntr == 0);
            MacDispatcher_LockStack = stackId;
            MacDispatcher_LockCntr++;
            rv = true;
        }
        else if (MacDispatcher_LockStack == stackId)
        {
            MacDispatcher_LockStack = stackId;
            MacDispatcher_LockCntr++;
            GP_ASSERT_DEV_INT(MacDispatcher_LockCntr != 0);
            rv = true;
        }
        else
        {
            rv = false;
        }
    }
    GP_LOG_PRINTF("LockClaim stack %i Count %i Success %i Locked by: %i", 0, stackId, MacDispatcher_LockCntr, rv,
                  MacDispatcher_LockStack);
    return rv;
}

void gpMacDispatcher_LockRelease(gpMacDispatcher_StackId_t stackId)
{
    GP_ASSERT_DEV_INT(stackId == MacDispatcher_LockStack);
    GP_ASSERT_DEV_INT(MacDispatcher_LockCntr);
    if((stackId == MacDispatcher_LockStack) && MacDispatcher_LockCntr)
    {
        MacDispatcher_LockCntr--;
        if(MacDispatcher_LockCntr == 0)
        {
            MacDispatcher_LockStack = GP_MAC_DISPATCHER_INVALID_STACK_ID;

            if(MacDispatcher_QueueGetEntries())
            {
                GP_LOG_PRINTF("QueueCheckAndCall %i pending", 0, MacDispatcher_QueueGetEntries());
                gpSched_ScheduleEvent(0, MacDispatcher_QueueCheckAndCall);
            }
        }
    }
    GP_LOG_PRINTF("LockRelease stack %i count %i", 0, stackId, MacDispatcher_LockCntr);
}

Bool gpMacDispatcher_LockedByThisStack(gpMacDispatcher_StackId_t stackId)
{
    return stackId == MacDispatcher_LockStack;
}

Bool gpMacDispatcher_Locked(void)
{
    return MacDispatcher_LockStack != GP_MAC_DISPATCHER_INVALID_STACK_ID;
}

gpMacDispatcher_StackId_t gpMacDispatcher_RegisterNetworkStack(gpMacDispatcher_StringIdentifier_t* stringIdentifier)
{
    gpMacDispatcher_StackId_t stackId = GP_MAC_DISPATCHER_INVALID_STACK_ID;
    UIntLoop i;

    GP_LOG_SYSTEM_PRINTF("Stack register: %c%c%c%c%c", 0, stringIdentifier->str[0], stringIdentifier->str[1],
                         stringIdentifier->str[2], stringIdentifier->str[3], stringIdentifier->str[4]);

    // check if stack is in the list
    for(i = 0; i < GP_DIVERSITY_NR_OF_STACKS; i++)
    {
        if(MEMCMP(gpMacDispatcher_StackMapping[i].stringId.str, stringIdentifier, sizeof(gpMacDispatcher_StringIdentifier_t)) == 0)
        {
            // stack is already known to the MacDispatcher, just return its previous stackId
            stackId = gpMacDispatcher_StackMapping[i].numericId;
            GP_LOG_SYSTEM_PRINTF("Stack is known, re-register %i! stackId: 0x%x", 0, i, stackId);
            //send a reset indication to the previous running stack, it is up to the application to decide what to do
            MacDispatcher_cbDriverResetIndication(gpMacDispatcher_ResultSecondStackRegistered, stackId);
            //clear mapping of previous running requests
            MacDispatcher_ClearStackIdPdHandleMapping(stackId);
            //Let MacCore know a stack has registered
            gpMacCore_StackAdded((gpMacCore_StackId_t)stackId);
            return stackId;
        }
    }

    // stack not in the list, check if there is room available
    for(i = 0; i < GP_DIVERSITY_NR_OF_STACKS; i++)
    {
        if(!MAC_DISPATCHER_STACK_REGISTERED(i))
        {
            // first time registration ==> allocate a stack Id
            stackId = i;
            MEMCPY(gpMacDispatcher_StackMapping[i].stringId.str, stringIdentifier, sizeof(gpMacDispatcher_StringIdentifier_t));
            gpMacDispatcher_StackMapping[i].numericId = stackId;
            MacDispatcher_InitAutoTxAntennaToggling(stackId);
            //Let MacCore know a new stack has registered
            gpMacCore_StackAdded((gpMacCore_StackId_t)stackId);
            GP_LOG_SYSTEM_PRINTF("Unknown stack, register %i! new stackId: 0x%x", 0, i, stackId);
            return stackId;
        }
    }
    return stackId;
}

gpMacDispatcher_Result_t gpMacDispatcher_UnRegisterNetworkStack(gpMacDispatcher_StackId_t stackId)
{
    gpMacDispatcher_Result_t result = gpMacDispatcher_ResultSuccess;

    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("Stack unregister: 0x%x", 0, stackId);
        MacDispatcher_UnRegisterCallbacks(stackId);
        //Let MacCore know a stack is removed
        gpMacCore_StackRemoved((gpMacCore_StackId_t)stackId);
        //Clear Pd Handle Mapping for this stack ID
        MacDispatcher_ClearStackIdPdHandleMapping(stackId);
    }
    else
    {
        result = gpMacDispatcher_ResultUnsupportedAttribute;
    }
    return result;
}

gpMacDispatcher_Result_t gpMacDispatcher_Reset(Bool setDefaultPib, gpMacDispatcher_StackId_t stackId)
{
    gpMacDispatcher_Result_t result = gpMacDispatcher_ResultInvalidParameter;
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_Reset %d", 0, stackId);
        result = gpMacCore_Reset(setDefaultPib, (gpMacCore_StackId_t)stackId);
        if (setDefaultPib && (result == gpMacCore_ResultSuccess))
        {
            MacDispatcher_InitAutoTxAntennaToggling(stackId);
        }

        // Clear Pd Handle Mapping for this stack ID
        MacDispatcher_ClearStackIdPdHandleMapping(stackId);
    }
    return (gpMacDispatcher_Result_t)result;
}

void gpMacDispatcher_DataRequest(gpMacCore_AddressMode_t srcAddrMode, gpMacCore_AddressInfo_t* pDstAddrInfo,
                                 UInt8 txOptions, gpMacCore_Security_t* pSecOptions,
                                 gpMacCore_MultiChannelOptions_t multiChannelOptions, gpPd_Loh_t pdLoh,
                                 gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_DataRequest stack %d", 0, (gpMacCore_StackId_t)stackId);
#if defined(GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION) || defined(GP_MACCORE_DIVERSITY_TIMEDTX)
        //Do not lock mac dispatcher for indirect or scheduled tranmission !
        if(GP_MACCORE_INDIRECT_TRANSMISSION_ENABLED(txOptions) || GP_MACCORE_TIMEDTX_ENABLED(txOptions))
        {
            if(MacDispatcher_SetStackIdForPdHandle(stackId,pdLoh.handle, false))
            {
                GP_LOG_PRINTF("gpMacCore_DataRequest Indirect",0);
                gpMacCore_DataRequest(srcAddrMode, pDstAddrInfo, txOptions, pSecOptions, multiChannelOptions, pdLoh,
                                      (gpMacCore_StackId_t)stackId);
            }
            else
            {
                GP_LOG_PRINTF("gpMacCore_DataRequest Indirect - PdHandle",0);
                MacDispatcher_cbDataConfirm(gpMacCore_ResultTransactionOverflow, pdLoh.handle,
                                            (gpMacCore_StackId_t)stackId);
            }
        }
        else
#endif //defined(GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION) || defined(GP_MACCORE_DIVERSITY_TIMEDTX)
        {
            if (gpMacDispatcher_LockClaim(stackId))
            {
                GP_LOG_PRINTF("gpMacCore_DataRequest %u", 0, (gpMacCore_StackId_t)stackId);
                GP_STAT_SAMPLE_TIME();

                if(MacDispatcher_SetStackIdForPdHandle(stackId,pdLoh.handle, true))
                {
                    gpMacCore_DataRequest(srcAddrMode, pDstAddrInfo, txOptions, pSecOptions, multiChannelOptions, pdLoh,
                                          (gpMacCore_StackId_t)stackId);
                }
                else
                {
                    GP_LOG_PRINTF("gpMacCore_DataRequest - PdHandle",0);
                    gpMacDispatcher_LockRelease((gpMacCore_StackId_t)stackId);
                    MacDispatcher_cbDataConfirm(gpMacCore_ResultTransactionOverflow, pdLoh.handle,
                                                (gpMacCore_StackId_t)stackId);
                }
            }
            else
            {
                if(!MacDispatcher_DataRequestPackAndQueue(srcAddrMode, pDstAddrInfo, txOptions, pSecOptions, multiChannelOptions, pdLoh, stackId))
                {
                    GP_LOG_PRINTF("gpMacCore_DataRequest - DataRequestPackAndQueue",0);
                    MacDispatcher_cbDataConfirm(gpMacCore_ResultTransactionOverflow, pdLoh.handle,
                                                (gpMacCore_StackId_t)stackId);
                }
            }
        }
    }
}

#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
gpMacDispatcher_Result_t gpMacDispatcher_ScheduleTimedTx(
    gpPd_Handle_t pdHandle, gpMacCore_TxTimingOptions_t timingOptions, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_ScheduleTimedTx stack %d pd %d", 0, stackId, pdHandle);
        return (gpMacDispatcher_Result_t)gpMacCore_ScheduleTimedTx(pdHandle, timingOptions,
                                                                   (gpMacCore_StackId_t)stackId);
    }
    return gpMacDispatcher_ResultInvalidParameter;
}
#endif // GP_MACCORE_DIVERSITY_TIMEDTX

#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
void gpMacDispatcher_PurgeRequest(gpPd_Handle_t pdHandle, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_PurgeRequest stack %d", 0, (gpMacCore_StackId_t)stackId);
        if(gpMacDispatcher_LockClaim(stackId))
        {
            GP_LOG_PRINTF("gpMacCore_PurgeRequest",0);
            gpMacCore_PurgeRequest(pdHandle, (gpMacCore_StackId_t)stackId);
        }
        else
        {
            if(!MacDispatcher_PurgeRequestPackAndQueue( pdHandle, stackId))
            {
                MacDispatcher_cbPurgeConfirm(gpMacCore_ResultTransactionOverflow, pdHandle,
                                             (gpMacCore_StackId_t)stackId);
            }
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION

#if defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
void gpMacDispatcher_ScanRequest(gpMacCore_ScanType_t scanType, UInt32 scanChannels, UInt8 scanDuration,
                                 UInt8 resultListSize, UInt8* pEdScanResultList, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        if(gpMacDispatcher_LockClaim(stackId))
        {
            GP_LOG_PRINTF("gpMacCore_ScanRequest typ:%x ch:%lx dur:%u %d", 0, scanType, (unsigned long)scanChannels,
                          scanDuration, (gpMacCore_StackId_t)stackId);
            gpMacCore_ScanRequest(scanType, scanChannels, scanDuration, resultListSize, pEdScanResultList,
                                  (gpMacCore_StackId_t)stackId);
        }
        else
        {
            if(!MacDispatcher_ScanRequestPackAndQueue(scanType, scanChannels, scanDuration, resultListSize, pEdScanResultList, stackId))
            {
                MacDispatcher_cbScanConfirm(gpMacCore_ResultTransactionOverflow, scanType, scanChannels, 0,
                                            pEdScanResultList, (gpMacCore_StackId_t)stackId);
            }
        }
    }
}
#endif // defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
void gpMacDispatcher_AssociateRequest(UInt8 logicalChannel, gpMacCore_AddressInfo_t* pCoordAddrInfo,
                                      UInt8 capabilityInformation, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_AssociateRequest stack %d", 0, (gpMacCore_StackId_t)stackId);
        if(gpMacDispatcher_LockClaim(stackId))
        {
            GP_LOG_PRINTF("gpMacCore_AssociateRequest",0);
            gpMacCore_AssociateRequest(logicalChannel, pCoordAddrInfo, capabilityInformation,
                                       (gpMacCore_StackId_t)stackId);
        }
        else
        {
            if(!MacDispatcher_AssociateRequestPackAndQueue(logicalChannel, pCoordAddrInfo, capabilityInformation, stackId))
            {
                MacDispatcher_cbAssociateConfirm(0xFFFF, gpMacCore_ResultTransactionOverflow, 0,
                                                 (gpMacCore_StackId_t)stackId);
            }
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
void gpMacDispatcher_AssociateResponse(MACAddress_t* pDeviceAddress, UInt16 associateShortAddress,
                                       gpMacDispatcher_Result_t status, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_AssociateResponse stack %d", 0, (gpMacCore_StackId_t)stackId);
        if(gpMacDispatcher_LockClaim(stackId))
        {
            GP_LOG_PRINTF("gpMacCore_AssociateResponse",0);
            gpMacCore_AssociateResponse(pDeviceAddress, associateShortAddress, status, (gpMacCore_StackId_t)stackId);
        }
        else
        {
            if(!MacDispatcher_AssociateResponsePackAndQueue(pDeviceAddress, associateShortAddress, status, stackId))
            {
                MacDispatcher_cbAssociateCommStatusIndication(NULL, NULL, gpMacCore_ResultTransactionOverflow, 0,
                                                              (gpMacCore_StackId_t)stackId);
            }
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
void gpMacDispatcher_OrphanResponse(MACAddress_t* pOrphanAddress, UInt16 shortAddress, Bool associatedMember,
                                    gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_OrphanResponse stack %d", 0, (gpMacCore_StackId_t)stackId);
        // Spec wise MAC layer does not trigger a callback for OrphanResponse when associatedMember is false
        if(associatedMember)
        {
            if(gpMacDispatcher_LockClaim(stackId))
            {
                GP_LOG_PRINTF("gpMacCore_OrphanResponse",0);
                gpMacCore_OrphanResponse(pOrphanAddress, shortAddress, associatedMember, (gpMacCore_StackId_t)stackId);
            }
            else
            {
                if(!MacDispatcher_OrphanResponsePackAndQueue(pOrphanAddress, shortAddress, associatedMember, stackId))
                {
                    MacDispatcher_cbOrphanCommStatusIndication(NULL, NULL, gpMacCore_ResultTransactionOverflow, 0,
                                                               (gpMacCore_StackId_t)stackId);
                }
            }
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
void gpMacDispatcher_PollRequest(gpMacCore_AddressInfo_t* pCoordAddrInfo, gpMacCore_Security_t* pSecOptions,
                                 gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_PollRequest stack %d", 0, (gpMacCore_StackId_t)stackId);
        if(gpMacDispatcher_LockClaim(stackId))
        {
            GP_LOG_PRINTF("gpMacCore_PollRequest",0);
            GP_STAT_SAMPLE_TIME();
            gpMacCore_PollRequest(pCoordAddrInfo, pSecOptions, (gpMacCore_StackId_t)stackId);
        }
        else
        {
            if(!MacDispatcher_PollRequestPackAndQueue(pCoordAddrInfo, pSecOptions, stackId))
            {
                MacDispatcher_cbPollConfirm(gpMacCore_ResultTransactionOverflow, pCoordAddrInfo, 0,
                                            (gpMacCore_StackId_t)stackId);
            }
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR

Bool gpMacDispatcher_IsValidStack(gpMacDispatcher_StackId_t stackId)
{
    return gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId);
}

// getters and setters
gpMacDispatcher_Result_t gpMacDispatcher_Start(gpMacCore_PanId_t panId, UInt8 logicalChannel, Bool panCoordinator,
                                               gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("gpMacDispatcher_Start id:%u pan:%x ch:%u coord:%u",0,stackId, panId, logicalChannel, panCoordinator);
        return (gpMacDispatcher_Result_t)gpMacCore_Start(panId, logicalChannel, panCoordinator,
                                                         (gpMacCore_StackId_t)stackId);
    }
    return gpMacDispatcher_ResultInvalidParameter;
}

void gpMacDispatcher_SetIndicateBeaconNotifications(Bool enable, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetIndicateBeaconNotifications(enable, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetIndicateBeaconNotifications(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetIndicateBeaconNotifications((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetForwardPollIndications(Bool enable, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetForwardPollIndications(enable, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetForwardPollIndications(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetForwardPollIndications((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}



#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
gpMacDispatcher_Result_t gpMacDispatcher_DataPending_QueueAdd(gpMacCore_AddressInfo_t* pAddrInfo,
                                                              gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_DataPending_QueueAdd(pAddrInfo, (gpMacCore_StackId_t)stackId);
    }
    return gpMacDispatcher_ResultInvalidParameter;
}

gpMacDispatcher_Result_t gpMacDispatcher_DataPending_QueueRemove(gpMacCore_AddressInfo_t* pAddrInfo,
                                                                 gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_DataPending_QueueRemove(pAddrInfo, (gpMacCore_StackId_t)stackId);
    }
    return gpMacDispatcher_ResultInvalidParameter;
}

gpMacDispatcher_Result_t gpMacDispatcher_DataPending_QueueClear(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_DataPending_QueueClear((gpMacCore_StackId_t)stackId);
    }
    return gpMacDispatcher_ResultInvalidParameter;
}
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION

void gpMacDispatcher_SetCurrentChannel(UInt8 channel, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetCurrentChannel(channel, (gpMacCore_StackId_t)stackId);
    }
}


UInt8 gpMacDispatcher_GetCurrentChannel(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetCurrentChannel((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetDefaultTransmitPowers(Int8* pDefaultTransmitPowerTable)
{
    gpMacCore_SetDefaultTransmitPowers(pDefaultTransmitPowerTable);
}

void gpMacDispatcher_SetTransmitPower(gpMacCore_TxPower_t transmitPower, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetTransmitPower(transmitPower, (gpMacCore_StackId_t)stackId);
    }
}

Int8 gpMacDispatcher_GetTransmitPower(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetTransmitPower((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_INT8_VALUE;
}

void gpMacDispatcher_SetCCAMode(UInt8 cCAMode, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetCCAMode(cCAMode, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetCCAMode(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetCCAMode((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
void gpMacDispatcher_SetCoordExtendedAddress(MACAddress_t* pCoordExtendedAddress, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetCoordExtendedAddress(pCoordExtendedAddress, (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_GetCoordExtendedAddress(MACAddress_t* pCoordExtendedAddress, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_GetCoordExtendedAddress(pCoordExtendedAddress, (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_SetCoordShortAddress(UInt16 shortAddress, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetCoordShortAddress(shortAddress, (gpMacCore_StackId_t)stackId);
    }
}

UInt16 gpMacDispatcher_GetCoordShortAddress(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetCoordShortAddress((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT16_VALUE;
}
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR

void gpMacDispatcher_SetPanCoordinator(Bool panCoordinator, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetPanCoordinator(panCoordinator, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetPanCoordinator(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetPanCoordinator((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetDsn(UInt8 dsn, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetDsn(dsn, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetDsn(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetDsn((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetMaxCsmaBackoffs(UInt8 maxCsmaBackoffs, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetMaxCsmaBackoffs(maxCsmaBackoffs, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetMaxCsmaBackoffs(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetMaxCsmaBackoffs((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetMinBE(UInt8 minBE, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetMinBE(minBE, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetMinBE(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetMinBE((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetMaxBE(UInt8 maxBE, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetMaxBE(maxBE, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetMaxBE(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetMaxBE((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetPanId(UInt16 panId, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetPanId(panId, (gpMacCore_StackId_t)stackId);
    }
}

UInt16 gpMacDispatcher_GetPanId(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetPanId((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT16_VALUE;
}

void gpMacDispatcher_SetRxOnWhenIdle(Bool rxOnWhenIdle, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetRxOnWhenIdle(rxOnWhenIdle, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetRxOnWhenIdle(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetRxOnWhenIdle((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetShortAddress(UInt16 shortAddress, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetShortAddress(shortAddress, (gpMacCore_StackId_t)stackId);
    }
}

UInt16 gpMacDispatcher_GetShortAddress(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetShortAddress((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT16_VALUE;
}

void gpMacDispatcher_SetAssociationPermit(Bool associationPermit, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetAssociationPermit(associationPermit, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetAssociationPermit(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetAssociationPermit((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetBeaconPayload(UInt8* pBeaconPayload, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetBeaconPayload(pBeaconPayload, (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_GetBeaconPayload(UInt8* pBeaconPayload, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_GetBeaconPayload(pBeaconPayload, (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_SetBeaconPayloadLength(UInt8 length, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetBeaconPayloadLength(length, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetBeaconPayloadLength(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetBeaconPayloadLength((gpMacCore_StackId_t)stackId);
    }
    return 0;
}

void gpMacDispatcher_EnableEnhancedFramePending(Bool enableEnhancedAck, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_EnableEnhancedFramePending(enableEnhancedAck, (gpMacCore_StackId_t)stackId);
    }
}

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
void gpMacDispatcher_SetStackInRawMode(Bool rawModeEnabled, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetStackInRawMode(rawModeEnabled, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetStackInRawMode(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetStackInRawMode((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

#if defined(GP_MACCORE_DIVERSITY_SECURITY_ENABLED) && defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
void gpMacDispatcher_SetRawModeEncryptionKeys(gpMacCore_KeyIdMode_t encryptionKeyIdMode,
                                              gpMacCore_KeyIndex_t encryptionKeyId, UInt8* pCurrKey,
                                              gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetRawModeEncryptionKeys(encryptionKeyIdMode, encryptionKeyId, pCurrKey,
                                           (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_SetRawModeNonceFields(UInt32 frameCounter, MACAddress_t* pExtendedAddress, UInt8 seclevel,
                                           gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetRawModeNonceFields(frameCounter, pExtendedAddress, seclevel, (gpMacCore_StackId_t)stackId);
    }
}
#endif // defined(GP_MACCORE_DIVERSITY_SECURITY_ENABLED) && defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)

gpMacDispatcher_Result_t gpMacDispatcher_ConfigureEnhAckProbing(UInt8 linkMetrics, MACAddress_t* pExtendedAddress,
                                                                UInt16 shortAddress, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return (gpMacDispatcher_Result_t)gpMacCore_ConfigureEnhAckProbing(linkMetrics, pExtendedAddress, shortAddress,
                                                                          (gpMacCore_StackId_t)stackId);
    }
    return gpMacDispatcher_ResultInvalidParameter;
}
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

void gpMacDispatcher_SetPromiscuousMode(UInt8 promiscuousMode, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetPromiscuousMode(promiscuousMode, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetPromiscuousMode(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetPromiscuousMode((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetTransactionPersistenceTime(UInt16 time, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetTransactionPersistenceTime(time, (gpMacCore_StackId_t)stackId);
    }
}

UInt16 gpMacDispatcher_GetTransactionPersistenceTime(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetTransactionPersistenceTime((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT16_VALUE;
}

void gpMacDispatcher_SetExtendedAddress(MACAddress_t* pExtendedAddress, gpMacDispatcher_StackId_t stackId)
{
    GP_LOG_PRINTF("gpMacDispatcher_SetExtendedAddress: stack:%x LSB:%lx MSB:%lx",0,stackId, (unsigned long)pExtendedAddress->LSB, (unsigned long)pExtendedAddress->MSB);
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetExtendedAddress(pExtendedAddress, (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_GetExtendedAddress(MACAddress_t* pExtendedAddress, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_GetExtendedAddress(pExtendedAddress, (gpMacCore_StackId_t)stackId);
    }
    GP_LOG_PRINTF("gpMacDispatcher_GetExtendedAddress: stack:%x LSB:%lx MSB:%lx",0,stackId, (unsigned long)pExtendedAddress->LSB, (unsigned long)pExtendedAddress->MSB);
}

void gpMacDispatcher_SetNumberOfRetries(UInt8 numberOfRetries, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetNumberOfRetries(numberOfRetries, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetNumberOfRetries(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetNumberOfRetries((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetCsmaMode(UInt8 csmaMode, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetCsmaMode(csmaMode, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetCsmaMode(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetCsmaMode((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetSecurityEnabled(Bool securityEnabled, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetSecurityEnabled(securityEnabled, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetSecurityEnabled(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetSecurityEnabled((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetBeaconStarted(Bool BeaconStarted, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetBeaconStarted(BeaconStarted, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetBeaconStarted(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetBeaconStarted((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetMacVersion(gpMacCore_MacVersion_t macVersion, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetMacVersion(macVersion, (gpMacCore_StackId_t)stackId);
    }
}

gpMacCore_MacVersion_t gpMacDispatcher_GetMacVersion(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetMacVersion((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

#ifdef GP_DIVERSITY_TX_ANTENNA_DIVERSITY
void gpMacDispatcher_SetTxAntenna(UInt8 txAntenna, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetTxAntenna(txAntenna, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetTxAntenna(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetTxAntenna((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}
#endif // GP_DIVERSITY_TX_ANTENNA_DIVERSITY

#ifdef GP_MACCORE_DIVERSITY_POLL_RECIPIENT
Bool gpMacDispatcher_AddNeighbour(gpMacCore_AddressInfo_t* pAddrInfo, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_AddNeighbour(pAddrInfo, (gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

Bool gpMacDispatcher_RemoveNeighbour(gpMacCore_AddressInfo_t* pAddrInfo, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_RemoveNeighbour(pAddrInfo, (gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_ClearNeighbours(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_ClearNeighbours((gpMacCore_StackId_t)stackId);
    }
}

gpMacDispatcher_Result_t gpMacDispatcher_SetDataPendingMode(gpMacCore_DataPendingMode_t dataPendingMode,
                                                            gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return (gpMacDispatcher_Result_t)gpMacCore_SetDataPendingMode(dataPendingMode, (gpMacCore_StackId_t)stackId);
    }
    return gpMacCore_ResultInvalidParameter;
}
#endif // GP_MACCORE_DIVERSITY_POLL_RECIPIENT

void gpMacDispatcher_SendDriverReset(gpMacDispatcher_StackId_t stackId)
{
    gpMacDispatcher_Callbacks_t* pCallbacks;

    GP_LOG_SYSTEM_PRINTF("SndDrvRst",0);

    GP_ASSERT_DEV_INT(stackId != GP_MAC_DISPATCHER_INVALID_STACK_ID);

    pCallbacks = MacDispatcher_GetCallbacks(stackId);
    if(pCallbacks != NULL && pCallbacks->driverResetIndicationCallback)
    {
        GP_LOG_SYSTEM_PRINTF("cb OK",0);
        pCallbacks->driverResetIndicationCallback(0, (gpMacCore_StackId_t)stackId);
    }
}

#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
void gpMacDispatcher_SetFrameCounter(UInt32 frameCounter, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetFrameCounter(frameCounter, (gpMacCore_StackId_t)stackId);
    }
}

UInt32 gpMacDispatcher_GetFrameCounter(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetFrameCounter((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT32_VALUE;
}

gpMacDispatcher_Result_t gpMacDispatcher_SetKeyDescriptor(gpMacCore_KeyDescriptor_t* pKeyDescriptor,
                                                          gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetKeyDescriptor` still executed", 0, stackId);
    }
    return (gpMacDispatcher_Result_t)gpMacCore_SetKeyDescriptor(pKeyDescriptor, index);
}

gpMacDispatcher_Result_t gpMacDispatcher_GetKeyDescriptor(gpMacCore_KeyDescriptor_t* pKeyDescriptor,
                                                          gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetKeyDescriptor` still executed", 0, stackId);
    }
    return (gpMacDispatcher_Result_t)gpMacCore_GetKeyDescriptor(pKeyDescriptor, index);
}

void gpMacDispatcher_SetKeyTableEntries(gpMacCore_KeyTablesEntries_t keyTableEntries, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetKeyTableEntries` still executed", 0, stackId);
    }
    gpMacCore_SetKeyTableEntries(keyTableEntries);
}

gpMacCore_KeyTablesEntries_t gpMacDispatcher_GetKeyTableEntries(gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetKeyTableEntries` still executed", 0, stackId);
    }
    return gpMacCore_GetKeyTableEntries();
}

gpMacDispatcher_Result_t gpMacDispatcher_SetDeviceDescriptor(gpMacCore_DeviceDescriptor_t* pDeviceDescriptor,
                                                             gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetDeviceDescriptor` still executed", 0, stackId);
    }
    return (gpMacDispatcher_Result_t)gpMacCore_SetDeviceDescriptor(pDeviceDescriptor, index);
}

gpMacDispatcher_Result_t gpMacDispatcher_GetDeviceDescriptor(gpMacCore_DeviceDescriptor_t* pDeviceDescriptor,
                                                             gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetDeviceDescriptor` still executed", 0, stackId);
    }
    return (gpMacDispatcher_Result_t)gpMacCore_GetDeviceDescriptor(pDeviceDescriptor, index);
}

void gpMacDispatcher_SetDeviceTableEntries(gpMacCore_DeviceTablesEntries_t deviceTableEntries,
                                           gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetDeviceTableEntries` still executed", 0, stackId);
    }
    gpMacCore_SetDeviceTableEntries(deviceTableEntries);
}

gpMacCore_DeviceTablesEntries_t gpMacDispatcher_GetDeviceTableEntries(gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetDeviceTableEntries` still executed", 0, stackId);
    }
    return gpMacCore_GetDeviceTableEntries();
}

gpMacDispatcher_Result_t gpMacDispatcher_SetSecurityLevelDescriptor(
    gpMacCore_SecurityLevelDescriptor_t* pSecurityLevelDescriptor, gpMacCore_Index_t index,
    gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetSecurityLevelDescriptor` still executed", 0,
                             stackId);
    }
    return (gpMacDispatcher_Result_t)gpMacCore_SetSecurityLevelDescriptor(pSecurityLevelDescriptor, index);
}

gpMacDispatcher_Result_t gpMacDispatcher_GetSecurityLevelDescriptor(
    gpMacCore_SecurityLevelDescriptor_t* pSecurityLevelDescriptor, gpMacCore_Index_t index,
    gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetSecurityLevelDescriptor` still executed", 0,
                             stackId);
    }
    return (gpMacDispatcher_Result_t)gpMacCore_GetSecurityLevelDescriptor(pSecurityLevelDescriptor, index);
}

gpMacCore_SecurityLevelTableEntries_t gpMacDispatcher_GetSecurityLevelTableEntries(gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetSecurityLevelTableEntries` still executed", 0,
                             stackId);
    }
    return (gpMacDispatcher_Result_t)gpMacCore_GetSecurityLevelTableEntries();
}

void gpMacDispatcher_SetSecurityLevelTableEntries(gpMacCore_SecurityLevelTableEntries_t securityLevelTableEntries,
                                                  gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetSecurityLevelTableEntries` still executed", 0,
                             stackId);
    }
    gpMacCore_SetSecurityLevelTableEntries(securityLevelTableEntries);
}

void gpMacDispatcher_SetDefaultKeySource(UInt8* pDefaultKeySource, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetDefaultKeySource` still executed", 0, stackId);
    }
    gpMacCore_SetDefaultKeySource(pDefaultKeySource);
}

void gpMacDispatcher_GetDefaultKeySource(UInt8* pDefaultKeySource, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetDefaultKeySource` still executed", 0, stackId);
    }
    gpMacCore_GetDefaultKeySource(pDefaultKeySource);
}

void gpMacDispatcher_SetPanCoordExtendedAddress(MACAddress_t* pPanCoordExtendedAddress,
                                                gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetPanCoordExtendedAddress` still executed", 0,
                             stackId);
    }
    gpMacCore_SetPanCoordExtendedAddress(pPanCoordExtendedAddress);
}

void gpMacDispatcher_GetPanCoordExtendedAddress(MACAddress_t* pPanCoordExtendedAddress,
                                                gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetPanCoordExtendedAddress` still executed", 0,
                             stackId);
    }
    gpMacCore_GetPanCoordExtendedAddress(pPanCoordExtendedAddress);
}

void gpMacDispatcher_SetPanCoordShortAddress(UInt16 PanCoordShortAddress, gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_SetPanCoordShortAddress` still executed", 0,
                             stackId);
    }
    gpMacCore_SetPanCoordShortAddress(PanCoordShortAddress);
}

UInt16 gpMacDispatcher_GetPanCoordShortAddress(gpMacDispatcher_StackId_t stackId)
{
    if(!gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_SYSTEM_PRINTF("WARNING: Invalid stack %u, `gpMacCore_GetPanCoordShortAddress` still executed", 0,
                             stackId);
    }
    return gpMacCore_GetPanCoordShortAddress();
}
#endif // GP_MACCORE_DIVERSITY_SECURITY_ENABLED

#ifdef GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS
void gpMacDispatcher_SetRegionalDomainSettings(UInt32 blockedChannelMask, gpMacCore_TxPower_t* pMaxTransmitPowerTable, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        GP_LOG_PRINTF("RDS for stack %u: blocked 0x%x", 2, (unsigned int)blockedChannelMask,
                      (gpMacCore_StackId_t)stackId);
        GP_LOG_PRINTF("0-7 [%u %u %u %u %u %u %u %u]", 2,
                            pMaxTransmitPowerTable[0],
                            pMaxTransmitPowerTable[1],
                            pMaxTransmitPowerTable[2],
                            pMaxTransmitPowerTable[3],
                            pMaxTransmitPowerTable[4],
                            pMaxTransmitPowerTable[5],
                            pMaxTransmitPowerTable[6],
                            pMaxTransmitPowerTable[7]);
        GP_LOG_PRINTF("8-15[%u %u %u %u %u %u %u %u]", 2,
                            pMaxTransmitPowerTable[8],
                            pMaxTransmitPowerTable[9],
                            pMaxTransmitPowerTable[10],
                            pMaxTransmitPowerTable[11],
                            pMaxTransmitPowerTable[12],
                            pMaxTransmitPowerTable[13],
                            pMaxTransmitPowerTable[14],
                            pMaxTransmitPowerTable[15]);

        gpMacCore_SetRegionalDomainSettings(blockedChannelMask, pMaxTransmitPowerTable, (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_GetRegionalDomainSettings(UInt32* blockedChannelMask, gpMacCore_TxPower_t* pMaxTransmitPowerTable, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_GetRegionalDomainSettings(blockedChannelMask, pMaxTransmitPowerTable, (gpMacCore_StackId_t)stackId);
    }
}
#endif // GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS

#ifdef GP_MACCORE_DIVERSITY_RX_WINDOWS
void gpMacDispatcher_EnableRxWindows(UInt32 dutyCycleOnTime, UInt32 dutyCyclePeriod, UInt16 recurrenceAmount, UInt32 startTime, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_EnableRxWindows(dutyCycleOnTime, dutyCyclePeriod, recurrenceAmount, startTime,
                                  (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_DisableRxWindows(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_DisableRxWindows((gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_EnableCsl(UInt16 dutyCyclePeriod, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_EnableCsl(dutyCyclePeriod, (gpMacCore_StackId_t)stackId);
    }
}

void gpMacDispatcher_UpdateCslSampleTime(UInt32 nextCslSampleTime, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_UpdateCslSampleTime(nextCslSampleTime, (gpMacCore_StackId_t)stackId);
    }
}
#endif // GP_MACCORE_DIVERSITY_RX_WINDOWS

UInt32 gpMacDispatcher_GetCurrentTimeUs(void)
{
    return gpMacCore_GetCurrentTimeUs();
}

void gpMacDispatcher_SetRetransmitOnCcaFail(Bool enable, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetRetransmitOnCcaFail(enable, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetRetransmitOnCcaFail(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetRetransmitOnCcaFail((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetRetransmitRandomBackoff(Bool enable, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetRetransmitRandomBackoff(enable, (gpMacCore_StackId_t)stackId);
    }
}

Bool gpMacDispatcher_GetRetransmitRandomBackoff(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetRetransmitRandomBackoff((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_BOOL_VALUE;
}

void gpMacDispatcher_SetMinBeRetransmit(UInt8 minBERetransmit, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetMinBeRetransmit(minBERetransmit, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetMinBeRetransmit(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetMinBeRetransmit((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

void gpMacDispatcher_SetMaxBeRetransmit(UInt8 maxBERetransmit, gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        gpMacCore_SetMaxBeRetransmit(maxBERetransmit, (gpMacCore_StackId_t)stackId);
    }
}

UInt8 gpMacDispatcher_GetMaxBeRetransmit(gpMacDispatcher_StackId_t stackId)
{
    if(gpMacCore_cbValidStack((gpMacCore_StackId_t)stackId))
    {
        return gpMacCore_GetMaxBeRetransmit((gpMacCore_StackId_t)stackId);
    }
    return MAC_DISPATCHER_DEFAULT_UINT8_VALUE;
}

Int8 gpMacDispatcher_GetRadioSensitivity(void)
{
    return gpMacCore_GetRadioSensitivity();
}

#ifdef GP_MACCORE_DIVERSITY_RX_WINDOWS
UInt8 gpMacDispatcher_GetCslAccuracy(void)
{
    return gpMacCore_GetCslAccuracy();
}

UInt8 gpMacDispatcher_GetCslUncertainty(void)
{
    return gpMacCore_GetCslUncertainty();
}
#endif // GP_MACCORE_DIVERSITY_RX_WINDOWS
