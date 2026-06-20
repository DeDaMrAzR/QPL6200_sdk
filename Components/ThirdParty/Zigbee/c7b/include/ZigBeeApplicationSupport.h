// ZigBeeApplicationSupport.h : header file
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
// as of January 17, 2008
//
// This module implements the application support sublayer (APS) according to
// ZigBee 2007

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeNetwork;
class CZigBeeNetworkScanManager;
class CZigBeeDeviceObjectRequest;
class CZigBeeAuxiliaryHeader;
class CZigBeeNetworkSecurityMaterial;
class CZigBeeNeighborInfo;
class CZigBeeApplication;
class CZigBeeApplicationSupport;
class CZigBeeStartupChannelInformation;
class CZigBeeTLVs;
class CZigBeeBeacon;


/////////////////////////////////////////////////////////////////////////////
// byte packing for all packet definitions

#pragma pack(push, 1)

/////////////////////////////////////////////////////////////////////////////
// CZigBeeAHR, APS header, clause 2.2.5.1

class CZigBeeAHR
{
	// typedefs & enums
	public:
		// Frame control field
		enum { maskType = 0x03, maskDeliveryMode = 0x0c, shiftDeliveryMode = 2,
			acknowledgeFormatCommand = 0x10, securityEnabled = 0x20,
			acknowledgeRequest = 0x40, extendedHeaderPresent = 0x80 };

		// Frame type sub-field, possible values
		enum { typeData, typeCommand, typeAcknowledgment, typeStub };

		// Delivery mode sub-field, possible values
		enum { deliveryUnicast, deliveryIndirect, deliveryBroadcast,
			deliveryGroupAddressing };

		// Fragmentation sub-field within extended header
		enum { fragmentNone, fragmentFirst, fragmentSubsequent,
			fragmentReserved };

		// Maximum number of bytes an AHR can have. This would be for an
		// acknowledgment frame to a fragmented data transfer, i.e. with an
		// extended header of 3 bytes
		enum { maxSize = 12 };

	// Attributes
	public:
		// Frame control field, clause 2.2.5.1.1
		unsigned char m_bFrameControl;

		// Destination endpoint, clause 2.2.5.1.2
		// Group address, clause 2.2.5.1.3
		// Cluster identifier, clause 2.2.5.1.4
		// Profile identifier, clause 2.2.5.1.5
		// Source endpoint, clause 2.2.5.1.6
		// APS counter, clause 2.2.5.1.7
		// Extended header, clause 2.2.5.1.8

	// Operations
	public:
		// Returns true, if this frame appears to be valid, false otherwise.
		// Invalid frames should be rejected and the only method allowed to be
		// called on frames that have not been checked for validity is
		// GetType()
		bool IsValid(const CPacketData::size_type size) const;

		// Returns the frame type
		unsigned char GetType() const;

		// Returns the delivery mode
		unsigned char GetDeliveryMode() const;

		// Returns the size of the variable-length application layer addressing
		// material contained in the APS header. Always followed by APS counter
		CPacketData::size_type GetAddressingMaterialSize() const;

		// Returns the APS header size, including fixed-length portions and
		// the length of optional fields
		CPacketData::size_type GetSize() const;

		// Same as GetSize(), but for secured frames, also includes the length
		// of the auxiliary header
		CPacketData::size_type GetSizeEx() const;

		// Returns the destination endpoint address, if present in the APDU
		bool GetDestinationEndpoint(unsigned char &bEndpoint) const;

		// Returns the group address, if present in the APDU
		bool GetGroupAddress(unsigned short &wAddress) const;

		// Returns profile and cluster identifiers, if present in the APDU
		bool GetProfileAndClusterIDs(unsigned short &wProfileID,
			unsigned short &wClusterID) const;

		// Returns the source endpoint, if present in the APDU
		bool GetSourceEndpoint(unsigned char &bEndpoint) const;

		// Returns the APS counter, which is always present in an APDU.
		unsigned char GetCounter() const;

		// Returns fragmentation information contained in the extended header.
		// Block number and acknowledgment bitfield might not be valid. The
		// caller is responsible for assessing whether they are or not
		bool GetFragmentationMode(unsigned char &bFragmentType,
			unsigned char &bBlockNumber,
			unsigned char &bAcknowledgements) const;

#ifdef _DEBUG
	public:
		// Dumps the fields of an APS header
		void Dump() const;
#endif

};

inline unsigned char CZigBeeAHR::GetType() const
	{ return (m_bFrameControl & maskType); }

inline unsigned char CZigBeeAHR::GetDeliveryMode() const
	{ return (m_bFrameControl & maskDeliveryMode) >> shiftDeliveryMode; }


/////////////////////////////////////////////////////////////////////////////
// restore previous packing

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeBindingTargets

class CZigBeeBindingTargets : public std::list<CZigBeeBindingTarget>
{
	// Construction
	public:
		CZigBeeBindingTargets();

	// Attributes
	public:
		// True, if the list shall be deleted automatically
		bool m_bAutoDelete;

		// True, as long as the list is in use
		bool m_bActive;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeFragmentTX

class CZigBeeFragmentTX
{
	// typedefs & enums
	public:
		enum { statePending, stateAddressLookup, stateTransmitInProgress,
			stateConfirmed, stateAcknowledged };

	// Construction
	public:
		CZigBeeFragmentTX(const CPacketData::size_type nOffset,
			const CPacketData::size_type nSize,
			const unsigned char nBlock);

		CZigBeeFragmentTX(const CZigBeeFragmentTX &fragment);

	// Attributes
	public:
		// Timestamp of the related NLME-DATA.confirm. Valid, if the state is
		// confirmed or acknowledged
		unsigned int m_nTimestamp;

		// Start of the fragment
		CPacketData::size_type m_nOffset;

		// Size of the fragment
		CPacketData::size_type m_nSize;

		// Fragment block number
		unsigned char m_nBlock;

		// A network layer handle is required for the transmission of a single
		// fragment
		unsigned char m_nHandle;

		// States a fragment can take (one of the above)
		unsigned char m_nState;

		// Status of the related NLME-DATA.confirm. Valid, if the state is
		// confirmed or acknowledged
		unsigned char m_nStatus;

	// Operations
	public:
		// Less operator (support for sorted STL containers)
		bool operator<(const CZigBeeFragmentTX &fragment) const;

#ifdef _DEBUG
		// Dumps a transmit fragment
		void Dump() const;
#endif

};

inline bool CZigBeeFragmentTX::operator<(const CZigBeeFragmentTX &fragment) const
	{ return m_nBlock < fragment.m_nBlock; }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTransactionTX

class CZigBeeTransactionTX : public CCompactTimerClient
{
	// Construction
	public:
		CZigBeeTransactionTX(CZigBeeApplicationSupport &aps);
		CZigBeeTransactionTX(const CZigBeeTransactionTX &transaction);

	// Attributes
	public:
		// A number of fragments that comprise the APDU
		std::set<CZigBeeFragmentTX> m_fragments;

		// A packet object holding a variable length AHR template that is
		// attached in front of each fragment and updated as appropriate
		CPacket m_header;

		// The ASDU that is being transferred in this transaction
		CPacket m_asdu;

		// Destination address (APSDE-DATA.request parameter)
		CZigBeeAddress m_destination;

		// Cookie passed to confirmation handler
		void *m_pCookie;

		// A responsive mode cookie (might be zero when failed to enter
		// responsive mode)
		unsigned int m_nCookie;

		// A list of binding targets to which a single request is being sent,
		// or null if this transaction is not referencing the binding table
		CZigBeeBindingTargets *m_pTargets;

		// Radius (APSDE-DATA.request parameter)
		unsigned char m_bRadius;

		// Transmit options (APSDE-DATA.request parameter)
		unsigned char m_bTransmitOptions;

		// Enhanced transmit options (APSDE-DATA.request parameter)
		unsigned char m_bEnhancedOptions;

		// Alias sequence number (APSDE-DATA.request parameter)
		unsigned char m_nAliasSequence;

		// Alias source address (APSDE-DATA.request parameter)
		CZigBeeAddress m_alias;

		// Number of tranmission attempts that have been made
		unsigned char m_nAttempt;

		// True, if an acknowledgment has been received. Notice that we might
		// received an acknowledgment sent in response to an earlier attempt...
		bool m_bAcknowledged;

		// True, if a route repair has been initiated once for this request.
		bool m_bRouteInvalidated;

	// Operations
	public:
		// Returns the ZigBee application support layer header object stored in
		// m_header
		const CZigBeeAHR &GetHeader() const;

		// Returns the first fragment that is in the pending state, i.e. it is
		// either the next to be transmitted or is currently being transmitted
		std::set<CZigBeeFragmentTX>::iterator GetNextFragment
			(const uint_fast8_t state = CZigBeeFragmentTX::statePending,
			const uint_fast8_t alternateState = UINT8_MAX);

		// Transmits the next fragment
		bool TransmitNextFragment();

		// Handles the NLDE-DATA.indication for a single fragment, which has
		// been identified and passed as the first argument
		void ConfirmData(const std::set<CZigBeeFragmentTX>::iterator fragment,
			const unsigned char nStatus, const unsigned int nTimestamp);

		// Process an acknowledgment frame
		void ProcessAcknowledgment(const CZigBeeAHR &header);

		// Completes a transaction by issuing an APSDE-DATA.confirm to the
		// specified application
		void CompleteTransaction(const unsigned char nStatus);

		// Indicate whether transmission is in progress
		bool IsOngoing() const;

	// Implementation
	public:
		// Link to APS instance
		CZigBeeApplicationSupport &m_aps;

		// A timer that is used to generate the interframe space in case of a
		// fragmented transmission and also implements the acknowledgment
		// timeout processing
		CCompactTimer *m_pTimer;

		// Called when the acknowledgment timeout has expired, without having
		// received an acknowledgment frame
		void OnAcknowledgmentTimeoutExpired(void *pArgument);

		// Called when the network layer reported a failure status during an
		// attempt to transmit an APS frame
		void OnRetryBackoffExpired(void *pArgument);

		// Finds this element in the TX transaction list at erases it
		void Destroy();

		// Called when the network address lookup is complete
		static void OnNetworkAddressLookupComplete
			(const CZigBeeAddress &address, void *pArgument,
			const unsigned char nStatus);

#ifdef _DEBUG
		// Dumps a transmit transaction
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTransactionRX

class CZigBeeTransactionRX
{
	// typedefs & enums
	public:
		enum { timestampValid = 0x01, sequenceValid = 0x02 };

	// Construction
	public:
		CZigBeeTransactionRX(const uint_fast16_t source);

		CZigBeeTransactionRX(const uint_fast16_t source,
			const uint_fast8_t sequence,
			const unsigned int timestamp = 0,
			const uint_fast8_t flags = timestampValid | sequenceValid);

	// Attributes
	public:
		// Timing information (receive time-stamp)
		unsigned int m_timestamp;

		// Source address
		uint16_t m_source;

		// APS sequence counter
		uint8_t m_sequence;

		// Flags
		uint8_t m_flags;

		// Total number of blocks expected
		uint8_t m_totalBlocks;

		// Number of blocks received
		uint8_t m_receivedBlocks;

		// In case of a fragmented ASDU, the reassembled ASDU
		CPacket m_asdu;

	// Operations
	public:
		bool operator<(const CZigBeeTransactionRX &rx) const;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSecurityMaterialUpdate

class CZigBeeApplicationSecurityMaterialUpdate :
	public CZigBeeApplicationSecurityMaterial
{
	// Construction
	public:
		CZigBeeApplicationSecurityMaterialUpdate();

	// Attributes
	public:
		// Timer that fires when key update validation failed, i.e. no APS
		// message encrypted with the new key is received in a timely fashion
		CCompactTimer *m_pTimer;

	// Implementation
	public:
		virtual ~CZigBeeApplicationSecurityMaterialUpdate();
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeJoiningDevice

class CZigBeeJoiningDevice
{
	// typedefs & enums
	public:
		// Joiner state: Establishing a link-key to secure the pipe between
		// trust center and joiner, and then optionally perform a device
		// interview via other application level messages, e.g. instigate a
		// DTLS certificate exchange. At the end of this the device is either
		// permanently added to the network, or not permitted onto the network.
		// In either case this ephemeral joiner information will vanish once
		// these steps have completed
		enum { stateKeyEstablishment, stateInterview };

	// Construction
	public:
		CZigBeeJoiningDevice(const CZigBeeAddress &address,
			const CZigBeeAddress &relay = CZigBeeAddress(),
			const void *const pTLVs = 0,
			const uint_fast8_t cbTLVs = 0);

	// Attributes
	public:
		// The address of the prospective joiner. This might be a combination
		// of both the short and extended address
		CZigBeeAddress m_address;

		// The network short address of the relaying router to use during the
		// onboarding process. This might be the trust center itself, in which
		// case the address is zero (0x0000), corresponding to the trust center
		// address, or it might be the address of a remote device
		CZigBeeAddress m_relay;

		// The set of TLVs that has been conveyed for by this joiner in its
		// network commissioning request
		CPacket m_tlvs;

		// Remaining lifetime for this joining device. This is to keep track of
		// stale entries in the joiner set and remove them after the security
		// timeout has expired and no subsequent activity prolonged the timeout
		CCompactTimerService::ticks_diff_t m_lifetime;

		// Extended device capabilities as advertised by the joiner in its
		// commissioning TLVs. This includes an indication of whether or not
		// this is a Zigbee Direct virtual device (bit #0); defaults to zero
		// if no such TLV was included. Note that this information is only
		// updated by ContinueAuthentication() and can therefore be assumed to
		// be valid in FinishAuthentication()
		uint16_t m_capabilities;

		// One of the states above
		uint8_t m_state;

		// The pre-shared secret that has been selected by the trust center for
		// key agreement out of the set of commonly supported/available
		// pre-shared secrets
		uint8_t m_selectedSecret;

	// Operations
	public:
		// Support for sorted STL containers
		bool operator<(const CZigBeeJoiningDevice &info) const;

	// Implementation
	public:

#ifdef _DEBUG
		// In debug builds, prints information about this joining device
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSupportManagement

class CZigBeeApplicationSupportManagement : public CCompactTimerClient,
	public CZigBeeApplicationSupportManagementBase
{
	// typedefs & enums
	public:
		// Invoked as a result of a network discovery as soon as network
		// discovery is complete
		typedef void (*ONNETWORKDISCOVERYCOMPLETE)
			(const unsigned char nStatus,
			const std::list<CZigBeeNetworkDescriptor> &networks,
			void *pArgument);

		// Invoked as a result of an energy detection scan
		typedef void (*ONENERGYDETECTIONSCANCOMPLETE)
			(const unsigned char nStatus, const unsigned int nUnscannedChannels,
			const std::list<unsigned char> &listEnergyDetect,
			void *pArgument);

		// Command identifiers for APS command frames
		enum { commandSKKE1 = 1, commandSKKE2, commandSKKE3, commandSKKE4,
			commandTransportKey, commandUpdateDevice, commandRemoveDevice,
			commandRequestKey, commandSwitchKey, commandEAInitiatorChallange,
			commandEAResponderChallange, commandEAInitiatorMACData,
			commandEAResponderMACData, commandTunnel, commandVerifyKey,
			commandConfirmKey, commandRelayDownstream, commandRelayUpstream };

		// Update device status codes
		enum { updateDeviceSecuredRejoin, updateDeviceUnsecuredJoin,
			updateDeviceLeft, updateDeviceUnsecuredRejoin };

		// Key types for key transport messages
		enum { typeNetworkKey = 1, typeApplicationLinkKey = 3,
			typeTrustCenterLinkKey = 4,
			typeEphemeralGlobalAuthorizationKey = 0xb0,
			typeEphemeralUniqueAuthorizationKey, typeBasicAuthorizationKey,
			typeAdministrativeAuthorizationKey };

	// Construction
	public:
		CZigBeeApplicationSupportManagement(CZigBeeApplicationSupport &aps);

	// Attributes
	public:
		// Returns true while the stack is starting up
		bool IsStartupInProgress() const;

		// Returns true if startup has been called once after a cold start
		bool HasStartedOnce() const;

		// Returns true if it is required to instigate a start-up sequence,
		// because the networking stack has not been started yet, or a rejoin
		// attempt triggered by parent-link failure detection has failed and
		// further attempts must be performed
		bool IsStartupRequired() const;

		// Returns true while the device is waiting for the initial network key
		// transport to occur
		bool IsKeyTransportPending() const;

		// Returns true if the local device is fully authenticated on the
		// network, i.e. it has a network key and a unique link-key with the
		// trust center (if the trust center supports unique keys)
		bool IsAuthenticated() const;

	// Operations
	public:
		// NLME-NETWORK-DISCOVERY.confirm(Status, NetworkCount,
		// NetworkDescriptor), clause 3.2.2.2
		void OnConfirmNetworkDiscovery(const unsigned char nStatus,
			const std::list<CZigBeeNetworkDescriptor> &networks);

		// NLME-NETWORK-FORMATION.confirm(Status), clause 3.2.2.4
		void OnConfirmNetworkFormation(const unsigned char nStatus);

		// NLME-PERMIT-JOINING.confirm(Status), clause 3.2.2.6
		void OnConfirmPermitJoining(const unsigned char nStatus);

		// NLME-START-ROUTER.confirm(Status), clause 3.2.2.8
		void OnConfirmStartRouter(const unsigned char nStatus);

		// NLME-ED-SCAN.confirm(Status, UnscannedChannels, EnergyDetectList),
		// clause 3.2.2.10
		void OnConfirmEnergyDetectionScan(const unsigned char nStatus,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> &listEnergyDetect);

		// NLME-JOIN.indication(NetworkAddress, ExtendedAddress,
		// CapabilityInformation, RejoinNetwork, SecureJoin), clause 3.2.2.12
		void OnIndicateJoin(const CZigBeeAddress &address,
			const unsigned char bCapabilities,
			const unsigned char nRejoinNetwork, const bool bSecureRejoin);

		// NLME-JOIN.confirm(Status, NetworkAddress, ExtendedPANID,
		// ActiveChannel), clause 3.2.2.13
		void OnConfirmJoin(const unsigned char nStatus,
			const unsigned short wNetworkAddress,
			const unsigned long long qwExtendedPANID,
			const unsigned char nActiveChannel);

		// NLME-LEAVE.indication(DeviceAddress, Rejoin), clause 3.2.2.17
		void OnIndicateLeave(const CZigBeeAddress &device, const bool bRejoin);

		// NLME-LEAVE.confirm(Status, DeviceAddress), clause 3.2.2.18, extended
		// by bRejoin parameter
		void OnConfirmLeave(const unsigned char nStatus,
			const CZigBeeAddress &device, const bool bRejoin);

		// NLME-SYNC.confirm(Status), clause 3.2.2.24
		void OnConfirmSynchronization(const unsigned char nStatus);

		// NLME-NWK-STATUS.indication(Status, NetworkAddr), clause 3.2.2.30
		void OnIndicateNetworkStatus(const unsigned char nStatus,
			const CZigBeeAddress &address, const bool bLocallyInitiated = true);

		// Direct call version of an APSME-BIND.request(SrcAddr, SrcEndpoint,
		// ClusterId, DstAddrMode, DstAddr, DstEndpoint), clause 2.2.4.3.1,
		// resulting in an immediate APSME-BIND.confirm(Status, SrcEndpoint,
		// ClusterId, DstAddrMode, DstAddr, DstEndpoint), clause 2.2.4.3.2.
		// Notice that source address is always the local address (no cache is
		// supported)
		unsigned char Bind(const unsigned char bSourceEndpoint,
			const unsigned short wClusterID, const CZigBeeAddress &destination);

		// Direct call version of an APSME-UNBIND.request(SrcAddr, SrcEndpoint,
		// ClusterId, DstAddrMode, DstAddr, DstEndpoint), clause 2.2.4.3.3,
		// resulting in an immediate APSME-UNBIND.confirm(Status, SrcEndpoint,
		// ClusterId, DstAddrMode, DstAddr, DstEndpoint), clause 2.2.4.3.4.
		// Notice that source address is always the local address (no cache is
		// supported)
		unsigned char Unbind(const unsigned char bSourceEndpoint,
			const unsigned short wClusterID, const CZigBeeAddress &destination);

		// Extended version of unbind, which can be used to remove specific
		// binding targets, or all binding targets for the given source
		// endpoint and cluster if the destination address type is "none"
		unsigned char UnbindEx(const unsigned char bSourceEndpoint,
			const unsigned short wClusterID, const CZigBeeAddress &destination);

		// Attempts to map the specified source address, source endpoint and
		// cluster ID key to a valid destination address, either a 64-bit IEEE
		// extended address and endpoint number, or a 16-bit group address.
		// Returns NOT_SUPPORTED, if the device does not support binding,
		// NO_BOUND_DEVICE, if no matching entry was found, or SUCCESS if there
		// are one or more binding targets available
		unsigned char LookupBinding(const unsigned char bSourceEndpoint,
			const unsigned short wClusterID,
			CZigBeeBindingTargets &targets) const;

		// Clear bindings for a specific endpoint or all endpoints
		virtual void ClearBindings(const unsigned char bSourceEndpoint = 0xff);

		// Walks the binding table and results all matching items. Needs to be
		// invoked in a loop. Initially, pContext must be initialized to null
		// by the caller, and this method must be invoked repeatedly until it
		// returns false
		bool WalkBindingTable(void *&pContext, CZigBeeBindingSource &source,
			CZigBeeBindingTarget &target) const;

		// Appends binding table entries to a packet object starting at the
		// specified index and limited by available space (capacity). Returns
		// the number of entries appended. The entries are stored in the common
		// ZDP format
		unsigned char AppendBindingTable(CPacket &packet,
			unsigned char &nTotal, unsigned char &nAppended,
			unsigned char nStartIndex, CPacketData::size_type nCapacity) const;

		// Direct call version of an APSME-ADD-GROUP.request(GroupAddress,
		// Endpoint), clause 2.2.4.5.1, resulting in an immediate
		// APSME-ADD-GROUP.confirm(Status, GroupAddress, Endpoint),
		// clause 2.2.4.5.2. A ubisys extension to this request is the MSB of
		// the status code being returned: If set to 1 the entry already
		// existed before, otherwise it has just been created
		virtual unsigned int AddGroup(const unsigned short wGroupAddress,
			const unsigned char bEndpoint);

		// Direct call version of an APSME-REMOVE-GROUP.request(GroupAddress,
		// Endpoint), clause 2.2.4.5.3, resulting in an immediate
		// APSME-REMOVE-GROUP.confirm(Status, GroupAddress, Endpoint),
		// clause 2.2.4.5.4
		virtual unsigned char RemoveGroup(const unsigned short wGroupAddress,
			const unsigned char bEndpoint);

		// Direct call version of an APSME-REMOVE-ALL-GROUPS.request(Endpoint),
		// clause 2.2.4.5.5, resulting in an immediate
		// APSME-REMOVE-ALL-GROUPS.confirm(Endpoint), clause 2.2.4.5.6
		virtual unsigned char RemoveAllGroups(const unsigned char bEndpoint);

		// Returns true, if the specified endpoint (or any of the active
		// endpoints in case of the broadcast endpoint address) belongs to the
		// specified group, in which case the node is deemed a member of the
		// group. Can be used to do lookups in a virtual nwkGroupIDTable or
		// to implement the view groups command of the ZCL groups cluster
		virtual bool IsGroupMember(const unsigned short wGroupAddress,
			const unsigned char bEndpoint = 255) const;

		// Performs the startup procedure according to clause 2.5.5.5.6.2.
		// Returns false, if the startup procedure has been started before and
		// is currently in progress. The startup mode is form or join a network
		bool Startup(const unsigned char nMode = startupDefault);

		// Conducts a network discovery scan, and when complete, invokes the
		// application supplied handler to process the results
		bool RequestNetworkDiscovery(const unsigned int nScanChannels,
			const unsigned char nScanDuration,
			const ONNETWORKDISCOVERYCOMPLETE pfnComplete, void *pArgument = 0,
			const uint_fast8_t options = 0,
			CZigBeeNetworkScanManager *const pScanManager = 0);

		// Conducts an energy detection scan, and when complete, invokes the
		// application supplied handler to process the results
		bool RequestEnergyDetectionScan(const unsigned int nScanChannels,
			const unsigned char nScanDuration,
			const ONENERGYDETECTIONSCANCOMPLETE pfnComplete,
			void *pArgument = 0, const uint_fast8_t options = 0,
			CZigBeeNetworkScanManager *const pScanManager = 0);

		// Conducts a temporary channel change and invokes the callback after
		// having changed back to the operational channel. This is done using
		// an extension to the MLME's scanning capabilities. Notice that the
		// channel is specified as an ordinal (not as a mask) and that the
		// duration is in seconds (not as exponent of super-frame durations)
		bool RequestTemporaryChannelChange(const unsigned char nChannel,
			const unsigned char nDuration,
			void *const pfnFilter,
			const ONTEMPORARYCHANNELCHANGECOMPLETE pfnComplete,
			void *pArgument = 0);

		// Stores persistent data in non-volatile storage
		void Store(const unsigned char bStartupControl = 0,
			const bool bPartial = true);

		// Loads persistent data from non-volatile storage
		void Load();

		// Clears all AIB attributes and resets their default values
		void Reset(const bool bWarmStart);

		// Obtains security material suitable for APS encryption
		bool GetSecurityMaterial(CZigBeeApplicationSecurityMaterial &material,
			const CZigBeeAddress &destination,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions);

		// Obtains APS security material corresponding to an ephemeral network
		// key type (global vs. unique)
		bool GetEphemeralMaterial(CZigBeeApplicationSecurityMaterial &material,
			const bool outgoing);

		// Processes an incoming command frame	(APSME command dispatching)
		void ProcessCommand(CZigBeeAddress &destination, CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &command,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// Returns a key that is ready to be used for AES decryption based on
		// the function that the key performs (data key, key-transport key or
		// key-load key)
		bool DeriveKey(unsigned char abDerivedKey[16],
			const unsigned char abLinkKey[16],
			const unsigned char nKeyIdentifier) const;

		// APSME-TRANSPORT-KEY.request(DestAddress, KeyType, TransportKeyData),
		// clause 4.4.3.1. Specialized for trust center link key (key type = 4)
		void RequestTransportKey(const CZigBeeAddress &destination,
			const CZigBeeApplicationSecurityMaterial &material,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions,
			const unsigned char bRadius = 0);

		// APSME-TRANSPORT-KEY.request(DestAddress, KeyType, TransportKeyData),
		// clause 4.4.3.1. Specialized for network key (key type = 1)
		void RequestTransportKey(const CZigBeeAddress &destination,
			const CZigBeeNetworkSecurityMaterial &material,
			const CZigBeeAddress &parent, const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions,
			const unsigned char bRadius = 0);

		// APSME-TRANSPORT-KEY.request(DestAddress, KeyType, TransportKeyData),
		// clause 4.4.3.1. Specialized for application link key (key type = 3)
		void RequestTransportKey(const CZigBeeAddress &destination,
			const CZigBeeAddress &partner, const bool bInitiator,
			const CZigBeeApplicationSecurityMaterial &material,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions,
			const unsigned char bRadius = 0,
			const void *const pTLVs = 0, const uint_fast8_t cbTLVs = 0);

		// APSME-UPDATE-DEVICE.request(DestAddress, DeviceAddress, Status,
		// DeviceShortAddress), clause 4.4.4.1
		void RequestUpdateDevice(const CZigBeeAddress &destination,
			const CZigBeeAddress &device, const unsigned char nStatus,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions);

		// APSME-REQUEST-KEY.request(DestAddress, KeyType, PartnerAddress),
		// clause 4.4.5.1
		void RequestKey(const CZigBeeAddress &destination,
			const unsigned char nKeyType,
			const unsigned char bTransmitOptions = 0,
			const unsigned char bEnhancedOptions = 0,
			const unsigned char nRadius = 0);

		// APSME-REQUEST-KEY.request(DestAddress, KeyType, PartnerAddress),
		// clause 4.4.5.1
		void RequestKey(const CZigBeeAddress &destination,
			const CZigBeeAddress &partner,
			const unsigned char bTransmitOptions = 0,
			const unsigned char bEnhancedOptions = 0,
			const unsigned char nRadius = 0);

		// APSME-SWITCH-KEY.request(DestAddress, KeySeqNumber), clause 4.4.6.1
		void RequestSwitchKey(const CZigBeeAddress &destination,
			const unsigned char nKeySequenceNumber,
			const unsigned char bTransmitOptions = 0,
			const unsigned char bEnhancedOptions = 0,
			const unsigned char nRadius = 0);

		// Sends a tunnel command. No APSME primitive is defined for this yet
		void RequestTunnelData(const CZigBeeAddress &parent,
			const CZigBeeAddress &destination, const CPacket &tunnelled,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions);

		// Remove device
		void RequestRemoveDevice(const CZigBeeAddress &device,
			const CZigBeeAddress &parent, const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions, const unsigned char nRadius);

		// Performs a factory reset. If no full reset is specified the factory
		// reset makes sure the outgoing frame counter is not reset to zero,
		// which makes sure a device can revisit a secure network (R21). If
		// the device shall not reset/restart it is possible to notify all
		// application and cluster instances of the factory reset, if the
		// caller does not intend to perform a reset after FactoryReset() has
		// returned. Notice: This function must not be called to instigate
		// partial factory resets
		void FactoryReset(const unsigned int nOptions = factoryResetDefaults);

		// Refreshes the trust center link key, i.e. obtains a unique link key
		// through a key request.
		// This method should be called, after a broadcast delivery time, when
		// the device announcement is broadcast after initial joining.
		// In addition, this method could also be called during reboot after
		// an initial TCLK exchange procedure has been cancelled and remains
		// unfinished e.g. due to power cycle .
		void RefreshTrustCenterLinkKey();

		// On a centralized security network with a trust center that is at
		// least revision 23 or later, obtains the symmetric passphrase from
		// the trust center after having successfully completed key agreement
		void RequestAuthenticationToken();

		// Ensures the incoming security frame counter is referring to a fresh
		// instance of a remote device's outgoing security frame counter by
		// sending a ZDO security challenge request and processing its response
		void RequestSecurityChallenge
			(const CZigBeeApplicationSecurityMaterial &material);

		// Sets the global key and stores it persistently
		void SetGlobalKey(const uint8_t key[16]);

		// Must be called by the APS security engine when a provisionary, new
		// unique trust center link key has been successfully used to decrypt
		// an APS data message
		void OnKeyUpdateValidated(std::map<unsigned long long,
			CZigBeeApplicationSecurityMaterialUpdate *>::iterator &pendingKey);

		// Adds security material to the set of permanent security material
		void AddSecurityMaterial(CZigBeeApplicationSecurityMaterial &material);

		// Rolls the network key by generating a new key, broadcasting the key
		// to all devices on the network and finally broadcasting a key switch
		// to execute the key switch
		void RollNetworkKey();

		// Notifies the APSME of changes to the active network security
		// material, for example if the network frame counter is incremented
		void OnSecurityMaterialModified
			(const CZigBeeNetworkSecurityMaterial &material);

		// Notifies the APSME of changes to the active network security
		// material, in particular an incremented incoming frame counter
		void OnSecurityMaterialModified(const CZigBeeNeighborInfo &neighbor);

		// Notifies the APSME of a trust center swapout, providing new,
		// provisional trust center security material (the swapout key), and
		// the replacement trust center's EUI-64
		void OnTrustCenterSwapout
			(CZigBeeApplicationSecurityMaterial &material);

		// NLME-TEMPORARY-CHANNEL-CHANGE.confirm, a custom extension to the
		// NLME. Invoked when the device reverts to the operational channel
		void OnConfirmTemporaryChannelChange(const unsigned char nStatus);

		// NLME-PANID-CONFLICT.indication, a custom extension to the NLME to
		// let the APSME or ZDO handle the PAN-ID conflict in lieu of the NLME.
		// The descriptor and beacon are valid if the conflict has been
		// detected by the local device, in which case the source address holds
		// either the short or extended address of the device sending the
		// beacon; both are null, if the conflict has been detected by a remote
		// router and reported to this network manager instance, in which case
		// the source address holds the address of the device reporting the
		// conflict. Should the application decide to resolve the conflict,
		// it may call ResolvePANIDConflict() to execute the actual change of
		// PAN ID. Care must taken to not perform PAN-ID conflict resolution
		// automatically and unconditionally, as this would pose a severe
		// vulnerability to certain cyber attacks
		void OnPANIDConflict(const std::set<unsigned short> &panids,
			const CZigBeeAddress &source,
			const CIEEE802154PANDescriptor *const pDescriptor = 0,
			const CZigBeeBeacon *const pBeacon = 0);

		// Resolves a PAN-ID conflict by scanning for other networks in the
		// vicinity and choosing a new random 16-bit PAN identifier that does
		// not conflict with any of the known networks and the ones provided
		// by the caller (e.g. from a network report)
		void ResolvePANIDConflict(const std::set<unsigned short> &panids);

	// Overrides
	public:
		// Returns true, if this device has joined the network using a
		// pre-configured link-key
		virtual bool JoinedUsingPreconfiguredKey();

	// Implementation
	public:
		// Joining device timer. On the trust center, this keeps track of
		// devices currently in the joining set
		CCompactTimer *m_pJoiningDeviceCleanupTimer;

		// Key-transport command frame time-out timer. Starts after joining a
		// network from scratch and expires after apsSecurityTimeoutPeriod,
		// when the device does not receive a network key
		CCompactTimer *m_pKeyTransportTimer;

		// Request-key timer. After joining a centralized security network,
		// the joining device will request a new, unique link key with the
		// trust center for subsequent key updates and TC rejoins
		CCompactTimer *m_pKeyExchangeTimer;

		// Device startup retry timer, following defined strategy for initial
		// "join via association" or rejoin to grant fast retry and reasonable
		// backoff period
		CCompactTimer *m_pStartupRetryTimer;

		// Completion handler for network discovery (non-zero, if a request
		// is pending)
		ONNETWORKDISCOVERYCOMPLETE m_pfnNetworkDiscoveryComplete;

		// Completion handler for energy detection scan (non-zero, if a request
		// is pending)
		ONENERGYDETECTIONSCANCOMPLETE m_pfnEnergyDetectionScanComplete;

		// Completion handler for a temporary channel change (custom scan). It
		// may be non-zero only while on the other channel
		ONTEMPORARYCHANNELCHANGECOMPLETE m_pfnTemporaryChannelChangeComplete;

		// Called-supplied argument passed to network-discovery or
		// energy-detection scan completion handler
		void *m_pArgument;

		// Startup flags, a combination of startup mode and startup state
		unsigned int m_nStartupFlags;

		// Network discovery counter. A join attempt is made after this counter
		// has reached the desired number of network discovery cycles
		unsigned char m_nNetworkDiscoveryCycle;

		// Trust center link key exchange state
		uint8_t m_keyExchangeState;

		// Trust center link key exchange/negotiation state
		enum { maxKeyExchangeAttempts = 16, stateKeyExchangeIdle = 0,
			stateQueryNodeDescriptor0, stateQueryNodeDescriptor1,
			stateKeyExchange1 = stateQueryNodeDescriptor1 +
				maxKeyExchangeAttempts,
			stateKeyVerification1 = stateKeyExchange1 + maxKeyExchangeAttempts };

		// True if the network key is currently being rolled
		bool m_bRollingNetworkKey;

		// True if the network is currently being updated to use a different
		// PAN ID or channel
		bool m_bNetworkUpdateInProgress;

		// Counter of attempted startups, will be reset to 0 when startup
		// succeeds or the maximum attempts have been reached
		uint32_t m_startupAttempts;

		// Number of ticks (symbol periods) to wait prior to entering the
		// next fast retry cycle. The backoff duration should be doubled
		// after each finished fast retry cycle but remain confined to the
		// maximum backoff duration
		CCompactTimerService::ticks_diff_t m_startupBackoffTicks;

		// When this device is acting as the trust center, this is the set of
		// active joiners currently trying to establish a link-key or being
		// interviewed prior to being authorized onto the network, or declined
		// such authorization
		std::set<CZigBeeJoiningDevice> m_joiningDevices;

		// Maximum number of devices that can join concurrently, i.e. which
		// need to complete key agreement and potentially device interview
		// and will eventually be provided the network key
		static const unsigned char nJoiningDevicesLimit;

		// Startup mode for next rejoin attempt which could be instigated
		// e.g. upon receipt of a leave request with subsequent rejoin enabled
		// or detection of parent link failure etc.
		uint8_t m_rejoinStartupMode;

		// Core stack revision of the Trust Center, if known, or zero
		// otherwise
		uint8_t m_trustCenterRevision;

		// The maximum duration in ticks that a node will wait till APS
		// key material undergoing the trust center key exchange process
		// becomes validated
		static const CCompactTimerService::ticks_diff_t
			trustcenterKeyExchangeValidationTimeoutTicks;

		// The maximum number of attempts made during an ongoing trust
		// center key exchange process (must not be more than 10)
		static const uint8_t trustcenterKeyExchangeAttemptLimit;

		// Number of consecutive network discoveries after startup, before
		// joining a network
		static const unsigned char nNetworkDiscoveryCycles;

		// Scan duration for channel formation and network discovery.
		static const unsigned char nScanDuration;

		// General delay period in ticks to start a TCLK exchange procedure
		// including querying stack core revision.
		static const CCompactTimerService::ticks_diff_t
			nTrustCenterKeyExchangeStartDelay;

		// Maximum of jitter delay period in ticks, which is meant to be
		// added to the general start delay period of a TCLK exchange
		// procedure. This aims to alleviate potential traffic congestion
		// in case of concurrent network steering to a legacy TC.
		static const CCompactTimerService::ticks_diff_t
			nTrustCenterKeyExchangeStartJitter;

		// Maximum time duration in ticks for a node waiting for a response
		// when sending an APS RequestKey or VerifyKey to the trust center
		// while refreshing TC link key.
		static const CCompactTimerService::ticks_diff_t
			nTrustCenterKeyExchangeTimeout;

		// Delay before attempting another node descriptor query in the context
		// of obtaining a unique trust center link key; applies in case the
		// first one did not succeed
		static const CCompactTimerService::ticks_diff_t
			nodeDescriptorQueryRetryDelay;

		// Permit joining extension delay. Devices joining a network will
		// broadcast a permit join ZDO command to extend the joining window in
		// multi-hop networks by another commissioning duration (180 seconds)
		static const CCompactTimerService::ticks_diff_t
			nPermitJoinExtensionDelay;

		// Default maximum number of rejoin attempts that will be made,
		// in a row, during one fast rejoin cycle
		static const uint16_t rejoinDefaultFastRetryLimit;

		// Working maximum number of rejoin attempts that will be made,
		// in a row, during one fast rejoin cycle
		static uint16_t rejoinFastRetryLimit;

		// Default number of ticks (symbol periods) to wait prior to
		// attempting a retry within a fast rejoin cycle after a previously
		// failed rejoin
		static const CCompactTimerService::ticks_diff_t
			rejoinDefaultFastRetryDelayTicks;

		// Working number of ticks (symbol periods) to wait prior to
		// attempting a retry within a fast rejoin cycle after a previously
		// failed rejoin
		static CCompactTimerService::ticks_diff_t rejoinFastRetryDelayTicks;

		// Default number of ticks (symbol periods) to wait prior to
		// entering a new fast rejoin cycle and attempting consecutive
		// retries after all rejoin attempts during a previous cycle have
		// failed. Base value, backoff duration should be twice of the
		// previous one, up to a specified maximum value. A value of 0x00
		// means do not perform any fast rejoin backoff.
		static const CCompactTimerService::ticks_diff_t
			rejoinDefaultBaseBackoffTicks;

		// Working number of ticks (symbol periods) to wait prior to
		// entering a new fast rejoin cycle and attempting consecutive
		// retries after all rejoin attempts during a previous cycle have
		// failed. Base value, backoff duration should be twice of the
		// previous one, up to a specified maximum value. A value of 0x00
		// means do not perform any fast rejoin backoff.
		static CCompactTimerService::ticks_diff_t rejoinBaseBackoffTicks;

		// Default maximum number of ticks (symbol periods) between two
		// consecutive fast rejoin cycles.
		static const CCompactTimerService::ticks_diff_t
			rejoinDefaultMaxBackoffTicks;

		// Working maximum number of ticks (symbol periods) between two
		// consecutive fast rejoin cycles.
		static CCompactTimerService::ticks_diff_t rejoinMaxBackoffTicks;

		// Default maximum number of iterations for a device to perform
		// fast rejoin backoff before the backoff duration is reset to
		// the base backoff duration. A value of 0x00 means do not reset
		// the backoff duration.
		static const uint16_t rejoinDefaultBackoffLimit;

		// Working maximum number of iterations for a device to perform
		// fast rejoin backoff before the backoff duration is reset to
		// the base backoff duration. A value of 0x00 means do not reset
		// the backoff duration.
		static uint16_t rejoinBackoffLimit;

		// Maximum number of "join via association" attempts that will be
		// made, in a row, during one fast join cycle
		static const uint16_t joinFastRetryLimit;

		// Number of ticks (symbol periods) to wait prior to attempting
		// a retry within a fast join cycle after a previously failed
		// "join via association" attempt
		static const CCompactTimerService::ticks_diff_t
			joinFastRetryDelayTicks;

		// Number of ticks (symbol periods) to wait prior to entering
		// a new fast join cycle and attempting consecutive retries after
		// all join attempts during a previous cycle have failed.
		// Base value, backoff duration should be twice of the previous
		// one, up to a specified maximum value. A value of 0x00 means
		// do not perform any fast join backoff.
		static const CCompactTimerService::ticks_diff_t joinBaseBackoffTicks;

		// Maximum number of ticks (symbol periods) between two consecutive
		// fast join cycles.
		static const CCompactTimerService::ticks_diff_t joinMaxBackoffTicks;

		// Maximum number of iterations for a device to perform fast join
		// backoff before the backoff duration is reset to the base backoff
		// duration. A value of 0x00 means do not reset the backoff duration.
		static const uint16_t joinBackoffLimit;

		// Indicates whether the device announcement during initial joining
		// via association shall be deferred until being fully authenticated,
		// i.e., after a unique link key has been established with the trust
		// center, an authentication token has been obtained, etc.
		static const bool deferDeviceAnnouncement;

		// The transmit options to use for an APS Update Device request
		// command, defaults to requiring acknowledgement and APS security,
		// unless re-defined.
		static const uint8_t requestUpdateDeviceTransmitOptions;

		// Indicate whether allow instigating TC rejoin attempt on
		// a distributed network
		static const bool instigateTCRejoinOnDistributedNetwork;

		// Static ASDU size limit for a single fragment during transmission.
		// If unspecified (0xff by default), the real fragment size will be
		// dynamically determined based on payload limit and additional
		// overheads.
		static const uint8_t singleFragmentLimit;

		// Default factory reset options
		static const uint32_t factoryResetDefaults;

		// A cookie passed to the network discovery completion handler for
		// network updates in the course of PANID conflict resolution
		typedef std::pair<std::set<unsigned short> *,
			CZigBeeApplicationSupportManagement *>
			CZigBeeNetworkUpdateCookie;

		// Flags for the UpdateDeviceInfo method
		enum { deviceInfoStore = 0x80, deviceInfoCreate = 0x40,
			deviceInfoForceUpdate = 0x20, deviceInfoCapabilitiesMask = 0x0f };

		// Potential key update failure reasons
		enum { descriptorTimeout, keyUpdateNotSupported, requestKeyTimeout,
			verifyKeyTimeout, confirmKeyFailure };

		// Fires when the device does not receive a key transport message
		// within apsSecurityTimeoutPeriod after an NLME-JOIN.indication
		// This timer only fires on a joining device
		void OnKeyTransportTimeoutExpired(void *pArgument = 0);

		// Fires when the device should make an attempt to request a new key,
		// which might be either an initial attempt or a subsequent attempt
		// This timer only fires on a joining device
		void OnKeyRequestTimeoutExpired(void *pArgument);

		// Fires when the device should make an attempt to verify a new key,
		// which might be either an initial attempt or a subsequent attempt
		// This timer only fires on a joining device
		void OnKeyVerificationTimeoutExpired(void *pArgument);

		// Key validation timer. When a new trust center link key has been
		// requested, this timer is started. Before it fires, a data frame
		// encrypted with the new trust center link key must have been received
		// This timer only fires on the trust center
		void OnKeyValidationTimeoutExpired(void *pArgument);

		// APS command handler for transport key command. This defaults to the
		// dispatch to specialized handlers, but the default implementation may
		// be overruled by product implementations to perform additional or
		// alternate key transport handling
		void OnTransportKey(CZigBeeAddress &destination,
			CZigBeeAddress &source, const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// Default handler for transport key commands. Dispatches to one of
		// the specialized handlers based on key type (network key, application
		// link key, or trust center link key).
		void DefaultOnTransportKey(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const CZigBeeAHR &header,
			const CPacket &packet, const unsigned char linkQuality,
			const unsigned int receiveTime, const bool securityUsed);

		// APS command handler for transport network key command
		void OnTransportKey1(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const CZigBeeAHR &header,
			const CPacket &packet, const unsigned char linkQuality,
			const unsigned int receiveTime, const bool securityUsed,
			const CZigBeeApplicationSecurityMaterial *const materialTC = 0);

		// APS command handler for transport application link key command
		void OnTransportKey3(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const CZigBeeAHR &header,
			const CPacket &packet, const unsigned char linkQuality,
			const unsigned int receiveTime, const bool securityUsed);

		// APS command handler for transport trust center link key command
		void OnTransportKey4(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const CZigBeeAHR &header,
			const CPacket &packet, const unsigned char linkQuality,
			const unsigned int receiveTime, const bool securityUsed);

		// APS command handler for update device (trust center only)
		void OnUpdateDevice(CZigBeeAddress &destination,
			CZigBeeAddress &source, const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// APS specialized handler for update-device: secured rejoin
		void OnDeviceRejoined(const CZigBeeAddress &device,
			const CZigBeeAddress &parent);

		// Notification provided to the application when a device securely
		// rejoined the network. This can be overloaded by providing the
		// symbol in the application if required (weak default in Compact7B
		// library)
		void OnNotifyDeviceRejoined(const CZigBeeAddress &device,
			const CZigBeeAddress &parent);

		// APS specialized handler for update-device: unsecured (re-)join
		void OnRequestDeviceJoin(const CZigBeeAddress &device,
			const CZigBeeAddress &parent, const bool bRejoin,
			const void *const pTLVs, const uint_fast8_t cbTLVs);

		// APS specialized handler for update-device: device left or NLME
		// leave indication (in which case source is null)
		void OnDeviceLeft(const CZigBeeAddress &device,
			const CZigBeeAddress &source);

		// Notification provided to the application when a device left the
		// network. This can be overloaded by providing the symbol in the
		// application if required (weak default in Compact7B library)
		void OnNotifyDeviceLeft(const CZigBeeAddress &device,
			const CZigBeeAddress &source);

		// APS command handler for remove device (trust center only)
		void OnRemoveDevice(CZigBeeAddress &destination,
			CZigBeeAddress &source, const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// APS command handler for request key (on the trust center only)
		void OnRequestKey(CZigBeeAddress &destination, CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// APS command handler for request key (application link key)
		void OnRequestKeyApplicationLinkKey(const CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &packet);

		// APS command handler for request key (trust center link key)
		void OnRequestKeyTrustCenterLinkKey(const CZigBeeAddress &source);

		// APS command handler for switch key
		void OnSwitchKey(CZigBeeAddress &destination, CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// Handles APSME-DATA.confirm for a switch key request
		void OnConfirmDataSwitchKey(const CZigBeeAddress &destination,
			const unsigned char bStatus, const unsigned int nTimestamp,
			void *const pCookie);

		// APS command handler for tunnel data
		void OnTunnelData(CZigBeeAddress &destination, CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// APS command handler for verify key
		void OnVerifyKey(CZigBeeAddress &destination, CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// APS command handler for confirm key
		void OnConfirmKey(CZigBeeAddress &destination, CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// APS command handler for relay downstream
		void OnRelayDownstream(CZigBeeAddress &destination,
			CZigBeeAddress &source, const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// APS command handler for relay upstream
		void OnRelayUpstream(CZigBeeAddress &destination,
			CZigBeeAddress &source, const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// Performs a key-switch. Either called immediately when a unicast
		// key-switch is received or by a timer, after the broadcast has been
		// delivered to the whole network
		void OnKeySwitchDelayElapsed(void *pArgument);

		// This potentially asynchronous function is called to authenticate a
		// device. The default implementation invokes QueryAuthenticateDevice()
		// and then CompleteAuthentication() with the results of the query.
		// Advanced applications might want to overload and perform additional
		// queries, e.g. involve an application running on a different core.
		// The implementation of this method might consider the key negotiation
		// capabilities of the prospective joining device by looking up the
		// device in the set of joining devices and inspecting the joiner TLVs
		void AuthenticateDevice(const CZigBeeAddress &device,
			const CZigBeeAddress &parent, const bool rejoin);

		// This function is called to reject a joining device. The default
		// implementation invokes RequestRemoveDevice(). Applications might
		// want to overload and either not send a remove device command or do
		// additional logic
		void RejectDevice(const CZigBeeAddress &device,
			const CZigBeeAddress &parent, const bool rejoin);

		// Completes authentication of a device by either sending it the
		// network key in a transport key message (direct or tunneled), or
		// rejecting it by timeout or a remove device to its transitional
		// parent router. This should not be called directly. Rather, it is
		// part of the commissioning flow and expected to be invoked by
		// QueryAuthenticate() at some point
		void CompleteAuthentication(const CZigBeeAddress &device,
			const CZigBeeAddress &parent, const bool rejoin,
			const bool accept);

		// This function is called when a device attempts to join and a
		// decision must be made as to whether or not it should be permitted on
		// the network. Can be used to implement black listing, white listing,
		// or query the host processor (or a separate data storage) for a
		// pre-configured link key (derived from install code) for the given
		// device. The default implementation erases any existing key upon
		// initial join and returns true.
		// The implementation of this method might consider the key negotiation
		// capabilities of the prospective joining device by looking up the
		// device in the set of joining devices and inspecting the joiner TLVs.
		// Notice that it is also possible to overload AuthenticateDevice() for
		// more control over the flow, e.g. in order to implement an
		// asynchronous (non-blocking) flow.
		bool QueryAuthenticateDevice(const CZigBeeAddress &device,
			const bool bRejoin);

		// Begins a trust center key exchange either on the trust center or on
		// the joiner side. The key is stored temporarily in the set of link
		// keys pending validation and will be automatically removed when not
		// validated
		CZigBeeApplicationSecurityMaterialUpdate *
		BeginKeyUpdate(const unsigned char nKeyType,
			const unsigned long long qwDeviceAddress);

		// This method is called when the trust center link-key exchange
		// sequence update failed at some point on a joining device. May be
		// optionally overloaded by applications if required
		void AfterKeyUpdateFailed(const uint_fast8_t reason);

		// Default implementation of AfterKeyUpdateFailed(), which should be
		// called by application-specific overloads of AfterKeyUpdateFailed()
		// in addition to any custom logic provided by the application
		void DefaultAfterKeyUpdateFailed(const uint_fast8_t reason);

		// Prepares anonymous or authenticated key agreement using the
		// specified scheme and passcode. This is similar to beginning key
		// agreement, with the difference that no key is computed yet; rather
		// a pending key update is registered
		CZigBeeApplicationSecurityMaterialUpdate *
		PrepareKeyAgreement(const uint_fast8_t nKeyType,
			const uint_fast64_t qwDeviceAddress,
			const uint_fast8_t nScheme, const uint_fast8_t nSecret);

		// One a key agreement has been prepared, this progresses key agreement
		// to the next stage, where a public point is computed and appended to
		// the supplied packet using the specified tag identifier. Returns a
		// failure status if no key agreement is in progress for the particular
		// device, i.e. PrepareKeyAgreement() had not been called before, or
		// the provisional entry in the apsDeviceKeyPairSet has expired
		uint_fast8_t ProgressKeyAgreement(CPacket &packet,
			const uint_fast64_t qwDeviceAddress,
			const uint_fast8_t tagPublicPoint);

		// Begins anonymous or authenticated key agreement using the specified
		// scheme and passcode
		CZigBeeApplicationSecurityMaterialUpdate *
		BeginKeyAgreement(CPacket &packet, const uint_fast8_t nKeyType,
			const uint_fast64_t qwDeviceAddress, const uint_fast8_t nScheme,
			const uint_fast8_t nSecret, const uint_fast8_t tagPublicPoint);

		// Sets-up a key update. The key is stored temporarily in the set of
		// link keys pending validation and will be automatically removed when
		// not validated
		void SetupSecurityMaterialUpdate
			(CZigBeeApplicationSecurityMaterialUpdate *pMaterial,
			const uint_fast8_t nKeyType, const uint_fast64_t qwDeviceAddress);

		// Continue anonymous or authenticated key agreement upon availability
		// of the other party's identity and public point
		CZigBeeApplicationSecurityMaterialUpdate *
		ContinueKeyAgreement(const uint_fast64_t qwDeviceAddress,
			const uint8_t publicPoint[32]);

		// Compares the hash value (HMAC) against the key in the material
		static bool VerifyKeyHash(const CZigBeeSecurityMaterial &material,
			const unsigned char abExpectedHash[16]);

		// Transmits a verify key request from a joining device to the trust
		// center in order to complete the unique trust center link-key
		// exchange sequence
		void VerifyKey(const CZigBeeAddress &destination,
			const unsigned char abHash[16],
			const unsigned char bTransmitOptions = 0,
			const unsigned char bEnhancedOptions =
				CZigBeeApplicationSupportBase::priorityImmediate,
			const unsigned char nRadius = 0);

		// Transmits a confirm key response frame from the trust center back to
		// a joining device in order to complete the unique trust center
		// link-key exchange sequence
		void ConfirmKey(const CZigBeeAddress &destination,
			const unsigned long long qwAddress,
			const unsigned char nStatus = 0,
			const unsigned char bTransmitOptions = 0,
			const unsigned char bEnhancedOptions =
				CZigBeeApplicationSupportBase::priorityImmediate,
			const unsigned char nRadius = 0);

		// Starts a verify key and confirm key exchange in order to finalize a
		// pending security material update
		void StartKeyVerification
			(CZigBeeApplicationSecurityMaterialUpdate *const pMaterial);

		// Returns either the primary or secondary channel mask, based on which
		// round is currently active
		unsigned int GetStartupChannelMask();

		// Advances to the next allowed startup strategy (modifies the startup
		// flags) and returns false, if there is no more strategy to attempt
		bool AdvanceStartupStrategy();

		// Continues the start-up sequence for the given EPID. If the advance
		// parameter is set to false, then ContinueStartup() is called in a
		// second pass and is actually conveyed a PANID to act on (join). In
		// this case it should not advance to the next strategy or scan for
		// networks
		bool ContinueStartup(const unsigned long long qwExtendedPANID,
			const bool bAdvance = true);

		// Called when the device has successfully joined the network. Provides
		// an product-level override to influence the post-joining behavior. By
		// default DefaultAfterNetworkJoined() will be called to broadcast the
		// device announcement and request an updated trust center link-key.
		// Applications may perform special behavior here in addition, before
		// or after the default steps are executed. It is highly recommended to
		// call DefaultAfterNetworkJoined() in products that override/extend
		// the default behavior
		void AfterNetworkJoined();

		// Called when the device has successfully joined the network, before
		// a unique trust center link key is requested. This method invokes ZDO
		// glue logic to trigger broadcast of device announcement and permit
		// joining extension and query of trust center link-keys
		void DefaultAfterNetworkJoined();

		// Called after joining, when the device announcement has settled, i.e.
		// the broadcast delivery time has expired
		void OnJoiningDelayElapsed(void *pArgument);

		// Called during joining, when the device should start the trust-center
		// link-key exchange
		void OnQueryDelayElapsed(void *pArgument);

		// Called in the course of joining, when querying the trust center's
		// node descriptor has completed
		static void OnQueryComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Called in the course of joining, when querying the trust center's
		// node descriptor has completed
		static void OnSecurityKeyAgreementComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Either restarts using the secondary channel set, or terminates the
		// startup process
		void StartupComplete(const unsigned char nStatus);

		// Called in the course of device startup, when the local device is
		// performing the trust center role on the network
		void OnTrustCenterStartupSucceeded();

		// Successfully completes the authentication stage. This could either
		// mean the device joined a legacy trust center and does not have a
		// unique trust center link-key, or the device has successfully
		// obtained a trust center link key. In R23+ centralized networks this
		// further includes requesting an authentication token, if it has not
		// been requested before
		void AuthenticationComplete();

		// Called when network discovery has completed after a PAN-ID conflict
		// was reported
		static void OnNetworkUpdateDiscoveryComplete
			(const unsigned char nStatus,
			const std::list<CZigBeeNetworkDescriptor> &networks,
			void *pArgument);

		// Chooses a non-conflicting PAN-ID and lets the NLME broadcast a
		// network update command. Also starts the update delay timer, which
		// actually updates the local device's PAN-ID after the update command
		// has propagated through the network
		static void InstigateNetworkUpdate(CZigBeeNetworkUpdateCookie *pCookie);

		// Verifies if a command that has just been received and is being
		// examined prior to execution has been sent encrypted with the proper
		// key (e.g. a trust center link key). Notice that when the device is
		// operating on a distributed security network, VerifySecurity() will
		// always return true, since such networks normally neither use nor
		// support APS security. If support for peer APS link keys in such
		// distributed security networks arises, an extended version of this
		// function should be added to support such use cases
		bool VerifySecurity(const CZigBeeAHR &header,
			const unsigned char requiredKeyType =
				CZigBeeApplicationSecurityMaterial::typeTrustCenter) const;

		// Verifies if a command that has just been received and is being
		// examined prior to execution has been sent encrypted with the proper
		// key (e.g. a trust center link key). Notice that when the device is
		// operating on a distributed security network, VerifySecurity() will
		// always return true, since such networks normally neither use nor
		// support APS security. If support for peer APS link keys in such
		// distributed security networks arises, an extended version of this
		// function, i.e. VerifySecurityEx(), should be added to support such
		// use cases
		bool VerifySecurityEx(const CZigBeeAHR &header,
			const unsigned char requiredKeyType,
			const CZigBeeApplicationSecurityMaterial &material) const;

		// Continues the authentication procedure. For joining devices that do
		// not support key agreement, this results in the final step being
		// executed. For devices that do support key agreement, this will kick
		// of key agreement, potentially followed by device interview
		void ContinueAuthentication(const CZigBeeAddress &device,
			const CZigBeeAddress &parent, const bool bRejoin);

		// This instigates an optional device interview, which allows the Trust
		// Center application to query a device over a secure relay tunnel,
		// prior to providing it the active network key. In this stage, the
		// joining device will be in an intermediary state. At the end of the
		// interview stage, when the Trust Center application decides to accept
		// the prospective joiner, it must call FinishAuthentication(); else,
		// it may either do nothing, or call CompleteAuthentication() to remove
		// the device from the network. The default implementation accepts all
		// by skipping over the interview stage altogether
		void InterviewDevice(const CZigBeeAddress &device);

		// Performs the last step of conveying the active network key to a new
		// joining (or rejoining) device
		void FinishAuthentication(const CZigBeeAddress &device,
			const CZigBeeAddress &parent, const bool bRejoin);

		// Obtains the joiner TLVs for the specified device. This is only for
		// the case where a joiner joins at the local device and the joiner
		// TLVs are stored in the extended portion of the neighbor table for
		// the joining device
		void GetJoiningDeviceTLVs(CPacket &tlvs, const CZigBeeAddress &device,
			const bool bAppend = true);

		// Adds a new device to set of joining devices
		std::set<CZigBeeJoiningDevice>::iterator
		AddJoiningDevice(const CZigBeeAddress &address,
			const CZigBeeAddress &relay = CZigBeeAddress(),
			const void *const pTLVs = 0, const uint_fast8_t cbTLVs = 0);

		// Schedules (or reschedules) a timer for when the next joining device
		// is about to expire. Will cancel any pending timer and determine the
		// timeout as appropriate
		void ScheduleJoiningDeviceCleanupTimer
			(const std::set<CZigBeeJoiningDevice>::iterator *const device = 0);

		// Refresh joining device lifetimes
		void RefreshJoiningDeviceLifetimes(const CCompactTimer &timer,
			const std::set<CZigBeeJoiningDevice>::iterator *const skip = 0);

		// Timer callback. Invoked when a joining device entry expires
		void OnJoiningDeviceCleanupDue(void *pArgument);

		// Given a set of TLVs from an inbound request frame, updates the
		// fragmentation cache accordingly, if the fragmentation parameters
		// global TLV is included in that set. Also sets the support for
		// frame counter synchronization as appropriate
		bool UpdateDeviceFeatureInfo(const CZigBeeTLVs &tlvs,
			const uint_fast8_t flags = deviceInfoStore,
			const uint_fast64_t extendedAddress =
			CZigBeeAddress::qwExtendedVoid,
			const uint_fast8_t coreStackRevision = 0);

		// Returns true if an ASDU of the specified size should be fragmented
		// into multiple smaller fragments. Takes into account whether the
		// ASDU exceeds the size of a single fragment, and also looks at the
		// peer's fragmentation capabilities, if available. For R23+ devices,
		// fragmentation support for certain ZDO commands is mandatory, and
		// the profile ID will be used to determine that support. Conversely,
		// the fact that the core stack is R23+ does not necessarily imply that
		// also the ZCL application endpoints support fragmentation
		uint_fast8_t ShouldFragment(const CZigBeeAddress &destination,
			const CPacketData::size_type asduSize,
			const uint_fast8_t transmitOptions, const uint16_t profileID,
			CPacketData::size_type &fragmentSize, uint8_t &fragmentCount);

		// Returns a remote device's defragmentation buffer capacity
		uint_fast8_t GetRemoteASDUSizeLimit(const CZigBeeAddress &destination,
			uint16_t &asduSizeLimit);

		// Default implementation of GetRemoteASDUSizeLimit(), which performs
		// a look-up in the local device information cache. This is suitable
		// for cases where both the local device and the remote device are
		// based on R23+ stacks. The extended address is required to be valid
		// for this lookup in
		uint_fast8_t DefaultGetRemoteASDUSizeLimit
			(const CZigBeeAddress &destination, uint16_t &asduSizeLimit);

		// Given a device address, ASDU size limit and options, updates the
		// device information cache. This supports three main use cases. One is
		// to store a remote device's EUI-64 to network short address mapping;
		// another is to store fragmentation capabilities of a remote device;
		// the third one is to flag support for frame counter synchronization.
		// If both the ASDU size limit and options are null, then only the
		// device short address will be updated and existing fragmentation
		// information is retained. Notice that currently just the 4 LSBs of
		// the option flags will be stored, and the ASDU size limit will be
		// capped at 8KB
		bool UpdateDeviceInformation(const CZigBeeAddress &device,
			const uint_fast16_t asduSizeLimit = 0,
			const uint_fast8_t options = 0,
			const uint_fast8_t flags = deviceInfoStore);

		// Transfers an APDU over the upstream or downstream pipe between a
		// joining device and the trust center. On the joining device, this
		// results in an upstream relay command being sent to the parent of
		// the prospective joiner. On a trust center, this results in a
		// downstream relay command to the joiner. This is true, both, for
		// single hop direct joins at the trust center, but also for multiple
		// hop joining via an intermediary router. Destination is the address
		// of the destination, which is all-F's for upstream messages, and a
		// valid EUI-64 in case of downstream messages
		void RequestRelayData(const CZigBeeAddress &address,
			const CZigBeeAddress &source, const CPacket &apdu);

		// Handles incoming upstream and downstream relay commands, both,
		// first/last hop, as well as multi-hop forwarding to/from the Trust
		// Center
		void OnIndicateRelayData(CZigBeeAddress &destination,
			CZigBeeAddress &source, const CZigBeeAHR &header, CPacket &packet,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed, const bool bUpstream);

		// Given a set of TLVs from an inbound request frame, decides which
		// key negotiation scheme to select, if any. In case a scheme is
		// selected, an appropriate TLV or set of TLVs will be appended to
		// the response packet ASDU
		uint_fast8_t SelectKeyAgreementScheme(const CZigBeeTLVs &tlvs,
			CPacket &asdu, const uint_fast8_t tag);

		// Invoked in the course of obtaining the network's symmetric
		// passphrase from the trust center after initial joining
		static void OnTokenRequestComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Invoked in the course of obtaining a remote devices APS security
		// outgoing frame counter
		static void OnSecurityChallengeComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Schedules a timer for when the next startup attempt should occur,
		// following the defined strategy for initial join or rejoin to
		// grant sufficient fast retries and appropriate backoffs
		void ScheduleNextStartupTimer(const uint_fast8_t state,
			const uint_fast8_t status =
				CZigBeeApplicationSupportBase::statusSuccess,
			const uint_fast16_t flags = 0);

		// Timer callback, invoked when startup should be performed
		void OnScheduledStartupDue(void *pArgument);

		// Delivers tunnel data. This is used to send a tunneled APS command
		// over the final hop to a joining device. Normally, defaults to
		// DefaultDeliverTunnelData(), but can be overloaded by applications or
		// other stack modules if need be
		void DeliverTunnelData(const CZigBeeAddress &address, CPacket &apdu);

		// Default implementation for deliver tunnel data. This is used to send
		// a tunneled APS command over the final hop to a joining device
		void DefaultDeliverTunnelData(const CZigBeeAddress &address,
			CPacket &apdu);

		// Returns true if a remote device, from the trust center's point of
		// view, has been fully authenticated on the network, i.e. this device
		// has successfully negotiated or exchanged a unique link-key with the
		// trust center and received a network key from the trust center.
		bool IsAuthenticated(const uint_fast64_t device);

		// Finish an incomplete factory reset procedure that has been
		// interrupted, e.g. by a power outage
		bool FinishFactoryReset();

#ifdef _DEBUG
		// Dumps the binding table
		void DumpBindingTable() const;

		// Dumps startup flags
		void DumpStartupFlags() const;

		// Dumps the set of joining devices
		void DumpJoiningDevices() const;

		// Dumps APSME information (including AIB)
		void Dump() const;
#endif

};


inline bool CZigBeeApplicationSupportManagement::IsStartupInProgress() const
	{ return m_nStartupFlags & startupInProgress ? true : false; }

inline bool CZigBeeApplicationSupportManagement::HasStartedOnce() const
	{ return m_nStartupFlags & startupInvokedOnce ? true : false; }

inline bool CZigBeeApplicationSupportManagement::IsKeyTransportPending() const
	{ return static_cast<bool>(m_pKeyTransportTimer); }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationMapEntry

class CZigBeeApplicationMapEntry
{
	// Attributes
	public:
		// Service application
		CZigBeeApplication *m_pApplication;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSupport

class CZigBeeApplicationSupport : public CZigBeeApplicationSupportBase,
	public CZigBeeNetworkClient, public CCompactTimerClient
{
	// typedefs & enums
	public:
		// Called when address lookup completed either successfully or without
		// success. The address contains either the extended address (when
		// network address lookup failed) or both addresses in case of success
		typedef void (*ADDRESSLOOKUPCALLBACK)(const CZigBeeAddress &address,
			void *pArgument, const unsigned char nStatus);

	// Construction
	public:
		CZigBeeApplicationSupport(CZigBeeNetwork &nwk);

	// Attributes
	public:
		// Link to network layer instance
		CZigBeeNetwork &m_nwk;

		// Application support management entity
		CZigBeeApplicationSupportManagement m_apsme;

	// Operations
	public:
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
			const unsigned char nSequence = 0);

		// This is a version of APSDE-DATA.request, which allows deferred data
		// requests, i.e. the APSDE-DATA.request is fired with the supplied
		// parameters after a pre-defined delay
		virtual void OnRequestDataDeferred(const CZigBeeAddress &destination,
			const unsigned short wProfileID, const unsigned short wClusterID,
			const unsigned char bSourceEndpoint, CPacket &asdu,
			const unsigned char bTransmitOptions, const unsigned char bRadius,
			void *const pCookie, const unsigned char nEnhancedOptions,
			const CZigBeeAddress &source, const unsigned char nSequence,
			const unsigned int nDelay);

		// INTRP-DATA.request(SrcAddrMode = 3, DstAddrMode, DstPANID,
		// DstAddress, ProfileID, ClusterID, ASDULength, ASDU, ASDUHandle)
		void OnRequestDataStub(const CZigBeeAddress &destination,
			const unsigned short wProfileID, const unsigned short wClusterID,
			CPacket &asdu, void *pArgument = 0);

		// Issues INTRP-DATA.confirm
		void OnConfirmDataStub(const unsigned char nStatus,
			const unsigned int nTimestamp, void *pArgument);

		// Issues INTRP-DATA.indication
		void OnIndicateDataStub(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Issues GP-DATA.indication
		void OnIndicateDataGP(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, CPacket &mpdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp);

		// Adds an application to the dynamic application map. Can be used to
		// implement the ConfigureEndpoint function in a gateway device, for
		// instance. If an application exists on the specified endpoint it is
		// replaced by the new application. It is first removed and - in case
		// of a heap application - also deleted. If another application exists
		// in the static application map, the function fails
		virtual bool AddApplication(CZigBeeApplication &application);

		// Removes an application from the dynamic application map. Notice,
		// that the application object is also automatically release via delete
		// in case of a heap application. Removing an application while the
		// network stack is operational is discouraged
		virtual bool RemoveApplication(CZigBeeApplication &application,
			const bool bDelete = true);

		// Returns a set of active application endpoints (except EP #0, ZDO)
		virtual void GetActiveEndpoints
			(std::set<unsigned char> &endpoints) const;

		// Returns the application object that services the specified endpoint,
		// or null if no such application exists
		virtual CZigBeeApplication *GetApplication
			(const unsigned char bEndpoint) const;

		// Notifies all applications that a device has just joined or rejoined
		// the network
		virtual void NotifyDeviceAnnouncement(const CZigBeeAddress &address,
			const unsigned char bCapabilities);

		// Notifies all applications that the device has started up
		virtual void NotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// Notifies all applications of the ongoing start-up progress status
		virtual void NotifyStartupProgress(const uint_fast8_t state,
			const uint_fast16_t flags, const uint_fast8_t status = statusSuccess);

		// Notifies all applications that the device has left the network
		virtual void NotifyNetworkLeave(const unsigned char nStatus,
			const bool bRejoin);

		// Called once after power-up in main() to initialize applications
		virtual void AfterPowerup();

		// Called periodically from a loop in main() to process idle tasks.
		// Returns true if there are more idle tasks pending, false otherwise.
		// Systems that implement an event-loop and enter sleep mode should use
		// this information and not enter sleep as long as there are idle tasks
		// pending
		virtual bool OnIdle() const;

		// Notifies all applications of a network status indication
		virtual void NotifyNetworkStatus(const unsigned char nStatus,
			const CZigBeeAddress &address);

		// Network address lookup service (avoids multiple concurrent lookups
		// for the same address), returns TRUE only if the network address
		// can be directly augmented without explicit ZDO query.
		bool LookupNetworkAddress(CZigBeeAddress &address,
			const ADDRESSLOOKUPCALLBACK pfnComplete, void *pArgument = 0,
			CZigBeeDeviceObjectRequest **const reference = 0);

		// Abort an ongoing network address lookup by dropping the maintained
		// completion handler
		void CancelNetworkAddressLookup(const CZigBeeAddress &address,
			const ADDRESSLOOKUPCALLBACK pfnComplete, void *pArgument = 0);

		// Verifies if a frame that has just been received and is being
		// examined prior to execution has been sent encrypted with the proper
		// key (e.g. a trust center link key). Calls VerifySecurityEx() with
		// the security material for the incoming frame
		bool VerifySecurity(const bool securityUsed,
			const unsigned char requiredKeyType =
				CZigBeeApplicationSecurityMaterial::typeTrustCenter) const;

		// Verifies if a frame that has just been received and is being
		// examined prior to execution has been sent encrypted with the proper
		// key (e.g. a trust center link key)
		bool VerifySecurityEx(const bool securityUsed,
			const unsigned char requiredKeyType,
			const CZigBeeApplicationSecurityMaterial &material) const;

	// Overrides (CZigBeeNetworkClient)
	public:
		// NLDE-DATA.confirm(Status, NsduHandle, TxTime), clause 3.2.1.2
		virtual void OnConfirmData(const unsigned char nStatus,
			const unsigned char nHandle, const unsigned int nTimestamp);

		// NLDE-DATA.indication(DstAddrMode, DstAddr, SrcAddr, NsduLength,
		// Nsdu, LinkQuality, RxTime, SecurityUse), clause 3.2.1.3
		virtual void OnIndicateData(CZigBeeAddress &destinationAddress,
			CZigBeeAddress &sourceAddress, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

	// Overrides (CZigBeeApplicationSupportBase)
	public:
		// Returns an address object with network address and 64-bit extended
		// IEEE address
		virtual CZigBeeAddress GetAddress() const;

		// Obtains the current active network security material. Returns false,
		// if no such material can be found
		virtual bool GetActiveNetworkSecurityMaterial
			(CZigBeeNetworkSecurityMaterial *&pMaterial) const;

		// Invalidates a 16-bit network address to 64-bit IEEE address mapping
		// for every occurrence of this mapping
		virtual void InvalidateAddressMapping(const CIEEE802154Address &address);

		// Given an address object with either a short or extended address,
		// this method completes the address object with the missing type of
		// address, if it is currently known. It does not initiate any address
		// lookups, however, which is left to higher layers, if required.
		// Returns true, if the address object is complete
		virtual bool AugmentAddress(CIEEE802154Address &address);

		// Performs all steps required to add a binding or remove a binding.
		// Can be called by CZigBeeApplication::ProcessBindOrUnbind()
		// implementations to perform the actual work after having performed
		// any additional checks, for example. The return value is a ZDO status
		// code and can be placed unmodified in a ZDO response frame
		virtual unsigned char DefaultBindOrUnbindProcessing
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const bool bTrustCenterOnly = false);

		// This method may be called only from within OnIndicateData() handlers
		// for APS frames and returns the APS security material used to decrypt
		// the incoming APS frame. If no APS security was applied, the pointer
		// is null
		virtual const CZigBeeApplicationSecurityMaterial *
			GetIncomingSecurityMaterial() const;

		// Enter responsive mode until LeaveResponsiveMode() is invoked with
		// the cookie provided by EnterResponsiveMode()
		virtual bool EnterResponsiveMode(unsigned int &nCookie);

		// Leave responsive mode for the context provided by the cookie
		virtual void LeaveResponsiveMode(const unsigned int nCookie);

		// Enter responsive mode, and automatically leave after the specified
		// time, or pre-emptively by calling LeaveResponsiveModeEx() for the
		// cancellable timer provided by EnterResponsiveModeEx()
		virtual bool EnterResponsiveModeEx(const unsigned int nDuration =
			CZigBeeApplicationInformationBase::nDefaultResponsiveModeDuration,
			CCompactTimer **const ppTimer = 0);

		// Leave responsive mode pre-emptively
		virtual bool LeaveResponsiveModeEx(CCompactTimer *&pTimer);

		// True if the device is currently in responsive (fast polling) mode
		virtual bool IsResponsive();

	// Overridables
	public:
		// Issues one or more APSDE-DATA.indications to their intended
		// destination applications within the local next higher layer entity
		virtual void IndicateData(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wProfileID,
			const unsigned short wClusterID, const CPacket &asdu,
			const unsigned char bStatus, const bool bSecurityUsed,
			const unsigned char nLinkQuality,
			const unsigned int nTimestamp) const;

		// Called twice during factory reset: Once before persistent storage is
		// reverted to factory fresh settings (stage bit false), and once after
		// the storage has been reverted (stage bit true). Additional bits may
		// be set, which define the scope of the reset, e.g. if this is a full
		// factory reset. A full factory reset wipes security frame counters as
		// well as all attributes which are not marked as "preserved". It is
		// possible to define additional reset behavior, which is specific to a
		// particular application and sets a new operating mode, for example
		virtual void OnFactoryReset(const unsigned int nOptions);

	// Implementation
	public:
		// Flags for decrypting. These flags enable various "sources" for keys
		// that are acceptable for a particular decryption attempt. These are
		// either global keys, the pre-configured key, a permanent key from
		// apsDeviceKeyPairSet, or a pending key that has been either requested
		// or negotiated but it is pending confirmation, or a trust center
		// swapout link-key
		enum { acceptDefaultTrustCenterLinkKey = 0x01,
			acceptDistributedSecurityLinkKey = 0x02,
			acceptPreconfiguredLinkKey = 0x04,
			acceptPermanentLinkKey = 0x08, acceptPendingLinkKey = 0x10,
			acceptTrustCenterSwapoutLinkKey = 0x20,
			updateExtendedAddress = 0x80 };

		// APS transmit queue limit
		static const unsigned char nQueueLimit;

		// APS defragmentation and reassembly queue limit (number of concurrent
		// inbound fragmented ASDU transfers)
		static const uint8_t inboundQueueLimit;

		// On-going transmit transactions
		std::multimap<unsigned char, CZigBeeTransactionTX> m_transactionsTX;

		// On-going receive transactions and APS duplicate rejection table
		std::set<CZigBeeTransactionRX> m_transactionsRX;

		// Timer used to expire inbound APS transactions (duplicate rejection
		// table entries and defragmentation contexts)
		CCompactTimer *m_transactionsRXTimer;

		// APS pending network address lookup requests
		std::multimap<unsigned long long,
			std::pair<ADDRESSLOOKUPCALLBACK, void *> > m_lookups;

#ifdef _DEBUG
		mutable unsigned int m_nIdle;
#endif

		// APS sequence counter
		unsigned char m_bSequence;

		// Application map. Maps endpoint numbers to application object
		// instances
		static const CZigBeeApplicationMapEntry applicationMap[];

		// Acceptable trust center link-keys. A combination of default trust
		// center link key (e.g. Home Automation), distributed security link
		// key (e.g. Light Link), pre-configured link key (e.g. Smart Energy,
		// HA Tarnhelm), unique trust center link key, and swapout link key
		static const uint8_t nAcceptableTrustCenterLinkKeys;

		// Gets the flags indicating acceptable trust center link keys
		static uint8_t GetAcceptableTrustCenterLinkKeys();

		// The security material used for decrypting the frame that is
		// currently being processed and dispatched
		const CZigBeeApplicationSecurityMaterial *m_pIncomingSecurityMaterial;

		// Called when the network address lookup is complete
		static void OnNetworkAddressLookupComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Creates a suitable APS header for data frames and acknowledgement
		// frames related to data frames and inserts it in front of the
		// supplied ASDU
		void PrependHeader(const CZigBeeAddress &destination,
			const uint_fast16_t profileID, const uint_fast16_t clusterID,
			const uint_fast8_t sourceEndpoint, CPacket &asdu,
			const uint_fast8_t transmitOptions, const uint_fast8_t sequence,
			const uint_fast8_t type, const uint_fast8_t block = UINT8_MAX);

		// Creates a suitable APS header for command frames and acknowledgement
		// frames related to command frames and inserts it in front of the
		// supplied ASDU
		void PrependHeader(const CZigBeeAddress &destination, CPacket &asdu,
			const uint_fast8_t transmitOptions, const uint_fast8_t sequence,
			const uint_fast8_t type);

		// Creates a secure APS command frame which can be either transmitted
		// using the NLDE or embedded in a tunnel command
		bool MakeSecure(const CZigBeeAddress &destination, CPacket &asdu,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions, const unsigned char bSequence,
			const unsigned char bType);

		// Assembles and transmits an APDU (data or data acknowledgement)
		void Transmit(const CZigBeeAddress &destination,
			const uint_fast16_t profileID, const uint_fast16_t clusterID,
			const uint_fast8_t sourceEndpoint, CPacket &asdu,
			const uint_fast8_t transmitOptions,
			const uint_fast8_t enhancedOptions, const uint_fast8_t radius,
			const uint_fast8_t sequence, const uint_fast8_t type,
			const uint_fast8_t block = UINT8_MAX);

		// Assembles and transmits an data acknowledgement
		void TransmitDataAcknowledgment(const CZigBeeAddress &destination,
			const uint_fast16_t profileID, const uint_fast16_t clusterID,
			const uint_fast8_t sourceEndpoint, const uint_fast8_t transmitOptions,
			const uint_fast8_t enhancedOptions, const uint8_t sequence,
			const uint_fast8_t block);

		// Assembles and transmits an APDU (command or command acknowledgement)
		void Transmit(const CZigBeeAddress &destination, CPacket &asdu,
			const uint_fast8_t transmitOptions,
			const uint_fast8_t enhancedOptions, const uint_fast8_t radius,
			const uint_fast8_t sequence, const uint_fast8_t type);

		// Inserts an APDU (conveying a data ASDU) to the transmit queue for
		// delivery and creates the appropriate fragments as far as required.
		// If the local stations is addressed (either directly or via group
		// member-ship) issues an APSDE-DATA.indication to the local NHLE
		// instead (or additionally in case of group addressing)
		void QueueTransmission(CZigBeeAddress destination,
			const unsigned short wProfileID, const unsigned short wClusterID,
			const unsigned char bSourceEndpoint, CPacket &asdu,
			const unsigned char bTransmitOptions,
			const unsigned char bRadius, void *const pCookie,
			CZigBeeBindingTargets *const pTargets,
			const unsigned char bEnhancedOptions,
			const CZigBeeAddress &alias = CZigBeeAddress(),
			const unsigned char nAliasSequence = 0);

		// Inserts an APDU (conveying an APS command) to the transmit queue for
		// delivery and creates the appropriate fragments as far as required.
		// If the local stations is addressed (either directly or via broadcast)
		// issues an appropriate indication to the APSME instead
		void QueueTransmission(CZigBeeAddress destination, CPacket &asdu,
			const unsigned char bTransmitOptions, const unsigned char bRadius,
			void *const pCookie, const unsigned char bEnhancedOptions);

		// Issues an APSDE-DATA.confirm to the appropriate application
		void ConfirmData(const CZigBeeAddress &destination,
			const unsigned char bSourceEndpoint, const unsigned char bStatus,
			const unsigned int nTimestamp, void *const pCookie,
			CZigBeeBindingTargets *const pTargets);

		// Issues a data confirmation to the APSME
		void ConfirmData(const CZigBeeAddress &destination,
			const unsigned char bStatus, const unsigned int nTimestamp,
			void *const pCookie);

		// Issues an APSDE-DATA.indications to the candidate application, if
		// appropriate. Returns true, if data indication has been processed and
		// there is no need to traverse the set of active application further
		bool IndicateData(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wProfileID,
			const unsigned short wClusterID, const CPacket &asdu,
			const unsigned char bStatus, const bool bSecurityUsed,
			const unsigned char nLinkQuality, const unsigned int nTimestamp,
			CZigBeeApplication &application) const;

		// Allows the application support layer to reject frames at a very
		// early stage of application layer processing. Returns true if the
		// frame shall be processed further; returns false if the message is to
		// be rejected and dropped without further processing
		bool Preprocess(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wProfileID,
			const unsigned short wClusterID, const CPacket &asdu,
			const unsigned char bStatus, const bool bSecurityUsed,
			const unsigned char nLinkQuality, const unsigned int nTimestamp,
			CZigBeeApplication &application) const;

		// Allows the application support layer to be notified of imminent
		// processing of an inbound APS message. This is executed at a late
		// stage of inbound APS frame processing
		void Preprocess(CZigBeeAddress &destination, CZigBeeAddress &source,
			const CZigBeeAHR &header, CPacket &command,
			const unsigned char nLinkQuality, const unsigned int nReceiveTime,
			const bool bSecurityUsed);

		// Allows the application support layer to be notified of the outcome
		// of processing outbound APS messages. This is executed at a late
		// stage of inbound APS frame processing
		void OnTransactionComplete(const CZigBeeTransactionTX &transaction,
			const uint_fast8_t status);

		// Processes an incoming acknowledgment frame
		void ProcessAcknowledgment(const CZigBeeAHR &header);

		// Processes an incoming acknowledgment frame
		void DefaultProcessAcknowledgment(const CZigBeeAHR &header);

		// Completes a transaction by issuing an APSDE-DATA.confirm to the
		// application that is registered on the corresponding source
		// endpoint
		void CompleteTransaction
			(std::list<CZigBeeTransactionTX>::iterator transaction);

		// Returns the number of currently on-going APS defragmentations
		uint_fast8_t GetDefragmentationsCount() const;

		// Default implementation for IsDuplicate(), which should be
		// called by application-specific overloads of IsDuplicate()
		// in addition to any custom logic provided by the application
		bool DefaultIsDuplicate(const CZigBeeAddress &sourceAddress,
			const CZigBeeAHR &header, const uint_fast8_t fragmentType,
			const uint_fast8_t block,
			std::set<CZigBeeTransactionRX>::iterator &transaction,
			const unsigned int receiveTime);

		// Returns true if an incoming frame from the specified source with the
		// supplied header is a duplicate frame and should be discarded
		bool IsDuplicate(const CZigBeeAddress &sourceAddress,
			const CZigBeeAHR &header, const uint_fast8_t fragmentType,
			const uint_fast8_t block,
			std::set<CZigBeeTransactionRX>::iterator &transaction,
			const unsigned int receiveTime);

		// Returns false if fragmentation parameters are unsupported or there is
		// a temporary lack of resources for defragmentation (too many fragments
		// pending defragmentation)
		bool VerifyFragmentation(const CZigBeeAddress &sourceAddress,
			const CPacket &nsdu, const CZigBeeAHR &header,
			const uint_fast8_t fragmentType, const uint_fast8_t block,
			std::set<CZigBeeTransactionRX>::const_iterator &transaction) const;

		// Timer call-back. Invoked when an inbound transaction has expired,
		// i.e. the duplicate rejection table entry is no longer required
		void OnTransactionRXExpired(void *pArgument);

		// Maintains the duplicate rejection table
		bool WalkDuplicates(std::set<CZigBeeTransactionRX>::iterator
			&oldestOverall, int &elapsedTimeOverall,
			std::set<CZigBeeTransactionRX>::iterator *const
				oldestPreferred = 0,
			int *const elapsedTimePreferred = 0,
			const uint_fast16_t address = CZigBeeAddress::wBroadcastAll,
			const bool expire = false);

		// Used during maintenance of the duplicate rejection table
		void CompareAndUpdate(std::set<CZigBeeTransactionRX>::iterator &i,
			std::set<CZigBeeTransactionRX>::iterator &oldest,
			int &mostElapsedTime, const int elapsedTime);

		// Schedules a timer for when the next inbound APS transaction is about
		// to expire. This coincides with expiration of the duplicate rejection
		// table entry. The timer timer must have expired or must have been
		// cancelled before
		void ScheduleTransactionsRXTimer(const int elapsedTimeOverall);

		// Schedules (or reschedules) a timer for when the next inbound APS
		// transaction is about to expire. This coincides with expiration of the
		// duplicate rejection table entry. Will cancel any pending timer and
		// determine the timeout as appropriate
		void ScheduleTransactionsRXTimer();

		// Inserts an auxiliary header in front of frames that employ security
		bool PrependAuxiliaryHeader(CPacket &apdu,
			CPacketData::size_type &cbHeaders,
			const CZigBeeApplicationSecurityMaterial &material,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions);

		// Inserts an auxiliary header in front of frames that employ security
		bool PrependAuxiliaryHeader(CPacket &asdu,
			const CZigBeeAddress &destination,
			CPacketData::size_type &cbHeaders,
			CZigBeeApplicationSecurityMaterial &material,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions, unsigned char &nStatus);

		// Takes an unsecured APDU, encrypts the payload adds the message
		// integrity code
		bool EncryptAndAppendIntegrityCode(CPacket &asdu,
			CPacketData::size_type &cbHeaders,
			const CZigBeeApplicationSecurityMaterial &material,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions, unsigned char &nStatus);

		// Takes an unsecured APDU, encrypts the payload adds the message
		// integrity code
		bool EncryptAndAppendIntegrityCode(CPacket &npdu,
			const CPacketData::size_type cbHeaders,
			const CZigBeeApplicationSecurityMaterial &material,
			const unsigned char bTransmitOptions,
			const unsigned char bEnhancedOptions);

		// Incoming frame processing for any APS-secured frame. Notice that
		// the allowed link keys will be tried in the order application link
		// key (implies unique trust center link key), pre-configured link key,
		// distributed security link key, default trust center link key. Once
		// authenticated (trust center address non-zero), only application
		// link keys should be enabled
		bool DecryptAndAuthenticate(CPacket &apdu,
			const CZigBeeAHR &header, CZigBeeAddress &source,
			const CZigBeeAddress &destination,
			CZigBeeApplicationSecurityMaterial &material,
			const unsigned char nFlags = acceptPermanentLinkKey);

		// Attempts to decrypt a frame using a specific key. Used by the main
		// DecryptAndAuthenticate() method above
		bool DecryptAndAuthenticate(CPacket &apdu,
			const CPacketData::size_type cbHeaders,
			const CZigBeeAddress &source, const unsigned char nFlags,
			const unsigned char nFlag,
			CZigBeeApplicationSecurityMaterial &finalMaterial,
			CZigBeeApplicationSecurityMaterial &material,
			const CZigBeeAuxiliaryHeader &aux);

		// Performs a freshness check for an inbound frame and updates the
		// incoming security frame counter. Called by DecryptAndAuthenticate()
		// as one of the final incoming security processing steps
		bool IsFresh(const CZigBeeAuxiliaryHeader &aux,
			CZigBeeApplicationSecurityMaterial &material,
			const bool allowSecurityChallenge);

		// Timer call-back used to fire APSDE-DATA.request after a delay
		void OnDataRequestDelayElapsed(void *pArgument);

		// This is an internal function dispatching either to the network layer
		// via NLDE-DATA.request, or to a remote device via the APDU relay
		void RequestData(const CZigBeeAddress &destination, CPacket &apdu,
			const unsigned char nHandle, const unsigned char nRadius,
			const unsigned char nNonmemberRadius,
			const bool bEnableRouteDiscovery, const bool bEnableNetworkSecurity,
			CZigBeeAddress source = CZigBeeAddress(),
			unsigned char nSequence = 0,
			const unsigned char nTransmitOptions = 0,
			const CIEEE802154Address *const pNextHopAddress = 0);

		// Transmit outbound upstream and downstream relay commands
		void TransmitRelayData(CZigBeeAddress destination, CPacket &asdu,
			const unsigned char bTransmitOptions, const unsigned char bRadius,
			void *const pCookie, const unsigned char bEnhancedOptions);

		// Create an APS transmission sequence number by consecutively
		// incrementing the internal sequence counter with roll-over allowed.
		// The created sequence number must not be used by any currently
		// pending APS tranmissions
		unsigned char AdvanceSequenceCounter();

#ifdef _DEBUG
		// Dumps the duplicate rejection table
		void DumpTransactionsRX() const;

		// Dumps relevant APS data (in particular application map)
		virtual void Dump() const;

		// Returns the human-readable name of a status enumeration value.
		// Does not only return APS status codes, but also NWK, MAC and PHY
		// codes, as appropriate
		virtual const char *GetStatusString(const unsigned char nValue);

		// Returns the human-readable name of a status enumeration value.
		// Does not only return APS status codes, but also NWK, MAC and PHY
		// codes, as appropriate
		static const char *GetStatusStringInternal(const unsigned char nValue);
#endif

};


inline const CZigBeeApplicationSecurityMaterial *
CZigBeeApplicationSupport::GetIncomingSecurityMaterial() const
{
	return m_pIncomingSecurityMaterial;
}

#define __COMPACT7B_BEGIN_APPLICATION_MAP() \
	const CZigBeeApplicationMapEntry \
	CZigBeeApplicationSupport::applicationMap[] = { \

#define __COMPACT7B_END_APPLICATION_MAP() \
		{ 0 } \
	};

#define __COMPACT7B_APPLICATION(application) \
	{ application },


/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSupportEx

class CZigBeeApplicationSupportEx : public CZigBeeApplicationSupport
{
	// Construction
	public:
		CZigBeeApplicationSupportEx(CZigBeeNetwork &nwk);

	// Attributes
	public:

	// Overrides
	public:
		// Adds an application to the dynamic application map. Can be used to
		// implement the ConfigureEndpoint function in a gateway device, for
		// instance. If an application exists on the specified endpoint it is
		// replaced by the new application. It is first removed and - in case
		// of a heap application - also deleted. If another application exists
		// in the static application map, the function fails
		virtual bool AddApplication(CZigBeeApplication &application);

		// Removes an application from the dynamic application map. Notice,
		// that the application object is also automatically release via delete
		// in case of a heap application. Removing an application while the
		// network stack is operational is discouraged
		virtual bool RemoveApplication(CZigBeeApplication &application,
			const bool bDelete = true);

		// Returns a set of active application endpoints (except EP #0, ZDO)
		virtual void GetActiveEndpoints
			(std::set<unsigned char> &endpoints) const;

		// Returns the application object that services the specified endpoint,
		// or null if no such application exists. Notice that you should have
		// a critical section lock around the block that uses GetApplication()
		// and the returned application pointer
		virtual CZigBeeApplication *GetApplication
			(const unsigned char bEndpoint) const;

		// Notifies all applications that a device has just joined or rejoined
		// the network
		virtual void NotifyDeviceAnnouncement(const CZigBeeAddress &address,
			const unsigned char bCapabilities);

		// Notifies all applications that the device has started up
		virtual void NotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// Notifies all applications of the ongoing start-up progress status
		virtual void NotifyStartupProgress(const uint_fast8_t state,
			const uint_fast16_t flags, const uint_fast8_t status = statusSuccess);

		// Notifies all applications that the device has left the network
		virtual void NotifyNetworkLeave(const unsigned char nStatus,
			const bool bRejoin);

		// Called once after power-up in main() to initialize applications
		virtual void AfterPowerup();

		// Called periodically from a loop in main() to process idle tasks.
		// Returns true if there are more idle tasks pending, false otherwise.
		// Systems that implement an event-loop and enter sleep mode should use
		// this information and not enter sleep as long as there are idle tasks
		// pending. Calls each dynamic application's OnIdle() method, and then
		// invokes the base class method
		virtual bool OnIdle() const;

		// Notifies all applications of a network status indication
		virtual void NotifyNetworkStatus(const unsigned char nStatus,
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

	// Overrides
	public:
		// Issues one or more APSDE-DATA.indications to their intended
		// destination applications within the local next higher layer entity
		virtual void IndicateData(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned short wProfileID,
			const unsigned short wClusterID, const CPacket &asdu,
			const unsigned char bStatus, const bool bSecurityUsed,
			const unsigned char nLinkQuality,
			const unsigned int nTimestamp) const;

	// Implementation
	public:
		// Dynamic application map (can be used to add and remove applications
		// at run-time, for example to facilitate implementation of gateways
		std::map<unsigned char, CZigBeeApplication *> m_applications;

#ifdef _DEBUG
		// Dumps relevant APS data (in particular application map)
		virtual void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDataStubDelegate

class CZigBeeDataStubDelegate
{
	// typedefs & enums
	public:

	// Attributes
	public:
		// Callback for indication handling
		void (*m_indicate)(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, const uint_fast16_t profileID,
			const uint_fast16_t clusterID, CPacket &asdu,
			const uint_fast8_t linkQuality, const uint_fast32_t timestamp);

		// Callback for confirmation handling
		void (*m_confirm)(const uint_fast8_t status,
			const uint_fast32_t timestamp, void *pArgument);

	// Operations
	public:
		// Process data stub indication
		static void Indicate(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, const uint_fast16_t profileID,
			const uint_fast16_t clusterID, CPacket &asdu,
			const uint_fast8_t linkQuality, const uint_fast32_t timestamp);

		// Process data stub confirmation
		static void Confirm(const uint_fast8_t status,
			const uint_fast32_t timestamp, void *pArgument);
};


#define __C7B_DATASTUB_DELEGATE_IMPLEMENT_EX(id, ...) \
	__COMPACT_SECTION_IMPLEMENT(CZigBeeDataStubDelegate, \
		c7b_datastubd, id, __VA_ARGS__)

#define __C7B_DATASTUB_DELEGATE_IMPLEMENT(identifier, ...) \
	__C7B_DATASTUB_DELEGATE_IMPLEMENT_EX( \
	__c7b_datastubd##identifier, identifier, __VA_ARGS__)


/////////////////////////////////////////////////////////////////////////////
// Global functions (application level overridables)

void OnZigBeeStartupComplete(const unsigned char nStatus,
	const unsigned short nFlags);

void OnZigBeeStartupProgress(const uint_fast8_t state,
	const uint_fast16_t flags, const uint_fast8_t status);

void OnZigBeeNetworkLeave(const uint8_t nStatus, const bool bRejoin,
	uint8_t &actionFlags);

void OnZigBeeAssociationPermitModified(const bool bAssociationPermit);

void OnZigBeeConfirmDataStub(const unsigned char nStatus,
	const unsigned int nTimestamp, void *pArgument);

void OnZigBeeIndicateDataStub(const CZigBeeAddress &source,
	const CZigBeeAddress &destination, const unsigned short wProfileID,
	const unsigned short wClusterID, CPacket &asdu,
	const unsigned char nLinkQuality, const unsigned int nTimestamp);
