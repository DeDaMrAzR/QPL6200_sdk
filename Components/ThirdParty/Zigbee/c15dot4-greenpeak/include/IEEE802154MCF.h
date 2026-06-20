// IEEE802154MCF.h : header file
//
// Copyright(c) 2008 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact15.4(TM) IEEE 802.15.4 compliant medium access control
// layer implementation. Definition of the MAC frame header (MHR)
//
// This implementation is based on the IEEE Standard for Information
// Technology - Telecommunications and information exchange between systems -
// Local and metropolitan area networks - specific requirements - Part 15.4:
// Wireless Medium Access Control (MAC) and Physical Layer (PHY)
// Specifications for Low-Rate Wireless Personal Area Networks (WPANs), as of
// September 8, 2006
//
// Collection of MAC command frames (payload definitions)

/////////////////////////////////////////////////////////////////////////////
// byte packing for all packet definitions

#ifndef __GNUC__
#pragma pack(push, 1)
#define __GNUC_PACKING__
#else
#define __GNUC_PACKING__ __attribute__ ((packed))
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCF

class CIEEE802154MCF
{
	// typedefs & enums
	public:
		// MAC command frame identifiers, according to section 7.3, Table 82
		enum { typeAssociationRequest = 0x01, typeAssociationResponse = 0x02,
			typeDisassociationNotification = 0x03, typeDataRequest = 0x04,
			typePANIDConflictNotification = 0x05, typeOrphanNotification = 0x06,
			typeBeaconRequest = 0x07, typeCoordinatorRealignment = 0x08,
			typeGTSRequest = 0x09 };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		unsigned char m_bType;

		// Variable-length data follows, depending on the exact type of
		// command frame. Derived, specialized classes exist for each of the
		// supported command frames

	// Operations
	protected:
		// Creates a CPacket object that holds simple command frames, which
		// have no fields other than the command frame identifier
		static void Create(CPacket &packet, const unsigned char bType);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFAssociationRequest, clause 7.3.1

class CIEEE802154MCFAssociationRequest : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeAssociationRequest };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

		// Capability information, encoded using bit fields detailed above
		unsigned char m_bCapabilities;

	// Operations
	public:
		// Creates a CPacket object that holds an association request command
		// frame
		static void Create(CPacket &packet, const unsigned char bCapabilities);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFAssociationResponse, clause 7.3.2

class CIEEE802154MCFAssociationResponse : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeAssociationResponse };

		// Valid values of the association status field, Table 83
		enum { statusSuccess = 0x00, statusAtCapacity = 0x01,
			statusAccessDenied = 0x02 };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

		// Contains the short address that the device may use in its
		// communications on the PAN until it is disassociated. If this value
		// equals 0xffff, the coordinator was not able to associate this device
		// to is PAN. A value of 0xfffe indicates that the device has been
		// successfully associated, but has not been allocated a short address.
		// In this case, the device shall communicate on the PAN using only its
		// 64-bit extended address
		unsigned short m_wShortAddress;

		// Association status field, encoded using bit fields detailed above
		unsigned char m_bStatus;

	// Operations
	public:
		// Creates a CPacket object that holds an association response command
		// frame
		static void Create(CPacket &packet, const unsigned short wShortAddress,
			const unsigned char bStatus);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFDisassociationNotification, clause 7.3.3

class CIEEE802154MCFDisassociationNotification : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeDisassociationNotification };

		// Valid values of the association status field, Table 83
		enum { reasonCoordinatorDriven = 0x01, reasonDeviceDriven = 0x02 };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

		// Disassociation reason
		unsigned char m_bReason;

	// Operations
	public:
		// Creates a CPacket object that holds an association response command
		// frame
		static void Create(CPacket &packet, const unsigned char bReason);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFDataRequest, clause 7.3.4

class CIEEE802154MCFDataRequest : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeDataRequest };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

	// Operations
	public:
		// Creates a CPacket object that holds a data request command frame
		static void Create(CPacket &packet);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFPANIDConflictNotification, clause 7.3.5

class CIEEE802154MCFPANIDConflictNotification : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typePANIDConflictNotification };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)
} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFOrphanNotification, clause 7.3.6

class CIEEE802154MCFOrphanNotification : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeOrphanNotification };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

	// Operations
	public:
		// Creates a CPacket object that holds an orphan notification command
		// frame
		static void Create(CPacket &packet);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFBeaconRequest, clause 7.3.7

class CIEEE802154MCFBeaconRequest : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeBeaconRequest };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

	// Operations
	public:
		// Creates a CPacket object that holds a beacon request command frame
		static void Create(CPacket &packet);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFCoordinatorRealignment, clause 7.3.8

class CIEEE802154MCFCoordinatorRealignment : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeCoordinatorRealignment };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

		// PAN identifier that the coordinator intents to use for all future
		// communications
		unsigned short m_wPANID;

		// Short address of the coordinator, i.e. value of macShortAddress
		unsigned short m_wCoordinatorShortAddress;

		// The logical channel that the coordinator intends to use for all
		// future communications
		unsigned char m_bLogicalChannel;

		// Set to 0xffff, if the coordinator realignment command is broadcast.
		// If sent directly to an orphaned device, this field shall contain the
		// short address that the orphaned device shall use to operate on the
		// PAN. If the orphaned device does not have a short address, because
		// it always uses its 64-bit extended address, this field shall contain
		// the value 0xfffe
		unsigned short m_wShortAddress;

		// Channel page that the coordinator intends to use for all future
		// communications. May be omitted if the new channel page is the same
		// as the previous one
		unsigned char m_bChannelPage;

	// Operations
	public:
		// Creates a CPacket object that holds a coordinator realignment command
		// frame. Notice, when bIncludeChannelPage is true: "The frame version
		// subfield shall be set to 0x01 if the channel page field is present."
		static void Create(CPacket &packet,
			const CIEEE802154Address &coordinator,
			const unsigned char bLogicalChannel,
			const unsigned short wShortAddress = 0xffff,
			const unsigned char bChannelPage = 0,
			const bool bIncludeChannelPage = false);

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MCFGTSRequest, clause 7.3.9

class CIEEE802154MCFGTSRequest : public CIEEE802154MCF
{
	// typedefs & enums
	public:
		// Command frame identifier value for this kind of command frame
		enum { type = typeGTSRequest };

		// Contents of the GTS characteristics field
		enum { characteristicsGTSLengthMask = 0x0f,
			characteristicsReceiveOnlyGTS = 0x10,
			characteristicsAllocation = 0x20 };

	// Attributes
	public:
		// Command frame identifier, common to all MAC command frames
		// unsigned char m_bType; (inherited)

		// GTS characteristics field
		unsigned char m_bCharacteristics;

	// Operations
	public:

} __GNUC_PACKING__;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154GTSDescriptor

class CIEEE802154GTSDescriptor
{
	// Attributes
	public:
		// Device short address
		unsigned short m_wShortAddress;

		// GTS starting slot
		unsigned char m_nStartingSlot : 4;

		// GTS length
		unsigned char m_nLength : 4;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154GTSAssignment, a list<> is returned by GetGTSAssignments()

class CIEEE802154GTSAssignment
{
	// Construction
	public:
		CIEEE802154GTSAssignment(const unsigned short wShortAddress,
			const unsigned char nStartingSlot, const unsigned char nLength,
			const bool bDirectionRX);

		CIEEE802154GTSAssignment();

	// Attributes
	public:
		// GTS descriptor, exact same format as in beacon frame
		CIEEE802154GTSDescriptor m_descriptor;

		// GTS direction (true = receive-only GTS, false = transmit-only GTS)
		bool m_bDirectionRX;

	// Operations
	public:

#ifdef _DEBUG
		// Dumps a GTS assignment (descriptor and direction)
		void Dump() const;
#endif

};

inline CIEEE802154GTSAssignment::CIEEE802154GTSAssignment() {}


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154Beacon, clause 7.2.2.1

class CIEEE802154Beacon
{
	// typedefs & enums
	public:
		// Format of the GTS specification field
		enum { maskGTSDescriptorCount = 0x07, permitGTS = 0x80 };

		// Format of the pending address specification field
		enum { maskShortAddressCount = 0x07, maskExtendedAddressCount = 0x70 };

		// Pending address specification field bit positions
		enum { shiftShortAddressCount = 0, shiftExtendedAddressCount = 4 };

	// Attributes
	protected:
		// Superframe specification field (always present). Access to this
		// structure through GetSuperframeSpecification and CreatePacket()
		// only to avoid alignment faults
		CIEEE802154Superframe m_superframe;

		// variable length GTS fields

		// variable length pending address fields

		// variable length beacon payload

	public:
		// Returns a copy	(guaranteed to be aligned) of the superframe
		// specification
		CIEEE802154Superframe GetSuperframeSpecification() const;

	// Operations
	public:
		// Returns the size of GTS fields, i.e. GTS specification,
		// GTS directions and GTS list
		unsigned char GetGTSFieldsSize() const;

		// Returns the size of pending address fields, i.e. specification and
		// list fields
		unsigned char GetPendingAddressFieldsSize() const;

		// Returns the size of the beacon frame, except for the beacon payload
		unsigned char GetCoreBeaconSize() const;

		// Returns a reference to the GTS specification field
		const unsigned char &GetGTSSpecification() const;

		// Returns a reference to the pending address specification field
		const unsigned char &GetPendingAddressSpecification() const;

		// Returns a list of pending addresses; short and extended addresses
		// might be mixed
		void GetPendingAddresses(std::list<CIEEE802154Address> &pending) const;

		// Returns a list of GTS assignments
		void GetGTSAssignments(std::list<CIEEE802154GTSAssignment> &gts) const;

		// Inserts a pending address list (specification and address entries)
		// in front of existing frame contents
		static void PrependPendingAddresses(CPacket &packet,
			const std::list<CIEEE802154Address> &pending);

		// Inserts a GTS assignment list (specification, directions, and slot
		// descriptors in front of existing frame contents
		static void PrependGTSAssignments(CPacket &packet,
			const std::list<CIEEE802154GTSAssignment> &gts, const bool bPermit);

		// Creates a CPacket object that holds a beacon frame. The beacon
		// payload, if applicable, must be stored in the packet object prior to
		// passing it to Create(). The list of pending addresses must be
		// arranged such that short addresses are followed by extended
		// addresses
		static void Create(CPacket &packet,
			const std::list<CIEEE802154Address> &pending,
			const std::list<CIEEE802154GTSAssignment> &gts,
			const CPacket &payload, const bool bPermitGTS = false,
			const unsigned char nBeaconOrder = 15,
			const unsigned char nSuperframeOrder = 15,
			const unsigned char nFinalCAPSlot = 15,
			const bool bIsPANCoordinator = false,
			const bool bPermitAssociation = true,
			const bool bBatteryLifeExtension = false);

#ifdef _DEBUG
		// Dumps a beacon frame given its MAC header and MAC payload portions
		static void Dump(const CIEEE802154MHR &header, const CPacket &payload);
#endif

} __GNUC_PACKING__;



/////////////////////////////////////////////////////////////////////////////
// restore previous packing

#ifndef __GNUC__
#pragma pack(pop)
#endif
