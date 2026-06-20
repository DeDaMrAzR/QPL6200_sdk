// ZigBeeDeviceObject.h : header file
//
// Copyright(C) 2008-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This implementation is based on the ZigBee Alliance's ZigBee Specification
// as of January 17, 2007 and includes updates for the 2012 and 2015 editions
//
// This module implements the ZigBee Device Objects (ZDO) according to the
// ZigBee 2007, 2012 and 2015 specifications. Makes use of the Compact7B
// application object framework and can be added to the application map with
// the usual macros that apply to other applications, too

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeTLVBase;
class CZigBeeTLVDictionaryEntry;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectRequest

class CZigBeeDeviceObjectRequest
{
	// typedefs & enums
	public:
		typedef void (*CALLBACK)(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

	// Construction
	public:
		CZigBeeDeviceObjectRequest(const unsigned char nSequence,
			const CALLBACK m_pfnComplete, void *const pArgument1,
			void *const pArgument2, const unsigned int nTimeout,
			const bool bIsResponse, const bool bMultipleResponses,
			CZigBeeDeviceObjectRequest **const reference = 0);

	// Attributes
	public:
		// First argument to the service routine
		void *m_pArgument1;

		// Optional second argument to the service routine
		void *m_pArgument2;

		// Called when the request has been completed
		CALLBACK m_pfnComplete;

		// Timeout for requests that require a response
		CCompactTimer *m_pTimer;

		// For sleeping end-devices, this is a responsive mode poll timer
		CCompactTimer *m_pResponsiveModeTimer;

		// Timeout allowed for the request (in microseconds)
		unsigned int m_nTimeout;

		// Timestamp (of transmitted request frame)
		unsigned int m_nTimestampTX;

		// Destination address (of response frame)
		CZigBeeAddress m_destination;

		// Source address (of response frame)
		CZigBeeAddress m_source;

		// Response frame (might be empty)
		mutable CPacket m_response;

		// Cluster ID (of received response frame)
		unsigned short m_wClusterID;

		// Sequence number for this request, managed by ZDO instance below
		unsigned char m_nSequence;

		// State (progress that the request has made)
		unsigned char m_nState;

		// Status (of APSDE-DATA.request)
		unsigned char m_nStatusTX;

		// Status (of received response frame)
		unsigned char m_nStatusRX;

		// Security status (of received response frame)
		bool m_bSecurityUsed;

		// Link quality indicator (for received response frame)
		unsigned char m_nLinkQuality;

		// Timestamp (of received response frame)
		unsigned int m_nTimestampRX;

		// If the request transmission can be aborted, this refers to the
		// instance pointer that references this request, or null otherwise
		CZigBeeDeviceObjectRequest **const m_reference;

	// Implementation
	public:
		// States
		enum { stateCreated = 0x01, stateQueued = 0x02, stateConfirmed = 0x04,
			stateResponseReceived = 0x08, stateTimeout = 0x10,
			stateIsResponse = 0x20, stateMultipleResponses = 0x40,
			stateComplete = 0x80 };

#ifdef _DEBUG
		// Dumps pending ZDO requests
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObject

class CZigBeeDeviceObject : public CZigBeeApplication,
	public CCompactTimerClient
{
	__COMPACT7B_DECLARE_SERVICE_MAP()

	// typedefs & enums
	public:
		// Application profile identifier (ZDO profile ID is 0x0000)
		enum { profileID };

		// Endpoint #0 is assigned to ZDO application (must not be changed)
		enum { endpoint };

		// Identifies response type frames
		enum { responseMask = 0x8000 };

		// ZigBee Device Profile cluster IDs
		enum
		{
			requestNetworkAddress, requestExtendedAddress,
			requestNodeDescriptor, requestPowerDescriptor,
			requestSimpleDescriptor, requestActiveEndpoints,
			requestMatchDescriptors, requestComplexDescriptor = 0x0010,
			requestUserDescriptor, requestDiscoveryCache,
			announceDevice, setUserDescriptor, requestSystemServerDiscovery,
			requestStoreDiscovery, requestStoreNodeDescriptor,
			requestStorePowerDescriptor, requestStoreActiveEndpoints,
			requestStoreSimpleDescriptor, requestRemoveNodeCache,
			requestFindNodeCache, requestExtendedSimpleDescriptor,
			requestExtendedActiveEndpoints, announceParent, requestEndDeviceBind,
			requestBind, requestUnbind, requestBindRegister,
			requestReplaceDevice, requestStoreBackupBindEntry,
			requestRemoveBackupBindEntry, requestBackupBindTable,
			requestRecoverBindTable, requestBackupSourceBind,
			requestRecoverSourceBind, requestClearAllBindings,
			requestManagementNetworkDiscovery = 0x0030,
			requestManagementNeighborTable, requestManagementRoutingTable,
			requestManagementBindingTable, requestManagementLeave,
			requestManagementDirectJoin, requestManagementPermitJoining,
			requestManagementNetworkUpdate = 0x0038,
			requestManagementSiteSurvey = 0x003c,
			requestSecurityKeyAgreement = 0x0040,
			requestSecurityToken = 0x0041,
			requestSecurityLevel = 0x0042,
			requestSecuritySetConfiguration = 0x0043,
			requestSecurityGetConfiguration, requestSecurityStartKeyUpdate,
			requestSecurityDecommission, requestSecurityChallenge,
			respondNetworkAddress = 0x8000, respondExtendedAddress,
			respondNodeDescriptor, respondPowerDescriptor,
			respondSimpleDescriptor, respondActiveEndpoints,
			respondMatchDescriptors, respondComplexDescriptor = 0x8010,
			respondUserDescriptor, respondDiscoveryCache,
			confirmUserDescriptor = 0x8014, respondSystemServerDiscovery,
			respondStoreDiscovery, respondStoreNodeDescriptor,
			respondStorePowerDescriptor, respondStoreActiveEndpoints,
			respondStoreSimpleDescriptor, respondRemoveNodeCache,
			respondFindNodeCache, respondExtendedSimpleDescriptor,
			respondExtendedActiveEndpoints, respondParentAnnouncement,
			respondEndDeviceBind, respondBind, respondUnbind,
			respondBindRegister, respondReplaceDevice,
			respondStoreBackupBindEntry, respondRemoveBackupBindEntry,
			respondBackupBindTable, respondRecoverBindTable,
			respondBackupSourceBind, respondRecoverSourceBind,
			respondClearAllBindings,
			respondManagementNetworkDiscovery = 0x8030,
			respondManagementNeighborTable, respondManagementRoutingTable,
			respondManagementBindingTable, respondManagementLeave,
			respondManagementDirectJoin, respondManagementPermitJoining,
			respondManagementNetworkUpdate = 0x8038,
			respondManagementSiteSurvey = 0x803c,
			respondSecurityKeyAgreement = 0x8040,
			respondSecurityToken = 0x8041,
			respondSecurityLevel = 0x8042,
			respondSecuritySetConfiguration = 0x8043,
			respondSecurityGetConfiguration, respondSecurityStartKeyUpdate,
			respondSecurityDecommission, respondSecurityChallenge
		};

		// ZDP status code enumerations
		enum { statusSuccess, statusInvalidRequestType = 0x80,
			statusDeviceNotFound, statusInvalidEndpoint, statusNotActive,
			statusNotSupported, statusTimeout, statusNoMatch,
			statusNoEntry = 0x88, statusNoDescriptor, statusInsufficientSpace,
			statusNotPermitted, statusTableFull, statusNotAuthorized,
			statusDeviceBindingTableFull, statusInvalidIndex,
			statusFrameTooLarge, statusBadKeyNegotiationMethod };

	// Construction
	public:
		CZigBeeDeviceObject(CZigBeeApplicationSupportBase &aps);

	// Attributes
	public:
		// 16-bit manufacturer code, assigned by the ZigBee Alliance
		static const unsigned short wManufacturerID;

		// Configuration mode. If set to true, the device will accept all
		// ZDO commands without APS encryption. If set to false, such commands
		// that modify critical configuration data, e.g. bindings, will be
		// accepted only if encrypted with the trust center link-key
		bool m_bAllowConfiguration;

	// Operations
	public:
		// Returns the ZDO application instance given an APS instance
		static CZigBeeDeviceObject &GetInstance
			(const CZigBeeApplicationSupportBase &aps);

		// Cancels all pending ZDO requests, for example, when the stack is
		// stopped for a restart
		void CancelRequests();

		// Cancel a particular pending ZDO request
		bool CancelRequest(CZigBeeDeviceObjectRequest *&request);

		// Looks at the APS incoming security material to determine whether the
		// APS transmit options should include security and if so uses the
		// incoming security material for outgoing messages
		unsigned char MirrorIncomingSecurity() const;

		// Should be called for end devices and routers with end device
		// capabilities after joining or rejoining a network. Initiates
		// broadcast of a Device_annce ZDP command
		void AnnounceDevice();

		// This version provides a means for ZigBee Green Power sinks to
		// announce Green Power devices after pairing (using alias addressing
		// with the specified sequence number)
		void AnnounceDeviceEx(const CZigBeeAddress &address,
			const unsigned char bCapabilities = 0,
			const unsigned char nSequenceEx = 0);

		// Initiates an address lookup request (extended -> short)
		void RequestNetworkAddress(const unsigned long long qwAddress,
			const unsigned char nRequestType =
				CZigBeeDeviceObjectNetworkAddressRequest::typeSingle,
	 		const unsigned char nStartIndex = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium |
				CZigBeeApplicationSupportBase::enableReflection,
			const uint_fast8_t transmitOptions = 0,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates an address lookup request (short -> extended)
		void RequestExtendedAddress(const CZigBeeAddress &destination,
			const unsigned short wNetworkAddress,
			const unsigned char nRequestType =
				CZigBeeDeviceObjectNetworkAddressRequest::typeSingle,
	 		const unsigned char nStartIndex = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument1 = 0, void *const pArgument2 = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Requests the node descriptor of a remote device
		void RequestNodeDescriptor(const CZigBeeAddress &destination,
			const unsigned short wNetworkAddress,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const void *const pTLVs = 0, const uint_fast8_t cbTLVs = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Requests the node power descriptor of a remote device
		void RequestPowerDescriptor(const CZigBeeAddress &destination,
			const unsigned short wNetworkAddress,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Requests the simple descriptor for the specified endpoint
		void RequestSimpleDescriptor(const CZigBeeAddress &destination,
			const unsigned short wNetworkAddress, const unsigned char bEndpoint,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Requests a list of active endpoints
		void RequestActiveEndpoints(const CZigBeeAddress &destination,
			const unsigned short wNetworkAddress,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Requests end-point matching based on profile ID and input/output
		// cluster lists
		void RequestMatchDescriptors(const CZigBeeAddress &destination,
			const unsigned short wNetworkAddress,
			const unsigned short wProfileID, const std::set<unsigned short> &in,
			const std::set<unsigned short> &out,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0, const bool bMultipleResponses = false,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium |
				CZigBeeApplicationSupportBase::enableReflection,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a bind request
		void RequestBind(const CZigBeeAddress &source,
			const unsigned short wClusterID, const CZigBeeAddress &destination,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates an unbind request
		void RequestUnbind(const CZigBeeAddress &source,
			const unsigned short wClusterID, const CZigBeeAddress &destination,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates an request to query the binding table
		void RequestBindingTable(const CZigBeeAddress &destination,
			const unsigned char nStartIndex = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a leave request
		void RequestLeave(const CZigBeeAddress &destination,
			const unsigned long long qwAddress = 0,
			const bool bRemoveChildren = true, const bool bRejoin = false,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const unsigned int nTimeout = defaultTimeout,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a permit joining request
		void RequestPermitJoining(const CZigBeeAddress &destination,
			const unsigned char nPermitDuration, const bool bTrustCenter,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium |
				CZigBeeApplicationSupportBase::enableReflection,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a neighbor table request
		void RequestNeighborTable(const CZigBeeAddress &destination,
			const unsigned char nStartIndex = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a network discovery request
		void RequestNetworkDiscovery(const CZigBeeAddress &destination,
			const unsigned char nScanDuration,
			const unsigned int nScanChannels,
			const unsigned char nStartIndex = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a routing table request
		void RequestRoutingTable(const CZigBeeAddress &destination,
			const unsigned char nStartIndex = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a system server discovery request
		void RequestServerDiscovery(const CZigBeeAddress &destination,
			const unsigned short nMask = 0xffff,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium |
				CZigBeeApplicationSupportBase::enableReflection,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Returns the server mask (for use in the node descriptor or system
		// server discovery)
		unsigned short GetServerMask() const;

		// Requests a remote device to perform an energy detection scan on the
		// specified set of channels and report the results back
		void RequestEnergyScan(const CZigBeeAddress &destination,
			const unsigned int nScanChannels,
	 		const unsigned char nScanDuration = 5,
			const unsigned char nScanCount = 1,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Requests a remote device to update its network manager address and
		// channel mask to the specified values
		void RequestNetworkManagerUpdate(const CZigBeeAddress &destination,
			const unsigned short wNetworkManagerAddress,
			const unsigned int nChannelMask, const unsigned char nUpdateID,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Requests a remote device to change channels
		void RequestChannelChange(const CZigBeeAddress &destination,
			const unsigned char nLogicalChannel, const unsigned char nUpdateID,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium |
				CZigBeeApplicationSupportBase::enableReflection,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Sends a network update notification (Mgmt_NWK_Update_notify) to the
		// specified device, usually the designated network manager. These may
		// be unsolicited reports (notifications) or sent in response to a
		// prior Mgmt_NWK_Update_req request. Notice: Unsolicited reports
		// should never be sent with a 0x8xxx cluster ID, because they violate
		// ZDO sequence number conventions
		void RespondNetworkUpdate(const CZigBeeAddress &destination,
			const unsigned char nTransmitOptions,
			const unsigned char nSequence, const unsigned char nStatus,
			const unsigned int nScannedChannels,
			const std::list<unsigned char> &listEnergyDetect,
			const unsigned short wClusterID = respondManagementNetworkUpdate);

		// Should be called for routers after reboot. Initiates
		// broadcast of a Device_annce ZDP command
		void AnnounceParent();

		// Performs all steps required to add a binding or remove a binding.
		// Can be called by CZigBeeApplication::ProcessBindOrUnbind()
		// implementations to perform the actual work after having performed
		// any additional checks, for example. The return value is a ZDO status
		// code and can be placed unmodified in a ZDO response frame
		unsigned char DefaultBindOrUnbindProcessing
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const bool bTrustCenterOnly = false);

		// Initiates a site survey request (Mgmt_NWK_Beacon_Survey_req) using
		// the specified channel mask. If the channel mask is zero, no channel
		// list structure TLV will be added
		void RequestSiteSurvey(const CZigBeeAddress &destination,
			const uint_fast32_t channelMask,
			const void *const pTLVs = 0, const uint_fast8_t cbTLVs = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitEnableSecurity |
				CZigBeeApplicationSupportBase::transmitExtendedNonce |
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Initiates a key negotiation request (SEC_Key_Update_Req) using
		// the specified key negotiation scheme, e.g. SPEKE using Curve25519
		void RequestSecurityKeyAgreement(const CZigBeeAddress &destination,
			const uint_fast8_t nScheme, const uint_fast8_t nSecret,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Asks a remote device to initiate a key negotiation request
		// (SEC_Start_Key_Update_Req) providing the choice of key agreement
		// scheme and pre-shared secret to the remote device (as TLVs)
		void RequestSecurityStartKeyUpdate(const CZigBeeAddress &destination,
			const void *const pTLVs, const uint_fast8_t cbTLVs,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Obtains the symmetric security authentication token, which can be
		// used later on to rotate trust center link keys
		void RequestSecurityToken(const CZigBeeAddress &destination,
			const uint_fast8_t token,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitEnableSecurity |
				CZigBeeApplicationSupportBase::transmitExtendedNonce |
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Obtains the device authentication level for a remote device by
		// querying the trust center
		void RequestSecurityLevel(const CZigBeeAddress &target,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitEnableSecurity |
				CZigBeeApplicationSupportBase::transmitExtendedNonce |
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			const CZigBeeAddress &destination = CZigBeeAddress(),
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Sets security configuration on a remote device. The particular
		// security configuration changes are passed as TLVs and will be
		// forwarded to the remote device unmodified
		void RequestSecuritySetConfiguration(const CZigBeeAddress &destination,
			const void *const pTLVs, const uint_fast8_t cbTLVs,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitEnableSecurity |
				CZigBeeApplicationSupportBase::transmitExtendedNonce |
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Gets security configuration on a remote device. The list of global
		// TLVs will be forwarded to the remote device unmodified; optionally,
		// the caller might include TLVs
		void RequestSecurityGetConfiguration(const CZigBeeAddress &destination,
			const uint8_t *const tags, const uint_fast8_t count,
			const void *const pTLVs = 0, const uint_fast8_t cbTLVs = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitEnableSecurity |
				CZigBeeApplicationSupportBase::transmitExtendedNonce |
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Instructs a remote device to clears all bindings that refer to the
		// devices enumerated in the list. A wild-card EUI-64 of "all-F"s may
		// be used to delete all bindings, including group binding
		void RequestClearAllBindings(const CZigBeeAddress &destination,
			const uint64_t addresses[], const uint_fast8_t count,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Notifies a remote device that a another device has been removed
		// from the network by the Trust Center. The particular device will
		// be removed from the binding table, the APS key pair table, and all
		// other tables that refer to a particular device etc. A wild-card is
		// not permissible
		void RequestDecommission(const CZigBeeAddress &destination,
			const uint64_t addresses[], const uint_fast8_t count,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Queries a remote device for its current APS outgoing security frame
		// counter. The caller must provide the challenge TLV, and any other
		// TLVs the caller might want to convey in the same message
		void RequestSecurityChallenge(const CZigBeeAddress &destination,
			const void *const pTLVs, const uint_fast8_t cbTLVs,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument1 = 0, void *const pArgument2 = 0,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityImmediate,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

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

		// This notification is invoked upon completion of the ZigBee start-up
		// routine
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// This notification provides network status notifications, which might
		// either have been generated by the local NLME or were received over
		// the air in a network status command
		virtual void OnNotifyNetworkStatus(const unsigned char nStatus,
			const CZigBeeAddress &address);

	// Implementation
	protected:
		// Service for profile ID = 0, cluster ID = 0x0000, NWK_addr_req
		void OnNetworkAddressRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x8000, NWK_addr_rsp; and
		// Service for profile ID = 0, cluster ID = 0x8001, IEEE_addr_rsp
		void OnAddressResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0001, IEEE_addr_req
		void OnExtendedAddressRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0002, Node_Desc_req
		void OnNodeDescriptorRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0003, Power_Desc_req
		void OnPowerDescriptorRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0004, Simple_Desc_req
		void OnSimpleDescriptorRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0005, Active_EP_req
		void OnActiveEndpointsRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0006, Match_Desc_req
		void OnMatchDescriptorsRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0013, Device_annce
		void OnDeviceAnnouncement(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0015,
		// System_Server_Discover_req
		void OnServerDiscoveryRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x001f, parent_annce
		void OnParentAnnouncement(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x001f, parent_annce_rsp
		void OnParentAnnouncementResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0021, Bind_req
		void OnBindRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0022, Unbind_req
		void OnUnbindRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x002b,
		// Clear_All_bindings_req
		void OnClearAllBindingsRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0030, Mgmt_NWK_Disc_req
		void OnManagementNetworkDiscoveryRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0031, Mgmt_Lqi_req
		void OnManagementNeighborTableRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0032, Mgmt_Lqi_req
		void OnManagementRoutingTableRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0033, Mgmt_Bind_req
		void OnManagementBindingTableRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0034, Mgmt_Leave_req
		void OnManagementLeaveRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0036,
		// Mgmt_Permit_Joining_req
		void OnManagementPermitJoiningRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0038,
		// Mgmt_NWK_Update_req
		void OnManagementNetworkUpdateRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x8038,
		// Mgmt_NWK_Update_notify
		void OnManagementNetworkUpdateResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x003c,
		// Mgmt_NWK_Beacon_Survey_req
		void OnManagementSiteSurveyRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0040,
		// Security_Start_Key_Negotiation_req
		void OnSecurityKeyAgreementRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Default implementation of OnSecurityKeyAgreementRequest(), which
		// should be called by application-specific overloads of
		// OnSecurityKeyAgreementRequest() in addition to any custom logic
		// provided by the application
		CPacket DefaultOnSecurityKeyAgreementRequest
			(const CZigBeeAddress &source, CPacket &asdu,
			const bool bSecurityUsed);

		// Service for profile ID = 0, cluster ID = 0x0041,
		// Security_Get_Authentication_Token_req
		void OnSecurityTokenRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0042,
		// Security_Get_Authentication_Level_req
		void OnSecurityLevelRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0043,
		// Security_Set_Configuration_req
		void OnSecuritySetConfigurationRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0044,
		// Security_Get_Configuration_req
		void OnSecurityGetConfigurationRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0045,
		// Security_Start_Key_Update_req
		void OnSecurityStartKeyUpdateRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0046,
		// Security_Decommission_req
		void OnDecommissionRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Service for profile ID = 0, cluster ID = 0x0047,
		// Security_Challenge_req
		void OnSecurityChallengeRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

	// Implementation
	public:
		// Limit for queued request
		static const unsigned char nQueueLimit;

		// Global TLVs modifiable via ZDO set security configuration request
		static const CZigBeeTLVDictionaryEntry globalTLVs[];

		// Default ZDO transaction timeout in microseconds
		static const uint32_t defaultTimeout;

		// Parent announce delay and jitter (lower bound, range) in symbol
		// periods. Here, the backoff is 10 seconds, with a random jitter of
		// 10 seconds
		enum { parentAnnounceBackoff = 625000, parentAnnounceJitter = 625000 };

		// Number of child records per parent_annce frame
		enum { maxChildrenPerParentAnnouncement = 10 };

		// Internal status code
		enum { statusSuppressResponse = 0xff };

		// Pending requests
		std::set<CZigBeeDeviceObjectRequest *> m_requests;

		// Parent announce timer. The timer initially starts after reboot and
		// is set to fire within 10..20 seconds. When it fires, the next chunk
		// of child table entries is broadcast. Whenever another announcement
		// is received, the device will back-off again for 10..20 seconds...
		CCompactTimer *m_pParentAnnouncementTimer;

		// The results of the most recent network discovery scan. A discovery
		// request with a start index = 0 will trigger a new network discovery
		// and subsequent requests with start index > 0 will not (instead they
		// return the next results from the result set)
		std::list<CZigBeeNetworkDescriptor> *m_pNetworks;

		// ZDP sequence counter
		unsigned char m_nSequence;

		// Returns true if restricted command may be executed. This must only
		// be invoked from within a ZDO command handler
		bool IsConfigurationAllowed(const CZigBeeAddress &device,
			const bool bSecurityUsed) const;

		// Creates a ZDO command frame with enough room for the specified
		// payload. Room for the sequence counter is automatically reserved
		static CPacket CreatePacket(const CPacketData::size_type cbPayload,
			const CPacketData::size_type cbHeaderCapacity = 0);

		// Adds an address object to a packet
		static void AppendAddress(CPacket &packet,
			const CZigBeeAddress &address);

		// Adds the list of associated devices starting at the specified index
		void AppendAssociatedDeviceList(CPacket &packet,
			const unsigned char nStartIndex = 0);

		// Processes a network or IEEE address request, i.e. performs address
		// lookup and stores the result in the response payload
		void ProcessAddressRequest(CZigBeeAddress &address,
			CZigBeeDeviceObjectNetworkAddressResponse &response,
			const bool bBroadcastRequest);

		// Transmits a ZDP command frame (either request or response) and
		// optionally waits for a mating response/confirmation frame, if
		// applicable
		void QueueCommand(const CZigBeeAddress &destination,
			const unsigned short wClusterID, const unsigned char nSequence,
			CPacket &packet, const unsigned char bEnhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const unsigned char bTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			const unsigned char bRadius = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument1 = 0, void *const pArgument2 = 0,
			const unsigned int nTimeout = 0,
			const bool bMultipleResponses = false,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Transmits a ZDP command frame (either request or response) and
		// optionally waits for a mating response/confirmation frame, if
		// applicable. This version provides alias addressing, e.g. for sending
		// a device_annce on behalf of a Green Power device
		void QueueCommandEx(const CZigBeeAddress &destination,
			const unsigned short wClusterID, const unsigned char nSequence,
			CPacket &packet, const unsigned char bEnhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const unsigned char bTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			const unsigned char bRadius = 0,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete = 0,
			void *const pArgument1 = 0, void *const pArgument2 = 0,
			const unsigned int nTimeout = 0,
			const CZigBeeAddress &source = CZigBeeAddress(),
			const unsigned char nSequenceEx = 0,
			const bool bMultipleResponses = false,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Called when a request has been processed. For requests that require
		// a response, the request is considered complete, when a mating
		// response or confirmation frame has been received, or the request
		// has timed out
		void CompleteRequest(CZigBeeDeviceObjectRequest *pRequest,
			const unsigned char nStatus, CPacket *const pResponse = 0);

		// Called when a request has timed-out, i.e. there was no response to
		// the original request within the specified timeout period
		void OnRequestTimeout(void *const pArgument);

		// Returns true, if we are currently anticipating a response with the
		// specified sequence counter. Cancels the pending timeout timer, if
		// bCancelTimer is true
		CZigBeeDeviceObjectRequest *
			IsAnticipatedResponse(const unsigned char nSequence,
			const bool bCancelTimer = true) const;

		// Performs typical request preprocessing, i.e. certain typical sanity
		// checks. Will remove the ZDP header (sequence number) and leave the
		// payload in place. Returns false if the request is not sane, e.g.
		// the ZDP payload is less than the minimum required; similarly, if a
		// request is received as broadcast or multicast, which shall only be
		// accepted as unicast, this will be a reason to reject the request
		bool StandardRequestPreprocessing(const CZigBeeAddress &destination,
			const CZigBeeAddress &source ,CPacket &asdu,
			const uint_fast16_t clusterID, uint8_t &sequence,
			const CPacketData::size_type requiredPayload = 0,
			const bool bAcceptMulticast = false);

		// Performs default processing that is required to complete pending
		// requests, for example. Should be called before a frame is processed.
		// The caller indicates whether the pending request
		bool StandardResponseProcessing(const unsigned short wClusterID,
			CPacket &asdu, CZigBeeDeviceObjectRequest *&pRequest,
			const bool bRequireRequest = true);

		// Initiates a bind or unbind request
		void RequestBindOrUnbind(const CZigBeeAddress &source,
			const unsigned short wClusterID, const CZigBeeAddress &destination,
			const CZigBeeDeviceObjectRequest::CALLBACK pfnComplete,
			void *const pArgument, const bool bUnbind,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			const uint_fast8_t transmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Handles incoming bind and unbind requests
		void HandleBindOrUnbindRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const bool bSecurityUsed);

		// Transmit a simple response with the specified status to a request
		// originating in the specified source and addressed to the specified
		// destination. Notice that the destination is only evaluated for
		// unicast vs. multicast, in order to decide whether or not to send
		// a response frame at all
		void TransmitSimpleResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const uint_fast16_t clusterID,
			const uint_fast8_t sequence, const uint_fast8_t status,
			const uint_fast8_t enhancedOptions =
				CZigBeeApplicationSupportBase::priorityImmediate,
			const CZigBeeDeviceObjectRequest::CALLBACK onComplete = 0,
			void *const argument1 = 0, void *const argument2 = 0);

		// Default service for all response frames that return a status, i.e.
		// profile ID = 0, cluster ID = 0x8021, Bind_rsp
		// profile ID = 0, cluster ID = 0x8022, Unbind_rsp
		void OnSimpleResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wClusterID,
			CPacket &asdu, const unsigned char bStatus,
			const bool bSecurityUsed, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Handles common parts of NWK_addr_req and IEEE_addr_req
		void HandleAddressRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, CPacket &asdu,
			CZigBeeDeviceObjectNetworkAddressResponse &response,
			const unsigned short wClusterID, const unsigned char nSequence,
			const unsigned char nRequestType, const unsigned char nStartIndex);

		// Called when the leave response has been transmitted
		static void OnManagementLeaveResponseComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Called either five or sixty seconds after the leave request has been
		// completed. If the request was unicast, the device will restart after
		// five seconds. In order to participate in network flooding, in case
		// of a broadcast request, the leave operation will be deferred by 60
		// seconds, instead.
		void OnLeaveDelayElapsed(void *pArgument);

		// Processes a Mgmt_NWK_Update_req of type "energy scan"
		unsigned char HandleEnergyScanRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned char nSequence,
			const CZigBeeDeviceObjectManagementEnergyScanRequest &request);

		// Called after an energy scan, which was remotely requested by the
		// network manager, has completed
		static void OnEnergyDetectionScanComplete(const unsigned char nStatus,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> &listEnergyDetect,
			void *pArgument);

		// Processes a Mgmt_NWK_Update_req of type "network manager update"
		unsigned char HandleUpdateRequest
			(const CZigBeeDeviceObjectManagementUpdateRequest &request);

		// Processes a Mgmt_NWK_Update_req of type "channel change"
		unsigned char HandleChannelChangeRequest
			(const CZigBeeDeviceObjectManagementChannelChangeRequest &request);

		// Called when a channel change has been instigated. This will make the
		// channel change effective after a broadcast delivery timeout
		void OnChannelChangeDelayElapsed(void *pArgument);

		// Schedules the next parent announcement to occur in 10..20 seconds
		void ScheduleNextParentAnnouncement();

		// Called when the next parent announcement frame should be sent
		void OnParentAnnounceJitterElapsed(void *pArgument);

		// Create the next parent announcement frame and determine of another
		// parent announcement frame needs to be sent
		bool CreateParentAnnouncement(CPacket &asdu, bool &bHaveMore);

		// This function removes conflicting child device entries from the
		// neighbor table and is also able to populate a list of actual
		// conflicts
		void ProcessParentConflicts
			(const CZigBeeDeviceObjectChildInfoList &children,
	 		CZigBeeDeviceObjectChildInfoList *const pConflicts = 0);

		// This gets called by the framework when a node_power_desc_req is
		// received and a response needs to be generated
		static void QueryPowerDescriptor
			(CZigBeeNodePowerDescriptor &descriptor);

		// Completion handler for network discovery requests
		static void OnNetworkDiscoveryComplete(const unsigned char nStatus,
			const std::list<CZigBeeNetworkDescriptor> &networks,
			void *pArgument);

		// Constructs a Mgmt_NWK_disc_rsp frame conveying results of a network
		// discovery scan. The scan results are expected to be available in
		// m_pNetworks. These results will be deleted once the last chunk of
		// network identifiers has been sent
		void RespondNetworkDiscovery(const CZigBeeAddress &destination,
			const unsigned char nTransmitOptions, const unsigned char nSequence,
			const unsigned char nStatus, const unsigned char nStartIndex = 0);

		// Completion handler for site survey requests
		static void OnSiteSurveyComplete(const unsigned char nStatus,
			const std::list<CZigBeeNetworkDescriptor> &networks,
			void *pArgument);

		// Constructs a Mgmt_Beacon_Survey_rsp frame conveying results of a
		// network discovery scan. The scan results are expected to be
		// available in
		// m_pNetworks. These results will be deleted once the last chunk of
		// network identifiers has been sent
		void RespondSiteSurvey(const CZigBeeAddress &destination,
			const unsigned char nTransmitOptions, const unsigned char nSequence,
			const unsigned char nStatus, const void *const pTLVs = 0,
			const uint_fast8_t cbTLVs = 0);

		// Continues processing of a management leave request, i.e. issues an
		// NLME-LEAVE.request
		void ContinueLeave(CZigBeeDeviceObjectManagementLeaveRequest *pRequest);

		// Completion handler for an NLME-LEAVE.request, which was issued in
		// response to a mgmt_leave_req
		void OnLeaveComplete(const uint_fast8_t nStatus,
			const CZigBeeAddress &device, const bool bRejoin, void *pArgument);

		// This method processes a configuration mode TLV in a security set
		// configuration request
		uint_fast8_t OnSetConfigurationMode(const CZigBeeTLVBase &tlv);

		// This method processes a network PANID change TLV in a security set
		// configuration request
		uint_fast8_t OnSetPendingPANDIDChange(const CZigBeeTLVBase &tlv);

		// This method processes a network channel change TLV in a security
		// set configuration request
		uint_fast8_t OnSetPendingChannelChange(const CZigBeeTLVBase &tlv);

		// Given a list of extended addresses encoded as an extended address
		// list TLV, deletes all affected binding table entries. Returns SUCCESS
		// when all delete operations where successful, or a failure status if
		// any operation failed. In case of failure to delete a particular
		// binding, other bindings will still be attempted to be deleted
		uint_fast8_t ClearAllBindings(const CZigBeeTLVBase &tlv,
			const CZigBeeAddress &initiator, const bool bAcceptWildcard = true);

		// Create a ZDO transmission sequence number by consecutively
		// incrementing the internal sequence counter with roll-over allowed.
		// The created sequence number must not be used by any currently
		// pending ZDO transmissions
		unsigned char AdvanceSequenceCounter();

#ifdef _DEBUG
		// Returns the human-readable identifier of the specified ZDP status
		// enumeration
		static const char *GetStatusString(const unsigned char nStatus);

		// Dumps pending ZDO requests
		virtual void Dump() const;
#endif

};
