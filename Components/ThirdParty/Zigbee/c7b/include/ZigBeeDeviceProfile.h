// ZigBeeDeviceProfile.h : header file
//
// Copyright(C) 2008-2022 ubisys technologies GmbH, Duesseldorf, Germany.
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
// CZigBeeDeviceObjectSimpleResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSimpleResponse
{
	// Attributes
	public:
		// The status of the ZDO command (bind_rsp, unbind_rsp)
		uint8_t m_nStatus;

#ifdef _DEBUG
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectNetworkAddressRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectNetworkAddressRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0000 };

		// Possible values for the RequestType field
		enum { typeSingle, typeExtended };

	// Attributes
	public:
		// The IEEE address to be matched by the Remote Device
		uint64_t m_qwAddress;

		// Request type for this command
		uint8_t m_nRequestType;

		// If the Request type for this command is Extended response, the
		// StartIndex provides the starting index for the requested elements
		// of the associated devices list
		uint8_t m_nStartIndex;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a NWK_addr_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectNetworkAddressResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectNetworkAddressResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8000 };

	// Attributes
	public:
		// The status of the NWK_addr_req command
		uint8_t m_bStatus;

		// The IEEE address to be matched by the Remote Device
		uint64_t m_qwExtendedAddress;

		// Request type for this command
		uint16_t m_wNetworkAddress;

		// Variable length NumAssocDev, StartIndex and NWKAddrAssocDevList
		// entries follow for extended requests

	// Operations
	public:
		// Given a packet (without fixed portion header above), extracts the
		// list of associated devices adds to the supplied list
		static void GetAssociatedDevices(CPacket &packet,
			uint8_t &bIndex, std::set<uint16_t> &devices);

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a NWK_addr_rsp command
		void Dump(const bool bExtended = false) const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectExtendedAddressRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectExtendedAddressRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0001 };

		// Possible values for the RequestType field
		enum { typeSingle, typeExtended };

	// Attributes
	public:
		// The IEEE address to be matched by the Remote Device
		uint16_t m_wNetworkAddress;

		// Request type for this command
		uint8_t m_nRequestType;

		// If the Request type for this command is Extended response, the
		// StartIndex provides the starting index for the requested elements
		// of the associated devices list
		uint8_t m_nStartIndex;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps an IEEE_addr_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectExtendedAddressResponse

class CZigBeeDeviceObjectExtendedAddressResponse : public
	CZigBeeDeviceObjectNetworkAddressResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8001 };
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectNodeDescriptorRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectNodeDescriptorRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0002 };

	// Attributes
	public:
		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Active_EP_rsp command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectNodeDescriptorResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectNodeDescriptorResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8002 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 16 };

		// Supported TLVs with local scope
		enum { tagKeyAgreementSelection };

	// Attributes
	public:
		// The status of the Node_Desc_req command
		uint8_t m_bStatus;

		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

		// ZigBee node descriptor
		CZigBeeNodeDescriptor m_descriptor;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Node_desc_rsp command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectPowerDescriptorRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectPowerDescriptorRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0003 };

	// Attributes
	public:
		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a power_desc_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectPowerDescriptorResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectPowerDescriptorResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8003 };

	// Attributes
	public:
		// The status of the Power_desc_req command
		uint8_t m_bStatus;

		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

		// ZigBee power descriptor
		CZigBeeNodePowerDescriptor m_descriptor;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a power_desc_rsp command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSimpleDescriptorRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSimpleDescriptorRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0004 };

	// Attributes
	public:
		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

		// Endpoint on the destination
		uint8_t m_bEndpoint;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Simple_Desc_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSimpleDescriptorResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSimpleDescriptorResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8004 };

	// Attributes
	public:
		// The status of the Simple_Desc_req command
		uint8_t m_bStatus;

		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

		// Length of the simple descriptor
		uint8_t m_bDescriptorSize;

		// Variable-length end-point simple descriptor follows here...

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Simple_Desc_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectActiveEndpointsRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectActiveEndpointsRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0005 };

	// Attributes
	public:
		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Active_EP_rsp command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectActiveEndpointsResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectActiveEndpointsResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8005 };

	// Attributes
	public:
		// The status of the Active_EP_req command
		uint8_t m_bStatus;

		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

		// The number of active endpoints on the remote device
		uint8_t m_bEndpointCount;

		// Variable length (ActiveEPCount bytes) ActiveEPList follows here

	// Operations
	public:
		// Returns the set of active endpoints conveyed in the response frame
		void GetActiveEndpoints(std::set<uint8_t> &endpoints) const;

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Active_EP_rsp command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectMatchDescriptorsRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectMatchDescriptorsRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0006 };

	// Attributes
	public:
		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

		// The 16-bit profile ID
		uint16_t m_wProfileID;

		// Input cluster list (count, list of 16-bit cluster IDs) follows

		// Output cluster list (count, list of 16-bit cluster IDs) follows

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Active_EP_rsp command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectMatchDescriptorsResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectMatchDescriptorsResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8006 };

	// Attributes
	public:
		// The status of the Match_descriptors_req command
		uint8_t m_bStatus;

		// The 16-bit network address of the device of interest
		uint16_t m_wNetworkAddress;

		// The number of matching endpoints on the remote device
		uint8_t m_bEndpointCount;

		// Variable length (MatchLength bytes) MatchList follows here

	// Operations
	public:
		uint_fast8_t GetMatchingEndpoint(const uint_fast8_t nIndex = 0) const;

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Active_EP_rsp command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectDeviceAnnouncement

#pragma pack(push, 1)

class CZigBeeDeviceObjectDeviceAnnouncement
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0013 };

	// Attributes
	public:
		// Request type for this command
		uint16_t m_wNetworkAddress;

		// The IEEE address to be matched by the Remote Device
		uint64_t m_qwExtendedAddress;

		// The status of the NWK_addr_req command
		uint8_t m_bCapabilities;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Device_annce command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectServerDiscoveryRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectServerDiscoveryRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0015 };

	// Attributes
	public:
		// The 16-bit server mask
		uint16_t m_wServerMask;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a System_server_discovery_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectServerDiscoveryResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectServerDiscoveryResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8015 };

	// Attributes
	public:
		// The resulting status code (either SUCCESS, or NO_MATCH)
		uint8_t m_nStatus;

		// The 16-bit server mask
		uint16_t m_wServerMask;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a System_server_discovery_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectBinding

#pragma pack(push, 1)

class CZigBeeDeviceObjectBinding
{
	// typedefs & enums
	public:
		typedef struct
		{
			// 64-bit IEEE destination address and endpoint
			uint64_t m_qwDestinationAddress;

			// Destination endpoint (in case of mode 3)
			uint8_t m_bDestinationEndpoint;
		} ADDRESSINGMATERIAL3;

	// Attributes
	public:
		// 64-bit IEEE source device address
		uint64_t m_qwSourceAddress;

		// Source endpoint number
		uint8_t m_bSourceEndpoint;

		// Cluster ID
		uint16_t m_wClusterID;

		// Destination addressing mode
		uint8_t m_bDestinationAddressingMode;

		union
		{
			// Extended address and endpoint (in case of mode 3)
			ADDRESSINGMATERIAL3 m_extended;

			// 16-bit group destination address (mode 1)
			uint16_t m_wDestinationAddress;
		};

	// Operations
	public:
		// Returns the actual size of this entry, based on the destination
		// addressing mode in use
		CPacketData::size_type GetSize() const;

		// Returns the complete source address including endpoint
		CZigBeeAddress GetSourceAddress() const;

		// Returns the complete destination address (either extended address
		// plus endpoint or group address)
		CZigBeeAddress GetDestinationAddress() const;

		// Returns the binding source
		CZigBeeBindingSource GetSource() const;

		// Returns the binding target
		CZigBeeBindingTarget GetTarget() const;

#ifdef _DEBUG
		// Dumps a Mgmt_bind_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectChildInfoList

#pragma pack(push, 1)

class CZigBeeDeviceObjectChildInfoList
{
	// Attributes
	public:
		// The number of child records that follow (each being a IEEE EUI-64)
		uint8_t m_nCount;

		// Variable number of child records (at least one, total number is
		// given above)
		uint64_t m_aqwExtended[1];
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectParentAnnouncement

#pragma pack(push, 1)

class CZigBeeDeviceObjectParentAnnouncement
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x001f };

	// Attributes
	public:
		// Child info list
		CZigBeeDeviceObjectChildInfoList m_devices;

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a parent_annce command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectParentAnnouncementResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectParentAnnouncementResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x801f };

	// Attributes
	public:
		// The status of the Simple_Desc_req command
		uint8_t m_bStatus;

		// Conflicting child info list
		CZigBeeDeviceObjectChildInfoList m_devices;

		// Variable-length end-point simple descriptor follows here...

	// Operations
	public:

	// Implementation
	public:

#ifdef _DEBUG
		// Dumps a Simple_Desc_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectBindRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectBindRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0021 };

		// Capacity is the greatest possible size of a bind request frame,
		// required is the least possible size
		enum { capacity = 21, required = 14 };

	// Attributes
	public:
		// Identifies the binding
		CZigBeeDeviceObjectBinding m_binding;

	// Operations
	public:
		// Retrieves the binding target address and endpoint, if applicable
		bool GetDestination(CZigBeeAddress &address) const;

		// Returns the size of this command frame based on the actual contents
		CPacketData::size_type GetSize() const;

	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectUnbindRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectUnbindRequest :
	public CZigBeeDeviceObjectBindRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0022 };

	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectClearAllBindingsRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectClearAllBindingsRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x002b };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagExtendedAddressList };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a clear all bindings request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectClearAllBindingsResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectClearAllBindingsResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x802b };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

	// Attributes
	public:
		// Status of the clear all bindings response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a clear all bindings response command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementNeighborTableRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementNeighborTableRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0031 };

	// Attributes
	public:
		// Start index, i.e. index of first device in the neighbor table,
		// that the requester is interested in
		uint8_t m_nStartIndex;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_Lqi_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementNetworkDiscoveryRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementNetworkDiscoveryRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0030 };

	// Attributes
	public:
		// Scan Channels itmap indicating the logical channels to scan
		uint32_t m_dwScanChannels;

		// Scan Duration
		uint8_t m_nScanDuration;

		// Start Index
		uint8_t m_nStartIndex;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_NWK_Disc_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectNetworkListEntry

#pragma pack(push, 1)

class CZigBeeDeviceObjectNetworkListEntry
{
	// typedefs & enums
	public:

	// Attributes
	public:
		// ExtendedPanID
		uint64_t m_qwExtendedPANID;

		// LogicalChannel
		unsigned int m_nLogicalChannel : 8;

		// StackProfile
		unsigned int m_nStackProfile : 4;

		// ZigBeeVersion
		unsigned int m_nZigBeeVersion : 4;

		// BeaconOrder
		unsigned int m_nBeaconOrder : 4;

		// SuperframeOrder
		unsigned int m_nSuperframeOrder : 4;

		// PermitJoining
		unsigned int m_bPermitJoining : 1;

		// Reserved
		unsigned int m_nReserved : 7;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementNetworkDiscoveryResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementNetworkDiscoveryResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8030 };

	// Attributes
	public:
		// Status
		uint8_t m_nStatus;

		// Network count (total number)
		uint8_t m_nCount;

		// The starting point in the NetworkList from the
		// NLME-NETWORK-DISCOVERY.confirm where reporting begins for this
		// response.
		uint8_t m_nStartIndex;

		// The number of network list descriptors reported within this response.
		uint8_t m_nListCount;

		// Variable length list (count as specified in list count) of network
		// descriptors follows...

	// Operations
	public:

#ifdef _DEBUG
		// Dumps
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementNeighborTableResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementNeighborTableResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8031 };

	// Attributes
	public:
		// Status
		uint8_t m_nStatus;

		// Total number of neighbor table entries within the remote device
		uint8_t m_nEntries;

		// Starting index within the neighbor table to begin reporting for the
		// neighbor table list included in this response
		uint8_t m_nStartIndex;

		// Number of neighbor table entries included within the neighbor list
		// included in this response
		uint8_t m_nCount;

		// Neighbor list. Either non-existent or up to two entries per response
		// CZigBeeNeighbor m_neighbors[0..2];

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_Lqi_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementRoutingTableRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementRoutingTableRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0032 };

	// Attributes
	public:
		// Start index
		uint8_t m_nStartIndex;

	// Operations
	public:
#ifdef _DEBUG
		// Dumps a Mgmt_rtg_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementRoutingTableResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementRoutingTableResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8032 };

	// Attributes
	public:
		// Status
		uint8_t m_nStatus;

		// Total number of routing table entries within the remote device
		uint8_t m_nEntries;

		// Starting index within the routing table to begin reporting for the
		// routing table list included in this response
		uint8_t m_nStartIndex;

		// Number of routing table entries included within the route list
		// included in this response
		uint8_t m_nCount;

		// Route list. Either non-existent or up to two entries per response
		// CZigBeeRoute m_routes[0..2];

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_Lqi_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementBindingTableRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementBindingTableRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0033 };

	// Attributes
	public:
		// Index of first entry to return
		uint8_t m_nStartIndex;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_bind_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementBindingTableResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementBindingTableResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8033 };

	// Attributes
	public:
		// Status
		uint8_t m_nStatus;

		// Total number of binding table entries currently stored in the device
		uint8_t m_nEntries;

		// Starting index of the first entry included in this response frame
		uint8_t m_nStartIndex;

		// Number of (variable length) binding table entries included in this
		// frame
		uint8_t m_nCount;

		// A number of variable-length binding table entries follows...


	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_bind_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementLeaveRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementLeaveRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0034 };

		// Possible values for the options field. Notice that the broadcast
		// flag is only used internally to indicate that a request was received
		// via broadcast addressing. This value is a reserved value in
		// over-the-air messages
		enum { rejoin = 0x80, removeChildren = 0x40, broadcast = 0x01,
			optionsMask = 0xc0 };

	// Attributes
	public:
		// The IEEE address to device to be removed from the network. Null, if
		// recipient is to be removed, one of recipient's children otherwise
		uint64_t m_qwAddress;

		// Command options
		uint8_t m_nOptions;

	// Operations
	public:
		// Returns true, if children shall be removed
		bool ShallRemoveChildren() const;

		// Returns true, if device shall rejoin
		bool ShallRejoin() const;

#ifdef _DEBUG
		// Dumps a Mgmt_leave_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementPermitJoiningRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementPermitJoiningRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0036 };

	// Attributes
	public:
		// Permit duration
		uint8_t m_nPermitDuration;

		// Trust Center enable/disable
		bool m_bTrustCenter;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_Permit_Joining_req command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementNetworkUpdateRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementNetworkUpdateRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0038 };

		// Special 'scan duration' values exist for channel changes and updates
		// of the designated network manager address
		enum { typeEnergyScan = 0, typeEnergyScan0 = 0, typeEnergyScan1,
			typeEnergyScan2, typeEnergyScan3, typeEnergyScan4, typeEnergyScan5,
			typeChannelChange = 0xfe, typeUpdate = 0xff };

	// Attributes
	public:
		// Scan channels
		uint32_t m_nScanChannels;

		// A value used to calculate the length of time to spend scanning each
		// channel; this value is passed to MLME-SCAN.request if is in the
		// range 0..5 to conduct an energy scan. If ScanDuration has a value of
		// 0xfe this is a request for channel change. If ScanDuration has a
		// value of 0xff this is a request to change the apsChannelMask and
		// nwkManagerAddr attributes
		uint8_t m_nScanDuration;

	// Operations
	public:
		// Returns either typeEnergyScan, typeChannelChange or typeUpdate to
		// discern the type of this network update request, i.e. aggregates
		// the different scan durations into a single type
		uint_fast8_t GetType() const;

#ifdef _DEBUG
		// Dumps a Mgmt_NWK_Update_req command
		void Dump() const;
#endif

};

#pragma pack(pop)

inline uint_fast8_t
CZigBeeDeviceObjectManagementNetworkUpdateRequest::GetType() const
{
	return (m_nScanDuration <= static_cast<uint8_t>(typeEnergyScan5)) ?
		typeEnergyScan : m_nScanDuration;
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementEnergyScanRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementEnergyScanRequest : public
	CZigBeeDeviceObjectManagementNetworkUpdateRequest
{
	// typedefs & enums
	public:
		// Request sub-type
		enum { type = typeEnergyScan };

	// Attributes
	public:
		// This field represents the number of energy scans to be conducted and
		//	reported. This field shall be present only if the ScanDuration is
		// within the range of 0x00 to 0x05
		uint8_t m_nScanCount;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_NWK_Update_req command (specialized for energy scan)
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementChannelChangeRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementChannelChangeRequest : public
	CZigBeeDeviceObjectManagementNetworkUpdateRequest
{
	// typedefs & enums
	public:
		// Request sub-type
		enum { type = typeChannelChange };

	// Attributes
	public:
		// The value of the nwkUpdateId contained in this request. This value
		// is set by the Network Channel Manager prior to sending the message
		uint8_t m_nUpdateID;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_NWK_Update_req command (specialized for channel change)
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementUpdateRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementUpdateRequest : public
	CZigBeeDeviceObjectManagementNetworkUpdateRequest
{
	// typedefs & enums
	public:
		// Request sub-type
		enum { type = typeUpdate };

	// Attributes
	public:
		// The value of the nwkUpdateId contained in this request. This value
		// is set by the Network Channel Manager prior to sending the message.
		// This should be ignored for a plain channel mask and network manager
		// address update
		uint8_t m_nUpdateID;

		// This field indicates the NWK address for the device with the Network
		//	Manager bit set in its Node Descriptor
		uint16_t m_wNetworkManagerAddress;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a Mgmt_NWK_Update_req command (specialized for channel mask
		// and network manager address update)
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectManagementNetworkUpdateNotification

#pragma pack(push, 1)

class CZigBeeDeviceObjectManagementNetworkUpdateNotification
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8038 };

	// Attributes
	public:
		// Status of the Mgmt_NWK_Update_notify command
		uint8_t m_nStatus;

		// List of channels scanned by the request
		uint32_t m_nScannedChannels;

		// Count of the total transmissions reported by the device
		uint16_t m_nTotalTransmissions;

		// Sum of the total transmission failures reported by the device
		uint16_t m_nTransmissionFailures;

		// The number of entries in the energy values list below
		uint8_t m_nCount;

		// Variable length list of energy values, one for each channel scanned,
		// being the result of an energy measurement made on each channel

	// Operations
	public:
		// Returns either typeEnergyScan, typeChannelChange or typeUpdate to
		// discern the type of this network update request, i.e. aggregates
		// the different scan durations into a single type
		uint_fast8_t GetType() const;

#ifdef _DEBUG
		// Dumps a Mgmt_NWK_Update_notify command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSiteSurveyRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSiteSurveyRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x003c };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagBeaconSurveyConfiguration };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security key agreement request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSiteSurveyResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSiteSurveyResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x803c };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagBeaconSurveryConfiguration, tagBeaconSurveyResults,
			tagPotentialParents };

	// Attributes
	public:
		// Status of the key update response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security start key agreement response command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityKeyAgreementRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityKeyAgreementRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0040 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tag25519PublicPoint };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security key agreement request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityKeyAgreementResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityKeyAgreementResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8040 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tag25519PublicPoint };

	// Attributes
	public:
		// Status of the key update response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security start key agreement response command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityTokenRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityTokenRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0041 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagTokenIdentifier };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security symmetric authentication token request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityTokenResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityTokenResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8041 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

	// Attributes
	public:
		// Status of the key update response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security symmetric authentication token response command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectAuthenticationLevelRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectAuthenticationLevelRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0042 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagAddressOfInterest };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security get authentication level request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectAuthenticationLevelResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectAuthenticationLevelResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8042 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagDeviceAuthenticationLevel };

	// Attributes
	public:
		// Status of the key update response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security get authentication level response command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecuritySetConfigurationRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecuritySetConfigurationRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0043 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security set configuration request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecuritySetConfigurationResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecuritySetConfigurationResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8043 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagProcessingStatus };

	// Attributes
	public:
		// Status of the security set configuration response
		uint8_t m_nStatus;

		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security set configuration request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityGetConfigurationRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityGetConfigurationRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0044 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

	// Attributes
	public:
		// Number of TLVs that are being requested
		uint8_t m_count;

		// Variable length list of TLVs
		uint8_t m_tags[1];

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security set configuration request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityGetConfigurationResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityGetConfigurationResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8044 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

	// Attributes
	public:
		// Status of the security get configuration response
		uint8_t m_status;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security set configuration request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityStartKeyUpdateRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityStartKeyUpdateRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0045 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Selected key agreement scheme and pre-shared secret TLV
		enum { tagKeyAgreementSelection };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security start key update request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityStartKeyUpdateResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityStartKeyUpdateResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8045 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

	// Attributes
	public:
		// Status of the key update response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security start key update response command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectDecommissionRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectDecommissionRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0046 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagExtendedAddressList };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security decommission request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectDecommissionResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectDecommissionResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8046 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

	// Attributes
	public:
		// Status of the security decommission response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security decommission response command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityChallengeRequest

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityChallengeRequest
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x0047 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagAPSCounterChallenge };

	// Attributes
	public:
		// No mandatory byte-packed fields; all fields are encoded as TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security challenge request command
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeDeviceObjectSecurityChallengeResponse

#pragma pack(push, 1)

class CZigBeeDeviceObjectSecurityChallengeResponse
{
	// typedefs & enums
	public:
		// ZDP cluster
		enum { cluster = 0x8047 };

		// Default capacity for TLVs
		enum { defaultTLVCapacity = 64 };

		// Supported TLVs with local scope
		enum { tagAPSCounterResponse };

	// Attributes
	public:
		// Status of the security challenge response
		uint8_t m_nStatus;

		// No other mandatory byte-packed fields; remaining fields are TLVs

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a security challenge response command
		void Dump() const;
#endif

};

#pragma pack(pop)
