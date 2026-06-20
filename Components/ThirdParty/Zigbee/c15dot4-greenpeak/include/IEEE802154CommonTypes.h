// IEEE802154CommonTypes.h : header file
//
// Copyright(C) 2008-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact15.4(TM) IEEE 802.15.4 compliant MAC and PHY layer HAL
// class library.
//
// This implementation is based on the IEEE Standard for Information
// Technology - Telecommunications and information exchange between systems -
// Local and metropolitan area networks - specific requirements - Part 15.4:
// Wireless Medium Access Control (MAC) and Physical Layer (PHY)
// Specifications for Low-Rate Wireless Personal Area Networks (WPANs), as of
// September 8, 2006
//
// Definition of common types used throughout the class library. Especially,
// types that are used in the MAC layer, for example addresses, frame
// specifications, etc.

/////////////////////////////////////////////////////////////////////////////
// CIEEE802154Sequence

template<class unsigned_t, class signed_t>
class CIEEE802154Sequence
{
	// Construction
	public:
		CIEEE802154Sequence(const unsigned_t value);

	// Attributes
	public:
		unsigned_t m_value;

	// Operations
	public:
		bool operator<(const CIEEE802154Sequence &s) const;
		CIEEE802154Sequence &operator=(const CIEEE802154Sequence &s);
		operator unsigned_t() const;
};

template<class unsigned_t, class signed_t>
inline CIEEE802154Sequence<unsigned_t, signed_t>::
CIEEE802154Sequence(const unsigned_t value) :
m_value(value)
{
}


template<class unsigned_t, class signed_t>
inline bool CIEEE802154Sequence<unsigned_t, signed_t>::
operator<(const CIEEE802154Sequence &s) const
{
	return (static_cast<signed_t>(m_value) -
		static_cast<signed_t>(s.m_value)) < 0;
}


template<class unsigned_t, class signed_t>
inline CIEEE802154Sequence<unsigned_t, signed_t> &
CIEEE802154Sequence<unsigned_t, signed_t>::
operator=(const CIEEE802154Sequence &s)
{
	m_value = s.m_value;
	return *this;
}


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154Address

class CIEEE802154Address
{
	// typedefs & enums
	public:
		// Common addressing modes. We have a special value of 6, which
		// translates to a short/network address when used with GetType(),
		// see below. Notice that GetType() == typeBoth will never return true!
		// Always use IsShortValid()/IsExtendedValid(). The 'alternate' type
		// denotes an alternate addressing scheme, e.g. for a virtual device
		// that is not connected via a real IEEE 802.15.4 PHY but is bridged
		// using some other transport. In that case all addressing material
		// might be used for different, potentially context-specific purposes
		enum { typeNone = 0, typeReserved = 1, typeShort = 2, typeExtended = 3,
			typeMask = 0x03, typeAlternate = 4, typeBoth = 6,
			typePANIDValid = 0x40, typeIgnoreMask = 0xf8 };

	// Construction
	public:
		explicit CIEEE802154Address(const uint8_t nType = typeNone);

		explicit CIEEE802154Address(const uint64_t qwAddress,
			const uint_fast16_t wPANID = UINT16_MAX);

		explicit CIEEE802154Address(const uint16_t wAddress,
			const uint_fast16_t wPANID = UINT16_MAX);

		CIEEE802154Address(const uint_fast64_t qwExtended,
			const uint_fast16_t wShort, const uint_fast16_t wPANID);

	// Attributes
	public:
		// 64-bit extended address (valid in extended addressing mode)
		uint64_t m_qwExtended;

		// 16-bit PAN identifier (valid in short and extended addressing
		// modes)
		uint16_t m_wPANID;

		// 16-bit short address (valid in short addressing mode)
		uint16_t m_wShort;

		// Returns the address type (none, short, extended, or any value
		// supported in derived classes (such as ZigBee group address).
		// However, the return value is limited to two bits (as defined by the
		// standard). An address object with all addresses valid appears as a
		// short address, when GetType() is used. Use the IsXyzValid() members
		// to check for a specific address type, if necessary
		uint_fast8_t GetType() const;

		// Returns the address type (none, short, extended, or any value
		// supported in derived classes (such as ZigBee group address).
		// Not limited to the type mask, i.e. two least significant bits. This
		// method is meant for copying address types, for example. Do not use
		// for typical operations, i.e. to check whether the short or extended
		// address is valid
		uint_fast8_t GetTypeEx() const;

		// Sets the address type (none, short, extended, or any value
		// supported in derived classes (such as ZigBee group address)
		void SetType(const uint_fast8_t nType);

		// Returns true, if PANID is valid
		bool IsPANIDValid() const;

		// Returns true, if 64-bit extended IEEE address is valid
		bool IsExtendedValid() const;

		// Returns true, if 16-bit short address is valid
		bool IsShortValid() const;

		// Returns ture, if this is an 'alternate' address
		bool IsAlternate() const;

	// Constants
	public:
		// Broadcast address of "all ones", for short addresses and PANIDs
		static const uint16_t wBroadcast = UINT16_MAX;

		// Value indicating that the device does not have a short address and
		// uses its 64-bit extended address for all communications on the PAN
		static const uint16_t wOnlyExtendedAddress = 0xfffe;

		// This is a void, i.e. invalid/unspecified/unknown extended address
		static const uint64_t qwExtendedVoid = UINT64_MAX;

	// Operations
	public:
		// Checks if this instance matches the specified object
		bool operator==(const CIEEE802154Address &address) const;

		// Checks if this instance matches the specfied extended address
		bool operator==(const uint64_t qwExtended) const;

		// Checks if this instance matches the specfied short address
		bool operator==(const uint16_t wShort) const;

		// Checks if addresses are different
		bool operator!=(const CIEEE802154Address &address) const;

		// Sets the PANID of the address object
		CIEEE802154Address &AssignPANID(const uint_fast16_t wPANID);

		// Invalidates the extended address portion of an address object, for
		// which both short and extended address are indicated as valid
		CIEEE802154Address &InvalidateExtended();

		// Invalidates the short address portion of an address object, for
		// which both short and extended address are indicated as valid
		CIEEE802154Address &InvalidateShort();

		// Validates the short and extended address portions of the address
		CIEEE802154Address &MakeBothValid();

	// Implementation
	protected:
		// Addressing mode. Identifies the addressing mode (short, extended, or
		// none), and thereby determines which of the other members are valid
		uint8_t m_nType;

	public:
		// Returns a human-readable string representation of all addressing
		// material contained in this address object
		std::string MakeString(const bool bWithPANID = true) const;

		// Converts an address to a human readable string
		operator std::string() const;
};

inline CIEEE802154Address::CIEEE802154Address(const uint8_t nType) :
m_nType(nType)
{
}

inline CIEEE802154Address::CIEEE802154Address(const uint64_t qwAddress,
	const uint_fast16_t wPANID) :
m_qwExtended(qwAddress), m_wPANID(wPANID), m_wShort(wOnlyExtendedAddress),
m_nType(typeExtended | typePANIDValid)
{
	ASSERT(qwAddress != qwExtendedVoid);
}

inline CIEEE802154Address::CIEEE802154Address(const uint16_t wAddress,
	const uint_fast16_t wPANID) :
m_wPANID(wPANID), m_wShort(wAddress), m_nType(typeShort | typePANIDValid)
{
}

inline CIEEE802154Address::CIEEE802154Address(const uint_fast64_t qwExtended,
	const uint_fast16_t wShort, const uint_fast16_t wPANID) :
m_qwExtended(qwExtended), m_wPANID(wPANID), m_wShort(wShort),
m_nType((wShort == wOnlyExtendedAddress) ?
	(typeExtended | typePANIDValid) :
	(typeBoth | typePANIDValid))
{
	if (qwExtended == qwExtendedVoid)
	{
		m_nType = (wShort == wOnlyExtendedAddress) ?
			typeNone :
			typeShort | typePANIDValid;
	}
}

inline uint_fast8_t CIEEE802154Address::GetTypeEx() const
	{ return m_nType; }

inline uint_fast8_t CIEEE802154Address::GetType() const
	{ return m_nType & typeMask; }

inline void CIEEE802154Address::SetType(const uint_fast8_t nType)
	{ m_nType = nType; }

inline bool CIEEE802154Address::IsPANIDValid() const
	{ return (m_nType & typePANIDValid) ? true : false; }

inline bool CIEEE802154Address::IsExtendedValid() const
{
	return ((m_nType & ~typeIgnoreMask) == typeExtended) ||
		((m_nType & ~typeIgnoreMask) == typeBoth);
}

inline bool CIEEE802154Address::IsShortValid() const
{
	return ((m_nType & ~typeIgnoreMask) == typeShort) ||
		((m_nType & ~typeIgnoreMask) == typeBoth);
}

inline bool CIEEE802154Address::IsAlternate() const
{
	return (m_nType & ~typeIgnoreMask) == typeAlternate;
}

inline bool CIEEE802154Address::operator==(const uint64_t qwExtended) const
	{ return IsExtendedValid() && (m_qwExtended == qwExtended); }

inline bool CIEEE802154Address::operator==(const uint16_t wShort) const
	{ return IsShortValid() && (m_wShort == wShort); }

inline bool CIEEE802154Address::operator!=(const CIEEE802154Address &address) const
	{ return !(*this == address); }

inline CIEEE802154Address &
CIEEE802154Address::AssignPANID(const uint_fast16_t wPANID)
{
	m_wPANID = wPANID;
	m_nType |= typePANIDValid;

	return *this;
}

inline CIEEE802154Address &CIEEE802154Address::InvalidateExtended()
{
	uint_fast8_t type = m_nType & typeIgnoreMask;

	if (IsShortValid())
		type |= typeShort;

	m_nType = type;

	return *this;
}

inline CIEEE802154Address &CIEEE802154Address::InvalidateShort()
{
	uint_fast8_t type = m_nType & typeIgnoreMask;

	if (IsExtendedValid())
		type |= typeExtended;

	m_nType = type;

	return *this;
}


inline CIEEE802154Address &CIEEE802154Address::MakeBothValid()
{
	// Make both short and extended address valid, except when the type is
	// 'alternate' (then preserve the 'alternate' type)
	if (!IsAlternate())
		m_nType = (m_nType & typeIgnoreMask) | typeBoth;

	return *this;
}


#ifdef _DEBUG

inline CIEEE802154Address::operator std::string() const
	{ return MakeString(); }

#endif


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154Superframe

class CIEEE802154Superframe
{
	// typedefs & enums
	public:
		// Format of the superframe specification field
		enum { maskBeaconOrder = 0x000f, maskSuperframeOrder = 0x00f0,
			maskFinalCAPSlot = 0x0f00, batteryLifeExtension = 0x1000,
			isPANCoordinator = 0x4000, associationPermit = 0x8000 };

		// Superframe specification field bit positions
		enum { shiftBeaconOrder = 0, shiftSuperframeOrder = 4,
			shiftFinalCAPSlot = 8 };

	// Construction
	public:

	// Attributes
	public:
		// The superframe specification, 16-bit
		uint16_t m_wSuperframeSpecification;

		// Returns the beacon order
		uint_fast8_t GetBeaconOrder() const;

		// Returns the superframe order
		uint_fast8_t GetSuperframeOrder() const;

		// Returns the final CAP slot
		uint_fast8_t GetFinalCAPSlot() const;

	// Operations
	public:
//#ifdef _DEBUG
		// Creates trace output for a superframe specification
		void Dump() const;
//#endif
};

inline uint_fast8_t CIEEE802154Superframe::GetBeaconOrder() const
	{ return (m_wSuperframeSpecification & maskBeaconOrder); }

inline uint_fast8_t CIEEE802154Superframe::GetSuperframeOrder() const
{
	return (m_wSuperframeSpecification & maskSuperframeOrder) >>
		shiftSuperframeOrder;
}

inline uint_fast8_t CIEEE802154Superframe::GetFinalCAPSlot() const
{
	return (m_wSuperframeSpecification & maskFinalCAPSlot) >>
		shiftFinalCAPSlot;
}


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154PANDescriptor

class CIEEE802154PANDescriptor
{
	// Attributes
	public:
		// PAN identifier and address of the coordinator as specified in the
		// received beacon frame
		CIEEE802154Address m_addressCoordinator;

		// The current logical channel occupied by the network
		uint8_t m_nLogicalChannel;

		// The current channel page occupied by the network
		uint8_t m_nChannelPage;

		// The superframe specification as specified in the received beacon
		// frame
		CIEEE802154Superframe m_superframe;

		// True if the beacon is from the PAN coordinator that is accepting
		// GTS requests
		bool m_bPermitGTS;

		// The LQI at which the network beacon was received
		uint8_t m_nLinkQuality;

		// The time at which the beacon frame was received (in symbols)
		uint32_t m_nTimestamp;

		// SUCCESS if there was no error in the security processing of the
		// frame. One of the other status codes indicating an error in the
		// security processing otherwise
		uint8_t m_nSecurityFailure;

		// The security level, key ID mode, key source and key index
		// CIEEE802154SecurityInfo m_security;

	// Operations
	public:

//#ifdef _DEBUG
		// Creates trace output for a PAN descriptor
		void Dump() const;
//#endif

};


/////////////////////////////////////////////////////////////////////////////
// byte packing for all frame definitions

#ifndef __GNUC__
#pragma pack(push, 1)
#define __GNUC_PACKING__
#else
#define __GNUC_PACKING__ __attribute__ ((packed))
#endif


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154PHR

class CIEEE802154PHR
{
	// typedefs & enums
	public:
		// PHY header frame overhead (PHR). May be used to pre-allocate memory
		// for packet headers
		enum { frameOverhead = 1 };

	// Attributes
	public:
		// PHY header contains eight-bit packet length
		uint8_t m_nLength;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MHR

class CIEEE802154MHR
{
	// typedefs & enums
	public:
		// Frame control field bit masks
		enum { maskType = 0x0007, securityEnabled = 0x0008,
			framePending = 0x0010, acknowledgeRequest = 0x0020,
			compressedPANID = 0x0040, maskDestinationAddressingMode = 0x0c00,
			maskVersion = 0x3000, maskSourceAddressingMode = 0xc000 };

		// Frame control field bit positions
		enum { shiftDestinationAddressingMode = 10, shiftVersion = 12,
			shiftSourceAddressingMode = 14 };

		// Frame control field, frame type subfield (clause 7.2.1.1.1)
		enum { typeBeacon = 0, typeData = 1, typeAcknowledgment = 2,
			typeCommand = 3 };

		// Frame control field, possible values of destination addressing mode
		// and source addressing mode subfields (7.2.1.1.6, 7.2.1.1.8)
		enum { addressingNone = CIEEE802154Address::typeNone,
			addressingShort = CIEEE802154Address::typeShort,
			addressingExtended = CIEEE802154Address::typeExtended };

		// MAC header frame overhead. May be used to pre-allocate memory for
		// packet headers
		enum { frameOverhead = 37 };

	// Attributes
	public:
		// Frame control field, clause 7.2.1.1
		uint16_t m_wFrameControl;

		// Sequence number
		uint8_t m_bSequence;

		// Variable-length addressing fields follow, depending on the source
		// and destination addressing modes, as well as the PAN ID compression
		// flag. A variable-length auxiliary security header might follow, too

	// Operations
	public:
		// Returns the source addressing mode
		uint_fast8_t GetSourceAddressingMode() const;

		// Returns the destination addressing mode
		uint_fast8_t GetDestinationAddressingMode() const;

		// Returns the frame version
		uint_fast8_t GetVersion() const;

		// Returns the frame type
		uint_fast8_t GetType() const;

		// Calculates the header length based on frame control field settings
		uint_fast8_t GetSize() const;

		// Calculates the size of destination addressing material. Takes into
		// account the PAN ID, short/long addresses, and whether addresses are
		// present at all
		uint_fast8_t GetDestinationAddressingMaterialSize() const;

		// Calculates the size of source addressing material. Takes into
		// account the PAN ID, short/long addresses, and whether addresses are
		// present at all. PANID compression is also taken into account
		uint_fast8_t GetSourceAddressingMaterialSize() const;

		// Performs some sanity checking on the header fields. Useful not only
		// in debug builds, but also to filter out invalid frames
		bool IsValid() const;

		// Address retrieval. Caution: Accesses memory beyond declared class
		// members (attributes), according to frame control field settings.
		// This version extracts short address information and returns false
		// if such information is not available
		bool GetSourceAddress(uint16_t &wPANID, uint16_t &wShortAddress) const;

		// Address retrieval. Caution: Accesses memory beyond declared class
		// members (attributes), according to frame control field settings.
		// This version extracts extended address information and returns false
		// if such information is not available
		bool GetSourceAddress(uint16_t &wPANID,
			uint64_t &qwExtendedAddress) const;

		// Address retrieval. Caution: Accesses memory beyond declared class
		// members (attributes), according to frame control field settings.
		// This version extracts arbitrary address information and returns it
		// as a CIEEE802154Address object
		CIEEE802154Address GetSourceAddress() const;

		// Address retrieval. Caution: Accesses memory beyond declared class
		// members (attributes), according to frame control field settings.
		// This version extracts short address information and returns false
		// if such information is not available
		bool GetDestinationAddress(uint16_t &wPANID,
			uint16_t &wShortAddress) const;

		// Address retrieval. Caution: Accesses memory beyond declared class
		// members (attributes), according to frame control field settings.
		// This version extracts extended address information and returns false
		// if such information is not available
		bool GetDestinationAddress(uint16_t &wPANID,
			uint64_t &qwExtendedAddress) const;

		// Address retrieval. Caution: Accesses memory beyond declared class
		// members (attributes), according to frame control field settings.
		// This version extracts arbitrary address information and returns it
		// as a CIEEE802154Address object
		CIEEE802154Address GetDestinationAddress() const;

//#ifdef _DEBUG
		// Dumps the fields of a MAC frame header
		void Dump() const;
//#endif

} __GNUC_PACKING__;


inline uint_fast8_t CIEEE802154MHR::GetSourceAddressingMode() const
	{ return (m_wFrameControl & maskSourceAddressingMode) >> shiftSourceAddressingMode; }

inline uint_fast8_t CIEEE802154MHR::GetDestinationAddressingMode() const
	{ return (m_wFrameControl & maskDestinationAddressingMode) >> shiftDestinationAddressingMode; }

inline uint_fast8_t CIEEE802154MHR::GetVersion() const
	{ return (m_wFrameControl & maskVersion) >> shiftVersion; }

inline uint_fast8_t CIEEE802154MHR::GetType() const
	{ return (m_wFrameControl & maskType); }


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MFR

class CIEEE802154MFR
{
	// typedefs & enums
	public:
		// MAC footer frame overhead. May be used to pre-allocate memory for
		// packet trailers
		enum { frameOverhead = 2 };

	// Attributes
	public:
		// Frame control field, clause 7.2.1.9
		uint16_t m_wFrameCheckSequence;

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// restore previous packing

#ifndef __GNUC__
#pragma pack(pop)
#endif


/////////////////////////////////////////////////////////////////////////////
// L2 frame overheads (maximum for header, exact for trailer)

enum { headerOverheadL2 =
	static_cast<uint_fast8_t>(CIEEE802154PHR::frameOverhead) +
	static_cast<uint_fast8_t>(CIEEE802154MHR::frameOverhead) };

enum { trailerOverheadL2 =
	static_cast<uint_fast8_t>(CIEEE802154MFR::frameOverhead) };
