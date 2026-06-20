/*
 * Copyright (c) 2012-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpMacCore_Helpers.c
 *   This file contains several helper functions to support the MAC protocol.
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
#include "gpPd.h"

#include "hal.h"
#include "gpHal.h"
#include "gpLog.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"

#include "gpSched.h"
#include "gpAssert.h"

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

STATIC_FUNC INLINE Bool MacCore_isValidAddressMode(UInt8 addressMode) {
    return (addressMode == gpMacCore_AddressModeShortAddress || addressMode == gpMacCore_AddressModeExtendedAddress);
}

 /*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

INLINE void MacCore_ReadStreamAndUpdatePd(UInt8* bytes, UInt8 length, gpPd_Loh_t* p_PdLoh)
{
    gpPd_ReadWithUpdate(p_PdLoh, length, bytes);
#ifdef GP_BIG_ENDIAN
    /* MEMCPY is done to minimize cast align warnings */
    switch(length)
    {
        UInt16 t2;
        UInt32 t4;
        UInt64Struct_t t8;

        case 2:
        {
            t2 = *((UInt16*)bytes);
            LITTLE_TO_HOST_UINT16(&t2);
            MEMCPY(bytes, &t2, length);
            break;
        }
        case 4:
        {
            t4 = *((UInt32*)bytes);
            LITTLE_TO_HOST_UINT32(&t4);
            MEMCPY(bytes, &t4, length);
            break;
        }
        case 8:
        {
            t8 = *((UInt64Struct_t*)bytes);
            LITTLE_TO_HOST_UINT64(&t8);
            MEMCPY(bytes, &t8, length);
            break;
        }
        default:
        {
            GP_ASSERT_DEV_EXT(false);
            break;
        }
    }
#endif // GP_BIG_ENDIAN
}

INLINE void MacCore_PrependStreamAndUpdatePd(UInt8* bytes, UInt8 length, gpPd_Loh_t* p_PdLoh)
{
#ifdef GP_BIG_ENDIAN
    {
        UInt8 *nbytes=NULL;
        UInt16 t2;
        UInt32 t4;
        UInt64Struct_t t8;

        switch(length)
        {
            case 2:
            {
                t2 = *((UInt16*)bytes);
                nbytes = (UInt8*)&t2;
                HOST_TO_LITTLE_UINT16(nbytes);
                break;
            }
            case 4:
            {
                t4 = *((UInt32*)bytes);
                nbytes = (UInt8*)&t4;
                HOST_TO_LITTLE_UINT32(nbytes);
                break;
            }
            case 8:
            {
                t8 = *((UInt64Struct_t*)bytes);
                nbytes = (UInt8*)&t8;
                HOST_TO_LITTLE_UINT64(nbytes);
                break;
            }
            default:
            {
                GP_ASSERT_DEV_EXT(false);
                break;
            }
        }
        gpPd_PrependWithUpdate(p_PdLoh, length, nbytes);
    }
#else
    gpPd_PrependWithUpdate(p_PdLoh, length, bytes);
#endif // GP_BIG_ENDIAN
}

void MacCore_WriteMacHeaderInPd(gpMacCore_FrameType_t type, gpMacCore_AddressInfo_t* pSrcAddrInfo, const gpMacCore_AddressInfo_t* pDstAddrInfo, UInt8 txOptions, UInt8 securityLevel,  gpPd_Loh_t *pPdLoh , gpMacCore_StackId_t stackId)
{
    UInt16 frameControl = 0x0;
    Bool identicalPans = (pSrcAddrInfo->panId == pDstAddrInfo->panId);

    // add frame type
    MACCORE_FRAMECONTROL_FRAMETYPE_SET(frameControl,type);
    // add security + frame version
    if (securityLevel != gpEncryption_SecLevelNothing)
    {
        MACCORE_FRAMECONTROL_SECURITY_SET(frameControl,0x01);
        MACCORE_FRAMECONTROL_FRAMEVERSION_SET(frameControl,gpMacCore_MacVersion2006);
    }
    else
    {
        if((pPdLoh->length > GP_MACCORE_MAX_MAC_SAFE_PAYLOAD_SIZE) && (gpMacCore_GetMacVersion(stackId) == gpMacCore_MacVersion2006))
        {
            MACCORE_FRAMECONTROL_FRAMEVERSION_SET(frameControl,gpMacCore_MacVersion2006);
        }
    }
    // add ACK-request
    if (txOptions & GP_MACCORE_TX_OPT_ACK_REQ)
    {
        MACCORE_FRAMECONTROL_ACKREQUEST_SET(frameControl, 0x01);
    }
    // add data pending bit
    if (txOptions & GP_MACCORE_TX_OPT_MORE_DATA_PENDING)
    {
        MACCORE_FRAMECONTROL_FRAMEPENDING_SET(frameControl, 0x01);
    }
    // add source addressing mode
    MACCORE_FRAMECONTROL_SRCADDRMODE_SET(frameControl,pSrcAddrInfo->addressMode);
    // add destination addressing mode
    MACCORE_FRAMECONTROL_DSTADDRMODE_SET(frameControl,pDstAddrInfo->addressMode);

    // enable pan compression if possible
    if(MacCore_isValidAddressMode(pSrcAddrInfo->addressMode) && MacCore_isValidAddressMode(pDstAddrInfo->addressMode) && identicalPans)
    {
        MACCORE_FRAMECONTROL_PANCOMPRESSION_SET(frameControl, 0x01);
    }
    // add source address
    if(pSrcAddrInfo->addressMode == gpMacCore_AddressModeShortAddress)
    {
        MacCore_PrependStreamAndUpdatePd((UInt8*)(&pSrcAddrInfo->address.Short), GP_MACCORE_SHORT_ADDR_SIZE, pPdLoh);
    }
    else if(pSrcAddrInfo->addressMode == gpMacCore_AddressModeExtendedAddress)
    {
        MacCore_PrependStreamAndUpdatePd((UInt8*)&pSrcAddrInfo->address.Extended, GP_MACCORE_EXT_ADDR_SIZE, pPdLoh);
    }
    // add source PAN ID
    if((!MacCore_isValidAddressMode(pDstAddrInfo->addressMode) || !identicalPans) && MacCore_isValidAddressMode(pSrcAddrInfo->addressMode))
    {
        MacCore_PrependStreamAndUpdatePd((UInt8*)&pSrcAddrInfo->panId, GP_MACCORE_PANID_SIZE, pPdLoh);
    }
    // add destination address
    if (pDstAddrInfo->addressMode == gpMacCore_AddressModeShortAddress)
    {
        MacCore_PrependStreamAndUpdatePd((UInt8*)&pDstAddrInfo->address.Short, GP_MACCORE_SHORT_ADDR_SIZE, pPdLoh);
    }
    else if (pDstAddrInfo->addressMode == gpMacCore_AddressModeExtendedAddress)
    {
        MacCore_PrependStreamAndUpdatePd((UInt8*)&pDstAddrInfo->address.Extended, GP_MACCORE_EXT_ADDR_SIZE, pPdLoh);
    }
    // add destination PAN ID
    if (pDstAddrInfo->addressMode != gpMacCore_AddressModeNoAddress)
    {
        MacCore_PrependStreamAndUpdatePd((UInt8*)&pDstAddrInfo->panId, GP_MACCORE_PANID_SIZE,pPdLoh);
    }

    // add sequence number (first write it in the header, then increment it)
    {
    UInt8 dsn = gpMacCore_GetDsn(stackId);
    gpPd_PrependWithUpdate(pPdLoh, 1, &dsn);
    gpMacCore_SetDsn(dsn+1, stackId);
    }

    // add frame control
    MacCore_PrependStreamAndUpdatePd((UInt8*)&frameControl, 2,pPdLoh);
}

/*
 * in section 7.5.8.2.1 Outgoing frame security procedure:
 * h) insert the auxiliary security header into the frame
 */

void MacCore_InitSrcAddressInfo(gpMacCore_AddressInfo_t *pSrcAddressInfo, gpMacCore_AddressMode_t addrMode, gpMacCore_StackId_t stackId)
{
    if(pSrcAddressInfo == NULL)
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }

    pSrcAddressInfo->addressMode = addrMode;
    pSrcAddressInfo->panId = gpMacCore_GetPanId(stackId);
    MEMSET((UInt8*)&pSrcAddressInfo->address, 0, sizeof(pSrcAddressInfo->address));

    switch(addrMode)
    {
        case gpMacCore_AddressModeExtendedAddress:
        {
            gpMacCore_GetExtendedAddress(&pSrcAddressInfo->address.Extended, stackId);
            break;
        }
        case gpMacCore_AddressModeShortAddress:
        {
            pSrcAddressInfo->address.Short = gpMacCore_GetShortAddress(stackId);
            break;
        }
        case gpMacCore_AddressModeNoAddress:
        {
            break;
        }
        default:
        {
            GP_ASSERT_DEV_INT(false);
            break;
        }
    }
}

// Note: this implementation only works without security.
Bool MacCore_AllocatePdLoh(gpPd_Loh_t* p_PdLoh)
{
    p_PdLoh->handle = gpPd_GetPd();
    if(gpPd_CheckPdValid(p_PdLoh->handle) != gpPd_ResultValidHandle)
    {
        return false;
    }
    p_PdLoh->offset = GP_MACCORE_MAX_MAC_HEADER_SIZE;
    p_PdLoh->length = 0;

    return true;
}

/* Apply Regional Domain Settings, then invoke gpHal_DataRequest. */
gpMacCore_Result_t MacCore_TxDataRequest(gpHal_DataReqOptions_t* pDataReqOptions, gpPd_Loh_t pdLoh, gpMacCore_StackId_t stackId)
{
#ifdef GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS
    if (DIVERSITY_REGIONALDOMAINSETTINGS())
    {
        gpMacCore_Result_t result;
        gpPad_Handle_t padHandle;
        gpMacCore_TxPower_t origTxPower;

        GP_ASSERT_DEV_EXT(stackId < DIVERSITY_NR_OF_STACKS);

        padHandle   = MacCore_GetPad(stackId);
        origTxPower = gpPad_GetTxPower(padHandle);

        {
            UInt8 channels[3];
            gpMacCore_TxPower_t newTxPower;
            UIntLoop i;

            gpPad_GetTxChannels(padHandle, channels);

            // Translate symbolic TX power to actual level in dBm.
            if (origTxPower == gpMacCore_TxPowerMin)
            {
                newTxPower = GPHAL_MIN_TRANSMIT_POWER;
            }
            else if (origTxPower == gpMacCore_TxPowerMax)
            {
                newTxPower = GPHAL_MAX_TRANSMIT_POWER;
            }
            else if (origTxPower == gpMacCore_TxPowerDefault &&
                     GP_MACCORE_CHECK_CHANNEL_VALID(channels[0]))
            {
                newTxPower = gpHal_GetDefaultTransmitPower(channels[0]);
            }
            else
            {
                newTxPower = origTxPower;
            }

            // Check TX channels against blocked list and apply power limits.
            for (i = 0; i < 3; i++)
            {
                UInt8 ch = channels[i];

                if (ch != GP_MACCORE_INVALID_CHANNEL)
                {
                    if (MacCore_IsRdsChannelBlocked(ch, stackId))
                    {
                        // Attempt to transmit on blocked channel.
                        GP_LOG_PRINTF("MacCore_TxDataRequest on blocked channel %d s=%d", 0, ch, stackId);
                        return gpMacCore_ResultInvalidParameter;
                    }
                    else
                    {
                        gpMacCore_TxPower_t maxTxPower = MacCore_GetRdsMaxTransmitPower(ch, stackId);
                        if (newTxPower > maxTxPower)
                        {
                            newTxPower = maxTxPower;
                        }
                    }
                }
            }

            // Set clipped TX power as temporary value in pad.
            gpPad_SetTxPower(padHandle, newTxPower);
        }

        // Send DataRequest to gpHal with clipped TX power.
        result = gpHal_DataRequest(pDataReqOptions, padHandle, pdLoh);

        // Restore previous TX power setting.
        gpPad_SetTxPower(padHandle, origTxPower);

        return result;
    }
    else
#endif //GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS
    {
        return gpHal_DataRequest(pDataReqOptions, MacCore_GetPad(stackId), pdLoh);
    }
}

gpMacCore_Result_t MacCore_SendCommand(gpMacCore_AddressInfo_t* pDestAddrInfo, gpMacCore_AddressInfo_t* pSrcAddrInfo, UInt8 txOptions, gpMacCore_Security_t *pSecOptions, UInt8 *pData, UInt8 len , gpMacCore_StackId_t stackId, gpHal_MacScenario_t scenario)
{
    gpPd_Loh_t              pdLoh;
    gpHal_DataReqOptions_t  dataReqOptions = { 0 };
    UInt8                   channels[3];
    gpMacCore_Result_t      result;

    if(!MacCore_AllocatePdLoh(&pdLoh))
    {
        return gpMacCore_ResultTransactionOverflow;
    }

    channels[0] = gpRxArbiter_GetStackChannel(stackId);
    channels[1] = GP_HAL_MULTICHANNEL_INVALID_CHANNEL;
    channels[2] = GP_HAL_MULTICHANNEL_INVALID_CHANNEL;
    dataReqOptions.macScenario = scenario;
    dataReqOptions.srcId = stackId;
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
    dataReqOptions.rawEncryptionEnable = false;
    dataReqOptions.rawKeepFrameCounter = false;
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)

    if(!GP_MACCORE_CHECK_CHANNEL_VALID(channels[0]))
    {
        //Channel not set yet ?
        GP_LOG_SYSTEM_PRINTF("Ch inv: %x", 0, channels[0]);
        return gpMacCore_ResultInvalidParameter;
    }

    // set transmit attributes for this stack
    gpPad_SetTxChannels(MacCore_GetPad(stackId), channels);

    // Write Data
    gpPd_WriteByteStream(pdLoh.handle, pdLoh.offset, len, pData);
    pdLoh.length += len;

    // can also be optimized by converting to assert if
    if(GP_MACCORE_NO_SECURITY_SPECIFIED(pSecOptions))
    {
        // no security ==> only write normal MAC header
        MacCore_WriteMacHeaderInPd(gpMacCore_FrameTypeCommand, pSrcAddrInfo, pDestAddrInfo, txOptions, gpEncryption_SecLevelNothing, &pdLoh , stackId);
        if(pdLoh.length > GP_MACCORE_MAX_PHY_PACKET_SIZE_NO_FCS)
        {
            // If any parameter in the MCPS-DATA.request primitive is not supported or is out of range, the MAC sublayer will issue the MCPS-DATA.confirm primitive with a status of INVALID_PARAMETER.
            result = gpMacCore_ResultInvalidParameter;
        }
    }
#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
    else
    {
        result = MacCore_PrepareOutgoingSecuredPacket(pSrcAddrInfo, pDestAddrInfo, &pdLoh, txOptions, pSecOptions , gpMacCore_FrameTypeCommand, stackId);
    }
#endif // GP_MACCORE_DIVERSITY_SECURITY_ENABLED

    // send command
    result = MacCore_TxDataRequest(&dataReqOptions, pdLoh, stackId);

    if(result != gpMacCore_ResultSuccess)
    {
        gpPd_FreePd(pdLoh.handle);
    }
    else
    {
#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)
        if(DIVERSITY_POLL_ORIGINATOR() || DIVERSITY_ASSOCIATION_ORIGINATOR())
        {
            if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs && scenario == gpHal_MacPollReq)
            {
                //Store handle as tracker (used in Poll DataConfirms)
                GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->pdHandle = pdLoh.handle;
            }
        }
#endif
    }
    return result;
}

#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)
gpMacCore_Result_t MacCore_SendCommandDataRequest(gpMacCore_AddressInfo_t * pCoordAddrInfo, Bool associationTrigger, gpMacCore_Security_t *pSecOptions, gpMacCore_StackId_t stackId)
{
    gpMacCore_AddressMode_t srcAddrMode;
    gpMacCore_AddressInfo_t srcAddrInfo;
    gpMacCore_AddressInfo_t dstAddrInfo;
    UInt8 data = gpMacCore_CommandDataRequest;
    UInt16 coordShortAddr;
    MACAddress_t coordExtendedAddress;

    if(associationTrigger)
    {
        srcAddrMode = gpMacCore_AddressModeExtendedAddress;
        dstAddrInfo.panId = pCoordAddrInfo->panId;
        if(GP_MACCORE_IS_SHORT_ADDR_UNALLOCATED(gpMacCore_GetCoordShortAddress(stackId)))
        {
            dstAddrInfo.addressMode = gpMacCore_AddressModeExtendedAddress;
            gpMacCore_GetCoordExtendedAddress(&(dstAddrInfo.address.Extended), stackId);
        }
        else
        {
            dstAddrInfo.addressMode = gpMacCore_AddressModeShortAddress;
            dstAddrInfo.address.Short = gpMacCore_GetCoordShortAddress(stackId);
        }
    }
    else
    {
        UInt16 shortAddress = gpMacCore_GetShortAddress(stackId);
        dstAddrInfo = *pCoordAddrInfo;

        if(GP_MACCORE_IS_SHORT_ADDR_BROADCAST(shortAddress) || GP_MACCORE_IS_SHORT_ADDR_UNALLOCATED(shortAddress))
        {
            srcAddrMode = gpMacCore_AddressModeExtendedAddress;
        }
        else
        {
            srcAddrMode = gpMacCore_AddressModeShortAddress;
        }
    }

    MacCore_InitSrcAddressInfo(&srcAddrInfo, srcAddrMode, stackId);

    // set coordinator address in HW - so that the receiver stops after receiving a data frame from the coordinator
    if(pCoordAddrInfo->addressMode == gpMacCore_AddressModeShortAddress)
    {
        coordShortAddr = pCoordAddrInfo->address.Short;
        gpMacCore_GetCoordExtendedAddress(&coordExtendedAddress, stackId);
    }
    else
    {
        coordShortAddr = gpMacCore_GetCoordShortAddress(stackId);
        coordExtendedAddress = pCoordAddrInfo->address.Extended;
    }
    gpHal_SetCoordExtendedAddress(&coordExtendedAddress);
    gpHal_SetCoordShortAddress(coordShortAddr);

    return MacCore_SendCommand(&dstAddrInfo, &srcAddrInfo, GP_MACCORE_TX_OPT_ACK_REQ, pSecOptions, &data, GP_MACCORE_DATA_REQUEST_CMD_LEN, stackId, gpHal_MacPollReq);
}
#endif //defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)


void MacCore_StopRunningRequests(gpMacCore_StackId_t stackId)
{
#if defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)  || \
    defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)      || \
    defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pScanState && (stackId == GP_MACCORE_GET_GLOBALS()->gpMacCore_pScanState->stackId))
    {
#ifdef GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR
        gpSched_UnscheduleEvent(MacCore_DoActiveScan);
#endif //GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
        gpSched_UnscheduleEvent(MacCore_DoOrphanScan);
#endif //GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
        gpSched_UnscheduleEvent(MacCore_ScanRxOffWindow);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
        gpPoolMem_Free(GP_MACCORE_GET_GLOBALS()->gpMacCore_pScanState);
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pScanState = NULL;
    }
#endif

#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
    MacCore_AssocRespStopRequests(stackId);
#endif //GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs && stackId == GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId)
    {
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
        gpSched_UnscheduleEvent(MacCore_AssociateSendCommandDataRequest);
        gpSched_UnscheduleEvent(MacCore_AssociateTimeout);
#endif //GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
        gpSched_UnscheduleEvent(MacCore_DelayedPollConfirm);
        gpPoolMem_Free(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs);
        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs = NULL;
    }
#endif //GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
}

#if (GP_DIVERSITY_NR_OF_STACKS > 1) || (defined(GP_DIVERSITY_JUMPTABLES))
gpMacCore_StackId_t MacCore_GetStackId(gpMacCore_AddressInfo_t *pAddressInfo)
{
    gpMacCore_StackId_t id;

    if ((pAddressInfo->addressMode == gpMacCore_AddressModeNoAddress)
        ||(pAddressInfo->panId == GP_MACCORE_PANID_BROADCAST))
    {
        return GP_MACCORE_STACK_UNDEFINED;
    }

    for(id=0; id < DIVERSITY_NR_OF_STACKS; id++)
    {
        if (pAddressInfo->panId == gpMacCore_GetPanId(id))
        {
            if(pAddressInfo->addressMode == gpMacCore_AddressModeShortAddress)
            {
                if(pAddressInfo->address.Short == GP_MACCORE_SHORT_ADDR_BROADCAST  ||
                    pAddressInfo->address.Short == gpMacCore_GetShortAddress(id))
                {
                    return id;
                }
            }
            else if(pAddressInfo->addressMode == gpMacCore_AddressModeExtendedAddress)
            {
                MACAddress_t extendedAddress;
                gpMacCore_GetExtendedAddress(&extendedAddress, id);
                if(   pAddressInfo->address.Extended.LSB == extendedAddress.LSB
                    && pAddressInfo->address.Extended.MSB == extendedAddress.MSB)
                {
                    return id;
                }
            }
        }
    }
    return GP_MACCORE_STACK_UNDEFINED;
}
#endif // (GP_DIVERSITY_NR_OF_STACKS > 1) || (defined(GP_DIVERSITY_JUMPTABLES))

// debug functions
#ifdef GP_DIVERSITY_DEVELOPMENT

void dumpPdLoh(gpPd_Loh_t pdLoh)
{
    GP_LOG_SYSTEM_PRINTF("LOH: %x %x %x",0,(UInt16)pdLoh.length, (UInt16)pdLoh.offset, (UInt16)pdLoh.handle);
}

void dumpPdContent(gpPd_Loh_t pdLoh)
{
    UInt16 i;
    dumpPdLoh(pdLoh);
    for(i = pdLoh.offset; i < pdLoh.offset + pdLoh.length; i+=6) {
        UInt8 bytes[6];
        gpPd_ReadByteStream(pdLoh.handle, i, 6, bytes);
        GP_LOG_SYSTEM_PRINTF("pd[%i] = %x %x %x %x %x %x",0,i,bytes[0],bytes[1],bytes[2],bytes[3],bytes[4], bytes[5]);
        gpLog_Flush();
    }

}

void dumpSecOptions(gpMacCore_Security_t* pSecOptions)
{
    GP_LOG_SYSTEM_PRINTF("Level: %x, mode: %x, index: %x",0,pSecOptions->securityLevel, pSecOptions->keyIdMode, pSecOptions->keyIndex);
}

void dumpAddressInfo(gpMacCore_AddressInfo_t* pAddrInfo)
{
    GP_LOG_SYSTEM_PRINTF("AddressMode: %x", 0, pAddrInfo->addressMode);

    if(pAddrInfo->addressMode == gpMacCore_AddressModeShortAddress)
    {
        GP_LOG_SYSTEM_PRINTF("Short address: %x", 0, pAddrInfo->address.Short);
    }
    else if(pAddrInfo->addressMode == gpMacCore_AddressModeExtendedAddress)
    {
        GP_LOG_SYSTEM_PRINTF("Extended address: %x %x", 0, (unsigned int)pAddrInfo->address.Extended.MSB, (unsigned int)pAddrInfo->address.Extended.LSB);
    }
    GP_LOG_SYSTEM_PRINTF("Pan ID: %x", 0, pAddrInfo->panId);
}

void dumpKeyDescriptor(gpMacCore_KeyDescriptor_t* keyDesc)
{
    UIntLoop i;

    if(keyDesc == NULL)
    {
        return;
    }
    GP_LOG_SYSTEM_PRINTF("Lkup: %i",0,keyDesc->keyIdLookupListEntries);
    for(i=0; i<GP_MACCORE_NUMBER_OF_KEY_ID_LOOKUP_DESCRIPTORS; i++)
    {
#define pData keyDesc->keyIdLookupDescriptors[i].lookupData

        GP_LOG_SYSTEM_PRINTF("lkup%i: s:%i",0, i, keyDesc->keyIdLookupDescriptors[i].lookupDataSize);
        GP_LOG_SYSTEM_PRINTF("%x %x %x %x %x", 0,(UInt16)((UInt16)pData[0] << 8)+(UInt16)pData[1],
                                                          (UInt16)((UInt16)pData[2] << 8)+(UInt16)pData[3],
                                                          (UInt16)((UInt16)pData[4] << 8)+(UInt16)pData[5],
                                                          (UInt16)((UInt16)pData[6] << 8)+(UInt16)pData[7], pData[8]);
#undef pData
        gpLog_Flush();
    }
    GP_LOG_SYSTEM_PRINTF("DevDesc: %i",0,keyDesc->keyDeviceListEntries);
    for(i = 0; i < GP_MACCORE_NUMBER_OF_KEY_DEVICE_DESCRIPTORS; i++)
    {
        GP_LOG_SYSTEM_PRINTF("DevDesc%i: blck:%i uniq:%i",0, i, keyDesc->keyDeviceDescriptorList[i].blackListed,
                                                             keyDesc->keyDeviceDescriptorList[i].uniqueDevice);
        //keyDesc->keyDeviceDescriptorList[i].deviceDescriptor;
        gpLog_Flush();
    }
    GP_LOG_SYSTEM_PRINTF("KeyUsage: %i",0,keyDesc->keyUsageListEntries);
    for(i = 0; i < GP_MACCORE_NUMBER_OF_KEY_USAGE_DESCRIPTORS; i++)
    {
        GP_LOG_SYSTEM_PRINTF("KeyUs%i: fId:%x ft:%x",0,i, keyDesc->keyUsageDescriptorList[i].commandFrameIdentifier,
                                                       keyDesc->keyUsageDescriptorList[i].frameType);
        gpLog_Flush();
    }
#define pData keyDesc->key
    GP_LOG_SYSTEM_PRINTF("key: %x %x %x %x %x %x %x %x", 0,(UInt16)((UInt16)pData[0] << 8)+(UInt16)pData[1],
                                                      (UInt16)((UInt16)pData[2] << 8)+(UInt16)pData[3],
                                                      (UInt16)((UInt16)pData[4] << 8)+(UInt16)pData[5],
                                                      (UInt16)((UInt16)pData[6] << 8)+(UInt16)pData[7],
                                                      (UInt16)((UInt16)pData[8] << 8)+(UInt16)pData[9],
                                                      (UInt16)((UInt16)pData[10] << 8)+(UInt16)pData[11],
                                                      (UInt16)((UInt16)pData[12] << 8)+(UInt16)pData[13],
                                                      (UInt16)((UInt16)pData[14] << 8)+(UInt16)pData[15]);
#undef pData
}

void dumpCCMOptions(gpEncryption_CCMOptions_t* opt)
{
    GP_LOG_SYSTEM_PRINTF("H:%i MIC:%i", 0, opt->pdHandle, opt->micLength);
    GP_LOG_SYSTEM_PRINTF("datO:%x datL:%x", 0, opt->dataOffset, opt->dataLength);
    GP_LOG_SYSTEM_PRINTF("auxO:%x auxL:%x", 0, opt->auxOffset, opt->auxLength);

#define pData opt->pKey
    GP_LOG_SYSTEM_PRINTF("key: %x %x %x %x %x %x %x %x", 0,(UInt16)((UInt16)pData[0] << 8)+(UInt16)pData[1],
                                                          (UInt16)((UInt16)pData[2] << 8)+(UInt16)pData[3],
                                                          (UInt16)((UInt16)pData[4] << 8)+(UInt16)pData[5],
                                                          (UInt16)((UInt16)pData[6] << 8)+(UInt16)pData[7],
                                                          (UInt16)((UInt16)pData[8] << 8)+(UInt16)pData[9],
                                                          (UInt16)((UInt16)pData[10] << 8)+(UInt16)pData[11],
                                                          (UInt16)((UInt16)pData[12] << 8)+(UInt16)pData[13],
                                                          (UInt16)((UInt16)pData[14] << 8)+(UInt16)pData[15]);
#undef pData
#define pData opt->pNonce
    GP_LOG_SYSTEM_PRINTF("non: %x %x %x %x %x %x %x", 0,  (UInt16)((UInt16)pData[0] << 8)+(UInt16)pData[1],
                                                          (UInt16)((UInt16)pData[2] << 8)+(UInt16)pData[3],
                                                          (UInt16)((UInt16)pData[4] << 8)+(UInt16)pData[5],
                                                          (UInt16)((UInt16)pData[6] << 8)+(UInt16)pData[7],
                                                          (UInt16)((UInt16)pData[8] << 8)+(UInt16)pData[9],
                                                          (UInt16)((UInt16)pData[10] << 8)+(UInt16)pData[11],
                                                          (UInt16)pData[12]);
#undef pData
    gpLog_Flush();
}
#endif // GP_DIVERSITY_DEVELOPMENT
