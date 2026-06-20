/*
 * Copyright (c) 2019, Qorvo Inc
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
#include <list>
#include <set>
#include <map>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Compact15dot4.h"
#include "Packet.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeClusterLibrary.h"
#include "gpLog.h"
#include "gpAssert.h"

#include "QorvoClusterGroupsClient.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

#define CharString_api2buf(pDest, pSource, len, pIndex)  generic_api2buf_1(pDest, pSource, pIndex, len)

#define MAX_GROUPNAME_LEN 16

/////////////////////////////////////   ////////////////////////////////////////
// CQorvoClusterGroupsClient
const unsigned short CQorvoClusterGroupsClient::nClusterRevision =
    CQorvoClusterGroupsClient::revision;

CQorvoClusterGroupsClient::CQorvoClusterGroupsClient
    (CZigBeeFoundationApplication &application) :
CZigBeeCluster(application, id, flagOutbound)
{
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterColorControl server to client command handlers

void CQorvoClusterGroupsClient::AddGroupResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{

    // GP_LOG_SYSTEM_PRINTF("== into AddGroupResponse ==", 0);
    // Transmit a default response frame if needed
    // TransmitDefaultResponse(destination, source, header, status);
}
void CQorvoClusterGroupsClient::ViewGroupResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status = statusUnsupportedClusterCommand;

    // Transmit a default response frame if needed
    TransmitDefaultResponse(destination, source, header, status);
}
void CQorvoClusterGroupsClient::GetGroupMembershipResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status = statusUnsupportedClusterCommand;

    // Transmit a default response frame if needed
    TransmitDefaultResponse(destination, source, header, status);
}
void CQorvoClusterGroupsClient::RemoveGroupResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{

    unsigned char status = statusUnsupportedClusterCommand;

    // Transmit a default response frame if needed
    TransmitDefaultResponse(destination, source, header, status);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterColorControl client to server command handlers

void CQorvoClusterGroupsClient::AddGroup(
    const CZigBeeAddress &destination,
    UInt16 groupId,
    char * groupName
)
{ 
    GP_ASSERT_SYSTEM(groupName[0] <= MAX_GROUPNAME_LEN);

    UInt16 index=0;
    CPacket AddGroupPacket;
    UInt8 groupNameLength = groupName[0];
    unsigned char pData[sizeof(UInt16) + MAX_GROUPNAME_LEN * sizeof(char)];

    UInt16_api2buf(pData, &groupId, 1, &index);
    UInt8_api2buf(pData, &(groupNameLength), 1, &index); // Length is first byte of string
    memcpy(pData+index,&(groupName[1]),groupNameLength); 
    index +=groupName[0]; 

    CreateRequest(AddGroupPacket, 
                  static_cast<unsigned char>(commandAddGroup),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse, 
                  0);
    AddGroupPacket.Append(pData, index);

    // AddGroupPacket.Dump();
    // GP_LOG_SYSTEM_PRINTF("destination.m_qwExtended[0x%x]",0, destination.m_qwExtended);
    GP_LOG_SYSTEM_PRINTF("groupId[%d]",0, groupId);
    GP_LOG_SYSTEM_PRINTF("destination.m_wPANID    [0x%x]",0, destination.m_wPANID);
    // GP_LOG_SYSTEM_PRINTF("destination.m_wShort    [0x%x]",0, destination.m_wShort);
    GP_LOG_SYSTEM_PRINTF("destination.m_nEndpoint [0x%x]",0, destination.m_nEndpoint);
    gpLog_Flush();

    OnRequestData(destination, AddGroupPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterGroupsClient::ViewGroup(const CZigBeeAddress &destination,
    UInt16 groupId
)
{
    CPacket ViewGroupPacket;
    unsigned char pData[sizeof(UInt16)];
    unsigned short index = 0;
    
    UInt16_api2buf(pData, &groupId, 1, &index);

    CreateRequest(ViewGroupPacket, 
                  static_cast<unsigned char>(commandViewGroup),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse, 
                  0);
    ViewGroupPacket.Append(pData, index);

    OnRequestData(destination, ViewGroupPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}
void CQorvoClusterGroupsClient::GetGroupMembership(const CZigBeeAddress &destination,
    UInt8 groupCount,
    UInt16 groupList
)
{
    CPacket GetGroupMembershipPacket;
    unsigned char pData[sizeof(UInt8) + sizeof(UInt16)];
    unsigned short index = 0;
    
    UInt8_api2buf(pData, &groupCount, 1, &index);
    UInt16_api2buf(pData, &groupList, 1, &index);

    CreateRequest(GetGroupMembershipPacket, 
                  static_cast<unsigned char>(commandGetGroupMembership),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse, 
                  0);
    GetGroupMembershipPacket.Append(pData, index);

    OnRequestData(destination, GetGroupMembershipPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}
void CQorvoClusterGroupsClient::RemoveGroup(const CZigBeeAddress &destination,
    UInt16 groupId
)
{
    CPacket RemoveGroupPacket;
    unsigned char pData[sizeof(UInt16)];
    unsigned short index = 0;
    
    UInt16_api2buf(pData, &groupId, 1, &index);

    CreateRequest(RemoveGroupPacket, 
                  static_cast<unsigned char>(commandRemoveGroup),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse, 
                  0);
    RemoveGroupPacket.Append(pData, index);

    OnRequestData(destination, RemoveGroupPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}
void CQorvoClusterGroupsClient::RemoveAllGroups(const CZigBeeAddress &destination
    
)
{
    CPacket RemoveAllGroupsPacket;

    CreateRequest(RemoveAllGroupsPacket, 
                  static_cast<unsigned char>(commandRemoveAllGroups),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse, 
                  0);

    OnRequestData(destination, RemoveAllGroupsPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}
void CQorvoClusterGroupsClient::AddGroupIfIdentifying(const CZigBeeAddress &destination,
    UInt16 groupId,
    char * groupName
)
{
    CPacket AddGroupIfIdentifyingPacket;
    unsigned char pData[sizeof(UInt16) + 16]; // charstring size is 16
    unsigned short index = 0;
    
    UInt16_api2buf(pData, &groupId, 1, &index);
    CharString_api2buf(pData, groupName, 16, &index);

    CreateRequest(AddGroupIfIdentifyingPacket, 
                  static_cast<unsigned char>(commandAddGroupIfIdentifying),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse, 
                  0);
    AddGroupIfIdentifyingPacket.Append(pData, index);

    OnRequestData(destination, AddGroupIfIdentifyingPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}


/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterGroupsClient command map

__COMPACT7B_IMPLEMENT_COMMAND_MAP(CQorvoClusterGroupsClient, CZigBeeCluster)

__COMPACT7B_BEGIN_COMMAND_MAP(CQorvoClusterGroupsClient)
    __COMPACT7B_GROUPSCLIENT_CLUSTER_COMMANDS()
__COMPACT7B_END_COMMAND_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterGroupsClient attribute map

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterGroupsClient,
	CZigBeeCluster)

__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterGroupsClient)
	__COMPACT7B_CLUSTERREVISION(CQorvoClusterGroupsClient)
__COMPACT7B_END_ATTRIBUTE_MAP()
