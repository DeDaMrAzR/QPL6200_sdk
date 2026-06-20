// IEEE802154MACPIB.h : header file
//
// Copyright(C) 2008-2016 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact15.4(TM) IEEE 802.15.4 compliant medium access control
// layer implementation
//
// This implementation is based on the IEEE Standard for Information
// Technology - Telecommunications and information exchange between systems -
// Local and metropolitan area networks - specific requirements - Part 15.4:
// Wireless Medium Access Control (MAC) and Physical Layer (PHY)
// Specifications for Low-Rate Wireless Personal Area Networks (WPANs), as of
// September 8, 2006
//
// Contains the MAC protocol information base (PIB) in a specifically adapted
// version for the GreenPeak MAC

/////////////////////////////////////////////////////////////////////////////
// forward declarations

class CIEEE802154MAC;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MACStatistics

class CIEEE802154MACStatistics
{
	// Attributes
	public:
		// A counter that is incremented each time the MAC layer receives a
		// broadcast
		uint32_t m_nBroadcastsReceived;

		// A counter that is incremented each time the MAC layer transmits a
		// broadcast
		uint32_t m_nBroadcastsTransmitted;

		// A counter that is incremented each time the MAC layer receives a
		// unicast
		uint32_t m_nUnicastsReceived;

		// A counter that is incremented each time the MAC layer transmits a
		// unicast
		uint32_t m_nUnicastsTransmitted;

		// A counter that is incremented each time the MAC layer retries a
		// unicast
		uint16_t m_nUnicastsRetried;

		// A counter that is incremented each time the MAC layer fails to send
		// a unicast
		uint16_t m_nUnicastsFailed;

		// A counter that is incremented each time a packet is dropped because
		// the PHY to MAC queue was exhausted
		uint16_t m_nInboundQueueOverflows;

		// "A counter that is equal to the average number of MAC retries needed
		// to send an APS message.". However, this is infeasible, and we just
		// return the average number of MAC retries
		uint16_t m_nAverageRetries;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MACPIBBeaconPayloadAccessor

class CIEEE802154MACPIBBeaconPayloadAccessor
{
	// Construction
	public:
		CIEEE802154MACPIBBeaconPayloadAccessor(CIEEE802154MAC &mac);

	// Operations
	public:
		// The assignment operator allows setting a new value
		CIEEE802154MACPIBBeaconPayloadAccessor &operator=
			(const CPacket &payload);

		// The type conversion operator allows getting the current value
		operator CPacket() const;

	// Implementation
	public:
		CIEEE802154MAC &m_mac;

		// This holds a copy of the beacon payload
		CPacket m_payload;
};


/////////////////////////////////////////////////////////////////////////////
// forward references

class CIEEE802154MAC;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MACPIBAssociationPermitAccessor

class CIEEE802154MACPIBAssociationPermitAccessor
{
	// Construction
	public:
		CIEEE802154MACPIBAssociationPermitAccessor(CIEEE802154MAC &mac);

	// Operations
	public:
		// The assignment operator allows setting a new value
		CIEEE802154MACPIBAssociationPermitAccessor &operator=
			(const bool bEnable);

		// The type conversion operator allows getting the current value
		operator bool() const;

		// Implementation
		public:
			CIEEE802154MAC &m_mac;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MACPIBFullFunctionDeviceAccessor

class CIEEE802154MACPIBFullFunctionDeviceAccessor
{
	// Construction
	public:
		CIEEE802154MACPIBFullFunctionDeviceAccessor(CIEEE802154MAC &mac);

	// Operations
	public:
		// The assignment operator allows setting a new value
		CIEEE802154MACPIBFullFunctionDeviceAccessor &operator=
			(const bool bFullFunctionDevice);

		// The type conversion operator allows getting the current value
		operator bool() const;

		// Implementation
		public:
			CIEEE802154MAC &m_mac;
			bool m_bValue;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MACPIBDataSequenceAccessor

class CIEEE802154MACPIBDataSequenceAccessor
{
	// Construction
	public:
		CIEEE802154MACPIBDataSequenceAccessor(CIEEE802154MAC &mac);

	// Operations
	public:
		// The assignment operator allows setting a new value
		CIEEE802154MACPIBDataSequenceAccessor &operator=
			(const uint_fast8_t dataSequence);

		// Prefix increment operator - sets a new DSN
		CIEEE802154MACPIBDataSequenceAccessor &operator++();

		// Postfix increment operator - sets a new DSN
		CIEEE802154MACPIBDataSequenceAccessor operator++(int);

		// The type conversion operator allows getting the current value
		operator uint_fast8_t() const;

		// Implementation
		public:
			CIEEE802154MAC &m_mac;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MACPIB

class CIEEE802154MACPIB
{
	// Construction
	public:
		CIEEE802154MACPIB(CIEEE802154MAC &mac);

	// typedefs & enums
	public:
		// These are a number of flags which affect the receiver's on state. If
		// any of these bits is set, the receiver shall be on; if all are clear
		// it will be turned off
		enum { whenIdle = 0x01, whileScanning = 0x02, expectingData = 0x04 };

	// Attributes
	public:
		// The address (either short or extended) of the coordinator through
		// which the device is associated. This is an aggregation of MAC PIB
		// variables 0x4a and 0x4b
		CIEEE802154Address m_addressCoordinator;

		// The 16-bit PAN ID of the network on which the device is operating.
		// This is MAC PIB variable 0x50
		unsigned short m_wPANID;

		// The 16-bit short address that the device uses to communicate in the
		// PAN. This is MAC PIB variable 0x53
		unsigned short m_wShortAddress;

		// True, if this device is currently operating as the PAN coordinator.
		// This is a private extension the the PIB
		bool m_bPANCoordinator;

		// Battery life extension. This is MAC PIB variable 0x43
		bool m_bBatteryLifeExtension;

		// True, if this is a full-function device. Allows having a combined,
		// single binary image for full and reduced functions devices, where
		// the device role can be configured by software, instead of requiring
		// dedicated builds. Useful for ROM implementation of the MAC or ZigBee
		// adapters with host controlled role. Notice it is still possible to
		// create a version that does not support FFD functionality at all, by
		// not defining __COMPACT15DOT4_FFD__
		CIEEE802154MACPIBFullFunctionDeviceAccessor m_bFullFunctionDevice;

#ifdef __COMPACT15DOT4_FFD__
		// Indication of whether a coordinator is currently allowing
		// association. This is MAC PIB variable 0x41 (optional for RFDs)
		CIEEE802154MACPIBAssociationPermitAccessor m_bAssociationPermit;
#endif

#ifdef __COMPACT15DOT4_FFD__
		// The beacon payload, contents and length, in octets. This is an
		// aggregation of MAC PIB variables 0x45 and 0x46 (optional for RFDs)
		CIEEE802154MACPIBBeaconPayloadAccessor m_beaconPayload;

		// Specification of how often the coordinator transmits its beacon.
		// This is MAC PIB variable 0x47 (optional for RFDs)
		unsigned char m_nBeaconOrder;

		// The sequence number added to the transmitted beacon frame (BSN).
		// This is MAC PIB variable 0x49 (optional for RFDs)
		unsigned char m_nBeaconSequence;
#endif

		// The sequence number added to the transmitted data or MAC command
		// frame (DSN). This is MAC PIB variable 0x4c
		CIEEE802154MACPIBDataSequenceAccessor m_nDataSequence;

		// Promiscuous mode (receive all mode).
		// This is MAC PIB variable 0x51 (optional for RFDs)
		bool m_bPromiscuousMode;

		// Indication of whether the MAC sublayer is to enable its receiver
		// during idle periods when expecting data. For a beacon-enabled PAN,
		//	this attribute is relevant only during the CAP of the incoming
		// superframe. For a non-beacon-enabled PAN, this attribute is relevant
		//	at all times. The LSB represents MAC PIB variable 0x52, while bit #1
		// is an extension that allows devices with "RX-on-when-idle" = false
		// have their receiver enabled when expecting data during a scan or
		// when polling their coordinator, for example
		unsigned char m_bReceiverOn;

#ifdef __COMPACT15DOT4_FFD__
		// The length of the active portion of the outgoing superframe,
		// including the beacon frame
		// This is MAC PIB variable 0x54 (optional for RFDs)
		unsigned char m_nSuperframeOrder;
#endif

#ifdef __COMPACT15DOT4_FFD__
		// The maximum time (in unit periods) that a transaction is stored by
		// a coordinator and indicated in its beacon.
		// This is MAC PIB variable 0x55 (optional for RFDs)
		static const unsigned int nTransactionPersistenceTime;
#endif

		// Maximum number of of back-offs the CSMA/CA algorithm will attempt
		// before declaring a channel access failure.
		// This is MAC PIB variable 0x4e
		static const unsigned char nMaxBackoffs;

		// Minimum value of the back-off exponent in the CSMA/CA algorithm.
		// This is MAC PIB variable 0x4f
		static const unsigned char nMinBackoffExponent;

		// Maximum value of the back-off exponent in the CSMA/CA algorithm.
		// This is MAC PIB variable 0x57
		static const unsigned char nMaxBackoffExponent;

		// The maximum number of CAP symbols in a beacon-enabled PAN, or
		// symbols in a nonbeacon-enabled PAN, to wait either for a frame
		// intended as a response to a data request frame or for a broadcast
		// frame following a beacon with the Frame Pending subfield set to one
		// This is MAC PIB variable 0x58
		static const unsigned short nMaxFrameTotalWaitTime;

		// The maximum number of retries allowed after a transmission failure
		// This is MAC PIB variable 0x59
		static const unsigned char nMaxFrameRetries;

		// The maximum time, in multiples of aBaseSuperframeDuration, a device
		// shall wait for a response command frame to be available following a
		// request command frame.
		// This is MAC PIB variable 0x5a
		static const unsigned int nResponseWaitTime;

		// The minimum number of symbols forming a long interframe space (LIFS)
		// period. Dependent on currently selected PHY. This is MAC PIB
		// variable ?, read-only
		static const unsigned char nMinLIFSPeriod;

		// The minimum number of symbols forming a short interframe space
		// (SIFS) period. Dependent on currently selected PHY. This is MAC PIB
		// variable ?, read-only
		static const unsigned char nMinSIFSPeriod;

		// The maximum number of symbols to wait for an acknowledgment frame
		// to arrive following a transmitted data frame.
		// This is MAC PIB variable 0x40, read-only
		static const unsigned char nAcknowledgmentWaitDuration;

		// Indication of whether a device automatically sends a data request
		// command if its address is listed in the beacon frame. This attribute
		// also affects the generation of the MLME-BEACON-NOTIFY.indication
		// primitive. This is MAC PIB variable 0x42
		static const bool bAutoRequest;

	// Operations
	public:
		// Resets the MAC PIB to its default values
		void SetDefaults(CIEEE802154MAC &mac);

	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};
