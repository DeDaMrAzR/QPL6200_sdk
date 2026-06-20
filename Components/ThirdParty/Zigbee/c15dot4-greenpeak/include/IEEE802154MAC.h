// IEEE802154MAC.h : header file
//
// Copyright(C) 2008-2024 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact15.4(TM) IEEE 802.15.4 compliant medium access control
// layer implementation. Interfaces to PHY layer implementations derived from
// the abstract CIEEE802154PHY base class.
//
// This implementation is based on the IEEE Standard for Information
// Technology - Telecommunications and information exchange between systems -
// Local and metropolitan area networks - specific requirements - Part 15.4:
// Wireless Medium Access Control (MAC) and Physical Layer (PHY)
// Specifications for Low-Rate Wireless Personal Area Networks (WPANs), as of
// September 8, 2006

/////////////////////////////////////////////////////////////////////////////
// forward referenced classes

class CIEEE802154MAC;
class CIEEE802154MCF;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCPSSAPClient

class CIEEE802154MCPSSAPClient
{
	// Overridables
	public:
		// MCPS-DATA.confirm(msduHandle, status, Timestamp) service primitive,
		// according to clause 7.1.1.2.1
		virtual void OnConfirmData(const unsigned char nHandle,
			const unsigned char nStatus,
			const unsigned int nTimestamp = 0) = 0;

		// MCPS-DATA.indication(SrcAddrMode, SrcPANId, SrcAddr, DstAddrMode,
		// DstPANId, DstAddr, msduLength, msdu, mpduLinkQuality, DSN, Timestamp,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex) service primitive,
		// according to clause 7.1.1.2.1
		virtual void OnIndicateData(const CIEEE802154Address &sourceAddress,
			const CIEEE802154Address &destinationAddress, CPacket &msdu,
			const unsigned char nLinkQuality,
			const unsigned char nDataSequence,
			const unsigned int nTimestamp = 0,
			const CIEEE802154SecurityInfo *const pSecurity = 0) = 0;

		// MCPS-DATA.indication(mpduLength, mpdu, mpduLinkQuality, Timestamp)
		// service primitive, according to clause 7.1.1.2.1, specifically for
		// frames received in promiscuous mode
		virtual void OnIndicatePromiscuousData(CPacket &mpdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp,
			const unsigned char nChannel);

	// Implementation
	public:
		virtual ~CIEEE802154MCPSSAPClient();
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MLMESAPClient

class CIEEE802154MLMESAPClient
{
	// Overridables
	public:
		// MLME-START.confirm(status), clause 7.1.14.2
		virtual void OnConfirmStart(const unsigned char nStatus) = 0;

		// MLME-SCAN.confirm(status, ScanType, ChannelPage, UnscannedChannels,
		//	ResultListSize, EnergyDetectList, PANDescriptorList),
		// clause 7.1.11.2
		virtual void OnConfirmScan(const unsigned char nStatus,
			const unsigned char nScanType, const unsigned char nChannelPage,
			const unsigned int nUnscannedChannels,
			const std::list<unsigned char> *const pListEnergyDetect) = 0;

		// MLME-BEACON-NOTIFY.indication(BSN, PANDescriptor, PendAddrSpec,
		// AddrList, sduLength, sdu), clause 7.1.5.1
		virtual void OnIndicateBeaconNotify(const unsigned char nSequence,
			const CIEEE802154PANDescriptor &descriptor, CPacket &sdu) = 0;

		// MLME-POLL.indication(address). This is an extension to the IEEE
		// 802.15.4 MAC introduced by the ZigBee Alliance core stack for its
		// release 21 and allows a parent router to inject a leave message for
		// child that has changed parents
		virtual bool OnIndicatePoll(const CIEEE802154MHR &header,
			unsigned char &bPendingControl, CPacketEx *&pPending) = 0;

		// MLME-POLL.confirm(status), clause 7.1.16.2
		virtual void OnConfirmPoll(const unsigned char nStatus) = 0;

		// MLME-COMM-STATUS.indication(PANId, SrcAddrMode, SrcAddr,
		// DstAddrMode, DstAddr, status, SecurityLevel, KeyIdMode, KeySource),
		// clause 7.1.12.1
		virtual void OnIndicateCommStatus(const CIEEE802154Address &source,
			const CIEEE802154Address &destination, const unsigned char nStatus,
			CIEEE802154SecurityInfo *const pSecurity = 0) = 0;

		// MLME-ASSOCIATE.confirm(AssocShortAddress, status, SecurityLevel,
		// KeyIdMode, KeySource, KeyIndex), clause 7.1.3.4
		virtual void OnConfirmAssociate(const unsigned short wShortAddress,
			const unsigned char nStatus,
			const CIEEE802154SecurityInfo *const pSecurity = 0) = 0;

#ifdef __COMPACT15DOT4_FFD__
		// MLME-ASSOCIATE.indication(DeviceAddress, CapabilityInformation,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.3.2
		virtual void OnIndicateAssociate(const CIEEE802154Address &device,
			const unsigned char bCapabilities,
			CIEEE802154SecurityInfo *const pSecurity = 0) = 0;
#endif

		// MLME-DISASSOCIATE.confirm(status, DeviceAddrMode, DevicePANId,
		// DeviceAddress), clause 7.1.4.3
		virtual void OnConfirmDisassociate(const unsigned char nStatus,
			const CIEEE802154Address &device) = 0;

		// MLME-DISASSOCIATE.indication(DeviceAddress, DisassociateReason,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.4.2
		virtual void OnIndicateDisassociate(const CIEEE802154Address &device,
			const unsigned char nReason,
			const CIEEE802154SecurityInfo *const pSecurity = 0) = 0;

#ifdef __COMPACT15DOT4_FFD__
		// MLME-ORPHAN.indication(OrphanAddress, SecurityLevel, KeyIdMode,
		// KeySource, KeyIndex)
		virtual void OnIndicateOrphan(const CIEEE802154Address &orphan,
			const CIEEE802154SecurityInfo *const pSecurity = 0) = 0;
#endif

		// Enhancement to the standard IEEE 802.15.4 MLME SAP. Notifies the NHL
		// of the MAC layer being idle, i.e. no packets pending in the RX and
		// TX queues and all other processing has completed
		virtual void OnIndicateIdle() = 0;

	// Implementation
	public:
		virtual ~CIEEE802154MLMESAPClient();
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MACClient

class CIEEE802154MACClient : public CIEEE802154MCPSSAPClient,
	public CIEEE802154MLMESAPClient
{
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154ScanInfo

class CIEEE802154ScanInfo
{
	// typedefs & enums
	public:
		// Callback function prototype for custom incoming frame filters
		typedef bool (*FILTERCALLBACK)(CIEEE802154MAC &mac,
			const CIEEE802154MHR &header, const CPacket &ppdu,
			const bool bAfterDequeue);

	// Construction
	public:
		CIEEE802154ScanInfo(const unsigned char nScanType,
			const unsigned char nChannelPage, const unsigned int nScanChannels,
			const unsigned int nScanDuration,
			const CIEEE802154SecurityInfo *const pSecurity,
			const unsigned short wOriginalPANID,
			const unsigned short wOriginalShortAddress,
			const unsigned char nOriginalChannel,
			const bool bOriginalTransceiverState);

	// Attributes
	public:
		// Scan duration in symbols (not the original ScanDuration paramater)
		const unsigned int m_nScanDuration;

		// Initially, the set of channels to scan. Each time a channel has been
		// scanned successfully, the corresponding bit is cleared and the next
		// channel on the list is scanned (or the scan is finished)
		unsigned int m_nScanChannels;

		// List of channels that have not been scanned due to channel access
		// failures. Not valid in ED scans
		unsigned int m_nUnscannedChannels;

		// An array of ED measurement results
		std::list<unsigned char> *m_pListEnergyDetect;

		// Scan type (ED, active, passive, orphan)
		const unsigned char m_nScanType;

		// Channel page
		const unsigned char m_nChannelPage;

	// Implementation
	public:
		~CIEEE802154ScanInfo();

		// Original transceiver state; saved before active/passive scanning
		// actually starts and restored based on this value afterwards
		unsigned char m_bOriginalTransceiverState;

		// Original channel; saved before active/passive scanning actually
		// starts and restored from this value afterwards
		unsigned char m_nOriginalChannel;

		// Original PAN identifier; saved before active/passive scanning
		// actually starts and restored from this value afterwards
		unsigned short m_wOriginalPANID;

		// Original channel; saved before active/passive scanning actually
		// starts and restored from this value afterwards
		unsigned short m_wOriginalShortAddress;

		// Pointer to the timer object that is used during scans. May be used
		// to cancel the timer
		CCompactTimer *m_pTimer;

		// Time when scan on current channel started. Used in conjunction with
		// the ScanDuration parameter passed as an argument in the
		// MLME-SCAN.request. Only valid for ED scans
		unsigned int m_nStartTime;

		// A filter for incoming frames (only used during custom scans)
		FILTERCALLBACK m_pfnFilter;

		// True, if at least one beacon request or orphan notification command
		// frame has been successfully transmitted
		bool m_bRequestTransmitted;

		// True, if a beacon (active/passive scan) or coordinator realignment
		// (orphan scan) frame has been received while scanning
		bool m_bResponseReceived;

		// Status code for the resulting MLME-SCAN.confirm
		unsigned char m_nStatus;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154PLMESAPClient

class CIEEE802154PLMESAPClient
{
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MLME

class CIEEE802154MLME : public CCompactTimerClient,
	public CIEEE802154PLMESAPClient
{
	friend class CIEEE802154MAC;

	// typedefs & enums
	public:
		// MLME-SCAN.request ScanType values, according to 7.1.11.1.1. We add
		// another scan mode ("custom"), that is not part of the IEEE 802.15.4
		// specification. This custom scan mode allows higher layer protocols,
		// such as ZigBee Green Power, to make use of the MAC layer's scanning
		// abilities when the device needs to change channels temporarily. For
		// example, this feature facilitates ZigBee Green Power waiting for a
		// channel request frame on a channel distinct form the operational
		// channel of the ZigBee network during GPD commissioning
		enum { scanEnergyDetection, scanActive, scanPassive, scanOrphan,
			scanCustom };

		// The lowest and highest channel numbers supported by the MAC/PHY. At
		// the same time, the difference is the size of the ED list. Notice
		// this is specific to 2.4GHz radios; if the stack is to support
		// multiple radios, this must be queried from the underlying PHY PIB
		enum { firstChannel = 11, lastChannel = 26 };

		// The implementation-specified maximum number of entries in the PAN
		// descriptor list
		enum { maxDescriptorCount = 25 };

		// The implementation-specified maximum number of entries in the ED
		// result list
		enum { maxEnergyDetectionResults = 16 };

		// These bits influence how MAC ACK frames are generated (with the
		// pending flag set or clear). Also reports the pending flag actually
		// used (which might differ from the intended setting in case timing
		// constraints were violated). This Green Peak MAC to Compact15.4
		// adaptation layer only supports a subset of the flags, as far as
		// required to map between both implementations
		enum { pendingIntended = 0x01, pendingActual = 0x02,
			pendingActualShift = 1, pendingInject = 0x04,
			pendingProvide = 0x08, pendingStage1 = 0x10, pendingEmit = 0x20,
			preprocessOnce = 0x40,
			preprocessNewFrame = CIEEE802154PHY::preprocessNewFrame };

	// Construction
	public:
		CIEEE802154MLME(CIEEE802154MAC &mac);

	// Attributes
	public:
		// Link to the associated MAC layer instance
		CIEEE802154MAC &m_mac;

		// Returns true, if a scan is currently in progress
		bool IsScanInProgress() const;

		// Extension for Qorvo: Optionally remap a scan-type (e.g. active scan)
		// to another (custom) scan-type (e.g. advanced active scan accounting
		// for co-located WiFi networks)
		static std::pair<std::pair<unsigned char, unsigned char>, bool> scanRemap;

	// Operations
	public:
		// Returns the first channel (starting from the lowest channel number)
		// that is flagged in the supplied channel mask
		static bool GetFirstEnabledChannel(const unsigned int nMask,
			unsigned char &nChannel);

		// Direct call version of an MLME-RESET.request(SetDefaultPIB), clause
		// 7.1.9.1, resulting in an immediate MLME-RESET.confirm(SUCCESS),
		// clause 7.1.9.2
		void Reset(const bool bSetDefaultPIB);

		// Direct call version of an MLME-SET.request(0x53, xx, shortAddress),
		// clause 7.1.13.1, resulting in an immediate MLME-SET.confirm(SUCCESS,
		// 0x53, xx), clause 7.1.13.2
		void SetShortAddress(const unsigned short wShortAddress);

		// Direct call version of an MLME-SET.request(0x50, xx, PANId),
		// clause 7.1.13.1, resulting in an immediate MLME-SET.confirm(SUCCESS,
		// 0x50, xx), clause 7.1.13.2
		void SetPANID(const unsigned short wPANID);

		// Direct call version of an MLME-SET.request(0x51, xx, promiscuous),
		// clause 7.1.13.1, resulting in an immediate MLME-SET.confirm(SUCCESS,
		// 0x51, xx), clause 7.1.13.2
		void EnablePromiscuousMode(const bool bEnable);

#ifdef __COMPACT15DOT4_FFD__
		// MLME-START.request(PANId, LogicalChannel, ChannelPage, StartTime,
		// BeaconOrder, SuperframeOrder, PANCoordinator, BatteryLifeExtension,
		// CoordRealignment, CoordRealignSecurityLevel, CoordRealignKeyIdMode,
		// CoordRealignKeySource, CoordRealignKeyIndex, BeaconSecurityLevel,
		// BeaconKeyIdMode, BeaconKeySource, BeaconKeyIndex), clause 7.1.14.1
		void OnRequestStart(const unsigned short wPANID,
			const unsigned char nChannel, const unsigned char nPage,
			const unsigned int nStartTime, const unsigned char nBeaconOrder,
			const unsigned char nSuperframeOrder, const bool bPANCoordinator,
			const bool bBatteryLifeExtension, const bool bRealignment,
			const CIEEE802154SecurityInfo *const pRealignSecurity = 0,
			const CIEEE802154SecurityInfo *const pBeaconSecurity = 0);
#endif

		// MLME-SCAN.request(ScanType, ScanChannels, ScanDuration, ChannelPage,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.11.1
		void OnRequestScan(const unsigned char nScanType,
			const unsigned int nScanChannels, const unsigned char nScanDuration,
			const unsigned char nChannelPage,
			const CIEEE802154SecurityInfo *const pSecurity = 0,
			const CIEEE802154ScanInfo::FILTERCALLBACK pfnFilter = 0);

		// MLME-ASSOCIATE.request(LogicalChannel, ChannelPage, CoordAddrMode,
		// CoordPANId, CoordAddress, CapabilityInformation, SecurityLevel,
		// KeyIdMode, KeySource, KeyIndex), clause 7.1.3.1
		void OnRequestAssociate(const unsigned char nChannel,
			const unsigned char nPage,	const CIEEE802154Address &coordinator,
			const unsigned char bCapabilities,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

#ifdef __COMPACT15DOT4_FFD__
		// MLME-ASSOCIATE.response(DeviceAddress, AssocShortAddress, status,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex), clause 7.1.3.3
		void OnRespondAssociate(const CIEEE802154Address &device,
			const unsigned short wShortAddress, const unsigned char nStatus,
			const CIEEE802154SecurityInfo *const pSecurity = 0);
#endif

		// MLME-DISASSOCIATE.request(DeviceAddrMode, DevicePANId,
		// DeviceAddress, DisassociateReason, TxIndirect, SecurityLevel,
		// KeyIdMode, KeySource, KeyIndex)
		void OnRequestDisassociate(const CIEEE802154Address &device,
			const unsigned char nReason, const bool bTransmitIndirect = false,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

#ifdef __COMPACT15DOT4_FFD__
		void OnRespondOrphan(const CIEEE802154Address &orphan,
			const unsigned short wShortAddress, const bool bAssociatedMember,
			const CIEEE802154SecurityInfo *const pSecurity = 0);
#endif

		// MLME-POLL.request(CoordAddrMode, CoordPANId, CoordAddress,
		// SecurityLevel, KeyIdMode, KeySource, KeyIndex)
		void OnRequestPoll(const CIEEE802154Address &coordinator,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

		// MCPS-DATA.indication-like service primitive for management data. It
		// is called by the MAC when frames other than data frames have been
		// received. Complete header made available
		void OnIndicateManagementData(const CIEEE802154MHR &header,
			CPacket &msdu, const unsigned char nLinkQuality,
			const unsigned int nTimestamp);

		// Returns the built-in, immutable EUI-64 burnt into a transceiver's
		// or system-on-chip's ROM in the factory. Platforms that do not
		// provide such a built-in EUI-64 return 'all-F's (void address)
		uint_fast64_t GetImmutableExtendedAddress() const;

#ifdef __COMPACT15DOT4_HARDWARE_ADDRESSING__
		// If the radio supports address filtering in hardware, we need to
		// inform the radio about the extended address (constant), short
		// address, PAN ID, and device role (PAN coordinator?). This method
		// transfers this information from the MAC PIB to the radio
		void UpdateAddressingMaterial();

		// If the radio supports address filtering in hardware, this function
		// reads the addressing information stored in the MAC and copies it
		// to the local MAC PIB mirror
		void RefreshAddressingMaterial(const bool bOnlyExtended = false);

		// Radio hardware, which maintains a child address cache, is notified
		// of changes to the NHL's view on what are child devices
		void UpdateChildAddressCache
			(const std::vector<unsigned short> &addresses,
			const bool onlyRebuild = false);
#endif

		// Reads the coordinator addressing material from the GreenPeak MAC
		void RefreshCoordinatorAddressingMaterial();

		// Inject a leave message (ZigBee 2015 child aging, cross-layer)
		void IndicatePoll(const CIEEE802154Address &address,
			unsigned char bPendingControl);

		// Called when the MAC is idle state
		void OnIdle();

		// Called to obtain MAC statistics
		void GetStatistics(CIEEE802154MACStatistics &statistics) const;

	// Implementation
	public:
		~CIEEE802154MLME();

		// Pointer to a scan info object if a MLME scan is currently in
		// progress, null otherwise
		CIEEE802154ScanInfo *m_pScanInfo;

		// Terminates a scan that is currently in progress and issues a
		// MLME-SCAN.confirm() primitive. Restores the original PANID that was
		// before the scan was started or switches to the new PANID (if
		// different from the broadcast address)
		void TerminateScan(const unsigned char nStatus);

		// Called when the scan timer has expired. In this Qorvo MAC API glue
		// layer, the scan timer is only used for "custom" scans, which are
		// used to implement temporary channel changes during bidirectional
		// Green Power commissioning (channel request/response exchanges)
		void OnScanTimerExpired(void *pArgument);

		// Maps a standard scan type to a custom replacement scan type
		static unsigned char RemapScanType(const unsigned char nScanType);

#ifdef _DEBUG
		// Dump MLME information
		void Dump() const;
#endif

	private:
		// Local shadow copy of the NWK child address cache which contains
		// the 16-bit short network addresses of all currently active child
		// devices in a sorted manner. This is used to distinguish real
		// changes, minimize remedy effort and avoid content fluctuation
		std::vector<unsigned short> m_childAddressesCache;
};

inline bool CIEEE802154MLME::IsScanInProgress() const
	{ return m_pScanInfo ? true : false; }


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MAC

class CIEEE802154MAC : public CIEEE802154PDSAPClient,
	public CIEEE802154PLMESAPClient, public CCompactTimerClient
{
	friend class CIEEE802154MLME;

	// typedefs & enums
	public:
		// Handle values. Handles 0..239 are available to the next higher layer
		// for data requests; handles 240..254 are reserved for the MLME or
		// other internal use; the value -1 (255) identifies an invalid handle
		enum { handleInvalid = 0xff };

		// MCPS-DATA.request transmit options. The transmitWithControlledTiming
		// option queues a frame and waits for the application to actually fire
		// transmission at a later stage
		enum { transmitAcknowledged = 0x01, transmitGTS = 0x02,
			transmitIndirect = 0x04, transmitWithControlledTiming = 0x10 };

		// Status codes
		enum { statusSuccess = 0, statusCounterError = 0xdb,
			statusBeaconLoss = 0xe0, statusChannelAccessFailure,
			statusDenied, statusDisableTransceiverFailure, statusSecurityError,
			statusFrameTooLong, statusInvalidGTS, statusInvalidHandle,
			statusInvalidParameter, statusNoAcknowledgement, statusNoBeacon,
			statusNoData, statusNoShortAddress, statusTransactionExpired = 0xf0,
			statusTransactionOverflow = 0xf1, statusInvalidAddress = 0xf5,
			statusLimitReached = 0xfa, statusScanInProgress = 0xfc };

		// Constants
		enum { baseSlotDuration = 60, numSuperframeSlots = 16,
			unitBackoffPeriod = 20, minMPDUOverhead = 9, minCAPLength = 440,
			maxSIFSFrameSize = 18, maxMPDUUnsecuredOverhead = 25,
			maxLostBeacons = 4, maxBeaconOverhead = 75,
			numGTSDescPersistenceTime = 4,
			baseSuperframeDuration = baseSlotDuration * numSuperframeSlots,
			maxSafePayloadSize =
				CIEEE802154PHY::maxPacketSize - maxMPDUUnsecuredOverhead,
			maxPayloadSize = CIEEE802154PHY::maxPacketSize - minMPDUOverhead,
			maxBeaconPayloadLength =
				CIEEE802154PHY::maxPacketSize - maxBeaconOverhead };

		// Capability information field
		enum { capabilityAlternatePANCoordinator = 0x01,
			capabilityFullFunctionDevice = 0x02, capabilityMainsPowered = 0x04,
			capabilityReceiverOnWhenIdle = 0x08,
			capabilitySupportsSecurity = 0x40,
			capabilityAllocateAddress = 0x80 };

	// Construction
	public:
		CIEEE802154MAC(CIEEE802154PHY &phy);

	// Attributes
	public:
		// Link to physical layer instance. This is typically a physical layer
		// convergence protocol implementation for a particular radio, i.e.
		// CC2420, CC2520, AT86RF230, or similar. Despite being a reference,
		// this may also be null,
		CIEEE802154PHY &m_phy;

		// Link to logical link layer instance. This is typically an 802.2 LLC
		// layer implementation, or a network layer implementation (ZigBee/NWK)
		CIEEE802154MACClient *m_pClient;

		// The 64-bit IEEE address assigned to the device (aExtendedAddress
		// MAC layer constant). This is obtained from the GreenPeak MAC
		static uint64_t qwExtendedAddress;

		// MAC protocol information base
		CIEEE802154MACPIB m_pib;

		// MAC layer management entity
		CIEEE802154MLME m_mlme;

		// Returns the GreenPeak MAC stack ID for this stack
		uint8_t GetStackID() const;

		// Returns true, if the device operates on a beacon-enabled PAN
		bool IsBeaconing() const;

	// Overrides (CIEEE802154PDSAPClient)
	public:
		// Implementation specific virtual overridable. Allows very early
		// MAC-level pre-processing of PHY frames, as soon as data begins to
		// arrive at the PHY. Can be used for time-critical tasks, such as
		// acknowledgment generation, for instance. Might be called several
		// times for the same frame during reception, until the frame is
		// complete (in which case cbRemaining is zero)
		virtual bool PreprocessData(const CPacket &psdu,
			const CPacketData::size_type cbRemaining);

		// PD-DATA.confirm(status) service primitive
		virtual void OnConfirmData(const unsigned char nStatus,
			const bool bAcknowledgment);

		// PD-DATA.confirm(status) service primitive
		virtual void OnConfirmData(const unsigned char nStatus);

		// PD-DATA.indication(psduLength, psdu, ppduLinkQuality) service
		// primitive. Notice that psduLength and psdu are part of the packet
		virtual void OnIndicateData(CPacket &psdu,
			const unsigned char nLinkQuality);

		// PLME-SET-TRX-STATE.confirm(status) primitive
		virtual void OnConfirmSetTransceiverState(const unsigned char nStatus);

	// Operations
	public:
		// MCPS-DATA.request(SrcAddrMode, DstAddrMode, DstPANId, DstAddr,
		// msduLength, msdu, msduHandle, TxOptions, SecurityLevel, KeyIdMode,
		// KeySource, KeyIndex) service primitive, clause 7.1.1.1.1. Notice:
		// if the NHL specifies advanced timing control in transmit options, it
		// MUST ALWAYS issue MCPS-COMMENCE.request or MCPS-ABORT.request by
		// calling Commence() or Abort(), respectively
		void OnRequestData(const unsigned char nSourceAddressingMode,
			const CIEEE802154Address &destinationAddress, CPacket &msdu,
			const unsigned char nHandle, const unsigned char bOptions = 0,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

		// Takes a MSDU and prepares it for injection (indirect transmission)
		CPacketEx *MakeInjectable(const CIEEE802154Address &source,
			const CIEEE802154Address &destination, CPacket &msdu,
			const unsigned char bOptions, const unsigned char nType,
			const CIEEE802154SecurityInfo *const pSecurity = 0);

#ifdef __COMPACT15DOT4_FFD__
		// Direct call version of MCPS-PURGE.request(msduHandle) service
		// primitive, clause 7.1.1.4 and MCPS-PURGE.confirm(msduHandle, status)
		// service primitive, clause 7.1.1.5, as immediate return value
		unsigned char Purge(const unsigned char nHandle);
#endif

		// This is an extension to IEEE 802.15.4 MAC, which immediately fires
		// transmission of a pending and queued packet with controlled timing.
		// Returns true, if the pending frame (which must have been queued with
		// the "controlled timing" transmit option is being transmitted, false
		// in case of an error (no pending frame with controlled timing)
		bool Commence();

		// This is an extension to IEEE 802.15.4 MAC, which aborts transmission
		//	of a pending (and queued) packet with controlled timing. Returns
		// true, if the pending frame (which must have been queued with the
		// "controlled timing" transmit option is being transmitted, false in
		// case of an error (no pending frame with controlled timing)
		bool Abort();

		// Registers a client instance
		void Register(CIEEE802154MACClient &client);

		// Must be called from within the higher layer thread to dequeue and
		// pump data indications
		void ProcessPendingPackets();

		// Can be used instead of ProcessPendingPackets() in a loop. Returns
		// true, if there are more packets pending, false otherwise
		bool ProcessNextPendingPacket();

		void HandleReceivedPacket(CPacket &mpdu,
				unsigned int nTimestamp, unsigned char nLinkQuality);

#ifndef __COMPACT15DOT4_HARDWARE_FCS__
		// Calculates and appends a frame check sequence to the specified
		// packet. This is a pure software implementation as a fall-back for
		// radios that do not support FCS generation in hardware
		static void AppendFrameCheckSequence(CPacket &mpdu);

		// Calculates and verifies the frame check sequence of the specified
		// packet. This is a pure software implementation as a fall-back for
		// radios that do not support FCS validation in hardware
		static bool ValidateFrameCheckSequence(const CPacket &mpdu);
#endif

		// Returns an address that can be used as a broadcast address for the
		// PAN this device operates on (bAllNetworks = false), or all devices
		// on all PANs at the POS
		CIEEE802154Address GetBroadcastAddress(const bool bAllNetworks = false) const;

		// Returns the source address in the specified form (either short or
		// extended address)
		CIEEE802154Address GetSourceAddress(const unsigned char nType) const;

		// Returns the source address in its default form
		CIEEE802154Address GetDefaultSourceAddress() const;

		// Returns the source address in extended form
		CIEEE802154Address GetExtendedSourceAddress() const;

		// Returns the source addressing mode that is appropriate for this
		// device, when MPDUs are assembled for transmission
		unsigned char GetDefaultSourceAddressingMode() const;

		// Returns true, if this station has successfully performed
		// association, or started a network as the PAN coordinator
		bool HasAssociated() const;

		// Returns a pointer to the packet, which is currently being processed,
		// i.e. dequeued and transferred to the next higher layer in an
		// MCPS-DATA.indication
		const CPacket *GetDequeued() const;

		// Indicate active state. Active means there is a transmission,
		// reception or scan currently in progress or imminent, e.g.
		// ongoing/pending transmission, received data pending processing,
		// receiver remaining in on state due to ongoing scanning,
		// expectation of immediate data reception or "RX-on-when-idle" = true.
		// Useful as a data point for power management.
		bool IsActive() const;

	// Implementation
	public:
		// Map of handles, which maps GreenPeak MAC PD handles to handles
		// defined by the next higher layer
		std::map<uint8_t, unsigned char> m_pendingTX;

		// Receive queue. Received MPDUs are queued for delivery to the next
		// higher layer. Not used by the provided default implementation of
		// QueueReceivedPacket().
		CPacketQueue m_queueRX;

		// This is the frame currently being processed (dequeued). Null if no
		// frame is being processed at present time
		CPacket *m_pDequeued;

		// Indicates if an immediate data request should be triggered in
		// response to the receipt of an inbound data frame with the pending
		// bit enabled
		bool m_dataRequest;

		// Inserts a frame header with the specified frame control subfield
		// settings in front of the supplied MSDU. Adding the MFR to the result
		// leads to an MPDU that can be passed to the PHY for transmission
		static void PrependHeader(CPacket &msdu,
			const CIEEE802154Address &source,
			const CIEEE802154Address &destination, unsigned short wFrameControl,
			const unsigned char nSequence,
			const CIEEE802154SecurityInfo *const pSecurity);

		// Determines the size of a frame header with the specified frame
		// control subfield settings
		static CPacketData::size_type DetermineHeaderSize
			(const CIEEE802154Address &source,
			const CIEEE802154Address &destination);

	protected:
		// Maximum number of packets in the MAC transmit queue
		static const unsigned char nTransmitQueueLimit;

		// Maximum number of packets in the MAC receive queue
		static const unsigned char nReceiveQueueLimit;

		// Maximum number of packets in the MAC indirect queue
		static const unsigned char nTransactionLimit;

		// Processes incoming packets before they are stored in the queue
		bool PreprocessPacket(const CPacket &psdu,
			const bool bValidateFrameCheckSequence = true);

		// Performs necessary processing related to acknowledgment frames
		bool ProcessAcknowledgment(const CPacket &psdu,
			const bool bAcknowledgmentTimerCanceled, const bool bAccept,
			bool &bReceivedAcknowledgment);

#ifndef __COMPACT15DOT4_HARDWARE_FCS__
		// Called repeatedly for each byte of MHR and payload to calculate the
		// 16-bit ITU-T CRC
		static unsigned short UpdateChecksum(const unsigned short nChecksum,
			unsigned char bData);
#endif

	public:
		// Returns the human-readable name of a MAC enumeration value according
		// to the 802.15.4 standard
		static const char *GetStatusString(const unsigned char nValue);

#ifdef _DEBUG
		// Dump a combination of MHR and MSDU, i.e. header and optionally data
		static void Dump(const CIEEE802154MHR &header, const CPacket &mpdu,
			const bool bWithData = false);

		// Dump a MPDU, i.e. header and optionally data
		static void Dump(const CPacket &mpdu, const bool bWithData = false);

		// Performs sanity checks on MAC layer data
		void AssertValid() const;

		// Dump relevant MAC layer data
		void Dump() const;
#endif

};

#ifdef __COMPACT15DOT4_FFD__
inline bool CIEEE802154MAC::IsBeaconing() const
	{ return m_pib.m_nBeaconOrder != 15; }
#else
inline bool CIEEE802154MAC::IsBeaconing() const
	{ return false; }
#endif

inline unsigned char CIEEE802154MAC::GetDefaultSourceAddressingMode() const
{
	return (m_pib.m_wShortAddress == CIEEE802154Address::wOnlyExtendedAddress) ?
		CIEEE802154Address::typeExtended : CIEEE802154Address::typeShort;
}

inline CIEEE802154Address CIEEE802154MAC::GetDefaultSourceAddress() const
	{ return GetSourceAddress(GetDefaultSourceAddressingMode()); }

inline CIEEE802154Address CIEEE802154MAC::GetExtendedSourceAddress() const
	{ return GetSourceAddress(CIEEE802154Address::typeExtended); }

inline CIEEE802154Address CIEEE802154MAC::GetBroadcastAddress(const bool bAllNetworks) const
{
	return CIEEE802154Address(CIEEE802154Address::wBroadcast, bAllNetworks ?
		CIEEE802154Address::wBroadcast : m_pib.m_wPANID);
}

inline bool CIEEE802154MAC::HasAssociated() const
	{ return m_pib.m_wShortAddress != CIEEE802154Address::wBroadcast; }

inline const CPacket *CIEEE802154MAC::GetDequeued() const
	{ return m_pDequeued; }
