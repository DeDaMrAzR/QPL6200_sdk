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

#ifndef _QORVO_CLUSTER_TOUCHLINKCOMMISSIONING_H_
#define _QORVO_CLUSTER_TOUCHLINKCOMMISSIONING_H_

#include "global.h"
#include "ZigBeeClusterTouchlink.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

//#define SecurityKey_api2buf(pDest, pSource, length, pIndex) generic_api2buf_1(pDest, pSource, pIndex, 16)     //MTK

typedef struct SecKey128BitStruct
{
    UInt8 keyByte[16];
}SecKey128Bit_t;

/////////////////////////////////////////////////////////////////////////////
// CQorvoTouchLinkClientTargetInfo

class CQorvoTouchLinkClientTargetInfo
{
    // typedefs & enums
    public:
    // Enumerations for the status for start/join network
        enum {
            statusSuccess   = 0x00,
            statusFailure   = 0x01,
            statusInvalid   = 0xFF
        };
    
    public:
        // key Bitmask from target which is match 
        unsigned short wKeyBitMask;
        
        // Target extended Pan ID
        MACAddress_t qwExtendedPanId;
        
        // Target logical Channel
        unsigned char m_nChannel;
        
        // Target device type
        unsigned char m_nDeviceType;
        
        // Target address
        CZigBeeAddress targetAddress;
        
        // status from target for the start/join status
        // 0x01 means failed
        unsigned char targetStatus;
        
        // initiator address to be used
        unsigned short m_nInitiatorNetworkAddress;
        
        // Start of the group ID range
        unsigned short m_nGroupIdRangeBegin;
        
        // End of the group ID range
        unsigned short m_nGroupIdRangeEnd;

        // Start of the Free Network address range
        unsigned short m_nFreeNetworkAddressRangeBegin;
        
        // End of the Free Network address range
        unsigned short m_nFreeNetworkAddressRangeEnd;

        // Start of the Free Group Identifier range
        unsigned short m_nFreeGroupIdRangeBegin;

        // End of the Free Group Identifier range
        unsigned short m_nFreeGroupIdRangeEnd;

        // Device Information Response  
        CZigBeeTouchlinkDeviceInformationResponse devInfoRsp; 

        // Device Information Record 
         CZigBeeTouchlinkDeviceInformation devInfoRec[5];
        
    // Construction
    public:
        CQorvoTouchLinkClientTargetInfo(void);
};

/////////////////////////////////////////////////////////////////////////////
// CQorvoTouchLinkScanResponseData

class CQorvoTouchLinkScanResponseData
{
    public:
        // Target Scan response
        CZigBeeTouchlinkScanResponse scanRsp;
        
        // Target extended Address
        uint64_t m_qwExtended;
        
        // RSSI of Target's scan response 
        int8_t nRSSI;
        
        // key Bitmask from target which is match 
        unsigned short keyBitMask;

        // scan channel
        uint8_t m_scanChannel;

    // Construction
    public:
        CQorvoTouchLinkScanResponseData(void);
};

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningClient

class CQorvoClusterTouchLinkCommissioningClient : public CZigBeeClusterTouchlink
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()
    __COMPACT7B_DECLARE_COMMAND_MAP()

    // typedefs & enums
    public:
        // Cluster identifier of the touch-link commissioning client cluster
        enum { id = 0x1000 };

        // Cluster revision of this implementation
        enum { revision = 3 };
        
        // Cluster command identifiers (client to server)
        enum {
            commandScanRequest = 0x00,                         /* Mandatory */
            commandDeviceInformationRequest = 0x02,            /* Mandatory */
            commandIdentifyRequest = 0x06,                     /* Mandatory */
            commandResetToFactoryNewRequest = 0x07,            /* Mandatory */
            commandNetworkStartRequest = 0x10,                 /* Mandatory */
            commandNetworkJoinRouterRequest = 0x12,            /* Mandatory */
            commandNetworkJoinEndDeviceRequest = 0x14,         /* Mandatory */
            commandNetworkUpdateRequest = 0x16,                /* Mandatory */
            commandGetGroupIdentifiersRequest = 0x41,          /* Mandatory */
            commandGetEndpointListRequest = 0x42,              /* Mandatory */
        };

        // Cluster command identifiers (server to client)
        enum {
            commandScanResponse = 0x01,                        /* Mandatory */
            commandDeviceInformationResponse = 0x03,           /* Mandatory */
            commandNetworkStartResponse = 0x11,                /* Mandatory */
            commandNetworkJoinRouterResponse = 0x13,           /* Mandatory */
            commandNetworkJoinEndDeviceResponse = 0x15,        /* Mandatory */
            commandEndpointInformation = 0x40,                 /* Optional */
            commandGetGroupIdentifiersResponse = 0x41,         /* Mandatory */
            commandGetEndpointListResponse = 0x42,             /* Mandatory */
        };
        
        // Enumerations for the touchlink commissioning staus 
        enum {
            statusStopped                   = 0x00,
            statusScanRequest               = 0x01,
            statusScanResponseReceived      = 0x02,
            statusDetermindedPotentialTarget = 0x03,
            statusTargetStartNetwork        = 0x04,
            statusTargetNetworkStarted      = 0x05,
            statusInitiatorRejoining        = 0x06,
            statusTargetStartJoining        = 0x07,
            statusTargetNetworkJoined       = 0x08,
            statusApplyNetwork              = 0x09,
        };
        
        // Enumerations for the ZLL profile
        enum { zllProfileID        = 0xC05E };
        
        // Enumerations for logical type in ZigBee Information
        enum { typeCoordinator, typeRouter, typeEndDevice };
        
        // Enumerations for key index to be used in the encryption algorithms
        enum { 
            keyDevelopment      = 0x00,
            keyMaster           = 0x04,
            keyPrecertification = 0x0f,
            keyUndefined        = 0xff,
        };
        
        /** @brief Enumerations for delay parameters */
        // Enumerations for identify duration(in second)
        enum {identifyDuration      = 10};
        
        // Enumerations for scan request loop timeout(in ms)
        enum {scanTimeout           = 8000,
              extendedScanTimeout   = 10000};
        
        // Enumerations for session timeout(in ms)
        enum {sessionDuration       = 8000};
        
        // Enumerations for delay from identify to network start/join(in ms)
        enum {networkStartJoinDelay = 3000};
        
        // Enumerations for delay from network start to reboot(in ms)
        enum {applyNetworkDelay     = 3000};
        
        // Enumerations for Free Address Assignment and Group Id Assignment settings
        enum { 
            customFreeNwkAddrTargetRange = 0x00ff,
            customFreeNwkAddrThreshold = 0x00ff,
            customFreeGroupIdTargetRange = 0x00ff,
            customFreeGroupIdThreshold = 0x00ff,
        };
        // Attribute identifiers of the TouchLink client cluster
		enum { 
            attributeFreeNetworkAddressRangeBegin = 0x00,
            attributeFreeNetworkAddressRangeEnd,
            attributeFreeGroupIdRangeBegin,
            attributeFreeGroupIdRangeEnd
        };

    // Construction
    public:
        CQorvoClusterTouchLinkCommissioningClient
            (CZigBeeFoundationApplication &application,
                const unsigned char logicalType = typeEndDevice,
                unsigned short initiatorAddress = 0,
                bool enableDeviceInfoRequest = false);

    // Implementation
    public:
        // Network key used in the touchLink commissioning
        static unsigned char abNetworkKey[16];
        
        // Primary Channel set used in the touchLink commissioning
        static const unsigned char abPrimaryScanChannelSet[8];
        
        // Secondary Channel set used in the touchLink commissioning
        static const unsigned char abSecondaryScanChannelSet[12];

        // Transaction ID used in the touchLink commissioning
        unsigned int m_nTransactionId;
        
        // Flag that to include device info Request in the touchLink commissioning
        bool m_bEnableDeviceInfoRequest;
        
        // initiator IEEE address
        MACAddress_t initiatorIeeeAddress;

        // initiator address to be used
        unsigned short m_nInitiatorNetworkAddress;

        // Start of the Network address range can be used for target
        unsigned short m_nFreeNetworkAddressRangeBegin;
        
        // Start of the Network address range can be used for target
        unsigned short m_nFreeNetworkAddressRangeEnd;

        // Start of the Group identifier range can be used for target
        unsigned short m_nFreeGroupIdRangeBegin;
        
        // Start of the Group identifier range can be used for target
        unsigned short m_nFreeGroupIdRangeEnd;

    protected:
        // Current index for the scan channel set during scan request loop
        unsigned char m_nScanChannelIndex;
        
        // device type of this device
        unsigned char m_nDeviceType;

        // Flag that primary channel set is used for scanning
        bool m_bDoPrimaryScan;

        // Flag that force to have channel scanning over secondary channels
        bool m_bForceSecondaryScan;

        // Channel set used in the touchLink commissioning
        unsigned char* m_pCurrentScanChannelSet;

        // size of current scan channel size
        unsigned char m_nCurrentScanChannelSize;

        // Class for storing scanned target information
        CQorvoTouchLinkClientTargetInfo m_scannedTargetInfo;

        // Class for storing target information
        CQorvoTouchLinkClientTargetInfo m_targetInfo;

        // Stored scan response with highest RSSI
        CQorvoTouchLinkScanResponseData m_storedScanRsp;
        
        // Encrypted Network key used in the touchLink commissioning
        SecKey128Bit_t abEncryptedNetworkKey;

        // Current status of the TouchLink commissioning
        unsigned char m_nTouchLinkStatus;

        // Current network update ID
        unsigned char m_nNetworkUpdateId;
        
        // Current network channel backup before start scanning
        unsigned char m_nCurrentNetworkChannel;

        // Flag to issue reset to factory new command
        bool m_bResetToFactoryNew;

    public:
        // read only access for internal class - target information
        const CQorvoTouchLinkClientTargetInfo& getTargetInfo() const{ return m_targetInfo;}
        void setTargetStatus(unsigned char targetStatus) {m_targetInfo.targetStatus = targetStatus;}
        const CQorvoTouchLinkClientTargetInfo& getScannedTargetInfo() const{ return m_scannedTargetInfo;}
        const CQorvoTouchLinkScanResponseData& getPotentialTargetInfo() const{ return m_storedScanRsp;}

    // Overrides
    public:
        // Called on update of Touchlink comissioning. Target detected, network started, Router Joined
        virtual void OnUpdateCommissioningInfo(const unsigned char nStatus, const CZigBeeAddress &destination);
        
    // Operations
    public:
        // Start TouchLink Commissioning - Scan Request loop 
        void StartScanning(bool resetToFactoryNewRequest = false, bool forceSecondaryScan = false);
        
        // Stop TouchLink Commissioning
        void StopScanning(void);
        
        // return true if current transaction session is active
        bool IsSessionActive(void);

        // Send Bind command to all mandatory clusters on all endpoints 
        void bindEndpoints(void);
        
    // Callback
    public:
        // Callback when touchlink commissioning status is changed
        virtual void OnTouchLinkCommissioningStatus(const unsigned char nState);
        
    protected:
        // Handler for looping the scan request
        static void LoopScanRequests(void* pArg);
        
        // Handler for transaction Session timeout
        static void SessionTimeout(void* pArg);
        
        // Start to trigger the detected target to start a new network.
        // called when device is factory new
        static void TargetStartNetwork(void* pArg);
        
        // Start to trigger the detected target to join initiator's network.
        // called when device is NOT factory new
        static void TargetJoinNetwork(void* pArg);

        // Start to trigger the detected target to reset to factory new.
        static void TargetResetToFactoryNew(void* pArg);
        
        // Delayed function to apply network parameters to newly discovered device
        static void ApplyNetworkParams(void* pArg);

        static void RestartStack();

        // Establish connection over TouchLink
        void ConnectTouchlink(void);

        // Reset saved scan response
        void resetScanResponse(void);

        // Store scan response
        void storeScanResponse(CZigBeeTouchlinkScanResponse scanRsp,
            uint64_t m_qwExtended,
            int8_t nRSSI,
            unsigned short keyBitMask,
            uint8_t scanChannel);

        // Initialize and reset target info
        void resetTargetInfo(void);
        
        // General field settings for network start/join commands
        bool configureNetworkCmdGeneralFields(void);

        // update the new network parameters into stack, reboot will be triggered afterward
        void ApplyToStack(CZigBeeTouchlinkCommissioningInfo info);
        
    // Receipt of Server to client command handling
    public:
        // Handler for Scan Response command
        void OnScanResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Device Information Response command
        void OnDeviceInformationResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Network Start Response command
        void OnNetworkStartResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Network Join Router Response command
        void OnNetworkJoinRouterResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Network Join End Device Response command
        void OnNetworkJoinEndDeviceResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Endpoint Information command
        void OnEndpointInformation(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        // Handler for Get Group Identifiers Response command
        void OnGetGroupIdentifiersResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Get Endpoint List Response command
        void OnGetEndpointListResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Operations of client to server command
    private:
        // Assembles and transmit Scan Request command
        void ScanRequest(const CZigBeeAddress &destination,
            unsigned int transactionId, unsigned char zigbeeInformation,
            unsigned char touchLinkInformation);
            
        // Assembles and transmit Device Information Request command
        void DeviceInformationRequest(const CZigBeeAddress &destination,
            unsigned int transactionId, unsigned char startIndex);
            
        // Assembles and transmit Identify Request command
        void IdentifyRequest(const CZigBeeAddress &destination,
            unsigned int transactionId, unsigned short identifyDuration);
            
        // Assembles and transmit Reset to Factory New Request command
        void ResetToFactoryNewRequest(const CZigBeeAddress &destination,
            unsigned int transactionId);
            
        // Assembles and transmit Network Start Request command
        void NetworkStartRequest(const CZigBeeAddress &destination,
            unsigned int transactionId, MACAddress_t extendedPanId,
            unsigned char keyIndex, SecKey128Bit_t encryptedNetworkKey,
            unsigned char logicalChannel, unsigned short panId,
            unsigned short networkAddress, unsigned short groupIdBegin,
            unsigned short groupIdEnd, unsigned short freeNetworkAddressRangeBegin,
            unsigned short freeNetworkAddressRangeEnd, unsigned short freeGroupIdentifierRangeBegin,
            unsigned short freeGroupIdentifierRangeEnd, MACAddress_t initiatorIeeeAddress,
            unsigned short initiatorNetworkAddress);
            
        // Assembles and transmit Network Join Reouter Request command
        void NetworkJoinRouterRequest(const CZigBeeAddress &destination,
            unsigned int transactionId, MACAddress_t extendedPanId,
            unsigned char keyIndex, SecKey128Bit_t encryptedNetworkKey,
            unsigned char networkUpdateId, unsigned char logicalChannel,
            unsigned short panId, unsigned short networkAddress,
            unsigned short groupIdBegin, unsigned short groupIdEnd, 
            unsigned short freeNetworkAddressRangeBegin, unsigned short freeNetworkAddressRangeEnd,
            unsigned short freeGroupIdentifierRangeBegin, unsigned short freeGroupIdentifierRangeEnd);
            
        // Assembles and transmit Network Join End Device Request command
        void NetworkJoinEndDeviceRequest(const CZigBeeAddress &destination,
            unsigned int transactionId, MACAddress_t extendedPanId,
            unsigned char keyIndex, SecKey128Bit_t encryptedNetworkKey,
            unsigned char networkUpdateId, unsigned char logicalChannel,
            unsigned short panId, unsigned short networkAddress,
            unsigned short groupIdBegin, unsigned short groupIdEnd, 
            unsigned short freeNetworkAddressRangeBegin, unsigned short freeNetworkAddressRangeEnd,
            unsigned short freeGroupIdentifierRangeBegin, unsigned short freeGroupIdentifierRangeEnd);
            
        // Assembles and transmit Network Update Request command
        void NetworkUpdateRequest(const CZigBeeAddress &destination,
            unsigned int transactionId, MACAddress_t extendedPanId,
            unsigned char networkUpdateId, unsigned char logicalChannel,
            unsigned short panId, unsigned short networkAddress);

        // Identify potential target and optionally retrieve device information
        void IdentifyPotentialTarget(void);

    // Function for client to server command
    public:
        // Assembles and transmit Get Group Identifier Request command
        void GetGroupIdentifiersRequest(const CZigBeeAddress &destination,
            unsigned char startIndex);
            
        // Assembles and transmit Get Endpoint List Request command
        void GetEndpointListRequest(const CZigBeeAddress &destination,
            unsigned char startIndex);
};

#define __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_NWKADDR_RANGE_BEGIN(cls, member) \
    __COMPACT7B_ATTRIBUTE_CLUSTER_M(cls, \
        CZigBeeDeviceObject::wManufacturerID, \
        CQorvoClusterTouchLinkCommissioningClient::attributeFreeNetworkAddressRangeBegin, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_NWKADDR_RANGE_END(cls, member) \
    __COMPACT7B_ATTRIBUTE_CLUSTER_M(cls, \
        CZigBeeDeviceObject::wManufacturerID, \
        CQorvoClusterTouchLinkCommissioningClient::attributeFreeNetworkAddressRangeEnd, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_GROUPID_RANGE_BEGIN(cls, member) \
    __COMPACT7B_ATTRIBUTE_CLUSTER_M(cls, \
        CZigBeeDeviceObject::wManufacturerID, \
        CQorvoClusterTouchLinkCommissioningClient::attributeFreeGroupIdRangeBegin, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_GROUPID_RANGE_END(cls, member) \
    __COMPACT7B_ATTRIBUTE_CLUSTER_M(cls, \
        CZigBeeDeviceObject::wManufacturerID, \
        CQorvoClusterTouchLinkCommissioningClient::attributeFreeGroupIdRangeEnd, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::manufacturerSpecific|CZigBeeAttribute::persistent|CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_CLUSTER_ATTRIBUTES() \
    __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_NWKADDR_RANGE_BEGIN(CQorvoClusterTouchLinkCommissioningClient, m_nFreeNetworkAddressRangeBegin) \
    __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_NWKADDR_RANGE_END(CQorvoClusterTouchLinkCommissioningClient, m_nFreeNetworkAddressRangeEnd) \
    __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_GROUPID_RANGE_BEGIN(CQorvoClusterTouchLinkCommissioningClient, m_nFreeGroupIdRangeBegin) \
    __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_FREE_GROUPID_RANGE_END(CQorvoClusterTouchLinkCommissioningClient, m_nFreeGroupIdRangeEnd) \
    /*End*/
#define __COMPACT7B_TOUCHLINKCOMMISSIONINGCLIENT_CLUSTER_COMMANDS() \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandScanResponse, &CQorvoClusterTouchLinkCommissioningClient::OnScanResponse) \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandDeviceInformationResponse, &CQorvoClusterTouchLinkCommissioningClient::OnDeviceInformationResponse)  \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandNetworkStartResponse, &CQorvoClusterTouchLinkCommissioningClient::OnNetworkStartResponse)  \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandNetworkJoinRouterResponse, &CQorvoClusterTouchLinkCommissioningClient::OnNetworkJoinRouterResponse)  \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandNetworkJoinEndDeviceResponse, &CQorvoClusterTouchLinkCommissioningClient::OnNetworkJoinEndDeviceResponse)  \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandEndpointInformation, &CQorvoClusterTouchLinkCommissioningClient::OnEndpointInformation)  \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandGetGroupIdentifiersResponse, &CQorvoClusterTouchLinkCommissioningClient::OnGetGroupIdentifiersResponse)   \
    __COMPACT7B_STUBCOMMAND_S2C(CQorvoClusterTouchLinkCommissioningClient::commandGetEndpointListResponse, &CQorvoClusterTouchLinkCommissioningClient::OnGetEndpointListResponse)  \
    /*End*/

#endif // _ZIGBEE_CLUSTER_TOUCHLINKCOMMISSIONING_H_

#define __COMPACT7B_STUBCOMMAND_C2S(command, pfn) { command, CZigBeeClusterCommand::typeStub, 0, static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },
