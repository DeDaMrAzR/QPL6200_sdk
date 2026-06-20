// ZigBeeClusterTouchlink.h : header file
//
// Copyright(C) 2018-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of ubisys ZigBee Cluster Library. Provides an implementation of the
// touch-link commissioning cluster

/////////////////////////////////////////////////////////////////////////////
// forward referenced classes

class CZigBeeNetworkManagement;
class CZigBeeNetworkFormationContext;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkMessage

#pragma pack(push, 1)

class CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// The 32-bit inter-PAN transaction identifier
		uint32_t m_nSessionID;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkScanRequest

#pragma pack(push, 1)

class CZigBeeTouchlinkScanRequest : public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// Information about the zigbee device role and RX-on-when-idle
		uint8_t m_nZigBeeInformation;

		// Information about the touchlink capabilities and state
		uint8_t m_nTouchlinkInformation;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkScanResponse

#pragma pack(push, 1)

class CZigBeeTouchlinkScanResponse : public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// The RSSI correction field is 8-bits in length and specifies a
		// pre-programmed RSSI correction offset
		uint8_t m_nRSSICorrection;

		// Information about the zigbee device role and RX-on-when-idle
		uint8_t m_nZigBeeInformation;

		// Information about the touchlink capabilities and state
		uint8_t m_nTouchlinkInformation;

		// The key bitmask field is 16-bits in length and specifies which keys
		// (and hence which encryption algorithms) are supported by the device
		uint16_t m_nSupportedSchemes;

		// The response identifier field is 32-bits in length and specifies a
		// random identifier for the response, used during the network key
		// transfer mechanism
		uint32_t m_nResponseID;

		// The extended PAN identifier field is 64-bits in length and specifies
		// the extended PAN identifier of the device responding to the scan
		// request
		uint64_t m_qwExtendedPANID;

		// The network update identifier field is 8-bits in length and specifies
		// the current value of the nwkUpdateId attribute of the originator
		uint8_t m_nNetworkUpdateID;

		// The logical channel field is 8-bits in length and specifies the
		// touchlink channel on which the device is operating
		uint8_t m_nChannel;

		// The PAN identifier field is 16-bits in length and specifies the
		// identifier of the PAN on which the device operates
		uint16_t m_wPANID;

		// The network address field is 16-bits in length and specifies the
		// 1current network address of the device
		uint16_t m_wNetworkAddress;

		// The number of sub-devices field is 8-bits in length and specifies
		// the number of sub-devices (endpoints) supported by the device
		uint8_t m_nDeviceCount;

		// The total group identifiers field is 8-bits in length and specifies
		// the number of unique group identifiers that this device requires
		uint8_t m_nTotalGroupCount;

		// If there is exactly one endpoint exposed over touch-link, its
		// information is included here, otherwise the frame ends here and all
		// endpoint information is gathered using device information requests
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkDeviceInformationRequest

#pragma pack(push, 1)

class CZigBeeTouchlinkDeviceInformationRequest :
	public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// Specifies the starting index (starting from 0) into the device table
		uint8_t m_nStartIndex;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkDeviceInformationResponse

#pragma pack(push, 1)

class CZigBeeTouchlinkDeviceInformationResponse :
	public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// Specifies the number of sub devices contained in the device, as
		// reported in the scan response inter-PAN command frame
		uint8_t m_nCount;

		// Specifies the starting index (starting from 0) into the device table
		// from which to get device information. This value of this field shall
		// be equal to the value of the start index field of the device
		// information request command frame
		uint8_t m_nStartIndex;

		// Specifies the number n of device information records that follow.
		// This value shall be in the range 0x00 � 0x05
		uint8_t m_nRecords;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkIdentifyRequest

#pragma pack(push, 1)

class CZigBeeTouchlinkIdentifyRequest : public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// Specifies the length of time the recipient is to remain in identify
		// mode
		uint16_t m_nIdentifyTime;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkCoreDeviceInformation

#pragma pack(push, 1)

class CZigBeeTouchlinkCoreDeviceInformation
{
	// Attributes
	public:
		// The endpoint identifier field is 8-bits in length and specifies the
		// endpoint identifier of the sub-device
		uint8_t m_nEndpoint;

		// The profile identifier field is 16-bits in length and specifies the
		// profile identifier supported by the sub-device
		uint16_t m_wProfileID;

		// The device identifier field is 16-bits in length and specifies the
		// device identifier supported by the sub-device
		uint16_t m_wDeviceID;

		// The version field is 8-bits in length and specifies the version of
		// the device description supported by the sub-device on the endpoint
		// specified by the endpoint identifier field
		uint8_t m_nVersion;

		// The group identifier count field is 8-bits in length and specifies
		// the number of group identifiers required by the sub-device
		uint8_t m_nGroupCount;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkDeviceInformation

#pragma pack(push, 1)

class CZigBeeTouchlinkDeviceInformation
{
	// Attributes
	public:
		// The endpoint identifier field is 8-bits in length and specifies the
		// endpoint identifier of the sub-device
		uint64_t m_qwAddress;

		// Core device information
		CZigBeeTouchlinkCoreDeviceInformation m_info;

		// Contains the sorting order of the sub-device referred to by this
		// device information record. This field is used to identify if a
		// sorting of sub-devices is needed and what the order is. A value of
		// zero indicates 'not sorted'; non-zero values indicate the order in
		// the list, with the value 0x01 indicating the top of the list
		uint8_t m_nSortOrder;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkFormNetworkRequest

#pragma pack(push, 1)

class CZigBeeTouchlinkFormNetworkRequest : public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// The "extended PAN identifier" field is 64-bits in length and
		// specifies the extended PAN identifier of the device responding
		// to the scan request. If this value is equal to zero, the target
		// shall determine the extended PAN ID for the new network
		uint64_t m_qwExtendedPANID;

		// Specifies the index of the key (and hence the protection method),
		// which was used to encrypt the network key
		uint8_t m_nScheme;

		// The network key to use for the network, encrypted according to the
		// algorithm indicated by the key index field
		uint8_t m_abKey[16];

		// The channel to be used for the new network. If this value is equal
		// to zero, the target shall determine the logical channel for the new
		// network
		uint8_t m_nChannel;

		// Contains the identifier of the new PAN. If this value is equal to
		// zero, the target shall determine the PAN identifier for the new
		// network
		uint16_t m_wPANID;

		// The "network address" field is 16-bits in length and specifies the
		// current network address of the device
		uint16_t m_wNetworkAddress;

		// The "group identifier begin" field is 16-bits in length and
		// specifies the start of the range of group identifiers that the
		// target can use for its endpoints
		uint16_t m_wFirstAllocatedGroupID;

		// The "group identifier end" field is 16-bits in length and
		// specifies the end of the range of group identifiers that the
		// target can use for its endpoints
		uint16_t m_wLastAllocatedGroupID;

		// The "free network address range begin" field is 16-bits in length
		// and specifies the start of the range of the network address that the
		// target can assign
		uint16_t m_wFirstAvailableAddress;

		// The "free network address range end" field is 16-bits in length
		// and specifies the end of the range of the network address that the
		// target can assign
		uint16_t m_wLastAvailableAddress;

		// The "free group identifier range begin" field is 16-bits in length
		// and specifies the start of the range of group identifiers that the
		// target can assign
		uint16_t m_wFirstAvailableGroupID;

		// The "free group identifier range end" field is 16-bits in length
		// and specifies the end of the range of group identifiers that the
		// target can assign
		uint16_t m_wLastAvailableGroupID;

		// The "initiator IEEE address" field is 64-bits in length and
		// specifies the IEEE address of the initiator of the new network
		uint64_t m_qwInitiatorAddress;

		// The "initiator network address" field is 16-bits in length and
		// specifies the short network address of the initiator of the new
		// network
		uint16_t m_wInitiatorAddress;

	// Implementation
	public:
#ifdef _DEBUG
		void Dump() const;
#endif
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkFormNetworkResponse

#pragma pack(push, 1)

class CZigBeeTouchlinkFormNetworkResponse : public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// Contains the status code corresponding to the result of the network
		// start request
		uint8_t m_nStatus;

		// Contains the extended identifier of the new PAN
		uint64_t m_qwExtendedPANID;

		// Network update identifier (always 0 for a new network)
		uint8_t m_nNetworkUpdateID;

		// The channel used by the new network
		uint8_t m_nChannel;

		// Contains the identifier of the new PAN
		uint16_t m_wPANID;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkJoinNetworkRequest

#pragma pack(push, 1)

class CZigBeeTouchlinkJoinNetworkRequest : public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// The extended PAN identifier field is 64-bits in length and specifies
		// the extended PAN identifier of the device responding to the scan
		// request
		uint64_t m_qwExtendedPANID;

		// Specifies the index of the key (and hence the protection method),
		// which was used to encrypt the network key
		uint8_t m_nScheme;

		// The network key to use for the network, encrypted according to the
		// algorithm indicated by the key index field
		uint8_t m_abKey[16];

		// The network update identifier field is 8-bits in length and specifies
		// the current value of the nwkUpdateId attribute of the originator
		uint8_t m_nNetworkUpdateID;

		// The channel to be used for the new network. If this value is equal
		// to zero, the target shall determine the logical channel for the new
		// network
		uint8_t m_nChannel;

		// Contains the identifier of the new PAN. If this value is equal to
		// zero, the target shall determine the PAN identifier for the new
		// network
		uint16_t m_wPANID;

		// The network address field is 16-bits in length and specifies the
		// current network address of the device
		uint16_t m_wNetworkAddress;

		// The network address field is 16-bits in length and specifies the
		// 1current network address of the device
		uint16_t m_wFirstAllocatedGroupID;

		// The network address field is 16-bits in length and specifies the
		// 1current network address of the device
		uint16_t m_wLastAllocatedGroupID;

		// The network address field is 16-bits in length and specifies the
		// 1current network address of the device
		uint16_t m_wFirstAvailableAddress;

		// The network address field is 16-bits in length and specifies the
		// 1current network address of the device
		uint16_t m_wLastAvailableAddress;

		// The network address field is 16-bits in length and specifies the
		// 1current network address of the device
		uint16_t m_wFirstAvailableGroupID;

		// The network address field is 16-bits in length and specifies the
		// 1current network address of the device
		uint16_t m_wLastAvailableGroupID;

	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkNetworkUpdateRequest

#pragma pack(push, 1)

class CZigBeeTouchlinkNetworkUpdateRequest : public CZigBeeTouchlinkMessage
{
	// Attributes
	public:
		// The extended PAN identifier field is 64-bits in length and contains
		// the extended PAN identifier of the network
		uint64_t m_qwExtendedPANID;

		// The network update identifier field is 8-bits in length and specifies
		// the current value of the nwkUpdateId attribute of the originator
		uint8_t m_nNetworkUpdateID;

		// The channel to be used for the new network. If this value is equal
		// to zero, the target shall determine the logical channel for the new
		// network
		uint8_t m_nChannel;

		// Contains the identifier of the new PAN. If this value is equal to
		// zero, the target shall determine the PAN identifier for the new
		// network
		uint16_t m_wPANID;

		// The network address field is 16-bits in length and specifies the
		// current network address of the device
		uint16_t m_wNetworkAddress;


	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeTouchlinkCommissioningInfo

class CZigBeeTouchlinkCommissioningInfo
{
	// Construction
	public:
		CZigBeeTouchlinkCommissioningInfo
			(const CZigBeeTouchlinkFormNetworkRequest &request,
			const uint_fast8_t nRequestSequence);

		CZigBeeTouchlinkCommissioningInfo
			(const CZigBeeTouchlinkJoinNetworkRequest &request,
			const uint_fast8_t nRequestSequence,
			const bool bAsRouter);

		CZigBeeTouchlinkCommissioningInfo
			(const CZigBeeTouchlinkCommissioningInfo &info);

	// Attributes
	public:
		// The network key to use for the network
		uint8_t m_abNetworkKey[16];

		// The extended PAN identifier of the device
		uint64_t m_qwExtendedPANID;

		// The network update identifier - only valid when joining a network,
		// not when forming a network
		uint8_t m_nNetworkUpdateID;

		// The channel to be used for the new network. If this value is equal
		// to zero, the target shall determine the logical channel for the new
		// network when forming a network
		uint8_t m_nChannel;

		// Contains the identifier of the new PAN. When forming a network and
		// this value is equal to zero, the target shall determine the PAN
		// identifier for the new network
		uint16_t m_wPANID;

		// The network address of the device
		uint16_t m_wNetworkAddress;

		// The SAS start-up control value (i.e. either form, resume or rejoin)
		uint8_t m_nStartupControl;

		// Start, join router or join end-device request ZCL sequence number
		uint8_t m_nRequestSequence;

		// Addressing material of the touch-link initiator
		CZigBeeAddress m_initiator;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterTouchlink

class CZigBeeClusterTouchlink : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the touch-link commissioning cluster
		enum { id = 0x1000 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Profile identifier for messages received over the stub APS. This is
		// the former ZigBee Light Link profile identifier
		enum { stubProfileID = 0xc05e };

		// Cluster command identifiers, part one. These are the commands
		// received over the standard ZigBee APS interface, i.e. those in the
		// utility part of touchlink
		enum { commandGetGroupIdentifiers = 0x41,
			commandGetEndpointList = 0x42 };

		// Cluster command identifiers, part two. These are the commands
		// received over the stub APS interface (inter-PAN), i.e. those in the
		// commissioning part of touchlink
		enum { commandScanRequest, commandScanResponse,
			commandDeviceInformationRequest, commandDeviceInformationResponse,
			commandIdentify = 0x06, commandResetToFactoryDefaults,
			commandFormNetwork = 0x10, commandFormNetworkResponse,
			commandJoinAsRouter, commandJoinAsRouterResponse,
			commandJoinAsEndDevice, commandJoinAsEndDeviceResponse,
			commandNetworkUpdate };

		// Encryption schemes (keys and associated cryptographic algorithms)
		enum { schemeDevelopment, schemeCertifiedProduct = 4,
			schemePrecertification = 15 };

		// Supported encryption schemes (keys and algorithms)
		enum { supportsDevelopment = 1 << schemeDevelopment,
			supportsCertifiedProduct = 1 << schemeCertifiedProduct,
			supportsPrecertification = 1 << schemePrecertification };

		// Touch-link information flags
		enum { factoryNew = 0x01, addressAssignmentCapable = 0x02,
			initiator = 0x10, zigbee3 = 0x80 };

		// ZigBee information flags
		enum { deviceRoleMask = 0x03, receiverOnWhenIdle = 0x04 };

		// Product-level options for enabling the touch-link behavior:
		// The least significant bit allows to disable touch-link completely
		// under application control, i.e. enabled when LSB = '1', disabled
		// otherwise. Use Enable() to change this value. Other flags include
		// a specific option to disable touch-link on centralized networks for
		// security reasons. Notice that this will disable all touch-link
		// commands, including start or join network, reset to factory new and
		// network update. Additionally, it is possible to restrict stealing,
		// i.e. prohibit re-commissioning via network start and network join
		// commands. Finally, factory reset via touch-link may be disabled.
		// These flags can be modified at runtime. For example, it is possible
		// to allow factory resets only within a certain period after power-up
		enum { enable = 0x01, enableOnCentralizedNetworks = 0x02,
			enableRecommissioning = 0x04, enableFactoryReset = 0x08,
			enableDefaultBehavior = enable | enableRecommissioning |
				enableFactoryReset };

	// Construction
	public:
		CZigBeeClusterTouchlink(CZigBeeFoundationApplication &application,
			const uint_fast8_t nOptions = enableDefaultBehavior);

	// Attributes
	public:
		// Returns true if a touch-link session is in progress. While it is in
		// progress, it is permissible to call GetSessionID()
		bool IsSessionActive() const;

	// Operations
	public:
		// Issues an INTRP-DATA.request for touch-link commissioning
		void OnRequestDataStub(const CZigBeeAddress &destination,
			CPacket &asdu, void *pArgument);

		// Enables or disables the touch-link interface. Notice that you might
		// have to enable the receiver using EnableReceiver() if the receiver
		// is not enabled
		void Enable(const bool bEnable = true);

		// Enables the receiver. Should be called when touch-link was disabled
		// at the time the networking stack was started, i.e. when Startup() of
		// CZigBeeApplicationSupportManagement was called and touch-link is
		// enabled afterwards. Must not be called, when the device is already
		// operating on a network
		void EnableReceiver();

	// Overridables
	public:
		// Invoked for any INTRP-DATA.confirm related to touch-link
		virtual void OnConfirmDataStub(const unsigned char nStatus,
			const unsigned int nTimestamp, void *pArgument);

		// Invoked for any INTRP-DATA.indication related to touch-link
		virtual void OnIndicateDataStub(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Invoked to identify. The default implementation forwards the request
		// to the identify cluster on all endpoints exposed over touch-link
		virtual void Identify(const uint16_t nDuration);

		// Invoked when a new session has just started. It is safe call
		// GetSessionID() from within this notification handler
		virtual void OnSessionEstablished();

		// Invoked when an existing session has expired. NOTICE: The session is
		// no longer active; hence, it is unsafe to call GetSessionID() from
		// within this notification handler. The parameter indicates whether
		// the session expired or was terminated, e.g. due to another scan
		// request
		virtual void OnSessionTerminated(const bool bExpired);

		// Called when a new set of network parameters has been commissioned
		// via touch-link. The device must now seize all previous activities
		// and start the zigbee networking stack. A simple way to do this is to
		// reboot; another way is to reset the application, network and MAC/PHY
		// and reinitialize and restart
		virtual void AfterCommit();

		// Called immediately before the new commissioning parameters will
		// take effect, i.e. immediately before forming or joining a network
		// in response to a touchlink commissioning action
		virtual void OnApply(const CZigBeeTouchlinkCommissioningInfo &info);

	// Overrides
	public:
		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// This notification is invoked when a device left the network
		virtual void OnNotifyNetworkLeave(const uint8_t nStatus,
			const bool bRejoin, uint8_t &actionFlags);

	// Implementation
	public:
		// Magic values for development key (Philips Lighting acronyms "PhLi"
		// and "CLSN" as 32-bit unsigned integers for little endian cores)
		enum { magic1 = 0x694c6850, magic2 = 0x4e534c43 };

		// Default identify time, in seconds
		enum { defaultIdentifyTime = 10 };

		// The maximum session life-time, in timer ticks (8 seconds
		// per specification)
		static const CCompactTimerService::ticks_diff_t sessionLifetime;

		// Cluster revision of the touch-link cluster
		static const unsigned short nClusterRevision;

		// The static elements of touch-link information, i.e. whether this is
		// a touch-link initiator or target, whether it supports zigbee 3.0 or
		// later, etc.
		static const uint8_t nCapabilities;

		// The RSSI correction field is 8-bits in length and specifies a
		// pre-programmed RSSI correction offset
		static const uint8_t nRSSICorrection;

		// The minimum RSSI value required for inter-PAN touch-link messages
		static const int8_t nMinimumRSSI;

		// Supported schemes (keys and associated ephemeral transport key
		// derivation algorithms) for the touch-link cluster
		static const uint16_t nSupportedSchemes;

		// The one and only touch-link cluster instance
		static CZigBeeClusterTouchlink *pInstance;

		// Commissioning data (used for deferred commissioning)
		CZigBeeTouchlinkCommissioningInfo *m_pCommissioningInfo;

		// Session timer. The timer argument is the 32-bit touch-link session
		CCompactTimer *m_pSessionTimer;

		// Last response identifier generated by this device for the current
		// session
		uint32_t m_nResponseID;

		// The IEEE 802.15.4 MAC layer capabilities of the initiator. This is
		// only valid when there is an active session
		uint8_t m_nInitiatorCapabilities;

		// The touch-link information of the initiator. This is only valid when
		// there is an active session
		uint8_t m_nInitiatorTouchlinkInformation;

		// A combination of flags that allow control of touch-link behavior.
		// For example, touch-link might be disabled completely, disabled on
		// centralized networks, or certain critical operations might be
		// restricted under application control
		uint8_t m_nEnable;

		// Enables or disables the touch-link interface for all supported
		// Touchlink Commissioning cluster instances
		static bool Enable(CZigBeeApplicationSupportBase &aps,
			const bool bEnable = true);

		// Called from within the global OnZigBeeIndicateDataStub(), if any
		// of the application endpoints includes the Touchlink Commissioning
		// cluster
		static void OnTouchLinkIndicateDataStub(const CZigBeeAddress &source,
			const CZigBeeAddress &destination, const uint_fast16_t profileID,
			const uint_fast16_t clusterID, CPacket &asdu,
			const uint_fast8_t linkQuality, const uint_fast32_t timestamp);

		// Called from within the global OnZigBeeConfirmDataStub(), if any
		// of the application endpoints includes the Touchlink Commissioning
		// cluster
		static void OnTouchLinkConfirmDataStub(const uint_fast8_t status,
			const uint_fast32_t timestamp, void *pArgument);

		// Returns the transaction sequence number of the active session. Must
		// not be called, when there is no active session, i.e. the session
		// timer is void
		uint32_t GetSessionID() const;

		// Enables the receiver for routers, unless touch-link is disabled
		void AutoEnableReceiver();

		// Processes incoming touch-link scan requests
		void OnScanRequest(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming touch-link device information requests
		void OnGetDeviceInformation(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming touch-link identify requests
		void OnIdentify(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming touch-link reset to factory defaults requests
		void OnResetToFactoryDefaults(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming touch-link form network requests
		void OnFormNetwork(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming touch-link join requests (both for joining as
		// router and joining as end-device)
		void OnJoinNetwork(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming touch-link network update requests
		void OnNetworkUpdate(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming Get group identifiers requests. Only required
		// for controller devices (e.g. remote controls)
		void OnGetGroupIdentifiers(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes incoming Get endpoint list requests. Only required for
		// controller devices (e.g. remote controls)
		void OnGetEndpointList(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// A timer call-back that fires when the active session expires
		void OnSessionExpired(void *pArgument);

		// Validates session identifiers on incoming touch-link messages
		bool ValidateSession(CPacket &asdu) const;

		// Verifies the incoming touch-link messages stem from an initiator in
		// touch-link range
		bool VerifyProximity(CPacket &asdu) const;

		// Transmits a simple response (only an 8-bit status code)
		void TransmitSimpleResponse(const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const uint_fast32_t nSessionID,
			const uint_fast8_t nStatus = statusSuccess,
			const bool bCommissioningUponConfirm = false);

		// Returns the set of endpoints, which are exposed over touch-link
		void GetExposedEndpoints(std::set<unsigned char> &endpoints) const;

		// Determines if the specified application is exposed over touch-link
		static bool IsExposed(const CZigBeeApplication &application);

		// Returns information about
		void GetDeviceInformation
			(const CZigBeeFoundationApplication &application,
			class CZigBeeTouchlinkDeviceInformation &info,
			const bool bUseAlternateProfile = false);

		// Returns the AES-128 ECB key to be used for encrypting/decrypting the
		// network key, according to the selected scheme (algorithm and key)
		// and session information (transaction and response identifiers).
		// Notice that in order to support the master key for certified
		// products, the default implementation must be overloaded by the final
		// application firmware
		static bool GetEncryptionKey(uint8_t abKey[16],
			const uint_fast32_t nSessionID, const uint_fast32_t nResponseID,
			const uint_fast8_t nScheme);

		// Provides a base implementation of GetEncryptionKey(), which can be
		// used by overloaded implementations to handle scheme #0 (development)
		// and scheme #15 (pre-certification)
		static bool DefaultGetEncryptionKey(uint8_t abKey[16],
			const uint_fast32_t nSessionID, const uint_fast32_t nResponseID,
			const uint_fast8_t nScheme);

		// Prepares calculation of an ephemeral transport key by expanding the
		// 32-bit session and response identifiers to a 128-bit key, which will
		// subsequently be encrypted with either the master key for certified
		// products or the pre-certification key
		static void ExpandKey(uint8_t abTransportKey[16],
			const uint_fast32_t nSessionID, const uint_fast32_t nResponseID);

		// Dervies the ephemeral (session-specific) transport key. On input,
		// abTransportKey must be filled with the session ID and abKey must
		// be either the pre-certification key or the master key for certified
		// products
		static void DeriveTransportKey(uint8_t abTransportKey[16],
			const uint8_t abKey[16]);

		// Encrypts the network key according to the specified scheme and using
		// the supplied session information
		static bool EncryptNetworkKey(uint8_t abNetworkKey[16],
			const uint_fast32_t nSessionID, const uint_fast32_t nResponseID,
			const uint_fast8_t nScheme);

		// Decrypts the network key according to the specified scheme and using
		// the supplied session information
		static bool DecryptNetworkKey(uint8_t abNetworkKey[16],
			const uint_fast32_t nScheme, const uint_fast32_t nSessionID,
			const uint_fast32_t nResponseID);

		// Performs a factory-reset, commits new commissioning info and invokes
		// AfterCommit(), which may reboot the device or restart the networking
		// stack and perform any other application-level shutdown/start-up
		// behavior that might be required
		void Commit(const CZigBeeTouchlinkCommissioningInfo &info);

		// Gracefully leaves the network (if the device is not factory new and
		// currently operating on a network) and then applies the new network
		// settings, i.e. forms or joins a network. Notice, this comprises a
		// factory-reset and reboot the device after leaving the network
		void Apply(const CZigBeeTouchlinkCommissioningInfo &info);

		// Invoked during network formation, immediately before network
		// formation is about to finish
		static bool BeforeFinishNetworkFormation(CZigBeeNetworkManagement &nlme,
			CZigBeeNetworkFormationContext &info);

		// Returns true if touch-link is currently enabled on the device,
		// taking into account global enable and enable on centralized network
		// policies
		bool IsEnabled() const;

		// Returns true if the command is a network commissioning command, i.e.
		// belongs to the group of network start, network join as router, and
		// network join as end-device
		static bool IsNetworkCommissioningCommand
			(const CZigBeeClusterLibraryHeader &header);

		// Returns true if the command is a factory reset
		static bool IsFactoryResetCommand
			(const CZigBeeClusterLibraryHeader &header);
};

#define __COMPACT7B_STUBCOMMAND_C2S(command, pfn) \
		{ command, CZigBeeClusterCommand::typeStub, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_STUBCOMMAND_S2C(command, pfn) \
		{ command, CZigBeeClusterCommand::typeStub | \
			CZigBeeClusterCommand::directionServerToClient, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_STUBCOMMAND_C2S_M(manufacturerID, command, pfn) \
		{ command, CZigBeeClusterCommand::typeStub | \
			CZigBeeClusterCommand::manufacturerSpecific, manufacturerID, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_STUBCOMMAND_S2C_M(manufacturerID, command, pfn) \
		{ command, CZigBeeClusterCommand::typeStub | \
			CZigBeeClusterCommand::directionServerToClient | \
			CZigBeeClusterCommand::manufacturerSpecific, manufacturerID, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterTouchlinkDispatch

class CZigBeeClusterTouchlinkDispatch
{
	// typedefs & enums
	public:

	// Attributes
	public:
		// Callback for enabling or disabling touchlink feature support
		bool (*m_enable)(CZigBeeApplicationSupportBase &aps,
			const bool bEnable);

	// Operations
	public:
		// Request enabling/disabling touchlink feature support
		static bool EnableTouchlink(CZigBeeApplicationSupportBase &aps,
			const bool enable);
};


#define __C7BFX_TOUCHLINK_DELEGATE_IMPLEMENT_EX(id, ...) \
	__COMPACT_SECTION_IMPLEMENT(CZigBeeClusterTouchlinkDispatch, \
		c7bfx_tld, id, __VA_ARGS__)

#define __C7BFX_TOUCHLINK_DELEGATE_IMPLEMENT(identifier, ...) \
	__C7BFX_TOUCHLINK_ENTITY_IMPLEMENT_EX( \
	__c7bfx_tld##identifier, identifier, __VA_ARGS__)
