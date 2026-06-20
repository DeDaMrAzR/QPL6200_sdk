// Packet.h : header file
//
// Copyright(C) 2008-2016 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Part of the ubisys(R) Compact15.4(TM) class library
//
// A powerful packet container class that provides easy packet assembly and
// disassembly in an efficient manner. This implementation allows the stack
// implementor to reserve space before and after the body. This allows adding
// headers and trailers to the actual payload as the packet traverses the
// stack. If enough space is reserved, no memory moving or copying is
// required. A copy-on-write feature further improves the overall design.
// The implementation is scalable. The stack designer may enable/disable
// features individually depending on the runtime environment (e.g. RFD vs.
// FFD device, tiny active RFID tag vs. PC, etc.).
//
// __CPACKET_ALLOW_GROW__ enables automatic packet growth beyond initial
// capacity limits (within capacity limits, automatic packet growth is always
// possible). __CPACKET_ALLOW_MOVE__ must be defined, too.
//
// __CPACKET_ALLOW_MOVE__ allows moving packet contents within the overall
// reserved space. This allows to shift between header and trailer capacity
// as required at the expence of a call to memmove().
//
// #define __CPACKET_ALLOW_COW__, to allow copy-on-write on packet level.
// Does not require __CPACKET_ALLOW_GROW__
//
// __CPACKET_SIZETYPE__ defines the data type used to store sizes, offsets,
// and capacity limits (default is byte yielding a maximum capacity of 255
// bytes). For 64KB capacity use unsigned short, for 4GB use unsigned long

/////////////////////////////////////////////////////////////////////////////
// IAR C++ compiler: additional link-time check

#ifdef __ICCARM__

#ifdef __CPACKET_SIZETYPE__
#error Do not define __CPACKET_SIZETYPE__ when using IAR C++
#endif

#ifdef __CPACKET_SIZETYPE_8__
#pragma rtmodel = "CPacket::size_type", "uint8_t"
#elif (defined __CPACKET_SIZETYPE_16__)
#pragma rtmodel = "CPacket::size_type", "uint16_t"
#elif (defined __CPACKET_SIZETYPE_32__)
#pragma rtmodel = "CPacket::size_type", "uint32_t"
#elif (defined __CPACKET_SIZETYPE_64__)
#pragma rtmodel = "CPacket::size_type", "uint64_t"
#else
#error Please define __CPACKET_SIZETYPE_8/16/32/64__ when using IAR C++
#endif

#endif


/////////////////////////////////////////////////////////////////////////////
// Default CPacketData::size_type, if none is defined

#ifndef __CPACKET_SIZETYPE__
#ifdef __CPACKET_SIZETYPE_8__
#define __CPACKET_SIZETYPE__ unsigned char
#elif (defined __CPACKET_SIZETYPE_16__)
#define __CPACKET_SIZETYPE__ unsigned short
#elif (defined __CPACKET_SIZETYPE_32__)
#define __CPACKET_SIZETYPE__ unsigned long
#elif (defined __CPACKET_SIZETYPE_64__)
#define __CPACKET_SIZETYPE__ unsigned long long
#else
#define __CPACKET_SIZETYPE__ std::size_t
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// CPacketData

class CPacketData
{
	friend class CPacket;

	// typedefs & enums
	public:
		// Type of size, offset, and capacity members
		typedef __CPACKET_SIZETYPE__ size_type;

		// Type of size, offset, and capacity arguments
		typedef size_t arg_type;

	// Construction
	public:
		explicit CPacketData(const arg_type nBodyAndTrailerCapacity,
			const arg_type nHeaderCapacity = 0);

		CPacketData(const CPacketData &data);

	// Operations
	public:
		// Increments the reference count by one and returns its current
		// value
		size_type AddReference();

		// Decrements the reference count by one. If it reaches zero, the
		// object is destroyed. As soon as Release() has returned zero, the
		// instance has been deleted and must not be accessed any more
		size_type Release();

		// Appends data at the end of the packet
		void Append(const void *const pData, const arg_type cbData);

		// Inserts data in front of the packet
		void Prepend(const void *const pData, const arg_type cbData);

		// Strips off a header, i.e. removes it from the packet
		void StripHeader(const arg_type cbHeader);

		// Extracts a header and removes it from the packet
		void RemoveHeader(void *pHeader, const arg_type cbHeader);

		// Strips of a trailer, i.e. removes it from the packet
		void StripTrailer(const arg_type cbTrailer);

		// Extracts a trailer and removes it from the packet
		void RemoveTrailer(void *pTrailer, const arg_type cbTrailer);

		// Returns the length of the packet
		size_type GetSize() const;

		// Returns the number of bytes that can be maximally stored in this
		// packet
		size_type GetCapacity() const;

		// Sets the size of the packet
		void SetSize(const arg_type nSize);

		// Returns a pointer to the start of packet data
		void *GetData() const;

		// Returns the number of bytes that can be prepended to the packet,
		// without causing packet growth or data being moved
		size_type AvailableHeaderCapacity() const;

		// Returns the number of bytes that can be prepended to the packet,
		// without causing packet growth (but allowing data being moved)
		size_type AvailableHeaderCapacityEx() const;

		// Returns the number of bytes that can be appended to the packet,
		// without causing packet growth or data being moved
		size_type AvailableTrailerCapacity() const;

		// Returns the number of bytes that can be appended to the packet,
		// without causing packet growth (but allowing data being moved)
		size_type AvailableTrailerCapacityEx() const;

	// Implementation
	public:
		~CPacketData();

#ifdef _DEBUG
		// Dumps the packet data
		void Dump(const arg_type start = 0, const bool asHex = true,
			const bool asText = false, const bool onlyContents = true,
			const bool newLine = true, char separator = ':') const;
#endif

	protected:
		// Packet data of variable length
		unsigned char *m_pbData;

		// Number of bytes currently stored in the body section
		size_type m_nSize;

		// Number of bytes that can be maximally stored in this packet
		size_type m_nCapacity;

		// Indicates the beginning of the actual data payload. Allows reserving
		// space in front of the payload for headers added as the packet
		// traverses the protocol stack
		size_type m_nOffset;

		// Reference counter. Each CPacket instance referencing this object
		// increments the reference count by one
		size_type m_nReference;

		// Allocates memory large enough to store at least nCapacity elements
		void Reserve(const arg_type nCapacity);
};

inline void *CPacketData::GetData() const
{
	ASSERT_VALIDHEAPBLOCK(m_pbData, m_nCapacity);
	return m_pbData + m_nOffset;
}

inline CPacketData::size_type CPacketData::GetSize() const
	{ return m_nSize; }

#ifdef __CPACKET_ALLOW_MOVE__
inline CPacketData::size_type CPacketData::GetCapacity() const
	{ return m_nCapacity; }
#else
inline CPacketData::size_type CPacketData::GetCapacity() const
	{ return m_nCapacity - m_nOffset; }
#endif


/////////////////////////////////////////////////////////////////////////////
// CPacket

class CPacket
{
	// Construction
	public:
		// Creates a new, empty packet with the specified initial capacities
		explicit CPacket(const CPacketData::arg_type nBodyAndTrailerCapacity,
			const CPacketData::arg_type nHeaderCapacity = 0);

		// Creates a new packet initialized with the specified body.
		// Additional space is reserved for header and trailer
		explicit CPacket(const void *const pData,
			const CPacketData::arg_type nSize,
			const CPacketData::arg_type nTrailerCapacity = 0,
			const CPacketData::arg_type nHeaderCapacity = 0);

		// Copy constructor
		CPacket(const CPacket &packet);

		// Creates an empty packet object (without associated data)
		CPacket();

	// Attributes
	public:
		// Returns true, if the packet is empty, i.e. no packet data has been
		// attached to this object
		bool IsEmpty() const;

		// Returns the length of the packet. Call this version if you know that
		// packet data object has been attached to this object, otherwise call
		// GetSizeEx(), which additionally checks against an empty packet
		CPacketData::size_type GetSize() const;

		// Returns the length of the packet. Call this version if you do not
		// know whether a packet data object has been attached to this object,
		// otherwise call GetSize() for improved performance
		CPacketData::size_type GetSizeEx() const;

		// Returns the number of bytes that can be maximally stored
		CPacketData::size_type GetCapacity() const;

		// Returns the number of bytes that can be prepended sans grow/move
		CPacketData::size_type AvailableHeaderCapacity() const;

		// Returns the number of bytes that can be prepended ans grow (may move)
		CPacketData::size_type AvailableHeaderCapacityEx() const;

		// Returns the number of bytes that can be appended sans grow/move
		CPacketData::size_type AvailableTrailerCapacity() const;

		// Returns the number of bytes that can be appended sans grow (may move)
		CPacketData::size_type AvailableTrailerCapacityEx() const;

		// Sets the size of the packet
		void SetSize(const CPacketData::arg_type nSize);

		// Returns a read-only pointer to packet data.
		const void *GetData() const;

		// Returns a read-only pointer to a location within packet data
		const void *GetDataAt(const CPacketData::arg_type nOffset) const;

		// Returns a writable pointer to packet data. Use the read-only
		// version if you do not really need to write to the packet
		void *GetData();

		// Returns a writable pointer to a location within packet data. Use the
		// read-only version if you do not really need to write to the packet
		void *GetDataAt(const CPacketData::arg_type nOffset);

		// Returns a read-only pointer to packet data or null if no packet data
		// is attached
		const void *GetDataEx() const;

		// Returns a writable pointer to packet data or null if no packet data
		// is attached. Use the read-only version if you do not really need to
		// write to the packet
		void *GetDataEx();

	// Operations
	public:
		// Appends data at the end of the packet
		void Append(const void *const pData,
			const CPacketData::arg_type cbData);

		// Appends data stored in another packet at the end of this packet
		void Append(const CPacket &packet);

		// Inserts data in front of the packet
		void Prepend(const void *const pData,
			const CPacketData::arg_type cbData);

		// Inserts data stored in another packet in front of this packet
		void Prepend(const CPacket &packet);

		// Strips of a header, i.e. removes it from the packet
		void StripHeader(const CPacketData::arg_type cbHeader);

		// Strips of a header, i.e. removes it from the packet, if the packet
		// is long enough or returns false otherwise
		bool StripHeaderEx(const CPacketData::arg_type cbHeader);

		// Extracts a header and removes it from the packet
		void RemoveHeader(void *pHeader,
			const CPacketData::arg_type cbHeader);

		// Extracts a header and removes it from the packet, if the packet is
		// long enough or returns false otherwise
		bool RemoveHeaderEx(void *pHeader,
			const CPacketData::arg_type cbHeader);

		// Strips of a trailer, i.e. removes it from the packet
		void StripTrailer(const CPacketData::arg_type cbTrailer);

		// Strips of a trailer, i.e. removes it from the packet, if the packet
		// is long enough or returns false otherwise
		bool StripTrailerEx(const CPacketData::arg_type cbTrailer);

		// Extracts a trailer and removes it from the packet
		void RemoveTrailer(void *pTrailer,
			const CPacketData::arg_type cbTrailer);

		// Extracts a trailer and removes it from the packet, if the packet is
		// long enough or returns false otherwise
		bool RemoveTrailerEx(void *pTrailer,
			const CPacketData::arg_type cbTrailer);

		// Clears the packet contents, such that the packet provides its
		// specified capacity. Header capacity may be specified if non-zero
		void Clear(const CPacketData::arg_type nHeaderCapacity = 0);

		// True, if packets are equal
		bool operator==(const CPacket &packet) const;

		// True, if packets are unequal
		bool operator!=(const CPacket &packet) const;

		// Assignment operator
		CPacket &operator=(const CPacket &packet);

		// Detaches the underlying packet object container and returns a packet
		// object that can be passed to higher/lower layers. Then attaches
		// another container object (or a null object). By using Attach() the
		// caller declares that no further access is intended to the original
		// packet object. If a null pointer is passed, the CPacketData object
		// will be invalidated, i.e. access to packet data is prohibited
		CPacket Attach(CPacketData *const pNewPacketData = 0);

		// Calls Attach() with a null object
		CPacket Detach();

		// Explicitly creates a copy (also works, if copy-on-write is not
		// enabled). Should only be used when a copy is really required. Only
		// the active data portion is copied, i.e. unused header and trailer
		// space is omitted. Optionally reserves additional header/trailer
		// capacity
		CPacket CreateCopy(const CPacketData::arg_type nTrailerCapacity = 0,
			const CPacketData::arg_type nHeaderCapacity = 0) const;

		// Creates an exact copy of the packet, including currently unused
		// header and trailer portions. This can be used to make sure that
		// lower layer headers are still accessible (cross-layer optimization),
		// for instance. Optionally reserves additional header/trailer
		// capacity
		CPacket CreateCopyEx(const CPacketData::arg_type nTrailerCapacity = 0,
			const CPacketData::arg_type nHeaderCapacity = 0) const;

#ifdef _DEBUG
		void Dump(const char *const message = 0,
			const CPacketData::arg_type start = 0,
			const bool asHex = true, const bool asText = false,
			const bool onlyContents = true, const bool newLine = true,
			const char separator = ':') const;
#endif

	// Implementation
	public:
		virtual ~CPacket();

	protected:
		// The CPacketData object that holds the actual data
		CPacketData *m_pData;

		// Called internally whenever write access to packet data is
		// required. Performs copy-on-write if necessary
		void PrepareWriteAccess();
};

#ifndef _DEBUG

inline CPacketData::size_type CPacket::GetSize() const
	{ return m_pData->GetSize(); }

inline CPacketData::size_type CPacket::GetCapacity() const
	{ return m_pData->GetCapacity(); }

inline CPacketData::size_type CPacket::AvailableHeaderCapacity() const
	{ return m_pData->AvailableHeaderCapacity(); }

inline CPacketData::size_type CPacket::AvailableHeaderCapacityEx() const
	{ return m_pData->AvailableHeaderCapacityEx(); }

inline CPacketData::size_type CPacket::AvailableTrailerCapacity() const
	{ return m_pData->AvailableTrailerCapacity(); }

inline CPacketData::size_type CPacket::AvailableTrailerCapacityEx() const
	{ return m_pData->AvailableTrailerCapacityEx(); }

inline const void *CPacket::GetData() const
	{ return m_pData->GetData(); }

inline void *CPacket::GetData()
	{ PrepareWriteAccess(); return m_pData->GetData(); }

inline const void *CPacket::GetDataAt(const CPacketData::arg_type nOffset) const
	{ return static_cast<const unsigned char *>(GetData()) + nOffset; }

inline void *CPacket::GetDataAt(const CPacketData::arg_type nOffset)
	{ return static_cast<unsigned char *>(GetData()) + nOffset; }

#endif

inline bool CPacket::IsEmpty() const
	{ return m_pData ? false : true; }

inline CPacketData::size_type CPacket::GetSizeEx() const
	{ return IsEmpty() ? 0 : GetSize(); }

inline const void *CPacket::GetDataEx() const
	{ return m_pData ? m_pData->GetData() : 0; }

inline void *CPacket::GetDataEx()
	{ return m_pData ? m_pData->GetData() : 0; }

inline CPacket CPacket::Detach()
	{ return Attach(); }
