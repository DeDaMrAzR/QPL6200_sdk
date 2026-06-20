// ZigBeeApplicationSupportBase.h : header file
//
// Copyright(C) 2008-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This implementation is based on several revisions of the ZigBee Alliance's
// ZigBee Specification related to the PRO feature set, currently: R17, R19,
// R20, R21, R22
//
// This module implements an abstraction of the application support sublayer
// (APS) and certain related entities, e.g. APSME, AIB, NLME, and NIB. It
// exposes only a subset of these, enough for applications to operate on this
// subset. This abstraction layer was introduced to facilitate solutions,
// where an application runs either on a full local zigbee stack or on a
// remote zigbee stack

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeApplicationSupportBase;
class CZigBeeApplication;
class CZigBeeStartupChannelInformation;
class CZigBeeNetworkSecurityMaterial;
class CZigBeeApplicationSecurityMaterial;
class CZigBeeApplicationSecurityMaterialUpdate;
class CZigBeeApplicationSecurityMaterialPersistentDataFull;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationInformationBase

class CZigBeeApplicationInformationBase
{
	// typedefs & enums
	public:
		// The maximum number of retries allowed after a transmission failure
		enum { maxFrameRetries = 3 };

		// The minimum required size of the APS duplicate rejection table
		enum { minDuplicateRejectionTableSize = 1 };

		// Type of security material that undergoes an exchange procedure
		enum { uniqueTrustCenterLinkKey = 0x01, authenticationToken = 0x02 };

	// Construction
	public:
		CZigBeeApplicationInformationBase();

	// Attributes
	public:
		// The EUI-64 of the trust center
		unsigned long long m_qwTrustCenterAddress;

		// The 64-bit address of a network to form or to join. This is AIB
		// attribute 0xc4
		unsigned long long m_qwUseExtendedPANID;

		// The mask of allowable channels for this device to use for network
		// operations. This is AIB attribute 0xc3
		unsigned int m_nChannelMask;

		// If non-zero, forced formation is being performed. Forced formation
		// results in the network formation to succeed, even if none of the
		// channels satisfies the energy threshold
		std::set<CZigBeeStartupChannelInformation> *m_pChannelsEx;

 		// The group table maps group IDs to local endpoints. A single group
		// could span multiple local endpoints. This table is shared between
		// the network and application support layers
		std::multimap<unsigned short, unsigned char> m_groups;

		// Cached application security material
		std::map<unsigned long long, CZigBeeApplicationSecurityMaterial>
			m_keys;

		// Pending key updates. Used on the trust center to keep track of
		// trust center link keys which have not been validated yet
		std::map<unsigned long long, CZigBeeApplicationSecurityMaterialUpdate *>
			m_pendingKeys;

		// The value to be used for the NonmemberRadius parameter when using
		// NWK layer multicast. This is AIB attribute 0xc6
		unsigned char m_bNonmemberRadius;

		// True if the device should become the ZigBee Coordinator on startup,
		// false otherwise. This is AIB attribute 0xc2
		bool m_bDesignatedCoordinator;

		// A flag controlling the use of insecure join at startup. This is AIB
		// attribute 0xc8
		bool m_bUseInsecureJoin;

		// Trust center policy: Allow handing out the network key to devices
		// that join through another router, even if the coordinator's permit
		// joining flag is disabled. This is mainly for testing to allow the
		// network to be opened at a single router only (to force a certain
		// topology/scenario). Notice that this, together with the MAC permit
		// joining status determines the "allowJoins" TC policy (0xad)
		bool m_bAllowAuthentication;

		// Trust center policy: Allow handing out the network keys to devices
		// that rejoin using well known or default (global) keys. Defaults to
		// false for ZigBee 3.0, but could be set to true for compatibility
		// with R20 devices. This is AIB attribute 0xb6
		bool m_bAllowRejoins;

		// Policy: Determines whether a device accepts a mgmt_leave_req where
		// the rejoin option is clear (permanent leave)
		bool m_bAcceptLeaveRequest;

		// Policy: Determines whether a device accepts a new network key, which
		// has not been delivered encrypted with the trust center link key
		bool m_bEnforceSecureNetworkKeyRotation;

		// A policy setting that enables forced network formation. If set to
		// true, active scans will be performed on all channels (not excluding
		// those exceeding the energy threshold) and if neither the primary nor
		// the secondary channel set offered a suitable channel, the best one
		// will be selected to form the network, instead of passing control to
		// the application with a STARTUP_FAILURE
		static const bool bForceFormation;

		// A policy setting that enables (R21 requirement) or disables rolling
		// the network key automatically when a frame counter in excess of half
		// the frame counter space is seen in the neighborhood of the TC
		static const bool bAutoRollNetworkKey;

		// A flag that indicates whether the device shall request a new trust
		// center link key after joining
		static const bool bRequestTrustCenterLinkKey;

		// A bitmap that indicates whether the device shall leave the network
		// after a failed security material exchange procedure. The types of
		// security materials that this policy pertains to are represented by
		// individual bit flags in the bitmap. If set to '1' and an exchange
		// of the related security material fails without a fall-back option
		// (e.g. initial exchange, on fall-back available), the device shall
		// leave the network (which typically results in another attempt to
		// join the same or another network from scratch)
		static const uint8_t leaveAfterFailedSecurityMaterialExchange;

		// Policy: Determines whether the trust center will act on ZDO permit
		// joining requests with the TC significance field set as '1'. When
		// set to false, the local device being a trust center will ignore such
		// requests as illegal; when set to true, it will execute the change in
		// association permit and allow new devices to join. Must be set to '1'
		// for EZ-mode network steering to work as expected on centralized
		// security networks. For higher security deployments a value of '0'
		// allows the trust center to remain the sole authority on the network
		// to permit new devices onto the network
		static const bool bAllowRemoteTrustCenterPolicyChange;

		// The maximum number of milliseconds to wait for an acknowledgement to
		// a transmitted frame
		static const unsigned int nAcknowledgmentWaitDuration;

		// The APS retry back-off duration, in case the NLDE returned a failure
		// status for the original request, in milliseconds
		static const unsigned int nRetryBackoffDuration;

		// The maximum number of milliseconds to wait for an expected security
		// command frame. This is TxDuration [50ms for 2.4 GHz devices] *
		// (2 * maximum network depth [15 for ZigBee PRO]) + worst-case AES
		// encrypt/decrypt times [200ms]. This is defined to be 1700ms for
		// 2.4 GHz ZigBee PRO. However, this time is too aggressive for multi-hop
		// joining under significant network load. This is AIB attribute 0xac
		static const unsigned int nSecurityTimeoutPeriod;

		// Default duration to remain responsive (increased poll-rate) after
		// joining a network for the first time. This should provide enough
		// time for interested parties to discover and configure the device,
		// and potentially instruct it to remain responsive for extended
		// periods of time via application-specific means
		static const unsigned int nDefaultResponsiveModeDuration;

		// The maximum size of the APS duplicate rejection table
		static const unsigned int nDuplicateRejectionTableLimit;

		// The maximum life-time of an entry in the duplicate rejection table,
		// normally (maxFrameRetries + 1) * acknowledgment wait duration
		static const unsigned int nDuplicateObservationTime;

		// The time during which an entry in the duplicate rejection table will
		// resist purging. Entries, which were in the duplicate rejection table
		// for at at least this period of time will be purged first when a new
		// message from the same device arrives
		static const unsigned int nDuplicatePurgeResistanceTime;

		// The duplicate sequence counter window for roll-over resilience
		static const uint8_t nDuplicateSequenceWindow;

		// Default broadcast address used for groupcast transmissions
		static const uint16_t defaultGroupcastAddress;

		// Alternate broadcast address used for groupcast transmissions
		static const uint16_t alternateGroupcastAddress;

		// Use APS security if the link key update mechanism is performed to
		// refresh the link key when the device has successfully joined the
		// network and has a unique link key
		static const bool useAPSSecurityForLinkKeyUpdate;

		// The maximum number of group addresses in the group table
		static const uint8_t groupTableLimit;

	// Operations
	public:
		// Stores group table in persistent storage
		void StoreGroups() const;

		// Loads group table from persistent storage
		void LoadGroups();

		// Returns true if a unique link-key for the specified destination
		// address exists, false otherwise (global/shared key)
		bool HaveUniqueKey(const unsigned long long qwDevice) const;

		// Returns true if a link-key for the specified destination address
		// exists, which has the specified flags, i.e. any of those in the
		// parameter 'set' must be '1' and all of those in parameter 'clear'
		// must be '0'; returns false otherwise
		bool HaveKeyWithFlags(const unsigned long long device,
			const uint_fast16_t set,
			const uint_fast16_t clear = 0) const;

		// Returns true if a unique trust center link-key is on record, which
		// is not a pre-configured key, i.e. has been assigned or negotiated
		bool HaveUniqueTrustCenterLinkKey() const;

		// Deletes a pending key update
		void DeletePendingKeyUpdate(const unsigned long long qwDevice);

		// Erases application layer security material from the cache
		void EraseSecurityMaterialCache(const unsigned long long qwDevice,
			const bool bKeepExplicitKeys = true);

		// Erases application layer security material from non-volatile memory
		// but does not alter the cache
		bool EraseSecurityMaterial(const unsigned long long qwDevice,
			const bool bKeepExplicitKeys = true) const;

		// Erases application layer security material from non-volatile memory
		// and from the cache
		bool EraseSecurityMaterialEx(const unsigned long long qwDevice,
			const bool bKeepExplicitKeys = true);

		// Erases all application layer security material from non-volatile
		// memory. This can be restricted to key of a specific type and with
		// all of the specified flags
		void EraseAllSecurityMaterial(const unsigned char nKeyType = -1,
			const uint_fast16_t flags = 0);

		// Stores application layer security material in non-volatile memory
		void StoreSecurityMaterial
			(CZigBeeApplicationSecurityMaterial &material) const;

		// Loads application layer security material from non-volatile memory
		void LoadSecurityMaterial();

		// Restores application layer security material from persistent data
		void LoadSecurityMaterial(CZigBeeApplicationSecurityMaterial &material,
			const CZigBeeApplicationSecurityMaterialPersistentDataFull &data)
			const;

		// Returns a pointer to application security material for the
		// specified device or null if no such material exists
		CZigBeeApplicationSecurityMaterial *
		GetSecurityMaterial(const uint_fast64_t device,
			const bool onlyPermanent = false);

	// Implementation
	public:
		// Primary channel set. When forming or joining a network, these
		// channels take precedence over the remaining set of channels
		static const unsigned int dwPrimaryChannelSet;

		// Erases security material from either the pending or permanent set
		std::map<unsigned long long,
		CZigBeeApplicationSecurityMaterial>::iterator
		EraseSecurityMaterial(const std::map<unsigned long long,
			CZigBeeApplicationSecurityMaterial>::iterator key);

		// Notifies the application of changes to the application security
		// material
		void OnSecurityMaterialModified
			(const CZigBeeApplicationSecurityMaterial &material);

#ifdef _DEBUG
		// Dumps the AIB
		void Dump() const;

		// Dumps security materials (link keys)
		void DumpSecurityMaterials() const;

		// Dumps the group table
		void DumpGroupTable() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSupportManagementBase

class CZigBeeApplicationSupportManagementBase
{
	// typedefs & enums
	public:
		// Invoked as a result of a device returning to its operational channel
		// after having conducted a temporary channel change
		typedef void (*ONTEMPORARYCHANNELCHANGECOMPLETE)
			(const unsigned char nStatus, void *pArgument);

		// Startup modes. The stack provides fine-grained control over how the
		// ZigBee stack is started and which actions it is allowed to perform.
		// 1) resume operation; 2) secure rejoin; 3) insecure (TC) rejoin;
		// 4) association; 5) formation. Notice this could be extended by
		// adding another option: channel scan with silent rejoin (i.e. no
		// rejoin request/response, just a channel scan and PANID verification)
		enum { startupAllowResume = 0x01, startupAllowSecureRejoin = 0x02,
			startupAllowTrustCenterRejoin = 0x04,
			startupAllowAssociation = 0x08, startupAllowNetworkFormation = 0x10,
			startupDefault = 0,
			startupAllowJoining = startupAllowResume | startupAllowSecureRejoin |
				startupAllowTrustCenterRejoin | startupAllowAssociation,
			startupAllowRejoining = startupAllowSecureRejoin |
				startupAllowTrustCenterRejoin,
			startupModes = 0x1f };

		// Startup states. Note: The upper half-word contains a copy of the
		// startup modes above where each attempted step/procedure has its flag
		// set to 1. The state is 'one-hot', i.e. after each attempt, the flag
		// is cleared and the next allowed attempt is made
		// startupSecondaryChannelSet must remain in the lower word, which gets
		// passed to Startup() as argument
		enum { startupInProgress = 0x0080, startupSecondaryChannelSet = 0x0040,
			startupSucceeded = 0x8000, startupSucceededOnce = 0x00010000,
			startupInvokedOnce = 0x00020000,
			startupSucceededViaAssociation = 0x00040000,
			startupViaOOBCommissioning = 0x00080000,
			startupPersistentSettings = startupAllowResume << 8,
			startupSecureRejoin = startupAllowSecureRejoin << 8,
			startupTrustCenterRejoin = startupAllowTrustCenterRejoin << 8,
			startupJoinViaAssociation = startupAllowAssociation << 8,
			startupNetworkFormation = startupAllowNetworkFormation << 8,
			startupStates = startupModes << 8,
			startupRejoin = startupSecureRejoin | startupTrustCenterRejoin };

		// Startup procedure progress states. "startupInstigated" means that
		// the stack is being started. This might comprise multiple attempts;
		// "startupComplete" means that stack start-up has completed
		// successfully or has finally failed after having performed the
		// configured number of attempts; "startupAttemptInstigated" means
		// that a new start-up attempt is being made. For example, for join
		// from scratch, this comprises active scans on primary and secondary
		// channels and subsequent association and commissioning steps;
		// "startupBackoff" means that an attempt to start has failed and the
		// device is waiting for the back-off period to elapse, before another
		// attempt is made; "startupAbort" means that stack start-up has been
		// aborted under application control, for example, due to out-of-band
		// commissioning via touch-link, Zigbee Direct or another interface.
		enum { startupInstigated, startupComplete, startupAttemptInstigated,
			startupBackoff, startupAbort };

		// Factory reset options, consisting of standard options like reboot,
		// clear security frame counters, discard preserved attributes,
		// revert to factory-fresh settings using a safe approach that guards
		// against power failures during the process via a backup storage area
		// and application-specific options, which shall not use the lower
		// eight bits. Partial resets keeping all principal commissioning
		// settings (core network parameters, groups and bindings) are also
		// possible.
		enum { factoryReboot = 0x01,
			factoryResetSecurityFrameCounters = 0x02,
			factoryDiscardPreservedAttributes = 0x04,
			factoryResetFull = factoryResetSecurityFrameCounters |
				factoryDiscardPreservedAttributes,
			factoryRevertSafe = 0x08,
			factoryDefault = factoryReboot | factoryRevertSafe,
			factoryResetStage2 = 0x80, factoryResetUserOptionMask = 0xff00,
			factoryResetUserOptionShift = 8 };

		// The is the base-device commissioning time. Will be used for permit
		// joining extension after a router joined the network and for finding
		// and binding
		enum { commissioningTime = 180 };

	// Construction
	public:
		CZigBeeApplicationSupportManagementBase
			(CZigBeeApplicationSupportBase &aps);

	// Attributes
	public:
		// Link to APS instance
		CZigBeeApplicationSupportBase &m_aps;

		// APS information base
		CZigBeeApplicationInformationBase m_aib;

	// Operations
	public:
		// Returns the Trust Center's address
		CZigBeeAddress GetTrustCenterAddress() const;

		// Returns true, if this device is currently operating as the network's
		// trust center
		bool IsTrustCenter() const;

		// Returns true, if this device is currently operating in a distributed
		// security network
		bool IsDistributedSecurityNetwork() const;

		// Returns true if the device has an active network key and thus can
		// operate on the network
		bool HaveNetworkKey() const;

	// Overridables
	public:
		// Clear bindings for a specific endpoint or all endpoints
		virtual void ClearBindings(const unsigned char bSourceEndpoint = 255) = 0;

		// Direct call version of an APSME-ADD-GROUP.request(GroupAddress,
		// Endpoint), clause 2.2.4.5.1, resulting in an immediate
		// APSME-ADD-GROUP.confirm(Status, GroupAddress, Endpoint),
		// clause 2.2.4.5.2. A ubisys extension to this request is the MSB of
		// the status code being returned: If set to 1 the entry already
		// existed before, otherwise it has just been created
		virtual unsigned int AddGroup(const unsigned short wGroupAddress,
			const unsigned char bEndpoint) = 0;

		// Direct call version of an APSME-REMOVE-GROUP.request(GroupAddress,
		// Endpoint), clause 2.2.4.5.3, resulting in an immediate
		// APSME-REMOVE-GROUP.confirm(Status, GroupAddress, Endpoint),
		// clause 2.2.4.5.4
		virtual unsigned char RemoveGroup(const unsigned short wGroupAddress,
			const unsigned char bEndpoint) = 0;

		// Direct call version of an APSME-REMOVE-ALL-GROUPS.request(Endpoint),
		// clause 2.2.4.5.5, resulting in an immediate
		// APSME-REMOVE-ALL-GROUPS.confirm(Endpoint), clause 2.2.4.5.6
		virtual unsigned char RemoveAllGroups(const unsigned char bEndpoint) = 0;

		// Returns true, if the specified endpoint (or any of the active
		// endpoints in case of the broadcast endpoint address) belongs to the
		// specified group, in which case the node is deemed a member of the
		// group. Can be used to do lookups in a virtual nwkGroupIDTable or
		// to implement the view groups command of the ZCL groups cluster
		virtual bool IsGroupMember(const unsigned short wGroupAddress,
			const unsigned char bEndpoint = 255) const = 0;

		// Conducts a temporary channel change and invokes the callback after
		// having changed back to the operational channel. This is done using
		// an extension to the MLME's scanning capabilities. Notice that the
		// channel is specified as an ordinal (not as a mask) and that the
		// duration is in seconds (not as exponent of super-frame durations)
		virtual bool RequestTemporaryChannelChange(const unsigned char nChannel,
			const unsigned char nDuration,
			void *const pfnFilter,
			const ONTEMPORARYCHANNELCHANGECOMPLETE pfnComplete,
			void *pArgument = 0) = 0;

		// Returns true, if this device has joined the network using a
		// pre-configured link-key
		virtual bool JoinedUsingPreconfiguredKey() = 0;

	// Implementation
	public:
		virtual ~CZigBeeApplicationSupportManagementBase();

#ifdef _DEBUG
		// Returns the human-readable name of a start-up progress enumeration
		// value
		static const char *GetStartupProgressString(const uint_fast8_t state);
#endif
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSupportBase

class CZigBeeApplicationSupportBase
{
	// typedefs & enums
	public:
		// TxOptions for APSDE-DATA.request primitive
		enum { transmitEnableSecurity = 0x01,
			useAlternateGroupcastAddress = 0x02,
			transmitAcknowledged = 0x04, transmitAllowFragmentation = 0x08,
			transmitExtendedNonce = 0x10, transmitForceIndirect = 0x40,
			useIncomingSecurityMaterial = 0x80 };

		// Enhanced transmit options. Currently includes a priority, can be
		// extended to include desired transmit power etc. Notice that frames
		// with immediate priority are immediately forwarded to the NWK layer
		// and are not blocked by pending APS transmissions. The security keys
		// can be masked into an auxiliary header frame control word. A header
		// might be provided by the caller (for example to implement the APS
		// tunnel command), and reflection allows the local device to receive
		// its own frame, when it is broadcast or multicast (it can always
		// receive unicast frames destined to itself)
		enum { priorityImmediate = 0, priorityHigh = 1, priorityMedium = 2,
			priorityLow = 3, priorityMask = 0x03, disableNetworkSecurity = 0x04,
			securityKeyTransportKey = 0x10,
			securityKeyLoadKey = 0x18, securityKeyMask = 0x18,
			securityKeyShift = 3, securityDefaultKey = 0x20,
			enableReflection = 0x40, headerIncluded = 0x80 };

		// Security status for APSDE-DATA.indications
		enum { securityNone, securityNetwork = 0x01,
			securityApplication = 0x02 };

		// The minimum number of octets added by the APS sub-layer to an ASDU
		// (apscMinHeaderOverhead)
		enum { minHeaderOverhead = 12 };

		// The maximum number of octets added by the stub APS/NWK to an ASDU
		enum { stubHeaderCapacity = 32 };

		// The maximum number of octets contained in a non-complex descriptor
		// (apscMaxDescriptorSize)
		enum { maxDescriptorSize = 64 };

		// APS sub-layer status values (table 2.26)
		enum { statusSuccess, statusASDUTooLong = 0xa0,
			statusDefragmentationDeferred, statusDefragmentationUnsupported,
			statusIllegalRequest, statusInvalidBinding, statusInvalidGroup,
			statusInvalidParameter, statusNoAcknowledgment, statusNoBoundDevice,
			statusNoShortAddress, statusNotSupported, statusCancelled,
			statusReserved, statusSecurityFailure, statusTableFull,
			statusUnsecured, statusUnsupportedAttribute,
			statusPeerCannotDefragment, statusUnknownFragmentationSupport };

	// Construction
	public:
		CZigBeeApplicationSupportBase
			(CZigBeeApplicationSupportManagementBase &apsme);

	// Attributes
	public:
		CZigBeeApplicationSupportManagementBase &m_apsme;

	// Operations
	public:
		// Creates a CPacket object suitable for carrying an APDU payload of
		// the specified size with enough headroom for the entire stack
		// overhead (APS, NWK, MAC and PHY). A value of -1 allocates the
		// maximum permissable APDU size (with or without fragmentation).
		// Additional space in the header part can optionally be reserved.
		static CPacket CreateAPDU(CPacketData::size_type cbPayload =
			static_cast<CPacketData::size_type>(-1),
			const CPacketData::size_type cbHeaderCapacity = 0);

		// Returns true, if the specified endpoint is a usual application
		// endpoint in the range 0x01 - 0xf0, false if it is the ZDO endpoint
		// null, a reserved endpoint or the endpoint broadcast address
		static bool IsApplicationEndpoint(const unsigned char bEndpoint);

		// Returns true, if the specified endpoint is a usual application
		// endpoint in the range 0x01 - 0xf0, or a reserved endpoint in the
		// range 0xf1 - 0xfe; false otherwise
		static bool IsApplicationEndpointEx(const unsigned char bEndpoint);

		// Returns transmit options that mirror the incoming security
		unsigned char MirrorIncomingSecurity() const;

	// Overridables (NLME interface exposed to application layer)
	public:
		// Returns an address object with network address and 64-bit extended
		// IEEE address
		virtual CZigBeeAddress GetAddress() const = 0;

		// Obtains the current active network security material. Returns false,
		// if no such material can be found
		virtual bool GetActiveNetworkSecurityMaterial
			(CZigBeeNetworkSecurityMaterial *&pMaterial) const = 0;

		// Invalidates a 16-bit network address to 64-bit IEEE address mapping
		// for every occurrence of this mapping
		virtual void InvalidateAddressMapping
			(const CIEEE802154Address &address) = 0;

		// Given an address object with either a short or extended address,
		// this method completes the address object with the missing type of
		// address, if it is currently known. It does not initiate any address
		// lookups, however, which is left to higher layers, if required.
		// Returns true, if the address object is complete
		virtual bool AugmentAddress(CIEEE802154Address &address) = 0;

	// Overridables (APS interface exposed to application layer)
	public:
		// Adds an application to the dynamic application map. Can be used to
		// implement the ConfigureEndpoint function in a gateway device, for
		// instance. If an application exists on the specified endpoint it is
		// replaced by the new application. It is first removed and - in case
		// of a heap application - also deleted. If another application exists
		// in the static application map, the function fails
		virtual bool AddApplication(CZigBeeApplication &application) = 0;

		// Removes an application from the dynamic application map. Notice,
		// that the application object is also automatically release via delete
		// in case of a heap application. Removing an application while the
		// network stack is operational is discouraged
		virtual bool RemoveApplication(CZigBeeApplication &application,
			const bool bDelete = true) = 0;

		// Returns a set of active application endpoints (except EP #0, ZDO)
		virtual void GetActiveEndpoints
			(std::set<unsigned char> &endpoints) const = 0;

		// Returns the application object that services the specified endpoint,
		// or null if no such application exists. Notice that you should have
		// a critical section lock around the block that uses GetApplication()
		// and the returned application pointer
		virtual CZigBeeApplication *GetApplication
			(const unsigned char bEndpoint) const = 0;

		// APSDE-DATA.request(DstAddrMode, DstAddress, DstEndpoint, ProfileID,
		// ClusterID, SrcEndpoint, ASDULength, ADSU, TxOptions, Radius). An
		// additional cookie allows the calling application to pass arguments
		// to the confirmation handler in order to simplify distinction of
		// different requests
		virtual void OnRequestData(const CZigBeeAddress &destination,
			const unsigned short wProfileID, const unsigned short wClusterID,
			const unsigned char bSourceEndpoint, CPacket &asdu,
			const unsigned char bTransmitOptions, const unsigned char bRadius,
			void *const pCookie = 0,
			const unsigned char bEnhancedOptions = priorityMedium,
			const CZigBeeAddress &source = CZigBeeAddress(),
			const unsigned char nSequence = 0) = 0;

		// This is a version of APSDE-DATA.request, which allows deferred data
		// requests, i.e. the APSDE-DATA.request is fired with the supplied
		// parameters after a pre-defined delay
		virtual void OnRequestDataDeferred(const CZigBeeAddress &destination,
			const unsigned short wProfileID, const unsigned short wClusterID,
			const unsigned char bSourceEndpoint, CPacket &asdu,
			const unsigned char bTransmitOptions, const unsigned char bRadius,
			void *const pCookie, const unsigned char nEnhancedOptions,
			const CZigBeeAddress &source, const unsigned char nSequence,
			const unsigned int nDelay) = 0;

		// This method may be called only from within OnIndicateData() handlers
		// for APS frames and returns the APS security material used to decrypt
		// the incoming APS frame. If no APS security was applied, the pointer
		// is null
		virtual const CZigBeeApplicationSecurityMaterial *
			GetIncomingSecurityMaterial() const = 0;

		// Enter responsive mode until LeaveResponsiveMode() is invoked with
		// the cookie provided by EnterResponsiveMode()
		virtual bool EnterResponsiveMode(unsigned int &nCookie) = 0;

		// Leave responsive mode for the context provided by the cookie
		virtual void LeaveResponsiveMode(const unsigned int nCookie) = 0;

		// Enter responsive mode, and automatically leave after the specified
		// time, or pre-emptively by calling LeaveResponsiveModeEx() for the
		// cancellable timer provided by EnterResponsiveModeEx()
		virtual bool EnterResponsiveModeEx(const unsigned int nDuration =
			CZigBeeApplicationInformationBase::nDefaultResponsiveModeDuration,
			CCompactTimer **const ppTimer = 0) = 0;

		// Leave responsive mode pre-emptively
		virtual bool LeaveResponsiveModeEx(CCompactTimer *&pTimer) = 0;

		// True if the device is currently in responsive (fast polling) mode
		virtual bool IsResponsive() = 0;

	// Overridables (ZDO interface exposed to application layer)
	public:
		// Performs all steps required to add a binding or remove a binding.
		// Can be called by CZigBeeApplication::ProcessBindOrUnbind()
		// implementations to perform the actual work after having performed
		// any additional checks, for example. The return value is a ZDO status
		// code and can be placed unmodified in a ZDO response frame
		virtual unsigned char DefaultBindOrUnbindProcessing
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const bool bTrustCenterOnly = false) = 0;

	// Implementation (AIB debug interface exposed to application layer)
	public:
		virtual ~CZigBeeApplicationSupportBase();

#ifdef _DEBUG
		// Dumps relevant APS data (in particular application map)
		virtual void Dump() const = 0;

		// Returns the human-readable name of a status enumeration value.
		// Does not only return APS status codes, but also NWK, MAC and PHY
		// codes, as appropriate
		virtual const char *GetStatusString(const unsigned char nValue) = 0;
#endif

};

inline bool CZigBeeApplicationSupportBase::IsApplicationEndpoint
	(const unsigned char bEndpoint)
{
	return (bEndpoint >= 0x01) && (bEndpoint <= 0xf0);
}

inline bool CZigBeeApplicationSupportBase::IsApplicationEndpointEx
	(const unsigned char bEndpoint)
{
	return (bEndpoint >= 0x01) && (bEndpoint <= 0xfe);
}
