// ZigBeeClusterGroups.h : header file
//
// Copyright(C) 2009-2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the groups cluster


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterGroups

class CZigBeeClusterGroups : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the groups cluster
		enum { id = 0x0004 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Cluster command identifiers (client to server)
		enum { commandAddGroup, commandViewGroup, commandGetGroupMembership,
			commandRemoveGroup, commandRemoveAllGroups,
			commandAddGroupIfIdentifying };

		// Cluster command identifiers (server to client)
		enum { commandAddGroupResponse, commandViewGroupResponse,
			commandGetGroupMembershipResponse, commandRemoveGroupResponse };

		// Attribute identifiers of the identify cluster
		enum { attributeNameSupport };

	// Construction
	public:
		CZigBeeClusterGroups(CZigBeeFoundationApplication &application);

	// Overrides
	public:

	// Implementation
	public:
		// Revision of the Groups cluster
		static const unsigned short nClusterRevision;

		// Attribute storage for attribute 0x0000, global scope, read-only
		static const unsigned char bNameSupport;

		// Command handler for "add group"
		void OnAddGroup(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "view group"
		void OnViewGroup(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "get group membership"
		void OnGetGroupMembership(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "remove group"
		void OnRemoveGroup(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "remove all groups"
		void OnRemoveAllGroups(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "add group if identifying"
		void OnAddGroupIfIdentifying(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Transmits a common group cluster response frame (add group response,
		// remove group response)
		void TransmitResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned char nCommand, const unsigned short wGroupID,
			const unsigned char nStatus = statusSuccess);

		// Implements the "add group" and "add group if identifying" commands.
		// Notice that in the second case (if identifying) this method should
		// only be called if the device is actually identifying
		void AddGroup(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const bool bIdentifying = false);
};

#define __COMPACT7B_GROUPS_NAMESUPPORT() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterGroups, \
		CZigBeeClusterGroups::attributeNameSupport, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly, bNameSupport)
