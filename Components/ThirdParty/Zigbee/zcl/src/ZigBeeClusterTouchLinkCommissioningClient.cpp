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
 * $Header: $
 * $Change: $
 * $DateTime: $
 *
 */

#include <climits>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Compact15dot4.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "MemoryDevice.h"
#include "PersistentStorage.h"
#include "IEEE802154PHY.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "IEEE802154MACPIB.h"
#include "IEEE802154MAC.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeStorage.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeCommissioningAttributes.h"
#include "ZigBeeCommissioningAttributesEx.h"
#include "ZigBeeNetworkCommands.h"
#include "ZigBeeNetwork.h"
#include "ZigBeeClusterLibrary.h"
#include "ZigBeeDeviceProfile.h"
#include "ZigBeeDeviceObject.h"
#include "QorvoClusterGroupsClient.h"
#include "QorvoDeviceType_LUT.h"

#include "QorvoClusterTouchLinkCommissioningClient.h"
#include "QorvoClusterTouchLinkCommissioningSecurity.h"

#include "gpLog.h"
#include "gpReset.h"
#include "gpSched.h"
#include "gpRandom.h"
#include "gpMacCore.h"
#include "gpHal_MAC.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_UBIZPRO

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterTouchLinkCommissioningClient, CZigBeeClusterTouchlink)

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient attribute map
__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterTouchLinkCommissioningClient)
    __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_CLUSTER_ATTRIBUTES()
__COMPACT7B_END_ATTRIBUTE_MAP()

#define SecurityKey_api2buf(pDest, pSource, length, pIndex) generic_api2buf_1(pDest, pSource, pIndex, 16)     //MTK
/////////////////////////////////////////////////////////////////////////////
// CQorvoTouchLinkClientTargetInfo
CQorvoTouchLinkClientTargetInfo::CQorvoTouchLinkClientTargetInfo(void)
{
    targetStatus = statusFailure;
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoTouchLinkScanResponseData
CQorvoTouchLinkScanResponseData::CQorvoTouchLinkScanResponseData(void)
{
    m_qwExtended = UINT64_MAX;
    nRSSI = INT8_MIN;
    keyBitMask = CQorvoClusterTouchLinkCommissioningClient::keyUndefined;
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient

__COMPACT_WEAK unsigned char CQorvoClusterTouchLinkCommissioningClient::
    abNetworkKey[16] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,0x88,
        0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,0x00};
    
__COMPACT_WEAK const unsigned char CQorvoClusterTouchLinkCommissioningClient::
    abPrimaryScanChannelSet[8] = {11,11,11,11,11,15,20,25};

__COMPACT_WEAK const unsigned char CQorvoClusterTouchLinkCommissioningClient::
    abSecondaryScanChannelSet[12] = {12,13,14,16,17,18,19,21,22,23,24,26};

CQorvoClusterTouchLinkCommissioningClient::CQorvoClusterTouchLinkCommissioningClient
    (CZigBeeFoundationApplication &application,
        const unsigned char logicalType,
        unsigned short initiatorAddress,
        bool enableDeviceInfoRequest):
CZigBeeClusterTouchlink(application, enableDefaultBehavior|enableOnCentralizedNetworks),
    m_nDeviceType(logicalType),
    m_bEnableDeviceInfoRequest(enableDeviceInfoRequest),
    m_nFreeNetworkAddressRangeBegin(0x0001),
    m_nFreeNetworkAddressRangeEnd(0xFFF7),
    m_nFreeGroupIdRangeBegin(0x0001),
    m_nFreeGroupIdRangeEnd(0xFEFF)
{
    //initialize TouchLink commissioning status
    m_nTouchLinkStatus = statusStopped;
    //initialize TouchLink network update ID
    m_nNetworkUpdateId = 0;
    
    //set flag to Outbound(this is a cluster client)
    m_nFlags &= ~flagInbound;
    m_nFlags |= flagOutbound;
}

/////////////////////////////////////////////////////////////////////////////

extern CZigBeeApplicationSupport aps;

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient operation

void CQorvoClusterTouchLinkCommissioningClient::StartScanning(bool resetToFactoryNewRequest, bool forceSecondaryScan)
{
    // Set reset to factory new flag
    m_bResetToFactoryNew = resetToFactoryNewRequest;

    // Reset the scan response buffer
    resetScanResponse();

    // drop as device is not in stopped status
    if (m_nTouchLinkStatus != statusStopped)
    {
        return;
    }
    
    //Enable answering to Scan Responses
    Enable(true);
    
    //  enable receiver Rx in phy layer.
    aps.m_nwk.m_mac.m_phy.OnRequestSetTransceiverState(CIEEE802154PHY::transceiverReceiverEnabled);
    
    // Reset TouchLink(initiator) parameters
    m_nScanChannelIndex = 0;

    // Configure to scan primary channels
    m_bDoPrimaryScan = true;
    
    // configure to scan secondary channels if m_bResetToFactoryNew is enabled
    if(resetToFactoryNewRequest || forceSecondaryScan)
    {
        m_bForceSecondaryScan = true;
    }
    else
    {
        m_bForceSecondaryScan = false;
    }

    // init the current scan channel info, and set to use Primary scan channel set
    m_pCurrentScanChannelSet = (unsigned char*)&abPrimaryScanChannelSet[0];
    m_nCurrentScanChannelSize = sizeof(abPrimaryScanChannelSet);

    // Reset Target Info
    resetTargetInfo();
    
    // Backup current network channel if device is joined
    if(aps.m_nwk.m_nlme.IsJoined())
    {
        m_nCurrentNetworkChannel = aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel;
    }
    
    // Generate random transaction ID
    gpRandom_GetNewSequence(sizeof(m_nTransactionId),
        reinterpret_cast<uint8_t *>(&m_nTransactionId));
    
    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusScanRequest;
    
    // Launch commissioning state callback
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    
    //Start scan request loop
    LoopScanRequests(this);
    
    // Schedule the stop scan request loop for timeout if not exist
    if (!gpSched_ExistsEventArg(SessionTimeout, this))
    {
        unsigned int timeout = !m_bResetToFactoryNew ? scanTimeout : extendedScanTimeout;
        gpSched_ScheduleEventArg(MS_TO_US(timeout), SessionTimeout, this);
    }
    
    // Disable end device polling when Touchlink scanning starts
    if(aps.m_nwk.m_nlme.IsJoined())
    {
        aps.m_nwk.m_nlme.DisablePolling();
    }
}

void CQorvoClusterTouchLinkCommissioningClient::StopScanning(void)
{
    // Reject stop scanning if device is not scanning in progress/received scan response and before starting network
    if ((m_nTouchLinkStatus != statusScanRequest) && 
        (m_nTouchLinkStatus != statusScanResponseReceived) && 
        (m_nTouchLinkStatus != statusDetermindedPotentialTarget ) && 
        (m_nTouchLinkStatus != statusTargetStartNetwork) &&
        (m_nTouchLinkStatus != statusTargetStartJoining) &&
        (m_nTouchLinkStatus != statusInitiatorRejoining))
    {
        return;
    }
    
    //Unschedule the Scan Requests Loop if exist
    if (gpSched_ExistsEventArg(LoopScanRequests, this))
    {
        gpSched_UnscheduleEventArg(LoopScanRequests, this);
    }
    
    //Unschedule starting network if exist
    if (gpSched_ExistsEventArg(TargetStartNetwork, this))
    {
        gpSched_UnscheduleEventArg(TargetStartNetwork, this);
    }
    
    //Unschedule join router/join end device if exist
    if (gpSched_ExistsEventArg(TargetJoinNetwork, this))
    {
        gpSched_UnscheduleEventArg(TargetJoinNetwork, this);
    }
    
    // Unschedule the timeout if exist
    if (gpSched_ExistsEventArg(SessionTimeout, this))
    {
        gpSched_UnscheduleEventArg(SessionTimeout, this);
    }
    
    // Restore network channel
    aps.m_nwk.m_mac.m_phy.SetCurrentChannel(m_nCurrentNetworkChannel);
    
    //Disable answering to Scan Responses
    Enable(false);
    
    // Clear transaction Session ID
    m_nTransactionId = 0;
    
    // Clear extended Pan ID
    m_targetInfo.qwExtendedPanId.MSB =0;
    m_targetInfo.qwExtendedPanId.LSB =0;
    
    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusStopped;
    
    // Launch commissioning state callback
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
}

bool CQorvoClusterTouchLinkCommissioningClient::IsSessionActive(void)
{
    if (m_nTransactionId)
    {
        GP_LOG_PRINTF("TouchLink: Session is Active, sessionID: 0x%08x\n",0, m_nTransactionId);
        return true;
    }
    else
    {
        GP_LOG_PRINTF("TouchLink: Session is NOT Active\n",0);
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient receipt of server to client command handlers

void CQorvoClusterTouchLinkCommissioningClient::OnScanResponse(
    const CZigBeeAddress &destination, const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    bool isSchemeSupported = false;
    
    // drop if the payload length is not valid
    if (asdu.GetSize() < 23)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid Scan Response received, length = %d\n", 0, asdu.GetSize());
        return;
    }
    
    // drop as device is not in Scan Request status
    if (m_nTouchLinkStatus != statusScanRequest)
    {
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;
    CZigBeeTouchlinkScanResponse response;

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();
    
    // Retrieve the mandatory parameter from payload
    // -- transaction ID
    UInt32_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nSessionID), pData, 1, &index);
    // -- RSSI correction
    UInt8_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nRSSICorrection), pData, 1, &index);
    // -- ZigBee information
    UInt8_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nZigBeeInformation), pData, 1, &index);
    // -- TouchLink information
    UInt8_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nTouchlinkInformation), pData, 1, &index);
    // -- KeyBitmask
    UInt16_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nSupportedSchemes), pData, 1, &index);
    // -- Response ID
    UInt32_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nResponseID), pData, 1, &index);
    // -- Extended Pan ID
    UInt64_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_qwExtendedPANID), pData, 1, &index);
    // -- Network Update ID
    UInt8_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nNetworkUpdateID), pData, 1, &index);
    // -- Logical Channel
    UInt8_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nChannel), pData, 1, &index);
    // -- Pan ID
    UInt16_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_wPANID), pData, 1, &index);
    // -- Network Address
    UInt16_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_wNetworkAddress), pData, 1, &index);
    // -- Number of sub-devices
    UInt8_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nDeviceCount), pData, 1, &index);
    // -- Group ID count
    UInt8_buf2api((UInt8*)&response + offsetof(CZigBeeTouchlinkScanResponse, m_nTotalGroupCount), pData, 1, &index);
    
    // Check transaction ID is matched
    if (m_nTransactionId != response.m_nSessionID)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid transaction ID... Skipping Device\n",0);
        return;   // skip device and search for next one
    }
    
    // Check if channel is valid
    if (!((response.m_nChannel == 0) || (response.m_nChannel <= 26) || (response.m_nChannel >= 11)))
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid logical channel... Skipping Device\n",0);
        return;   // skip device and search for next one
    }
    
    // Skip target if device is factory new and target is end device
    if (!aps.m_nwk.m_nlme.IsJoined() && (response.m_nZigBeeInformation & typeEndDevice))
    {
        GP_LOG_SYSTEM_PRINTF("  Device is factory New and target is End Device, skip target\n ",0);

        return;   // skip target and search for next one
    }
    
    GP_LOG_PRINTF("TouchLink: local scheme: %04x\n",0, CZigBeeClusterTouchlink::nSupportedSchemes);
    GP_LOG_PRINTF("TouchLink: target scheme: %04x\n",0, response.m_nSupportedSchemes);
    unsigned short keyBitMask = keyUndefined;
    // Check and Store Key bitmask for a matched key scheme
    if (BIT_TST(CZigBeeClusterTouchlink::nSupportedSchemes,keyDevelopment) &&
        BIT_TST(response.m_nSupportedSchemes,keyDevelopment))
    {
        m_scannedTargetInfo.wKeyBitMask=keyDevelopment;
        keyBitMask = keyDevelopment;
        isSchemeSupported = true;
        GP_LOG_PRINTF("TouchLink: keybit = keyDevelopment\n",0);
    }
    if (BIT_TST(CZigBeeClusterTouchlink::nSupportedSchemes,keyPrecertification) &&
        BIT_TST(response.m_nSupportedSchemes,keyPrecertification))
    {
        m_scannedTargetInfo.wKeyBitMask=keyPrecertification;
        keyBitMask = keyPrecertification;
        isSchemeSupported = true;
        GP_LOG_PRINTF("TouchLink: keybit = keyPrecertification\n",0);
    }
    if (BIT_TST(CZigBeeClusterTouchlink::nSupportedSchemes,keyMaster) &&
        BIT_TST(response.m_nSupportedSchemes,keyMaster))
    {
        m_scannedTargetInfo.wKeyBitMask=keyMaster;
        keyBitMask = keyMaster;
        isSchemeSupported = true;
        GP_LOG_PRINTF("TouchLink: keybit = keyMaster\n",0);
    }
    if (!isSchemeSupported)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Key Mismatch... Skipping Device\n",0);
        // skip device and search for next one
        return;
    }
    m_scannedTargetInfo.m_nChannel = response.m_nChannel;
    
    // -- Target Device Type
    if (response.m_nZigBeeInformation & typeRouter) 
    {
        m_scannedTargetInfo.m_nDeviceType = typeRouter;
    }
    else if (response.m_nZigBeeInformation & typeEndDevice) 
    {
        m_scannedTargetInfo.m_nDeviceType = typeEndDevice;
    }
    else
    {
        m_scannedTargetInfo.m_nDeviceType = typeCoordinator;
    }
    // Launch commissioning state callback
    m_nTouchLinkStatus = statusScanResponseReceived;
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);

    // Verify if there was already a scan response received on this channel with a higher RSSI
    uint8_t nCorrelation;
    int8_t nRSSI;
    aps.m_nwk.m_mac.m_phy.GetRSSIAndCorrelation(asdu, nRSSI, nCorrelation);
    if(nRSSI > m_storedScanRsp.nRSSI)
    {
        // Store the scan response for this channel
        storeScanResponse(response, source.m_qwExtended, nRSSI, keyBitMask, aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel);
    }
}

void CQorvoClusterTouchLinkCommissioningClient::OnDeviceInformationResponse(
    const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    // drop if the payload length is not valid
    if (asdu.GetSize() < 7)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid Device Information Response, length = %d\n", 0, asdu.GetSize());
        return;
    }

    // drop as device is not in Scan Response received status
    if (m_nTouchLinkStatus != statusDetermindedPotentialTarget)
    {
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    UInt8* pData;

    UInt32 transactionId = 0;

    // Retrieve the payload
    pData = (UInt8*)asdu.GetData();

    // -- transaction ID
    UInt32_buf2api(&transactionId, pData, 1, &index);
    
    // Check transaction ID is matched
    if (m_nTransactionId != transactionId)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid transaction ID... Skipping Device\n",0);
        return;   // skip device and search for next one
    }

    // -- Number of sub-devices
    UInt8_buf2api(&m_targetInfo.devInfoRsp.m_nCount, pData, 1, &index);
    // -- Start Index
    UInt8_buf2api(&m_targetInfo.devInfoRsp.m_nStartIndex, pData, 1, &index);
    // -- Device Info. Record Count
    UInt8_buf2api(&m_targetInfo.devInfoRsp.m_nRecords, pData, 1, &index);
    for( uint8_t i=0 ; i<m_targetInfo.devInfoRsp.m_nRecords; i++)
    {
        UInt64_buf2api(&m_targetInfo.devInfoRec[i].m_qwAddress, pData, 1, &index);
        UInt8_buf2api(&m_targetInfo.devInfoRec[i].m_info.m_nEndpoint, pData, 1, &index);
        UInt16_buf2api(&m_targetInfo.devInfoRec[i].m_info.m_wProfileID, pData, 1, &index);
        UInt16_buf2api(&m_targetInfo.devInfoRec[i].m_info.m_wDeviceID, pData, 1, &index);
        UInt8_buf2api(&m_targetInfo.devInfoRec[i].m_info.m_nVersion, pData, 1, &index);
        UInt8_buf2api(&m_targetInfo.devInfoRec[i].m_info.m_nGroupCount, pData, 1, &index);
        UInt8_buf2api(&m_targetInfo.devInfoRec[i].m_nSortOrder, pData, 1, &index);
    }
    
    // Send out Identify request to selected device
    IdentifyRequest(m_targetInfo.targetAddress, m_nTransactionId, identifyDuration);

    if (!m_bResetToFactoryNew)
    {
        ConnectTouchlink();
    }
    else
    {
        // Delayed triggering of reset to factory new request
        gpSched_ScheduleEventArg(MS_TO_US(100), TargetResetToFactoryNew, this);
    }
}

void CQorvoClusterTouchLinkCommissioningClient::bindEndpoints(void)
{
    /*  STEP 25: If the initiator is a simple device, it SHALL establish binding links in the
        binding table to the target. Conversely, if the initiator is not a simple
        device, it MAY establish binding links in the binding table to the target. If
        binding links are to be established, the initiator SHALL then, based on the
        endpoint and device identifier information received in the scan response
        and/or device information response inter-PAN command frames, establish
        binding links in the binding table for matching client/server clusters on the
        initiator and the corresponding server/client clusters on the target. The
        initiator sets bdbCommissioningStatus to SUCCESS, sets
        bdbNodeIsOnANetwork to TRUE and it SHALL terminate the touchlink
        procedure for an initiator.
    */
    devType_t* targetDevType;
    // For every device in the Target
    for( UInt8 i=0 ; i<m_targetInfo.devInfoRsp.m_nRecords; i++)
    {
        // Lookup required Cluster list
        targetDevType = DeviceType_LUT_getDeviceTypebyId(m_targetInfo.devInfoRec[i].m_info.m_wDeviceID);
        if(targetDevType == NULL)
        {
            GP_LOG_PRINTF("No target device type found for device ID: %d.\n",0,m_targetInfo.devInfoRec[i].m_info.m_wDeviceID);
            return;
        }
        for( UInt8 j=0 ; j<targetDevType->mandatoryServerClusterSize; j++)
        {       
            CZigBeeAllFoundationApplications applications(aps);
            // Loop over all local endpoints
            while(applications.Advance())
            {
                // Compare against local clusters
                if(applications->FindCluster(targetDevType->mandatoryServerClusters[j], CZigBeeCluster::flagOutbound) != NULL)
                {
                    GP_LOG_PRINTF("Binding cluster 0x%04x between Target endpoint 0x%04x and initiator endpoint 0x%04x\n",0,targetDevType->mandatoryServerClusters[j], m_targetInfo.devInfoRec[i].m_info.m_nEndpoint,applications->m_bEndpoint);
                    CZigBeeAddress targetAddress = CZigBeeAddress(CIEEE802154Address(m_targetInfo.devInfoRec[i].m_qwAddress));
                    targetAddress.SetEndpoint(m_targetInfo.devInfoRec[i].m_info.m_nEndpoint);
                    //  Create a binding entry for each matched cluster.
                    aps.m_apsme.Bind(applications->m_bEndpoint, targetDevType->mandatoryServerClusters[j] ,targetAddress);
                }
                else
                {
                    GP_LOG_PRINTF("No match for cluster 0x%04x\n",0,targetDevType->mandatoryServerClusters[j]);
                }
            }        
        }
    }
}

void CQorvoClusterTouchLinkCommissioningClient::ConnectTouchlink(void)
{
    GP_LOG_SYSTEM_PRINTF("UpdateID target: %i, initiator: %i\n", 0, (int8_t)m_storedScanRsp.scanRsp.m_nNetworkUpdateID, (int8_t)aps.m_nwk.m_nlme.m_nib.m_nUpdateID);
    GP_LOG_SYSTEM_PRINTF("PANID target: %x, initiator: %x\n", 0, m_storedScanRsp.scanRsp.m_wPANID, aps.m_nwk.m_mac.m_pib.m_wPANID);
    if (m_storedScanRsp.scanRsp.m_wPANID == aps.m_nwk.m_mac.m_pib.m_wPANID)
    {
        
        if (((int8_t)m_storedScanRsp.scanRsp.m_nNetworkUpdateID) < ((int8_t)aps.m_nwk.m_nlme.m_nib.m_nUpdateID))
        {
            // STEP 9: If the network update identifier field of the scan response command frame is lower than nwkUpdateId 
            // (i.e., the target has missed a channel change), the initiator SHALL generate and transmit a touchlink
            // commissioning cluster network update request command frame to the target with the network update 
            // identifier field set to nwkUpdateId and the logical channel field set to the current operating 
            // channel of the initiator.
            NetworkUpdateRequest(m_targetInfo.targetAddress,
                                 m_nTransactionId, 
                                 m_targetInfo.qwExtendedPanId,
                                 aps.m_nwk.m_nlme.m_nib.m_nUpdateID, 
                                 m_targetInfo.m_nChannel,
                                 aps.m_nwk.m_mac.m_pib.m_wPANID, 
                                 m_storedScanRsp.scanRsp.m_wNetworkAddress);
        }
        else
        {
            if (((int8_t)m_storedScanRsp.scanRsp.m_nNetworkUpdateID) > ((int8_t)aps.m_nwk.m_nlme.m_nib.m_nUpdateID))
            {
                // STEP 9: If the network update identifier field of the scan response command frame is higher than 
                // nwkUpdateId (i.e., the initiator has missed a channel change), the initiator SHALL set 
                // nwkUpdateId and its current operating channel to the values of the network update identifier 
                // and logical channel fields, respectively, from the scan response command frame. 
                CZigBeeCommissioningAttributesEx attributes;
                
                // Retrieve the Startup Attribute
                attributes.Load();

                // Update the nwkUpdateId and current operating channel from the scan response command frame.
                attributes.m_nChannel = m_storedScanRsp.scanRsp.m_nChannel;
                attributes.m_nNetworkUpdateID = m_storedScanRsp.scanRsp.m_nNetworkUpdateID;
               
                // Store and commit the Startup Attribute
                attributes.Store();
                attributes.Commit(aps);
  
            }
        }
        
        // If the logical type field of the node descriptor for the initiator is equal to 0b010 (Zigbee end device), 
        // initiator SHALL continue from step 19; otherwise, the initiator SHALL continue from step 25.
        if (aps.m_nwk.m_nlme.IsEndDevice())
        {
            // The initiator SHALL perform a network rejoin request. To do this, the initiator issues the NLME-JOIN.request 
            // primitive with the ExtendedPANId parameter set to the extended PAN identifier of the selected network, 
            //the RejoinNetwork parameter set to 0x02 (the node is joining the network using the NWK rejoining procedure), 
            // the ScanChannels parameter set to 0x00000000, the ScanDuration parameter set to 0x00, 
            // the CapabilityInformation set appropriately for the node and the SecurityEnable parameter set to TRUE. 
            // On receipt of the NLME-1313 JOIN.confirm primitive from the NWK layer, the initiator is notified 
            // of the status of the request for a network rejoin. The initiator SHALL then continue from step 25.
            // Stop current running commissioning schedular before start
            aps.m_apsme.ScheduleNextStartupTimer(CZigBeeApplicationSupportManagement::startupAbort);

            // update stack's rejoin mode
            if(aps.m_apsme.IsDistributedSecurityNetwork())
            {
                aps.m_apsme.m_rejoinStartupMode = CZigBeeApplicationSupportManagement::startupAllowSecureRejoin;
            }
            else
            {
                aps.m_apsme.m_rejoinStartupMode = CZigBeeApplicationSupportManagement::startupAllowRejoining;
            }

            CZigBeeCommissioningAttributesEx attributes;
            
            // Retrieve the Startup Attribute
            attributes.Load();
            // Define the extended PANID the End Device should rejoin to
            attributes.m_qwExtendedPANID = m_storedScanRsp.scanRsp.m_qwExtendedPANID;

            // Store and commit the Startup Attribute
            attributes.Store();
            attributes.Commit(aps);

            m_nTouchLinkStatus = statusInitiatorRejoining;
            OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
            // launch stack startup with flag
            aps.m_apsme.Startup(aps.m_apsme.m_rejoinStartupMode);
            return;
        }
        else
        {
            // When the Initiator is a router, no rejoin is required
            m_nTouchLinkStatus = statusTargetNetworkJoined;
            OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
            m_targetInfo.targetStatus = statusSuccess;
            bindEndpoints();
        }
        StopScanning();
        // Restore normal poll
        aps.m_nwk.m_nlme.EnablePolling(aps.m_nwk.m_nlme.m_nib.normalPollInterval);
        
        // Update TouchLink commissioning status
        m_nTouchLinkStatus = statusStopped;
        OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    }
    else
    {
        //STEP 10: Verify if initiator is on a centralized network
        if(aps.m_nwk.m_nlme.IsJoined() && !aps.m_apsme.IsDistributedSecurityNetwork())
        {
            StopScanning();
            // Restore normal poll
            aps.m_nwk.m_nlme.EnablePolling(aps.m_nwk.m_nlme.m_nib.normalPollInterval);
            // Update TouchLink commissioning status
            m_nTouchLinkStatus = statusStopped;
            OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
            return;
        }
        //STEP 11: Verify if initiator is address assignment capable
        if(!(m_nInitiatorTouchlinkInformation & CZigBeeClusterTouchlink::addressAssignmentCapable))
        {
            StopScanning();
            // Restore normal poll
            aps.m_nwk.m_nlme.EnablePolling(aps.m_nwk.m_nlme.m_nib.normalPollInterval);
            // Update TouchLink commissioning status
            m_nTouchLinkStatus = statusStopped;
            OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
            return;
        }

        //STEP 12
        // if device is factory new, start the network, else join the network
        if (!aps.m_nwk.m_nlme.IsJoined())
        {
            //STEP 13: Verify if initiator is a router
            if (!aps.m_nwk.m_nlme.IsRouter())
            {
                //STEP 14: Verify if Target is a router
                if(m_targetInfo.m_nDeviceType == typeRouter)
                {
                    //STEP 15: initiate starting network after few seconds
                    gpSched_ScheduleEventArg(MS_TO_US(networkStartJoinDelay)
                        ,TargetStartNetwork,this);
                    return;
                }
                else
                {
                    m_nTouchLinkStatus=statusStopped;
                    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
                    return;
                }
            }
            else
            {
                //STEP 20 & 21: Start a new network
                //Not supported
                m_nTouchLinkStatus=statusStopped;
                OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
                return;
            }
            
        }
        // STEP 22: initiate join router/join end device after few seconds
        gpSched_ScheduleEventArg(MS_TO_US(networkStartJoinDelay)
            ,TargetJoinNetwork,this);
    }
}

void CQorvoClusterTouchLinkCommissioningClient::OnNetworkStartResponse(
    const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    // drop if the payload length is not valid
    if (asdu.GetSize() < 17)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid Network Start Response, length = %d\n", 0, asdu.GetSize());
        return;
    }

    // drop as device is not in target start network status
    if (m_nTouchLinkStatus != statusTargetStartNetwork)
    {
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    UInt8* pData;
    
    UInt32 transactionId;

    // Retrieve the payload
    pData = (UInt8*)asdu.GetData();
    
    // -- transaction ID
    UInt32_buf2api(&transactionId, pData, 1, &index);
    // -- Status
    UInt8_buf2api(&m_targetInfo.targetStatus, pData, 1, &index);
    // -- Extended Pan ID
    UInt64_buf2api((UInt8*)&m_targetInfo.qwExtendedPanId, pData, 1, &index);
    // -- Network Update ID
    UInt8_buf2api(&m_nNetworkUpdateId, pData, 1, &index);
    // -- Logical Channel
    UInt8_buf2api(&m_targetInfo.m_nChannel, pData, 1, &index);
    // -- Pan ID
    UInt16_buf2api(&m_targetInfo.targetAddress.m_wPANID, pData, 1, &index);

    /* 
      STEP 16: The initiator SHALL then enable its receiver and wait for at most  bdbcRxWindowDuration seconds
      or until a corresponding network start response inter-PAN command frame is received from the intended target 
      with the same inter-PAN transaction identifier field matching that used by the initiator in its scan request command frame.
    */
    
    // Check transaction ID is matched
    if (m_nTransactionId != transactionId)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid transaction ID... Skipping Device\n",0);
        return;   // skip device and search for next one
    }
    
    // Confirm source MAC address
    if (m_storedScanRsp.m_qwExtended != source.m_qwExtended )
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Received MAC address does not match target\n",0);
        /*
          According to test speciciation, proceed until time out:

          After bdbcTLRxWindowDuration touchlink fails:
          (DUT is ZED): DUT does NOT become operational on the network with the parameters
          indicated in the network start/join router request.
        */
        return;
    }

    /*
      STEP 16:if a corresponding network start response inter-PAN command frame is received within  bdbcRxWindowDuration seconds,
      but with a non-zero value in the Status  parameter, the initiator sets bdbCommissioningStatus to NO_NETWORK
      and it SHALL terminate the touchlink procedure for an initiator.
    */
    if (m_targetInfo.targetStatus != statusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Target failed to form a network\n",0);
        // Clear transaction Session ID
        m_nTransactionId = 0;
        //Update TouchLink commissioning status
        m_nTouchLinkStatus = statusStopped;
        // Callback update to application layer
        OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
        return;
    }

    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusTargetNetworkStarted;
    
    // Callback update to application layer
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    
    // Update device operational channel
    aps.m_nwk.m_mac.m_phy.SetCurrentChannel(m_targetInfo.m_nChannel);

    // Store network channel
    m_nCurrentNetworkChannel = aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel;

    // Restore normal poll
    aps.m_nwk.m_nlme.EnablePolling(aps.m_nwk.m_nlme.m_nib.normalPollInterval);
    
    // Schedule to apply the network parameter to join to new network
    gpSched_ScheduleEventArg(MS_TO_US(applyNetworkDelay),ApplyNetworkParams,this);
}

void CQorvoClusterTouchLinkCommissioningClient::RestartStack()
{
        //Trigger rejoining via a stack restart
        aps.m_apsme.ScheduleNextStartupTimer(CZigBeeApplicationSupportManagement::startupAbort);
        // update stack's rejoin mode
        if(aps.m_apsme.IsDistributedSecurityNetwork())
        {
            aps.m_apsme.m_rejoinStartupMode = CZigBeeApplicationSupportManagement::startupAllowSecureRejoin;
        }
        else
        {
            aps.m_apsme.m_rejoinStartupMode = CZigBeeApplicationSupportManagement::startupAllowRejoining;
        }
        // launch stack startup with flag - Secure rejoin
        aps.m_apsme.Startup(aps.m_apsme.m_rejoinStartupMode);
}


void CQorvoClusterTouchLinkCommissioningClient::OnNetworkJoinRouterResponse(
    const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    // drop if the payload length is not valid
    if (asdu.GetSize() < 5)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid Network Join Router Response, length = %d\n", 0, asdu.GetSize());
        return;
    }
    
    // drop as device is not in target join network status
    if (m_nTouchLinkStatus != statusTargetStartJoining)
    {
        return;
    }
    
    //Get Data from payload 
    UInt16 index;
    UInt8* pData;
    
    UInt32 transactionId;
    
    // Retrieve the payload
    pData = (UInt8*)asdu.GetData();
    index = 0;
    
    // -- transaction ID
    UInt32_buf2api(&transactionId, pData, 1, &index);
    // -- Status
    UInt8_buf2api(&m_targetInfo.targetStatus, pData, 1, &index);
    
    /* 
      STEP 24: The initiator SHALL then enable its receiver and wait for at most bdbcRxWindowDuration seconds
      or until a corresponding response inter-PAN command frame is received from the intended target
      with the same inter-PAN transaction identifier field matching that used by the initiator
      in its scan request command frame.
    */

    // Check transaction ID is matched
    if (m_nTransactionId != transactionId)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid transaction ID... Skipping Device\n",0);
        return;   // skip device and search for next one
    }
    
    // Confirm source MAC address
    if (m_storedScanRsp.m_qwExtended != source.m_qwExtended )
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Received MAC address does not match target\n",0);
        /*
          According to test speciciation, proceed until time out:
          
          After bdbcTLRxWindowDuration touchlink fails:
          (DUT is ZED): DUT does NOT become operational on the network with the parameters
          indicated in the network start/join router request.
        */
        return;
    }

    /*
      STEP 24: If a corresponding response inter-PAN command frame is not received within bdbcRxWindowDuration seconds
      or if a corresponding response inter-PAN command frame is received within bdbcRxWindowDuration seconds 
      but with a non-zero value in the Status parameter, the initiator sets bdbCommissioningStatus to TARGET_FAILURE 
      and it SHALL terminate the touchlink procedure for an initiator.
    */
    if (m_targetInfo.targetStatus != statusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Target failed to form a network\n",0);
        // Clear transaction Session ID
        m_nTransactionId = 0;
        //Update TouchLink commissioning status
        m_nTouchLinkStatus = statusStopped;
        // Callback update to application layer
        OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
        return;
    }
    
    // Update device operational channel
    aps.m_nwk.m_mac.m_phy.SetCurrentChannel(m_nCurrentNetworkChannel);
    
    // Restore normal poll
    aps.m_nwk.m_nlme.EnablePolling(aps.m_nwk.m_nlme.m_nib.normalPollInterval);
    
    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusTargetNetworkJoined;
    
    // Callback update to application layer
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    
    // Clear transaction Session ID as session is completed
    m_nTransactionId = 0;
    
    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusStopped;

    // Callback update to application layer
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);

    //Check if we have parent link and if not, restart stack
    if(aps.m_apsme.IsStartupRequired() && aps.m_apsme.HasStartedOnce())
    {
        gpSched_ScheduleEvent(MS_TO_US(networkStartJoinDelay),RestartStack);
    }
}

void CQorvoClusterTouchLinkCommissioningClient::OnNetworkJoinEndDeviceResponse(
    const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    // drop if the payload length is not valid
    if (asdu.GetSize() < 5)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid Network Join Router Response, length = %d\n", 0, asdu.GetSize());
        return;
    }
    
    // drop as device is not in target join network status
    if (m_nTouchLinkStatus != statusTargetStartJoining)
    {
        return;
    }
    
    //Get Data from payload 
    UInt16 index;
    UInt8* pData;
    
    UInt32 transactionId;
    
    // Retrieve the payload
    pData = (UInt8*)asdu.GetData();
    index = 0;
    
    // -- transaction ID
    UInt32_buf2api(&transactionId, pData, 1, &index);
    // -- Status
    UInt8_buf2api(&m_targetInfo.targetStatus, pData, 1, &index);
    
    // Check transaction ID is matched
    if (m_nTransactionId != transactionId)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid transaction ID... Skipping Device\n",0);
        return;   // skip device and search for next one
    }
    
    // Update device operational channel
    aps.m_nwk.m_mac.m_phy.SetCurrentChannel(m_nCurrentNetworkChannel);
    
    // Restore normal poll
    aps.m_nwk.m_nlme.EnablePolling(aps.m_nwk.m_nlme.m_nib.normalPollInterval);
    
    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusTargetNetworkJoined;
    
    // Callback update to application layer
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    
    // Clear transaction Session ID as session is completed
    m_nTransactionId = 0;
    
    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusStopped;

    // Callback update to application layer
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    
}

void CQorvoClusterTouchLinkCommissioningClient::OnEndpointInformation(
    const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    // drop if the payload length is not valid
    if (asdu.GetSize() < 16)
    {
        GP_LOG_SYSTEM_PRINTF("TouchLink: Invalid Endpoint Information, length = %d\n", 0, asdu.GetSize());
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    UInt8* pData;

    MACAddress_t ieeeAddress;
    UInt16 networkAddress;
    UInt8 endpointId;
    UInt16 profileId;
    UInt16 deviceId;
    UInt8 version;
    
    // Retrieve the payload
    pData = (UInt8*)asdu.GetData();
    
    // -- IEEE Address
    UInt64_buf2api((UInt8*)&ieeeAddress, pData, 1, &index);
    // -- network Address
    UInt16_buf2api(&networkAddress, pData, 1, &index);
    // -- endpoint
    UInt8_buf2api(&endpointId, pData, 1, &index);
    // -- profile
    UInt16_buf2api(&profileId, pData, 1, &index);
    // -- device ID
    UInt16_buf2api(&deviceId, pData, 1, &index);
    // -- version
    UInt8_buf2api(&version, pData, 1, &index);

    // Transmit a default response frame if needed
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterTouchLinkCommissioningClient::OnGetGroupIdentifiersResponse(
    const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status = statusUnsupportedClusterCommand;
#if 0
    UInt16 index = 0;
    UInt8* pData;

    UInt8 total;
    UInt8 startIndex;
    UInt8 count;
    GroupInformationRecord groupInformationRecordList;

    pData = (UInt8*)asdu.GetData();

    UInt8_buf2api(&total, pData, 1, &index);
    UInt8_buf2api(&startIndex, pData, 1, &index);
    UInt8_buf2api(&count, pData, 1, &index);
    GroupInformationRecord_buf2api(&groupInformationRecordList, pData, 1, &index);

    /* Behavior */
#endif
    // Transmit a default response frame if needed
    TransmitDefaultResponse(destination, source, header, status);
}

void CQorvoClusterTouchLinkCommissioningClient::OnGetEndpointListResponse(
    const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status = statusUnsupportedClusterCommand;
#if 0
    UInt16 index = 0;
    UInt8* pData;

    UInt8 total;
    UInt8 startIndex;
    UInt8 count;
    EndpointInformationRecord endpointInformationRecordList;

    pData = (UInt8*)asdu.GetData();

    UInt8_buf2api(&total, pData, 1, &index);
    UInt8_buf2api(&startIndex, pData, 1, &index);
    UInt8_buf2api(&count, pData, 1, &index);
    EndpointInformationRecord_buf2api(&endpointInformationRecordList, pData, 1, &index);

    /* Behavior */
#endif
    // Transmit a default response frame if needed
    TransmitDefaultResponse(destination, source, header, status);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient private client command API

void CQorvoClusterTouchLinkCommissioningClient::ScanRequest(
    const CZigBeeAddress &destination, unsigned int transactionId,
    unsigned char zigbeeInformation, unsigned char touchLinkInformation)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandScanRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    
    asdu.Append(&transactionId, sizeof(unsigned int));
    asdu.Append(&zigbeeInformation, sizeof(unsigned char));
    asdu.Append(&touchLinkInformation, sizeof(unsigned char));
    
    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::DeviceInformationRequest(
    const CZigBeeAddress &destination, unsigned int transactionId,
    unsigned char startIndex)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandDeviceInformationRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
                  
    asdu.Append(&transactionId, sizeof(unsigned int));
    asdu.Append(&startIndex, sizeof(unsigned char));

    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::IdentifyRequest(
    const CZigBeeAddress &destination, unsigned int transactionId,
    unsigned short identifyDuration)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandIdentifyRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
                  
    asdu.Append(&transactionId, sizeof(unsigned int));
    asdu.Append(&identifyDuration, sizeof(unsigned short));

    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::ResetToFactoryNewRequest(
    const CZigBeeAddress &destination, unsigned int transactionId)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandResetToFactoryNewRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
                  
    asdu.Append(&transactionId, sizeof(unsigned int));
    
    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::NetworkStartRequest(
    const CZigBeeAddress &destination, unsigned int transactionId,
    MACAddress_t extendedPanId, unsigned char keyIndex,
    SecKey128Bit_t encryptedNetworkKey, unsigned char logicalChannel,
    unsigned short panId, unsigned short networkAddress,
    unsigned short groupIdBegin, unsigned short groupIdEnd,
    unsigned short freeNetworkAddressRangeBegin, 
    unsigned short freeNetworkAddressRangeEnd, 
    unsigned short freeGroupIdentifierRangeBegin,
    unsigned short freeGroupIdentifierRangeEnd, MACAddress_t initiatorIeeeAddress,
    unsigned short initiatorNetworkAddress)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandNetworkStartRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  56);
                  
    asdu.Append(&transactionId, sizeof(unsigned int));
    asdu.Append(&extendedPanId, sizeof(MACAddress_t));
    asdu.Append(&keyIndex, sizeof(unsigned char));
    asdu.Append(&encryptedNetworkKey.keyByte, sizeof(SecKey128Bit_t));
    asdu.Append(&logicalChannel, sizeof(unsigned char));
    asdu.Append(&panId, sizeof(unsigned short));
    asdu.Append(&networkAddress, sizeof(unsigned short));
    asdu.Append(&groupIdBegin, sizeof(unsigned short));
    asdu.Append(&groupIdEnd, sizeof(unsigned short));
    asdu.Append(&freeNetworkAddressRangeBegin, sizeof(unsigned short));
    asdu.Append(&freeNetworkAddressRangeEnd, sizeof(unsigned short));
    asdu.Append(&freeGroupIdentifierRangeBegin, sizeof(unsigned short));
    asdu.Append(&freeGroupIdentifierRangeEnd, sizeof(unsigned short));
    asdu.Append(&initiatorIeeeAddress, sizeof(MACAddress_t));
    asdu.Append(&initiatorNetworkAddress, sizeof(unsigned short));
    
    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::NetworkJoinRouterRequest(
    const CZigBeeAddress &destination,
    unsigned int transactionId, MACAddress_t extendedPanId,
    unsigned char keyIndex, SecKey128Bit_t encryptedNetworkKey,
    unsigned char networkUpdateId, unsigned char logicalChannel,
    unsigned short panId, unsigned short networkAddress,
    unsigned short groupIdBegin, unsigned short groupIdEnd, 
    unsigned short freeNetworkAddressRangeBegin, unsigned short freeNetworkAddressRangeEnd,
    unsigned short freeGroupIdentifierRangeBegin, unsigned short freeGroupIdentifierRangeEnd)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandNetworkJoinRouterRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  47);
                  
    asdu.Append(&transactionId, sizeof(unsigned int));
    asdu.Append(&extendedPanId, sizeof(MACAddress_t));
    asdu.Append(&keyIndex, sizeof(unsigned char));
    asdu.Append(&encryptedNetworkKey.keyByte, sizeof(SecKey128Bit_t));
    asdu.Append(&networkUpdateId, sizeof(unsigned char));
    asdu.Append(&logicalChannel, sizeof(unsigned char));
    asdu.Append(&panId, sizeof(unsigned short));
    asdu.Append(&networkAddress, sizeof(unsigned short));
    asdu.Append(&groupIdBegin, sizeof(unsigned short));
    asdu.Append(&groupIdEnd, sizeof(unsigned short));
    asdu.Append(&freeNetworkAddressRangeBegin, sizeof(unsigned short));
    asdu.Append(&freeNetworkAddressRangeEnd, sizeof(unsigned short));
    asdu.Append(&freeGroupIdentifierRangeBegin, sizeof(unsigned short));
    asdu.Append(&freeGroupIdentifierRangeEnd, sizeof(unsigned short));

    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::NetworkJoinEndDeviceRequest(
    const CZigBeeAddress &destination,
    unsigned int transactionId, MACAddress_t extendedPanId,
    unsigned char keyIndex, SecKey128Bit_t encryptedNetworkKey,
    unsigned char networkUpdateId, unsigned char logicalChannel,
    unsigned short panId, unsigned short networkAddress,
    unsigned short groupIdBegin, unsigned short groupIdEnd, 
    unsigned short freeNetworkAddressRangeBegin, unsigned short freeNetworkAddressRangeEnd,
    unsigned short freeGroupIdentifierRangeBegin, unsigned short freeGroupIdentifierRangeEnd)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandNetworkJoinEndDeviceRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  47);
                  
    asdu.Append(&transactionId, sizeof(unsigned int));
    asdu.Append(&extendedPanId, sizeof(MACAddress_t));
    asdu.Append(&keyIndex, sizeof(unsigned char));
    asdu.Append(&encryptedNetworkKey.keyByte, sizeof(SecKey128Bit_t));
    asdu.Append(&networkUpdateId, sizeof(unsigned char));
    asdu.Append(&logicalChannel, sizeof(unsigned char));
    asdu.Append(&panId, sizeof(unsigned short));
    asdu.Append(&networkAddress, sizeof(unsigned short));
    asdu.Append(&groupIdBegin, sizeof(unsigned short));
    asdu.Append(&groupIdEnd, sizeof(unsigned short));
    asdu.Append(&freeNetworkAddressRangeBegin, sizeof(unsigned short));
    asdu.Append(&freeNetworkAddressRangeEnd, sizeof(unsigned short));
    asdu.Append(&freeGroupIdentifierRangeBegin, sizeof(unsigned short));
    asdu.Append(&freeGroupIdentifierRangeEnd, sizeof(unsigned short));
    
    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::NetworkUpdateRequest(
    const CZigBeeAddress &destination,
    unsigned int transactionId, MACAddress_t extendedPanId,
    unsigned char networkUpdateId, unsigned char logicalChannel,
    unsigned short panId, unsigned short networkAddress)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandNetworkUpdateRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  18);
                  
    asdu.Append(&transactionId, sizeof(unsigned int));
    asdu.Append(&extendedPanId, sizeof(MACAddress_t));
    asdu.Append(&networkUpdateId, sizeof(unsigned char));
    asdu.Append(&logicalChannel, sizeof(unsigned char));
    asdu.Append(&panId, sizeof(unsigned short));
    asdu.Append(&networkAddress, sizeof(unsigned short));
    
    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient public client command API
void CQorvoClusterTouchLinkCommissioningClient::GetGroupIdentifiersRequest(
    const CZigBeeAddress &destination, unsigned char startIndex)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandGetGroupIdentifiersRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
                  
    asdu.Append(&startIndex, sizeof(unsigned char));
    
    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

void CQorvoClusterTouchLinkCommissioningClient::GetEndpointListRequest(
    const CZigBeeAddress &destination, unsigned char startIndex)
{
    CPacket asdu;
    
    // Create ZCL command frame, including payload
    CreateRequest(asdu,
                  static_cast<unsigned char>(commandGetEndpointListRequest),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
                  
    asdu.Append(&startIndex, sizeof(unsigned char));
    
    // Transmit ZCL frame via APS.INTRP-DATA.request
    aps.OnRequestDataStub(destination, zllProfileID, id, asdu);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient private function

void CQorvoClusterTouchLinkCommissioningClient::resetScanResponse(void)
{
    m_storedScanRsp.m_qwExtended = UINT64_MAX;
    m_storedScanRsp.nRSSI = INT8_MIN;
    m_storedScanRsp.keyBitMask = keyUndefined;
}

void CQorvoClusterTouchLinkCommissioningClient::storeScanResponse(CZigBeeTouchlinkScanResponse scanRsp, uint64_t m_qwExtended, int8_t nRSSI, unsigned short keyBitMask, uint8_t scanChannel)
{
    m_storedScanRsp.scanRsp = scanRsp;
    m_storedScanRsp.m_qwExtended = m_qwExtended;
    m_storedScanRsp.nRSSI = nRSSI;
    m_storedScanRsp.keyBitMask= keyBitMask;
    m_storedScanRsp.m_scanChannel = scanChannel;
}

void CQorvoClusterTouchLinkCommissioningClient::resetTargetInfo(void)
{
    // Reset and initialze all parametergi
    m_scannedTargetInfo.targetAddress.m_qwExtended = UINT64_MAX;
    m_scannedTargetInfo.wKeyBitMask = keyUndefined;
    m_scannedTargetInfo.qwExtendedPanId.MSB = 0xFFFFFFFF;
    m_scannedTargetInfo.qwExtendedPanId.LSB = 0xFFFFFFFF;
    m_scannedTargetInfo.m_nChannel = 0;
    m_scannedTargetInfo.m_nDeviceType = 0xFF;
    m_scannedTargetInfo.m_nInitiatorNetworkAddress = 0xFFFF;
    m_scannedTargetInfo.targetStatus = CQorvoTouchLinkClientTargetInfo::statusInvalid; 
    m_targetInfo.targetAddress.m_qwExtended = UINT64_MAX;
    m_targetInfo.wKeyBitMask = keyUndefined;
    m_targetInfo.qwExtendedPanId.MSB = 0xFFFFFFFF;
    m_targetInfo.qwExtendedPanId.LSB = 0xFFFFFFFF;
    m_targetInfo.m_nChannel = 0;
    m_targetInfo.m_nDeviceType = 0xFF;
    m_targetInfo.m_nInitiatorNetworkAddress = 0xFFFF;
    m_targetInfo.targetStatus = CQorvoTouchLinkClientTargetInfo::statusInvalid; 
    m_targetInfo.m_nFreeNetworkAddressRangeBegin = 0x0000;
    m_targetInfo.m_nFreeNetworkAddressRangeEnd = 0x0000;
    m_targetInfo.m_nFreeGroupIdRangeBegin = 0x0000;
    m_targetInfo.m_nFreeGroupIdRangeEnd = 0x0000;
}

bool CQorvoClusterTouchLinkCommissioningClient::configureNetworkCmdGeneralFields(void)
{
    // TOUCHLINK COMMISSIONING ATTRIBUTES
    /*
        If the device is not address assignment capable or it has joined a network through classical
        Zigbee joining mechanisms, this attribute should be set to 0x0000.
    */
    if(!(m_nInitiatorTouchlinkInformation & CZigBeeClusterTouchlink::addressAssignmentCapable) || (aps.m_nwk.m_nlme.IsJoined() && !aps.m_apsme.IsDistributedSecurityNetwork()))
    {
        m_nFreeNetworkAddressRangeBegin = 0x0000;
        m_nFreeNetworkAddressRangeEnd = 0x0000;
        m_nFreeGroupIdRangeBegin = 0x0000;
        m_nFreeGroupIdRangeEnd = 0x0000;
    }
    /* Restore default values when factory new */
    if (m_nInitiatorTouchlinkInformation & CZigBeeClusterTouchlink::factoryNew)
    {
        m_nFreeNetworkAddressRangeBegin = 0x0001;
        m_nFreeNetworkAddressRangeEnd = 0xFFF7;
        m_nFreeGroupIdRangeBegin = 0x0001;
        m_nFreeGroupIdRangeEnd = 0xFEFF;

        /*
            When a factory-new initiator device which is assignment capable has just formed a new network, it shall take the
            group identifiers, starting from G_min (i.e., 0x0001) for its own endpoints and shall then increment G_min with the number
            of endpoints supported on the device.
        */
        std::set<unsigned char> activeEndpointSet;
        aps.GetActiveEndpoints(activeEndpointSet);
        uint8_t activeEndpointCount = activeEndpointSet.size();
        m_nFreeGroupIdRangeBegin = m_nFreeGroupIdRangeBegin + activeEndpointCount;

        /*
            When a factory-new initiator device, which is address assignment capable, has just formed a new network, it
            shall assign itself the network address (i.e., 0x0001) and then increment , i.e., the range changes
            to (0x0002 0xfff7).
        */
        m_nInitiatorNetworkAddress = m_nFreeNetworkAddressRangeBegin;
        m_nFreeNetworkAddressRangeBegin = m_nFreeNetworkAddressRangeBegin + 1;
        m_targetInfo.m_nInitiatorNetworkAddress = m_nInitiatorNetworkAddress;
    }

    else
    {
        gpHal_SourceIdentifier_t srcId = gpHal_SourceIdentifier_0;
        m_nInitiatorNetworkAddress = gpHal_GetShortAddress(srcId);
        m_targetInfo.m_nInitiatorNetworkAddress = m_nInitiatorNetworkAddress;
    }

    // NETWORK ADDRESS FIELD
    /* 
        If the value of the aplFreeNwkAddrRangeBegin attribute is equal to 0x0000
        (initiator joined a network using MAC association), the address SHALL be
        stochastically generated according to the classical Zigbee mechanism. 
    */
    if(m_nFreeNetworkAddressRangeBegin == 0x0000)
    {
        // Generate random short address for target
        m_targetInfo.targetAddress.m_wShort = rand16();
    }

    /*
        If the value of the aplFreeNwkAddrRangeBegin attribute is not equal to 0x0000,
        the address SHALL be equal to aplFreeNwkAddrRangeBegin
        and then this value SHALL be incremented
    */
    else
    {
        m_targetInfo.targetAddress.m_wShort = m_nFreeNetworkAddressRangeBegin;
        m_nFreeNetworkAddressRangeBegin = m_nFreeNetworkAddressRangeBegin + 1;
    }

    // GROUP IDENTIFIER BEGIN/END FIELDS
    /*
        If the target requested a set of group identifiers in its scan response command frame
        and the value of the aplFreeGroupIDAddrRangeBegin attribute (see [R2]) is equal to
        0x0000 (initiator joined a network using MAC association), the group identifiers
        begin and group identifiers end fields SHALL be set to 0x0000.
     */
    if(m_storedScanRsp.scanRsp.m_nTotalGroupCount != 0)
    {
        if(m_nFreeGroupIdRangeBegin == 0x0000)
        {
            m_targetInfo.m_nGroupIdRangeBegin = 0x0000;
            m_targetInfo.m_nGroupIdRangeEnd = 0x0000;
        }
        /*
            If the target requested a set of group identifiers in its scan response command frame and the value
            of the aplFreeGroupIDAddrRangeBegin attribute is not equal to 0x0000, a range of
            group identifiers SHALL be allocated for the target and the group identifiers begin
            and group identifiers end fields set accordingly.
        */
        else if((m_nFreeGroupIdRangeBegin + m_storedScanRsp.scanRsp.m_nTotalGroupCount) <= m_nFreeGroupIdRangeEnd)
        {
            m_targetInfo.m_nGroupIdRangeBegin = m_nFreeGroupIdRangeBegin;
            m_targetInfo.m_nGroupIdRangeEnd = m_nFreeGroupIdRangeBegin + m_storedScanRsp.scanRsp.m_nTotalGroupCount;
            //If full group ID pool is depleted begin and end value should be the same
            if((m_nFreeGroupIdRangeBegin + m_storedScanRsp.scanRsp.m_nTotalGroupCount) == m_nFreeGroupIdRangeEnd)
            {
                m_nFreeGroupIdRangeBegin = m_nFreeGroupIdRangeBegin + m_storedScanRsp.scanRsp.m_nTotalGroupCount;
            }
            else
            {
                m_nFreeGroupIdRangeBegin = m_nFreeGroupIdRangeBegin + m_storedScanRsp.scanRsp.m_nTotalGroupCount + 1;
            }
        }
        /*
            When no sufficient group identifiers are available for the target, the new device shall not join the net
        */
        else
        {
            // Do not proceed with Touchlink connection
            return false;
        }
    }
    /*
        If the value of the group identifiers begin field is equal to zero, a
        range of group identifiers has not been allocated and this field shall be ignored
    */
    else
    {
        m_targetInfo.m_nGroupIdRangeBegin = 0x0000;
        m_targetInfo.m_nGroupIdRangeEnd = 0x0000;
    }

    //FREE NETWORK RANGE BEGIN/END FIELDS AND FREE GROUP IDENTIFIER RANGE BEGIN/END FIELDS
    /*
        If the target indicated that it was address assignment
        capable in its scan response command frame
    */

    if (m_storedScanRsp.scanRsp.m_nTouchlinkInformation & CZigBeeClusterTouchlink::addressAssignmentCapable)
    {
        /*
            and the value of the aplFreeNwkAddrRangeBegin attribute (see [R2])
            is equal to 0x0000, the free network address range begin, free network address range
            end, free group identifier range begin and free group identifier range end fields
            SHALL be set to 0x0000.
        */
        if(m_nFreeNetworkAddressRangeBegin == 0x0000)
        {
            m_targetInfo.m_nFreeNetworkAddressRangeBegin = 0x0000;
            m_targetInfo.m_nFreeNetworkAddressRangeEnd = 0x0000;
            m_targetInfo.m_nFreeGroupIdRangeBegin = 0x0000;
            m_targetInfo.m_nFreeGroupIdRangeEnd = 0x0000;
        }
        /*
            and the value of the aplFreeNwkAddrRangeBegin attribute is not equal to 0x0000, 
            a range of network addresses and group identifiers SHALL be allocated for the target
            to use for its own purposes and the free network address range begin,
            free network address range end, free group identifier range begin
            and free group identifier range end fields set accordingly.
        */
        else
        {
            /*
                If, division of a free network address range or free group identifier range, is smaller than an
                implementation specific threshold, the new device shall not be joined to the network.
            */
            if((m_nFreeNetworkAddressRangeEnd < (m_nFreeNetworkAddressRangeBegin + customFreeNwkAddrTargetRange + customFreeNwkAddrThreshold)) ||
                (m_nFreeGroupIdRangeEnd < (m_nFreeGroupIdRangeBegin + customFreeGroupIdTargetRange + customFreeGroupIdThreshold)))
            {
                    // Do not proceed with Touchlink connection
                    return false;
            }
            /*
                The initiator shall split its own free network address range and free group identifier range at an implementation
                specified point and the upper range (i.e., highest in value) shall be assigned to the new address assignment
                capable device.
            */
            m_targetInfo.m_nFreeNetworkAddressRangeEnd = m_nFreeNetworkAddressRangeEnd;
            m_nFreeNetworkAddressRangeEnd = m_nFreeNetworkAddressRangeEnd - customFreeNwkAddrTargetRange;
            m_targetInfo.m_nFreeNetworkAddressRangeBegin = m_nFreeNetworkAddressRangeEnd +1;

            m_targetInfo.m_nFreeGroupIdRangeEnd = m_nFreeGroupIdRangeEnd;
            m_nFreeGroupIdRangeEnd = m_nFreeGroupIdRangeEnd - customFreeGroupIdTargetRange;
            m_targetInfo.m_nFreeGroupIdRangeBegin = m_nFreeGroupIdRangeEnd +1;
        }
    }
    else
    {
        m_targetInfo.m_nFreeNetworkAddressRangeBegin = 0x0000;
        m_targetInfo.m_nFreeNetworkAddressRangeEnd = 0x0000;
        m_targetInfo.m_nFreeGroupIdRangeBegin = 0x0000;
        m_targetInfo.m_nFreeGroupIdRangeEnd = 0x0000;
    }

    const CZigBeeAttribute *const pFreeNetworkAddressRangeBegin = FindAttribute(attributeFreeNetworkAddressRangeBegin,
                        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, CZigBeeDeviceObject::wManufacturerID);
    const CZigBeeAttribute *const pFreeNetworkAddressRangeEnd = FindAttribute(attributeFreeNetworkAddressRangeEnd,
                        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, CZigBeeDeviceObject::wManufacturerID);
    const CZigBeeAttribute *const pFreeGroupIdRangeBegin = FindAttribute(attributeFreeGroupIdRangeBegin,
                        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, CZigBeeDeviceObject::wManufacturerID);
    const CZigBeeAttribute *const pFreeGroupIdRangeEnd = FindAttribute(attributeFreeGroupIdRangeEnd,
                        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, CZigBeeDeviceObject::wManufacturerID);
    ASSERT(pFreeNetworkAddressRangeBegin);
    ASSERT(pFreeNetworkAddressRangeEnd);
    ASSERT(pFreeGroupIdRangeBegin);
    ASSERT(pFreeGroupIdRangeEnd);
    SetAttributeValue(*pFreeNetworkAddressRangeBegin, &m_nFreeNetworkAddressRangeBegin, sizeof(uint16_t));
    SetAttributeValue(*pFreeNetworkAddressRangeEnd, &m_nFreeNetworkAddressRangeEnd, sizeof(uint16_t));
    SetAttributeValue(*pFreeGroupIdRangeBegin, &m_nFreeGroupIdRangeBegin, sizeof(uint16_t));
    SetAttributeValue(*pFreeGroupIdRangeEnd, &m_nFreeGroupIdRangeEnd, sizeof(uint16_t));
    pFreeNetworkAddressRangeBegin->Store(*this);
    pFreeNetworkAddressRangeEnd->Store(*this);
    pFreeGroupIdRangeBegin->Store(*this);
    pFreeGroupIdRangeEnd->Store(*this);
    return true;
}

void CQorvoClusterTouchLinkCommissioningClient::LoopScanRequests(void* pArg)
{
    CQorvoClusterTouchLinkCommissioningClient *ctrl = 
        (CQorvoClusterTouchLinkCommissioningClient*)pArg;

    uint8_t nextChannelIndex;
    //Verify scan responses were received after 1 loop
    if(ctrl->m_nScanChannelIndex == 0)
    {
        /*
            2 scenarios before identifying potential target:
            - secondary channels is optional
            - force to have both primary and secondary channels scanned
        */ 
        if(!ctrl->m_bForceSecondaryScan || (ctrl->m_bForceSecondaryScan && ctrl->m_bDoPrimaryScan))
        {
            if(ctrl->m_storedScanRsp.keyBitMask != keyUndefined)
            {
                ctrl->IdentifyPotentialTarget();
                return;
            }
        }

        //set the current channel set according to the flag m_bDoPrimaryScan
        if(ctrl->m_bDoPrimaryScan)
        {
            ctrl->m_pCurrentScanChannelSet = (unsigned char*)&abPrimaryScanChannelSet[0];
            ctrl->m_nCurrentScanChannelSize = sizeof(abPrimaryScanChannelSet);            
        }
        else
        {
            ctrl->m_pCurrentScanChannelSet = (unsigned char*)&abSecondaryScanChannelSet[0];
            ctrl->m_nCurrentScanChannelSize = sizeof(abSecondaryScanChannelSet);
        }
    }
    //Proceed with scanning
    ctrl->m_nInitiatorTouchlinkInformation = (
        CZigBeeClusterTouchlink::initiator |
        CZigBeeClusterTouchlink::addressAssignmentCapable);
    // Set Tx Power to Max
    aps.m_nwk.m_mac.m_phy.SetTransmitPowerEx(gpMacCore_TxPowerMax);

    // Update current channel for scan request
    aps.m_nwk.m_mac.m_phy.SetCurrentChannel(ctrl->m_pCurrentScanChannelSet[ctrl->m_nScanChannelIndex]);
    
    if (!aps.m_nwk.m_nlme.IsJoined())
    {
        ctrl->m_nInitiatorTouchlinkInformation |= CZigBeeClusterTouchlink::factoryNew;
    }
        
    // Update next channel Index
    if (ctrl->m_nScanChannelIndex >= (ctrl->m_nCurrentScanChannelSize -1))
    {
        nextChannelIndex = 0;
        if(ctrl->m_bForceSecondaryScan)
        {
            //toggle the flag
            ctrl->m_bDoPrimaryScan = !ctrl->m_bDoPrimaryScan;
        }
    }
    else
    {
        nextChannelIndex = ctrl->m_nScanChannelIndex+ 1;
    }

    GP_LOG_SYSTEM_PRINTF("TouchLink: Scan Request, Ch: %d\n",0,ctrl->m_pCurrentScanChannelSet[ctrl->m_nScanChannelIndex]);
    
    // Send scan request
    ctrl->ScanRequest(CZigBeeAddress(CZigBeeAddress::wBroadcastAll),
        ctrl->m_nTransactionId,
        ctrl->m_nDeviceType,
        ctrl->m_nInitiatorTouchlinkInformation); 

    // Schedule next request with 0.25s delay
    gpSched_ScheduleEventArg(250000,LoopScanRequests,ctrl);

    
    // update channel indes to next channel
    ctrl->m_nScanChannelIndex = nextChannelIndex;
 
}

void CQorvoClusterTouchLinkCommissioningClient::SessionTimeout(void* pArg)
{
    CQorvoClusterTouchLinkCommissioningClient *ctrl = 
        (CQorvoClusterTouchLinkCommissioningClient*)pArg;

    GP_LOG_SYSTEM_PRINTF("TouchLink: Commissioning %s\n",0, aps.m_nwk.m_nlme.IsJoined()?"Success!":"Session Timeout");
    // Launch the stop scanning 
    ctrl-> StopScanning();
}

void CQorvoClusterTouchLinkCommissioningClient::TargetStartNetwork(void* pArg)
{
    CQorvoClusterTouchLinkCommissioningClient *ctrl = 
        (CQorvoClusterTouchLinkCommissioningClient*)pArg;
    
    MACAddress_t initiatorIeeeAddress;

    // Generate random Pan ID for the new network
    ctrl->m_targetInfo.targetAddress.m_wPANID = rand16();
    
    // Set ext. panId to 0, target shall determine the extended PAN identifier for the new network
    ctrl->m_targetInfo.qwExtendedPanId.LSB = 0;
    ctrl->m_targetInfo.qwExtendedPanId.MSB = 0;
    
    // Retrieve MAC address for initiator IEEE address
    gpHal_GetExtendedAddress(&initiatorIeeeAddress, 0);

    bool configNwkParamsSuccesfull = false;
    configNwkParamsSuccesfull = ctrl->configureNetworkCmdGeneralFields();
    if (!configNwkParamsSuccesfull)
    {
        ctrl->StopScanning();
        return;
    }
    // required to send request to 0xFFFF instead of specific PAN ID
    CZigBeeAddress destAddress(CIEEE802154Address(
        ctrl->m_targetInfo.targetAddress.m_qwExtended,
        ctrl->m_targetInfo.targetAddress.m_wShort,
        0xFFFF));
    destAddress.SetType(CIEEE802154Address::typeExtended);
    
    //Update TouchLink commissioning status
    ctrl->m_nTouchLinkStatus = statusTargetStartNetwork;
    
    // Launch commissioning state callback
    ctrl->OnTouchLinkCommissioningStatus(ctrl->m_nTouchLinkStatus);

    // Send network start request to target
    ctrl->NetworkStartRequest(destAddress,
                            ctrl->m_nTransactionId,
                            ctrl->m_targetInfo.qwExtendedPanId,
                            ctrl->m_targetInfo.wKeyBitMask,
                            ctrl->abEncryptedNetworkKey,
                            ctrl->m_targetInfo.m_nChannel,
                            ctrl->m_targetInfo.targetAddress.m_wPANID,
                            ctrl->m_targetInfo.targetAddress.m_wShort,
                            ctrl->m_targetInfo.m_nGroupIdRangeBegin,
                            ctrl->m_targetInfo.m_nGroupIdRangeEnd,
                            ctrl->m_targetInfo.m_nFreeNetworkAddressRangeBegin,
                            ctrl->m_targetInfo.m_nFreeNetworkAddressRangeEnd,
                            ctrl->m_targetInfo.m_nFreeGroupIdRangeBegin,
                            ctrl->m_targetInfo.m_nFreeGroupIdRangeEnd,
                            initiatorIeeeAddress,
                            ctrl->m_targetInfo.m_nInitiatorNetworkAddress);
}

void CQorvoClusterTouchLinkCommissioningClient::TargetJoinNetwork(void* pArg)
{
    CQorvoClusterTouchLinkCommissioningClient *ctrl = 
        (CQorvoClusterTouchLinkCommissioningClient*)pArg;

    bool configNwkParamsSuccesfull = false;
    configNwkParamsSuccesfull = ctrl->configureNetworkCmdGeneralFields();
    if (!configNwkParamsSuccesfull)
    {
        ctrl->StopScanning();
        return;
    }
        
    //Use current network's Pan ID
    ctrl->m_targetInfo.targetAddress.m_wPANID = aps.m_nwk.m_mac.m_pib.m_wPANID;
    
    //Use current network's extended Pan ID
    memcpy(&ctrl->m_targetInfo.qwExtendedPanId, &aps.m_nwk.m_nlme.m_nib.m_qwExtendedPANID, sizeof(MACAddress_t));
        
    // required to send request to 0xFFFF instead of specific PAN ID
    CZigBeeAddress destAddress(CIEEE802154Address(
        ctrl->m_targetInfo.targetAddress.m_qwExtended,
        ctrl->m_targetInfo.targetAddress.m_wShort,
        0xFFFF));
    destAddress.SetType(CIEEE802154Address::typeExtended);
    
    //Update TouchLink commissioning status
    ctrl->m_nTouchLinkStatus = statusTargetStartJoining;
    
    // Launch commissioning state callback
    ctrl->OnTouchLinkCommissioningStatus(ctrl->m_nTouchLinkStatus);
    
    if((ctrl->m_targetInfo.m_nDeviceType == typeRouter) ||
       (ctrl->m_targetInfo.m_nDeviceType == typeCoordinator))
    {
        // Send network join Router request to target
        ctrl->NetworkJoinRouterRequest(destAddress,
                                ctrl->m_nTransactionId,
                                ctrl->m_targetInfo.qwExtendedPanId,
                                ctrl->m_targetInfo.wKeyBitMask,
                                ctrl->abEncryptedNetworkKey,
                                ctrl->m_nNetworkUpdateId,
                                ctrl->m_nCurrentNetworkChannel,
                                ctrl->m_targetInfo.targetAddress.m_wPANID,
                                ctrl->m_targetInfo.targetAddress.m_wShort,
                                ctrl->m_targetInfo.m_nGroupIdRangeBegin,
                                ctrl->m_targetInfo.m_nGroupIdRangeEnd,
                                ctrl->m_targetInfo.m_nFreeNetworkAddressRangeBegin,
                                ctrl->m_targetInfo.m_nFreeNetworkAddressRangeEnd,
                                ctrl->m_targetInfo.m_nFreeGroupIdRangeBegin,
                                ctrl->m_targetInfo.m_nFreeGroupIdRangeEnd);
    }
    else // typeEndDevice
    {
        // Send network join End Device request to target
        ctrl->NetworkJoinEndDeviceRequest(destAddress,
                                ctrl->m_nTransactionId,
                                ctrl->m_targetInfo.qwExtendedPanId,
                                ctrl->m_targetInfo.wKeyBitMask,
                                ctrl->abEncryptedNetworkKey,
                                ctrl->m_nNetworkUpdateId,
                                ctrl->m_targetInfo.m_nChannel,
                                ctrl->m_targetInfo.targetAddress.m_wPANID,
                                ctrl->m_targetInfo.targetAddress.m_wShort,
                                ctrl->m_targetInfo.m_nGroupIdRangeBegin,
                                ctrl->m_targetInfo.m_nGroupIdRangeEnd,
                                ctrl->m_targetInfo.m_nFreeNetworkAddressRangeBegin,
                                ctrl->m_targetInfo.m_nFreeNetworkAddressRangeEnd,
                                ctrl->m_targetInfo.m_nFreeGroupIdRangeBegin,
                                ctrl->m_targetInfo.m_nFreeGroupIdRangeEnd);
    }
}

void CQorvoClusterTouchLinkCommissioningClient::TargetResetToFactoryNew(void* pArg)
{
    CQorvoClusterTouchLinkCommissioningClient *ctrl = 
        (CQorvoClusterTouchLinkCommissioningClient*)pArg;

    GP_LOG_SYSTEM_PRINTF("Touchlink: Request reset to factory new to the target 0x%08lx%08lx (ch %d)\n", 0,
        reinterpret_cast<MACAddress_t *>(&ctrl->m_targetInfo.targetAddress.m_qwExtended)->MSB,
        reinterpret_cast<MACAddress_t *>(&ctrl->m_targetInfo.targetAddress.m_qwExtended)->LSB,
        ctrl->m_targetInfo.m_nChannel);

    // Send reset to factory new request
    ctrl->ResetToFactoryNewRequest(ctrl->m_targetInfo.targetAddress, ctrl->m_nTransactionId);

    // Terminate Touchlink process
    ctrl->StopScanning();
}

void CQorvoClusterTouchLinkCommissioningClient::ApplyNetworkParams(void* pArg)
{
    CQorvoClusterTouchLinkCommissioningClient *ctrl = (CQorvoClusterTouchLinkCommissioningClient*)pArg;
    CZigBeeTouchlinkCommissioningInfo info = info;
    
    // update parameter to apply
    // -- logical channel
    info.m_nChannel = ctrl->m_targetInfo.m_nChannel;
    // -- extended Pan ID
    memcpy(&info.m_qwExtendedPANID, &ctrl->m_targetInfo.qwExtendedPanId,sizeof(MACAddress_t));
    // -- Pan ID
    info.m_wPANID = ctrl->m_targetInfo.targetAddress.m_wPANID;
    // -- Device Address 
    info.m_wNetworkAddress = ctrl->m_targetInfo.m_nInitiatorNetworkAddress;
    // --  SAS start-up control value
    info.m_nStartupControl = CZigBeeCommissioningAttributes::startupCommissioned;
    // --  Network key
    memcpy(info.m_abNetworkKey,abNetworkKey,sizeof(abNetworkKey));
    // --  Network update ID
    info.m_nNetworkUpdateID = ctrl->m_nNetworkUpdateId;
    
    //Update TouchLink commissioning status
    ctrl->m_nTouchLinkStatus = statusApplyNetwork;
    
    // Launch commissioning state callback
    ctrl->OnTouchLinkCommissioningStatus(ctrl->m_nTouchLinkStatus);
       
    //CZigBeeClusterTouchlink::Apply(info);
    ctrl->ApplyToStack(info);
}

void CQorvoClusterTouchLinkCommissioningClient::ApplyToStack(CZigBeeTouchlinkCommissioningInfo info)
{
    CZigBeeNetworkSecurityMaterial nwk_material;
    CZigBeeCommissioningAttributesEx attributes;
    
    // Retrieve the Startup Attribute
    attributes.Load();
    
    // Update the Startup Attribute according to the Touchlink Commissioning Info.
    attributes.m_nChannel = info.m_nChannel;
    attributes.m_qwExtendedPANID = info.m_qwExtendedPANID;
    attributes.m_wPANID = info.m_wPANID;
    attributes.m_wShortAddress = info.m_wNetworkAddress;
    attributes.m_bStartupControl = info.m_nStartupControl;
    attributes.m_nNetworkUpdateID = info.m_nNetworkUpdateID;
    
    // Set active network key sequence = 0
    attributes.m_bNetworkKeySequence = 0;   
    
    // Set TrustCenter Address to all 0xF for distributed network
    attributes.m_qwTrustCenterAddress = 0xFFFFFFFFFFFFFFFFULL;
    
    // Set channel mask to all channels
    attributes.m_dwChannelMask = 0x7fff800; 
    
    // Store and commit the Startup Attribute
    attributes.Store();
    attributes.Commit(aps);
    
    // Set active network key(seq 0) into NWK layer
    nwk_material.m_nKeySequenceNumber = 0; 
    nwk_material.m_nKeyType = CZigBeeSecurityMaterial::typeNetworkKey;
    memcpy(nwk_material.m_abKey, abNetworkKey, sizeof(attributes.m_abNetworkKey));
    aps.m_nwk.m_nlme.StoreSecurityMaterial(nwk_material);
    aps.m_nwk.m_nlme.Store();

    //Trigger rejoining via a stack restart
    m_nTouchLinkStatus = statusInitiatorRejoining;
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    RestartStack();
}

void CQorvoClusterTouchLinkCommissioningClient::IdentifyPotentialTarget(void)
{
    // Update channel to prefered TL target
    uint8_t channel = m_storedScanRsp.scanRsp.m_nChannel;
    if(channel == 0)
    {
        channel = m_storedScanRsp.m_scanChannel;
    }
    aps.m_nwk.m_mac.m_phy.SetCurrentChannel(channel);
    // Generate encrypted network key 
    m_targetInfo.wKeyBitMask = m_storedScanRsp.keyBitMask;
    unsigned char temp_key[16] = {0};
    unsigned char temp_Encrypted_key[16] = {0};
    
    memcpy(temp_Encrypted_key, abNetworkKey, sizeof(abNetworkKey));
    
    // -- Encrypt transport key with Network key
    GetEncryptionKey(temp_key,__cfcore_be32(m_storedScanRsp.scanRsp.m_nSessionID),__cfcore_be32(m_storedScanRsp.scanRsp.m_nResponseID),m_targetInfo.wKeyBitMask);
    
    DeriveTransportKey(temp_Encrypted_key,temp_key);
    
    // Store network parameters to access target
    // -- Encrypted network key
    memcpy(&abEncryptedNetworkKey,
        temp_Encrypted_key,sizeof(temp_Encrypted_key));
        
    GP_LOG_PRINTF("temp_Encrypted_key:",0);
    //gpLog_PrintBuffer(sizeof(temp_Encrypted_key),temp_Encrypted_key);
    
    GP_LOG_PRINTF("abEncryptedNetworkKey:",0);
    //gpLog_PrintBuffer(sizeof(SecKey128Bit_t),abEncryptedNetworkKey.keyByte);
    
    // -- Extended Pan ID 
    memcpy(&m_targetInfo.qwExtendedPanId,
        (UInt8*)&(m_storedScanRsp.scanRsp) + offsetof(CZigBeeTouchlinkScanResponse, m_qwExtendedPANID),sizeof(m_targetInfo.qwExtendedPanId));
    // -- Target logical channel
    m_targetInfo.m_nChannel = m_storedScanRsp.scanRsp.m_nChannel;
    // -- Target short & MAC address
    m_targetInfo.targetAddress =
        CZigBeeAddress(CIEEE802154Address(m_storedScanRsp.m_qwExtended,m_storedScanRsp.scanRsp.m_wNetworkAddress,m_storedScanRsp.scanRsp.m_wPANID));
    m_targetInfo.targetAddress.SetType(CIEEE802154Address::typeExtended);
    
    // -- Target Device Type
    if (m_storedScanRsp.scanRsp.m_nZigBeeInformation & typeRouter) 
    {
        m_targetInfo.m_nDeviceType = typeRouter;
    }
    else if (m_storedScanRsp.scanRsp.m_nZigBeeInformation & typeEndDevice) 
    {
        m_targetInfo.m_nDeviceType = typeEndDevice;
    }
    else
    {
        m_targetInfo.m_nDeviceType = typeCoordinator;
    }
    //Reschedule the sessionTimeout 
    if (gpSched_ExistsEventArg(SessionTimeout, this))
    {
        gpSched_UnscheduleEventArg(SessionTimeout, this);

        gpSched_ScheduleEventArg(MS_TO_US(sessionDuration), SessionTimeout, this);
    }
    
    //Update TouchLink commissioning status
    m_nTouchLinkStatus = statusDetermindedPotentialTarget;
    // Callback update to application layer
    OnTouchLinkCommissioningStatus(m_nTouchLinkStatus);
    // Send Device info request if enabled.
    if (m_bEnableDeviceInfoRequest)
    {
        // Send out Device info request to selected device
        DeviceInformationRequest(m_targetInfo.targetAddress, m_nTransactionId, 0);
    }
    else
    {
        // Send out Identify request to selected device
        IdentifyRequest(m_targetInfo.targetAddress
            , m_nTransactionId, identifyDuration);

        if (!m_bResetToFactoryNew)
        {
            ConnectTouchlink();
        }
        else
        {
            // Delayed triggering of reset to factory new request
            gpSched_ScheduleEventArg(MS_TO_US(100), TargetResetToFactoryNew, this);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient command map

__COMPACT7B_IMPLEMENT_COMMAND_MAP(CQorvoClusterTouchLinkCommissioningClient, CZigBeeCluster)

__COMPACT7B_BEGIN_COMMAND_MAP(CQorvoClusterTouchLinkCommissioningClient)
    __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_CLUSTER_COMMANDS()
__COMPACT7B_END_COMMAND_MAP()
