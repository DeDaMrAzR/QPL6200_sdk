// ZigBeeNetworkCommands.h : header file
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
// This module provides classes that simplify network layer management entity
// command frame processing

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeNHR;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeBeacon, clause 3.6.7

#pragma pack(push, 1)

class CZigBeeBeacon
{
	// typedefs & enums
	public:
		// Protocol ID
		enum { protocolZigBee };

	// Attributes
	public:
		// Protocol ID
		uint32_t m_nProtocolID : 8;

		// Stack profile
		uint32_t m_nStackProfile : 4;

		// Protocol version
		uint32_t m_nProtocolVersion : 4;

		// Two reserved bits. Notice: These two bits have been used by the
		// manufacturer-specific Works With All Hubs cluster defined by Amazon
		// as indicators for trust center connectivity and long up time (24h+)
		// and should not be used for other purposes (though no standardization
		// is expected because the standard approach defined in R23 0.5 draft
		// is based on a more versatile tag-length-value (TLV) paradigm, as
		// suggested by ubisys in the course of a new link-key agreement scheme
		uint32_t m_nReserved : 2;

		// Router capacity
		uint32_t m_bRouterCapacity : 1;

		// Device depth
		uint32_t m_nDeviceDepth : 4;

		// End device capacity
		uint32_t m_bEndDeviceCapacity : 1;

		// The globally unique ID for the PAN of which the beaconing device is
		// a member
		uint64_t m_qwExtendedPANID;

		// Beacon transmission offset
		uint32_t m_nTransmissionOffset : 24;

		// Network update ID
		uint32_t m_nUpdateID : 8;

#ifdef _DEBUG
	public:
		// Dumps the fields of a NWK frame header
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRouteRequestCommand, clause 3.4.1

#pragma pack(push, 1)

class CZigBeeRouteRequestCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 1 };

		// Command options field (3.4.1.3.1)
		enum { optionMulticast = 0x40, optionExtendedDestinationAddress = 0x20,
			optionManyToOneMask = 0x18, optionManyToOneShift = 3 };

		// Possible values of the many-to-one subfield within the option field
		// (3.4.1.3.1.1)
		enum { manytooneNone, manytooneSupportRouteRecord,
			manytooneNoRouteRecord };

	// Attributes
	public:
		// Command options
		uint8_t m_nOptions;

		// Route request identifier
		uint8_t m_nRequestID;

		// Destination address
		uint16_t m_wDestinationAddress;

		// Path cost
		uint8_t m_nPathCost;

		// Destination address (IEEE extended address). Only present, if
		// IEEE destination address option bit is set
		uint64_t m_qwDestinationAddress;

	// Operations
	public:
		// Returns the extended 64-bit IEEE destination address, if present
		bool GetDestinationAddress(uint64_t &qwAddress) const;

		// Returns the destination network address and extended address, if
		// present
		CZigBeeAddress GetDestinationAddress() const;

		// Returns the size of this route request command, depending on the
		// options field
		size_t GetSize() const;

		// Returns one of the many-to-one route options
		uint_fast8_t GetManyToOneType() const;

#ifdef _DEBUG
	public:
		// Dumps the fields of a NWK frame header
		void Dump() const;
#endif

};

#pragma pack(pop)

inline size_t CZigBeeRouteRequestCommand::GetSize() const
{
	return (m_nOptions & optionExtendedDestinationAddress) ?
		sizeof(CZigBeeRouteRequestCommand) :
		sizeof(CZigBeeRouteRequestCommand) - sizeof(uint64_t);
}

inline uint_fast8_t CZigBeeRouteRequestCommand::GetManyToOneType() const
	{ return (m_nOptions & optionManyToOneMask) >> optionManyToOneShift; }

inline CZigBeeAddress CZigBeeRouteRequestCommand::GetDestinationAddress() const
{
	CZigBeeAddress address(m_wDestinationAddress);

	if (GetDestinationAddress(address.m_qwExtended))
		address.SetType(CIEEE802154Address::typeBoth);

	return address;
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRouteReplyCommand, clause 3.4.2

#pragma pack(push, 1)

class CZigBeeRouteReplyCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 2 };

		// Command options field (3.4.1.3.1)
		enum { optionMulticast = 0x40, optionExtendedResponderAddress = 0x20,
			optionExtendedOriginatorAddress = 0x10 };

	// Attributes
	public:
		// Command options
		uint8_t m_nOptions;

		// Route request identifier
		uint8_t m_nRequestID;

		// Originator address
		uint16_t m_wOriginatorAddress;

		// Responder address
		uint16_t m_wResponderAddress;

		// Path cost
		uint8_t m_nPathCost;

		// Originator IEEE address (if originator IEEE address option bit is
		// set) or responder IEEE address (if responder IEEE address option bit
		// is set and originator IEEE address option bit is not set)
		uint64_t m_qwAddress1;

		// Responder IEEE address (if originator and responder address option
		// bits are both set)
		uint64_t m_qwAddress2;

	// Operations
	public:
		// Returns the extended 64-bit IEEE originator address, if present
		bool GetOriginatorAddress(uint64_t &qwAddress) const;

		// Returns the originator address. Network address is always valid,
		// extended address might be valid
		CZigBeeAddress GetOriginatorAddress() const;

		// Returns the extended 64-bit IEEE responder address, if present
		bool GetResponderAddress(uint64_t &qwAddress) const;

		// Returns the responder address. Network address is always valid,
		// extended address might be valid
		CZigBeeAddress GetResponderAddress() const;

		// Returns the size of this route reply command, depending on the
		// options field
		size_t GetSize() const;

#ifdef _DEBUG
	public:
		// Dumps the fields of a NWK frame header
		void Dump() const;
#endif

};

#pragma pack(pop)

inline CZigBeeAddress CZigBeeRouteReplyCommand::GetOriginatorAddress() const
{
	CZigBeeAddress address(m_wOriginatorAddress);

	if (GetOriginatorAddress(address.m_qwExtended))
		address.SetType(CIEEE802154Address::typeBoth);

	return address;
}


inline CZigBeeAddress CZigBeeRouteReplyCommand::GetResponderAddress() const
{
	CZigBeeAddress address(m_wResponderAddress);

	if (GetResponderAddress(address.m_qwExtended))
		address.SetType(CIEEE802154Address::typeBoth);

	return address;
}


inline size_t CZigBeeRouteReplyCommand::GetSize() const
{
	size_t nSize = sizeof(CZigBeeRouteReplyCommand);

	if (!(m_nOptions & optionExtendedResponderAddress))
		nSize -= sizeof(uint64_t);

	if (!(m_nOptions & optionExtendedOriginatorAddress))
		nSize -= sizeof(uint64_t);

	return nSize;
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkStatusCommand, clause 3.4.3

#pragma pack(push, 1)

class CZigBeeNetworkStatusCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 3 };

		// Status codes for network status command frame. Also used in the
		// NLME-ROUTE-DISCOVERY.confirm's NetworkStatus parameter
		enum { statusNoRouteAvailable, statusTreeLinkFailure,
			statusNonTreeLinkFailure, statusLowBatteryLevel,
			statusNoRoutingCapacity, statusNoIndirectCapacity,
			statusIndirectTransactionExpiry, statusTargetDeviceUnavailable,
			statusTargetAddressUnallocated, statusParentLinkFailure,
			statusValidateRoute, statusSourceRouteFailure,
			statusManyToOneRouteFailure, statusAddressConflict,
			statusVerifyAddresses, statusPANIdentifierUpdate,
			statusNetworkAddressUpdate, statusBadFrameCounter,
			statusBadKeySequence, statusUnknownCommand,
			statusPANIDConflictReport };

		// Extended status codes. These are not being used over-the-air,
		// but in NLME-NWK-STATUS.indications
		enum { statusExPermitJoiningUpdate = 0xf0, statusExNetworkKeySwitch };

	// Attributes
	public:
		// Status code
		uint8_t m_bStatus;

		// Destination address
		uint16_t m_wAddress;

		// Variable-length payload (depending on status code) and optionally TLVs

	// Implementation
	public:
		// Verify whether it is a valid Network Status command frame
		static bool IsValidFrame(const CPacket &frame);
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeLeaveCommand, clause 3.4.4

#pragma pack(push, 1)

class CZigBeeLeaveCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 4 };

		// Possible command option flags
		enum { optionRejoin = 0x20, optionRequest = 0x40,
			optionRemoveChildren = 0x80  };

	// Attributes
	public:
		// Command options
		uint8_t m_bOptions;

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a leave command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRouteRecordCommand, clause 3.4.5

#pragma pack(push, 1)

class CZigBeeRouteRecordCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 5 };

	// Attributes
	public:
		// Relay count. Number of 16-bit network addresses that follow
		uint8_t m_nRelayCount;

		// Relay list. Records the 16-bit network addresses of all nodes en
		// route in the order the packet visits each node along its path from
		// source to destination

	// Operations
	public:
		// Returns the source route relay at the specified index
		uint_fast16_t GetSourceRouteRelay(const uint_fast8_t nIndex) const;

#ifdef _DEBUG
	public:
		// Dumps the route record command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRejoinRequestCommand, clause 3.4.6

#pragma pack(push, 1)

class CZigBeeRejoinRequestCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 6 };

	// Attributes
	public:
		// Device capabilities (same as IEEE 802.15.4 MAC capability bitfield)
		uint8_t m_bCapabilities;

#ifdef _DEBUG
	public:
		// Dumps the fields of a rejoin request
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeRejoinResponseCommand, clause 3.4.7

#pragma pack(push, 1)

class CZigBeeRejoinResponseCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 7 };

	// Attributes
	public:
		// Network address. If not successful, contains the broadcast address
		uint16_t m_wAddress;

		// Rejoin status (a MAC layer join status)
		uint8_t m_bStatus;

	// Operations
	public:

#ifdef _DEBUG
	public:
		// Dumps the rejoin response command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeLinkStatusEntry

#pragma pack(push, 1)

class CZigBeeLinkStatusEntry
{
	// typedefs & enums
	public:
		// Status field masks
		enum { statusIncomingCostMask = 0x07, statusOutgoingCostMask = 0x70,
			statusOutgoingCostShift = 4 };

	// Attributes
	public:
		// Neighbor network address
		uint16_t m_wAddress;

		// Link status
		uint8_t m_bStatus;

	// Operations
	public:
		// Returns the incoming cost
		inline uint_fast8_t GetIncomingCost() const;

		// Returns the outgoing cost
		inline uint_fast8_t GetOutgoingCost() const;
};

#pragma pack(pop)

inline uint_fast8_t CZigBeeLinkStatusEntry::GetIncomingCost() const
	{ return m_bStatus & statusIncomingCostMask; }

inline uint_fast8_t CZigBeeLinkStatusEntry::GetOutgoingCost() const
	{ return (m_bStatus & statusOutgoingCostMask) >> statusOutgoingCostShift; }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeLinkStatusCommand, clause 3.4.8

#pragma pack(push, 1)

class CZigBeeLinkStatusCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 8 };

		// Command options
		enum { optionCountMask = 0x1f, optionFirstFrame = 0x20,
			optionLastFrame = 0x40 };

	// Attributes
	public:
		// Command options field
		uint8_t m_bOptions;

		// Variable length link status list follows...

	// Operations
	public:
		// Returns the number of link status entries contained in this frame
		uint_fast8_t GetCount() const;

		// Returns the size of this link status command frame in bytes
		uint_fast8_t GetSize() const;

		// QuickCheck performs an analysis of whether the local device is in
		// the neighbor table of the remote device that sent the present link
		// status frame. It returns true only if the local device is not in
		// the neighbor table of the remote device with certainty. This
		// assessment is not reciprocal, i.e. if QuickCheck() returns false,
		// the local device may or may not be in the remote device's neighbor
		// table
		// Notice: QuickCheck() must only be called when GetLinkStatus()
		// returned false, otherwise the result is inaccurate
		bool QuickCheck(const uint_fast16_t wAddress) const;

		// Returns the link status entry for the specified address, if it is
		// contained in the list
		bool GetLinkStatus(const uint_fast16_t wAddress,
			CZigBeeLinkStatusEntry &entry) const;

	// Implementation
	public:
		// Returns a reference to the link status entry which can be used for
		// read and write access
		CZigBeeLinkStatusEntry &GetAt(const uint_fast8_t nIndex);

		// Returns a reference to the link status entry which can be used for
		// read access
		const CZigBeeLinkStatusEntry &GetAt(const uint_fast8_t nIndex) const;

#ifdef _DEBUG
		// In debug builds, dumps this link status command frame
		void Dump() const;
#endif

};

#pragma pack(pop)

inline uint_fast8_t CZigBeeLinkStatusCommand::GetCount() const
	{ return m_bOptions & optionCountMask; }

inline uint_fast8_t CZigBeeLinkStatusCommand::GetSize() const
{
	return sizeof(CZigBeeLinkStatusCommand) +
		GetCount() * sizeof(CZigBeeLinkStatusEntry);
}

inline CZigBeeLinkStatusEntry &CZigBeeLinkStatusCommand::GetAt(const uint_fast8_t nIndex)
{
	return *reinterpret_cast<CZigBeeLinkStatusEntry *>
		(reinterpret_cast<uint8_t *>(this) +
		 sizeof(CZigBeeLinkStatusCommand) +
		 sizeof(CZigBeeLinkStatusEntry) * nIndex);
}

inline const CZigBeeLinkStatusEntry &CZigBeeLinkStatusCommand::GetAt(const uint_fast8_t nIndex) const
{
	return *reinterpret_cast<const CZigBeeLinkStatusEntry *>
		(reinterpret_cast<const uint8_t *>(this) +
		 sizeof(CZigBeeLinkStatusCommand) +
		 sizeof(CZigBeeLinkStatusEntry) * nIndex);
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkReportCommand, clause 3.4.9

#pragma pack(push, 1)

class CZigBeeNetworkReportCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 9 };

		// Report type and count
		enum { reportTypeMask = 0xe0, reportTypeShift = 5, countMask = 0x1f };

		// Report types
		enum { typePANIDConflict };

	// Attributes
	public:
		// Command options (count and report identifier)
		uint8_t m_nOptions;

		// The network's 64-bit extended PANID
		uint64_t m_qwExtendedPANID;

		// Variable length report information follows

	// Operations
	public:
		// Returns the total size of the report, if the report type is known
		bool GetSize(CPacketData::size_type &nSize) const;

#ifdef _DEBUG
	public:
		// Dumps the rejoin response command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkUpdateCommand, clause 3.4.10

#pragma pack(push, 1)

class CZigBeeNetworkUpdateCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 10 };

		// Report type and count
		enum { updateTypeMask = 0xe0, updateTypeShift = 5, countMask = 0x1f };

		// Report types
		enum { typePANIDUpdate };

	// Attributes
	public:
		// Command options (count and update identifier)
		uint8_t m_nOptions;

		// The network's 64-bit extended PANID
		uint64_t m_qwExtendedPANID;

		// Network update ID (incremented for each new instance of the network)
		uint8_t m_nUpdateID;

		// Variable length report information follows

#ifdef _DEBUG
	public:
		// Dumps the rejoin response command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkUpdateCommandPANID, clause 3.4.10 (specialized for PAN-ID)

#pragma pack(push, 1)

class CZigBeeNetworkUpdateCommandPANID : public CZigBeeNetworkUpdateCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { type = typePANIDUpdate };

	// Attributes
	public:
		// The new PAN-ID for the network
		uint16_t m_wPANID;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeEndDeviceTimeoutRequestCommand, clause 3.4.11

#pragma pack(push, 1)

class CZigBeeEndDeviceTimeoutRequestCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 11 };

	// Attributes
	public:
		// Request time-out field
		uint8_t m_nTimeout;

		// End-device configuration
		uint8_t m_nConfiguration;

	// Implementation
	public:

#ifdef _DEBUG
		// In debug builds, dumps this end-device timeout request command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeEndDeviceTimeoutResponseCommand, clause 3.4.12

#pragma pack(push, 1)

class CZigBeeEndDeviceTimeoutResponseCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 12 };

		// Command options
		enum { statusSuccess, statusIncorrectValue };

	// Attributes
	public:
		// Response status field
		uint8_t m_nStatus;

		// Parent capabilities
		uint8_t m_nCapabilities;

	// Implementation
	public:

#ifdef _DEBUG
		// In debug builds, dumps this end-device timeout request command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeCommissioningRequestCommand, clause 3.4.14

#pragma pack(push, 1)

class CZigBeeCommissioningRequestCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 14 };

		// Command options: initial join, subsequent rejoin or establish
		// trusted link
		enum { typeJoin, typeRejoin, typeEstablish };

	// Attributes
	public:
		// Commissioning type field
		uint8_t m_nType;

		// Device capabilities
		uint8_t m_nCapabilities;

	// Implementation
	public:

#ifdef _DEBUG
		// In debug builds, dumps this commissioning request command frame
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeCommissioningResponseCommand, clause 3.4.15

#pragma pack(push, 1)

class CZigBeeCommissioningResponseCommand
{
	// typedefs & enums
	public:
		// Command identifier
		enum { id = 15 };

	// Attributes
	public:
		// Address assigned to the joining/rejoining device, or 0xffff if the
		// commissioning request was denied
		uint16_t m_wAddress;

		// Device capabilities
		uint8_t m_nStatus;

	// Implementation
	public:

#ifdef _DEBUG
		// In debug builds, dumps this commissioning response command frame
		void Dump() const;
#endif

};

#pragma pack(pop)
