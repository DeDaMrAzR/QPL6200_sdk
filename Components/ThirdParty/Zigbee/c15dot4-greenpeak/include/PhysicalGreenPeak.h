// PhysicalGreenPeak.h : header file
//
// Copyright(C) 2016-2017 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// IEEE 802.15.4 compliant physical layer interface. This implementation maps
// to the GP MAC Dispatcher API (glue logic between ubisys and GreenPeak)

/////////////////////////////////////////////////////////////////////////////
// CPhysicalGreenPeak

class CPhysicalGreenPeak : public CIEEE802154PHY, public CRadioGreenPeak
{
	// Construction
	public:
		CPhysicalGreenPeak();

	// Attributes
	public:
		// For GreenPeak chips, which support multiple concurrent stacks
		// operating on a single device, this identifies the stack
		uint8_t m_nStackID;

	// Overrides
	public:
		// Called once after startup to perform low-level initialization of the
		// PHY
		virtual void Initialize();

#ifdef __COMPACT15DOT4_HARDWARE_FCS__
		// Given the packet (as passed to the MAC layer by PD-DATA.indication),
		// this method returns true, if the frame check sequence is valid, i.e.
		// the received CRC matches the expected (computed) value. This is
		// actually a MAC feature (radio hardware support for MAC layer)
		virtual bool ValidateFrameCheckSequence(const CPacket &packet) const;

		// Given the packet, this method prepares the frame such that the frame
		// check sequence is correctly determined and updated/inserted by the
		// radio hardware
		virtual void AppendFrameCheckSequence(CPacket &packet) const;
#endif

#ifdef __COMPACT15DOT4_HARDWARE_ADDRESSING__
		// Provides a means for the MAC layer to convey local addressing
		// information (extended address, short address, PAN ID, device role)
		// to radios that include hardware support for address recognition
		virtual void SetupAddressRecognition(const unsigned short wPANID = 0xffff,
			const unsigned long long qwExtendedAddress = 0xffffffffffffffffLL,
			const unsigned short wShortAddress = 0xfffe,
			const bool bPANCoordinator = false,
			const bool bAcceptReserved = false, const bool bEnable = true);
#endif

#ifdef __COMPACT15DOT4_HARDWARE_ACKNOWLEDGE__
		// If the radio features hardware support for generation of
		// acknowledgement frames, this method can be used to construct and
		// transmit an ACK frame. The sequence number of the last received
		// frame is copied to the ACK frame. The argument influences the
		// "frame pending" subfield of the frame control field
		virtual bool TransmitAcknowledge(const bool bPending = false);
#endif

#ifdef __COMPACT15DOT4_HARDWARE_AUTO_ACKNOWLEDGE__
		// Provides control over automatic acknowledgments. Enables or disables
		// automatic acknowledgments, for example when performing clear channel
		// assessments or energy detection scans
		virtual void EnableAutoAcknowledge(const bool bEnable = true);

		// If the radio features hardware support for automatic generation of
		// acknowledgement frames, this method can be used to cancel a specific
		// acknowledgment frame transmission, for example if the MAC layer's
		// RX queue is at capacity
		virtual bool CancelAutoAcknowledge();

		// Notifies the hardware accelerator that all pending frames have been
		// purged from the indirect transaction queue
		virtual void ClearPendingAddressMap();

		// Notifies the hardware accelerator that there is a pending frame for
		// the device with the specified short address
		virtual bool SetPending(const unsigned short wPANID,
			const unsigned short wShort);

		// Notifies the hardware accelerator that there is a pending frame for
		// the device with the specified extended address
		virtual bool SetPending(const unsigned long long qwExtendedAddress);

		// Notifies the hardware accelerator that there is no longer a pending
		// frame for the device with the specified short address
		virtual bool ClearPending(const unsigned short wPANID,
			const unsigned short wShort);

		// Notifies the hardware accelerator that there is no longer a pending
		// frame for the device with the specified extended address
		virtual bool ClearPending(const unsigned long long qwExtendedAddress);

		// Returns true if the last acknowledgment frame has been sent with the
		// pending flag set (true) or cleared (false)
		virtual bool GetLastPendingIndication();
#endif

	// Operations
	public:

#ifdef __COMPACT15DOT4_HARDWARE_RETRANSMIT__
		// Must be used as first stage of a PD-DATA.request(psduLength, psdu)
		// service primitive, clause 6.2.1.1. Notice that psduLength and psdu
		// are encapsulated in the CPacket object. Implementations might modify
		// the packet, i.e. inserting a PHY header (frame length) in-place is
		// OK. Notice that the PSDU is only transferred to the radio, but
		// transmission does not start, until OnRequestDataEx() is invoked
		virtual void TransferData(CPacket &psdu, const bool bRetransmission);

		// Can be used to flush the transmit FIFO, i.e. to clear data that has
		// been put into the transmit FIFO by TransferData(), but shall not be
		// sent
		virtual void FlushData();

		// Second state of the PD-DATA.request. Intended for radios that
		// support retransmission of the last frame transfered to their TX
		// FIFO. If the radio supports hardware CCA, the paramter can be used
		// to invoke tranmission. Returns true, if transmission has started,
		// false otherwise
		virtual void OnRequestDataEx(const bool bWithCCA = false);
#else
		// PD-DATA.request(psduLength, psdu) service primitive, clause 6.2.1.1.
		// Notice that psduLength and psdu are encapsulated in the CPacket
		// object. Implementations might modify the packet, i.e. inserting a
		// PHY header (frame length) in-place is OK. Notice that the mating
		// PD-DATA.confirm() is issued either immediately (i.e. before the call
		// to OnRequestData() returns), or after transmission has completed,
		// i.e. after the call to OnRequestData() has returned)
		virtual void OnRequestData(CPacket &psdu);
#endif

#ifdef __COMPACT15DOT4_HARDWARE_RANDOM__
		// Returns a cryptographically strong random number
		virtual unsigned int GetRandomValue();
#endif

		// This version of the PD-DATA.request takes a complete PPDU including
		// PHR and transfers it to the underlying radio hardware. Could be used
		// for retransmissions, when the radio does not support retransmissions
		// using data stored in its transmission FIFO. If bTransmit is false,
		// transmission is not invoked, i.e. the PPDU is delivered to the radio
		// but no transmission occurs. In this case, call OnRequestDataEx() to
		// commence transmission
		virtual void OnRequestDataEx(const CPacket &ppdu,
			const bool bTransmit = true);

		// This is a direct call interface for a sequence of PLME-CCA.request()
		// service primitive resulting in an immediate PLME-CCA.confirm(status)
		// The return value is the status paramter of the confirm primitive
		virtual unsigned char PerformClearChannelAssessment();

		// This is the direct call version for a sequence of PLME-ED.request()
		// service primitive resulting in an immediate PLME-ED.confirm(status,
		// EnergyLevel). The return value is the status paramter of the confirm
		// primitive
		virtual unsigned char PerformEnergyDetection
			(unsigned char &nEnergyLevel);

		// Returns the raw RSSI and correlation values for a frame that was
		// received using this PHY. The packet object must have been provided
		// by the OnIndicate() method or must be an exact copy, i.e. made using
		// CPacket::CreateCopyEx()
		virtual void GetRSSIAndCorrelation(const CPacket &mpdu,
			signed char &nRSSI, unsigned char &nCorrelation);

		// PLME-SET-TRX-STATE.request(state) (clause 6.2.2.7)
		virtual void OnRequestSetTransceiverState(const unsigned char bState);

		// Direct call, specialized version of a sequence of
		// PLME-SET.request(PIBAttribute = 0x00, PIBAttributeValue),
		// clause 6.2.2.9 and PLME-SET.confirm(status, PIBAttribute = 0x00),
		// clause 6.2.2.10.
		virtual unsigned char SetCurrentChannel(const unsigned char nChannel);

		// Direct call, specialized version of a sequence of
		// PLME-SET.request(PIBAttribute = 0x04, PIBAttributeValue),
		// clause 6.2.2.9 and PLME-SET.confirm(status, PIBAttribute = 0x04),
		// clause 6.2.2.10.
		virtual unsigned char SetCurrentPage(const unsigned char nPage);

		// Sets transmit power, specified in dBm
		virtual void SetTransmitPowerEx(const signed char nPower);

	// CPhysicalGreenPeak-specific extension
	public:
		signed char GetTransmitPower();

	// Implementation
	public:
		virtual ~CPhysicalGreenPeak();

		// Instance identifier. This is used to assemble the stack identifier
		// string. The first instance will use c154 (ubisys Compact15.4), and
		// other instances (in separate processes) will use c155, c156 in such
		static uint8_t nInstanceID;

		// Computes the link quality assessment (LQA) value by combining the
		// LQI and RSSI values as reported by the radio
		static unsigned char CalculateLinkQuality
			(unsigned char nCorrelationValue, signed char nRSSI);

	public:
#ifdef _DEBUG
		// Dumps relevant information including the the current PHY state,
		// radio status and radio FSM state
		virtual void Dump() const;

		// Dumps Qorvo software version information
		static void DumpVersionInfo();
#endif

};
