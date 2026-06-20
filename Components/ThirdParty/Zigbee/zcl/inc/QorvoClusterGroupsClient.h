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

#ifndef _QORVO_CLUSTER_GROUPSCLIENT_H_
#define _QORVO_CLUSTER_GROUPSCLIENT_H_

#include "global.h"

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterGroupsClient
class CQorvoClusterGroupsClient : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()
    __COMPACT7B_DECLARE_COMMAND_MAP()

    // typedefs & enums
    public:
        // Cluster identifier of Groups cluster
        enum { id = 0x0004 };

        // Cluster revision of this implementation
        enum { revision = 3 };

        // Cluster command identifiers
        enum { 
            commandAddGroup = 0x00,                            /* Mandatory */
            commandViewGroup = 0x01,                           /* Mandatory */
            commandGetGroupMembership = 0x02,                  /* Mandatory */
            commandRemoveGroup = 0x03,                         /* Mandatory */
            commandRemoveAllGroups = 0x04,                     /* Mandatory */
            commandAddGroupIfIdentifying = 0x05,               /* Mandatory */
            commandAddGroupResponse = 0x00,                    /* Mandatory */
            commandViewGroupResponse = 0x01,                   /* Mandatory */
            commandGetGroupMembershipResponse = 0x02,          /* Mandatory */
            commandRemoveGroupResponse = 0x03,                 /* Mandatory */
        };

        // Identifiers of the Groups cluster
        enum { 
            attributeGroupNameSupport = 0x0000,                /* Mandatory */
        };

    // Construction
    public:
        CQorvoClusterGroupsClient
            (CZigBeeFoundationApplication &application);

    // Server to client command handling
    protected:
        void AddGroupResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        void ViewGroupResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        void GetGroupMembershipResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        void RemoveGroupResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
    
    // Client to server command handling
    public:
        void AddGroup(const CZigBeeAddress &destination,
                unsigned short groupId,
                char* groupName
             );
        void ViewGroup(const CZigBeeAddress &destination,
                unsigned short groupId
             );
        void GetGroupMembership(const CZigBeeAddress &destination,
                unsigned char groupCount,
                unsigned short groupList
             );
        void RemoveGroup(const CZigBeeAddress &destination,
                unsigned short groupId
             );
        void RemoveAllGroups(const CZigBeeAddress &destination
                
             );
        void AddGroupIfIdentifying(const CZigBeeAddress &destination,
                unsigned short groupId,
                char* groupName
             );

    // Implementation
    public:
        // Revision of the Groups client cluster
        static const unsigned short nClusterRevision;
};


#define __COMPACT7B_GROUPSCLIENT_CLUSTER_COMMANDS() \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterGroupsClient::commandAddGroupResponse, &CQorvoClusterGroupsClient::AddGroupResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterGroupsClient::commandViewGroupResponse, &CQorvoClusterGroupsClient::ViewGroupResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterGroupsClient::commandGetGroupMembershipResponse, &CQorvoClusterGroupsClient::GetGroupMembershipResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterGroupsClient::commandRemoveGroupResponse, &CQorvoClusterGroupsClient::RemoveGroupResponse) \
    /*End*/

#endif // _ZIGBEE_CLUSTER_GROUPSCLIENT_H_
