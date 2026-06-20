// ZigBeeClusterScenes.h : header file
//
// Copyright(C) 2009-2015 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the scenes cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterScenes

class CZigBeeClusterScenes : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the scenes cluster
		enum { id = 0x0005 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Cluster command identifiers (client to server)
		enum { commandAddScene, commandViewScene, commandRemoveScene,
			commandRemoveAllScenes, commandStoreScene, commandRecallScene,
			commandGetSceneMembership, commandEnhancedAddScene = 0x40,
			commandEnhancedViewScene, commandCopyScene };

		// Cluster command identifiers (server to client)
		enum { commandAddSceneResponse, commandViewSceneResponse,
			commandRemoveSceneResponse, commandRemoveAllScenesResponse,
			commandStoreSceneResponse, commandReserved1,
			commandGetSceneMembershipResponse,
			commandEnhancedAddSceneResponse = 0x40,
			commandEnhancedViewSceneResponse, commandCopySceneResponse };

		// Attribute identifiers of the identify cluster
		enum { attributeSceneCount, attributeCurrentScene,
			attributeCurrentGroup, attributeSceneValid, attributeNameSupport,
			attributeLastConfiguredBy };

		enum { globalGroupID = 0, globalSceneID = 0 };

	// Construction
	public:
		CZigBeeClusterScenes(CZigBeeFoundationApplication &application);

	// Operations
	public:
		// Creates or updates a scene table entry
		unsigned char StoreScene(const unsigned short wGroupID,
			const unsigned char nSceneID, CPacket &scene,
			const bool bEnhanced = false);

		// Retrieves a scene table entry
		unsigned char LoadScene(const unsigned short wGroupID,
			const unsigned char nSceneID, CPacket &scene, bool &bEnhanced);

		// Notifies the scene cluster about removal of a group (or all groups,
		// if the group ID is null)
		void OnRemoveGroup(const unsigned short wGroupID = 0);

		// Notifies the scene cluster about an attribute change on a cluster
		// on the same endpoint that has scene table extension fields
		void OnSceneAttributeChanged();

		// Stores the current settings in new or existing scene table entry
		unsigned char StoreScene(const unsigned short wGroupID = globalGroupID,
			const unsigned char nSceneID = globalSceneID,
			const unsigned short wTransitionTime = 0);

		// Recalls the specified scene
		unsigned char RecallScene(const unsigned short wGroupID = globalGroupID,
			const unsigned char nSceneID = globalSceneID,
			const unsigned short wTransitionTime =
				static_cast<unsigned short>(-1));

	// Overrides
	public:
		// Called in an early stage when the device is powered-up. Used to set
		// the scene count correctly after power-up
		virtual void AfterPowerup();

	// Implementation
	public:
		// Revision of the Scenes cluster
		static const unsigned short nClusterRevision;

		// Limit on the scene table's capacity; a value of 0 indicates no
		// explicit capacity limitation
		static const uint8_t nCapacityLimit;

		// Upper bound for a scene table entry including transition time, name
		// in header and extension field sets in trailer portions
		enum { sceneTrailerCapacity = 96, sceneHeaderCapacity = 19 };

		// Command options for the copy scene command
		enum { modeCopyAllScenes = 0x01 };

		// Attribute storage for attribute 0x0004, global scope, read-only
		static const unsigned char bNameSupport;

		// Attribute storage for attribute 0x0002, read-only
		unsigned short m_nCurrentGroup;

		// Attribute storage for attribute 0x0001, read-only
		unsigned char m_nCurrentScene;

		// Attribute storage for attribute 0x0000, read-only
		unsigned char m_nSceneCount;

		// Attribute storage for attribute 0x0003, read-only
		bool m_bSceneValid;

		// Stores scene extension field sets of clusters on the same endpoint
		bool AppendSceneAttributes(CPacket &packet) const;

		// Command handler for "add scene" and "enhanced add scene"
		void OnAddScene(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "view scene" and "enhanced view scene"
		void OnViewScene(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "remove scene"
		void OnRemoveScene(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "remove all scenes"
		void OnRemoveAllScenes(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "store scene"
		void OnStoreScene(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "recall scene"
		void OnRecallScene(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "get scene membership"
		void OnGetSceneMembership(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Command handler for "copy scene"
		void OnCopyScene(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Transmits a common scenes cluster response frame (add scene
		// response, remove scene response)
		void TransmitResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned char nCommand, const unsigned short wGroupID,
			const unsigned char nSceneID,
			const unsigned char nStatus = statusSuccess);

		// Recalculates the scene count based on the number of persistent items
		void UpdateSceneCount();
};

#define __COMPACT7B_SCENES_SCENECOUNT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterScenes::attributeSceneCount, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_SCENES_CURRENTSCENE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterScenes::attributeCurrentScene, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_SCENES_CURRENTGROUP(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterScenes::attributeCurrentGroup, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_SCENES_SCENEVALID(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterScenes::attributeSceneValid, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_SCENES_NAMESUPPORT() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterScenes, \
		CZigBeeClusterScenes::attributeNameSupport, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly, bNameSupport)

#define __COMPACT7B_SCENES_LASTCONFIGUREDBY(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterScenes::attributeLastConfiguredBy, \
		CZigBeeAttribute::typeIEEEAddress, 0, \
		CZigBeeAttribute::accessReadOnly, member)
