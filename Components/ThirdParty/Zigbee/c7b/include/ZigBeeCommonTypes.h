// ZigBeeCommonTypes.h : header file
//
// Copyright(C) 2008-2020 ubisys technologies GmbH, Duesseldorf, Germany.
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
// This module defines common classes and types frequently used in the
// Compact7B implementation of ZigBee 2007

/////////////////////////////////////////////////////////////////////////////
// CZigBeeAddress

class CZigBeeAddress : public CIEEE802154Address
{
	// typedefs & enums
	public:
		// In addition to the types declared in the base class, a third type is
		// available for network addressing (group addresses) and a flag has
		// been introduced that denotes whether the endpoint identifier field
		// in the present address object is valid. With the addition of ZigBee
		// Green Power, another type has been added: Device ID is a new type
		// used to convey a 32-bit ZigBee Green Power Device ID (GPDID). This
		// will be available in the lower 32-bits of m_qwExtended. A 'custom'
		// flag can be used at the application's discretion and its meaning
		// depends on the context where it is being used. For example, it will
		// be set to true by the Green Power sink in the source address of
		// data indications, which are forwarded to the application layer in the
		// course of translating Green Power to classic ZCL. In that case, the
		// address type will be none (to prevent any over-the-air messages),
		// but applications looking at the custom flag would know which GPD has
		// originated the translated message. Notice that the 'don't compare
		// endpoint' flag overloads the MAC layer's PAN-ID valid flag. This is
		// acceptable, because this flag is only used in ephemeral address
		// objects, which are never passed down the stack. A further use case
		// has been added in R23, where custom flag 2 denotes an address object
		// of type 'extended address' for a device that has not fully joined
		// the network yet and uses the APS relay for all communications. Such
		// a device is not authenticated, nor authorized, and only limited
		// functionality must be made available, e.g. the ability to negotiate
		// a link key with the trust center, or interview the device prior to a
		// decision as to whether or not it is going to be permitted onto the
		// network. In this case, the network short address field, albeit not
		// marked as valid, contains either 0xffff to denote local relay, or a
		// valid short address, to indicate remote relay. Additionally, an R23
		// device that also supports secure NPDU, in particular a Zigbee Direct
		// Device (ZDD), or Zigbee Direct Virtual Device (ZVD), will have the
		// custom flag 3 set for tunneled connections. This is overloaded with
		// the 32-bit device ID type for Green Power devices
		enum { typeGroup = 1, typeNetwork = typeShort, typeDeviceID = 8,
			typeCustomFlag = 0x10, typeCustomFlag2 = 0x20, typeCustomFlag3 = 8,
			typeDontCompareEndpointFlag = 0x40, typeEndpointFlag = 0x80 };

		// Destination group selector mask
		enum { broadcastReceiverOnWhenIdle = 0x01,
			broadcastRoutersAndCoordinator = 0x02,
			broadcastLowPowerRouters = 0x04,
			broadcastAny = 0x07 };

		// Special endpoint address for a broadcast to all endpoints
		enum { endpointBroadcast = 255 };

	// Construction
	public:
		CZigBeeAddress();

		CZigBeeAddress(const CIEEE802154Address &address);

		CZigBeeAddress(const char *const address);

		CZigBeeAddress(const uint16_t wAddress,
			const uint8_t nType = typeNetwork);

		CZigBeeAddress(const uint64_t qwExtendedAddress,
			const uint16_t wNetworkAddress);

		CZigBeeAddress(const uint_fast64_t qwExtendedAddress,
			const uint_fast16_t wNetworkAddress, const uint_fast8_t nEndpoint,
			const uint_fast8_t nType = static_cast<uint_fast8_t>(typeBoth) |
				typeEndpointFlag);

		CZigBeeAddress(const CZigBeeAddress &address,
			const uint_fast8_t nEndpoint);

	// Attributes
	public:
		// Broadcast address for all devices in a PAN
		static const uint16_t wBroadcastAll;

		// Broadcast address for all devices, which have their receivers on
		// during idle periods
		static const uint16_t wBroadcastReceiverOnWhenIdle;

		// Broadcast address for all routers and the coordinator
		static const uint16_t wBroadcastRoutersAndCoordinator;

		// Broadcast address for low-power routers
		static const uint16_t wBroadcastLowPowerRouters;

		// This address object can be used to address a specific end-point.
		// Note that the endpoint will not be used for address comparisons. It
		// is only meant allow passing addressing material along in the stack
		// as a single entity
		uint8_t m_nEndpoint;

	// Operations
	public:
		// Returns true, if the specified address is either the address of the
		// ZigBee Coordinator and Trust Center (0x0000), or belongs to the
		// range of special broadcast and reserved addresses (0xfff8..0xffff)
		static bool IsSpecial(const uint_fast16_t wAddress);

		// Returns true, if the specified address is a broadcast address. The
		// destination group mask can be specified
		static bool IsBroadcast(const uint16_t wAddress,
			const uint_fast8_t bGroupMask = broadcastAny);

		// Returns true, if this object holds broadcast address, subject to
		// the specified broadcast group
		bool IsBroadcast(const uint8_t bGroupMask = broadcastAny) const;

		// Returns true, if this object holds a broadcast or multicast address
		bool IsMulticast() const;

		// Returns true, if GetEndpoint() would return a valid endpoint
		bool IsEndpointValid() const;

		// Returns true, if the endpoint shall be compared by operator== and
		// operator<
		bool ShallCompareEndpoint() const;

		// Returns the endpoint identifier part of the address. Must only be
		// called when IsEndpointValid() returns true
		uint_fast8_t GetEndpoint() const;

		// Sets the endpoint identifier part of the address. Notice that the
		// PANID becomes void as a matter of fact
		void SetEndpoint(const uint_fast8_t nEndpoint);

		// Returns the 32-bit Green Power device identifier, if applicable
		uint32_t GetDeviceID() const;

		// Equality check. Handles group addresses and 32-bit Green Power
		// device identifiers. Other comparisons are left to the base class
		// operator
		bool operator==(const CZigBeeAddress &address) const;

		// Inequality check. Calls the equality operator and negates the result
		bool operator!=(const CZigBeeAddress &address) const;

		// Less check. Handles group addresses and 32-bit Green Power
		// device identifiers. Other comparisons are left to the base class
		// operator. This enables CZigBeeAddress being used as a key in STL
		// containers like set and map
		bool operator<(const CZigBeeAddress &address) const;

	// Implementation
	public:
		// Convert address object to a human readable string representation
		operator std::string() const;
};

inline bool CZigBeeAddress::IsSpecial(const uint_fast16_t wAddress)
	{ return !wAddress || (wAddress > 0xfff7); }

inline bool CZigBeeAddress::IsBroadcast(const uint8_t bGroupMask) const
	{ return (GetType() == typeNetwork) && IsBroadcast(m_wShort, bGroupMask); }

inline bool CZigBeeAddress::IsMulticast() const
	{ return (GetType() == typeGroup) || IsBroadcast(); }

inline bool CZigBeeAddress::IsEndpointValid() const
	{ return (IsShortValid() || IsExtendedValid()) && (m_nType & typeEndpointFlag); }

inline bool CZigBeeAddress::ShallCompareEndpoint() const
	{ return IsEndpointValid() && !(m_nType & typeDontCompareEndpointFlag); }

inline uint_fast8_t CZigBeeAddress::GetEndpoint() const
	{ return m_nEndpoint; }

inline void CZigBeeAddress::SetEndpoint(const uint_fast8_t nEndpoint)
	{ m_nEndpoint = nEndpoint; m_nType |= typeEndpointFlag; }

inline uint32_t CZigBeeAddress::GetDeviceID() const
{
	ASSERT((GetTypeEx() == typeDeviceID) || (GetTypeEx() == typeNone));

	return static_cast<uint32_t>(m_qwExtended);
}

inline bool CZigBeeAddress::operator!=(const CZigBeeAddress &address) const
	{ return !(*this == address); }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeBindingSource

class CZigBeeBindingSource
{
	// Construction
	public:
		CZigBeeBindingSource();

		CZigBeeBindingSource(const uint_fast64_t qwAddress,
			const uint_fast8_t bEndpoint, const uint_fast16_t wClusterID);

	// Attributes
	public:
		// Device address (typically own address, except for primary and backup
		// binding table caches). A broadcast address of "all ones" indicates
		// the end of a binding table. REVISIT: If not used as cache,
		// the 64-bit source address can be removed
		uint64_t m_qwAddress;

		// Cluster ID
		uint16_t m_wClusterID;

		// Endpoint
		uint8_t m_bEndpoint;

	// Operations
	public:
		// Operator providing STL sorted container support
		bool operator<(const CZigBeeBindingSource &source) const;

		// Checks for equality
		bool operator==(const CZigBeeBindingSource &source) const;

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps the binding source
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeBindingTarget

class CZigBeeBindingTarget
{
	// Construction
	public:
		CZigBeeBindingTarget();

		CZigBeeBindingTarget(const CZigBeeAddress &address);

		CZigBeeBindingTarget(const uint_fast16_t wGroupAddress);

		CZigBeeBindingTarget(const uint_fast64_t qwExtendedAddress,
			uint_fast8_t bEndpoint);

	// Attributes
	public:
		union
		{
			// Device address, unique 64-bit IEEE hardware address
			uint64_t m_qwExtendedAddress;

			// Group address
			uint16_t m_wGroupAddress;
		};

		// Endpoint (applies to extended address, only)
		uint8_t m_bEndpoint;

		// Type of address (CZigBeeAddress::typeGroup or typeExtended)
		uint8_t m_bType;

	// Operations
	public:
		// Checks for equality
		bool operator==(const CZigBeeBindingTarget &target) const;

		// Constructs and returns an appropriate CZigBeeAddress object
		operator CZigBeeAddress() const;

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps the binding target
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNeighborTableEntry

#pragma pack (push, 1)

class CZigBeeNeighborTableEntry
{
	// typedefs & enums
	public:
		// Device type and relation
		enum { typeCoordinator = 0, typeRouter = 1, typeEndDevice = 2,
			typeUnknown = 3, typeMask = 0x03, receiverOnWhenIdle = 0x04,
			receiverModeUnknown = 0x08, receiverModeMask = 0x0c,
			relationParent = 0, relationChild = 0x10, relationSibling = 0x20,
			relationNone = 0x30, relationPreviousChild = 0x40,
			relationUnauthenticatedChild = 0x50, relationMask = 0x70 };

		// Flags
		enum { flagPermitJoiningEnabled = 0x01,
			flagPermitJoiningUnknown = 0x02 };

	// Attributes
	public:
		// The 64-bit extended PAN identifier of the neighboring device
		uint64_t m_qwExtendedPANID;

		// The 64-bit extended IEEE address that is unique to every device. If
		// unknown at the time of the request equal to 0xffffffffffffffff
		uint64_t m_qwExtendedAddress;

		// The 16-bit network address of the neighboring device
		uint16_t m_wNetworkAddress;

		// The type of the neighboring device and its relation to this device
		uint8_t m_nType;

		// Flags (such as permit joining currently enabled, disabled, etc.)
		uint8_t m_nFlags;

		// Depth (hops from coordinator). Not used in ZigBee PRO
		uint8_t m_nDepth;

		// Estimated link quality for RF transmissions from this device
		uint8_t m_nLinkQuality;

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps the neighbor information
		void Dump() const;
#endif

};

#pragma pack (pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRoutingTableEntry

#pragma pack (push, 1)

class CZigBeeRoutingTableEntry
{
	// typedefs & enums
	public:
		// Possible values and flags for the route status field
		enum { statusActive, statusDiscoveryUnderway, statusDiscoveryFailed,
			statusInactive, statusValidationUnderway, statusMask = 0x07,
			memoryConstrained = 0x08, manyToOne = 0x10,
			routeRecordRequired = 0x20 };

	// Attributes
	public:
		// The 16-bit network address or Group ID of this route
		uint16_t m_wDestinationAddress;

		// The status and flags of the route
		uint8_t m_nStatusAndFlags;

		// The 16-bit network address of the next hop on the way to the
		// destination
		uint16_t m_wNextHop;

#ifdef _DEBUG
	public:
		// Dumps this routing table entry
		void Dump() const;
#endif

};

#pragma pack (pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkDescriptor

class CZigBeeNetworkDescriptor
{
	friend class CZigBeeNetworkManagement;
	friend class CZigBeeNetworkScanManager;
	friend class CZigBeeApplicationSupportManagement;
	friend class CZigBeeNeighborInfo;

	// Attributes
	public:
		// The 64-bit PAN identifier of the network
		uint64_t m_qwExtendedPANID;

		// The current logical channel occupied by the network
		uint8_t m_nLogicalChannel;

		// A ZigBee stack profile identifier indicating the stack profile in
		// use in the discovered network
		uint8_t m_nStackProfile;

		// The version of the ZigBee protocol in use in the discovered network
		uint8_t m_nZigBeeVersion;

		// This specifies how often the MAC sub-layer beacon is to be
		// transmitted by a given device on the network
		uint8_t m_nBeaconOrder;

		// For beacon-oriented networks, that is, beacon order < 15, this
		// specifies the length of the active period of the superframe
		uint8_t m_nSuperframeOrder;

		// A value of TRUE indicates that at least one ZigBee router on the
		// network currently permits joining, i.e. its NWK has been issued an
		// NLME-PERMIT-JOINING primitive and, the time limit if given, has not
		// yet expired
		bool m_bPermitJoining;

		// This value is set to true if the device is capable of accepting join
		// requests from router-capable devices
		bool m_bRouterCapacity;

		// This value is set to true if the device is capable of accepting join
		// requests from end devices and set to FALSE otherwise
		bool m_bEndDeviceCapacity;

	// Operations
	public:
		// Returns true if the network descriptor is a valid Zigbee PRO network
		// descriptor
		bool IsValid() const;

#ifdef _DEBUG
		// Shows the contents of the network descriptor
		void Dump() const;
#endif

	// Implementation
	protected:
		// This is the 16-bit short PANID used on the network. Notice that
		// there might be more than one network descriptor with equal 64-bit
		// extended PANIDs, but different 16-bit short PANIDs due to network
		// cluttering (proprietary extension to the standard)
		uint16_t m_wPANID;

		// Best of link qualities for any of the devices found on this
		// particular network (proprietary extension to the standard)
		uint8_t m_nLinkQuality;

		// Number of devices found on that particular network
		// (proprietary extension to the standard)
		uint8_t m_nDevices;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNodeDescriptor

#pragma pack(push, 1)

class CZigBeeNodeDescriptor
{
	// typedefs & enums
	public:
		// Device flags
		enum { flagsCoordinator, flagsRouter = 0x0001, flagsEndDevice = 0x0002,
			flagsHasComplexDescriptor = 0x0008, flagsHasUserDescriptor = 0x0010,
			flagsSupports868Band = 0x0800, flagsSupports915Band = 0x2000,
			flagsSupports2400Band = 0x4000 };

		// System server roles
		enum { serverPrimaryTrustCenter = 0x0001,
			serverBackupTrustCenter = 0x0002,
			serverPrimaryBindingTableCache = 0x0004,
			serverBackupBindingTableCache = 0x0008,
			serverPrimaryDiscoveryCache = 0x0010,
			serverBackupDiscoveryCache = 0x0020,
			serverNetworkManager = 0x0040,
			serverCoreStackRevisionMask = 0xfe00,
			serverCoreStackRevisionShift = 9 };

		// Descriptor capabilities
		enum { hasExtendedActiveEndpoints = 0x01,
			hasExtendedSimpleDescriptor = 0x02 };

	// Attributes
	public:
		// Logical type, complex/user descriptor available, APS and frequency
		// band fields combined
		uint16_t m_wDeviceFlags;

		// IEEE 802.15.4 MAC capabilities
		uint8_t m_bCapabilities;

		// Manufacturer code, as assigned by the ZigBee Alliance
		uint16_t m_wManufacturerID;

		// Buffer size (non-fragmented)
		uint8_t m_nMaximumBufferSize;

		// Size of maximum incoming ASDU
		uint16_t m_nMaximumIncomingTransferSize;

		// System server capabilities of this node and core stack revision
		uint16_t m_nServerMask;

		// Size of maximum outgoing ASDU
		uint16_t m_nMaximumOutgoingTransferSize;

		// Available optional descriptors
		uint8_t m_bDescriptorCapabilties;

	// Operations
	public:
		// Returns the core stack revision portion of the server flags
		uint8_t GetCoreStackRevision() const;
};

#pragma pack(pop)

inline uint8_t CZigBeeNodeDescriptor::GetCoreStackRevision() const
	{ return m_nServerMask >> serverCoreStackRevisionShift; }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNodePowerDescriptor

#pragma pack(push, 1)

class CZigBeeNodePowerDescriptor
{
	// typedefs & enums
	public:
		// Power mode values
		enum { modeAlwaysOn, modePeriodicalWakeUp, modeEventTriggeredWakeUp };

		// Power sources
		enum { sourceMains = 0x01, sourceRechargeable = 0x02,
			sourceDisposable = 0x04 };

		// Power source levels
		enum { levelCritical, level33Percent = 0x04, level66Percent = 0x08,
			level100Percent = 0x0c };

	// Attributes
	public:
		// One of the pwoer mode values above
		uint32_t m_nCurrentPowerMode : 4;

		// A combination of the power sources above
		uint32_t m_nAvailablePowerSources : 4;

		// The currently used power source, which is a single bit out of the
		// available power sources
		uint32_t m_nCurrentPowerSource : 4;

		// In case of rechargeable or disposable power sources, the remaining
		// battery life expressed as one of the power source levels above
		uint32_t m_nCurrentPowerLevel : 4;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeSimpleDescriptor

#pragma pack(push, 1)

class CZigBeeSimpleDescriptor
{
	// typedefs & enums
	public:
		// Device version limited to four bits at present time
		enum { deviceVersionMask = 0x0f };

	private:
		// Prevent direct instantiation of this class
		// This class must be placed into storage of sufficient size to
		// provide space for the in/out cluster lists
		CZigBeeSimpleDescriptor();

	private:
		// Prevent copy and assignment
		// As above, this class requires extra space allocated beyond the
		// actual definition of its class members. It can thus not be copied
		// as class.
		CZigBeeSimpleDescriptor(const CZigBeeSimpleDescriptor &);
		CZigBeeSimpleDescriptor& operator=(const CZigBeeSimpleDescriptor &);

	// Attributes
	public:
		// Endpoint
		uint8_t m_bEndpoint;

		// Application profile identifier
		uint16_t m_wProfileID;

		// Application device identifier
		uint16_t m_wDeviceID;

		// Application device version
		uint8_t m_bDeviceVersion;

		// Variable-length application input and output cluster information
		// follows here. Use GetInputClusters() and GetOutputClusters() to
		// retrieve cluster lists...

	// Operations
	public:
		/*
		// Returns the application input cluster list as a set
		void GetInputClusters(std::set<uint16_t> &clusters) const;

		// Returns the application output cluster list as a set
		void GetOutputClusters(std::set<uint16_t> &clusters) const;
		*/

		// Returns the total size of the full descriptor, i.e. including fixed
		// header and variable-length cluster lists
		CPacketData::size_type GetSize() const;

		// Returns the sets of input and output cluster IDs encoded in this
		// simple descriptor
		void GetClusters(std::set<uint16_t> &in,
			std::set<uint16_t> &out) const;

		// Returns the device version
		uint_fast8_t GetDeviceVersion() const;

	// Implementation
	public:
		// Returns the sets of input and output cluster IDs, which are expected
		// to be stored in the order input cluster count, input cluster list,
		// output cluster count, output cluster list (packed)
		static void GetClusterLists(const void *const pPacketData,
			std::set<uint16_t> &in, std::set<uint16_t> &out,
			size_t nOffset = 0);

		// Returns a set of cluster IDs, either input or output, depending on
		// the supplied offset
		static void GetClusterList(const void *const pPacketData,
			std::set<uint16_t> &clusters, const size_t nOffset);

		// Appends a set of cluster IDs, either input or output, to the packet
		// object
		static void AppendClusterList(CPacket &packet,
			const std::set<uint16_t> &clusters);

#ifdef _DEBUG
		// Dumps information contained in a simple descriptor
		void Dump() const;
#endif

};

#pragma pack(pop)

inline uint_fast8_t CZigBeeSimpleDescriptor::GetDeviceVersion() const
	{ return m_bDeviceVersion & deviceVersionMask; }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeChannelList

#pragma pack(push, 1)

class CZigBeeChannelList
{
	// Construction
	public:
		CZigBeeChannelList();
		CZigBeeChannelList(const uint_fast8_t count);

	// Attributes
	public:
		// Number of channel page structures
		uint8_t m_count;

		// Variable-length array of channel page structures
};

#pragma pack(pop)

inline CZigBeeChannelList::CZigBeeChannelList()
{
}

inline CZigBeeChannelList::CZigBeeChannelList(const uint_fast8_t count) :
m_count(count)
{
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeChannelList1

#pragma pack(push, 1)

class CZigBeeChannelList1 : public CZigBeeChannelList
{
	// Construction
	public:
		CZigBeeChannelList1(const uint32_t channelMask);

	// Attributes
	public:
		// Simplified version of channel list for a single channel page. For
		// Zigbee 3.0 devices in the 2.4GHz band this is sufficient
		uint32_t m_channelMask;
};

#pragma pack(pop)

inline CZigBeeChannelList1::CZigBeeChannelList1(const uint32_t channelMask) :
CZigBeeChannelList(1), m_channelMask(channelMask)
{
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkClient

class CZigBeeNetworkClient
{
	// Overrdiables
	public:

};


/////////////////////////////////////////////////////////////////////////////
// DumpClusterList()

#ifdef _DEBUG

void DumpClusterList(const std::set<uint16_t> &clusters,
	const char *const pszDescription);

#endif
