// IEEE802154PHY.h : header file
//
// Copyright(c) 2008-2024 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// IEEE 802.15.4 compliant physical layer interface. Abstract base class that
// is used by the Compact15.4 IEEE 802.15.4 MAC layer implementation. The
// base class is used by physical layer convergence protocol sublayers, e.g.
// for the CC2420 radio.
//
// Most 802.15.4 solutions provide some MAC layer hardware support as well.
// This is also covered in a generic, extensible fashion.
//
// This implementation is based on the IEEE Standard for Information
// Technology - Telecommunications and information exchange between systems -
// Local and metropolitan area networks - specific requirements - Part 15.4:
// Wireless Medium Access Control (MAC) and Physical Layer (PHY)
// Specifications for Low-Rate Wireless Personal Area Networks (WPANs), as of
// September 8, 2006

/////////////////////////////////////////////////////////////////////////////
// CIEEE802154PDSAPClient

class CIEEE802154PDSAPClient
{
	// Overridables
	public:
		// Implementation specific virtual overridable. Allows very early
		// MAC-level pre-processing of PHY frames, as soon as data begins to
		// arrive at the PHY. Can be used for time-critical tasks, such as
		// acknowledgment generation, for instance. Might be called several
		// times for the same frame during reception, until the frame is
		// complete (in which case cbRemaining is zero)
		virtual bool PreprocessData(const CPacket &psdu,
			const CPacketData::size_type bFlagsAndRemaining);

		// PD-DATA.confirm(status) service primitive, clause 6.2.1.2
		virtual void OnConfirmData(const unsigned char nStatus,
			const bool bAcknowledgment) = 0;

		// PD-DATA.confirm(status) service primitive, clause 6.2.1.2
		virtual void OnConfirmData(const unsigned char nStatus) = 0;

		// PD-DATA.indication(psduLength, psdu, ppduLinkQuality) service
		// primitive, clause 6.2.1.3. Notice that psduLength and psdu are
		// encapsulated in the packet object
		virtual void OnIndicateData(CPacket &psdu,
			const unsigned char nLinkQuality) = 0;

		// PLME-SET-TRX-STATE.confirm(status), clause 6.2.2.8
		virtual void OnConfirmSetTransceiverState(const unsigned char nStatus) = 0;

	// Implementation
	public:
		virtual ~CIEEE802154PDSAPClient();
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154PHYPIB

class CIEEE802154PHYPIB
{
	// Construction
	public:
		CIEEE802154PHYPIB();

	// Attributes
	public:
		// The RF channel to use for all following transmissions and receptions
		// This is PHY PIB variable 0x00
		unsigned char m_nCurrentChannel;

		// This is the current PHY channel page. This is used in conjunction
		// with phyCurrentChannel to uniquely identify the channel currently
		// being used. This is PHY PIB variable 0x04
		unsigned char m_nCurrentPage;

		// The maximum number of symbols in a frame
		// This is PHY PIB variable 0x05, read-only
		const unsigned short m_nMaxFrameDuration;

		// The duration of the synchronization header (SHR) in symbols for the
		// current PHY. This is PHY PIB variable 0x06, read-only
		const unsigned char m_nSynchronizationHeaderDuration;

		// The number of symbols per octet for the current PHY
		// This is PHY PIB variable 0x07, read-only
		const unsigned char m_nSymbolsPerOctet;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154PHY

class CIEEE802154PHY
{
	// typedefs & enums
	public:
		// PHY constants according to clause 6.4.1
		enum { maxPacketSize = 127, turnaroundTime = 12, ccaTime = 8 };

		// Transceiver states. These correspond to the PHY enumerations stated
		// in clause 6.2.3, however with a slightly different notation. The
		// values map to (TRX_OFF, RX_ON, TX_ON, BUSY_TX, BUSY_RX,
		// FORCE_TRX_OFF), in that order
		enum { transceiverOff = 8, transceiverReceiverEnabled = 6,
			transceiverTransmitterEnabled = 9, transceiverTransmitting = 2,
			transceiverReceiving = 1, transceiverForceOff = 3 };

		// CCA result codes. These correspond to the PHY enumerations stated
		// in clause 6.2.3, however with a slightly different notation. The
		// values map to (TRX_OFF, RX_ON, TX_ON, BUSY_TX, BUSY_RX, BUSY, IDLE),
		// in that order
		enum { statusTransceiverOff = 8, statusReceiverEnabled = 6,
			statusTransmitterEnabled = 9, statusTransmitting = 2,
			statusReceiving = 1, statusSuccess = 7, statusChannelBusy = 0,
			statusChannelIdle = 4 };

		// Other status codes. These correspond to the PHY enumerations stated
		// in clause 6.2.3, too
		enum { statusInvalidParameter = 5 };

		// This must be the same value as CRadio...::headerCapacity for radios
		// that will be used via this PHY interface
		enum { radioHeaderCapacity = 1 };

		// Preprocess flags. The MSB indicates if this is a new frame, e.g.
		// PreprocessData() is called for the first time for a particular frame
		// or if it is call for a second, third or consecutive time... The LSBs
		// indicate the number of bytes remaining to be transferred
		enum { preprocessNewFrame = 0x80, preprocessRemainingMask = 0x7f };

	// Construction
	public:
		CIEEE802154PHY();

	// Attributes
	public:
		// PD-SAP client (usually the MAC layer)
		CIEEE802154PDSAPClient *m_pClient;

		// PHY PIB
		CIEEE802154PHYPIB m_pib;

	// Operations
	public:
		// Called once after startup to perform low-level initialization of the
		// PHY
		virtual void Initialize() = 0;

#ifdef __COMPACT15DOT4_HARDWARE_FCS__
		// Given the packet (as passed to the MAC layer by PD-DATA.indication),
		// this method returns true, if the frame check sequence is valid, i.e.
		// the received CRC matches the expected (computed) value. This is
		// actually a MAC feature (radio hardware support for MAC layer)
		virtual bool ValidateFrameCheckSequence(const CPacket &packet) const = 0;

		// Given the packet, this method prepares the frame such that the frame
		// check sequence is correctly determined and updated/inserted by the
		// radio hardware
		virtual void AppendFrameCheckSequence(CPacket &packet) const = 0;
#endif

#ifdef __COMPACT15DOT4_HARDWARE_ADDRESSING__
		// Provides a means for the MAC layer to convey local addressing
		// information (extended address, short address, PAN ID, device role)
		// to radios that include hardware support for address recognition
		virtual void SetupAddressRecognition(const unsigned short wPANID = 0xffff,
			const unsigned long long qwExtendedAddress = 0xffffffffffffffffLL,
			const unsigned short wShortAddress = 0xfffe,
			const bool bPANCoordinator = false,
			const bool bAcceptReserved = false,	const bool bEnable = true) = 0;
#endif

#ifdef __COMPACT15DOT4_HARDWARE_ACKNOWLEDGE__
		// If the radio features hardware support for generation of
		// acknowledgement frames, this method can be used to construct and
		// transmit an ACK frame. The sequence number of the last received
		// frame is copied to the ACK frame. The argument influences the
		// "frame pending" subfield of the frame control field
		virtual bool TransmitAcknowledge(const bool bPending = false) = 0;
#endif

#ifdef __COMPACT15DOT4_HARDWARE_AUTO_ACKNOWLEDGE__
		// Provides control over automatic acknowledgments. Enables or disables
		// automatic acknowledgments, for example when performing clear channel
		// assessments or energy detection scans
		virtual void EnableAutoAcknowledge(const bool bEnable = true) = 0;

		// If the radio features hardware support for automatic generation of
		// acknowledgement frames, this method can be used to cancel a specific
		// acknowledgment frame transmission, for example if the MAC layer's
		// RX queue is at capacity
		virtual bool CancelAutoAcknowledge() = 0;

		// Notifies the hardware accelerator that all pending frames have been
		// purged from the indirect transaction queue
		virtual void ClearPendingAddressMap() = 0;

		// Notifies the hardware accelerator that there is a pending frame for
		// the device with the specified short address
		virtual bool SetPending(const unsigned short wPANID,
			const unsigned short wShort) = 0;

		// Notifies the hardware accelerator that there is a pending frame for
		// the device with the specified extended address
		virtual bool SetPending(const unsigned long long qwExtendedAddress) = 0;

		// Notifies the hardware accelerator that there is no longer a pending
		// frame for the device with the specified short address
		virtual bool ClearPending(const unsigned short wPANID,
			const unsigned short wShort) = 0;

		// Notifies the hardware accelerator that there is no longer a pending
		// frame for the device with the specified extended address
		virtual bool ClearPending(const unsigned long long qwExtendedAddress) = 0;

		// Returns true if the last acknowledgment frame has been sent with the
		// pending flag set (true) or cleared (false)
		virtual bool GetLastPendingIndication() = 0;
#endif

#ifdef __COMPACT15DOT4_HARDWARE_RETRANSMIT__
		// Must be used as first stage of a PD-DATA.request(psduLength, psdu)
		// service primitive, clause 6.2.1.1. Notice that psduLength and psdu
		// are encapsulated in the CPacket object. Implementations might modify
		// the packet, i.e. inserting a PHY header (frame length) in-place is
		// OK. Notice that the PSDU is only transferred to the radio, but
		// transmission does not start, until OnRequestDataEx() is invoked. If
		// this is a retransmission, the PPDU has already been assembled and
		// should not be modified further
		virtual void TransferData(CPacket &psdu,
			const bool bRetransmission) = 0;

		// Can be used to flush the transmit FIFO, i.e. to clear data that has
		// been put into the transmit FIFO by TransferData(), but shall not be
		// sent
		virtual void FlushData() = 0;

		// Second state of the PD-DATA.request. Intended for radios that
		// support retransmission of the last frame transfered to their TX
		// FIFO. If the radio supports hardware CCA, the parameter can be used
		// enable/disable CCA for this particular tranmission. Returns true,
		//	if transmission has started, false otherwise
		virtual void OnRequestDataEx(const bool bWithCCA = false) = 0;
#else
		// PD-DATA.request(psduLength, psdu) service primitive, clause 6.2.1.1.
		// Notice that psduLength and psdu are encapsulated in the CPacket
		// object. Implementations might modify the packet, i.e. inserting a
		// PHY header (frame length) in-place is OK. Notice that the mating
		// PD-DATA.confirm() is issued either immediately (i.e. before the call
		// to OnRequestData() returns), or after transmission has completed,
		// i.e. after the call to OnRequestData() has returned)
		virtual void OnRequestData(CPacket &psdu) = 0;
#endif

#ifdef __COMPACT15DOT4_HARDWARE_RANDOM__
		// Returns a cryptographically strong random number
		virtual unsigned int GetRandomValue() = 0;
#endif

		// This version of the PD-DATA.request takes a complete PPDU including
		// PHR and transfers it to the underlying radio hardware. Could be used
		// for retransmissions, when the radio does not support retransmissions
		// using data stored in its transmission FIFO. If bTransmit is false,
		// transmission is not invoked, i.e. the PPDU is delivered to the radio
		// but no transmission occurs. In this case, call OnRequestDataEx() to
		// commence transmission
		virtual void OnRequestDataEx(const CPacket &ppdu,
			const bool bTransmit = true) = 0;

		// This is the direct call version for a sequence of PLME-CCA.request()
		// service primitive (clause 6.2.2.1) resulting in an immediate
		// PLME-CCA.confirm(status) (clause 6.2.2.2). bStatus is the status
		// value returned by PLME-CCA.confirm
		virtual unsigned char PerformClearChannelAssessment() = 0;

		// This is the direct call version for a sequence of PLME-ED.request()
		// service primitive (clause 6.2.2.3) resulting in an immediate
		// PLME-ED.confirm(status, EnergyLevel) (clause 6.2.2.4)
		virtual unsigned char
			PerformEnergyDetection(unsigned char &nEnergyLevel) = 0;

		// Returns the raw RSSI and correlation values for a frame that was
		// received using this PHY. The packet object must have been provided
		// by the OnIndicate() method or must be an exact copy, i.e. made using
		//	CPacket::CreateCopyEx()
		virtual void GetRSSIAndCorrelation(const CPacket &mpdu,
			signed char &nRSSI, unsigned char &nCorrelation) = 0;

		// PLME-SET-TRX-STATE.request(state) (clause 6.2.2.7)
		virtual void OnRequestSetTransceiverState(const unsigned char bState) = 0;

		// Direct call, specialized version of a sequence of
		// PLME-SET.request(PIBAttribute = 0x00, PIBAttributeValue),
		// clause 6.2.2.9 and PLME-SET.confirm(status, PIBAttribute = 0x00),
		// clause 6.2.2.10.
		virtual unsigned char SetCurrentChannel(const unsigned char nChannel) = 0;

		// Direct call, specialized version of a sequence of
		// PLME-SET.request(PIBAttribute = 0x04, PIBAttributeValue),
		// clause 6.2.2.9 and PLME-SET.confirm(status, PIBAttribute = 0x04),
		// clause 6.2.2.10.
		virtual unsigned char SetCurrentPage(const unsigned char nPage) = 0;

		// Sets transmit power, specified in dBm
		virtual void SetTransmitPowerEx(const signed char nPower) = 0;

		// Registers the PD-SAP client. The client's virtual functions will be
		// called whenever a confirm or indication primitive needs to be
		// processed
		void Register(CIEEE802154PDSAPClient &client);

	// Implementation
	public:
		virtual ~CIEEE802154PHY();

		// Lists the supported channels. Currently, this limits the use to a
		// single band per stack instance. If support for R22 multi-band is
		// required, this needs to be per MAC/PHY instance
		static const unsigned int nSupportedChannels;

		// Time when the last frame start has been detected (both, transmit
		// and receive)
		unsigned int m_nFrameStartTime;

		// Time when the last frame end has been detected (both, transmit
		// and receive)
		unsigned int m_nFrameEndTime;

		// Timestamp of the last sent/received frame
		unsigned int m_nTimestamp;

		// Current perception of transceiver state (either TRX_OFF, TX_ON,
		// RX_ON, BUSY_TX, or BUSY_RX). Notice that this state is synchronized
		// with the actual CC2520 radio state and synchronization might be lost
		// during periods when interrupts are disabled. Notice that the only
		// stable state is TRX_OFF. Other states are transitional and volatile
		unsigned char m_nState;

#ifdef _DEBUG
	public:
		// Returns the human-readable name of a PHY enumeration value according
		// to the 802.15.4 standard
		static const char *GetStatusString(const unsigned char nValue);

		// This debug-only method dumps the current state
		virtual void Dump() const;
#endif

};
