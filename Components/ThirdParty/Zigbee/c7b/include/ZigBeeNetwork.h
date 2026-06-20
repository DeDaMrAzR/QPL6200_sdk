// ZigBeeNetwork.h : header file
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
// as of January 17, 2007
//
// This module implements the network layer (NWK) according to ZigBee 2007.
// It builds on the Compact15.4(TM) IEEE 802.15.4 MAC layer implementation

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeNetwork;
class CZigBeeNetworkManagement;
class CZigBeeStorageItem;
class CZigBeeNetworkLeaveCookie;
class CZigBeeRouteDiscoveryInfo;
class CZigBeeApplicationSupport;
class CZigBeeApplication;
class CZigBeeTLVs;


/////////////////////////////////////////////////////////////////////////////
// byte packing for all packet definitions

#pragma pack(push, 1)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNHR, clause 3.3.1

class CZigBeeNHR
{
	// typedefs & enums
	public:
		// Protocol version and discover route sub-fields
		enum { maskType = 0x0003, maskVersion = 0x003c,
			maskDiscoverRoute = 0x00c0, shiftVersion = 2,
			shiftDiscoverRoute = 6, multicast = 0x0100,
			securityEnabled = 0x0200, sourceRoute = 0x0400,
			extendedDestinationAddress = 0x0800,
			extendedSourceAddress = 0x1000, endDeviceInitiator = 0x2000 };

		// ZigBee Protocol versions (rather a major frame type)
		enum { versionLegacyZigBee = 1, versionZigBee = 2,
			versionZigBeeGreenPower = 3 };

		// Frame type sub-field, possible values
		enum { typeData, typeCommand, typeReserved, typeStub };

		// Discover route sub-field
		enum { routeDiscoverySuppressed, routeDiscoveryEnabled };

	// Attributes
	public:
		// Frame control field, clause 2.4.1.1
		unsigned short m_wFrameControl;

		// Destination address, clause 2.4.1.2 (data, command)
		unsigned short m_wDestinationAddress;

		// Source address, clause 2.4.1.3 (data, command)
		unsigned short m_wSourceAddress;

		// Radius field, clause 2.4.1.4 (data, command)
		unsigned char m_bRadius;

		// Sequence number field, clause 2.4.1.5 (data, command)
		unsigned char m_bSequence;

		// Extended IEEE address of destination (optional)
		// Extended IEEE address of source (optional)
		// Multicast Control field
		// Source routing information (optional)

	// Operations
	public:
		// Returns the protocol version
		unsigned char GetVersion() const;

		// Returns the frame type
		unsigned char GetType() const;

		// Returns the route discovery options
		unsigned char GetRouteOptions() const;

		// Returns the NWK header size, including fixed-length portions and
		// the length of optional fields. It does not include the size of the
		// auxiliary header for secured frames; use GetSizeEx() if you want to
		// skip over all headers
		CPacketData::size_type GetSize() const;

		// Same as GetSize(), but for secured frames, also includes the length
		// of the auxiliary header
		CPacketData::size_type GetSizeEx() const;

		// Returns the destination IEEE address, if present in the NWK header.
		// This returns the L2 hardware address, not the network address. The
		// network address can be obtained via m_wDestinationAddress
		bool GetDestinationAddress(uint64_t &qwAddress) const;

		// Returns the destination address as address object that contains a
		// network address and might also contain an extended address
		CZigBeeAddress GetDestinationAddress() const;

		// Returns the source IEEE address, if present in the NWK header
		// This returns the L2 hardware address, not the network address. The
		// network address can be obtained via m_wSourceAddress
		bool GetSourceAddress(uint64_t &qwAddress) const;

		// Returns the source address as address object that contains a network
		// address and might also contain an extended address
		CZigBeeAddress GetSourceAddress() const;

		// Returns the multicast control field, if present in the NWK header
		bool GetMulticastControl(unsigned char &bMulticastControl) const;

		// Returns a pointer to the beginning of the source route subframe
		const unsigned char *GetSourceRouteInfo() const;

		// Returns a pointer to the beginning of the source route subframe
		unsigned char *GetSourceRouteInfo();

		// Returns source routing information, i.e. number of relays available
		// and current relay index
		bool GetSourceRouteInfo(unsigned char &bCount,
			unsigned char &bIndex) const;

		// Returns the specified relay entry
		unsigned short GetSourceRouteRelay(const unsigned char bIndex) const;

	// Implementation
	protected:

#ifdef _DEBUG
	public:
		// Dumps the fields of a NWK frame header
		void Dump() const;
#endif

};

inline unsigned char CZigBeeNHR::GetVersion() const
	{ return (m_wFrameControl & maskVersion) >> shiftVersion; }

inline unsigned char CZigBeeNHR::GetType() const
	{ return (m_wFrameControl & maskType); }

inline unsigned char CZigBeeNHR::GetRouteOptions() const
	{ return (m_wFrameControl & maskDiscoverRoute) >> shiftDiscoverRoute; }


/////////////////////////////////////////////////////////////////////////////
// restore previous packing

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNeighborInfoEx

class CZigBeeNeighborInfoEx
{
	// Attributes
	public:
		// A network descriptor in the same format that is used to convey
		// network information via the NLME-NETWORK-DISCOVERY.confirm primitive
		CZigBeeNetworkDescriptor m_descriptor;

		// The tree depth of the neighbor device. This is irrelevant and
		// potentially inaccurate and misleading a mesh network and should
		// always be ignored (leftover of legacy tree-routing)
		unsigned char m_nDepth;

		// An indication of whether the device has been ruled out as a
		// potential parent
		bool m_bPotentialParent;

		// Reflects the value of nwkUpdate as reported in the node's beacon
		unsigned char m_nUpdateID;

		// Total number of bytes of all TLVs advertised by the router. Notice
		// that this will be zero for R22 and earlier Zigbee devices, or other
		// kinds of 802.15.4 networks
		uint8_t m_cbTLVs;

		// For R23+ devices, variable-length TLV-encoded data follows, which
		// holds information about the network and the router, as advertised
		// in beacons or received via network commissioning requests. The
		// overall length of the TLV data is indicated by the m_cbTLVs member
		// above. Use GetTLVs() to obtain a pointer conveniently

	// Operations
	public:
		// Returns a read-only pointer to the TLVs associated with this neighbor
		const void *GetTLVs() const;

		// Returns a read-only pointer to the TLVs associated with this neighbor
		void *GetTLVs();

#ifdef _DEBUG
		// Shows the contents of the extended neighbor table entry
		void Dump() const;
#endif

};


inline const void *CZigBeeNeighborInfoEx::GetTLVs() const
{
	return &m_cbTLVs + sizeof(uint8_t);
}


inline void *CZigBeeNeighborInfoEx::GetTLVs()
{
	return &m_cbTLVs + sizeof(uint8_t);
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNeighborInfo

class CZigBeeNeighborInfo
{
	// typedefs & enums
	public:
		// ZigBee device types
		enum { typeCoordinator, typeRouter, typeEndDevice };

		// Possible relationships. Notice that the specification mentions an
		// "unauthenticated child", but we use the term "unauthenticated",
		// instead. This is because this state can also occur for routers when
		// they join or rejoin, and we don't consider routers as child devices
		enum { relationParent, relationChild, relationSibling, relationNone,
			relationPreviousChild, relationUnauthenticated };

		// Flags (specific to ubisys). Notice 'hub-connectivity', 'preferred
		// parent', '24h+ up-time' and 'acceptable LQA' are a group of flags,
		// which need to remain adjacent and retain their order/priority for
		// parent selection to work as expected. When new flags are added, or
		// existing ones removed, above-mentioned relationship must remain
		// intact
		enum { flagAnnounce = 0x01, flagSupportsCommissioning = 0x02,
			flagRelayEnabled = 0x04, flagTimeoutNegotiationSupported = 0x08,
			flagTunnelMask = 0x30, flagTunnelShift = 4,
			flagSupportsKeyAgreement = 0x80,
			flagHubConnectivity = 0x0400, flagPreferredParent = 0x0200,
			flag24hPlusUptime = 0x0100, flagAcceptableLinkQuality = 0x0800 };

	// Construction
	public:
		CZigBeeNeighborInfo(CZigBeeNeighborInfoEx *pExtended = 0);
		CZigBeeNeighborInfo(const CZigBeeNeighborInfo &info);
		CZigBeeNeighborInfo(const unsigned short wNetworkAddress);

	// Attributes
	public:
		// 64-bit IEEE address that is unique to every device. This field shall
		// be present if the neighbor is a parent or child of the device. It
		// might be 0xffffffffffffffff, indicating that the extended address is
		// currently unknown
		uint64_t m_qwExtendedAddress;

		// Incoming frame counter for this neighbor. Conceptually this is not
		// part of the neighbor table, but a distinct attribute. For memory
		// optimization reasons we use a field in the neighbor table in the
		// actual implementation
		unsigned int m_nFrameCounter;

		// The 16-bit network address of the neighboring device. This field
		// shall be present in every neighbor table entry. It might be 0xfffe,
		// indicating that only the extended address is valid
		unsigned short m_wNetworkAddress;

		// The previous 16-bit network address of the neighboring device, if
		// any. This field is a proprietary ubisys extension, which facilitates
		// address conflict resolution on behalf of sleepy end-device children.
		// It might be 0xfffe, indicating no previous address
		unsigned short m_wPreviousNetworkAddress;

		// The relationship between the neighbor and the current device.
		// This field shall be present in every neighbor table entry
		unsigned char m_nRelationship;

		// A value indicating if previous transmissions to the device were
		// successful or not. Higher values indicate more failures.
		// This field shall be present in every neighbor table entry
		unsigned char m_nTransmitFailures;

		// The estimated link quality for RF transmission from this device
		// after having applied the Link Quality Assessment metric. See
		// sub-clause 3.6.3 and 3.6.4.1 for a discussion of how this should
		// be applied. This field shall be present in every neighbor table
		// entry
		uint8_t m_nLinkQuality;

		// Capabilities of the neighbor device (FFD vs. RFD, RX-on-when-idle)
		unsigned char m_bCapabilities;

		// If the neighbor is a router: The number of nwkLinkStatusPeriod
		// intervals since a link status command was received
		unsigned char m_nAge;

		// If the neighbor is a router: The cost of an outgoing link as
		// measured by the neighboring router. A value of 0 indicates no
		// outgoing cost is available
		// This field is mandatory if nwkSymLink is true
		unsigned char m_nOutgoingCost;

		// If the neighbor is an end-device: The device configuration bitmap
		// introduced as part of ZigBee PRO R21. Notice that this is currently
		// unused in R21
		unsigned char m_nEndDeviceConfiguration;

		// If the neighbor is an end-device: The end-device timeout
		// enumeration introduced as part of ZigBee PRO R21 as requested by the
		// end-device after joining or rejoining
		unsigned char m_nEndDeviceTimeout;

		// Flags for this neighbor table entry. These are specific to Compact7B
		// and are not part of the specification
		uint16_t m_flags;

		// This reserved byte is available (32-bit alignment)
		unsigned char m_bReserved;

		// Life-time counter. If this entry belongs to an authenticated end
		// device child, this value is initialized to the negotiated or default
		// end-device timeout after reboot and whenever a keep-alive message
		// has been received. Then, it specifies the time, in seconds, that the
		// entry will remain active. If it reaches zero, the end-device has
		// timed out and will be removed from the child table.
		// This field is also used during association, when a new devices joins
		// at this router. In this case it is used to purge the device from the
		// neighbor table, if it no encrypted message is received within the
		// security timeout. In this case, the time is specified in ticks
		std::pair<CCompactTimerService::ticks_diff_t, bool> m_lifetime;

		// Means by which the device attempts to join (association, orphaning,
		// or network rejoin), or else, how this node obtained knowledge of its
		// neighbor (active scan, link status command frame)
		unsigned char m_nJoinMethod;

		// A value indicating if previous transmissions to the device were
		// successful or not. This is limited to the most recent transmissions
		unsigned char m_nTransmitFailuresEx;

		// Extended neighbor information. Only available after a network
		// discovery and before joining a network, or for a device that is
		// currently rejoining to this router. In the latter case, only the
		// TLVs contained in the extended information object are valid
		CZigBeeNeighborInfoEx *m_pExtended;

	// Operations
	public:
		// Discards extended neighbor information
		void DiscardExtended();

		// Assignment operator (STL container support)
		CZigBeeNeighborInfo &operator=(const CZigBeeNeighborInfo &info);

		// Less operator (STL sorted container support)
		bool operator<(const CZigBeeNeighborInfo &info) const;

		// Returns all addressing material that is available for this neighbor
		CZigBeeAddress GetAddress() const;

		// Returns the device type, i.e. coordinator, router or end-device
		unsigned char GetDeviceType() const;

		// Returns true, if the device has its receiver turned on during idle
		// periods
		bool IsReceiverOnWhenIdle() const;

		// Returns true, if the device is an end-device
		bool IsEndDevice() const;

		// Returns true, if the device is a router (including the coordinator)
		bool IsRouter() const;

		// Returns true, if the neighbor is considered as authenticated on the
		// network
		bool IsAuthenticated() const;

		// Returns true, if the device is authorized as relay source or target
		bool IsRelayEnabled() const;

		// Returns zero if this neighbor is not connected via an NPDU tunnel
		// or a tunnel number between 1 and 3 if it is connected via a tunnel
		uint_fast8_t GetTunnelIdentifier() const;

		// Returns the device's extended device capabilities, or zero if no
		// such capabilities are on record
		static uint_fast16_t GetCapabilitiesEx(const void *const pTLVs,
			const uint_fast8_t cbTLVs);

		// Returns the device's extended device capabilities, or zero if no
		// such capabilities are on record
		uint_fast16_t GetCapabilities() const;

		// Returns true if the device has any of the specified capabilities
		bool HasCapabilities(const uint_fast16_t capabilities) const;

		// Update the link quality value. This will internally apply the
		// Link Quality Assessment (LQA) metric in order to, e.g., mitigate
		// the effect of singular outliers.
		void UpdateLinkQuality(const uint8_t linkQuality);

	// Implementation
	public:
		~CZigBeeNeighborInfo();

		// Three most recent raw link quality measurements, on which the Link
		// Quality Assessment metric should be applied to mitigate the effect
		// of singular measurement outliers, e.g., due to short, transitional
		// radio interference. See sub-clause 3.6.3 for a discussion of how
		// this filtering should be applied.
		uint8_t m_rawLinkQualities[3];

		// Index value for tracking the position of the latest raw link
		// quality measurement, stored in the measurement array. The boolean
		// flag, when set to true, indicates that the circular buffer has
		// been filled and all elements are valid.
		std::pair<uint_fast8_t, bool> m_indexLinkQuality;

#ifdef _DEBUG
		// Shows the contents of the neighbor table entry
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeParentRanking

class CZigBeeParentRanking
{
	// typedefs & enums
	public:

	// Construction
	public:
		CZigBeeParentRanking(const CIEEE802154Address &preferredAddress);

	// Attributes
	public:

	// Overridables
	public:
		// Compares two neighbors and returns true if a < b, i.e. a should
		// be ranked higher as a potential parent than b. Applications may
		// override the default to provide their own fine-tuned ranking,
		// potentially taking advantage of application layer knowledge, which
		// is not immediately available to the core stack. However, care must
		// taken to not violate core specification requirements
		bool operator()(const CZigBeeNeighborInfo &a,
			const CZigBeeNeighborInfo &b) const;

	// Implementation
	public:
		static const bool preferKeyNegotiationSupport;

		// Preferred address if precedent ranking criteria don't yield
		// an outcome for a more favorable parent; should only play a role
		// for a rejoin process where a rejoiner may prefer its previous
		// parent router if still available.
		CIEEE802154Address m_preferredAddress;

		// Default implementation for the ranking comparision operator,
		// which should be called by application-specific overloads of
		// operator() in addition to any custom logic provided by the
		// application
		bool DefaultRankingComparator(const CZigBeeNeighborInfo &a,
			const CZigBeeNeighborInfo &b) const;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDispatch

class CZigBeeDispatch
{
	// Attributes
	public:
		// Client instance
		CZigBeeNetworkClient *m_pClient;

		// General-purpose argument to the callback function
		void *m_pArgument;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDispatchDataConfirm

class CZigBeeDispatchDataConfirm : public CZigBeeDispatch
{
	// typedefs & enums
	public:
		// Callback to a MCPS-DATA.confirm(msduHandle, status, Timestamp)
		// handler. When requesting data, register a callback with the
		// specific handle
		typedef void (CZigBeeNetworkClient::*CALLBACK)
			(const unsigned char nHandle, const unsigned char nStatus,
			const unsigned int nTimestamp, void *pArgument);

	// Construction
	public:
		CZigBeeDispatchDataConfirm(const unsigned char nHandle = -1);

	// Attributes
	public:
		// Member function to invoke
		CALLBACK m_pfnCallback;

		// Destination address for the transmission
		unsigned short m_wDestination;

		// Value to match against MCPS-DATA.confirm's handle parameter
		unsigned char m_nHandle;

		// True if the callback has been invoked
		bool m_bInvoked;

	// Operations
	public:
		// Operator Less (supports STL sorted containers)
		bool operator<(const CZigBeeDispatchDataConfirm &dispatch) const;

		// Invokes the dispatch and marks it as invoked
		void Invoke(const unsigned char nStatus,
			const unsigned int nTimestamp);
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeBroadcastTransaction

class CZigBeeBroadcastTransaction
{
	// Construction
	public:
		CZigBeeBroadcastTransaction();
		CZigBeeBroadcastTransaction(const CZigBeeBroadcastTransaction &transaction);

	// Attributes
	public:
		// The set of nodes from which we have heard a transmission of this
		// network-wide broadcast (only required for coordinator/routers and
		// end devices with macRxOnWhenIdle equal to true)
		std::set<unsigned short> m_neighborRelays;

		// The broadcast packet that has been received and should be relayed
		// up to nwkMaxBroadcastRetries times (NPDU)
		CPacket m_npdu;

		// The transmit time-stamp of the first transmission attempt
		uint_fast32_t m_nTimestamp;

		// The 16-bit network address of the broadcast initiator
		unsigned short m_wSourceAddress;

		// The NWK layer sequence number of the initiator's broadcast
		unsigned char m_nSequence;

		// A countdown timer indicating the number of ticks until this
		// entry expires; the initial value is nBroadcastDeliveryTime
		uint32_t m_expirationTime;

		// A countdown timer indicating the number of ticks until the passive
		// acknowledgment timeout for a previous transmission expires;
		// the initial value is nPassiveAcknowledgmentTimeout
		uint16_t m_passiveAcknowledgmentTime;

		// A countdown timer indicating the number of ticks until the
		// broadcast jitter timeout for next transmission expires
		uint16_t m_jitterTime;

		// Number of times this broadcast has been transmitted by the present
		// device
		unsigned char m_nAttemptCount;

		// The original handle which holds the data confirmation dispatch
		// information, as set-up by the next higher layer, before the
		// broadcast transmit logic created its own handles to intercept the
		// MCPS-DATA.confirm of the initial broadcast attempt
		unsigned char m_nHandle;

	// Operations
	public:
		// Assignment operator (STL container support)
		CZigBeeBroadcastTransaction &operator=
			(const CZigBeeBroadcastTransaction &transaction);

		// Less operator (STL sorted container support)
		bool operator<(const CZigBeeBroadcastTransaction &broadcast) const;

		// Get time period in ticks to the next pending expiration
		uint_fast32_t GetPendingTimeout() const;

#ifdef _DEBUG
	public:
		// Dumps this broadcast transaction table entry
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRoute

class CZigBeeRoute
{
	// typedefs & enums
	public:
		// Possible values and flags for the route status field. In addition
		// to the flags defined by the ZigBee standard, 'source-route' is an
		// extension for internal use that indicates a source route has been
		// selected for delivering a frame to its next hop. This flag is only
		// used for ephemeral route objects which are created when delivering
		// a frame. Routing table entries never have this flag set
		enum { statusActive, statusDiscoveryUnderway, statusDiscoveryFailed,
			statusInactive, statusValidationUnderway, statusMask = 0x07,
			memoryConstrained = 0x08, manyToOne = 0x10,
			routeRecordRequired = 0x20, sourceRoute = 0x80 };

	// Construction
	public:
		CZigBeeRoute();
		CZigBeeRoute(const unsigned short wDestinationAddress,
			const unsigned char nStatusAndFlags = statusInactive);

	// Attributes
	public:
		// The 16-bit network address or Group ID of this route
		unsigned short m_wDestinationAddress;

		// The 16-bit network address of the next hop on the way to the
		// destination
		unsigned short m_wNextHop;

		// The status of the route
		unsigned char m_nStatusAndFlags;

	// Operations
	public:
		// Assignment operator (STL container support)
		//CZigBeeRoute &operator=(const CZigBeeRoute &route);

		// Less operator (STL sorted container support)
		bool operator<(const CZigBeeRoute &route) const;

		// Returns the status portion of the status-and-flags member
		unsigned char GetStatus() const;

		// Sets the status portion of the status-and-flags member without
		// modifying the flags
		void SetStatus(const unsigned char nStatus);

	// Implementation
	public:
		// Route discovery table entry. At any time only one discovery table
		// entry may acquire a lock on this routing table entry. This is to
		// avoid routing loops that may occur when there are multiple route
		// requests in progress where either the originator or the responder
		// are identical. If the bool is false, then the iterator is void and
		// there is no route discovery table entry referencing this routing
		// table entry
		CZigBeeRouteDiscoveryInfo *m_pDiscoveryInfo;

#ifdef _DEBUG
	public:
		// Dumps this routing table entry
		void Dump() const;
#endif

};

inline unsigned char CZigBeeRoute::GetStatus() const
	{ return m_nStatusAndFlags & statusMask; }

inline void CZigBeeRoute::SetStatus(const unsigned char nStatus)
	{ m_nStatusAndFlags &= ~statusMask; m_nStatusAndFlags |= nStatus; }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRouteDiscoveryInfo

class CZigBeeRouteDiscoveryInfo
{
	// typedefs & enums
	public:
		typedef void (CZigBeeNetworkManagement::*CALLBACK)
			(const unsigned char nStatus,
			const unsigned char nNetworkStatus,
			const CZigBeeRoute *const pRoute, void *pArgument);

	// Construction
	public:
		CZigBeeRouteDiscoveryInfo();

	private:
		// Take care when copy-constructing. Make sure that info.m_pTimer
		// is null, otherwise the timer will be referenced by to instances
		// and potentially cancelled to early (when one instance is deleted)
		//CZigBeeRouteDiscoveryInfo(const CZigBeeRouteDiscoveryInfo &info);

	// Attributes
	public:
		// The 16-bit network address of the route request's initiator
		unsigned short m_wSourceAddress;

		// The 16-bit network address of the device that has sent the most
		// recent lowest cost route request command frame corresponding to
		// this entry's route request identifier and source address
		unsigned short m_wSenderAddress;

		// A sequence number for a route request command frame that is
		// incremented each time a device initiates a route request
		unsigned char m_nRequestID;

		// The accumulated path cost from the source of the route request to
		// the current device
		unsigned char m_nForwardCost;

		// The accumulated path cost from the current device to the
		// destination device
		unsigned char m_nResidualCost;

		// Number of times this request has been transmitted
		unsigned char m_nAttemptCount;

		// A timer that is used for initiation of broadcasts, route validation
		// and aging of the route discovery table entry
		CCompactTimer *m_pTimer;

		// Routing table entry related to this route request. May be void if
		// the route discovery table entry is for a route request targeting the
		// local device
		std::pair<std::set<CZigBeeRoute>::iterator, bool> m_route;

		// Completion callback handler
		CALLBACK m_pfnComplete;

		// User-supplied argument passed to completion callback handler
		void *m_pArgument;

		// Route request command payload, updated with most recent path cost
		CZigBeeRouteRequestCommand m_request;

		// The sequence number for the route request command
		unsigned char m_nSequence;

		// The radius for the route request command
		unsigned char m_nRadius;

		// Route discovery status
		unsigned char m_nStatus;

		// True if the completion handler has already been invoked once
		bool m_bComplete;

	// Operations
	public:
		// Less operator (STL sorted container support)
		bool operator<(const CZigBeeRouteDiscoveryInfo &info) const;

	private:
		// Discovery info objects may not be assigned
		CZigBeeRouteDiscoveryInfo &operator=
			(const CZigBeeRouteDiscoveryInfo &info);

	// Implementation
	public:
		~CZigBeeRouteDiscoveryInfo();

#ifdef _DEBUG
	public:
		// Dumps this route discovery table entry
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeSourceRoute

class CZigBeeSourceRoute
{
	// Construction
	public:
		CZigBeeSourceRoute(const unsigned short wAddress,
			CZigBeeSourceRoute *const pPreviousHop = 0);

	// Attributes
	public:
		// Destination for this source route. This is also a key in the set of
		// source routes maintained by the concentrator and therefore immutable
		const unsigned short m_wAddress;

		// Number of other source routes that refer to this object. This number
		// is maintained as source routes are added and removed. When zero, no
		// other source routes refer to this source route
		unsigned short m_nReference;

		// Previous hop in the route back to the concentrator
		CZigBeeSourceRoute *m_pPreviousHop;

	// Operations
	public:
		bool operator<(const CZigBeeSourceRoute &route) const;

	// Implementation
	public:
#ifdef _DEBUG
		void Dump() const;
#endif

};

inline CZigBeeSourceRoute::CZigBeeSourceRoute(const unsigned short wAddress,
	CZigBeeSourceRoute *const pPreviousHop) :
m_wAddress(wAddress), m_nReference(0), m_pPreviousHop(pPreviousHop)
{
}

inline bool CZigBeeSourceRoute::operator<(const CZigBeeSourceRoute &route) const
	{ return m_wAddress < route.m_wAddress; }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkSecurityMaterial

class CZigBeeNetworkSecurityMaterial : public CZigBeeSecurityMaterial
{
	// Attributes
	public:
		// Set of incoming frame counter values and corresponding device
		// addresses. Maps 64-bit extended addresses to frame counter values
		// std::map<unsigned long long, unsigned int> m_incomingFrameCounters;
		// Notice: As an optimization, this has been moved to the neighbor
		// table

	// Construction
	public:
		CZigBeeNetworkSecurityMaterial(const unsigned char nKeyType = typeVoid);

	// Attributes
	public:
		// Returns true if this is an ephemeral key
		static bool IsEphemeral(const uint_fast8_t keyType);

		// Returns true if this is an ephemeral key
		bool IsEphemeral() const;

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps information stored in this security material set
		void Dump() const;
#endif

};


inline bool CZigBeeNetworkSecurityMaterial::IsEphemeral
	(const uint_fast8_t keyType)
{
	return (keyType == typeEphemeralGlobalAuthorization) ||
		(keyType == typeEphemeralUniqueAuthorization);
}


inline bool CZigBeeNetworkSecurityMaterial::IsEphemeral() const
{
	return IsEphemeral(m_nKeyType);
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkMRUEntry

class CZigBeeNetworkMRUEntry
{
	// Construction
	public:
		CZigBeeNetworkMRUEntry(const uint16_t initiator,
			const uint8_t sequence, const uint8_t radius = 0);

	// Attributes
	public:
		// Remaining monitoring period for a Most Recent Unicast (MRU)
		// outbound data transmission, in timer ticks. This is to keep track
		// of stale entries in the MRU maintenance set and remove them after
		// the monitoring period has expired
		uint32_t m_lifetime;

		// Initiator's network address of this MRU entry
		uint16_t m_initiator;

		// Sequence number of this MRU entry
		uint8_t m_sequence;

		// Radius of this MRU entry
		uint8_t m_radius;

	// Operations
	public:
		// Support for sorted STL containers
		bool operator<(const CZigBeeNetworkMRUEntry &info) const;

#ifdef _DEBUG
		// Dump information of this data transmission
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkAddressMap

typedef std::map<unsigned short, std::pair<unsigned long long, bool> >
	CZigBeeNetworkAddressMap;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkInformationBase

class CZigBeeNetworkInformationBase
{
	// Construction
	public:
		CZigBeeNetworkInformationBase();

	// Constants
	public:
		// Network layer protocol ID for ZigBee
		enum { protocolID = 0 };

		// Existing stack profiles (ZigBee and ZigBee PRO)
		enum { profileZigBee = 1, profileZigBeePRO = 2 };

		// The stack profile in use. This implementation does only support PRO
		enum { stackProfile = profileZigBeePRO };

		// Accumulated PHR, MHR and MFR overheads for 802.15.4 frames
		// created by this network layer (nwkcMACFrameOverhead)
		enum { frameOverheadL2 = 11 };

		// The minimum number of octets added by the NWK layer to an NSDU
		// (nwkcMinHeaderOverhead)
		enum { minHeaderOverhead = 8 };

		// The maximum size of the auxiliary header introduced by security
		enum { maxSecurityHeaderOverhead = CZigBeeAuxiliaryHeader::maxSize,
			maxSecurityTrailerOverhead = 16 };

		// The maximum broadcast jitter time measured in symbol durations.
		// Notice that the core specification, starting with R20, expresses
		// this value (nwkcMaxBroadcastJitter) in octet durations, where for
		// the 250kbps 2.4GHz PHY one octet takes two PHY symbols.
		enum { maxBroadcastJitter = 0x7d0 << 1 };

		// Time duration in symbol durations, on the originator of a multicast
		// route request, between receiving a route reply and sending a message
		// to validate the route. Notice that the core specification, starting
		// with R20, expresses this value in octet durations, where for the
		// 250kbps 2.4GHz PHY one octet takes two PHY symbols.
		enum { waitBeforeValidation = 0x9c40 << 1 };

		// Time duration in symbol periods until a route discovery expires.
		// Notice that the core specification, starting with R20, expresses
		// this value in octet durations, where for the 250kbps 2.4GHz PHY one
		// octet takes two PHY symbols.
		enum { routeDiscoveryTime = 0x4c4b4 << 1 };

		// The number of times the broadcast transmission of a route
		// request command frame is retried on relay by an intermediate ZigBee
		// router or ZigBee coordinator
		enum { routeRequestRetries = 2 };

		// The number of milliseconds between retries of a broadcast route
		// request command frame. Notice that the core specification, starting
		// with R20, expresses this value in octet durations, where for the
		// 250kbps 2.4GHz PHY one octet takes two PHY symbols.
		enum { routeRequestRetryInterval = 0x1f02 << 1 };

		// The minimum jitter, in symbol durations, for broadcast
		// retransmission of a route request command frame. Notice that the
		// core specification, starting with R20, expresses this value in octet
		// durations, where for the 250kbps 2.4GHz PHY one octet takes two PHY
		// symbols.
		enum { minRouteRequestJitter = 0x3f << 1 };

		// The maximum jitter, in symbol durations, for broadcast
		// retransmission of a route request command frame. Notice that the
		// core specification, starting with R20, expresses this value in octet
		// durations, where for the 250kbps 2.4GHz PHY one octet takes two PHY
		// symbols.
		enum { maxRouteRequestJitter = 0xfa0 << 1 };

		// Concentrator flags
		enum { isConcentrator = 0x01, maintainsRouteCache = 0x02,
			concentratorRadiusMask = 0xf8, concentratorRadiusShift = 3 };

		// Address conflict detection flags
		enum { enableBroadcastAddressConflicts = 0x01,
			detectBroadcastAddressConflicts = 0x02 };

		// Flags that determine certain advanced device behavior
		enum { flagAdvertiseRouterCapabilitiesInReservedBits = 0x01 };

		// Router capabilities. The two LSBs might be mirrored to reserved bits
		// #16 (24h+ up-time) and #17 (hub connectivity) in the beacon info.
		// Other bits are available as TLVs in extended beacons in R23+
		enum { router24hPlusUptime = 0x01, routerHubConnectivity = 0x02,
			routerPreferredParent = 0x04 };

		// Virtual device capabilities
		enum { virtualDevice = 0x01, virtualTrafficOptimizations = 0x02 };

		// Advanced network connectivity maintenance flags. Poor parent link
		// quality and poor end-to-end connectivity are two criteria, which,
		// when constantly observed over a certain period of time, typically
		// 24 hours, will trigger a rejoin of an end-device. On the contrary,
		// poor end-to-end responsiveness would immediately trigger a rejoin
		// as long as not yet attempted once in the past over the same
		// period of time.
		// Notice that other failures, e.g. direct parent link failures, will
		// cause rejoins much sooner. Advanced parent link maintenance covers
		// more subtle long-term issues, which, when not dealt with, might
		// cause faster battery drain due to a larger number of retries, higher
		// latency, etc.
		enum { poorParentLinkQuality = 0x01, poorEndToEndConnectivity = 0x02,
			poorEndToEndResponsiveness = 0x04, advancedParentLinkCriteria = 0x07,
			advancedParentLinkTimerCriteria = 0x03,
			advancedParentLinkMaintenance = 0x80 };

	// Attributes
	public:
		// Channels with an energy level above this threshold will be ruled
		// out of the set of potential operating channels early in the network
		// formation process. This threshold should be set such that reflects
		// the energy level, which would result in MAC:CHANNEL_ACCESS_FAILURE
		// status in response to attempts to send a MAC frame
		static unsigned char nAcceptableEnergyLevel;

		// Potential parents with an incoming link quality assessment worse
		// than this value, will not be used for joining the network. A value
		// of 255 disables this check. The standard prescribes a value of 25
		static const uint8_t acceptableLinkQuality;

		// Limit on neighbor table size. Notice that a single unencrypted link
		// status can hold up to 31 neighbors. Security overhead reduces this
		// to 26 neighbors per link status message. Allowing three link status
		// messages in a row, this yields 78 neighbors
		static const unsigned int nNeighborTableLimit;

		// Number of concurrent network broadcasts supported by this device
		static const unsigned int nBroadcastTableLimit;

		// Number of route discovery table entries supported by this device
		static const unsigned int nRouteDiscoveryTableLimit;

		// Number of routing table entries supported by this device
		static const unsigned int nRoutingTableLimit;

		// Number of source route cache table entries supported by this device
		static const unsigned int nSourceRouteLimit;

		// Number of address mapping table entries supported by this device
		static const unsigned int nAddressMapLimit;

		// The security level for outgoing and incoming NWK frames. This is NIB
		// attribute 0xa0
		static __COMPACT_STOCK uint8_t nSecurityLevel;

		// The maximum time duration in ticks allowed for the parent and all
		// child devices to retransmit a broadcast message (passive
		// acknowledgment timeout). This is NIB attribute 0x82
		static const unsigned short nPassiveAcknowledgmentTimeout;

		// The maximum number of retries allowed after a broadcast transmission
		// failure. This is NIB attribute 0x83
		static const unsigned char nMaxBroadcastRetries;

		// The depth a device can have. This is NIB attribute 0x85
		static const unsigned char nMaxDepth;

		// Time duration in ticks that a broadcast message needs to encompass
		// the entire network. This is a calculated quantity based on other NIB
		// attributes. This is NIB attribute 0x88. Notice: Although the
		// standard specifies duration in seconds, this value is actually
		// specified in units of ticks (refer to CC2480 documentation and the
		// ZCD_NV_BCAST_DELIVERY_TIME configuration value)
		static const unsigned int nBroadcastDeliveryTime;

		// Time duration in ticks that a broadcast message's current expiration
		// timeout must exceed so that re-transmission could be initiated if
		// necessary. Once below this threshold, a broadcast message remains
		// maintained in the broadcast table till its expiration but won't
		// perform further transmission retries. This accounts for propagation
		// delays, in particular processing and queuing delays in neighboring
		// devices. Effectively, the retransmission window must be limited to
		// only a proportion of the broadcast table entry lifetime. This
		// threshold defines this proportion.
		static const unsigned int nBroadcastRetransmissionGuardTime;

		// If this is set to 0, the NWK layer shall calculate link cost from
		// all neighbor nodes using the LQI values reported by the MAC layer;
		// otherwise, it shall report a constant value. This is NIB attribute
		// 0x89
		static const bool bReportConstantCost;

		// The time in seconds between regular link status command frames.
		// This is NIB attribute 0xa6
		static const uint8_t nLinkStatusPeriod;

		// Timespan in timer ticks between initial link status command
		// frames. Applying a shorter period than the regular one could help
		// accelerating symmetric route establishment on existing router
		// neighbors towards the new router node.
		// This timespan is also used to throttle pro-active link status
		// messages (it is the minimum interval between such link status
		// frames).
		static const
		CCompactTimerService::ticks_diff_t startupLinkStatusPeriodTicks;

		// Timespan in timer ticks as base delay period for initiating an
		// reactive link status command frame as reply, upon receipt of an
		// empty link status frame from a router that is currently not being
		// maintained in the neighbor table. The reactive link status frame
		// reply would help accelerating symmetric route establishment on
		// the router that sent the empty link status frame.
		static const
		CCompactTimerService::ticks_diff_t reactiveLinkStatusPeriodTicks;

		// The number of missed link status command frames before resetting the
		// link costs to zero. This is NIB attribute 0xa7
		static const unsigned char nRouterAgeLimit;

		// A flag that determines whether the network layer should detect and
		// correct conflicting addresses. This is NIB attribute 0xa8
		static const bool bUniqueAddresses;

		// Indicates whether security shall be applied to incoming and outgoing
		// NWK data frames. If set to 0x01, security processing shall be
		// applied to all incoming and outgoing frames except data frames
		// destined for the current device that have the security sub-field of
		// the frame control field set to 0. If this attribute has a value of
		// 0x01, the NWK layer shall not relay frames that have the security
		// sub-field of the frame control field set to 0. The SecurityEnable
		// parameter of the NLDE-DATA.request primitive shall override the
		// setting of this attribute. This is NIB attribute 0xa5
		static const bool bSecureAllFrames;

		// This is NIB attribute 0xaa. This policy determines whether or not a
		// remote NWK leave request command frame received by the local device
		// is accepted
		static const bool bAcceptLeaveRequest;

		// The default timeout value for legacy end-devices (R21 draft)
		static const unsigned char nDefaultEndDeviceTimeout;

		// The value to use when negotiating an end-device timeout with an R21+
		// parent router. It is possible to dynamically re-negotiate another
		// timeout during operation using NegotiateTimeout()
		static const unsigned char nEndDeviceTimeout;

		// Parent capabilities. These are the capabilities of the device when
		// it operates as coordinator or router
		static const unsigned char bParentCapabilities;

		// The hop count radius for concentrator route discoveries. This is NIB
		// attribute 0x9e
		static const unsigned char nConcentratorRadius;

		// The time in seconds between concentrator route discoveries. If set
		// to 0x0000, the discoveries are done at start up and by the next
		// higher layer only. In this implementation, this value is specified in
		// symbol durations. This is NIB attribute 0x9f
		static const unsigned int nConcentratorDiscoveryTime;

		// Default advanced maintenance flags
		static const uint8_t defaultAdvancedMaintenanceFlags;

		// Set of network security material descriptors capable of maintaining
		// an active and alternate network key. This is NIB attribute 0xa1
		CZigBeeNetworkSecurityMaterial m_securityMaterial[2];

		// The current set of neighbor table entries in the device. Organized
		// as multiset with sorting criteria defined by CZigBeeNeighborInfo.
		// This is set of neighbors for normal network operation, i.e. once
		// the device is commissioned. This is NIB attribute 0x87
		std::multiset<CZigBeeNeighborInfo> m_neighbors;

		// The current set of routing table entries in the device. This is NIB
		// attribute 0x8b
		std::set<CZigBeeRoute> m_routes;

		// If this device is operating as a concentrator, this is the current
		// set of source routing table entries in the device. This is a series
		// of linked source route entries of type CZigBeeSourceRoute.
		// This is NIB attribute 0x9c.
		std::set<CZigBeeSourceRoute> m_sourceRoutes;

		// The current set of broadcast transaction table entries in the
		// device. This is NIB attribute 0x98, read-only
		std::set<CZigBeeBroadcastTransaction> m_broadcasts;

		// The current set of 64-bit IEEE to 16-bit network address mappings.
		// In this implementation, the preferred mapping direction is 16-bit
		// network address to 64-bit IEEE address, though. This is NIB
		// attribute 0xa9
		CZigBeeNetworkAddressMap m_addressMap;

		// A set of global (network-wide) values, which the local device shall
		// advertise in its beacon payload, encoded as global TLVs
		CPacket m_globalAdvertisements;

		// A set of local (device-specific) values, which the local device
		// shall advertise in its beacon payload, encoded as global TLVs
		CPacket m_localAdvertisements;

		// The extended PAN identifier for the PAN of which the device is a
		// member. The value 0x0000000000000000 means the extended PAN
		// identifier is unknown, i.e. the node is not joined to a network.
		// This is NIB attribute 0x9a
		unsigned long long m_qwExtendedPANID;

		// The 16-bit address of the designated network channel manager.
		// This is NIB attribute 0x92 and defaults to 0x0000 in a centralized
		// security network and 0xffff (no network manager) in a distributed
		// security network
		unsigned short m_wManagerAddress;

		// The 16-bit PAN identifier that is eligible for a network update.
		// If this value equals 0xffff, the network layer will accept a PAN
		// identifier change to an arbitrary value, otherwise it will reject
		// network update commands attempting to move the network to a PAN
		// identifier that is different than the value stored in this NIB
		// attribute
		unsigned short m_wAcceptablePANID;

		// The channel number that is eligible for a network update. If this
		// value equals 0xff, the network layer will accept a channel change
		// to an arbitrary channel, otherwise it will reject channel change
		// commands attempting to move the network to a channel that is
		// different than the value stored in this NIB attribute
		unsigned char m_nAcceptableChannel;

		// A sequence number used to identify outgoing frames (see sub-clause
		// 3.6.2). This is NIB attribute 0x81, read-only
		unsigned char m_nSequence;

		// A count of unicast transmissions made by the NWK layer on this
		// device. Each time the NWK layer transmits a unicast frame, by
		// invoking the MCPS-DATA.request primitive of the MAC sub-layer, it
		// shall increment this counter. When either the NHL performs an
		// NLME-SET.request on this attribute or if the value of nwkTxTotal
		// rolls over past 0xffff the NWK layer shall reset to 0x00 each
		// Transmit Failure field contained in the neighbor table. This is NIB
		// attribute 0x8d
		unsigned short m_nTransmitTotal;

		// The first element of the pair indicates the total number of PAN ID
		// conflicts that have been seen by the local device. This value can
		// be reset to 0 by the next higher layer. It is a saturating counter.
		// The second element of the pair stores the time since the last reset
		std::pair<uint16_t, uint16_t> m_panidConflicts;

		// The current route symmetry setting: True means that routes are
		// considered to be comprised of symmetric links. Backward and forward
		// routes are created during one-route discovery and they are
		// identical. False indicates that routes are not considered to be
		// comprised of symmetric links. Only the forward route is stored
		// during route discovery. For ZigBee PRO this attribute is true, for
		// the simpler (legacy) ZigBee it is false. This is NIB attribute 0x8e
		// The value of this attribute is always TRUE in this implementation
		//bool m_bSymmetricLinks;

		// This field shall contain the device capability information
		// established at network joining time.
		// This is NIB attribute 0x8f, read-only
		unsigned char m_bCapabilities;

		// A value that determines the method used to assign addresses.
		// This is NIB attribute 0x90
		// Always stochastic in ZigBee PRO
		// unsigned char m_nAddressAllocation;

		// The value identifying a snapshot of the network settings this node
		// is operating with. This is NIB attribute 0x94
		unsigned char m_nUpdateID;

		// The sequence number of the active network key in
		// nwkSecurityMaterialSet. This is NIB attribute 0xa2
		unsigned char m_nActiveKeySequenceNumber;

		// True, if the device is properly authenticated (according to the
		// network's security policy). Equivalent to bdbDeviceIsOnANetwork
		bool m_bAuthenticated;

		// Parent capabilities. If this is a coordinator or router, these are
		// the capabilities of this device; if this is an end-device, then the
		// parent's capabilities are accessible here
		unsigned char m_bParentCapabilities;

		// Set of flags that allows configuration of certain network layer
		// behavior
		uint8_t m_nFlags;

		// Router capabilities (applies to coordinators and routers). This is a
		// combination of features that are supported by the local device and
		// should be advertised in beacons. In ZigBee R23 and beyond these will
		// be advertised using a tag-length-value (TLV) scheme; some of these
		// bits (trust center connectivity and long up-time) might also be
		// advertised in reserved bits #16 (24h+ up-time) and #17 (hub
		// connectivity) of beacon info field in R22 and earlier beacons
		uint16_t m_routerCapabilities;

		// Advanced maintenance flags
		uint8_t m_advancedMaintenanceFlags;

		// The 16-bit address that the device uses to communicate with the
		// PAN. This attribute reflects the value of the MAC PIB attribute
		// macShortAddress and any changes made by the higher layer will be
		// reflected in the MAC PIB attribute value as well. This is NIB
		// attribute 0x96
		// unsigned short m_wNetworkAddress;

		// A flag determining if this device is a concentrator. Extensions by
		// ubisys distinguish high-RAM (route cache) and low-RAM (no route
		// cache) concentrators. Also the concentrator radius is included.
		// This is an enhanced version of NIB attribute 0x9d.
		unsigned char m_bConcentrator;

		// A flag that enables address conflict detection based on the fact
		// whether or not a broadcast transaction record (BTR) exists when a
		// device receives a broadcast message with the source equal to its
		// own address. Even if enabled, detection is suppressed for a
		// broadcast delivery time after network start-up to avoid false
		// detection, when a device hears rebroadcasts of its own broadcast
		// shortly after reboot
		static unsigned char bAddressConflictDetection;

		// Poll rate in normal operating mode, when the device is not expecting
		// unsolicited inbound traffic. Specified in timer ticks of 16us. A
		// value of null means the device does not poll regularly. Only applies
		// to sleeping end-devices
		static CCompactTimerService::ticks_diff_t normalPollInterval;

		// Poll rate in responsive mode, when the device is expecting inbound
		// traffic, for example during trust center link-key exchange, while
		// waiting for APS acknowledgments, etc. Specified in timer ticks of
		// 16us. This value must in a range of 100ms to 7.5seconds, with a
		// recommended rate of 250ms. Only applies to sleeping end-devices
		static CCompactTimerService::ticks_diff_t responsivePollInterval;

		// Number of ticks (symbol periods) to wait before attempting a retry
		// during polling due to a MAC layer failure. Here 250ms. The initial
		// attempt is always immediate
		static unsigned int pollRetryDelayTicks;

		// Number of ticks (symbol periods) to wait before attempting a
		// network-level retry due to a MAC layer failure. Base value.
		// Defaults to 250ms for the first retry, and consequently 500ms,
		// 750ms for second and third retries, respectively. The initial
		// attempt is always immediate
		static const unsigned int dataRetryDelayTicks;

		// Number of poll attempts that will be made, in a row, in case that
		// network-layer instigated polling fails
		static const uint8_t pollAttemptLimit;

		// Number of network-level retries that will be made, in a row, in
		// case that a MAC data request fails
		static const uint8_t dataAttemptLimit;

		// Maximum number of retries that will be made, in a row, in case
		// that end device timeout negotiation fails due to missing response
		static const uint8_t timeoutNegotiationAttemptLimit;

	// Operations
	public:
		// Sets default values for network variables
		void SetDefaults();

		// Updates the nwkBroadcastDeliveryTime. Must be called each
		// time either of nwkMaxDepth, nwkPassiveAckTimeout or
		// nwkBroadcastRetries has been modified
		void UpdateBroadcastDeliveryTime();

		// Must be called when either the NHL performs an NLME-SET.request
		// on nwkTxTotal or if the value of nwkTxTotal rolls over past 0xffff
		void AfterTransmitTotalRollover();

		// Modifies the security level (persistently)
		static void SetSecurityLevel(const unsigned char nLevel);

		// Modifies the extended address (persistently)
		static void SetExtendedAddress
			(const unsigned long long qwExtendedAddress);

#ifdef _DEBUG
	public:
		// In debug builds, shows the values of all NIB attributes
		void Dump() const;

		// In debug builds, shows the contents of the neighbor table
		void DumpNeighborTable() const;

		// In debug builds, shows the contents of the address map
		void DumpAddressMap() const;

		// In debug builds, shows the contents of the broadcast transaction
		// table
		void DumpBroadcastTransactionTable() const;

		// In debug builds, shows the contents of the routing table
		void DumpRoutingTable() const;

		// In debug builds, shows the contents of the source route cache
		void DumpSourceRoutes() const;

		// In debug builds, shows the contents of the child address cache
		void DumpChildAddressCache() const;

		// Validates a neighbor table iterator
		bool IsValid(const std::multiset<CZigBeeNeighborInfo>::const_iterator
			&neighbor) const;

		// Validates a broadcast transaction table entry
		bool IsValid(const CZigBeeBroadcastTransaction &btr);
#endif

	// Implementation
	public:
		// Child address cache. This cache contains the 16-bit short network
		// addresses of all current child devices in a sorted array, which can
		// be quickly tested for a particular address using e.g. binary search.
		// The set will be used to distinguish active end-devices, which should
		// not receive a leave message when polling
		std::vector<unsigned short> m_childAddressCache;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeStartupChannelInformation

class CZigBeeStartupChannelInformation
{
	// Construction
	public:
		CZigBeeStartupChannelInformation(const unsigned char nChannel,
			const unsigned char nEnergyLevel);

		CZigBeeStartupChannelInformation
			(const CZigBeeStartupChannelInformation &info);

	// Attributes
	public:
		// Logical channel
		unsigned char m_nChannel;

		// Energy level
		unsigned char m_nEnergyLevel;

		// A set of PAN descriptors for each IEEE 802.15.4 device on this
		// channel. Includes ZigBee and other IEEE 802.15.4 devices that
		// respond to beacon request commands
		std::list<CIEEE802154PANDescriptor> m_devices;

	// Operations
	public:
		// Less operator (STL sorted container support)
		bool operator<(const CZigBeeStartupChannelInformation &info) const;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkFormationContext

class CZigBeeNetworkFormationContext
{
	// Construction
	public:
		CZigBeeNetworkFormationContext(const unsigned int nScanChannels,
			const unsigned char nScanDuration, const unsigned char nBeaconOrder,
			const unsigned char nSuperframeOrder,
			const bool bBatteryLifeExtension, const bool bFormAsRouter,
			std::set<CZigBeeStartupChannelInformation> *const pChannelsEx = 0,
			const bool bForceFormation = false,
			const unsigned long long qwExtendedPANID = 0ull);

	// Attributes
	public:
		// Set of acceptable channels that have passed an initial energy
		// detection scan, sorted according to increasing energy level
		std::set<CZigBeeStartupChannelInformation> m_channels;

		// If non-zero, the network layer keeps collecting information about
		// scanned channels across different network formation attempts, each
		// potentially covering separate channel sets. Notice that this is not
		// reset by an NLME-RESET.request and the higher layer is responsible
		// for configuring, allocating and releasing this object
		std::set<CZigBeeStartupChannelInformation> *const m_pChannelsEx;

		// The 64-bit network identifier suggested for the network
		unsigned long long m_qwExtendedPANID;

		// Set of acceptable channels, i.e. those channels where the energy was
		// below the black-listing threshold for new networks
		unsigned int m_nAcceptableChannels;

		// The 16-bit network identifier suggested for the new network
		unsigned short m_wPANID;

		// The 16-bit network address suggested for the forming device
		unsigned short m_wShortAddress;

		// Holds the value of the ScanChannels parameter passed to the
		// NLME-NETWORK-FORMATION.request
		const unsigned int m_nScanChannels;

		// Holds the value of the ScanDuration parameter passed to the
		// NLME-NETWORK-FORMATION.request
		const unsigned char m_nScanDuration;

		// Holds the value of the BeaconOrder parameter passed to the
		// NLME-NETWORK-FORMATION.request
		const unsigned char m_nBeaconOrder;

		// Holds the value of the SuperFrame parameter passed to the
		// NLME-NETWORK-FORMATION.request
		const unsigned char m_nSuperframeOrder;

		// Holds the value of the BatteryLifeExtension parameter passed to the
		// NLME-NETWORK-FORMATION.request
		const bool m_bBatteryLifeExtension;

		// Form a network as router. This can be used by the APSME to form a
		// distributed security network
		const bool m_bFormAsRouter;

		// If true, network formation will not fail, even if there is not a
		// single channel below the threshold. This can be used to implement a
		// fall-back mechanism, which still allows to form networks in hostile
		// RF environments
		const bool m_bForceFormation;

		// The channel to form the network on
		unsigned char m_nChannel;

	// Operations
	public:
		// Adds active scan information to m_channels and m_pChannelsEx, as far
		// as applicable
		void AugmentChannels(const CIEEE802154PANDescriptor &descriptor);

	// Implementation
	public:
		// Augments a specific channel collection with device information
		static void AugmentChannels(const CIEEE802154PANDescriptor &descriptor,
			std::set<CZigBeeStartupChannelInformation> &channels);
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkScanManager

class CZigBeeNetworkScanManager
{
	// typedefs & enums
	public:
		// The type of scan that is being performed by this manager. Notice
		// that specialized, derived manager classes may be used for each of
		// these scans if need be
		enum { energyDetection, networkDiscovery, formation, join, rejoin,
			orphan, temporaryChannelChange };

		// Additional flags that control the scan manager behavior
		enum { filterPermitJoining = 0x01, filterEndDeviceCapacity = 0x02,
			filterRouterCapacity = 0x04, scanCompleted = 0x20,
			autoDiscardScanResults = 0x40, autoDelete = 0x80 };

		// Reasons to reject a beacon
		enum { payloadTooShort, sourceAddressingTypeMismatch,
			protocolMismatch1, protocolMismatch2, protocolMismatch3,
			foreignZigbeeNetwork, filterMismatchPermitJoining,
			filterMismatchEndDeviceCapacity, filterMismatchRouterCapacity,
			filterApplicationSpecific };

		// Completion handler for scans
		typedef void (CZigBeeNetworkManagement::*COMPLETECALLBACK)
			(const unsigned char nStatus, const unsigned char nScanType,
			const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// Container for scan results
		typedef std::multiset<CZigBeeNeighborInfo, CZigBeeParentRanking>
			results_t;

	// Construction
	public:
		CZigBeeNetworkScanManager(CZigBeeNetworkManagement &nlme,
			const uint_fast8_t type, const COMPLETECALLBACK pfnOnComplete,
			const uint_fast64_t filterPANID = 0,
			const uint_fast8_t options = autoDelete,
			const CIEEE802154Address &preferredAddress = CIEEE802154Address());

	// Attributes
	public:
		// The NLME that this scan context belongs to
		CZigBeeNetworkManagement &m_nlme;

		// A completion handler for this scan. Notice that the scan manager
		// will not be automatically deleted, such that it may be reused for
		// additional scans
		COMPLETECALLBACK m_pfnOnComplete;

		// If this EPID is different from zero, it will be used to filter
		// beacon frames (for example during an active scan initiated by a
		// network rejoin request)
		uint64_t m_filterPANID;

		// Identifies what this scan manager is being used for, exactly.
		// For example, if it is being used for an energy scan or a network
		// discovery scan in advance of a network join or rejoin, etc.
		uint8_t m_type;

		// Specific scan manager behavior. Allows the scan manager to exist
		// beyond the point where the completion handler will be invoked;
		// and allows to keep scan entries around in the neighbor table,
		// such that the completion handler would have a chance to look at
		// each one and reorder/reprioritize potentially based on higher
		// level application knowledge
		uint8_t m_options;

		// Ranking comparator object used for sorting the received scan
		// results in descending order
		CZigBeeParentRanking m_ranking;

		// A set of progressively updated scan results, also referred to as the
		// discovery table in R23 and beyond. This is very similar to the set
		// of neighbors with two distinctions: (1) ranking is different than
		// the one used during normal operation; (2) extended information is
		// available for these devices, which is typically discarded once the
		// device is operating normally, i.e. has joined a particular network
		results_t m_results;

		// Maximum size of the discovery table
		static const results_t::size_type resultLimit;

		// Maximum number of parent routers that should be attempted during
		// an active rejoin process.
		static const uint8_t attemptLimit;

		// Maximum number of retries that should be conducted per potential
		// router if retry is deliberately intended
		static const uint8_t retryLimit;

	// Operations
	public:
		// Searches the discovery table for the specified device, based on its
		// network or extended address and its logical channel
		std::multiset<CZigBeeNeighborInfo, CZigBeeParentRanking>::iterator
		FindDevice(const CIEEE802154Address &address,
			const uint_fast8_t logicalChannel);

		// Retain results from a previous scan. This will move the scan
		// results provided by the caller to the result set of this instance.
		// This can be a result set separately maintained by the application,
		// or one gathered by a previously used scan manager instance.
		void AssignResults(results_t &results, const bool move = true);

	// Overridables
	public:
		// This is a generic handler for MLME-SCAN.confirm. It will invoke the
		// completion handler registered by the caller
		virtual void OnConfirmScan(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// This is a generic handler for MLME-BEACON-NOTIFY.indication
		virtual void OnIndicateBeacon(const unsigned char nSequence,
			const CIEEE802154PANDescriptor &descriptor, CPacket &sdu);

		// This overridable is invoked by the generic handler for all devices
		// that should be added to the set of discovered devices during a
		// particular scan. The default adds these devices to the neighbor
		// table as extended entries. It is possible to override this method
		// and store these entries elsewhere, for example
		virtual void OnDiscarded(const CIEEE802154PANDescriptor &descriptor,
			CPacket &sdu, const uint_fast8_t reason);

		// This overridable is invoked by the generic handler for all devices
		// that should be added to the set of discovered devices during a
		// particular scan. The default adds these devices to the neighbor
		// table as extended entries. It is possible to override this method
		// and store these entries elsewhere, for example
		virtual void OnDiscovered(CZigBeeNeighborInfo &info);

		// This method serves as an application-level hook to reject beacons,
		// which otherwise would qualify from a core stack perspective, for
		// application-specific reasons. For example, an device might have
		// joined that network earlier and determined that it is not suitable
		// for the application and should not be reattempted at this point
		virtual bool EvaluateApplicationFilters
			(const CIEEE802154PANDescriptor &descriptor,
			const CPacket &sdu) const;

	// Implementation
	public:
		virtual ~CZigBeeNetworkScanManager();

		// Number of parent routers that have been attempted during an
		// ongoing rejoin procedure
		uint8_t m_attemptCounter;

#ifdef _DEBUG
		// Dump scan manager state and current scan result set
		virtual void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkFormationScanManager

class CZigBeeNetworkFormationScanManager : public CZigBeeNetworkScanManager
{
	// typedefs & enums
	public:
		typedef bool (*BEFOREFINISHCALLBACK)(CZigBeeNetworkManagement &nlme,
			CZigBeeNetworkFormationContext &context);

	// Construction
	public:
		CZigBeeNetworkFormationScanManager(CZigBeeNetworkManagement &nlme,
			const COMPLETECALLBACK pfnOnComplete,
			const unsigned int nScanChannels, const unsigned char nScanDuration,
			const unsigned char nBeaconOrder,
			const unsigned char nSuperframeOrder,
			const bool bBatteryLifeExtension, const bool bFormAsRouter,
			std::set<CZigBeeStartupChannelInformation> *const pChannelsEx = 0,
			const bool bForceFormation = false,
			const unsigned long long qwExtendedPANID = 0ull,
			const BEFOREFINISHCALLBACK pfnBeforeFinish = 0);

	// Attributes
	public:
		// If this scan manager instance is used during network formation,
		// additional formation context is available in this member
		CZigBeeNetworkFormationContext m_context;

		// Optional callback allowing finishing touches on network formation,
		// e.g. tweaking final network parameters like 16- and 64-bit network
		// identifiers, the operating channel, or the forming devices network
		// short address. If the callback returns false, network formation is
		// suspended and either needs to be resumed by a call to
		// FinishNetworkFormation() or terminated by calling
		// TerminateNetworkFormation(). At the time this callback is invoked,
		// the operating parameters have not been altered yet, but the new,
		// suggested network settings (after energy and active scans) are
		// available in the supplied start-up information object
		BEFOREFINISHCALLBACK m_pfnBeforeFinish;

	// Overrides
	public:
		// This is a handler for MLME-BEACON-NOTIFY.indication specifically
		// for network formation
		virtual void OnIndicateBeacon(const unsigned char nSequence,
			const CIEEE802154PANDescriptor &descriptor, CPacket &sdu);
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkSurveyScanManager

class CZigBeeNetworkSurveyScanManager : public CZigBeeNetworkScanManager
{
	// Construction
	public:
		CZigBeeNetworkSurveyScanManager(CZigBeeNetworkManagement &nlme,
			const uint_fast8_t type, const COMPLETECALLBACK pfnOnComplete,
			const uint_fast64_t filterPANID = 0,
			const uint_fast8_t options = 0);

	// Attributes
	public:
		// Total number of beacons surveyed
		uint8_t m_totalBeacons;

		// Total number of beacons on the current Zigbee network
		uint8_t m_onNetworkBeacons;

		// Number of potential parent routers on the current Zigbee network
		uint8_t m_potentialParents;

		// Number of router devices on foreign Zigbee networks in the vicinity
		uint8_t m_foreignZigbeeDevices;

	// Overridables
	public:
		// This is a generic handler for MLME-BEACON-NOTIFY.indication
		virtual void OnIndicateBeacon(const unsigned char nSequence,
			const CIEEE802154PANDescriptor &descriptor, CPacket &sdu);

		// This overridable is invoked by the generic handler for all devices
		// that should be added to the set of discovered devices during a
		// particular scan. The default adds these devices to the neighbor
		// table as extended entries. It is possible to override this method
		// and store these entries elsewhere, for example
		virtual void OnDiscarded(const CIEEE802154PANDescriptor &descriptor,
			CPacket &sdu, const uint_fast8_t reason);

		// This overridable is invoked by the generic handler for all devices
		// that should be added to the set of discovered devices during a
		// particular scan. The default adds these devices to the neighbor
		// table as extended entries. It is possible to override this method
		// and store these entries elsewhere, for example
		virtual void OnDiscovered(CZigBeeNeighborInfo &info);
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkManagement

class CZigBeeNetworkManagement : public CZigBeeNetworkClient,
	public CCompactTimerClient
{
	friend class CZigBeeNetwork;
	friend class CZigBeeNetworkPersistentData;

	// typedefs & enums
	public:
		// Possible values for the RejoinNetwork parameter used in the
		// NLME-JOIN.request service primitive and neighbor table entries
		// joinExNetworkDiscovery indicates that the entry has been created
		// from a beacon during network discovery. joinExLinkStatus indicates
		// that the neighbor table entry has been added in response to a link
		// status command frame received from the device, joinExAuthenticate
		// indicates that a valid secure incoming message has lead to the
		// creation of this entry and only the incoming frame counter is valid.
		// joinExPersistent means that the entry has been created due to child
		// information being restored from persistent storage
		enum { joinViaAssociation, joinViaOrphaning, joinViaNetworkRejoin,
			joinChangeChannel, joinExNetworkDiscovery = 0x80,
			joinExLinkStatus, joinExAuthenticate, joinExPersistent };

		// Possible address allocation strategies
		enum { allocationDistributed, allocationReserved,
			allocationStochastic };

		// Capabilities regarding child-aging
		enum { capabilityPollKeepAlive = 0x01,
			capabilityTimeoutRequestKeepAlive = 0x02 };

		// Type of link status transmissions. "regular" refers to a regular
		// periodical transmission; "initial" refers to the initial empty
		// transmission after start-up; "reactive" refers to a reactive
		// (irregular) transmission which is meant to be an reactive reply,
		// upon receipt of an empty link status frame from a by then unknown
		// neighbor node
		enum { linkStatusRegular, linkStatusInitial, linkStatusReactive };

		// Optional application-level completion handler for a leave request
		typedef void (CZigBeeApplication::*ONLEAVECOMPLETE)
			(const uint_fast8_t nStatus, const CZigBeeAddress &device,
			const bool bRejoin, void *pArgument);

	// Construction
	public:
		CZigBeeNetworkManagement(CZigBeeNetwork &nwk);

	// Attributes
	public:
		// The network layer instance this NLME belongs to
		CZigBeeNetwork &m_nwk;

		// ZigBee network layer information base (NIB)
		CZigBeeNetworkInformationBase m_nib;

		// Sets the MAC layer capabilities (FFD vs. RFD) and PAN coordinator
		void SetCapabilities(const unsigned char bCapabilities);

		// Returns true, if the device is currently joined to a network, i.e.
		// nwkExtendedPANID is non-zero
		bool IsJoined() const;

		// Returns true, if this node is a member of the group specified in the
		// wAddress parameter
		bool IsGroupMember(const unsigned short wAddress) const;

		// Returns true, if the present device is the ZigBee coordinator
		bool IsCoordinator() const;

		// Returns true, if the present device is a ZigBee router
		// (or coordinator)
		bool IsRouter() const;

		// Returns true, if the present device is a ZigBee end device
		bool IsEndDevice() const;

		// Returns true, if the present device has its receiver turned on
		// during idle periods
		bool IsReceiverOnWhenIdle() const;

		// Returns true, if this device is the designated network manager
		bool IsNetworkManager() const;

		// Returns true, if this is a Zigbee Virtual Device (ZVD), in the sense
		// that the device itself does not have a native IEEE 802.15.4 MAC/PHY
		// available. For example, if the stack is running as an app on a smart
		// phone, smart speaker or computer and connects to a real 15.4 Zigbee
		// network via a Zigbee Direct Device (ZDD), an intermediary dual-stack
		// Zigbee/BLE proxy
		bool IsVirtualDevice() const;

		// Returns true if the supplied key type should be considered a network
		// key, including any network key surrogates understood by virtual
		// devices
		bool IsNetworkKey(const uint_fast8_t keyType) const;

		// Returns zero for a standard network key; else if the current active
		// network key is ephemeral returns the corresponding type (unique vs.
		// global). Ephemeral keys are provided to a Zigbee Virtual Device when
		// it is joining a legacy network for the first time. Notice that the
		// traffic that can be exchanged with an ephemeral key is heavily
		// restricted and is conceptually limited to allow a establishing a
		// unique key with the trust center
		uint_fast8_t GetEphemeralKeyType();

	// Operations
	public:
		// NLME-NETWORK-DISCOVERY.request(ScanChannels, ScanDuration), clause
		// 3.2.2.1
		void OnRequestNetworkDiscovery(const unsigned int nScanChannels,
			const unsigned char nScanDuration,
			const uint_fast8_t options = CZigBeeNetworkScanManager::autoDelete,
			CZigBeeNetworkScanManager *const pScanManager = 0);

		// NLME-NETWORK-FORMATION.request(ScanChannels, ScanDuration,
		// BeaconOrder, SuperframeOrder, BatteryLifeExtension), clause 3.2.2.3
		void OnRequestNetworkFormation(const unsigned int nScanChannels,
			const unsigned char nScanDuration, const unsigned char nBeaconOrder,
			const unsigned char nSuperframeOrder,
			const bool bBatteryLifeExtension, const bool bFormAsRouter = false,
			std::set<CZigBeeStartupChannelInformation> *const pChannelsEx = 0,
			const bool bForceFormation = false,
			const CZigBeeNetworkFormationScanManager::BEFOREFINISHCALLBACK
				pfnBeforeFinish = 0);

		// Direct call version of an NLME-PERMIT-JOINING.request(PermitDuration),
		// clause 3.2.2.5, resulting in an immediate NLME-PERMIT-JOINING.confirm,
		// clause 3.2.2.6
		unsigned char PermitJoining(const unsigned char nPermitDuration);

		// NLME-START-ROUTER.request(BeaconOrder, SuperframeOrder,
		// BatteryLifeExtension), clause 3.2.2.7
		void OnRequestStartRouter(const unsigned char nBeaconOrder,
			const unsigned char nSuperframeOrder,
			const bool bBatteryLifeExtension);

		// NLME-ED-SCAN.request(ScanChannels, ScanDuration), clause 3.2.2.9
		void OnRequestEnergyDetectionScan(const unsigned int nScanChannels,
			const unsigned char nScanDuration,
			const uint_fast8_t options = CZigBeeNetworkScanManager::autoDelete,
			CZigBeeNetworkScanManager *const pScanManager = 0);

		// NLME-JOIN.request(ExtendedPANID, RejoinNetwork, ScanChannels,
		// ScanDuration, CapabilityInformation, SecurityEnable),
		// clause 3.2.2.11. Notice: Don't use this to change channels. This
		// would always result in INVALID_REQUEST. To change channels, use the
		// direct call version ChangeChannel()
		void OnRequestJoin(const unsigned long long qwExtendedPANID,
			const unsigned char nRejoinNetwork,
			const unsigned int nScanChannels, const unsigned char nScanDuration,
			const unsigned char bCapabilities,
			const bool bEnableSecurity = false);

		// Direct call version of NLME-JOIN.request(..., RejoinNetwork = 0x03,
		// ...), i.e. change channel. The tentative flag allows to probe if
		// a channel change with the specified channel mask would succeed or
		// return a failure status. If the "verify acceptable" option is set,
		// the NIB value m_nAcceptableChannel will be
		unsigned char ChangeChannel(const unsigned int nScanChannels,
			const bool bTentative = false, const bool bVerifyAcceptable = false);

		// NLME-LEAVE.request(DeviceAddress, RemoveChildren, Rejoin),
		// clause 3.2.2.16. Optionally, a CZigBeeApplication member function
		// can be invoked with the status of the leave request
		void OnRequestLeave(const CZigBeeAddress &device,
			const bool bRemoveChildren, const bool bRejoin,
			CZigBeeApplication *const pApplication = 0,
			const ONLEAVECOMPLETE pfnOnComplete = 0,
			void *const pArgument = 0);

		// Direct call version of an NLME-RESET.request(WarmStart),
		// clause 3.2.2.19, resulting in an immediate
		// NLME-RESET.confirm(SUCCESS), clause 3.2.2.20
		void Reset(const bool bWarmStart);

		// NLME-SYNC.request(Track = FALSE)
		void OnRequestSynchronization(const bool bForce = false);

		// NLME-ROUTE-DISCOVERY.request(DstAddrMode, DstAddr, Radius,
		// NoRouteCache), clause 3.2.2.31
		void OnRequestRouteDiscovery(const CZigBeeAddress &destination,
			const unsigned char nRadius, const bool bNoRouteCache = false);

		// Selects a suitable 16-bit network identifier (PAN-ID) for the
		// network, avoiding any of the supplied network identifiers and
		// preferring the supplied one, if any
		static unsigned short ChoosePANID(const std::set<unsigned short> &ids,
			const unsigned short wPreferred = CZigBeeAddress::wBroadcastAll);

		// Instructs the NLME to switch to the specified PAN-ID after a while.
		// If the originate flag is set, the local NLME instance will broadcast
		// a network update command first, before scheduling its own update
		void OnNetworkUpdate(const unsigned short wPANID,
			const bool bOriginate = false);

		// MLME-BEACON-NOTIFY.indication(BSN, PANDescriptor, PendAddrSpec,
		// AddrList, sduLength, sdu), clause 7.1.5.1
		void OnIndicateBeaconNotify(const unsigned char nSequence,
			const CIEEE802154PANDescriptor &descriptor, CPacket &sdu);

		// MLME-SCAN.confirm(status, ScanType, ChannelPage, UnscannedChannels,
		// ResultListSize, EnergyDetectList, PANDescriptorList),
		// clause 7.1.11.2
		void OnConfirmScan(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// MLME-START.confirm(status), clause 7.1.14.2
		void OnConfirmStart(const unsigned char nStatus);

		// MLME-ASSOCIATE.confirm(AssocShortAddress, status, SecurityLevel,
		// KeyIdMode, KeySource, KeyIndex), clause 7.1.3.4
		void OnConfirmAssociate(const unsigned short wShortAddress,
			const unsigned char nStatus,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

#ifdef __COMPACT15DOT4_FFD__
		// MLME-ASSOCIATE.indication(DeviceAddress, CapabilityInformation,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.3.2
		void OnIndicateAssociate(const CIEEE802154Address &device,
			const unsigned char bCapabilities,
			CIEEE802154SecurityInfo *const pSecurity = 0);
#endif

		// MLME-DISASSOCIATE.confirm(status, DeviceAddrMode, DevicePANId,
		// DeviceAddress)
		void OnConfirmDisassociate(const unsigned char nStatus,
			const CIEEE802154Address &device);

		// MLME-DISASSOCIATE.indication(DeviceAddress, DisassociateReason,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.4.2
		void OnIndicateDisassociate(const CIEEE802154Address &device,
			const unsigned char nReason,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

#ifdef __COMPACT15DOT4_FFD__
		// MLME-ORPHAN.indication(OrphanAddress, SecurityLevel, KeyIdMode,
		// KeySource, KeyIndex)
		void OnIndicateOrphan(const CIEEE802154Address &orphan,
			const CIEEE802154SecurityInfo *const pSecurity = 0);
#endif

		// MLME-COMM-STATUS.indication(PANId, SrcAddrMode, SrcAddr,
		// DstAddrMode, DstAddr, status, SecurityLevel, KeyIdMode, KeySource),
		// clause 7.1.12.1
		void OnIndicateCommStatus(const CIEEE802154Address &source,
			const CIEEE802154Address &destination, const unsigned char nStatus,
			CIEEE802154SecurityInfo *const pSecurity = 0);

		// Returns the parent's network address. Returns a void address,
		// if there is currently no parent device in the neighbor table
		CZigBeeAddress GetParentAddressEx() const;

		// Returns the parent's network address. Returns the broadcast address,
		// if there is currently no parent device in the neighbor table
		unsigned short GetParentAddress() const;

		// Modify parent's flags. Clears the flags specified in the second
		// argument and sets the ones in the first argument.
		bool ModifyParentFlags(const uint_fast16_t set,
			const uint_fast16_t clear);

		// Tries to map a 16-bit network address to an extended 64-bit address
		// by searching neighbor table and address map
		bool LookupAddress(const uint16_t wNetworkAddress,
			uint64_t &qwExtendedAddress);

		// Tries to map a 64-bit extended address to a 16-bit address by
		// searching neighbor table and address map. Notice that the reverse
		// lookup is generally associated with greater computational cost
		bool LookupAddress(const uint64_t qwExtendedAddress,
			uint16_t &wNetworkAddress);

		// Adds a 16-bit network address to 64-bit IEEE address mapping. If the
		// mapping is added explicitly, an address map entry will always be
		// created. Otherwise, if a neighbor table entry exists, that will be
		// used. In case of encountering an address conflict, related resolution
		// may be optionally disabled. It is also possible to perform address
		// conflict detection only without adding a new address mapping entry.
		bool AddAddressMapping(const CIEEE802154Address &address,
			const bool bExplicit = false,
			const bool allowConflictResolution = true,
			const bool detectConflictOnly = false);

		// Invalidates a 16-bit network address to 64-bit IEEE address mapping
		// for every occurrence of this mapping
		void InvalidateAddressMapping(const CIEEE802154Address &address);

		// Given an address object with either a short or extended address,
		// this method completes the address object with the missing type of
		// address, if it is currently known. It does not initiate any address
		// lookups, however, which is left to higher layers, if required.
		// Returns true, if the address object is complete
		bool AugmentAddress(CIEEE802154Address &address);

		// Returns true, if the device whose capability information is given
		// is a ZigBee router (or coordinator), false otherwise
		static bool IsRouter(const unsigned char bCapabilities);

		// Returns true, if the device whose capability information is given
		// is a ZigBee end device, or a router that is operating as an end
		// device
		static bool IsEndDevice(const unsigned char bCapabilities);

		// Returns true, if the device whose capability information is given
		// has its receiver turned on during idle periods
		static bool IsReceiverOnWhenIdle(const unsigned char bCapabilities);

		// Returns the remaining time (in seconds), this device will allow
		// other devices to join
		unsigned char GetPermitDuration() const;

		// Should be called after network discovery has been completed and
		// extended neighbor information is no longer required
		void ReleaseExtendedNeighborInformation();

		// Appends one or more neighbor table entries in ZDO compatible format
		// to a packet. Can be used to populate a mgmt_lqi_rsp frame
		unsigned char AppendNeighborTable(CPacket &packet,
			unsigned char &nTotal, unsigned char &nAppended,
			unsigned char nIndex, CPacketData::size_type nCapacity) const;

		// Appends one or more routing table entries in ZDO compatible format
		// to a packet. Can be used to populate a mgmt_rtg_rsp frame
		unsigned char AppendRoutingTable(CPacket &packet,
			unsigned char &nTotal, unsigned char &nAppended,
			unsigned char nIndex, CPacketData::size_type nCapacity) const;

		// Obtains the network security material for the specified key sequence
		// number. Returns false, if no such material can be found
		bool GetNetworkSecurityMaterial
			(CZigBeeNetworkSecurityMaterial *&material,
			 const unsigned char keySequenceNumber,
			 const bool acceptStaleKey = false);

		// Obtains the current active network security material. Returns false,
		// if no such material can be found
		bool GetActiveNetworkSecurityMaterial
			(CZigBeeNetworkSecurityMaterial *&pMaterial);

		// Obtains the current active network security material, increments the
		// frame counter and returns the set that can be used to create a new
		// out-going frame
		bool GetSecurityMaterial(CZigBeeNetworkSecurityMaterial &material);

		// Stores vital networking parameters in non-volatile storage, including
		// core data, extended PAN-ID, and parent information (in case this is
		// an end-device)
		void Store();

		// Loads persistent data from non-volatile storage. Might be called
		// early during power-up to load only the parts required to configure
		// the ZigBee stack (i.e. IEEE 802.15.4 capabilities). Also called
		// to fetch operational attributes like the network address etc.
		void Load(const bool bAfterPowerup = false);

		// Stores vital core networking parameters in non-volatile storage
		void StoreCoreData();

		// Stores end-device child information in a neighbor-table entry in
		// persistent storage
		void StoreEndDeviceInfo(const CZigBeeNeighborInfo &info) const;

		// Attempts to finds an end-device child entry in persistent storage
		static bool FindEndDeviceInfo(CZigBeeStorageItem &item,
			const size_t cbItem, const CZigBeeNeighborInfo &info);

		// Finds and erases an end-device child entry in persistent storage
		static bool EraseEndDeviceInfo(const CZigBeeNeighborInfo &neighbor);

		// Loads end-device child information records and creates suitable
		// neighbor-table entries in the neighbor table
		void LoadEndDevices();

		// Erases all end-device records previously stored
		bool EraseAllEndDeviceInfos();

		// On end-devices, stores the parent router information (addresses and
		// capabilities)
		void StoreParentInformation();

		// On end-devices, loads the parent router information (addresses and
		// capabilities)
		void LoadParentInformation();

		// On end-devices, clears the parent router information (addresses and
		// capabilities)
		bool EraseParentInformation() const;

		// Returns the network security material key type for this device, i.e.
		// standard network key for a typical IEEE 802.15.4 device, and basic
		// authorization key for a virtual device
		uint_fast8_t GetDefaultKeyType() const;

		// Stores the network security material (the 128-bit AES key), indexed
		// by key type and key sequence number
		void StoreSecurityMaterial
			(const CZigBeeNetworkSecurityMaterial &material) const;

		// Stores the network security material (the 128-bit AES key), indexed
		// by key type and key sequence number
		bool LoadSecurityMaterial(CZigBeeNetworkSecurityMaterial &material,
			const unsigned char nKeySequenceNumber,
			const unsigned char nKeyType =
				CZigBeeNetworkSecurityMaterial::typeNetworkKey) const;

		// Erases the network security material as specified by its sequence
		// number. The material erased is either a standard network key, or,
		// in case the local device is a virtual device, a basic authorization
		// key
		bool EraseSecurityMaterial(const uint_fast8_t keySequenceNumber) const;

		// Erases either all network security material of the specified type
		bool EraseSecurityMaterialEx(const bool all = false) const;

		// Clears network key storage, i.e. flushes all existing keys
		void ClearNetworkKeys();

		// Stores the outgoing frame counter of the given security material by
		// first adding the grace sequence number period the the frame counter,
		// indexed by key type and key sequence number
		void StoreFrameCounter(const CZigBeeNetworkSecurityMaterial &material,
			const unsigned int nAhead = storageInterval) const;

		// Loads the outgoing frame counter of the given security material
		bool LoadFrameCounter(CZigBeeNetworkSecurityMaterial &material) const;

		// Set all incoming frame counters to -1. This should only be done when
		// a new network key is about to become active
		void InvalidateAllFrameCounters();

		// Set the incoming network security frame counter of a particular
		// neighbor to -1. This should only be done in exceptional cases, e.g.
		// from within the APSME QueryAuthenticateDevice() handler when an R20
		// or earlier device joins after a factory reset and is provided the
		// network key again
		void InvalidateFrameCounter(const CIEEE802154Address &address);

		// Activates a new network key, which must have been loaded before
		bool SwitchKey(const unsigned char nNewKeySequence);

		// Performs necessary verifications and updates to neighbor table
		// entries during incoming security frame processing
		bool Authenticate(const CZigBeeAddress &address,
			const bool authenticate,
			const uint_fast32_t frameCounter = -1);

		// Authenticates a neighbor
		bool Authenticate(CZigBeeNeighborInfo &neighbor,
			const uint_fast32_t frameCounter = UINT32_MAX);

		// For a frame that has been received over-the-air, returns the MAC
		// header portion
		static const CIEEE802154MHR &GetL2Header(const CPacket &packet);

		// Must be called by the local ZDO instance when a device_annce frame
		// is received. This will help the network layer detect short address
		// conflicts and clear-out stale end-device child entries from its
		// child table. The implementation assumes that the packet is a real
		// OTA packet, including MAC headers (which are required to obtain the
		// L2 source address).
		void OnDeviceAnnouncement(const CZigBeeAddress &address,
			const CPacket &asdu);

		// Checks if a message originated by an end-device indicates that the
		// child has changed to a different parent router. If so, removes the
		// device in question from the neighbor table. Returns true, if a stale
		// child table entry was erased and the iterator is void
		bool PruneStale(const CZigBeeNHR &nhr, const CZigBeeAddress &source,
			const CZigBeeAddress &destination, const CPacket &npdu,
			std::multiset<CZigBeeNeighborInfo>::iterator child);

		// This is part of R21 child aging. A device, that is none of this
		// router's end-devices has polled. This method injects a network leave
		// message to be sent to the former child device
		CPacketEx *InjectLeave(const CIEEE802154MHR &header);

		// This is part of R21 child aging. As a minimum this will invoke the
		// InjectLeave() to inject a network leave message. It will also queue
		// an additional ZDO management leave message, in case the end-device
		// does not accept a network leave request due to policy settings or
		// the EUI-64 being absent from the NHR
		CPacketEx *InjectLeaveEx(const CIEEE802154MHR &header);

		// This is part of address conflict resolution on behalf of a sleepy
		// end-device child. It will inject an unsolicited rejoin response and
		// clear the child's previous network address afterwards (one-off)
		CPacketEx *InjectUnsolicitedRejoinResponse(const CIEEE802154MHR &header,
			CZigBeeNeighborInfo &child);

		// This is part of R21 child aging and address conflict resolution on
		// behalf of sleepy end-device child. It is typically called from the
		// MLME-POLL.indication handler and will inject either a leave and
		// rejoin or an unsolicited rejoin response
		CPacketEx *InjectUnsolicited(const CIEEE802154MHR &header);

		// Returns true, if the supplied address belongs to a child device. If
		// so, returns the MAC capabilities (sleepy vs. non-sleepy)
		bool IsChild(const CZigBeeAddress &address,
			unsigned char &bCapabilities);

		// Generates a new network key. Upon success the returned key sequence
		// number identifies the new key. The key is only added to the set of
		// network security materials, but not activated
		bool GenerateNetworkKey(unsigned char &nKeySequence);

		// Returns an unused network security material set storage slot. The
		// active network key is typically in slot #0, while #1 is a storage
		// for the next network key pending activation
		CZigBeeNetworkSecurityMaterial *
		AllocateSecurityMaterialStorage(const bool bPurge = true);

		// Enhancement to the NLME, which allows to invoke a "custom" scan;
		// Custom scans are an extension to the IEEE 802.15.4 MAC, which allow
		// a device to temporarily change channels for an intermittent task.
		// For example, a GPP/GPS could use this feature to meet a GPD on the
		// rendezvous channel during commissioning to deliver a channel
		// configuration frame
		void OnRequestTemporaryChannelChange(const unsigned char nChannel,
			const unsigned char nDuration,
			const CIEEE802154ScanInfo::FILTERCALLBACK pfnFilter = 0);

		// This method is invoked by the APSME when it has received a network
		// key after joining or rejoining (via Trust Center rejoin) the network
		// and gives the NLME an opportunity to perform message exchanges that
		// require security; for instance, end-devices may start negotiating
		// a keep-alive timeout with their parent router
		void AfterNetworkJoined(const bool bRejoin);

		// This method is invoked by the APSME when startup succeeded
		void OnStartupSucceeded();

		// Returns an iterator identifying the parent device
		std::multiset<CZigBeeNeighborInfo>::iterator GetParent();

		// Returns an iterator identifying the parent device
		std::multiset<CZigBeeNeighborInfo>::const_iterator GetParent() const;

		// Searches the neighbor table for the specified device, based on its
		// network or extended address
		std::multiset<CZigBeeNeighborInfo>::iterator
		GetNeighbor(const CIEEE802154Address &address);

		// Add a new neighbor table entry
		std::multiset<CZigBeeNeighborInfo>::iterator AddNeighbor
			(const CZigBeeNeighborInfo &neighbor);

		// Removes an existing neighbor table entry
		std::multiset<CZigBeeNeighborInfo>::iterator RemoveNeighbor
			(const std::multiset<CZigBeeNeighborInfo>::iterator neighbor,
			 const uint_fast8_t endDeviceMaintenance = reschedule | rebuild,
			 const bool notifyNeighborLoss = true,
			 const bool invalidateRoutes = true);

		// For sleeping and non-sleeping end-devices: Start polling after
		// joining, rejoining or resuming network operation. For non-sleeping
		// end-devices, this will ensure the proper keep-alive is put in place
		void StartPolling(const bool bResponsive = false);

		// End-devices only: Enables automatic polling at the specified rate or
		// changes the rate (e.g. slow vs. fast polling). The rate is specified
		// as symbol period between poll attempts. Notice that this will result
		// in NLME-SYNC.confirm primitives being issued as if the NHL had a
		// corresponding NLME-SYNC.request
		void EnablePolling(const CCompactTimerService::ticks_diff_t interval =
			468750);

		// End-devices only: Stops automatic polling. Then, the application is
		// responsible for maintaining the end-device timeout keep-alive
		void DisablePolling();

		// End-devices only: Sends an end-device timeout request and polls for
		// the response. This only applies to sleepy and non-sleepy end-devices,
		// not to routers or the coordinator
		void NegotiateTimeout(const unsigned char nTimeout =
			CZigBeeNetworkInformationBase::nEndDeviceTimeout);

		// Routers and the coordinator only: Makes this device a concentrator.
		// The device will start sending periodic many-to-one route requests
		// if nwkConcentratorDiscoveryTime is non-zero and will also start
		// building and maintaining a source route cache
		void EnableConcentrator(const bool bEnable = true,
			const unsigned int nDiscoveryTime =
				CZigBeeNetworkInformationBase::nConcentratorDiscoveryTime,
			const unsigned char nRadius =
				CZigBeeNetworkInformationBase::nConcentratorRadius,
			const bool bMaintainRouteCache = true);

		// Sets the network update ID to the specified value, updates the
		// beacon payload and stores the new ID persistently
		void SetUpdateID(const unsigned char nUpdateID);

		// Address conflict detected for local device. Need to assign a new
		// random address and let the ZDO announce the address change. Also
		// broadcast a network address conflict with the offending address
		void OnAddressConflict(const CZigBeeAddress &address);

		// Address conflict detected for a device in the neighbor table. Need
		// to invalidate the short address and broadcast a network address
		// conflict with the offending address
		void OnAddressConflict(const CZigBeeAddress &address,
			std::multiset<CZigBeeNeighborInfo>::iterator &neighbor,
			const bool invalidateAndEmitNetworkStatus = true);

		// Address conflict detected for a device in the address map. Need to
		// delete the mapping and broadcast a network address conflict with the
		// offending address
		void OnAddressConflict(const CZigBeeAddress &address,
			const CZigBeeNetworkAddressMap::iterator &entry);

		// Resolves a local address conflict by changing the network address to
		// the specified address (or choosing a new random address and letting
		// the ZDO announce the address change. Returns the old address
		CZigBeeAddress ResolveAddressConflict(unsigned short wNewAddress =
			CZigBeeAddress::wBroadcastAll);

		// Resolves an address conflict on behalf of an end-device child by
		// choosing a new random address and letting transmitting a rejoin
		// response, unsolicited, to the child
		void ResolveAddressConflict
			(std::multiset<CZigBeeNeighborInfo>::iterator &neighbor);

		// Emit network status for an address conflict. Filters the coordinator
		// address and invokes EmitNetworkStatus() otherwise
		void EmitAddressConflict(const CZigBeeAddress &address,
			const bool bReflect = true);

		// Emit network status. This results in a network status command frame
		// being broadcast to all devices which have their receivers on while
		// idle and, by default, also a reflection of the command to the next
		// higher layer by issuing an NLME-NWK-STATUS.indication
		void EmitNetworkStatus(const unsigned char nStatus,
			const CZigBeeAddress &address, const bool bReflect = true,
			const CZigBeeAddress &alias = CZigBeeAddress(),
			const unsigned char nSequenceEx = 0,
			const CZigBeeAddress &destination = CZigBeeAddress());

		// Removes neighbor table entries with a relation of "none" from the
		// neighbor table; this is useful to purge
		void PurgeUnrelatedNeighbors();

		// Enter responsive mode until LeaveResponsiveMode() is invoked with
		// the cookie provided by EnterResponsiveMode()
		bool EnterResponsiveMode(unsigned int &nCookie);

		// Leave responsive mode for the context provided by the cookie
		void LeaveResponsiveMode(const unsigned int nCookie);

		// Enter responsive mode, and automatically leave after the specified
		// time, or pre-emptively by calling LeaveResponsiveModeEx() for the
		// cancellable timer provided by EnterResponsiveModeEx()
		bool EnterResponsiveModeEx(const unsigned int nDuration,
			CCompactTimer **const ppTimer = 0);

		// Leave responsive mode pre-emptively
		bool LeaveResponsiveModeEx(CCompactTimer *&pTimer);

		// True if the device is currently in responsive (fast polling) mode
		static bool IsResponsive();

		// Returns the preferred end-device keep-alive method, i.e. MAC data
		// poll or end-device timeout negotiation keep-alive. Returns zero, if
		// no keep-alive shall be used (not supported by the parent), or
		// capabilityPollKeepAlive = 0x01, if MAC data poll is the preferred
		// strategy, or capabilityTimeoutRequestKeepAlive = 0x02 if end-device
		// timeout negotiation is the preferred strategy
		unsigned char PreferredKeepAliveStrategy() const;

		// Changes the normal and responsive mode poll rates in operation
		void ModifyPollIntervals(const CCompactTimerService::ticks_diff_t
			normal, const CCompactTimerService::ticks_diff_t responsive);

		// Updates the beacon payload, i.e. calls CreateBeaconPayload() and
		// assigns the resulting payload packet to the MAC layer beacon payload
		void UpdateBeaconPayload();

		// Performs clean-up after a scan. This must be called from within
		// scan confirmation handlers, prior to issuing another scan
		void AfterScan();

		// Deletes the scan manager instance and clears the instance pointer.
		// This must be called for scan managers, which did not have the auto-
		// delete option set when constructed
		void DeleteScanManager();

	// Overridables
	public:
		// Processes command data frames, received via an MCPS-DATA.indication
		void OnIndicateCommand(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

	// Implementation
	public:
		// Channel pages 0xf0, 0xf1, 0xf2 are used to distinguish virtual scan
		// information for tunnel #1, #2 and #3
		enum { channelPageTunnel1 = 0xf0, channelPageTunnel2,
			channelPageTunnel3 };

		// Finishes network formation, i.e. utilizes the choosen channel,
		// PAN identifier and short addresses to start the MAC layer
		void FinishNetworkFormation();

		// Terminates a network formation request with the specified status
		// code. Performs clean-up required after startup has been initiated
		void TerminateNetworkFormation(const unsigned char nStatus);

		// Creates local advertisements for routers after successful start
		void OnRouterStartupSucceeded();

		// Notifies the network manager of a PAN-ID conflict that has just been
		// detected
		void EmitPANIDConflict();

		// This is the maximum depth that can be coded in the beacon payload,
		// not to confuse with nwkMaxDepth, which is defined in the device
		// profile...
		enum { maxDepth = 15 };

		// Outgoing frame counter non-volatile storage store-ahead interval.
		// This must be a power of two and allows the device to not store all
		// frame counter increments, but store the counter only when the pre-
		// reserved counters have been consumed
		enum { storageInterval = 0x1000 };

		// Flags for end-device maintenance behavior. Reschedule the timer and
		// rebuild the address cache
		enum { suppress, reschedule = 0x01, rebuild = 0x02 };

		// Number of ticks (symbol periods) to wait before attempting to
		// negotiate end-device timeout. Currently set to a second
		enum { timeoutNegotiationDelayTicks = 62500u };

		// Maximum duration, in ticks, for the end-device maintenance timer.
		// This is relevant if the timer is too narrow. For example, a 32 bit
		// timer running at the 2.4GHz DSSS PHY symbol rate of 62500Hz would
		// potentially overflow for the maximum permitted end-device timeout
		// period of approximately 18 days. For platforms with 64 bit timers
		// this is no real consideration. Set this to at most 2^30 - 1 for
		// 32bit timers, or at most 2^62 - 1 for 64bit timers to avoid overflow
		static const CCompactTimerService::ticks_diff_t
			maximumMaintenanceInterval;

		// A bit mask with the active responsive mode polling locks
		static unsigned int nAcquiredResponsiveModeLocks;

		// An entry that has just been pruned from the neighbor table and will
		// be permanently removed unless expressly retained by the next higher
		// layer when inbound data processing is finished
		CZigBeeNeighborInfo *m_pStaleChild;

		// Rejoin response timer
		CCompactTimer *m_pRejoinResponseTimer;

		// Link status period timer
		CCompactTimer *m_pLinkStatusTimer;

		// Unauthenticated device clean-up timer
		CCompactTimer *m_pUnauthenticatedCleanupTimer;

		union
		{
			// On ZigBee routers: End-device maintenance timer (R21 child aging)
			CCompactTimer *m_pEndDeviceMaintenanceTimer;

			// On ZigBee end-devices: poll timer
			CCompactTimer *m_pPollTimer;
		};

		// Polling interval in ticks for ongoing periodical polling procedure
		CCompactTimerService::ticks_diff_t m_pollInterval;

		// On ZigBee end-devices: end-device time-out negotiation timer
		CCompactTimer *m_pTimeoutNegotiationTimer;

		// Permit joining timer (expires PermitDuration seconds after a
		// NLME-PERMIT-JOINING.request has been issued)
		CCompactTimer *m_pPermitJoiningTimer;

		// Network update timer. Whenever the network is going to be informed
		// of a network update (new PAN-ID, etc.) this timer defers the actual
		// update by broadcast delivery time
		CCompactTimer *m_pNetworkUpdateTimer;

		// Concentrator discovery timer. If the concentrator is configured to
		// broadcast many-to-one route requests periodically, this timer will
		// instigate route discovery when due
		CCompactTimer *m_pConcentratorDiscoveryTimer;

		// Advanced network connectivity maintenance timer. Fires once every
		// 24h to perform advanced network connectivity maintenance
		CCompactTimer *m_pAdvancedMaintenanceTimer;

		// Network startup timer that is used for either suppressing initial
		// broadcast conflict detection on a router device or initiating
		// initial responsive mode on a sleepy end device after startup
		CCompactTimer *m_networkStartupTimer;

		// The scan manager acts on MLME scan results, i.e. MLME-SCAN.confirm
		// and MLME-BEACON-NOTIFY.indication. It may be installed prior to
		// issuing an MLME-SCAN.request, in which case the confirm primitives
		// mentioned above will be routed to the according member functions
		// for processing
		CZigBeeNetworkScanManager *m_pScanManager;

		// Poll attempts made in this round
		uint8_t m_pollAttemptCount;

		// When set to true, network rejoins will be done insecure, even if a
		// network key is present. This allows for updating the key, in case
		// a key exchange has been missed or ascertaining that the router
		// through which the device rejoins is connected to the trust center
		bool m_bInsecureRejoin;

		// Device depth (0 to 15). This no longer maintained or used since R21
		// and is only here for passing historic compliance test cases
		unsigned char m_nDepth;

		/*
		// True, if the link status timer was cancelled due to a scan request
		// and should be re-scheduled afterwards
		bool m_bRecreateLinkStatusTimer;
		*/

		// True, if a start request has been issued to the MAC and the NLME is
		// expecting a confirmation; allows independent startup of the MAC, for
		// example to support MAC-level packet capture (sniffer) applications
		// in combination with ZigBee applications, where the device does not
		// belong to a network yet (and the MAC shall not be started using the
		// ZigBee stack start-up procedure). Instead, the application will fire
		// a MLME-START.request directly and the NLME needs to ignore the mating
		// MLME-START.confirm
		bool m_bStarting;

		// On ZigBee end-devices: number of attempts that have been made for
		// end-device timeout negotiation
		uint8_t m_attemptTimeoutNegotiation;

		// True if reset procedure is currently in progress
		bool m_resetInProgress;

		// Time of last rejoin due to advanced parent link maintenance
		CCompactTimerService::ticks_t m_advancedMaintenanceTimestamp;

		// Advanced network maintenance timer
		static const CCompactTimerService::ticks_t advancedMaintenanceInterval;

		// Time of the last regular link status transmission
		CCompactTimerService::ticks_t m_regularLinkStatusTimestamp;

		// Time of the last link status transmission, either regular
		// or reactive
		CCompactTimerService::ticks_t m_linkStatusTimestamp;

		// Always treat the coordinator and trust center as a simplistic,
		// i.e. 'low RAM', concentrator. This results in always sending route
		// records to the coordinator regardless of it actually requesting
		// such route record frames via many-to-one (MTO) routing. This will
		// mimic the behavior of certain third-party stacks that rely on this
		// kind of non-standard behavior. This is disabled by default and
		// recommended to be left disabled unless there is specific reason to
		// support such non-standard behavior in a legacy target system.
		static const bool forceTCIsSimpleConcentrator;

		// Determines the number of historical (past) network security material
		// that shall be retained in persistent storage. This number is
		// typically 0 for standard IEEE 802.15.4 devices and must be set to at
		// least 1 for devices featuring trusted links in order to support
		// rejoins of virtual devices in a secure manner
		static const uint8_t securityMaterialHistory;

		// Preferred short network address, either for the local node or a
		// respective child. Defaults to 0xfffe so that the network address
		// will be randomly generated.
		static const uint16_t preferredNetworkAddress;

		// Must be called after the capabilities have changed to configure the
		// MAC PIB accordingly. Inherently called by SetCapabilities()
		void AfterCapabilitiesModified();

		// Checks if the neighbor table can profit from the address mapping
		bool MaintainNeighborInfo(const CZigBeeAddress &address);

		// When the network layer receives a broadcast frame, it calls this
		// member function to check, whether normal broadcast processing should
		// be applied to the frame. If this function returns false, the frame
		// is passed to its intended recipient without duplicate rejection.
		// Moreover, the frame is not retransmitted in this case
		bool PreProcessBroadcast(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, const CPacket &npdu,
			const bool bInspecting = false) const;

		// Create a Route Reply command frame
		static CPacket CreateRouteReplyFrame
			(const CZigBeeRouteReplyCommand &reply);

		// Called on arrival of a route request command frame
		void OnIndicateRouteRequest(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a route reply command frame
		void OnIndicateRouteReply(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a network status command frame
		void OnIndicateNetworkStatus(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Default implementation of OnIndicateNetworkStatus(), which should be
		// called by application-specific overloads of OnIndicateNetworkStatus()
		// in addition to any custom logic provided by the application
		void DefaultOnIndicateNetworkStatus
			(const CIEEE802154Address &sourceAddress, const CZigBeeNHR &header,
			CPacket &nsdu, const unsigned char nLinkQuality,
			const unsigned char nDataSequence, const unsigned long nReceiveTime);

		// Called on arrival of a leave command frame
		void OnIndicateLeave(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a route record command frame
		void OnIndicateRouteRecord(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a rejoin request command frame
		void OnIndicateRejoinRequest(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a rejoin response command frame
		void OnIndicateRejoinResponse(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a link status command frame
		void OnIndicateLinkStatus(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a network report command frame
		void OnIndicateNetworkReport(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a network update command frame
		void OnIndicateNetworkUpdate(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of an end-device timeout request command frame
		void OnIndicateTimeoutRequest(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of an end-device timeout response command frame
		void OnIndicateTimeoutResponse(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Clears the neighbor table and deletes all entries
		void RemoveAllNeighbors();

		// Creates a CPacket object that carries the ZigBee network beacon
		// payload according to clause 3.6.7
		CPacket CreateBeaconPayload() const;

		// Creates a CPacket object that can carry a command frame of the
		// specifed payload length (excluding the leading command identifier)
		static CPacket CreateCommandFrame(const unsigned char bCommand,
			const CPacketData::size_type cbPayload);

		// Continues network formation, i.e. chooses a channel, PAN identifier,
		// and starts the MAC layer
		void ContinueNetworkFormation();

		// Completes network formation, i.e. initializes depth, beacon payload
		// and security material
		void CompleteNetworkFormation();

		// Collects the PANIDs of the supplied PAN ID descriptor list
		static void CollectPANIDs(std::set<unsigned short> &ids,
			const std::list<CIEEE802154PANDescriptor> &networks);

		// Called when the local device detects a PAN-ID conflict
		void OnPANIDConflict(const CIEEE802154PANDescriptor &descriptor,
			const CZigBeeBeacon &beacon);

		// Selects a suitable 16-bit network identifier (PAN-ID) for the
		// network, avoiding any known network identifiers in the surroundings
		// and preferring the specified PANID, if it does not result in a
		// conflict with any existing 16-bit network identifier
		static unsigned short ChoosePANID
			(const std::list<CIEEE802154PANDescriptor> &networks,
			const unsigned short wPreferred = CZigBeeAddress::wBroadcastAll);

		// Selects a channel from the given collection of channels by searching
		// for the channel with the least number of networks and devices. When
		// there are multiple channels with the equally low number of networks,
		// the one with the least energy will be preferred
		static bool ChooseChannel
			(const std::set<CZigBeeStartupChannelInformation> &channels,
			std::set<CZigBeeStartupChannelInformation>::const_iterator &choice);

		// Selects a suitable network address for the local node if bOwnAddress
		// is true, or a prospective child (if bOwnAddress is false)
		unsigned short ChooseNetworkAddress(unsigned short wAddress = 0,
			const bool bOwnAddress = true) const;

		// Sets or clears the permit joining flag. If permit joining is opened
		// indefinitely, sets a flag in persistent storage to make sure the
		// network permit joining state persists across power-cycles
		void SetPermitJoining(const bool bEnable,
			const bool bPersistent = false);

		// Processes a (re-)join request and returns true, if the device
		// has been successfully added to the neighbor table as a child (or
		// an appropriate entry already existed)
		uint8_t ProcessJoinRequest(const CIEEE802154Address &child,
			const unsigned char bCapabilities,
			const unsigned char nRejoinNetwork,
			std::multiset<CZigBeeNeighborInfo>::iterator &neighbor,
			const bool bSecure = false,
			const bool bCommissioning = false,
			const unsigned char nLinkQuality = 0, const void *const pTLVs = 0,
			const uint_fast8_t cbTLVs = 0);

		// Sends a rejoin or network commissioning response frame, solicited
		// or unsolicited, to the destination using the provided nextHop
		// address
		void TransmitRejoinResponse(const CZigBeeAddress &destination,
			const CIEEE802154Address &nextHop, const CZigBeeAddress &address,
			const uint_fast8_t status, const uint_fast8_t capabilities,
	 		const bool secure = true, const bool commissioning = false);

		// If this timer fires, the local station did not receive a rejoin
		// response after having sent a rejoin request
		void OnRejoinResponseTimeout(void *pArgument);

		// Processes a rejoin response or MLME-ASSOCIATE.confirm and issues
		// an NLME-JOIN.confirm after maintaining neighbor table information,
		// etc.
		void ProcessJoinResponse(const unsigned short wNetworkAddress,
			const unsigned char nStatus, const bool bSecure);

		// This method initiates a join via MAC layer association
		void JoinViaAssociation(const unsigned long long qwExtendedPANID,
			const unsigned int nScanChannels);

		// This method initiates a join via MAC layer orphan scanning
		void JoinViaOrphaning(const unsigned int nScanChannels);

		// This method initiates a join via a network rejoin request frame
		void JoinViaNetworkRejoin(const unsigned long long qwExtendedPANID,
			const unsigned int nScanChannels, const unsigned char nScanDuration,
			const bool bEnableSecurity = true);

		// Given a list of network descriptors and an EPID, this method returns
		// true if a matching entry exists, false otherwise. Also provides the
		// iterator, if required
		static bool FindNetwork(std::list<CZigBeeNetworkDescriptor> &networks,
			const unsigned long long qwExtendedPANID,
			const unsigned short wPANID, const unsigned char nLogicalChannel,
			std::list<CZigBeeNetworkDescriptor>::iterator *pEntry = 0);

		// Rebuilds the child table cache after any relevant changes to the
		// neighbor table, e.g. a new child device joining or a previous child
		// leaving the network, having timed-out or having changing parents
		void RebuildChildAddressCache();

		// Process beacons
		void ProcessBeacon(const unsigned char nSequence,
			const CIEEE802154PANDescriptor &descriptor, CPacket &sdu);

		// Removes all entries from the neighbor table that are not belonging
		// to the specified network
		void CleanupNeighborTable(const unsigned long long qwExtendedPANID,
			const unsigned int nChannelMask);

		// Removes extened neighbor information collected during a network
		// discovery from the neighbor table after the device has successfully
		// joined a network
		void TidyNeighborTable();

		// Picks the next entry in the discovery table that has the potential
		// parent flag set
		std::multiset<CZigBeeNeighborInfo>::const_iterator
		PickNextPotentialParent(const uint_fast64_t qwExtendedPANID,
			const bool bRejoining);

		// Clear the potential parent flag for an entry in the discovery
		// table due to a failed join or rejoin attempt unless retry is
		// deliberately intended. For the latter case, increase the failure
		// counter while ensuring that the potential parent flag remains
		// set until a predefined retry threshold is reached.
		void UnmarkPotentialParent
			(const std::multiset<CZigBeeNeighborInfo>::iterator &parent,
			const bool bRejoin, const bool retry = false);

		// Verify whether at least one entry in the discovery table is
		// eligible for being a potential router parent
		bool HaveMorePotentialParents(const uint8_t type) const;

		// Collect network descriptors based on the outcome of a performed
		// active scan procedure. Return true if at least one network exists.
		bool CollectNetworkDescriptors
			(std::list<CZigBeeNetworkDescriptor> &networks);

		// Sets (modifies) the relationship to neighbor. Maintains child table
		// in non-volatile memory upon changes
		void ModifyRelationship(CZigBeeNeighborInfo &neighbor,
			const unsigned short nRelationship);

		// Schedules the unauthenticated device clean-up timer by looking at
		// the neighbor table
		void ScheduleUnauthenticatedCleanupTimer
			(const CZigBeeNeighborInfo *const info = 0);

		// Refreshes unauthenticated device lifetimes in the neighbor table
		// based on how much time has elapsed since the timer was started. This
		// needs to be called whenever a running timer is cancelled and
		// replaced with a new one, or the timer fires
		void RefreshUnauthenticatedLifetimes(const CCompactTimer &timer,
			const CZigBeeNeighborInfo *const skip = 0);

		// Ages out unauthenticated devices, which did not send at least one
		// network encrypted recently. Note there is a single timer, which
		// handles all unauthenticated devices
		void OnUnauthenticatedCleanupDue(void *pArgument);

		// Completes the joining process after association or rejoin
		void CompleteJoin(const CIEEE802154Address &device);

		// Specialized MLME-DATA.confirm handler for rejoin response frames
		void OnConfirmDataRejoinResponse(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Default implementation of OnConfirmDataRejoinResponse(), which
		// should be called by application-specific overloads of
		// OnConfirmDataRejoinResponse() in addition to any custom logic
		// provided by the application
		void DefaultOnConfirmDataRejoinResponse(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Generic MLME-DATA.confirm handler
		void OnConfirmDataGeneric(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// This method handles an MLME-SCAN.confirm for a MLME-SCAN.request
		// that has been issued in response to a NLME-NETWORK-DISCOVERY.request
		void OnConfirmScanNetworkDiscovery(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// This method handles an MLME-SCAN.confirm for a MLME-SCAN.request
		// that has been issued in response to a NLME-ED-SCAN.request
		void OnConfirmScanEnergyDetection(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// This method handles an MLME-SCAN.confirm for a MLME-SCAN.request
		// that has been issued in response to a temporary channel change
		void OnConfirmScanTemporaryChannelChange(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// This method handles an MLME-SCAN.confirm for a MLME-SCAN.request
		// that has been issued in response to a NLME-JOIN.request with the
		// RejoinNetwork parameter indicating a rejoin via orphaning
		void OnConfirmScanJoinViaOrphaning(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// This method handles an MLME-SCAN.confirm for a MLME-SCAN.request
		// that has been issued in response to a NLME-JOIN.request with the
		// RejoinNetwork parameter indicating a rejoin via network rejoin
		void OnConfirmScanNetworkRejoin(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// This method handles an MLME-SCAN.confirm for a MLME-SCAN.request
		// that has been issued in response to a NLME-NETWORK-FORMATION.request
		// to perform an energy detection scan
		void OnConfirmScanNetworkFormationStage1(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// This method handles an MLME-SCAN.confirm for a MLME-SCAN.request
		// that has been issued in response to a NLME-NETWORK-FORMATION.request
		// to perform an active scan on a set of channels with acceptable
		// energy level (outcome of stage 1)
		void OnConfirmScanNetworkFormationStage2(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// Creates a new instance of the link status timer based on the
		// specified timespan. The boolean flag indicates whether the next
		// pending link status frame is a regular transmission.
		void CreateLinkStatusTimer(const bool regular,
			const CCompactTimerService::ticks_diff_t timeSpan);

		// Creates a link status command frame and transmits it. Should be
		// invoked once every nwkLinkStatusPeriod seconds, with some random
		// jitter no avoid potential synchronization. Also ages out routers
		// for which no link status message was received for a while
		void OnLinkStatusPeriodElapsed(void *pArgument);

		// Returns the current cost estimation for a particular neighbor link.
		// Returns false if the neighbor is not in the neighbor table or if the
		// outgoing cost indicates an asymmetric link. Otherwise the worse of
		// incoming and outgoing costs is returned as path cost
		bool GetLinkCost(const CZigBeeAddress &address,
			unsigned char &nLinkCost);

		// Transmits a route reply command frame, which as been prepared in
		// the packet object
		void TransmitRouteReply(CIEEE802154Address &nextHop,
			CPacket &nsdu, const unsigned char nRadius = 0);

		// Broadcasts a route request command frame
		void TransmitRouteRequest(CZigBeeRouteDiscoveryInfo &info);

		// Originates a route record frame for a frame with the given header,
		// which might be either a frame originating at the local node or one
		// of its end-device children
		void InitiateRouteRecordIfRequired(const CZigBeeNHR &header,
			const CZigBeeRoute &route);

		// Locates a source route entry in the source route cache or creates a
		// new one if it doesn't exist. The entry is initially not linked to a
		// previous hop
		bool MaintainSource(const unsigned short wAddress,
			std::set<CZigBeeSourceRoute>::iterator &source);

		// Creates or updates a source route entry in the source route cache,
		// linking it to a previous hop. Both devices are identified by their
		// short network addresses. If an iterator is provided, it will be
		// pointing to the new source route cache entry
		bool MaintainSourceRoute(const unsigned short wAddress,
			const unsigned short wPreviousHop = CZigBeeAddress::wBroadcast,
			std::set<CZigBeeSourceRoute>::iterator *pSource = 0);

		// Invalidates the source route to a specific destination and all other
		// source routes that are directly or indirectly affected by the broken
		// path
		void InvalidateSourceRoute(const unsigned short wAddress);

		// Invalidates a source route. This is called internally be other
		// methods
		void InvalidateSourceRoute(std::set<CZigBeeSourceRoute>::iterator &s);

		// Create a route request command frame
		static CPacket CreateRouteRequestFrame
			(const CZigBeeRouteRequestCommand &request);

		// Timer callback, invoked when random delay has expired
		void OnRouteRequestJitterElapsed(void *pArgument);

		// Called after a route request has been broadcast
		void OnConfirmDataRouteRequest(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Completes route discovery for a request that has an associated entry
		// in the route discovery table
		void CompleteRouteDiscovery(const unsigned char nStatus,
			const unsigned char nNetworkStatus,
			const CZigBeeRoute *const pRoute, CZigBeeRouteDiscoveryInfo &info);

		// Shortcut for CompleteRouteDiscovery() in case of success
		void CompleteRouteDiscovery(const CZigBeeRoute *const pRoute,
			CZigBeeRouteDiscoveryInfo &info);

		// Completes route discovery (works for requests without an entry)
		void CompleteRouteDiscovery(const unsigned char nStatus,
			const unsigned char nNetworkStatus,
			const CZigBeeRoute *const pRoute,
			const CZigBeeRouteDiscoveryInfo::CALLBACK pfnComplete,
			void *const pArgument);

		// Aborts route discovery, performs routing table and route discovery
		// table clean-up and invokes the corresponding completion handler
		void AbortRouteDiscovery(CZigBeeRouteDiscoveryInfo &info,
			const unsigned char nStatus);

		// Returns true, if the routing table entry identified by route is
		// referenced by a route discovery table entry other than exempt
		bool IsRouteReferenced(const std::set<CZigBeeRoute>::const_iterator route,
			const CZigBeeRouteDiscoveryInfo *const pExempt = 0) const;

		// Called when a route discovery table entry has expired
		void OnRouteDiscoveryExpired(void *pArgument);

		// Called when a route discovery initiated by a
		// NLME-ROUTE-DISCOVERY.request has completed
		void OnRouteDiscoveryRequestComplete(const unsigned char nStatus,
			const unsigned char nNetworkStatus,
			const CZigBeeRoute *const pRoute, void *pArgument);

		// Called when a route discovery initiated by an incoming route request
		// has completed
		void OnIndicateRouteRequestDiscoveryComplete(const unsigned char nStatus,
			const unsigned char nNetworkStatus,
			const CZigBeeRoute *const pRoute, void *pArgument);

		// Called when a route discovery initiated by the NLDE has completed.
		// This might be due to a data frame needing forwarding (router), or
		// a data request from the next higher layer
		void OnRouteDiscoveryComplete(const unsigned char nStatus,
			const unsigned char nNetworkStatus,
			const CZigBeeRoute *const pRoute, void *pArgument);

		// MCPS-DATA.confirm handler for a rejoin request
		void OnConfirmDataRejoinRequest(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Called PermitDuration seconds after a NLME-PERMIT-JOINING.request
		// has been issued
		void OnPermitDurationTimerElapsed(void *pArgument);

		// MCPS-DATA.confirm handler for a leave command
		void OnConfirmDataLeave(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Finishes a leave request, i.e. actually commits the leave operations
		void FinishLeave(CZigBeeNetworkLeaveCookie &cookie);

		// This timer function is executed when a device is requested to leave
		// and the request was broadcast. It will continue normal operation
		// to allow the leave request to propagate through the network and
		// will then leave and rejoin
		void OnLeaveDelayElapsed(void *pArgument);

		// Invoked when a neighbor is about to be removed from the neighbor
		// table. The second argument specifies whether the end-device
		// maintenance timer should be re-scheduled and the child address cache
		// rebuilt, if necessary; additionally, all routes using the specified
		// neighbor as their next hop may be invalidated
		void OnNotifyNeighborLoss
			(const std::multiset<CZigBeeNeighborInfo>::iterator lost,
			 const uint_fast8_t endDeviceMaintenance = reschedule | rebuild,
			 const bool invalidateRoutes = false);

		// Avanced hook that is being executed when a neighbor is about to be
		// removed from the neighbor table
		void BeforeRemoveNeighbor(const CZigBeeNeighborInfo &neighbor);

		// Sorting predicate for lists returned by network discovery
		static bool NetworkDiscoveryCompare(const CZigBeeNetworkDescriptor &a,
			const CZigBeeNetworkDescriptor &b);

		// Create a network status command frame
		static CPacket CreateNetworkStatusFrame(const uint_fast8_t status,
			const CZigBeeAddress &address, const void *const pTLVs = 0,
			const uint_fast8_t cbTLVs = 0);

		// Transmit network status command frame
		void TransmitNetworkStatus(const CZigBeeAddress &destination,
			const unsigned char nStatus, const CZigBeeAddress &address,
			const CIEEE802154Address *const pNextHop = 0,
			const CZigBeeAddress &alias = CZigBeeAddress(),
			const unsigned char nSequenceEx = 0, const void *const pTLVs = 0,
			const uint_fast8_t cbTLVs = 0);

		// MCPS-DATA.confirm handler for a network status command
		void OnConfirmDataNetworkStatus(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Returns the next key sequence number to use. Walks the network
		// security material and returns the next sequence number available
		unsigned char GetNextKeySequenceNumber() const;

		// Returns the key sequence number of an assigned but unused network
		// key material if any, and verifies if an already rotated network key
		// is still pending expiration
		bool GetPendingKeySequenceNumber(unsigned char &sequence,
			bool &stale) const;

		// Called, when the active network security material is modified, e.g.
		// when the outgoing frame counter changes or the key is updated
		void OnSecurityMaterialModified
			(const CZigBeeNetworkSecurityMaterial &material) const;

		// Updates the address of an existing neighbor table entry and also an
		// implicit/explicit address mapping that might exist. Never simply
		// change the address in a existing entry, because the address is key
		// in the neighbor set. Instead, call this method, which will erase and
		// reinsert the entry and also properly maintain the address map.
		// Notice the iterator will be changed to point to the modified (new)
		// entry on return, so don't use it as iterator in a loop. Avoid using
		// this method if there is no change at all
		void ChangeNeighborAddress(std::multiset<CZigBeeNeighborInfo>::
			iterator &neighbor, const unsigned short wAddress);

		// Invoked on an end-device a few instants after the network key has
		// been delivered, after the device announcement broadcast has settled.
		// Will send an end-device timeout request to the router parent
		void OnTimeoutNegotiationTimeout(void *pArgument);

		// MCPS-DATA.confirm handler for an end-device timeout request command
		void OnConfirmDataTimeoutRequest(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Timer that polls the parent for the end-device timeout response or
		// reattempts time-out negotiation if it failed before
		void OnTimeoutResponseTimeout(void *pArgument);

		// Converts a timeout enumeration value to the unit actually stored in
		// the neighbor table. The specification states 'seconds', but we use
		// timer ticks
		static bool ConvertTimeout(CCompactTimerService::ticks_diff_t &timeout,
			const uint_fast8_t enumeratedValue);

		// Schedules the end-device timeout timer by looking at the end-device
		// lifetimes in the the neighbor table. The child entry that expires
		// first, sets the lifetime for the remaining devices. Caution: When
		// this method returns, a number of end-device child entries might have
		// been removed from the neighbor table and any related iterators might
		// have been invalidated
		// Skip refreshing a particular child end device's lifetime, if specified.
		// This is usually necessary if the lifetime has just been reset due to
		// regular end device keep-alive maintenance.
		void ScheduleEndDeviceMaintenanceTimer
			(const std::multiset<CZigBeeNeighborInfo>::iterator *const info = 0);

		// Refreshes the end-device lifetime in the neighbor table based on how
		// much time has elapsed since the timer was started. This needs to be
		// called whenever a running timer is cancelled and replaced with a new
		// one, or the timer fires. Caution: When this method returns, a number
		// of end-device child entries might have been removed from the neighbor
		// table and any related iterators might have been invalidated
		// Skip refreshing a particular child end device's lifetime, if specified.
		// This is usually necessary if the lifetime has just been reset due to
		// regular end device keep-alive maintenance.
		void RefreshEndDeviceLifetimes(const CCompactTimer &timer,
			 const unsigned char endDeviceMaintenance = rebuild | reschedule,
			 const std::multiset<CZigBeeNeighborInfo>::iterator *const skip = 0);

		// Ages out end-devices, which haven't sent a keep-alive recently. Note
		// there is a single time, which handles all end-devices
		void OnEndDeviceMaintenanceDue(void *pArgument);

		// Create an injectable NPDU from the supplied NSDU (or NWK management
		// frame)
		bool MakeInjectable(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,	const unsigned char nSequence,
			CPacket &nsdu, const unsigned char nRadius,
			const bool bEnableSecurity, const unsigned char nType);

		// Maintains end-device relationship when the router receives what it
		// accepts as an end-device keep-alive (currently either MAC data poll
		// or orphan notification)
		bool MaintainEndDevice(const CIEEE802154Address &device);

		// Determines a suitable interval for an end-device to perform its
		// keep-alive (either MAC data poll or timeout negotiation)
		CCompactTimerService::ticks_diff_t
		CalculateEndDeviceKeepAliveInterval() const;

		// Schedules an end-device timeout negotiation sequence as a keep-alive
		// for routers that only support this kind of keep-alive, and not the
		// more efficient MAC data poll keep-alive. Also used to trigger the
		// initial negotiation
		void ScheduleEndDeviceTimeoutNegotiation
			(const bool bInitialRequest = false);

		// Polls the parent router regularly
		void OnPollTimerElapsed(void *pArgument);

		// Retries a data polls shortly after a poll failure up to a certain
		// number of attempts
		void OnPollRetryTimerElapsed(void *pArgument = 0);

		// Timer that leaves responsive mode after a predetermine timeout
		void OnResponsiveModeTimeoutElapsed(void *pArgument);

		// Arms the broadcast address conflict detection shortly after start-up
		void OnBroadcastConflictGuardTimeElapsed(void *pArgument);

		// This timer fires when a pending network update should be applied
		void OnNetworkUpdateDelayElapsed(void *pArgument);

		// Issues an NLME-ROUTE-DISCOVERY.request with no destination address,
		// i.e. broadcasts a many-to-one route request
		void OnConcentratorDiscoveryPeriodElapsed(void *pArgument = 0);

		// Retains a child device that was initially deemed stale by the
		// network layer, but then the higher layer wishes to revise this
		// decision based on additional context not available to the network
		// layer; or removes the device permanently from the neighbor table.
		// Returns the iterator of the retained neighbor (or end, if the
		// neighbor is not retained)
		std::multiset<CZigBeeNeighborInfo>::iterator
		FinishPruneStale(const bool bRetain = false);

		// Updates local advertisements, in particular the router information
		// TLV, such that it mirrors information stored elsewhere in the NIB
		void UpdateLocalAdvertisements();

		// Sends a network commissioning request to a prospective parent. The
		// parent must be a neighbor and its entry must include extended, i.e.
		// discovery, information. An optional set of TLVs can be passed to
		// the trust center or the parent piggy-backed on the network
		// commissioning frame. Notice that the caller must not pass the local
		// device's key agreement capabilities, fragmentation parameters, or
		// extended device capabilities (in case of a virtual device)
		void RequestCommissioning
			(std::multiset<CZigBeeNeighborInfo>::const_iterator &parent,
			const uint_fast8_t nType,
			const void *const pTLVs = 0, const uint_fast8_t cbTLVs = 0,
			const void *const pParentTLVs = 0,
			const uint_fast8_t cbParentTLVs = 0);

		// Called on arrival of a network commissioning request command frame
		void OnIndicateCommissioningRequest(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Called on arrival of a network commissioning response command frame
		void OnIndicateCommissioningResponse(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned long nReceiveTime);

		// Schedules advanced network connectivity maintenance, which occurs
		// once every 24 hours in order to optimize network connectivity. For
		// an end-device, this might result in a rejoin if either the parent
		// link was constantly poor quality, or no APS acknowledgment has been
		// received over a period of 24 hours
		void ScheduleAdvancedMaintenance();

		// Advanced parent link maintenance performed by end-devices
		void OnAdvancedParentLinkMaintenanceDue(void *argument);

		// Advanced network connectivity maintenance for routers
		void OnAdvancedRouterConnectivityMaintenanceDue(void *argument);

		// Called when either a poor parent link is observed, or in absence
		// of a an end-to-end connectivity proof. The criterion must be one or
		// more of the criteria encoded in the advanced connectivity flags
		void UpdateAdvancedParentLinkCriteria(const uint_fast8_t value,
			const uint_fast8_t mask);

		// Called within a few seconds after a network key switch occurred.
		// During this time both the previously active (current) and new
		// (prospective) network key will be accepted for inbound messages
		// to prevent any multi-hop connectivity issues. When this key
		// rotation window expires, the previous key will be purged from
		// the network security material set.
		void OnNetworkKeyRotationWindowExpired(void *pArgument);

		// Called when a virtual device has opened a tunnel, i.e. trusted link
		void SetupTunnel(CZigBeeNeighborInfo &neighbor,
			const CZigBeeAddress &address, const CIEEE802154Address &source);

		// Called when a virtual device did not complete authentication in due
		// time, e.g. after initial join or trust center rejoin
		void OnTunnelAuthenticationTimeout
			(const CZigBeeNeighborInfo &neighbor);

		// Returns true if the local device should reply to route requests on
		// behalf of the specified neighbor device or initiate route records on
		// behalf; this overloadable function is for special applications, for
		// example, assisting Zigbee Virtual Devices operating behind a trusted
		// link in joining legacy networks
		bool ShouldProxyRoute(const CZigBeeNeighborInfo &neighbor) const;

		// Broadcasts a network leave indication with the specified options
		void EmitLeave(const CZigBeeAddress &address,
			const CZigBeeLeaveCommand &leave,
			CZigBeeNetworkClient *const pClient,
			const CZigBeeDispatchDataConfirm::CALLBACK pfnCallback,
			void *pArgument);

		// Unicasts a network leave & rejoin request
		void EmitRejoinRequest(const CZigBeeAddress &address);

		// Calculate the total path cost based on the path cost so far and
		// the link cost to this hop
		uint_fast8_t CalculatePathCost(const uint_fast8_t pathCost,
			const uint_fast8_t linkCost);

		// Update the link quality in the neighbor table entry (MAC source)
		// and the related flags for link quality assessment
		void UpdateLinkQuality(const CIEEE802154Address &sourceAddress,
			const uint_fast8_t linkQuality);
};

inline bool CZigBeeNetworkManagement::IsRouter(const unsigned char bCapabilities)
	{ return (bCapabilities & CIEEE802154MAC::capabilityFullFunctionDevice); }

inline bool CZigBeeNetworkManagement::IsEndDevice(const unsigned char bCapabilities)
	{ return !IsRouter(bCapabilities); }

inline bool CZigBeeNetworkManagement::IsReceiverOnWhenIdle(const unsigned char bCapabilities)
	{ return (bCapabilities & CIEEE802154MAC::capabilityReceiverOnWhenIdle); }

inline bool CZigBeeNetworkManagement::IsRouter() const
	{ return IsRouter(m_nib.m_bCapabilities); }

inline bool CZigBeeNetworkManagement::IsEndDevice() const
	{ return IsEndDevice(m_nib.m_bCapabilities); }

inline bool CZigBeeNetworkManagement::IsReceiverOnWhenIdle() const
	{ return IsReceiverOnWhenIdle(m_nib.m_bCapabilities); }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDispatchCommStatusIndication

class CZigBeeDispatchCommStatusIndication : public CZigBeeDispatch
{
	// typedefs & enums
	public:
		// Callback to a MLME-COMM-STATUS.indication(msduHandle, status,
		// Timestamp) handler. When requesting data, register a callback
		// with the specific handle
		typedef void (CZigBeeNetworkClient::*CALLBACK)
			(const CIEEE802154Address &source,
			 const CIEEE802154Address &destination,
			 const unsigned char nStatus,
			 CIEEE802154SecurityInfo *const pSecurity, void *pArgument);

	// Attributes
	public:
		// Member function to invoke
		CALLBACK m_pfnCallback;

		// Source address to compare (or no address, if source should be
		// ignored)
		CIEEE802154Address m_source;

		// Destination address to compare (or no address, if destination
		// should be ignored)
		CIEEE802154Address m_destination;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkPendingPacket

class CZigBeeNetworkPendingPacket
{
	// Construction
	public:
		CZigBeeNetworkPendingPacket(const CPacket &npdu,
			const unsigned char nHandle, const unsigned char nTransmitOptions);

	// Attributes
	public:
		// Fully assembled NDPU that can be passed to the MAC layer
		CPacket m_npdu;

		// Network layer handle for this NPDU
		unsigned char m_nHandle;

		// MAC layer transmit options
		unsigned char m_nTransmitOptions;

	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkQueuePacket

class CZigBeeNetworkQueuePacket
{
	// Construction
	public:
		CZigBeeNetworkQueuePacket(const CIEEE802154Address &destination,
			const CPacket &msdu, const uint_fast8_t transmitOptions);

	// Attributes
	public:
		// Fully assembled NDPU that can be passed to the MAC layer
		CPacket m_npdu;

		// Destination address for the MSDU
		CIEEE802154Address m_destination;

		// Timer for retransmissions
		CCompactTimer *m_timer;

		// MAC layer transmit options
		uint8_t m_transmitOptions;

		// Number of transmission attempts made for this MSDU
		uint8_t m_attempt;

	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetwork

class CZigBeeNetwork : public CIEEE802154MACClient,
	public CZigBeeNetworkClient, public CCompactTimerClient
{
	// typedefs & enums
	public:
		// Network layer status codes
		enum { statusSuccess = 0, statusAbort = 0xc0,
			statusInvalidParameter = 0xc1,
			statusInvalidRequest, statusNotPermitted, statusStartupFailure,
			statusAlreadyPresent, statusSynchronizationFailure,
			statusNeighborTableFull, statusUnknownDevice,
			statusUnsupportedAttribute, statusNoNetworks,
			statusMaxFrameCounterReached = 0xcc, statusNoKey,
			statusBadCCMOutput, statusNoRoutingCapacity,
			statusRouteDiscoveryFailed, statusRouteError,
			statusBroadcastTableFull, statusFrameNotBuffered,
			statusInvalidInterface = 0xd5, statusMissingTLV, statusInvalidTLV };

	// Construction
	public:
		CZigBeeNetwork(CIEEE802154MAC &mac);

	// Attributes
	public:
		// Link to the IEEE 802.15.4 MAC layer instance (L2)
		CIEEE802154MAC &m_mac;

		// ZigBee application support sub-layer instance (L7)
		CZigBeeApplicationSupport *m_pAPS;

		// ZigBee network layer management entity (NLME)
		CZigBeeNetworkManagement m_nlme;

	// Operations
	public:
		// Returns a bit field with broadcast destination groups this device
		// belongs to
		unsigned char GetBroadcastGroupMask() const;

		// Given the device capabilities, returns the broadcast group mask
		static uint_fast8_t GetBroadcastGroupMask
			(const uint_fast8_t bCapabilities);

		// Returns true, if the specfied address is a broadcast address. The
		// destination group mask is derived from the network information base
		bool IsBroadcast(const unsigned short wAddress) const;

		// Returns an address object with network address and 64-bit extended
		// IEEE address
		CZigBeeAddress GetAddress() const;

		// Calculates a link cost based on the specified LQI value
		static unsigned char CalculateLinkCost(const unsigned char nLinkQuality);

		// Returns the maximum payload size for a data request frame to the
		// specified destination address
		CPacketData::size_type QueryPayloadLimit
			(const CZigBeeAddress &destination = CZigBeeAddress(),
			const bool allowSourceRouting = true);

		// NLDE-DATA.request(DstAddrMode, DstAddr, NsduLength, Nsdu,
		// NsduHandle, Radius, NonmemberRadius, DiscoverRoute, SecurityEnable),
		// clause 3.2.1.1
		// Only use handles returned by CreateDataRequestHandle(). Use them
		// once.
		// Enhanced for ZigBee Green Power (ZigBee Core Stack R21) to support
		// alias addressing for ZigBee Green Power devices. If the calling code
		// provides a source address (alias), it must also provide a sequence
		// number
		void OnRequestData(const CZigBeeAddress &destination, CPacket &nsdu,
			const unsigned char nHandle, const unsigned char nRadius,
			const unsigned char nNonmemberRadius,
			const bool bEnableRouteDiscovery, const bool bEnableSecurity,
			CZigBeeAddress source = CZigBeeAddress(),
			unsigned char nSequence = 0,
			const unsigned char nTransmitOptions = 0,
			const CIEEE802154Address *const pNextHopAddress = 0);

		// Returns a handle that can be used for a subsequent call to
		// OnRequestData(). The caller must invoke OnRequestData() once, each
		// time this method returns true, having generated a new handle
		bool CreateDataRequestHandle(unsigned char &nHandle,
			void *pArgument = 0);

		// Maps the status of TLV parser to a suitable network layer status
		static uint_fast8_t MapStatus(const CZigBeeTLVs &tlvs);

	// Overrides
	public:
		// MCPS-DATA.confirm(msduHandle, status, Timestamp) service primitive,
		// according to clause 7.1.1.2.1
		virtual void OnConfirmData(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp);

		// MCPS-DATA.indication(SrcAddrMode, SrcPANId, SrcAddr, DstAddrMode,
		// DstPANId, DstAddr, msduLength, msdu, mpduLinkQuality, DSN, Timestamp,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex) service primitive,
		// according to clause 7.1.1.2.1
		virtual void OnIndicateData(const CIEEE802154Address &sourceAddress,
			const CIEEE802154Address &destinationAddress, CPacket &msdu,
			const unsigned char nLinkQuality,
			const unsigned char nDataSequence,
			const unsigned int nTimestamp = 0,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

		// MLME-COMM-STATUS.indication(PANId, SrcAddrMode, SrcAddr,
		// DstAddrMode, DstAddr, status, SecurityLevel, KeyIdMode, KeySource),
		// clause 7.1.12.1
		virtual void OnIndicateCommStatus(const CIEEE802154Address &source,
			const CIEEE802154Address &destination, const unsigned char nStatus,
			CIEEE802154SecurityInfo *const pSecurity = 0);

		// MLME-START.confirm(status), clause 7.1.14.2
		virtual void OnConfirmStart(const unsigned char nStatus);

		// MLME-SCAN.confirm(status, ScanType, ChannelPage, UnscannedChannels,
		// ResultListSize, EnergyDetectList, PANDescriptorList),
		// clause 7.1.11.2
		virtual void OnConfirmScan(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect);

		// MLME-BEACON-NOTIFY.indication(BSN, PANDescriptor, PendAddrSpec,
		// AddrList, sduLength, sdu), clause 7.1.5.1
		virtual void OnIndicateBeaconNotify(const unsigned char nSequence,
			const CIEEE802154PANDescriptor &descriptor, CPacket &sdu);

		// MLME-POLL.indication(address). This is an extension to the IEEE
		// 802.15.4 MAC introduced by the ZigBee Alliance core stack for its
		// release 21 and allows a parent router to inject a leave message for
		// child that has changed parents
		virtual bool OnIndicatePoll(const CIEEE802154MHR &header,
			unsigned char &bPendingControl, CPacketEx *&pPending);

		// MLME-POLL.confirm(status), clause 7.1.16.2
		virtual void OnConfirmPoll(const unsigned char nStatus);

		// MLME-ASSOCIATE.confirm(AssocShortAddress, status, SecurityLevel,
		// KeyIdMode, KeySource, KeyIndex), clause 7.1.3.4
		virtual void OnConfirmAssociate(const unsigned short wShortAddress,
			const unsigned char nStatus,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

#ifdef __COMPACT15DOT4_FFD__
		// MLME-ASSOCIATE.indication(DeviceAddress, CapabilityInformation,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.3.2
		virtual void OnIndicateAssociate(const CIEEE802154Address &device,
			const unsigned char bCapabilities,
			CIEEE802154SecurityInfo *const pSecurity = 0);
#endif

		// MLME-DISASSOCIATE.confirm(status, DeviceAddrMode, DevicePANId,
		// DeviceAddress)
		virtual void OnConfirmDisassociate(const unsigned char nStatus,
			const CIEEE802154Address &device);

		// MLME-DISASSOCIATE.indication(DeviceAddress, DisassociateReason,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.4.2
		virtual void OnIndicateDisassociate(const CIEEE802154Address &device,
			const unsigned char nReason,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

#ifdef __COMPACT15DOT4_FFD__
		// MLME-ORPHAN.indication(OrphanAddress, SecurityLevel, KeyIdMode,
		// KeySource, KeyIndex)
		virtual void OnIndicateOrphan(const CIEEE802154Address &orphan,
			const CIEEE802154SecurityInfo *const pSecurity = 0);
#endif

		// Enhancement to the standard IEEE 802.15.4 MLME SAP. Notifies the NHL
		// of the MAC layer being idle, i.e. no packets pending in the RX and
		// TX queues and all other processing has completed
		virtual void OnIndicateIdle();

	// Implementation
	public:
		// Broadcast expiration timer period (6250 symbol periods = 100ms)
		enum { broadcastTimerPeriod = 6250u };

		// Number of network layer frames that will be buffered, e.g. if a
		// route is not available yet and must be discovered first
		static const unsigned int nPendingQueueLimit;

		// Number of network layer frames that will be queued for network
		// level retries. If more frames are queued than allowed by this
		// limit, they will still be forwarded to the MAC layer, but
		// there will not be any network-level retries for these frames
		static const unsigned int queueLimit;

		// Transmit failure threshold. If more than the specified number of
		// consecutive transmission attempts failed at the MAC layer, the link
		// is considered to have failed. This value should not be too low. In
		// case of heavy network load, this would lead to a cascade where more
		// messages are generated (route repair), worsening the problem that
		// caused the link failure
		static const uint8_t nTransmitFailureThreshold;

		// Route discovery threshold between 1 and 7. A neighboring device
		// is entitiled to be the next hop for a pending transmission if
		// the outgoing cost to this device is lower than the threshold.
		// Otherwise, route discovery should be initiated despite the
		// existing neighbor table entry.
		// A threshold of 8 means, that no route discovery is performed
		// as long as the outgoing cost is nonzero.
		static const uint8_t routeDiscoveryThreshold;

		// Dispatch handlers for MCPS-DATA.confirms. At the same time an
		// inventory of handles that are currently in use at the network
		// layer
		std::set<CZigBeeDispatchDataConfirm> m_handles;

		// Dispatch handlers for MLME-COMM-STATUS.indications
		std::list<CZigBeeDispatchCommStatusIndication> m_dispatchStatus;

		// A queue of frames pending route discovery. Notice that this is not
		// only used on routers, but also on end-device, in case the device is
		// currently rejoining and has no active parent while a new frame is
		// queued by the application layer
		std::multimap<unsigned short, CZigBeeNetworkPendingPacket> m_pending;

		// Periodic timer that is called once every 100ms as long as the BTT is
		// not empty. Decrements expiration time and removes out-dated BTRs
		CCompactTimer *m_pBroadcastExpirationTimer;

		// Route discovery table
		std::set<CZigBeeRouteDiscoveryInfo> m_routeDiscovery;

		// Transmit queue for MAC layer frames. These are maintained at the
		// network layer so as to allow for network level retries across the
		// board
		std::map<uint8_t, CZigBeeNetworkQueuePacket> m_queue;

		// Route request ID counter for route request command frames
		unsigned char m_nRequestID;

		// Time duration in ticks that a unicast outbound transmission should
		// be maintained for detecting and resolving potential routing loops
		// when forwarding an inbound network data frame.
		// Considering the maximum number of hops is limited to 30 in Zigbee
		// networks and an average forwarding delay of e.g. 50ms, this period
		// is set to 50ms * 30 = 1.5s by default. This helps to limit the heap
		// consumption for this feature.
		static const uint32_t mruLifetime;

		// Maximum number of most recent unicast outbound transmissions that
		// should be maintained for detecting and resolving potential routing
		// loops when forwarding an inbound network data frame
		static const uint32_t mruLimit;

		// Set of Most Recent Unicast (MRU) outbound transmissions, either
		// locally initiated or forwarded. Each entry in this set contains
		// information about the initiator's network address, sequence number
		// and radius. This information aids in the detection and resolution
		// of potential routing loops when forwarding an inbound network data
		// frame
		std::set<CZigBeeNetworkMRUEntry> m_mrus;

		// Timer utilized for cleaning-up staled entries maintained in the
		// set of most recent unicast outbound transmissions
		CCompactTimer *m_mruMaintenanceTimer;

		// Generates a random time jitter within the specifed range of symbol
		// periods. The returned value is the number of PHY symbols to
		// wait, so this value can be used with the IEEE 802.15.4 symbol timer
		static unsigned int GenerateRandomJitter(const unsigned int nLowerBound,
			const unsigned int nRange);

		// Turns an NSDU into a NPDU by inserting a NWK header in front of the
		// payload. Increments the sequence counter. Pass 0xffffffffffffffff as
		// extended IEEE destination address, if no 64-bit destination address
		// shall be included
		void PrependHeader(CPacket &npdu,
			const unsigned char nType, const CIEEE802154Address &destination,
			const CIEEE802154Address &source, const unsigned char nSequence,
			const unsigned char nRadius, const unsigned char nNonmemberRadius,
			const bool bEnableRouteDiscovery, const bool bEnableSecurity);

		// Transmits a data or command frame. Adds an appropriate NHR to the
		// NSDU and instructs the MAC to transmit the resulting NPDU with the
		// specified transmit options (and an optional next hop override).
		void Transmit(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, const unsigned char nSequence,
			CPacket &nsdu, const unsigned char nHandle,
			const unsigned char bRadius, const unsigned char bNonmemberRadius,
			const bool bEnableRouteDiscovery, const bool bEnableSecurity,
			const unsigned char nType, const unsigned char nTransmitOptions = 0,
			const CIEEE802154Address *const pNextHopAddress = 0);

		// Returns a handle that can be used for a subsequent invocation of
		// MCPS-DATA.request(). The caller must invoke MCPS-DATA.request()
		// once, each time this method returns true, having generated a new
		// handle
		bool CreateDataRequestHandleEx(unsigned char &nHandle,
			CZigBeeNetworkClient *const pClient,
			const CZigBeeDispatchDataConfirm::CALLBACK pfnCallback,
			void *pArgument);

		// Releases the specified handle, so it can be reused for new requests
		void ReleaseHandle(const unsigned char nHandle);

		// Issues an NLDE-DATA.confirm to the next-higher layer and maintains
		// link state by calling MaintainLinkState()
		void ConfirmData(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp = 0);

		// Should be called for any confirmation from the MAC conveying the
		// status of a transmission attempt, including normal data transfers
		// and special ones like MAC data poll
		void MaintainLinkState
			(const std::multiset<CZigBeeNeighborInfo>::iterator &neighbor,
			const unsigned char nStatus);

		// Should be called for any confirmation from the MAC conveying the
		// status of a transmission attempt, including normal data transfers
		// and special ones like MAC data poll. This version should be called
		// when the short address is available and a lookup in the neighbor
		// table should be done automatically
		void MaintainLinkState(const unsigned short wNextHop,
			const unsigned char nStatus);

		// Subscription to MCPS-DATA.confirm(msduHandle, status, Timestamp)
		// for MAC frames generated in response to a NLDE-DATA.request
		void OnConfirmDataData(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Subscription to MCPS-DATA.confirm(msduHandle, status, Timestamp)
		// for MAC frames generated as a result of a network-wide broadcast
		void OnConfirmDataBroadcast(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Subscription to MCPS-DATA.confirm(msduHandle, status, Timestamp)
		// for MAC frames generated as a result of a network-wide broadcast,
		// which are translated into indirect unicasts to sleepies
		void OnConfirmDataBroadcastDispatch(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Returns true if a BTR exists for the specified frame
		bool HaveBroadcastTransaction(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header);

		// Performs a quick broadcast inspection. This will be typically used
		// to avoid unsecuring a frame that is going to be dropped anyway due
		// to an existing BTR record. Also maintains the set of neighbors that
		// have relayed the frame (passive acknowledgments)
		bool InspectBroadcast(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, const CPacket &npdu);

		// Performs all broadcast related actions, i.e. creates or updates BTR,
		// initiates retransmission, etc. Returns true, if the frame should
		// be passed to the next higher layer, false otherwise. Also called
		// when initiating a broadcast (i.e. not only in the receive but also
		// in the transmit path).
		bool ProcessBroadcast(const CIEEE802154Address &sourceAddress,
			const CZigBeeNHR &header, const CPacket &npdu,
			unsigned char *const pnHandle = 0, bool *const pbHaveRecord = 0);

		// Translates a broadcast NPDU into a number of indirect unicast
		// transmissions for all sleeping end-devices of this node and queues
		// these frames at the MAC layer
		void DispatchToSleepingEndDevices(const CPacket &npdu);

		// Translates a broadcast NPDU into a number of tunneled transmissions
		// for all virtual devices of this node and queues these frames at the
		// tunnel data service
		void DispatchToTunneledDevices(const CPacket &npdu);

		// Passes the NPDU to the MAC layer after a random broadcast jitter
		// period. Returns false, if the retransmission retry limit has been
		// reached, true otherwise
		void RetransmitBroadcast(CZigBeeBroadcastTransaction &btr);

		// Timer callback. Called after the random broadcast jitter has elapsed
		// and the frame can be passed to the MAC layer for transmission
		void OnBroadcastJitterElapsed(void *pArgument);

		// Timer callback. Called nwkPassiveAckTimeout after a broadcast
		// retransmission
		void OnPassiveAcknowledgmentTimeout(void *pArgument);

		// Timer callback. Invoked once every second to decrement the lifetime
		// timers of each broadcast entry and remove expired records
		void OnExpireBroadcastTransactions(void *pArgument);

		// Returns true, if all neighbors have retransmitted the broadcast
		// frame, i.e. the passive acknowledgment succeeded
		bool HaveAllNeighborsRelayed(const CZigBeeBroadcastTransaction &btr);

		// Checks the source route cache for an entry referring to the supplied
		// destination address and updates the (ephemeral) CZigBeeRoute object
		// with the immediate next hop and also provides the source route - in
		// reverse order - to the destination. Notice that the NHR conveys the
		// source route in the same order as it is provided here, i.e. the
		// relay closest to the destination is listed first; the relay closest
		// to the originator is listed last
		bool LookupSourceRoute(const CZigBeeAddress &destination,
			CZigBeeRoute &route,
			std::vector<unsigned short> &reverseSourceRoute);

		// Given a frames destination address, this method returns true, if it
		// has a next hop towards the destination. If source routes are allowed
		// to be used, provide a vector which receives the reverse source route
		bool GetNextHop(const CZigBeeAddress &destination, CZigBeeRoute &route,
			unsigned char &nTransmitOptions,
			const bool bEnableRouteDiscovery = true,
			std::vector<unsigned short> *const pSourceRoute = 0);

		// Adds a weak, provisional default route for the specified destination
		// using the specified next-hop. Will not override an existing viable
		// routing path, but just provide a fall-back in case there is no route
		// available
		bool SupplyDefaultRoute(const CZigBeeAddress &destination,
			const uint_fast16_t nextHop);

		// Determines whether the route discovery table entry can take over
		// ownership of the routing table entry. Also returns true, if the
		// route discovery table already possesses the lock
		static bool CanLock(const std::set<CZigBeeRoute>::iterator &route,
			CZigBeeRouteDiscoveryInfo *const pDiscoveryInfo);

		// Creates a lock on the routing table entry for the given owning route
		// discovery table entry
		static void AcquireLock(std::set<CZigBeeRoute>::iterator &route,
			CZigBeeRouteDiscoveryInfo *const pDiscoveryInfo);

		// Releases the lock on the routing table entries tied to this route
		// discovery table entry
		void ReleaseLocks(const CZigBeeRouteDiscoveryInfo *const pDiscoveryInfo);

		// Creates or updates a routing table entry for the specified
		// destination. The caller may prevent an existing active route being
		// updated by calling the method with bUpdateExisting set as false.
		// In any case, if the route is currently locked by a route discovery
		// table entry (and the owner is not the caller), the routing table
		// entry will not be modified in order to avoid potential routing loops
		// when concurrent route discoveries are in progress where either the
		// originator or the responder appear in multiple route discovery
		// requests
		std::set<CZigBeeRoute>::iterator
		MaintainRoute(const CZigBeeAddress &destinationAddress,
			const unsigned char nStatusAndFlags,
			const unsigned short wNextHop = CIEEE802154Address::wBroadcast,
			const bool bForceUpdateExisting = false, bool *const bNew = 0,
			bool *const bModified = 0,
			CZigBeeRouteDiscoveryInfo *const pDiscoveryInfo = 0);

		// Returns a route discovery table entry for the specified source and
		// sequence number. Returns the iterator and an indication whether the
		// entry existed before (false) or has just been created (true)
		std::pair<std::set<CZigBeeRouteDiscoveryInfo>::iterator, bool>
		ObtainDiscoveryInfo(const unsigned short wSourceAddress,
			const unsigned char nRequestID, const bool bAllowCreate = true);

		// Tries to discover a route to the specified destination. Returns
		// true, if route discovery has been initiated and shall continue,
		// false otherwise. In particular, false is returned when a route
		// request is being processed where the path cost is inferior to what
		// was previously available
		bool DiscoverRoute(const CZigBeeAddress &destinationAddress,
			const unsigned short wSourceAddress, const unsigned char nRequestID,
			const CIEEE802154Address &sender, const unsigned char nSequence,
			const unsigned char nRadius, const unsigned char nOptions,
			const CZigBeeRouteDiscoveryInfo::CALLBACK pfnComplete = 0,
			void *pArgument = 0,
			const CZigBeeRouteRequestCommand *const pRequest = 0);

		// Returns true, if a route discovery is in progress for the given
		// address, which originated at the local device. This will help
		// avoiding route requests for targets for which a route is already
		// being searched
		bool IsRouteDiscoveryInProgress(const CZigBeeAddress &address) const;

		// Forwards packets which were pending a route to their intended next
		// hop, or drops the frame if no next hop could be identified. This is
		// always called by the network layer, before invoking the specific
		// route discovery callbacks in the course of normal processing
		void OnRouteDiscoveryComplete(const unsigned char nStatus,
			const unsigned char nNetworkStatus,
			const CZigBeeRoute *const pRoute);

		// Delivers a frame using the routing service and returns the options
		// of the route, which has been used. These options can be stored and
		// evaluated in case of a link failure in order to quickly determine
		// which kind of route has failed and which network status code to
		// generate (if any)
		void Deliver(CPacket &npdu, const unsigned char nHandle,
			const bool bEnableRouteDiscovery, const bool bAllowSourceRouting,
			unsigned char nTransmitOptions,
			const CIEEE802154Address *const pNextHopAddress = 0,
			CZigBeeRoute *const pRoute = 0);

		// Forwards a frame to the next hop on its way to the final destination
		void DeliverToNextHop(CPacket &npdu, const uint_fast8_t handle,
			const uint16_t nextHop, const uint8_t transmitOptions);

		// Forwards a frame to the next hop on its way to the final destination
		void DeliverToNextHop(CPacket &npdu, const uint_fast8_t handle,
			CIEEE802154Address &nextHop, uint8_t transmitOptions);

		// Queues a frame that is ready for delivery to the MAC layer for
		// transmission. This queue is to allow for network-level retries
		// of outbound data frames towads teh MAC layer
		void QueueData(const CIEEE802154Address &destination, CPacket &npdu,
			const uint_fast8_t handle, const uint_fast8_t options);

		// Delivers a frame to the MAC layer for transmission. Notice that this
		// is for any on-network transmission, i.e. excluding inter-PAN
		void RequestData(const CIEEE802154Address &destination, CPacket &npdu,
			const uint_fast8_t handle, const uint_fast8_t options,
			const bool finalizeSecurity = true);

		// Performs a MCPS data retry after a certain period of time
		void OnDataRetryDelayExpired(void *pArgument);

		// MCPS-DATA.confirm handler for a frame that has been transmitted
		// as a result of multihop forwarding
		void OnConfirmDataForward(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Invalidates the specified routing table entry. If the entry is being
		// referenced by a route discovery table entry and force is true, then
		// the route will be marked as inactive, the entry remains and
		// InvalidateRoute() returns false. Otherwise the entry will be deleted
		// and InvalidateRoute() returns true, setting the route iterator one
		// past the removed entry
		bool InvalidateRoute(std::set<CZigBeeRoute>::iterator &route,
			const bool bForce = false);

		// Invalidates/removes the route to the specified device, if any.
		// Retruns true if a routing table entry existed and was invalidated
		bool InvalidateRoute(const CZigBeeAddress &address,
			const bool bForce = false);

		// If the local station is notified about a tree or non-tree link
		// failure, this method must be called in order to invalidate the
		// according routing table entry (end-to-end route repair)
		void OnNotifyLinkFailure(const CZigBeeAddress &address);

		// If the local station detects a broken link and it still has an entry
		// in the neighbor table, this method must be called in order to
		// clear the outgoing cost. Optionally, also invalidate all routing
		// table entries which use the specified link as their next hop target
		void OnLinkFailure(const std::multiset<CZigBeeNeighborInfo>::iterator
			neighbor, const bool invalidateRoutes = false);

		// If the local station detects a broken link, this method must be
		// called in order to invalidate all routing table entries which use
		// the specified link as their next hop target
		void OnLinkFailure(const unsigned short wNextHop);

		// If the local station detects a broken link due to not receiving
		// response during end-device timeout negotiation, this method should
		// be called to indicate the parent link failure
		void OnParentLinkFailure();

		// Takes an unsecured NPDU and adds security (auxiliary header)
		bool PrependAuxiliaryHeader(CPacket &npdu,
			CPacketData::size_type &cbHeaders,
			const CZigBeeNetworkSecurityMaterial &material);

		// Takes an unsecured NPDU and adds security (auxiliary header).
		// Returns status codes suitable for confirmations in case of errors
		bool PrependAuxiliaryHeader(CPacket &nsdu,
			CPacketData::size_type &cbHeaders,
			CZigBeeNetworkSecurityMaterial &material,
			const bool bEnableSecurity, unsigned char &nStatus);

		// Takes an unsecured NPDU, encrypts the payload and adds the message
		// integrity code. The frame counter provided in the security material
		// will be used to update the auxiliary header (which must be present)
		bool EncryptAndAppendIntegrityCode(CPacket &npdu,
			const CPacketData::size_type cbHeaders,
			const CZigBeeSecurityMaterial &material);

		// Takes an unsecured NPDU, encrypts the payload and adds the message
		// integrity code. Returns status codes suitable for confirmations in
		// case of errors. Use this version if the size of NWK and AUX headers
		// is readily available
		bool EncryptAndAppendIntegrityCode(CPacket &npdu,
			CPacketData::size_type &cbHeaders,
			const CZigBeeSecurityMaterial &material,
			const bool bEnableSecurity, unsigned char &nStatus);

		// Takes an unsecured NPDU, encrypts the payload and adds the message
		// integrity code if necessary. Obtains a fresh frame counter using the
		// active network security material
		bool EncryptAndAppendIntegrityCode(CPacket &npdu,
			unsigned char &nStatus);

		// Takes a secured NPDU, decrypts the payload and verifies the message
		// integrity code
		bool DecryptAndAuthenticate(CPacket &npdu, const CZigBeeNHR &header,
			const CIEEE802154Address &source,
			const CIEEE802154Address &destination);

		// Applies low-level filtering and link quality maintenance based on
		// MAC addressing material. These checks are performed prior to any
		// security processing. Returns false if frame should be dropped
		// without further processing
		bool ApplyLowLevelExaminations(const CIEEE802154Address &source,
			const CIEEE802154Address &destination,
			const unsigned char nLinkQuality);

		// Default implementation of ApplyLowLevelExaminations(), which
		// should be called by application-specific overloads of
		// ApplyLowLevelExaminations() in addition to any custom logic
		// provided by the application
		bool DefaultApplyLowLevelExaminations(const CIEEE802154Address &source,
			const CIEEE802154Address &destination,
			const unsigned char nLinkQuality);

		// Applies high-level filtering after successful security processing.
		// Returns false if frame should be dropped without further processing
		bool ApplyHighLevelExaminations(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, CZigBeeNHR &header,
			CPacket &nsdu);

		// Applies end-device related filtering and maintenance
		bool ApplyEndDeviceExaminations(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, CZigBeeNHR &header,
			CPacket &msdu);

		// Sets or clears the end-device initiator flag, based on the device's
		// role (coordinator/router or end-device)
		void FixupEndDeviceInitiator(CPacket &npdu) const;

		// Sets or clears the end-device initiator flag according to the flag
		// provided as argument
		static void FixupEndDeviceInitiator(CPacket &npdu,
			const bool bEndDevice);

		// Takes secured or unsecured NPDU and creates a retransmittable copy
		// for network-wide broadcasts or unicast forwarding. Incoming security
		// headers are removed, and outgoing security headers with local
		// security credentials added. The packet will not be encrypted and the
		// integrity code will not be appended, unless explicitly requested
		bool CreateRetransmitCopy(CPacket &copy, const CPacket &npdu,
			unsigned char &nStatus,
			const bool bEncryptAndAppendIntegrityCode = false,
			const CPacketData::size_type cbTrailerExtra = 0,
			const CPacketData::size_type cbHeaderExtra = 0);

		// Allows for finishing touches on a packet that needs to be forwarded.
		// Typically the right place to modify the source routing index and
		// append to the route record list. In case of a source-routed frame,
		// also provides the next hop that shall be used for forwarding
		bool PreparePacketForwarding(CPacket &prepared, const CPacket &npdu,
			CIEEE802154Address &nextHop, unsigned char &nTransmitOptions);

		// This must be called for incoming frames to maintain the route record
		// required flag of many-to-one routes
		void MaintainManyToOneRoute(const CZigBeeNHR &header);

		// Inserts a source routing relay list into the network header. This
		// might lead to a reallocation of the packet object and the header
		// reference becomes void
		static void EmplaceSourceRoute(CPacket &npdu, CZigBeeNHR &header,
			const std::vector<unsigned short> &sourceRoute);

		// Timer callback that is invoked when the device should attempt to
		// forward a multihop packet to its next hop
		void OnPacketForwardingDelayExpired(void *pArgument);

		// Network layer glue to stub APS (inter-PAN messaging)
		void OnRequestDataStub(const CZigBeeAddress &destination,
			CPacket &nsdu, void *pArgument);

		// MCPS-DATA.confirm handler for inter-PAN messages (stub APS)
		void OnConfirmDataStub(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Network layer glue to stub APS (inter-PAN messaging)
		void OnIndicateDataStub(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Network layer glue to GP stub (ZigBee Green Power). This initial
		// preparation stage attempts to put the GPDF at the top of the MAC TX
		// queue and arm the MAC for time-controlled transmission of the GPDF.
		// Notice that this will block the outgoing TX queue until the frame is
		// actually tramsmitted by calling OnRequestDataGP2(). The MAC layer
		// might not be able to service this request and will confirm with a
		// status of "DENIED" in this case
		void OnRequestDataGP1(const CZigBeeAddress &destination,
			CPacket &nsdu, void *pArgument = 0);

		// Network layer glue to GP stub (ZigBee Green Power). This final stage
		// must be invoked when the GP application object wants the frame to
		// appear "on-air" immediately
		void OnRequestDataGP2();

		// MCPS-DATA.confirm handler for ZigBee Green Power messages (GP stub)
		void OnConfirmDataGP(const unsigned char nHandle,
			const unsigned char nStatus, const unsigned int nTimestamp,
			void *pArgument);

		// Network layer glue to GP stub (ZigBee Green Power)
		void OnIndicateDataGP(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, CPacket &nsdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp);

		// Sends the supplied NPDU over a trusted link
		void OnRequestDataTunnel(CPacket &npdu, const uint_fast8_t handle,
			const uint_fast8_t tunnelIdentifier);

		// Processes confirmation for the transmission of an NPDU over a
		// trusted link
		void OnConfirmDataTunnel(const unsigned char nHandle,
			const unsigned char nStatus);

		// Process inbound traffic received over a trusted link
		void OnIndicateDataTunnel(CPacket &npdu);

		// Given a network address, returns a tunnel identifier if this address
		// pertains to a device using a trusted link, or 0 if it is a device
		// connected via a native IEEE 802.15.4 link
		uint_fast8_t ResolveTunnelIdentifier(const CIEEE802154Address &address);

		// Given a network address, determines whether the address is that of a
		// trusted link device, and if so, adjusts the address flags as needed
		void FixupTunnel(CIEEE802154Address &address, uint8_t &transmitOptions);

		// Determines link-layer transmit options for a specific route
		void DetermineTransmitOptions(const CZigBeeRoute &route,
			uint8_t &transmitOptions);

		// Called for all incoming network frames (except Green Power and
		// inter-PAN), which passed applicable security processing, relevant
		// filtering and inspection. Allows implementation of additional
		// filtering, early network packet inspection (including frames not
		// destined to the receiving node), tracking network traffic, etc.
		static bool PreprocessData(CZigBeeNetwork &nwk,
			const CIEEE802154Address &source,
			const CIEEE802154Address &destination, CPacket &msdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp);

		bool PreprocessData(const CIEEE802154Address &source,
			const CIEEE802154Address &destination, CPacket &msdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp);

		// Second stage of incoming data processing. At this point the NPDU is
		// trusted (if the frame was secured), i.e. it has passed incoming
		// network security processing and the data is unencrypted
		void OnIndicateData2(const CIEEE802154Address &sourceAddress,
			const CIEEE802154Address &destinationAddress, CPacket &msdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp,
			const CIEEE802154SecurityInfo *const pSecurity);

		// Schedules the broadcast expiration timeout timer by looking at the
		// broadcast transaction's next due timeout in the the broadcast table.
		// The broadcast entry having the soonest timeout sets the expiration
		// timeout for the remaining entries.
		void ScheduleBroadcastExpirationTimer(const CCompactTimer *pTimer = 0);

		// Refresh all pending timeouts in the broadcast table based on how
		// much time has elapsed since the timer was started. This needs to be
		// called whenever a running timer is cancelled and replaced with a new
		// one, or the timer fires. This is to called exclusively by
		// ScheduleBroadcastExpirationTimer(). Do not call directly
		void RefreshBroadcastExpirationTimes(const uint_fast32_t elapsed);

		// Fix-up remaining time based on whether a timer is currently active
		uint_fast32_t FixupTimeout(const uint_fast32_t timeSpan) const;

		// Schedule the maintenance timer for monitored unicast outbound data
		// transmissions by evaluating each entry's remaining tracking period.
		// The transmission entry with the soonest tracking period end
		// determines the timeout duration for the tracking timer
		void ScheduleMRUMaintenanceTimer();

		// Refresh the maintenance periods of recent unicast outbound data
		// transmissions that are being tracked for detecting and resolving
		// potential routing loops when forwarding an inbound network data
		// frame
		void RefreshMRULifetimes(const CCompactTimer &timer);

		// Timer callback, invoked when a monitored unicast outbound data
		// transmission's maintenance period expires
		void OnMRUMaintenanceDue(void *pArgument);

		// Drop all pending frames and confirm with the "Abort" status if
		// necessary
		void DropPendingFrames(const bool confirm = true);

#ifdef _DEBUG
	public:
		// Dumps NWK tables and state information
		void Dump() const;

		// In debug builds, shows the contents of the route discovery table
		void DumpRouteDiscoveryTable() const;

		// In debug builds, dumps the tracked unicast outbound data
		// transmissions
		void DumpMRUs() const;

		// Returns the human-readable name of a network layer status value
		// according to the ZigBee standard
		static const char *GetStatusString(const unsigned char nValue);

		// Checks if the routing table is valid
		bool IsRoutingTableValid
			(std::set<CZigBeeRoute>::const_iterator *const pRoute = 0) const;

		// Checks if the route discovery table is valid
		bool IsRouteDiscoveryTableValid() const;

		// Executes a sanity check and asserts or warns about potential
		// inconsistencies
		void AssertValid() const;
#endif

};

inline bool CZigBeeNetwork::IsBroadcast(const unsigned short wAddress) const
	{ return CZigBeeAddress::IsBroadcast(wAddress, GetBroadcastGroupMask()); }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkPreprocessDataDelegate

class CZigBeeNetworkPreprocessDataDelegate
{
	// typedefs & enums
	public:

	// Attributes
	public:
		// Callback for data preprocessing
		bool (*m_preprocess)(CZigBeeNetwork &nwk,
			const CIEEE802154Address &source,
			const CIEEE802154Address &destination, CPacket &msdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp);

	// Operations
	public:
		// Preprocess an inbound frame
		static bool Preprocess(CZigBeeNetwork &nwk,
			const CIEEE802154Address &source,
			const CIEEE802154Address &destination, CPacket &msdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp);
};


#define __NWK_PREPROCESS_DELEGATE_IMPLEMENT_EX(id, ...) \
	__COMPACT_SECTION_IMPLEMENT(CZigBeeNetworkPreprocessDataDelegate, \
		nwk_preprocess, id, __VA_ARGS__)

#define __NWK_PREPROCESS_DELEGATE_IMPLEMENT(identifier, ...) \
	__NWK_PREPROCESS_DELEGATE_IMPLEMENT_EX( \
	__nwk_preprocess_data##identifier, identifier, __VA_ARGS__)
