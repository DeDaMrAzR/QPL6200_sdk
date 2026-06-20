// ZigBeeFoundationApplication.h : header file
//
// Copyright(C) 2009-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Provides an application foundation that suites ZigBee Cluster Library
// paradigms and provides support for attributes with global scope,
// application class scope, or application instance scope. Supports multiple
// instances of the same application (for example multi-channel load switch
// providing multiple endpoints), and a single application serving multiple
// end-points

/////////////////////////////////////////////////////////////////////////////
// CZigBeeFoundationCookie

class CZigBeeFoundationCookie
{
	// Construction
	public:
		CZigBeeFoundationCookie(CZigBeeCluster *const m_pCluster,
			void *const pCookie = 0);

	// Attributes
	public:
		// User-specified cookie
		void *m_pCookie;

		// Identifies the cluster that originated this request
		CZigBeeCluster *m_pCluster;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeFoundationApplication

class CZigBeeFoundationApplication : public CZigBeeApplication
{
	// typedefs & enums
	public:
		// Application access control privilege levels
		enum { privilegeNone, privilegePeer, privilegeTrustCenter };

		// Foundation application access restriction models: unrestricted
		// access, only talk to peer devices with an application link-key, only
		// talk to the trust center (require trust center link-key), or inherit
		// the settings from global (node) flags
		enum { policyUnrestricted = privilegeNone,
			policyPeerOnly = privilegePeer,
			policyTrustCenterOnly = privilegeTrustCenter, policyInherit };

		// These are application flags that identify the application as a
		// ZigBee foundation application with ZCL support. The commissioning
		// flags specify whether the application shall be selected as initiator
		// and/or target for push-button commissioning (EZ-mode). In addition,
		// an application endpoint can be exposed over touchlink, possibly with
		// an alternate device ID for backwards compatibility with the legacy
		// Light Link profile. For a touchlink-enabled endpoint the upper byte
		// of the flags contains an optional sort order (zero defaults to no
		// specific sort order) and an optional number of group identifiers to
		// be allocated to the device. Access control policy, acknowledgment
		// request policy and default response request policy are also
		// available as configurable items
		enum { commissioningInitiator = 0x01, commissioningTarget = 0x02,
			touchlink = 0x04, hasAlternateDeviceID = 0x08,
			alternateDeviceVersionMask = 0x0f00, alternateDeviceVersionShift = 8,
			touchlinkSortOrderMask = 0xf000, touchlinkSortOrderShift = 12,
			touchlinkGroupCountMask = 0xff0000, touchlinkGroupCountShift = 16,
			accessShift = 24, accessMask = 3 << accessShift,
			accessUnrestricted = policyUnrestricted << accessShift,
			accessPeerOnly = policyPeerOnly << accessShift,
			accessTrusterCenterOnly = policyTrustCenterOnly << accessShift,
			accessInherit = policyInherit << accessShift,
			acknowledgmentPolicyShift = 26,
			acknowledgmentPolicyMask = 3 << acknowledgmentPolicyShift,
			acknowledgmentPolicyDefault = 0 << acknowledgmentPolicyShift,
			acknowledgmentPolicyAlways = 1 << acknowledgmentPolicyShift,
			acknowledgmentPolicyNever = 2 << acknowledgmentPolicyShift,
			defaultResponsePolicyShift = 28,
			defaultResponsePolicyMask = 3 << defaultResponsePolicyShift,
			defaultResponsePolicyDefault = 0 << defaultResponsePolicyShift,
			defaultResponsePolicyAlways = 1 << defaultResponsePolicyShift,
			defaultResponsePolicyNever = 2 << defaultResponsePolicyShift };

		// Global foundation application configuration flags, e.g. device-wide
		// cluster policies
		enum { globalAccessShift = 0,
			globalAccessMask = 3 << globalAccessShift,
			globalAccessUnrestricted = policyUnrestricted << globalAccessShift,
			globalAccessPeerOnly = policyPeerOnly << globalAccessShift,
			globalAccessTrusterCenterOnly =
				policyTrustCenterOnly << globalAccessShift,
			globalAcknowledgmentPolicyDefault = 0x0000,
			globalAcknowledgmentPolicyAlways = 0x0010,
			globalAcknowledgmentPolicyNever = 0x0020,
			globalAcknowledgmentPolicyMask = 0x0030,
			globalAcknowledgmentPolicyShift = 4,
			globalDefaultResponsePolicyDefault = 0x0000,
			globalDefaultResponsePolicyAlways = 0x0040,
			globalDefaultResponsePolicyNever = 0x0080,
			globalDefaultResponsePolicyMask = 0x00c0,
			globalDefaultResponsePolicyShift = 6,
			globalPersistentMask = 0x7fff, globalLoaded = 0x8000 };

	// Construction
	public:
		CZigBeeFoundationApplication(CZigBeeApplicationSupportBase &aps,
			const uint_fast16_t wProfileID, const uint_fast16_t wDeviceID,
			const uint_fast8_t bDeviceVersion, const uint_fast8_t bEndpoint,
			const uint_fast32_t nFlags = 0,
			const uint_fast16_t wAlternateDeviceID = 0);

	// Attributes
	public:
		// An alternate device ID, which is valid if the application flags have
		// the hasAlternateDeviceID flag set
		uint16_t m_wAlternateDeviceID;

		// Global application flags. These flags apply across all foundation
		// application endpoints, i.e. their scope is node-wide
		static uint16_t nGlobalFlags;

	// Operations
	public:
		// APSDE-DATA.request(DstAddrMode, DstAddress, DstEndpoint, ProfileID,
		// ClusterID, SrcEndpoint, ASDULength, ADSU, TxOptions, Radius). An
		// additional cookie allows the calling application to pass arguments
		// to the confirmation handler in order to simplify distinction of
		// different requests
		void OnRequestData(const CZigBeeAddress &destination,
			CZigBeeCluster &cluster, CPacket &asdu,
			const unsigned char bTransmitOptions, const unsigned char bRadius,
			void *const pCookie = 0,
			const unsigned char bEnhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium);

		// Should be called when an attribute's value has been updated, either
		// by a command or by a write access
		void NotifyAttributeUpdate(const unsigned short wClusterID,
			const unsigned short wAttributeID, const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0);

		// Should be called when an attribute's value is about to be retrieved,
		// either by a read command or by a scheduled attribute report
		void RequestAttributeRefresh(const unsigned short wClusterID,
			const unsigned short wAttributeID, const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0);

		// Modifies global foundation application flags, i.e. those with a
		// device-wide scope, and stores these configuration changes
		// persistently
		static void ModifyGlobalFlags(const uint_fast16_t nSet,
			const uint_fast16_t nClear);

		// Returns the effective access control policy (none, peer only, or
		// trust center only), after resolving inheritance, if applicable
		uint_fast8_t GetRequiredPrivilegeLevel() const;

		// Returns the acknowledgment request policy (default, always, never),
		// after resolving inheritance, if applicable
		uint_fast8_t GetAcknowledgmentPolicy() const;

		// Returns the default response request (or rather "disable") policy
		// (default, always, never) after resolving inheritance, if applicable
		uint_fast8_t GetDefaultResponsePolicy() const;

		// Allows walking cluster maps, starting with the final class up to the
		// base class
		virtual const CZigBeeCluster *const *
		WalkClusterMaps(const CZigBeeCluster *const *const pMap) const;

		// Finds a particular cluster on this application endpoint matching the
		// specified cluster identifier and flags (inbound vs. outbound)
		virtual CZigBeeCluster *
		FindCluster(const unsigned short wClusterID,
			const unsigned char nFlags = 0) const;

		// Finds the first cluster instance on this device matching the
		// specified cluster identifier and flags
		static CZigBeeCluster *FindCluster(CZigBeeApplicationSupportBase &aps,
			const unsigned short wClusterID, const unsigned char nFlags = 0);

	// Overridables
	public:
		// Finds a particular subscription (notification sink) specified by
		// cluster and attribute identifiers. Notice that cluster and attribute
		// identifiers are related to a particular application profile
		// identifier. Each endpoint supports only one profile identifier
		virtual const CZigBeeClusterNotification *
		FindSubscription(const unsigned short wClusterID,
			const unsigned short wAttributeID, const unsigned char nOptions,
			const unsigned short wManufacturerCode) const;

		// APSDE-DATA.request(DstAddrMode, DstAddress, DstEndpoint, ProfileID,
		// ClusterID, SrcEndpoint, ASDULength, ADSU, TxOptions, Radius). An
		// additional cookie allows the calling application to pass arguments
		// to the confirmation handler in order to simplify distinction of
		// different requests. This extended version is provided such that
		// derived classes are able to preprocess outbound message templates
		virtual void OnRequestDataEx(const CZigBeeAddress &destination,
			CZigBeeCluster &cluster, CPacket &asdu,
			const unsigned char bTransmitOptions, const unsigned char bRadius,
			void *const pCookie = 0,
			const unsigned char bEnhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium);

	// Overrides
	public:
		// APSDE-DATA.confirm(DstAddrMode, DstAddress, DstEndpoint,
		// SrcEndpoint, Status, TxTime), clause 2.2.4.1.2
		virtual void OnConfirmData(const CZigBeeAddress &destination,
			const unsigned char bSourceEndpoint, const unsigned char bStatus,
			const unsigned int nTimestamp, void *const pCookie);

		// APSDE-DATA.indication(DstAddrMode, DstAddress, DstEndpoint,
		// SrcAddrMode, SrcAddress, SrcEndpoint, ProfileId, ClusterId,
		// asduLength, asdu, Status, SecurityStatus, LinkQuality, RxTime),
		// clause 2.2.4.1.3
		virtual void OnIndicateData(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wProfileID,
			const unsigned short wClusterID, CPacket &asdu,
			const unsigned char bStatus, const bool bSecurityUsed,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called in an early stage when the device is powered-up. Foundation
		// applications implement automatic loading of cluster attributes
		// marked as "persistent" here
		virtual void AfterPowerup();

		// This notification is invoked upon completion of the ZigBee start-up
		// routine
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// This notification is invoked when a device left the network
		virtual void OnNotifyNetworkLeave(const uint8_t nStatus,
			const bool bRejoin, uint8_t &actionFlags);

		// This notification provides network status notifications, which might
		// either have been generated by the local NLME or were received over
		// the air in a network status command
		virtual void OnNotifyNetworkStatus(const unsigned char nStatus,
			const CZigBeeAddress &address);

		// Called twice during factory reset: Once before persistent storage is
		// reverted to factory fresh settings (stage bit false), and once after
		// the storage has been reverted (stage bit true). Additional bits may
		// be set, which define the scope of the reset, e.g. if this is a full
		// factory reset. A full factory reset wipes security frame counters as
		// well as all attributes which are not marked as "preserved". It is
		// possible to define additional reset behavior, which is specific to a
		// particular application and sets a new operating mode, for example
		virtual void OnFactoryReset(const unsigned int nOptions);

		// Handles a ZDO bind or unbind request for the application
		virtual unsigned char ProcessBindOrUnbindRequest
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const CZigBeeAddress &originator);

		// Deletes all persistent items stored by the foundation application.
		// This is useful for implementing partial factory resets or removing
		// application endpoints at run-time
		virtual void RestoreDefaults(const bool bRemoveAllReferences = true);

	// Implementation
	protected:
		// Performs a lookup using the supplied profile, cluster and attribute
		// identifiers. Returns null if no appropriate attribute was found
		CZigBeeCluster *FindCluster(const unsigned short wClusterID,
			const unsigned char nFlags,
			const CZigBeeCluster *const *const pMap) const;

		// Creates a list of input or output clusters given the cluster map.
		// Clusters that are either concealed or inactive will not be listed
		void DeterminePublicClusters(std::set<unsigned short> &clusters,
			const unsigned char nFlags,
			const CZigBeeCluster *const *const pMap) const;

		// Finds a particular subscription (notification sink) specified by
		// cluster and attribute identifiers. Notice that cluster and attribute
		// identifiers are related to a particular application profile
		// identifier. Each endpoint supports only one profile identifier
		static const CZigBeeClusterNotification *
		FindSubscription(const unsigned short wClusterID,
			const unsigned short wAttributeID, const unsigned char nOptions,
			const unsigned short wManufacturerCode,
			const CZigBeeClusterNotification *const pMap);

		// Processes a data indication by dispatching to registered clusters;
		// returns true if the message has been processed and no further
		// processing shall take place. Returns false to indicate that there
		// was no matching cluster and the application should process the
		// message. If bGenerateDefaultResponse is true, this method returns
		// true if the cluster does not exist and issues a default response
		// with a status of UNSUPPORTED_CLUSTER towards the requesting device
		bool ProcessDataIndication(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wProfileID,
			const unsigned short wClusterID, CPacket &asdu,
			const unsigned char bStatus, const bool bSecurityUsed,
			const unsigned char nLinkQuality, const unsigned int nTimestamp,
			const bool bGenerateDefaultResponse = true);

#ifdef _DEBUG
	public:
		// Prints application state information to the debug output
		virtual void Dump() const;
#endif

};


#define __COMPACT7B_DECLARE_CLUSTER_MAP() \
	protected: \
	static const CZigBeeCluster *const clusterMap[]; \
	public: \
	virtual const CZigBeeCluster *const * \
		WalkClusterMaps(const CZigBeeCluster *const *ppMap) const; \
	virtual CZigBeeCluster *FindCluster(const unsigned short wClusterID, \
		const unsigned char nFlags = 0) const;

#define __COMPACT7B_IMPLEMENT_CLUSTER_MAP(cls, parent) \
	const CZigBeeCluster *const * \
	cls::WalkClusterMaps(const CZigBeeCluster *const *ppMap) const \
	{ \
		return !ppMap ? clusterMap : \
			parent::WalkClusterMaps(ppMap == clusterMap ? 0 : ppMap); \
	} \
	CZigBeeCluster *cls::FindCluster(const unsigned short wClusterID, \
	const unsigned char nFlags) const \
	{ \
		CZigBeeCluster *pCluster = \
			CZigBeeFoundationApplication::FindCluster(wClusterID, nFlags, \
				clusterMap); \
		return pCluster ? pCluster : parent::FindCluster(wClusterID, nFlags); \
	}

#define __COMPACT7B_TEMPLATE_CLUSTER_MAP(cls, parent) \
	template <typename _type> inline \
	const CZigBeeCluster *const * \
	cls<_type>::WalkClusterMaps(const CZigBeeCluster *const *ppMap) const \
	{ \
		return !ppMap ? clusterMap : \
			parent::WalkClusterMaps(ppMap == clusterMap ? 0 : ppMap); \
	} \
	template <typename _type> inline \
	CZigBeeCluster *cls<_type>::FindCluster(const unsigned short wClusterID, \
	const unsigned char nFlags) const \
	{ \
		CZigBeeCluster *pCluster = \
			CZigBeeFoundationApplication::FindCluster(wClusterID, nFlags, \
				clusterMap); \
		return pCluster ? pCluster : parent::FindCluster(wClusterID, nFlags); \
	}
	
#define __COMPACT7B_DECLARE_CLUSTER_MAP_EX() \
	__COMPACT7B_DECLARE_CLUSTER_MAP() \
	public: \
		void AddCluster(CZigBeeCluster *const pCluster); \
		void RemoveCluster(CZigBeeCluster *const pCluster); \
	protected: \
		std::vector<CZigBeeCluster *> m_clusterMap;

#define __COMPACT7B_IMPLEMENT_CLUSTER_MAP_EX(cls, parent) \
	const CZigBeeCluster *const * \
	cls::WalkClusterMaps(const CZigBeeCluster *const *ppMap) const \
	{ \
		return !ppMap && !m_clusterMap.empty() ? &m_clusterMap.front() : \
			(ppMap == &m_clusterMap.front()) ? \
			clusterMap : \
			parent::WalkClusterMaps(ppMap == clusterMap ? 0 : ppMap); \
	} \
	CZigBeeCluster *cls::FindCluster(const unsigned short wClusterID, \
	const unsigned char nFlags) const \
	{ \
		CZigBeeCluster *pCluster = m_clusterMap.empty() ? 0 : \
			CZigBeeFoundationApplication::FindCluster(wClusterID, nFlags, \
			&m_clusterMap.front()); \
		\
		if (!pCluster) \
			pCluster = CZigBeeFoundationApplication::FindCluster(wClusterID, \
				nFlags, clusterMap); \
		return pCluster ? pCluster : parent::FindCluster(wClusterID, nFlags); \
	} \
	void cls::AddCluster(CZigBeeCluster *const pCluster) \
	{ \
		CZigBeeCluster *const pOffset = \
			reinterpret_cast<CZigBeeCluster *> \
			(reinterpret_cast<uintptr_t>(pCluster) - \
			reinterpret_cast<uintptr_t>(this)); \
		\
		if (m_clusterMap.empty()) \
			m_clusterMap.push_back(pOffset); \
		else \
			m_clusterMap.back() = pOffset; \
		\
		m_clusterMap.push_back(0); \
	} \
	void cls::RemoveCluster(CZigBeeCluster *const pCluster) \
	{ \
		CZigBeeCluster *const pOffset = \
			reinterpret_cast<CZigBeeCluster *> \
			(reinterpret_cast<uintptr_t>(pCluster) - \
			reinterpret_cast<uintptr_t>(this)); \
		\
		m_clusterMap.resize(std::distance(m_clusterMap.begin(), \
			std::remove(m_clusterMap.begin(), m_clusterMap.end(), pOffset))); \
		ASSERT((m_clusterMap.size() > 1) || \
			((m_clusterMap.size() == 1) && (m_clusterMap.front() == 0))); \
		if (m_clusterMap.size() == 1) \
			m_clusterMap.clear(); \
	}

#define __COMPACT7B_BEGIN_CLUSTER_MAP(cls) \
	const CZigBeeCluster *const cls::clusterMap[] = {

#define __COMPACT7B_END_CLUSTER_MAP \
		0 \
	};

#define __COMPACT7B_CLUSTER(cls, member) \
	&static_cast<cls *>(0)->member,

#define __COMPACT7B_IMPLEMENT_INCLUSTER_LIST(cls, parent) \
	void cls::GetInputClusters(std::set<unsigned short> &clusters) const \
	{ \
		parent::GetInputClusters(clusters); \
		DeterminePublicClusters(clusters, CZigBeeCluster::flagInbound, \
			clusterMap); \
	}

#define __COMPACT7B_IMPLEMENT_OUTCLUSTER_LIST(cls, parent) \
	void cls::GetOutputClusters(std::set<unsigned short> &clusters) const \
	{ \
		parent::GetOutputClusters(clusters); \
		DeterminePublicClusters(clusters, CZigBeeCluster::flagOutbound, \
			clusterMap); \
	}

#define __COMPACT7B_IMPLEMENT_INCLUSTER_LIST_EX(cls, parent) \
	void cls::GetInputClusters(std::set<unsigned short> &clusters) const \
	{ \
		parent::GetInputClusters(clusters); \
		DeterminePublicClusters(clusters, CZigBeeCluster::flagInbound, \
			clusterMap); \
		if (!m_clusterMap.empty()) \
			DeterminePublicClusters(clusters, CZigBeeCluster::flagInbound, \
			&m_clusterMap.front()); \
	}

#define __COMPACT7B_IMPLEMENT_OUTCLUSTER_LIST_EX(cls, parent) \
	void cls::GetOutputClusters(std::set<unsigned short> &clusters) const \
	{ \
		parent::GetOutputClusters(clusters); \
		DeterminePublicClusters(clusters, CZigBeeCluster::flagOutbound, \
			clusterMap); \
		if (!m_clusterMap.empty()) \
			DeterminePublicClusters(clusters, CZigBeeCluster::flagOutbound, \
			&m_clusterMap.front()); \
	}

#define __COMPACT7B_DECLARE_NOTIFICATION_MAP() \
	static const CZigBeeClusterNotification notificationMap[]; \
	virtual const CZigBeeClusterNotification *FindSubscription \
		(const unsigned short wClusterID, const unsigned short wAttributeID, \
		const unsigned char nOptions, \
		const unsigned short wManufacturerCode) const;

#define __COMPACT7B_IMPLEMENT_NOTIFICATION_MAP(cls, parent) \
	const CZigBeeClusterNotification *\
	cls::FindSubscription(const unsigned short wClusterID, \
		const unsigned short wAttributeID, const unsigned char nOptions, \
		const unsigned short wManufacturerCode) const \
	{ \
		const CZigBeeClusterNotification *pSubscription = \
			CZigBeeFoundationApplication::FindSubscription(wClusterID, \
				wAttributeID, nOptions, wManufacturerCode, notificationMap); \
		return pSubscription ? pSubscription : \
			parent::FindSubscription(wClusterID, wAttributeID, nOptions, \
			wManufacturerCode); \
	}

#define __COMPACT7B_BEGIN_NOTIFICATION_MAP(cls) \
	const CZigBeeClusterNotification cls::notificationMap[] = {

#define __COMPACT7B_END_NOTIFICATION_MAP \
		{ 0, 0, 0, 0, 0, 0 } \
	};

#define __COMPACT7B_CLUSTER_NOTIFICATION(cluster, attribute, options, \
	update, refresh) \
	{ \
		cluster, attribute, 0, options, \
		static_cast<CZigBeeClusterNotification::MEMBERCALLBACK>(update), \
		static_cast<CZigBeeClusterNotification::MEMBERCALLBACK>(refresh) },

#define __COMPACT7B_CLUSTER_NOTIFICATION_M(cluster, attribute, options, \
	manufacturerCode, update, refresh) \
	{ \
		cluster, attribute, manufacturerCode, \
		options | CZigBeeAttribute::manufacturerSpecific, \
		static_cast<CZigBeeClusterNotification::MEMBERCALLBACK>(update), \
		static_cast<CZigBeeClusterNotification::MEMBERCALLBACK>(refresh) },


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAllFoundationApplications

class CZigBeeAllFoundationApplications
{
	// Construction
	public:
		CZigBeeAllFoundationApplications
			(const CZigBeeApplicationSupportBase &aps);

	// Attributes
	public:
		// Returns true if the current result is valid (non-null). In this
		// case the foundation application is accessible and more results
		// might be available after calling GetNext()
		operator bool() const;

		// Returns the current foundation application
		CZigBeeFoundationApplication *operator->() const;

		// Returns the current foundation application
		CZigBeeFoundationApplication &operator*() const;

	// Operations
	public:
		// Advances to the next foundation application
		CZigBeeAllFoundationApplications &Advance();

	// Implementation
	protected:
		// Reference to the application support base to use for iterating
		const CZigBeeApplicationSupportBase &m_aps;

		// Set of active endpoints that we need to iterate over
		std::set<unsigned char> m_endpoints;

		// Application endpoint iterator
		std::set<unsigned char>::iterator m_iterator;

		// Pointer to current application
		CZigBeeFoundationApplication *m_pApplication;
};
