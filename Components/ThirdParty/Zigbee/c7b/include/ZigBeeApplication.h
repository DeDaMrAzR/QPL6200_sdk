// ZigBeeApplication.h : header file
//
// Copyright(C) 2008-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This implementation is based on the ZigBee Alliance's ZigBee Specification
// as of January 17, 2007
//
// Provides CZigBeeApplication, the base class for all application objects
// residing on endpoints on the stack. Notice that CZigBeeApplication is used
// by ZDO and MSP applications and is also the base for ZCL-compliant
// application objects via CZigBeeFoundationApplication, a derived class

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeAddress;
class CZigBeeNetwork;
class CZigBeeServiceMapEntry;
class CZigBeeApplicationSupportBase;

/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplication

class CZigBeeApplication
{
	// typedefs & enums
	public:
		// Prototype for member functions that handle cluster messages
		typedef void (CZigBeeApplication::*CLUSTERSERVICE)
			(const CZigBeeAddress &destination, const CZigBeeAddress &source,
			 const unsigned short wClusterID, CPacket &asdu,
			 const unsigned char bStatus, const bool bSecurityUsed,
			 const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Application flags (dynamically allocated) application object, etc.
		// The lower bits and the upper byte may be used by derived classes
		// The "concealed"
		// flag, when set, will prevent the application from being listed in the
		// simple descriptor, or resulting in an application match. The application
		// remains operational, i.e. it can send and receive messages.
		enum { foundationApplication = 0x80, mutableApplication = 0x40,
			heapApplication = 0x20, flagConcealed = 0x10000000};

		// Action priority flags upon network leave notification.
		// The "Action Survey" bits represents the highest action priority
		// that is conveyed during a preliminary survey iteration through
		// all applications and clusters (static or dynamic). The "Action
		// Trigger" bits represents the minimum priority that an application
		// or cluster has to possess so that leave-related action could be
		// conducted during the second primary iteration. Once conducted,
		// the "Action Triggered" bit should be set correspondingly. The
		// "Final Reset" bit indicates whether a final reset is still
		// necessary after both iterations have been finished.
		enum { leaveActionPriorityDefault = 0x00, leaveActionPriorityLow = 0x02,
			leaveActionPriorityMedium = 0x04, leaveActionPriorityHigh = 0x06,
			leaveActionPriorityMask = 0x07, leaveActionTriggerShift = 3,
			leaveActionSurveyMask = leaveActionPriorityMask,
			leaveActionTriggerMask =
				leaveActionPriorityMask << leaveActionTriggerShift,
			leaveActionFinalReset = 0x40, leaveActionTriggered = 0x80 };

	// Construction
	public:
		CZigBeeApplication(CZigBeeApplicationSupportBase &aps,
			const uint_fast16_t wProfileID, const uint_fast8_t bEndpoint,
			const uint_fast32_t nFlags = 0, const uint_fast16_t wDeviceID = 0,
			const uint_fast8_t bDeviceVersion = 0);

	// Attributes
	public:
		// Glue to APS instance (next lower layer of protocol stack)
		CZigBeeApplicationSupportBase &m_aps;

		// Application flags
		uint32_t m_nFlags;

		// Application profile identifier
		uint16_t m_wProfileID;

		// Application device identifier
		uint16_t m_wDeviceID;

		// Application device version
		uint8_t m_bDeviceVersion;

		// Endpoint this application resides in
		const uint8_t m_bEndpoint;

		// True, if this is a foundation application. In this case, it is safe
		// to cast to CZigBeeFoundationApplication, which provides framework
		// support for applications based on the cluster library
		bool IsFoundationApplication() const;

		// True, if this is a mutable application. Mutable applications allow
		// addition and removal of clusters at runtime
		bool IsMutableApplication() const;

	// Overridables
	public:
		// Dispatches the specified message
		virtual const CZigBeeServiceMapEntry *
			FindService(const unsigned short wClusterID) const;

	// Overridables
	public:
		// Returns set of inbound clusters
		virtual void GetInputClusters(std::set<unsigned short> &clusters) const;

		// Returns set of outbound clusters
		virtual void GetOutputClusters(std::set<unsigned short> &clusters) const;

		// Appends a simple descriptor to the specified packet object
		virtual bool GetSimpleDescriptor(CPacket &packet) const;

	// Overridables
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

		// This notification is invoked upon reception of a ZDO device
		// announcement, i.e. after an end-device or a router with end-device
		// capabilities has joined or rejoined the network
		virtual void OnNotifyDeviceAnnouncement(const CZigBeeAddress &address,
			const unsigned char bCapabilities);

		// This notification is invoked upon completion of the ZigBee start-up
		// routine
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// This notification is invoked upon progress state change of ongoing
		// ZigBee start-up process
		virtual void OnNotifyStartupProgress(const uint_fast8_t state,
			const uint_fast16_t flags, const uint_fast8_t status);

		// This notification is invoked when the device left the network
		virtual void OnNotifyNetworkLeave(const uint8_t nStatus,
			const bool bRejoin, uint8_t &actionFlags);

		// Called during idle periods of the device. Returns true if the device
		// is active and should not go to sleep
		virtual bool OnIdle();

		// Called in an early stage when the device is powered-up. Ideal place
		// for initialization of peripherals that are going to be used by the
		// application during normal operation
		virtual void AfterPowerup();

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
		// particular application and sets a new operating mode, for example.
		// In addition, applications might want to support partial factory
		// resets, where only application data and state is reverted, but the
		// device remains on the network and does not reboot
		virtual void OnFactoryReset(const unsigned int nOptions);

		// Deletes all persistent storage items for an application. This is
		// useful for implementing partial factory resets or removing
		// application endpoints at run-time. If bRemoveAllReferences is true,
		// the application is also requested to delete all references, including
		// binding table entries and group memberships
		virtual void RestoreDefaults(const bool bRemoveAllReferences = true);

		// Handles a ZDO bind or unbind request for the application
		virtual unsigned char ProcessBindOrUnbindRequest
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const CZigBeeAddress &originator);

	// Operations
	public:
		// Attempts to complete addressing information based on a look-up in
		// addressing material available to the network layer. Does not issue
		// over-the-air address requests, though
		bool AugmentAddress(CZigBeeAddress &address);

		// Returns true, if the application endpoint is currently a member of
		// the specified group
		bool IsGroupMember(const unsigned short wGroupID) const;

	// Implementation
	public:
		virtual ~CZigBeeApplication();

	protected:
		// Performs a lookup using the supplied profile and cluster IDs.
		// Returns null if no appropriate service was found
		static const CZigBeeServiceMapEntry *
			FindService(const unsigned short wClusterID,
			const CZigBeeServiceMapEntry *const pMap);

	public:

#ifdef _DEBUG
		// Prints application state information to the debug output
		virtual void Dump() const;
#endif

};

inline bool CZigBeeApplication::IsFoundationApplication() const
	{ return (m_nFlags & foundationApplication) ? true : false; }

inline bool CZigBeeApplication::IsMutableApplication() const
	{ return (m_nFlags & mutableApplication) ? true : false; }


/////////////////////////////////////////////////////////////////////////////
// Explicit in-/out cluster list macros

#define __COMPACT7B_DECLARE_OUTCLUSTER_LIST() \
	virtual void GetOutputClusters(std::set<unsigned short> &clusters) const;

#define __COMPACT7B_BEGIN_OUTCLUSTER_LIST(cls, parent) \
	void cls::GetOutputClusters(std::set<unsigned short> &clusters) const \
	{ \
		parent::GetOutputClusters(clusters);

#define __COMPACT7B_DECLARE_INCLUSTER_LIST() \
	virtual void GetInputClusters(std::set<unsigned short> &clusters) const;

#define __COMPACT7B_BEGIN_INCLUSTER_LIST(cls, parent) \
	void cls::GetInputClusters(std::set<unsigned short> &clusters) const \
	{ \
		parent::GetInputClusters(clusters);

#define __COMPACT7B_END_CLUSTER_LIST() \
	}

#define __COMPACT7B_CLUSTER_LIST_ENTRY(clusterID) \
	clusters.insert(clusterID);


/////////////////////////////////////////////////////////////////////////////
// CZigBeeServiceMapEntry

class CZigBeeServiceMapEntry
{
	// Attributes
	public:
		// Cluster ID that will be handled by this entry
		unsigned short m_wClusterID;

		// Handler function that will be called when a frame for the cluster
		// with the specified ID has been received
		CZigBeeApplication::CLUSTERSERVICE m_pfnService;
};

#define __COMPACT7B_DECLARE_SERVICE_MAP() \
	public: \
	static const CZigBeeServiceMapEntry serviceMap[]; \
	virtual const CZigBeeServiceMapEntry *FindService \
		(const unsigned short wClusterID) const;

#define __COMPACT7B_IMPLEMENT_SERVICE_MAP(cls, parent) \
	const CZigBeeServiceMapEntry *cls::FindService(const unsigned short wClusterID) const \
		{ \
			const CZigBeeServiceMapEntry *pService = \
				CZigBeeApplication::FindService(wClusterID, serviceMap); \
			return pService ? \
				pService : parent::FindService(wClusterID); \
		}

#define __COMPACT7B_BEGIN_SERVICE_MAP(cls) \
	const CZigBeeServiceMapEntry cls::serviceMap[] = {

#define __COMPACT7B_END_SERVICE_MAP() \
		{ 0, 0 } \
	};

#define __COMPACT7B_SERVICE(cluster, handler) \
	{ cluster, static_cast<CZigBeeApplication::CLUSTERSERVICE>(handler) },
