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


#ifndef _QORVO_CLUSTER_IASZONE_CLIENT_H_
#define _QORVO_CLUSTER_IASZONE_CLIENT_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterIASZoneClient

class CQorvoClusterIASZoneClientCallbacks
{
    public:
        // Called on reception of a ZoneEnrollRequest
        typedef void (*ZoneEnrollRequest)(const CZigBeeAddress &source,
            unsigned short zoneType, unsigned short manufacturerCode);
        // Called on reception of a ZoneStatusNotification
		typedef void (*ZoneStatusNotification)(const CZigBeeAddress &source,
		    unsigned short IasZoneStatus, unsigned char extendedStatus, unsigned char zoneId, unsigned short delay);
    
    public:
        CQorvoClusterIASZoneClientCallbacks(ZoneEnrollRequest cbZoneEnroll, 
                                            ZoneStatusNotification cbZoneStatusNot) :
                                            m_ZoneEnrollRequest(cbZoneEnroll),
                                            m_ZoneStatusNotification(cbZoneStatusNot) {}

    public:
        ZoneEnrollRequest      m_ZoneEnrollRequest;
        ZoneStatusNotification m_ZoneStatusNotification;
                                             
};

class CQorvoClusterIASZoneClient : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()
    __COMPACT7B_DECLARE_COMMAND_MAP()

    // typedefs & enums
    public:
        // Cluster identifier of the IASZone cluster
        enum { id = 0x0500 };

        // Cluster revision of this implementation
        enum { revision = 2 };

        // Cluster command identifiers
        enum { 
            commandZoneEnrollResponse = 0x00,
            commandInitiateNormalOperationMode = 0x01,
            commandInitiateTestMode = 0x02,
            commandZoneStatusChangeNotification = 0x00,
            commandZoneEnrollRequest = 0x01
        };
        
        // Identifiers of the IAS Zone cluster
        enum { 
            attributeZoneState = 0x0000,
            attributeZoneType = 0x0001,
            attributeZoneStatus = 0x0002,
            attributeIasCieAddress = 0x0010,
            attributeZoneId = 0x0011,
            attributeNumberOfZoneSensitivityLevelsSupported = 0x0012,
            attributeCurrentZoneSensitivityLevel = 0x0013,
        };
        
        // Enrollment response status
        enum {
            EnrollSuccess = 0x0,
            EnrollNotSupported =0x1,
            EnrollNoEnrollPermit = 0x2,
            EnrollTooManyZones = 0x3,
        };
        
    // Construction
    public:
        CQorvoClusterIASZoneClient(CZigBeeFoundationApplication &application);
        
        CQorvoClusterIASZoneClient(CZigBeeFoundationApplication &application, 
                                   const CQorvoClusterIASZoneClientCallbacks &callbacks);

    // Internal members
    private:
        CQorvoClusterIASZoneClientCallbacks m_callbacks;
        
    // Client to server command handling
    public:
        void ZoneEnrollResponse(const CZigBeeAddress &destination,
             unsigned char enrollResponseCode, unsigned char zoneId);
        
        void InitiateNormalOperationMode(const CZigBeeAddress &destination);
        void InitiateTestMode(const CZigBeeAddress &destination, unsigned char testModeDuration, unsigned char currentZoneSensitivityLevel);

    // Server to client command handling
    protected:
        void OnZoneStatusChangeNotification(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);
        
        void OnZoneEnrollRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

    // Implementation
    public:
        // Revision of the IASZone client cluster
        static const unsigned short nClusterRevision;
};

#define __COMPACT7B_IASZONE_CLUSTER_COMMANDS() \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterIASZoneClient::commandZoneStatusChangeNotification, &CQorvoClusterIASZoneClient::OnZoneStatusChangeNotification) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterIASZoneClient::commandZoneEnrollRequest, &CQorvoClusterIASZoneClient::OnZoneEnrollRequest) \
    /*End*/

#endif // _QORVO_CLUSTER_IASZONE_CLIENT_H_
