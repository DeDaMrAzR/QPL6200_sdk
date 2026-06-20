/*
 * Copyright (c) 2012-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpMacCore.c
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
// #define GP_LOCAL_LOG
// General includes
#include "hal.h"
#include "gpHal.h"
#include "gpPd.h"
#include "gpLog.h"
#include "gpAssert.h"
#include "gpPoolMem.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"
#include "gpMacCore_enumcheck.h"

#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT) ||     \
    defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
#include "gpSched.h"
#endif

#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "gpHal_DEFS.h"
#endif // XP4002

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE

#define SHORT_ADDR_SIZE    sizeof(UInt16)
#define EXTENDED_ADDR_SIZE sizeof(MACAddress_t)

#define GP_MACCORE_BEACONORDER_MASK                0xF
#define GP_MACCORE_BEACONORDER_NO_PERIODIC_BEACONS 0xF
#define GP_MACCORE_GTS_PERMIT                      0x80

// #define MAX_PENDING_ADDRESSES   7
#define MAC_TRANSMISSION_START_CB()                                                                                    \
    {                                                                                                                  \
        if(GP_MACCORE_GET_GLOBALS()->MacCore_TransmissionStartCb)                                                      \
        {                                                                                                              \
            GP_MACCORE_GET_GLOBALS()->MacCore_TransmissionStartCb();                                                   \
        }                                                                                                              \
    }

#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
#if !defined(GP_DIVERSITY_GPHAL_XP4002)
#define GP_MACCORE_ALLOCATE_TIMED_EVENT gpHal_GetAbsoluteEvent
#define GP_MACCORE_INVALID_TIMED_EVENT  GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID
#else
#define GP_MACCORE_ALLOCATE_TIMED_EVENT gpHal_RtEventTimedTxAllocate
#define GP_MACCORE_INVALID_TIMED_EVENT  RT_EVENT_HANDLE_INVALID
#endif // !XP4002
#endif // GP_MACCORE_DIVERSITY_TIMEDTX

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

//----------------
// Helper functions
//----------------

static gpMacCore_Result_t MacCore_Hal2MacResult(gpHal_Result_t hal_result)
{
    // This switch is the same switch as in gpHal_ISR->gpHal_DataConfirmInterrupt
    // It should contain the only possible values for gpHal_Result_t
    gpMacCore_Result_t result = gpMacCore_ResultFailed;
    switch(hal_result)
    {
        case gpHal_ResultSuccess:
            result = gpMacCore_ResultSuccess;
            break;
        case gpHal_ResultGrantTimeout:
        case gpHal_ResultCCAFailure:
            result = gpMacCore_ResultChannelAccessFailure;
            break;
        case gpHal_ResultNoAck:
            result = gpMacCore_ResultNoAck;
            break;
        default:
            GP_ASSERT_DEV_INT(false);
    }
    return result;
}

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
gpMacCore_Result_t MacCore_SendCommandCoordinatorRealignment(MACAddress_t* pDestAddr, UInt16 orphanShortAddress,
                                                             Bool broadCast, gpMacCore_StackId_t stackId)
{
    UInt16 coordShortAddress;
    gpMacCore_AddressInfo_t dstAddressInfo;
    gpMacCore_AddressInfo_t srcAddrInfo;
    UInt8 payload[GP_MACCORE_COORDINATOR_REALIGNMENT_CMD_LEN];

    MacCore_InitSrcAddressInfo(&srcAddrInfo, gpMacCore_AddressModeExtendedAddress, stackId);
    coordShortAddress = gpMacCore_GetShortAddress(stackId);
    dstAddressInfo.panId = GP_MACCORE_PANID_BROADCAST;

    if(broadCast)
    {
        dstAddressInfo.addressMode = gpMacCore_AddressModeShortAddress;
        dstAddressInfo.address.Short = GP_MACCORE_SHORT_ADDR_BROADCAST;
    }
    else
    {
        dstAddressInfo.addressMode = gpMacCore_AddressModeExtendedAddress;
        MEMCPY(&dstAddressInfo.address, pDestAddr, sizeof(MACAddress_t));
    }
    // construct the payload
    payload[0] = gpMacCore_CommandCoordinatorRealignment;
    HOST_TO_LITTLE_UINT16(&srcAddrInfo.panId);
    MEMCPY(&payload[1], &srcAddrInfo.panId, GP_MACCORE_PANID_SIZE);
    HOST_TO_LITTLE_UINT16(&coordShortAddress);
    MEMCPY(&payload[3], &coordShortAddress, GP_MACCORE_SHORT_ADDR_SIZE);
    payload[5] = gpRxArbiter_GetStackChannel(stackId);
    HOST_TO_LITTLE_UINT16(&orphanShortAddress);
    MEMCPY(&payload[6], &orphanShortAddress, GP_MACCORE_SHORT_ADDR_SIZE);

    // send the command
    return MacCore_SendCommand(&dstAddressInfo, &srcAddrInfo, GP_MACCORE_TX_OPT_ACK_REQ, NULL, payload,
                               GP_MACCORE_COORDINATOR_REALIGNMENT_CMD_LEN, stackId, gpHal_MacDefault);
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT

// check that address modes are valid
static gpMacCore_Result_t MacCore_ValidAddrModes(gpMacCore_AddressMode_t srcAddrMode,
                                                 gpMacCore_AddressMode_t dstAddrMode, gpMacCore_FrameType_t frameType)
{
    // Address modes None+None is allowed for Ack frames.
    if((srcAddrMode == gpMacCore_AddressModeNoAddress) && (dstAddrMode == gpMacCore_AddressModeNoAddress) &&
       (frameType != gpMacCore_FrameTypeAcknowledge))
    {
        return gpMacCore_ResultInvalidAddress;
    }

    if((srcAddrMode == gpMacCore_AddressModeReserved || srcAddrMode > gpMacCore_AddressModeExtendedAddress) ||
       (dstAddrMode == gpMacCore_AddressModeReserved || dstAddrMode > gpMacCore_AddressModeExtendedAddress))
    {
        return gpMacCore_ResultInvalidParameter;
    }

    return gpMacCore_ResultSuccess;
}

#ifndef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
STATIC_FUNC gpMacCore_Result_t MacCore_ValidateClearText(MacCore_HeaderDescriptor_t* dataIndicationMacValues)
{
    UInt8 secLevel;

    if(!MACCORE_FRAMECONTROL_SECURITY_GET(dataIndicationMacValues->frameControl))
    {
        secLevel = gpEncryption_SecLevelNothing;
    }
    else
    {
        if(!MACCORE_FRAMECONTROL_FRAMEVERSION_GET(dataIndicationMacValues->frameControl))
        {
            // In section 7.5.8.2.3 Incoming frame security procedure b) security + old frame version ==> legacy
            return gpMacCore_ResultUnsupportedLegacy;
        }
        secLevel = dataIndicationMacValues->secOptions.securityLevel;
    }
    // In section 7.5.8.2.3 Incoming frame security procedure d) return success or unsupported security, depending on
    // the MAC's security setting
    return (secLevel == gpEncryption_SecLevelNothing) ? gpMacCore_ResultSuccess : gpMacCore_ResultUnsupportedSecurity;
}
#endif //! GP_MACCORE_DIVERSITY_SECURITY_ENABLED

/*
 * This function will read all data in the MAC header of the packet and write it in the pMacHeaderDecoded structure.
 * After this function has been executed, the p_PdLoh points directly after the MAC header.
 * So make sure to call this function only once on each incoming packet!
 *
 * This functions returns the length of the MAC header or 0xFF in case of an invalid header has been detected.
 */
STATIC_FUNC UInt8 MacCore_AnalyseMacHeader(gpPd_Loh_t* p_PdLoh, MacCore_HeaderDescriptor_t* pMacHeaderDecoded)
{
    Bool dstPanIdPresent, srcPanIdPresent;

    UInt8 startMacHdr = p_PdLoh->offset;
    UInt8 packetLength = p_PdLoh->length;

    // read frame control
    MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->frameControl, 2, p_PdLoh);
    // read sequence number
    gpPd_ReadWithUpdate(p_PdLoh, 1, &pMacHeaderDecoded->sequenceNumber);
    // get frametype
    pMacHeaderDecoded->frameType =
        (gpMacCore_FrameType_t)MACCORE_FRAMECONTROL_FRAMETYPE_GET(pMacHeaderDecoded->frameControl);
    // get destination address mode
    pMacHeaderDecoded->dstAddrInfo.addressMode =
        (gpMacCore_AddressMode_t)MACCORE_FRAMECONTROL_DSTADDRMODE_GET(pMacHeaderDecoded->frameControl);
    // get source addressing mode
    pMacHeaderDecoded->srcAddrInfo.addressMode =
        (gpMacCore_AddressMode_t)MACCORE_FRAMECONTROL_SRCADDRMODE_GET(pMacHeaderDecoded->frameControl);

    if(MacCore_ValidAddrModes(pMacHeaderDecoded->srcAddrInfo.addressMode, pMacHeaderDecoded->dstAddrInfo.addressMode,
                              pMacHeaderDecoded->frameType) != gpMacCore_ResultSuccess)
    {
        GP_LOG_PRINTF("Invalid header seen", 0);
        return GP_MACCORE_INVALID_HEADER_LENGTH;
    }

    if(MACCORE_FRAMECONTROL_FRAMEVERSION_GET(pMacHeaderDecoded->frameControl) < gpMacCore_MacVersion2015)
    {
        dstPanIdPresent = (pMacHeaderDecoded->dstAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress);
        srcPanIdPresent = (pMacHeaderDecoded->srcAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress);
        if((pMacHeaderDecoded->dstAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress) &&
           (pMacHeaderDecoded->srcAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress))
        {
            srcPanIdPresent = !(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(pMacHeaderDecoded->frameControl));
        }
    }
    else
    {
        dstPanIdPresent = (pMacHeaderDecoded->dstAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress);
        srcPanIdPresent = (pMacHeaderDecoded->srcAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress);
        if((pMacHeaderDecoded->dstAddrInfo.addressMode == gpMacCore_AddressModeNoAddress) &&
           (pMacHeaderDecoded->srcAddrInfo.addressMode == gpMacCore_AddressModeNoAddress))
        {
            dstPanIdPresent = (MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(pMacHeaderDecoded->frameControl));
        }
        if((pMacHeaderDecoded->dstAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress) &&
           (pMacHeaderDecoded->srcAddrInfo.addressMode == gpMacCore_AddressModeNoAddress))
        {
            dstPanIdPresent = !(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(pMacHeaderDecoded->frameControl));
        }
        if((pMacHeaderDecoded->dstAddrInfo.addressMode == gpMacCore_AddressModeNoAddress) &&
           (pMacHeaderDecoded->srcAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress))
        {
            srcPanIdPresent = !(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(pMacHeaderDecoded->frameControl));
        }
        if((pMacHeaderDecoded->dstAddrInfo.addressMode == gpMacCore_AddressModeExtendedAddress) &&
           (pMacHeaderDecoded->srcAddrInfo.addressMode == gpMacCore_AddressModeExtendedAddress))
        {
            dstPanIdPresent = !(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(pMacHeaderDecoded->frameControl));
            srcPanIdPresent = false;
        }
        else
        {
            srcPanIdPresent = !(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(pMacHeaderDecoded->frameControl));
        }
    }

    if(pMacHeaderDecoded->dstAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress)
    {
        if(dstPanIdPresent)
        {
            // Get PAN ID
            MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->dstAddrInfo.panId, 2, p_PdLoh);
        }
        // Get address
        if(pMacHeaderDecoded->dstAddrInfo.addressMode == gpMacCore_AddressModeExtendedAddress)
        {
            // Get extended address
            MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->dstAddrInfo.address.Extended, EXTENDED_ADDR_SIZE,
                                          p_PdLoh);
        }
        else
        {
            // Get short address
            MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->dstAddrInfo.address.Short, SHORT_ADDR_SIZE,
                                          p_PdLoh);
        }
    }
    if(pMacHeaderDecoded->srcAddrInfo.addressMode >= gpMacCore_AddressModeShortAddress)
    {
        // Get PAN ID
        if(srcPanIdPresent)
        {
            MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->srcAddrInfo.panId, 2, p_PdLoh);
        }
        else
        {
            pMacHeaderDecoded->srcAddrInfo.panId = pMacHeaderDecoded->dstAddrInfo.panId;
        }

        // Get address
        if(pMacHeaderDecoded->srcAddrInfo.addressMode == gpMacCore_AddressModeExtendedAddress)
        {
            // Get extended address
            MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->srcAddrInfo.address.Extended, EXTENDED_ADDR_SIZE,
                                          p_PdLoh);
        }
        else
        {
            // Get short address
            MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->srcAddrInfo.address.Short, SHORT_ADDR_SIZE,
                                          p_PdLoh);
        }
    }

    // Get security properties
    if(MACCORE_FRAMECONTROL_SECURITY_GET(pMacHeaderDecoded->frameControl))
    {
        // get security control
        UInt8 secControl;

        gpPd_ReadWithUpdate(p_PdLoh, 1, &secControl);
        pMacHeaderDecoded->secOptions.securityLevel = MACCORE_SECCONTROL_SECLEVEL_GET(secControl);
        pMacHeaderDecoded->secOptions.keyIdMode = MACCORE_SECCONTROL_KEYIDMODE_GET(secControl);
        // get framecounter
        MacCore_ReadStreamAndUpdatePd((UInt8*)&pMacHeaderDecoded->frameCounter, 4, p_PdLoh);
        switch(pMacHeaderDecoded->secOptions.keyIdMode)
        {
            case gpMacCore_KeyIdModeExplicit1Octet:
            {
                break;
            }
            case gpMacCore_KeyIdModeExplicit4Octet:
            {
                // get key source
                MacCore_ReadStreamAndUpdatePd(pMacHeaderDecoded->secOptions.pKeySource.pKeySource4, 4, p_PdLoh);
                break;
            }
            case gpMacCore_KeyIdModeExplicit8Octet:
            {
                // get key source
                MacCore_ReadStreamAndUpdatePd(pMacHeaderDecoded->secOptions.pKeySource.pKeySource8, 8, p_PdLoh);
                break;
            }
            case gpMacCore_KeyIdModeImplicit:
            {
                // do nothing
                break;
            }
            default:
            {
                GP_LOG_PRINTF("Invalid header keyIdMode: %x", 0, pMacHeaderDecoded->secOptions.keyIdMode);
                return GP_MACCORE_INVALID_HEADER_LENGTH;
            }
        }
        if(pMacHeaderDecoded->secOptions.keyIdMode != gpMacCore_KeyIdModeImplicit)
        {
            // get key index
            gpPd_ReadWithUpdate(p_PdLoh, 1, &pMacHeaderDecoded->secOptions.keyIndex);
        }
    }
    else
    {
        pMacHeaderDecoded->secOptions.keyIndex = 0;
    }
    if(packetLength < p_PdLoh->offset - startMacHdr)
    {
        GP_LOG_PRINTF("Invalid header length: %i < %i-%i", 0, packetLength, p_PdLoh->offset, startMacHdr);
        return GP_MACCORE_INVALID_HEADER_LENGTH;
    }
    return p_PdLoh->offset - startMacHdr;
}

#if defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)
// Note: in case the number of addresses does not match the actual number present in the beacon, we should drop this
// beacon...
STATIC_FUNC void MacCore_ProcessBeacon(gpPd_Loh_t pdLoh, MacCore_HeaderDescriptor_t* pMdi, gpHal_RxInfo_t* rxInfo)
{
    gpMacCore_PanDescriptor_t panDescriptor;
    UInt8 pendAddrSpec, gtsFields;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
    if(DIVERSITY_RAW_FRAMES())
    {
        if((maccore_globals->gpMacCore_pScanState == NULL) && !MacCore_IndicateBeaconNotificationsNeeded())
        {
            /* we received a beacon frame, but we were not scanning */
            /* but the OpenThread stack might be scanning */
            /* so, ignore the frame */
            GP_LOG_SYSTEM_PRINTF("Beacon while no scan", 0);
            return;
        }
    }
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

    panDescriptor.linkQuality = gpPd_GetLqi(pdLoh.handle);
    panDescriptor.timeStamp = gpPd_GetRxTimestamp(pdLoh.handle);

    MEMCPY(&panDescriptor.coordAddressInfo, &pMdi->srcAddrInfo, sizeof(gpMacCore_AddressInfo_t));
    panDescriptor.logicalChannel = rxInfo->rxChannel;
    MacCore_ReadStreamAndUpdatePd((UInt8*)&panDescriptor.superFrame, 2, &pdLoh);

    // offset points at GTS field
    gpPd_ReadWithUpdate(&pdLoh, 1, &gtsFields);
    gpPd_ReadWithUpdate(&pdLoh, 1, &pendAddrSpec);
    if(gtsFields != 0 || pendAddrSpec != 0)
    {
        // only analyze a beacon if gts and pending address are both 0.
        // we van't handle this stuff.
        // we do indicate the beacon to the upper layer as we still use it for pan id conflict.

        // make an exception for GTSPermit + BO=15:
        // Note that this is a interoperability fix. According to the spec, this should not occur.
        if((pendAddrSpec == 0) && (gtsFields == GP_MACCORE_GTS_PERMIT) &&
           ((panDescriptor.superFrame & GP_MACCORE_BEACONORDER_MASK) == GP_MACCORE_BEACONORDER_NO_PERIODIC_BEACONS))
        {
            // this might still be acceptable, since the beacon order indicates that it's not a beacon-enabled pan
            // Note also that if the gtsFields is higher then 0x80 (because it contains a GTS list), then
            // we still drop the content.
        }
        else
        {
            // don't analyze the content of the beacon.
            pdLoh.length = 0;
        }
    }

    if(pdLoh.length <= GP_MACCORE_MAX_BEACON_PAYLOAD_LENGTH)
    {
        gpMacCore_StackId_t stackId;
        UInt8* pBeaconPayload = NULL;

        // pending address related fields are not used
        // sduLength and sdu(data) itself is part of the pdLoh, the offset points to Beacon payload (if present)
        for(stackId = 0; stackId < DIVERSITY_NR_OF_STACKS; stackId++)
        {
            if(gpMacCore_cbValidStack(stackId))
            {
                if(gpMacCore_GetIndicateBeaconNotifications(stackId) ||
                   (maccore_globals->gpMacCore_pScanState != NULL &&
                    maccore_globals->gpMacCore_pScanState->stackId == stackId))
                {
                    // Enhanced to compare all the slot channels
                    if((panDescriptor.logicalChannel != GP_RX_ARBITER_INVALID_CHANNEL) &&
                       (gpRxArbiter_IsAnActiveChannel(stackId, panDescriptor.logicalChannel)))
                    {
                        if(!pBeaconPayload && pdLoh.length)
                        {
                            pBeaconPayload = GP_POOLMEM_MALLOC(pdLoh.length);
                            if(pBeaconPayload)
                            {
                                gpPd_ReadByteStream(pdLoh.handle, pdLoh.offset, pdLoh.length, pBeaconPayload);
                            }
                        }
                        if(maccore_globals->gpMacCore_pScanState != NULL &&
                           maccore_globals->gpMacCore_pScanState->stackId == stackId)
                        {
                            // For active scan, we can use this variable to count the beacons
                            // We only have to count them, no need to store in a list
                            maccore_globals->gpMacCore_pScanState->resultListSize++;
                        }
                        gpMacCore_cbBeaconNotifyIndication(pMdi->sequenceNumber, &panDescriptor, stackId,
                                                           pBeaconPayload ? pdLoh.length : 0, pBeaconPayload);
                    }
                    else
                    {
                    }
                }
            }
        }
        if(pBeaconPayload)
        {
            gpPoolMem_Free(pBeaconPayload);
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT
// format: superframe spec (2) | GTS fields (1) | Pending address fields (1) | Beacon payload (variable)
STATIC_FUNC void MacCore_ConstructBeacon(gpPd_Loh_t* p_PdLoh, UInt8 stackId)
{
    UInt16 superframeSpecification;
    UInt8 beaconLength;
    UInt8 offset = p_PdLoh->offset;

    // construct the superframe specification field
    // beacon order 0xf =  non-beacon-enabled personal area network
    // super frame order = 0xf -> has no meaning if beacon order is 0xf
    // final cap slot = 0xf -> has no meaning if beacon order is 0xf
    // battery life extension is not supported -> 0
    superframeSpecification = 0x0fff;
    MACCORE_SFS_PAN_COORDINATOR_SET(superframeSpecification, gpMacCore_GetPanCoordinator(stackId));
    MACCORE_SFS_ASSOCIATION_PERMIT_SET(superframeSpecification, gpMacCore_GetAssociationPermit(stackId));
    HOST_TO_LITTLE_UINT16(&superframeSpecification);
    gpPd_WriteByteStream(p_PdLoh->handle, offset, 2, (UInt8*)&superframeSpecification);
    p_PdLoh->length += 2;
    offset += 2;
    // add GTS fields
    gpPd_WriteByte(p_PdLoh->handle, offset, GP_MACCORE_GTS_FIELDS);
    p_PdLoh->length++;
    offset++;
    // add pending address fields
    gpPd_WriteByte(p_PdLoh->handle, offset, GP_MACCORE_PENDING_ADDRESS_FIELDS);
    p_PdLoh->length++;
    offset++;
    // add beacon payload
    beaconLength = gpMacCore_GetBeaconPayloadLength(stackId);
    if(beaconLength)
    {
        UInt8 beaconPayload[GP_MACCORE_MAX_BEACON_PAYLOAD_LENGTH];
        gpMacCore_GetBeaconPayload(beaconPayload, stackId);
        // beacon payload should already be endianess aware !!
        gpPd_WriteByteStream(p_PdLoh->handle, offset, beaconLength, beaconPayload);
        p_PdLoh->length += beaconLength;
    }
}

STATIC_FUNC void MacCore_TransmitBeacon(gpPd_Loh_t* p_PdLoh, gpHal_RxInfo_t* rxInfo)
{
    gpPd_Loh_t local_pdloh;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    // this code is executed in response to both a beacon request
    // and a beacon confirm. once a beacon request is received, each
    // beacon confirm triggers the next beacon transmission until
    // stacks are exhausted
    if(maccore_globals->MacCore_BeaconRequestStackId >= DIVERSITY_NR_OF_STACKS)
    {
        // memory will be re-allocated as the next beacon request is processed
        gpPd_FreePd(p_PdLoh->handle);
        // all beacons were sent out, wait for the next beacon request
    }
    else
    {
        UIntLoop stackId;
        Bool validStack = false;

        // find the next valid stack; continue where we left off
        // make sure that an invalid stack does not prevent other stacks
        // from sending out beacons
        for(stackId = maccore_globals->MacCore_BeaconRequestStackId; stackId < DIVERSITY_NR_OF_STACKS; stackId++)
        {
            if(gpMacCore_cbValidStack(stackId) && gpMacCore_GetBeaconStarted(stackId) &&
               MacCore_RxForThisStack(stackId, gpMacCore_AddressModeShortAddress, rxInfo->rxChannel))
            {
                validStack = true;
                maccore_globals->MacCore_BeaconRequestStackId = stackId;
                break; // valid stack found - resume
            }
        }

        if(validStack)
        {
            gpMacCore_AddressMode_t srcAddrMode;
            gpMacCore_AddressInfo_t srcAddressInfo;
            gpMacCore_AddressInfo_t dstAddressInfo;
            gpHal_DataReqOptions_t dataReqOptions = {0};
            UInt8 channels[3];

            // reuse the same memory fragment, overwriting old data (otherwise new data will be concatenated)
            p_PdLoh->length = 0;

            // construct beacon (and write in PD)
            MacCore_ConstructBeacon(p_PdLoh, maccore_globals->MacCore_BeaconRequestStackId);

            {
                UInt16 shortAddress;
                // construct mac frame header
                shortAddress = gpMacCore_GetShortAddress(maccore_globals->MacCore_BeaconRequestStackId);

                if(GP_MACCORE_IS_SHORT_ADDR_UNALLOCATED(shortAddress))
                {
                    srcAddrMode = gpMacCore_AddressModeExtendedAddress;
                }
                else
                {
                    srcAddrMode = gpMacCore_AddressModeShortAddress;
                }
            }

            MacCore_InitSrcAddressInfo(&srcAddressInfo, srcAddrMode, maccore_globals->MacCore_BeaconRequestStackId);

            dstAddressInfo.addressMode = gpMacCore_AddressModeNoAddress;
            MacCore_WriteMacHeaderInPd(gpMacCore_FrameTypeBeacon, &srcAddressInfo, &dstAddressInfo, 0, 0, p_PdLoh,
                                       maccore_globals->MacCore_BeaconRequestStackId);

            channels[0] = gpMacCore_GetCurrentChannel(maccore_globals->MacCore_BeaconRequestStackId);
            channels[1] = GP_HAL_MULTICHANNEL_INVALID_CHANNEL;
            channels[2] = GP_HAL_MULTICHANNEL_INVALID_CHANNEL;
            dataReqOptions.macScenario = gpHal_MacDefault;
            dataReqOptions.srcId = maccore_globals->MacCore_BeaconRequestStackId;
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
            dataReqOptions.rawEncryptionEnable = false;
            dataReqOptions.rawKeepFrameCounter = false;
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)

            gpPad_SetTxChannels(MacCore_GetPad(maccore_globals->MacCore_BeaconRequestStackId), channels);

            MEMCPY(&local_pdloh, p_PdLoh, sizeof(gpPd_Loh_t));

            if(MacCore_TxDataRequest(&dataReqOptions, local_pdloh, stackId) != gpHal_ResultSuccess)
            {
                gpPd_FreePd(p_PdLoh->handle);
                return;
            }

            // once a confirm is received for this request, another beacon will be
            // sent out if needed or reset this counter
            ++maccore_globals->MacCore_BeaconRequestStackId;
        }
        else
        {
            // no valid stack found - since we are busy with the last stack, make sure
            // memory is release because there will be no incoming confirm to do
            // it - beacon not sent out.
            gpPd_FreePd(p_PdLoh->handle);
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT

STATIC_FUNC void MacCore_ProcessData(gpPd_Loh_t pdLoh, MacCore_HeaderDescriptor_t* pMdi, UInt8 macHeaderLength,
                                     gpHal_RxInfo_t* rxInfo)
{
    UInt8 result;
#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
    result = MacCore_IncomingFrameSecurityProcedure(&pdLoh, pMdi, macHeaderLength);
#else
    result = MacCore_ValidateClearText(pMdi);
#endif // GP_MACCORE_DIVERSITY_SECURITY_ENABLED
    if(result != gpMacCore_ResultSuccess)
    {
        UIntLoop i;
        GP_LOG_PRINTF("decryption failed: %x stack %x ", 4, result, pMdi->stackId);
        if(pMdi->stackId != GP_MACCORE_STACK_UNDEFINED)
        {
            gpMacCore_cbSecurityFailureCommStatusIndication(&pMdi->srcAddrInfo, &pMdi->dstAddrInfo, result,
                                                            pMdi->stackId, gpPd_GetRxTimestamp(pdLoh.handle));
        }
        else
        {
            for(i = 0; i < DIVERSITY_NR_OF_STACKS; i++)
            {
                if(gpMacCore_cbValidStack(i) &&
                   MacCore_RxForThisStack(i, pMdi->dstAddrInfo.addressMode, rxInfo->rxChannel))
                {
#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
                    if((!DIVERSITY_RAW_FRAMES()) || (i != MacCoreGetRawStack()))
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES
                    {
                        gpMacCore_cbSecurityFailureCommStatusIndication(&pMdi->srcAddrInfo, &pMdi->dstAddrInfo, result,
                                                                        i, gpPd_GetRxTimestamp(pdLoh.handle));
                    }
                }
            }
        }
        gpPd_FreePd(pdLoh.handle);
    }
    else
    {
#ifdef GP_MACCORE_DIVERSITY_DIAGCNTRS
        if(DIVERSITY_DIAGCNTRS())
        {
            if(pMdi->srcAddrInfo.addressMode == gpMacCore_AddressModeShortAddress)
            {
                MacCore_DiagRxCntrUpdateLastMessage(&pdLoh, pMdi->srcAddrInfo.address.Short);
            }
        }
#endif // GP_MACCORE_DIVERSITY_DIAGCNTRS
        if(pdLoh.length > 0)
        {
            if((pMdi->srcAddrInfo.addressMode == gpMacCore_AddressModeShortAddress &&
                pMdi->srcAddrInfo.address.Short == GP_MACCORE_SHORT_ADDR_BROADCAST) &&
               (pMdi->srcAddrInfo.panId == GP_MACCORE_PANID_BROADCAST))
            {
                GP_LOG_PRINTF("DataIndication dropped l:%u", 0, __LINE__);
                gpPd_FreePd(pdLoh.handle);
                return;
            }
#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
            if(DIVERSITY_POLL_ORIGINATOR())
            {
                if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs &&
                   GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->assoc == false)
                {
                    // We might not know the parent short or extended address, so don't do a check if
                    // parent short/extended addr is invalid.
                    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.addressMode ==
                       gpMacCore_AddressModeShortAddress)
                    {
                        if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.address.Short ==
                               pMdi->srcAddrInfo.address.Short ||
                           GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.address.Short ==
                               GP_MACCORE_SHORT_ADDR_BROADCAST) // Or not know
                        {
                            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = gpMacCore_ResultSuccess;
                        }
                    }
                    else if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.addressMode ==
                            gpMacCore_AddressModeExtendedAddress)
                    {
                        if((GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.address.Extended.LSB ==
                                pMdi->srcAddrInfo.address.Extended.LSB &&
                            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.address.Extended.MSB ==
                                pMdi->srcAddrInfo.address.Extended.MSB) ||
                           (GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.address.Extended.LSB ==
                                0xffffffff &&
                            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo.address.Extended.MSB ==
                                0xffffffff)) // Or not known
                        {
                            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = gpMacCore_ResultSuccess;
                        }
                    }
                }
            }
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
            if(pMdi->stackId != GP_MACCORE_STACK_UNDEFINED)
            {
                GP_STAT_SAMPLE_TIME();
                gpMacCore_cbDataIndication(&pMdi->srcAddrInfo, &pMdi->dstAddrInfo, pMdi->sequenceNumber,
                                           &pMdi->secOptions, pdLoh, pMdi->stackId);
            }
            else
            {
                UIntLoop count = 0;
                UIntLoop i;
                UInt8 stackList[GP_DIVERSITY_NR_OF_STACKS];
                gpPd_Handle_t pdList[GP_DIVERSITY_NR_OF_STACKS];

                GP_ASSERT_DEV_INT(DIVERSITY_NR_OF_STACKS <= GP_DIVERSITY_NR_OF_STACKS);
                // count number of stacks with an indication for optimal gpPd reusage
                for(i = 0; i < DIVERSITY_NR_OF_STACKS; i++)
                {
                    if(gpMacCore_cbValidStack(i) &&
                       MacCore_RxForThisStack(i, pMdi->dstAddrInfo.addressMode, rxInfo->rxChannel))
                    {
#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
                        if((!DIVERSITY_RAW_FRAMES()) || (gpMacCore_GetBeaconPayloadLength(i) != 0xFF))
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES
                        {
                            if(0 == count)
                            {
                                // don't allocate an unnecessary copy
                                pdList[count] = pdLoh.handle;
                            }
                            else
                            {
                                pdList[count] = gpPd_CopyPd(pdLoh.handle);
                                if(GP_PD_INVALID_HANDLE == pdList[count])
                                {
                                    UIntLoop idx;
                                    // All or nothing
                                    GP_LOG_SYSTEM_PRINTF("DataIndication dropped l:%u", 0, __LINE__);
                                    for(idx = 0; idx < count; ++idx)
                                    {
                                        gpPd_FreePd(pdList[idx]);
                                    }
                                    return;
                                }
                            }

                            stackList[count] = i;
                            count++;
                        }
                    }
                }
                // indicate to all selected stacks
                if(count)
                {
                    for(i = 0; i < count; i++)
                    {
                        gpPd_Loh_t pdLohCopy = pdLoh;
                        GP_STAT_SAMPLE_TIME();
                        pdLohCopy.handle = pdList[i];
                        gpMacCore_cbDataIndication(&pMdi->srcAddrInfo, &pMdi->dstAddrInfo, pMdi->sequenceNumber,
                                                   &pMdi->secOptions, pdLohCopy, stackList[i]);
                    }
                }
                else
                {
                    // dataIndication not handled
                    GP_LOG_PRINTF("DataIndication dropped l:%u", 0, __LINE__);
                    gpPd_FreePd(pdLoh.handle);
                }
            }
        }
        else
        {
            gpPd_FreePd(pdLoh.handle);
        }
    }
}

STATIC_FUNC void MacCore_cbDataConfirm(gpMacCore_Result_t result, gpPd_Handle_t pdHandle)
{
#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
    if(DIVERSITY_INDIRECT_TRANSMISSION())
    {
        if(MacCore_cbIndDataConfirm(result, pdHandle))
        {
            return;
        }
    }
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
    if(gpMacCore_ResultSuccess != result)
    {
        gpPd_SetTxAckRssi(pdHandle, 0);
        gpPd_SetTxAckLqi(pdHandle, 0);
    }
    gpMacCore_cbDataConfirm(result, pdHandle);
}

STATIC_FUNC void MacCore_HalDataConfirm_dummy(gpHal_Result_t status, gpPd_Loh_t pdLoh, UInt8 lastChannelUsed)
{
    NOT_USED(status);
    NOT_USED(pdLoh);
    NOT_USED(lastChannelUsed);
}
STATIC_FUNC void MacCore_HalDataIndication_dummy(gpPd_Loh_t pdLoh, gpHal_RxInfo_t* rxInfo)
{
    NOT_USED(pdLoh);
    NOT_USED(rxInfo);
}
#ifdef GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
STATIC_FUNC void MacCore_cbEDConfirm_dummy(UInt16 channelMask, UInt8* proto_energy_level_list)
{
    NOT_USED(channelMask);
    NOT_USED(proto_energy_level_list);
}
#endif

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpMacCore_Init(void)
{
    UIntLoop stackId;
#if defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT) ||                                                             \
    defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
#endif // defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT) || defined(GP_DIVERSITY_JUMPTABLES) &&
       // defined(GP_DIVERSITY_ROM_CODE)

    // Register gpHal callback functions
    gpHal_RegisterDataConfirmCallback(MacCore_HalDataConfirm);
    gpHal_RegisterDataIndicationCallback(MacCore_HalDataIndication);
    //
#if defined(GP_HAL_DIVERSITY_RAW_ENHANCED_ACK_RX)
    gpHal_MacSetMaxTransferTime(GP_MACCORE_MAX_TXFRAME_PLUS_RXENHACK_TIME);
#else
    gpHal_MacSetMaxTransferTime(GP_MACCORE_MAX_TXFRAME_PLUS_RXNORMALACK_TIME);
#endif
#if defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
    if(DIVERSITY_SCAN_ED_ORIGINATOR())
    {
        gpHal_RegisterEDConfirmCallback(gpHal_cbEDConfirm);
    }
#endif // GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR

    for(stackId = 0; stackId < DIVERSITY_NR_OF_STACKS; stackId++)
    {
        MacCore_InitPad(stackId);
    }

#if defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT) ||                                                             \
    defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
    maccore_globals->MacCore_BeaconRequestStackId = 0;
#endif // defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT) || defined(GP_DIVERSITY_JUMPTABLES) &&
       // defined(GP_DIVERSITY_ROM_CODE)

#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
    if(DIVERSITY_INDIRECT_TRANSMISSION())
    {
        MacCore_IndTxInit();
        MacCore_InitDataPending();
    }
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION

#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
    if(DIVERSITY_TIMEDTX())
    {
        // Allocate event ID for use with timed TX.
        GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId = GP_MACCORE_INVALID_TIMED_EVENT;
        GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_PdHandle = GP_PD_INVALID_HANDLE;
        GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_State = gpMacCore_TimedTxState_Idle;
    }
#endif

#ifdef GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS
    if(DIVERSITY_REGIONALDOMAINSETTINGS())
    {
        MacCore_InitRegionalDomainSettings();
    }
#endif

}

void gpMacCore_DeInit(void)
{
    UIntLoop stackId;

    for(stackId = 0; stackId < DIVERSITY_NR_OF_STACKS; stackId++)
    {
        if(gpPad_CheckPadValid(MacCore_GetPad(stackId)) == gpPad_ResultValidHandle)
        {
            gpMacCore_StackRemoved(stackId);
        }
    }

#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
    if(DIVERSITY_TIMEDTX())
    {
        // Release scheduled event.
        if(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId != GP_MACCORE_INVALID_TIMED_EVENT)
        {
#if !defined(GP_DIVERSITY_GPHAL_XP4002)
            gpHal_UnscheduleAbsoluteEvent(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId);
            gpHal_FreeAbsoluteEvent(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId);
#else
            gpHal_RtEventTimedTxRelease(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId);
#endif
            GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId = GP_MACCORE_INVALID_TIMED_EVENT;
        }
    }
#endif // GP_MACCORE_DIVERSITY_TIMEDTX

    // UnRegister gpHal callback functions
    gpHal_RegisterDataConfirmCallback(MacCore_HalDataConfirm_dummy);
    gpHal_RegisterDataIndicationCallback(MacCore_HalDataIndication_dummy);
#if defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
    if(DIVERSITY_SCAN_ED_ORIGINATOR())
    {
        gpHal_RegisterEDConfirmCallback(MacCore_cbEDConfirm_dummy);
    }
#endif // GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
}

void gpMacCore_DataRequest(gpMacCore_AddressMode_t srcAddrMode, gpMacCore_AddressInfo_t* pDstAddrInfo, UInt8 txOptions,
                           gpMacCore_Security_t* pSecOptions, gpMacCore_MultiChannelOptions_t multiChannelOptions,
                           gpPd_Loh_t pdLoh, gpMacCore_StackId_t stackId)
{
    gpHal_DataReqOptions_t dataReqOptions = {0};
    gpMacCore_Result_t result = gpMacCore_ResultSuccess;
    gpMacCore_AddressInfo_t srcAddressInfo;
#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

    GP_LOG_PRINTF("Mac DR sId:%u h:%u l:%u txOpt:%x", 0, stackId, pdLoh.handle, pdLoh.length, txOptions);

    // Check correct channel setting
    if(((multiChannelOptions.channel[0] != GP_MACCORE_INVALID_CHANNEL) &&
        !GP_MACCORE_CHECK_CHANNEL_VALID(multiChannelOptions.channel[0])) ||
       ((multiChannelOptions.channel[1] != GP_MACCORE_INVALID_CHANNEL) &&
        !GP_MACCORE_CHECK_CHANNEL_VALID(multiChannelOptions.channel[1])) ||
       ((multiChannelOptions.channel[2] != GP_MACCORE_INVALID_CHANNEL) &&
        !GP_MACCORE_CHECK_CHANNEL_VALID(multiChannelOptions.channel[2])))
    {
        GP_LOG_SYSTEM_PRINTF("wrong ch: %u %u %u", 0, multiChannelOptions.channel[0], multiChannelOptions.channel[1],
                             multiChannelOptions.channel[2]);
        MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
        return;
    }

    if(GP_MACCORE_TIMEDTX_ENABLED(txOptions))
    {
#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
        if(DIVERSITY_TIMEDTX())
        {
            if(GP_MACCORE_INDIRECT_TRANSMISSION_ENABLED(txOptions))
            {
                GP_LOG_PRINTF("Can not use timed TX with indirect TX", 0);
                MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
                return;
            }

            if(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_State != gpMacCore_TimedTxState_Idle)
            {
                GP_LOG_PRINTF("Scheduled TX queue already in use (max 1 packet)", 0);
                MacCore_cbDataConfirm(gpMacCore_ResultTransactionOverflow, pdLoh.handle);
                return;
            }

            // Allocate event descriptor.
            if(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId == GP_MACCORE_INVALID_TIMED_EVENT)
            {
                GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId = GP_MACCORE_ALLOCATE_TIMED_EVENT();
                GP_ASSERT_DEV_INT(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId != GP_MACCORE_INVALID_TIMED_EVENT);
                GP_LOG_PRINTF("timedtx eventid %d", 0, GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId);
            }
            if(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId == GP_MACCORE_INVALID_TIMED_EVENT)
            {
                // Allocation of event descriptor failed.
                MacCore_cbDataConfirm(gpMacCore_ResultTransactionOverflow, pdLoh.handle);
                return;
            }
        }
#endif // GP_MACCORE_DIVERSITY_TIMEDTX
        if(!DIVERSITY_TIMEDTX())
        {
            GP_LOG_PRINTF("Timed TX not supported", 0);
            MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
            return;
        }
    }

    if(GP_MACCORE_INDIRECT_TRANSMISSION_ENABLED(txOptions))
    {
        // Add to queue and return
#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
        if(DIVERSITY_INDIRECT_TRANSMISSION())
        {
            GP_MACCORE_INDIRECT_TRANSMISSION_CLEAR(txOptions);
            if(!MacCore_IndTxAddElement(srcAddrMode, pDstAddrInfo, txOptions, pSecOptions, multiChannelOptions, pdLoh,
                                        stackId))
            {
                MacCore_cbDataConfirm(gpMacCore_ResultTransactionOverflow, pdLoh.handle);
            }
        }
#endif // GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
        if(!DIVERSITY_INDIRECT_TRANSMISSION())
        {
            GP_LOG_PRINTF("IndTx not supported", 0);
            MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
        }
        return;
    }

    if(GP_MACCORE_INJECT_ENABLED(txOptions))
    {
#ifdef GP_MACCORE_DIVERSITY_POLL_RECIPIENT
        if(DIVERSITY_POLL_RECIPIENT())
        {
            UInt8 index;

            GP_MACCORE_INJECT_CLEAR(txOptions);
            // Check Indirect Tx queue
            index = MacCore_IndTxBufferFindElemForTx(pDstAddrInfo, stackId);
            if(index != 0xFF)
            {
                txOptions |= GP_MACCORE_TX_OPT_MORE_DATA_PENDING;
            }
        }
#endif // GP_MACCORE_DIVERSITY_POLL_RECIPIENT
        if(!DIVERSITY_POLL_RECIPIENT())
        {
            GP_LOG_PRINTF("Inject not supported", 0);
            MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
            return;
        }
    }

    GP_STAT_SAMPLE_TIME();

    // check parameters
    if(GP_MACCORE_CHECK_IF_ADDRESSMODE_NOT_NONE(srcAddrMode) &&
       ((pDstAddrInfo == NULL) || GP_MACCORE_CHECK_IF_ADDRESSMODE_NOT_NONE(pDstAddrInfo->addressMode)))
    {
        GP_LOG_PRINTF("Inv addr", 0);
        MacCore_cbDataConfirm(gpMacCore_ResultInvalidAddress, pdLoh.handle);
        return;
    }

    if(GP_MACCORE_CHECK_IF_ADDRESSMODE_INVALID(srcAddrMode))
    {
        GP_LOG_PRINTF("inv src addr mode", 0);
        MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
        return;
    }

    if(GP_MACCORE_CHECK_IF_ADDRESSINFO_INVALID(pDstAddrInfo))
    {
        GP_LOG_PRINTF("Inv dst info", 0);
        MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
        return;
    }

    if((multiChannelOptions.channel[0] == GP_MACCORE_INVALID_CHANNEL) &&
       (gpMacCore_GetCurrentChannel(stackId) == GP_MACCORE_INVALID_CHANNEL))
    {
        GP_LOG_PRINTF("MacCore Data req invalid channel", 0);
        MacCore_cbDataConfirm(gpMacCore_ResultInvalidParameter, pdLoh.handle);
        return;
    }

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

    MacCore_InitSrcAddressInfo(&srcAddressInfo, srcAddrMode, stackId);

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
    dataReqOptions.rawEncryptionEnable = false;
    dataReqOptions.rawKeepFrameCounter = false;
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
    GP_LOG_PRINTF("Raw gpMacCore_DataRequest opts=%x", 0, txOptions);

    if(DIVERSITY_RAW_FRAMES() && ((txOptions & GP_MACCORE_TX_OPT_RAW) != 0))
    {
        /* the mac header is already present in the higher layer data */
        GP_LOG_PRINTF("Raw gpMacCore_DataRequest st=%d pd=%d", 0, stackId, pdLoh.handle);
        GP_MACCORE_GET_GLOBALS_CONST()->MacCore_RawFrameInfoPtr->raw[pdLoh.handle] = true;
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
        if(GP_MACCORE_NO_SECURITY_SPECIFIED(pSecOptions))
        {
            // pass. No further action needed.
        }
        else
        {
            // set up the encryption config.
            dataReqOptions.rawEncryptionEnable = true;
            if(txOptions & GP_MACCORE_TX_OPT_RAW_KEEP_FRAMECOUNTER)
            {
                dataReqOptions.rawKeepFrameCounter = true;
            }
            MacCore_GetRawModeEncryptionOffsets(pdLoh, &dataReqOptions.rawAuxOffset, &dataReqOptions.rawDataOffset,
                                                &dataReqOptions.rawCslIeOffset);
        }
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
    }
    else
    {
        GP_MACCORE_GET_GLOBALS_CONST()->MacCore_RawFrameInfoPtr->raw[pdLoh.handle] = false;
#else
    {
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

        //  can also be optimized by converting to assert if
        if(GP_MACCORE_NO_SECURITY_SPECIFIED(pSecOptions))
        {
            // no security ==> only write normal MAC header
            MacCore_WriteMacHeaderInPd(gpMacCore_FrameTypeData, &srcAddressInfo, pDstAddrInfo, txOptions,
                                       gpEncryption_SecLevelNothing, &pdLoh, stackId);
            if(pdLoh.length > GP_MACCORE_MAX_PHY_PACKET_SIZE_NO_FCS)
            {
                // If any parameter in the MCPS-DATA.request primitive is not supported or is out of range, the MAC
                // sublayer will issue the MCPS-DATA.confirm primitive with a status of INVALID_PARAMETER.
                result = gpMacCore_ResultInvalidParameter;
            }
        }
        else
        {
#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
            result = MacCore_PrepareOutgoingSecuredPacket(&srcAddressInfo, pDstAddrInfo, &pdLoh, txOptions, pSecOptions,
                                                          gpMacCore_FrameTypeData, stackId);
#else
            result = gpMacCore_ResultUnsupportedSecurity;
#endif // GP_MACCORE_DIVERSITY_SECURITY_ENABLED
        }
    }

    if(result != gpMacCore_ResultSuccess)
    {
        GP_LOG_PRINTF("MacCore Data req unsuccessful MAC result %x", 0, result);
        MacCore_cbDataConfirm(result, pdLoh.handle);
        return;
    }

    if(multiChannelOptions.channel[0] == GP_MACCORE_INVALID_CHANNEL)
    {
        multiChannelOptions.channel[0] = gpMacCore_GetCurrentChannel(stackId);
        multiChannelOptions.channel[1] = GP_MACCORE_INVALID_CHANNEL;
        multiChannelOptions.channel[2] = GP_MACCORE_INVALID_CHANNEL;
    }

    // set channels
    gpPad_SetTxChannels(MacCore_GetPad(stackId), multiChannelOptions.channel);

    dataReqOptions.macScenario = GP_MACCORE_TIMEDTX_ENABLED(txOptions) ? gpHal_MacTimedTx : gpHal_MacDefault;
    dataReqOptions.srcId = stackId;

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

    {
        result = MacCore_TxDataRequest(&dataReqOptions, pdLoh, stackId);
    }
    if(result != gpHal_ResultSuccess)
    {
        GP_LOG_PRINTF("MacCore Data req unsuccessful HAL result %x", 2, result);
        MacCore_cbDataConfirm(result, pdLoh.handle);
        return;
    }

#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
    if(DIVERSITY_TIMEDTX())
    {
        if(GP_MACCORE_TIMEDTX_ENABLED(txOptions))
        {
            // Mark scheduled TX queue busy.
            GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_PdHandle = pdLoh.handle;
            GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_State = gpMacCore_TimedTxState_Queued;
        }
    }
#endif // GP_MACCORE_DIVERSITY_TIMEDTX

}

gpMacCore_Result_t gpMacCore_ScheduleTimedTx(gpPd_Handle_t pdHandle, gpMacCore_TxTimingOptions_t timingOptions,
                                             gpMacCore_StackId_t stackId)
{
    gpMacCore_Result_t result = gpMacCore_ResultInvalidParameter; // case for Timed TX not supported
#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
    if(DIVERSITY_TIMEDTX())
    {
        // Check that the specified packet is queued but not yet scheduled.
        if((GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_State != gpMacCore_TimedTxState_Queued) ||
           (GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_PdHandle != pdHandle))
        {
            return gpMacCore_ResultInvalidHandle;
        }

        GP_ASSERT_DEV_EXT(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId != GP_MACCORE_INVALID_TIMED_EVENT);
        GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_State = gpMacCore_TimedTxState_Scheduled;

#if !defined(GP_DIVERSITY_GPHAL_XP4002)
        gpHal_AbsoluteEventDescriptor_t eventDesc;
        // Create event descriptor and push it to the gpHal event scheduler.
        eventDesc.exTime = timingOptions.txTimestamp;
        eventDesc.recPeriod = 0;
        eventDesc.recAmount = 0;
        eventDesc.customData = 0;
        eventDesc.executionOptions = GP_ES_EXECUTION_OPTIONS_EXECUTE_IF_TOO_LATE;
        eventDesc.interruptOptions = 0;
        eventDesc.control = 0x0;
        GP_ES_SET_EVENT_RESULT(eventDesc.control, gpHal_EventResultInvalid);
        GP_ES_SET_EVENT_STATE(eventDesc.control, gpHal_EventStateScheduled);
        eventDesc.type = gpHal_EventTypeTXPacket;
        gpHal_ScheduleAbsoluteEvent(&eventDesc, GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId);
#else
        gpHal_DataRequest_ScheduleTimedTx(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId, pdHandle,
                                          timingOptions.txTimestamp);
#endif // !defined(GP_DIVERSITY_GPHAL_XP4002)

        result = gpMacCore_ResultSuccess;
    }
#endif // GP_MACCORE_DIVERSITY_TIMEDTX
    return result;
}

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
void gpMacCore_OrphanResponse(MACAddress_t* pOrphanAddress, UInt16 shortAddress, Bool associatedMember,
                              gpMacCore_StackId_t stackId)
{
    if(associatedMember) // device is associated with coord
    {
        gpMacCore_Result_t result;

        result = MacCore_SendCommandCoordinatorRealignment(pOrphanAddress, shortAddress, false, stackId);

        if(result != gpMacCore_ResultSuccess)
        {
            gpMacCore_AddressInfo_t srcAddr;
            gpMacCore_AddressInfo_t dstAddr;
            // Get SrcAddr attributes
            MacCore_InitSrcAddressInfo(&srcAddr, gpMacCore_AddressModeShortAddress, stackId);
            // Get DstAddr attributes
            dstAddr.addressMode = gpMacCore_AddressModeExtendedAddress;
            MEMCPY(&dstAddr.address.Extended, pOrphanAddress, sizeof(MACAddress_t));
            // derive PanID
            dstAddr.panId = gpMacCore_GetPanId(stackId);
            gpMacCore_cbOrphanCommStatusIndication(&srcAddr, &dstAddr, result, stackId, 0);
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT

#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
void MacCore_DelayedPollConfirm(void)
{
    if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs)
    {
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
        if(DIVERSITY_ASSOCIATION_ORIGINATOR())
        {
            // Path only used in case of failure cases
            // Association Confirm already triggered when receiving Assoc Response
            if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->assoc)
            {
                // if timeout has not been reached yet, re-schedule data poll
                if(gpSched_ExistsEvent(MacCore_AssociateTimeout))
                {
                    gpSched_ScheduleEvent(
                        (UInt32)((UInt32)GP_MACCORE_RESPONSE_WAIT_TIME * (UInt32)GP_MACCORE_SYMBOL_DURATION),
                        MacCore_AssociateSendCommandDataRequest);
                }
                else // if timeout reached during in flight data poll, confirm fail poll
                {
                    // Result is filled in in pPollReqArgs -> used in Confirm handler
                    MacCore_HandleAssocConf();
                }
                return;
            }
        }
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
        {
            gpMacCore_AddressInfo_t coordAddrInfo = GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->coordAddrInfo;
            gpMacCore_Result_t result = GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result;
            gpPd_TimeStamp_t txTimestamp = GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->txTimestamp;
            gpPoolMem_Free(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs);
            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs = NULL;

            gpMacCore_cbPollConfirm(result, &coordAddrInfo, txTimestamp);
        }
    }
}
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR

Bool MacCore_RxForThisStack(gpMacCore_StackId_t stackId, gpMacCore_AddressMode_t dstAddrMode, UInt8 rxChannel)
{
    UInt8 compareChannel;

    if(dstAddrMode == gpMacCore_AddressModeNoAddress && !gpMacCore_GetPanCoordinator(stackId) // Pan coordinator allows
                                                                                              // ModeNoAddress
       && (gpHal_GetFrameTypeFilterMask() & GPHAL_ENUM_FRAME_TYPE_FILTER_BCN_MASK)) // Shouldn't get ModeNoAddress when
                                                                                    // beacons are already filtered
    {
        return false;
    }

    compareChannel = rxChannel;
#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
    if(DIVERSITY_POLL_ORIGINATOR())
    {
        // TODO: check logic of this...!
        if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs != NULL)
        {
            // Poll request is waiting for its data
            if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId == stackId)
            {
                return true;
            }
            else
            {
                compareChannel = gpRxArbiter_GetStackChannel(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId);
            }
        }
    }
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR

    // Enhanced to compare all the slot channels
    if((compareChannel == GP_RX_ARBITER_INVALID_CHANNEL) || !(gpRxArbiter_IsAnActiveChannel(stackId, compareChannel)))
    {
        return false;
    }
#if(GP_DIVERSITY_NR_OF_STACKS > 1)
    if(gpRxArbiter_GetStackRxOn(stackId) || gpRxArbiter_GetDutyCycleEnabled(stackId))
    {
        return true;
    }
    return false;
#else  // (GP_DIVERSITY_NR_OF_STACKS>1)

    // single-stack applications can only receive if their own radio is on.
    // No need for extra checks.
    return true;
#endif // (GP_DIVERSITY_NR_OF_STACKS>1)
}

//----------------
// gpHal callback functions
//----------------

void MacCore_HalDataConfirm(gpHal_Result_t hal_status, gpPd_Loh_t pdLoh, UInt8 lastChannelUsed)
{
    MacCore_HeaderDescriptor_t mdi;
    gpMacCore_Result_t status = MacCore_Hal2MacResult(hal_status);


    GP_STAT_SAMPLE_TIME();

#ifdef GP_MACCORE_DIVERSITY_TIMEDTX
    if(DIVERSITY_TIMEDTX())
    {
        if(pdLoh.handle == GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_PdHandle)
        {
            // Packet from scheduled TX queue now confirmed.
            GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_State = gpMacCore_TimedTxState_Idle;
            GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_PdHandle = GP_PD_INVALID_HANDLE;
            if(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId != GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID)
            {
#if !defined(GP_DIVERSITY_GPHAL_XP4002)
                gpHal_FreeAbsoluteEvent(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId);
#else
                gpHal_RtEventTimedTxRelease(GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId);
#endif
                GP_MACCORE_GET_GLOBALS()->MacCore_TimedTx_EventId = GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID;
            }
        }
    }
#endif // GP_MACCORE_DIVERSITY_TIMEDTX

    MacCore_AnalyseMacHeader(&pdLoh, &mdi);
    mdi.stackId = MacCore_GetStackId(&mdi.srcAddrInfo);

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
    if(DIVERSITY_RAW_FRAMES())
    {
        if(GP_MACCORE_GET_GLOBALS_CONST()->MacCore_RawFrameInfoPtr->raw[pdLoh.handle])
        {
            {
                GP_LOG_PRINTF("Raw gpMacCore_cbDataConfirm  pd=%d: l:%u ch:%u", 0, pdLoh.handle, pdLoh.length,
                              lastChannelUsed);

#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
                if(MACCORE_FRAMECONTROL_SECURITY_GET(mdi.frameControl))
                {
                    gpMacCore_cbSecurityFrameCounterIndication(mdi.frameCounter, mdi.stackId);
                }
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
                MacCore_cbDataConfirm(status, pdLoh.handle);
                GP_MACCORE_GET_GLOBALS_CONST()->MacCore_RawFrameInfoPtr->raw[pdLoh.handle] = false;
            }
            return;
        }
    }
#endif

    switch(mdi.frameType)
    {
        case gpMacCore_FrameTypeData:
        {
            if(lastChannelUsed != GP_HAL_MULTICHANNEL_INVALID_CHANNEL)
            {
                // it was a gpHal multichannel retransmit
                // => adapt the current stack channel
                gpRxArbiter_SetStackChannel(lastChannelUsed, MacCore_GetStackId(&mdi.srcAddrInfo));
            }
#ifdef GP_MACCORE_DIVERSITY_DIAGCNTRS
            if(DIVERSITY_DIAGCNTRS())
            {
                if(MACCORE_FRAMECONTROL_DSTADDRMODE_GET(mdi.frameControl) == gpMacCore_AddressModeShortAddress)
                {
                    MacCore_DiagTxCntrUpdateAvgRetryMessageSent(gpPd_GetTxRetryCntr(pdLoh.handle),
                                                                mdi.dstAddrInfo.address.Short);
                }
            }
#endif // GP_MACCORE_DIVERSITY_DIAGCNTRS

            MacCore_cbDataConfirm(status, pdLoh.handle);
            return;
        }
        case gpMacCore_FrameTypeCommand:
        {
            gpMacCore_Command_t command;
#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT) ||     \
    defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT)
            gpPd_TimeStamp_t txTime;
            txTime = gpPd_GetTxTimestamp(pdLoh.handle);
#endif
            command = gpPd_ReadByte(pdLoh.handle, pdLoh.offset);

            switch(command)
            {
                case gpMacCore_CommandBeaconRequest:
                {
                    break;
                }
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
                case gpMacCore_CommandOrphanNotification:
                {
                    break;
                }
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
                case gpMacCore_CommandAssociationResponse:
                {
                    if(DIVERSITY_ASSOCIATION_RECIPIENT())
                    {
                        mdi.stackId = MacCore_GetStackId(&mdi.srcAddrInfo);
                        MacCore_cbAssocConfirm(&mdi.srcAddrInfo, &mdi.dstAddrInfo, status, mdi.stackId, txTime);
                    }
                    break;
                }
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
                case gpMacCore_CommandAssociationRequest:
                {
                    if(DIVERSITY_ASSOCIATION_ORIGINATOR())
                    {
                        if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs)
                        {
                            if(status == gpHal_ResultSuccess)
                            {
                                gpSched_ScheduleEvent((UInt32)((UInt32)GP_MACCORE_RESPONSE_WAIT_TIME *
                                                               (UInt32)GP_MACCORE_SYMBOL_DURATION),
                                                      MacCore_AssociateSendCommandDataRequest);
                            }
                            else
                            {
                                GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = status;
                                GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->txTimestamp = txTime;
                                MacCore_HandleAssocConf();
                            }
                        }
                    }
                    break;
                }
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
                case gpMacCore_CommandDataRequest:
                {
                    if(DIVERSITY_POLL_ORIGINATOR())
                    {
                        // FP bit is not correctly read on K5 and status is always successful
                        if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs &&
                           (GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->pdHandle == pdLoh.handle)) // Skipping old
                                                                                                         // dataConfirms
                                                                                                         // (expected
                                                                                                         // only from a
                                                                                                         // Association
                                                                                                         // poll
                                                                                                         // request)
                        {
                            /* Do not use the result here from the poll confirm.
                               GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result is initially set to no data
                               and is updated by sw if we received correct packet. */
                            if(status == gpHal_ResultNoAck)
                            {
                                GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result = gpMacCore_ResultNoAck;
                            }
                            GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->txTimestamp = txTime;
                            // schedule poll/assoc confirm as we want to make sure all dataIndication interrupts are
                            // handled.
                            gpSched_ScheduleEvent(0, MacCore_DelayedPollConfirm);
                        }
                    }
                    break;
                }
#endif // GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
                case gpMacCore_CommandCoordinatorRealignment:
                {
                    if(DIVERSITY_SCAN_ORPHAN_RECIPIENT())
                    {
                        mdi.stackId = MacCore_GetStackId(&mdi.srcAddrInfo);
                        gpMacCore_cbOrphanCommStatusIndication(&mdi.srcAddrInfo, &mdi.dstAddrInfo, status, mdi.stackId,
                                                               txTime);
                    }
                    break;
                }
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
                default:
                {
                    break;
                }
            }
            break;
        }
#ifdef GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT
        case gpMacCore_FrameTypeBeacon: // Beacon confirmed
        {
            if(DIVERSITY_SCAN_ACTIVE_RECIPIENT())
            {
                // send out the next beacon if needed - beacon confirmed
                gpHal_RxInfo_t rxInfo;
                rxInfo.rxChannel = lastChannelUsed;
                MacCore_TransmitBeacon(&pdLoh, &rxInfo);
                return;
            }
        }
#endif // GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT
        default:
        {
            break;
        }
    }

    // In all cases free the handle
    gpPd_FreePd(pdLoh.handle);
}

// MacCore_HalDataIndication - packet reception at MAC layer
void MacCore_HalDataIndication(gpPd_Loh_t pdLoh, gpHal_RxInfo_t* rxInfo)
{
    MacCore_HeaderDescriptor_t mdi;
    UInt8 macHeaderLength;
#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
    gpPd_Loh_t pdLoh_backup = pdLoh;
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES


    // analyze the MAC header and check if it contains invalid data in framecontrol
    MEMSET((UInt8*)&mdi, 0, sizeof(MacCore_HeaderDescriptor_t));

    macHeaderLength = MacCore_AnalyseMacHeader(&pdLoh, &mdi);
    mdi.stackId = MacCore_GetStackId(&mdi.dstAddrInfo);


    if(macHeaderLength == GP_MACCORE_INVALID_HEADER_LENGTH)
    {
        gpPd_FreePd(pdLoh.handle);
        return;
    }
    // fixed for indirect tx,not yet for assoc, when reworking assoc, fix this. (merge
    // GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs and GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs drop
    // packets which are received on another channel than the stackchannel. drop packet if a packet is received even
    // though the receiver for the stack was not on.
    if(mdi.stackId != GP_MACCORE_STACK_UNDEFINED)
    {
        if((!MacCore_RxForThisStack(mdi.stackId, mdi.dstAddrInfo.addressMode, rxInfo->rxChannel)) &&
           (mdi.frameType != gpMacCore_FrameTypeAcknowledge)) // ACK frames can be received if RxOnWhenIdle is not set.
        {
            GP_LOG_PRINTF("drop %i %i %i %i", 0, mdi.stackId, gpRxArbiter_GetStackRxOn(mdi.stackId),
                          gpRxArbiter_GetStackChannel(mdi.stackId), gpRxArbiter_GetCurrentRxChannel());
            gpPd_FreePd(pdLoh.handle);
            return;
        }
    }

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
    if(DIVERSITY_RAW_FRAMES())
    {
        // if the mdi.stackId is GP_MACCORE_STACK_UNDEFINED or is equal to the OpenThread stackId
        // then send a copy to the openthread stack

        // if the mdi.stackId is GP_MACCORE_STACK_UNDEFINED or is equal to any other stackid
        // then send a copy to the all stacks, except the OpenThread stack !!!!

        if(mdi.stackId == GP_MACCORE_STACK_UNDEFINED)
        {
            UInt8 stackid = MacCoreGetRawStack();
            // use the backup pdloh to be able to return the frame with the mac header included.
            if((mdi.frameType == gpMacCore_FrameTypeAcknowledge) &&
               (MACCORE_FRAMECONTROL_FRAMEVERSION_GET(mdi.frameControl) != gpMacCore_MacVersion2015))
            {
                // drop non-enhanced ACK frame
                gpPd_FreePd(pdLoh.handle);
                return;
            }

            if(stackid != GP_MACCORE_STACK_UNDEFINED)
            {
                pdLoh_backup.handle = gpPd_CopyPd(pdLoh.handle);

                if(GP_PD_INVALID_HANDLE == pdLoh_backup.handle)
                {
                    GP_LOG_PRINTF("Can't allocated Pd for raw stack", 0);
                    gpPd_FreePd(pdLoh.handle);
                    return;
                }

                GP_LOG_PRINTF("Raw1 gpMacCore_cbDataIndication st=%d seq=%d", 0, stackid, mdi.sequenceNumber);
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
                MacCore_StoreLinkMetrics(&mdi, pdLoh_backup);
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
                gpMacCore_cbDataIndication(&mdi.srcAddrInfo, &mdi.dstAddrInfo, mdi.sequenceNumber, &mdi.secOptions,
                                           pdLoh_backup, stackid);
            }
        }
        if((mdi.stackId != GP_MACCORE_STACK_UNDEFINED) && (gpMacCore_GetBeaconPayloadLength(mdi.stackId) == 0xFF))
        {
       // GP_LOG_SYSTEM_PRINTF("Raw2 gpMacCore_cbDataIndication st=%d seq=%d",0, mdi.stackId, mdi.sequenceNumber);
#if defined(GP_MACCORE_DIVERSITY_SECURITY_ENABLED) && defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
            MacCore_StoreLinkMetrics(&mdi, pdLoh_backup);
#endif // defined(GP_MACCORE_DIVERSITY_SECURITY_ENABLED) && defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
            gpMacCore_cbDataIndication(&mdi.srcAddrInfo, &mdi.dstAddrInfo, mdi.sequenceNumber, &mdi.secOptions,
                                       pdLoh_backup, mdi.stackId);
            return;
        }
    }
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES

    switch(mdi.frameType)
    {
#if defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)
        case gpMacCore_FrameTypeBeacon:
        {
            if(DIVERSITY_SCAN_ACTIVE_ORIGINATOR())
            {
                MacCore_ProcessBeacon(pdLoh, &mdi, rxInfo);
            }
            gpPd_FreePd(pdLoh.handle);
            break;
        }
#endif // GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR
        case gpMacCore_FrameTypeData:
        {
            MacCore_ProcessData(pdLoh, &mdi, macHeaderLength, rxInfo);
            break;
        }
        case gpMacCore_FrameTypeCommand:
        {
            UInt8 result;
#ifdef GP_MACCORE_DIVERSITY_SECURITY_ENABLED
            result = MacCore_IncomingFrameSecurityProcedure(&pdLoh, &mdi, macHeaderLength);
#else
            result = MacCore_ValidateClearText(&mdi);
#endif // GP_MACCORE_DIVERSITY_SECURITY_ENABLED
            if(result != gpMacCore_ResultSuccess)
            {
                GP_LOG_SYSTEM_PRINTF("dec fail:%x", 0, result);
                if(mdi.stackId != GP_MACCORE_STACK_UNDEFINED)
                {
                    gpMacCore_cbSecurityFailureCommStatusIndication(&mdi.srcAddrInfo, &mdi.dstAddrInfo, result,
                                                                    mdi.stackId, gpPd_GetRxTimestamp(pdLoh.handle));
                }
                else
                {
                    UIntLoop i;
                    for(i = 0; i < DIVERSITY_NR_OF_STACKS; i++)
                    {
                        if(gpMacCore_cbValidStack(i) &&
                           MacCore_RxForThisStack(i, mdi.dstAddrInfo.addressMode, rxInfo->rxChannel))
                        {
#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
                            if((!DIVERSITY_RAW_FRAMES()) || (i != MacCoreGetRawStack()))
#endif // GP_MACCORE_DIVERSITY_RAW_FRAMES
                            {
                                gpMacCore_cbSecurityFailureCommStatusIndication(
                                    &mdi.srcAddrInfo, &mdi.dstAddrInfo, result, i, gpPd_GetRxTimestamp(pdLoh.handle));
                            }
                        }
                    }
                }
                gpPd_FreePd(pdLoh.handle);
            }
            else
            {
                gpMacCore_Command_t command;
#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT) || defined(GP_MACCORE_DIVERSITY_POLL_RECIPIENT) ||             \
    defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT)
                gpPd_TimeStamp_t rxTime = gpPd_GetRxTimestamp(pdLoh.handle);
#endif // defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT) || defined(GP_MACCORE_DIVERSITY_POLL_RECIPIENT)
                command = gpPd_ReadByte(pdLoh.handle, pdLoh.offset);
                pdLoh.offset++;
                switch(command)
                {
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
                    case gpMacCore_CommandAssociationRequest:
                    {
                        if(DIVERSITY_ASSOCIATION_RECIPIENT())
                        {
                            // assoc request is always 2 bytes
                            // mdi.stackId has to be valid here -> stack is based on pan id
                            if(pdLoh.length == GP_MACCORE_ASSOCIATION_REQUEST_CMD_LEN &&
                               mdi.stackId != GP_MACCORE_STACK_UNDEFINED)
                            {
                                // only respond to association request commands when association permit is true
                                if(gpMacCore_GetAssociationPermit(mdi.stackId))
                                {
                                    UInt8 capabilityInformation = gpPd_ReadByte(pdLoh.handle, pdLoh.offset);
                                    gpMacCore_cbAssociateIndication(&mdi.srcAddrInfo.address, capabilityInformation,
                                                                    mdi.stackId, rxTime);
                                }
                            }
                        }
                        break;
                    }
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
                    case gpMacCore_CommandAssociationResponse:
                    {
                        if(DIVERSITY_ASSOCIATION_ORIGINATOR())
                        {
                            // mdi.stackId not relevant here -> use
                            // GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId Assoc request pending,
                            // if response here, we received data
                            if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs &&
                               GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->assoc == true &&
                               pdLoh.length == GP_MACCORE_ASSOCIATION_RESPONSE_CMD_LEN)
                            {
                                gpPd_ReadByteStream(
                                    pdLoh.handle, pdLoh.offset, 2,
                                    (UInt8*)&GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->responseShortAddr);
                                pdLoh.offset += 2;
                                LITTLE_TO_HOST_UINT16(
                                    &GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->responseShortAddr);
                                GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result =
                                    gpPd_ReadByte(pdLoh.handle, pdLoh.offset);
                                if(GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->result == gpMacCore_ResultSuccess)
                                {
                                    gpMacCore_SetCoordExtendedAddress(
                                        &mdi.srcAddrInfo.address.Extended,
                                        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId);
                                    gpMacCore_SetShortAddress(
                                        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->responseShortAddr,
                                        GP_MACCORE_GET_GLOBALS()->gpMacCore_pPollReqArgs->stackId);
                                }
                                // if success or not, always send the callback up to upper layer, when assoc response is
                                // correctly received just keep pooling if data poll is not MAC ACKed or if no packet is
                                // received on the RX window.
                                MacCore_HandleAssocConf();
                            }
                        }
                        break;
                    }
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
#ifdef GP_MACCORE_DIVERSITY_POLL_RECIPIENT
                    case gpMacCore_CommandDataRequest:
                    {
                        if(DIVERSITY_POLL_RECIPIENT())
                        {
                            // mdi.stackId has to be valid here -> if not, drop
                            if(pdLoh.length == GP_MACCORE_DATA_REQUEST_CMD_LEN &&
                               mdi.stackId != GP_MACCORE_STACK_UNDEFINED)
                            {
#ifdef GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
                                MacCore_AssocRsp_t* pAssocRsp =
                                    MacCore_GetAssocRespPointer(mdi.srcAddrInfo.address.Extended);
                                if(DIVERSITY_ASSOCIATION_RECIPIENT() &&
                                   mdi.srcAddrInfo.addressMode == gpMacCore_AddressModeExtendedAddress &&
                                   pAssocRsp != NULL)
                                {
                                    gpSched_ScheduleEventArg(0, MacCore_cbAssocResp, (void*)pAssocRsp);
                                }
                                else
#endif // GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
                                {
                                    Bool framePending;
                                    framePending = MacCore_CheckIfDataPending(&mdi.srcAddrInfo, mdi.stackId);
                                    MacCore_cbPollIndication(&mdi.srcAddrInfo, mdi.stackId, rxTime, framePending);
                                }
                            }
                        }
                        break;
                    }
#endif // GP_MACCORE_DIVERSITY_POLL_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
                    case gpMacCore_CommandOrphanNotification:
                    {
                        if(DIVERSITY_SCAN_ORPHAN_RECIPIENT())
                        {
                            // mdi.stackId not valid here (other side lost his stack) -> ignore
                            // orphan has extended src address
                            if(mdi.srcAddrInfo.addressMode == gpMacCore_AddressModeExtendedAddress &&
                               pdLoh.length == GP_MACCORE_ORPHAN_NOTIFICATION_CMD_LEN)
                            {
                                UInt8 stackId;
                                for(stackId = 0; stackId < DIVERSITY_NR_OF_STACKS; stackId++)
                                {
                                    if(gpMacCore_cbValidStack(stackId) &&
                                       MacCore_RxForThisStack(stackId, gpMacCore_AddressModeExtendedAddress,
                                                              rxInfo->rxChannel))
                                    {
                                        gpMacCore_cbOrphanIndication(&mdi.srcAddrInfo.address.Extended, stackId,
                                                                     rxTime);
                                    }
                                }
                            }
                        }
                        break;
                    }
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
                    case gpMacCore_CommandCoordinatorRealignment:
                    {
                        if(DIVERSITY_SCAN_ORPHAN_ORIGINATOR())
                        {
                            gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
                            // mdi.stackId not relevant here -> use maccore_globals->gpMacCore_pScanState->stackId
                            if(pdLoh.length == GP_MACCORE_COORDINATOR_REALIGNMENT_CMD_LEN &&
                               gpSched_UnscheduleEvent(MacCore_DoOrphanScan) && maccore_globals->gpMacCore_pScanState)
                            {
                                UInt8 temp8;
                                UInt16 temp16;
                                gpPd_ReadByteStream(pdLoh.handle, pdLoh.offset, 2, (UInt8*)&temp16);
                                LITTLE_TO_HOST_UINT16(&temp16);
                                gpMacCore_SetPanId(temp16, maccore_globals->gpMacCore_pScanState->stackId);
                                pdLoh.offset += 2;

                                gpPd_ReadByteStream(pdLoh.handle, pdLoh.offset, 2, (UInt8*)&temp16);
                                LITTLE_TO_HOST_UINT16(&temp16);
                                gpMacCore_SetCoordShortAddress(temp16, maccore_globals->gpMacCore_pScanState->stackId);
                                pdLoh.offset += 2;

                                gpPd_ReadByteStream(pdLoh.handle, pdLoh.offset, 1, (UInt8*)&temp8);
                                gpRxArbiter_SetStackChannel(temp8, maccore_globals->gpMacCore_pScanState->stackId);
                                pdLoh.offset += 1;

                                gpPd_ReadByteStream(pdLoh.handle, pdLoh.offset, 2, (UInt8*)&temp16);
                                LITTLE_TO_HOST_UINT16(&temp16);
                                gpMacCore_SetShortAddress(temp16, maccore_globals->gpMacCore_pScanState->stackId);

                                gpMacCore_SetCoordExtendedAddress(&mdi.srcAddrInfo.address.Extended,
                                                                  maccore_globals->gpMacCore_pScanState->stackId);

                                MacCore_HandleOrphanScanEnd(gpMacCore_ResultSuccess);
                            }
                        }
                        break;
                    }
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR

#ifdef GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT
                    case gpMacCore_CommandBeaconRequest: // Beacon request received
                    {
                        if(DIVERSITY_SCAN_ACTIVE_RECIPIENT())
                        {
                            // mdi.stackId not valid here (no src addressing) -> ignore
                            // according to the standard, beacon requests must have a broadcast panID address in them
                            if(mdi.dstAddrInfo.panId == GP_MACCORE_PANID_BROADCAST &&
                               pdLoh.length == GP_MACCORE_BEACON_REQUEST_CMD_LEN)
                            {
                                gpPd_Loh_t BeaconPd;
                                gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

                                maccore_globals->MacCore_BeaconRequestStackId = 0;
                                // initialize pd - allocate memory for beacons
                                if(MacCore_AllocatePdLoh(&BeaconPd))
                                {
                                    // check whether to transmit is done inside MacCore_TransmitBeacon()
                                    // the memory slot is released once all beacons have been transmitted
                                    MacCore_TransmitBeacon(&BeaconPd, rxInfo);
                                }
                            }
                        }
                        break;
                    }
#endif // GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT
                    default:
                    {
                        GP_LOG_PRINTF("Unknown cmd:%x", 0, command);
                        break;
                    }
                }
                gpPd_FreePd(pdLoh.handle);
            }
            break;
        }
        default:
        {
            gpPd_FreePd(pdLoh.handle);
            // GP_LOG_SYSTEM_PRINTF("unknown frametype %x", 2, mdi.frameType);
            break;
        }
    }
}


#ifdef GP_MACCORE_DIVERSITY_RX_WINDOWS
void gpMacCore_EnableRxWindows(UInt32 dutyCycleOnTime, UInt32 dutyCyclePeriod, UInt16 recurrenceAmount,
                               UInt32 startTime, gpMacCore_StackId_t stackId)
{
    UInt8 channel = gpMacCore_GetCurrentChannel(stackId);
    gpRxArbiter_EnableRxWindows(channel, dutyCycleOnTime, dutyCyclePeriod, recurrenceAmount, startTime, stackId);
}

void gpMacCore_DisableRxWindows(gpMacCore_StackId_t stackId)
{
    gpRxArbiter_DisableRxWindows(stackId);
}

void gpMacCore_EnableCsl(UInt16 dutyCyclePeriod, gpMacCore_StackId_t stackId)
{
    gpHal_EnableCsl(dutyCyclePeriod);
}

void gpMacCore_UpdateCslSampleTime(UInt32 nextCslSampleTime, gpMacCore_StackId_t stackId)
{
    gpHal_UpdateCslSampleTime(nextCslSampleTime);
}
#endif // GP_MACCORE_DIVERSITY_RX_WINDOWS

UInt32 gpMacCore_GetCurrentTimeUs(void)
{
    UInt32 now;
    HAL_TIMER_GET_CURRENT_TIME_1US(now);
    return now;
}

Int8 gpMacCore_GetRadioSensitivity(void)
{
    return gpHal_GetRadioSensitivity();
}

#ifdef GP_MACCORE_DIVERSITY_RX_WINDOWS
UInt8 gpMacCore_GetCslAccuracy(void)
{
    return gpHal_GetSleepClockAccuracy();
}

UInt8 gpMacCore_GetCslUncertainty(void)
{
    return gpHal_GetRxUncertainty();
}
#endif // GP_MACCORE_DIVERSITY_RX_WINDOWS
