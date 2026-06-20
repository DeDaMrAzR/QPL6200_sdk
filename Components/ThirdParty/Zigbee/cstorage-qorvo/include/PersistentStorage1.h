// PersistentStorage1.h : header file
//
// Copyright(C) 2011-2019 ubisys technologies, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This file is part of the CompactStorage(TM) 1.0 library and provides an
// adaptation layer, which provides an implementation of CPersistentStorage1
// on top of Qorvo's NVM API
//
// IMPORTANT NOTICE: This specialized version is binary compatible with the
// original CPersistentStorage1 class, resulting in the ability to swap
// implementations at link time without the requirement to build dependent
// software components against this version at compile time. If this approach
// changes, make sure to build against this header file instead of the
// original one

/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage1Item

#pragma pack(push, 1)

class CPersistentStorage1Item
{
	// typedefs & enums
	public:
		typedef unsigned short size_type;

		// Item types. Custom data item types may be defined in type range
		// 9 - 15 in derived classes if required
		enum { typeUnused, typeDirectory, typeBadPageRecord, typeGuard = 7,
			typeData };

		// Item flags (includes the type)
		enum { flagTypeMask = 0x000f, flagVolatile = 0x8000,
			flagReadOnly = 0x4000, flagFragmented = 0x2000,
			flagForceFlush = 0x1000, flagFragmentMask = 0x0f00,
			flagVersionMask = 0x00f0, flagVersionShift = 4 };

		// Maximum directory item size. This is for the directory entry itself,
		// not the associated data block, which is limited by size_type and
		// available storage space alone
		enum { maximumSize = 16 };

	// Construction
	public:
		CPersistentStorage1Item(unsigned short wFlags = typeUnused,
			const size_type nCapacity = 0, const size_type nSize = 0,
			const CMemoryDevice::address_type address = 0);

	// Attributes
	public:
		// Location of actual data
		CMemoryDevice::address_type m_address;

		// Storage item flags (combination of the flags defined above)
		unsigned short m_wFlags;

		// Actual size of data
		size_type m_nSize;

		// Total reserved storage space (for binary data, strings, etc.)
		size_type m_nCapacity;

		// Application-specific identifiers (property id, etc.) defined in
		// derived classes...

	// Operations
	public:
		// Returns true, if this is a directory directory item
		bool IsDirectoryItem() const;

		// Returns the type of item
		unsigned char GetType() const;

		// Returns the value size
		size_type GetValueSize() const;

		// Returns a read-only pointer to the raw key
		const void *GetKey() const;

		// Returns a read/write pointer to the raw key
		void *GetKeyEx();

		// Returns the payload data version number (aka data storage format
		// identifier) of the item
		uint8_t GetVersion() const;

		// Returns the number of blocks required to store the specified number
		// of bytes using the specified granularity
		static unsigned int SizeToBlocks(const size_type nSize,
			const unsigned short nGranularityShift);

		// Returns the number of blocks required to store the specified number
		// of bytes using the specified granularity
		static size_type BlocksToSize(const unsigned int nBlocks,
			const unsigned short nGranularityShift);

		// Copy the value data to a destination address
		void CopyValue(void *const destination, const size_type length) const;
};

#pragma pack(pop)

inline unsigned char CPersistentStorage1Item::GetType() const
	{ return (m_wFlags & flagTypeMask); }

inline CPersistentStorage1Item::size_type
	CPersistentStorage1Item::GetValueSize() const
	{ return m_nSize; }

inline const void *CPersistentStorage1Item::GetKey() const
	{ return this + 1; }

inline void *CPersistentStorage1Item::GetKeyEx()
	{ return this + 1; }

inline bool CPersistentStorage1Item::IsDirectoryItem() const
	{ return GetType() < typeData; }

inline uint8_t CPersistentStorage1Item::GetVersion() const
	{ return (m_wFlags & flagVersionMask) >> flagVersionShift; }


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage1ItemGeneric

class CPersistentStorage1ItemGeneric : public CPersistentStorage1Item
{
	// Attributes
	public:
		unsigned char m_abCustomData[CPersistentStorage1Item::maximumSize -
			sizeof(CPersistentStorage1Item)];

	// Operations
	public:
		// Returns the size of the key
		size_type GetKeySize() const;

		// Sets the size of the key
		void SetKeySize(const size_type cbKey);

	// Implementation
	public:

#ifdef _DEBUG
		void Dump(const size_type cbItem = maximumSize) const;
#endif

};


inline CPersistentStorage1Item::size_type
CPersistentStorage1ItemGeneric::GetKeySize() const
{
	// CPersistentStorage version 1 has fixed key capacity/size
	return sizeof(m_abCustomData);
}


inline void CPersistentStorage1ItemGeneric::SetKeySize(const size_type cbKey)
{
	// CPersistentStorage version 1 has fixed key capacity/size
	ASSERT(cbKey == sizeof(m_abCustomData));
}


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage1PageTrailer

class CPersistentStorage1PageTrailer
{
	// typedefs & enums
	public:
		enum { flagGranularity8 = 0, flagGranularity16 = 1,
			flagGranularity32 = 2, flagGranularityMask = 0x0003,
			flagVolatile = 0x8000 };

	// Attributes
	public:
		// Variable length available block information in front of the flags
		// here. The total size depends on the flash page size, e.g. 256, 512
		// or 4096 bytes and the granularity of the page. The flash page size
		// divided by the granularity gives the number of blocks available in
		// the page. However this trailer will also occupy some of the blocks,
		// such that the remaining blocks need to be mapped to an array of
		// bits. If a bit in the bitmap is set, the corresponding block is
		// available (not used). For example, a page size of 256 bytes with a
		// granularity of 8 bytes per block results in 32 blocks, where 31 of
		// those are actually usable (trailer consists of four bytes for the
		// 32 bits, 16-bit flags and 16-bit checksum, i.e. 8 bytes = 1 block
		// for the trailer). As another example, a page size of 4096 bytes
		// with a granularity of 16 bytes per block results in 256 blocks,
		// where 253 of those are actually usable (trailer consists of 32 bytes
		// for the 256 bits, 16-bit flags and 16-bit checksum, i.e. 36 bytes
		// = 3 blocks for the trailer). In general: trailer_blocks =
		// ((page_size / (granularity * 8) + 4) + granularity - 1) / granularity

		// Granularity of memory blocks. Memory blocks are guaranteed to be
		// aligned according to the block granularity. The trailer is always
		// the last block (of the specified granularity)
		unsigned short m_wFlags;

		// Checksum, 16-bit CRC over the entire page, including this trailer
		unsigned short m_wCheckSum;

	// Operations:
	public:
		// Returns the total size of the trailer
		static CPersistentStorage1Item::size_type
		GetSize(const unsigned short wFlags,
			const CPersistentStorage1Item::size_type nPageSize);

		// Returns the total size of the trailer
		CPersistentStorage1Item::size_type
		GetSize(const CPersistentStorage1Item::size_type nPageSize) const;

		// Returns the number of bytes available to the application
		static CPersistentStorage1Item::size_type
		GetCapacity(const unsigned short wFlags,
			const CPersistentStorage1Item::size_type nPageSize);

		// Returns the number of blocks available to the application
		static unsigned int GetCapacityEx(const unsigned short wFlags,
			const CPersistentStorage1Item::size_type nPageSize);

		// Returns the number of total usable blocks and a pointer to the
		// available block map
		unsigned int GetAvailableBlocks(unsigned char *&pbAvailableBlockMap,
			const CPersistentStorage1Item::size_type nPageSize) const;
};


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage1Table

class CPersistentStorage1Table
{
	// typedefs & enums
	public:
		// Encoding of current usage pattern. Notice: This is limited to two
		// bits, so do not add more states without increasing the bits per page
		enum { pageAvailable, pageDirectory, pageData, pageBad,
			pageVolatileData };

	// Construction
	public:
		CPersistentStorage1Table();

	// Attributes
	public:
		// Returns the number of pages
		size_t GetSize() const;

	// Operations
	public:
		// Allocates the memory required to store the page table for the
		// specified number of pages. Initially, marks all pages as available
		void Allocate(const size_t nPages);

		// Returns the current usage pattern for the specified page
		unsigned int GetUsage(const size_t nPage) const;

		// Sets the current usage pattern for the specified page
		void SetUsage(const size_t nPage, const unsigned int nUsage);

		// Sames as GetUsage()
		unsigned int operator[](const size_t nPage) const;

	// Implementation
	public:
		~CPersistentStorage1Table();

	protected:
		// Number of pages
		size_t m_nPages;

		// Page table: allocation/usage map. For each page in storage space,
		// this map holds two bits indicating the current usage pattern
		unsigned int *m_pnAllocationMap;
};

inline size_t CPersistentStorage1Table::GetSize() const
	{ return m_nPages; }


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage1

class CPersistentStorage1
{
	// typedefs & enums
	public:
		// Identifier of the Qorvo NVM pool used by the ubisys stack
		enum { pool = 1 };

		typedef bool(*PREDICATE)(const CPersistentStorage1Item &item,
			void *const argument);

	// Construction
	public:
		CPersistentStorage1(CPagedFlashDevice &device,
			const CMemoryDevice::address_type address = pool,
			const CPersistentStorage1Item::size_type cbStorage = 0,
			const CPersistentStorage1Item::size_type cbItem =
				sizeof(CPersistentStorage1Item),
			const unsigned char nGranularityShift = 0,
			const CMemoryDevice::address_type offset = 0);

	// Attributes
	public:
		// The Qorvo NVM pool to use
		const CMemoryDevice::address_type m_address;

		// Not used in this implementation
		const CPersistentStorage1Item::size_type m_cbStorage;

		// Size of CPersistentStorage1Item derived item (application specific)
		const CPersistentStorage1Item::size_type m_cbItem;

		// Not used in this implementation
		const unsigned char m_nGranularityShift;

	// Operations
	public:
		// Must be called once after start-up. Returns true if the storage is
		// valid and ready to use
		bool Initialize();

		// Updates page information
		bool RefreshPageInfo(const bool includeVolatile = true,
			const bool entire = true);

		// Returns true, if the storage is valid
		bool IsValid() const;

		// Clears the storage and prepares it for additional entries
		bool Format() const;

		// Returns the item stored at the specified address
		void GetItem(CPersistentStorage1Item &item,
			const CMemoryDevice::address_type address,
			const CPersistentStorage1Item::size_type cbItem =
				sizeof(CPersistentStorage1Item)) const;

		// Advances to the next item. Can be called repeatedly to iterate over
		// all items. Notice that this method returns all items including
		// directory items, available (unused) items, etc. Returns true, if the
		// item data is valid, false otherwise. Also updates the next item's
		// address. Expects valid data on entry
		bool GetNextItem(CPersistentStorage1Item &item,
			CMemoryDevice::address_type &address,
			const CPersistentStorage1Item::size_type cbItem =
				sizeof(CPersistentStorage1Item)) const;

		// Finds the specified user data item. Compares the portion of the
		// directory entry after the capacity element
		bool FindItem(CMemoryDevice::address_type &address,
			CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem) const;

		// Makes a unique item with a common part of the specified size by
		// appending cbUnique bytes, which make a unique item key taken as
		// a whole
		bool MakeUniqueItem(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbCommon,
			const CPersistentStorage1Item::size_type cbUnique = 1,
			bool *const existing = 0, const PREDICATE predicate = 0,
			void *const argument = 0) const;

		// Allocates storage for a new data item. On entry, the data item's
		// capacity field must contain the amount of data storage space
		// required. On successful return, the m_address field contains the
		// location of the allocated storage space for the data. The capacity
		// might have been increased and is a multiple of the data block
		// granularity. If initial data is provided, the new item will also
		// have the provided data of the specified item size
		bool Allocate(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			const void *const pData = 0,
			const CPersistentStorage1Item::size_type cbData = 0,
			const unsigned short nFlags = 0,
			const CPersistentStorage1Item::size_type cbCapacity = 0) const;

		// Deletes a particular item from the item storage. Returns true if the
		// item was found and removed
		bool Free(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem) const;

		// Deletes matching items from the item storage. Returns true if either
		// no matching item exists or all matching items have been deleted
		bool FreeEx(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem) const;

		// Attempts to locate the specified item and updates the size and
		// capacity members of the item record, if an item was found
		bool Lookup(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem);

		// Stores a specific item, which may have been allocated before or will
		// be allocated automatically if allowed. Make sure to set the item's
		// requested capacity correctly before invoking Store(). Finds the item
		// based on the unique part of the storage item directory entry
		bool Store(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			const void *const pData,
			const CPersistentStorage1Item::size_type cbData,
			const unsigned short nFlags = 0,
			const CPersistentStorage1Item::size_type cbCapacity = 0) const;

		// Stores a specific item, which may have been allocated before or will
		// be allocated automatically. Make sure to set the item's requested
		// capacity correctly before invoking Store(). Attempts to grow the
		// item if it has not currently enough storage capacity to hold the new
		// data. Finds the item based on the unique part of the storage item
		// directory entry
		bool StoreEx(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			const void *const pData,
			const CPersistentStorage1Item::size_type cbData,
			const unsigned short nFlags = 0,
			CPersistentStorage1Item::size_type cbCapacity = 0) const;

		// Retrieves the contents of the specified data item
		bool Load(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			void *const pData, const CPersistentStorage1Item::size_type cbData,
			CPersistentStorage1Item::size_type *const pcbData = 0) const;

		// Returns the location and size of the specified item's data contents.
		// If the memory device is mapped into memory space (e.g. internal
		// flash ROM), the data can be accessed via usual read memory
		// operations
		bool GetData(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			void *&pData, CPersistentStorage1Item::size_type &cbData) const;

		// Restore existing storage items from an external storage into the
		// current storage
		void RestoreItems(CPersistentStorage1 &storage);

	// Implementation
	public:
		// A paged flash memory device
		CPagedFlashDevice &m_device;

		// True if the storage is valid (consistency checks successful)
		static bool bValid;

		// Class of tokens stored in NVM, either regular or volatile
		enum { classVolatile, classRegular };

#ifdef _DEBUG
		// Dumps storage info
		void Dump(const bool bDumpItems = true) const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage1Search

class CPersistentStorage1Search
{
	friend class CPersistentStorage1;

	// typedefs & enums
	public:
		typedef bool (*PREDICATE)(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			const CPersistentStorage1Search &search);

	// Construction
	public:
		CPersistentStorage1Search(const CPersistentStorage1 &storage,
			const CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			const PREDICATE pfnPredicate = 0, void *const pArgument = 0);

	// Attributes
	public:
		// Argument to the optional predicate function
		void *const m_pArgument;

	// Operations
	public:
		// Resets the search context, i.e. searches from the beginning
		void Startover();

		// Reinitializes the search context after an item has been deleted if
		// the underlying storage requires this. Returns true of the search is
		// starting over, false if it will resume
		bool AfterFree(const bool bForce = false);

		// Finds the next item
		bool FindNext(CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem);

		// Loads the data contents of the current item
		bool Load(void *const pData,
			const CPersistentStorage1Item::size_type cbData,
			CPersistentStorage1Item::size_type *const pcbData = 0) const;

	// Implementation
	public:
		~CPersistentStorage1Search();

		// Length of the currently found storage item
		uint8_t m_cbItemLength;

	protected:
		// Glue to persistent storage instance to be used for searching
		const CPersistentStorage1 &m_storage;

		// In the original class, there are two CMemoryDevice::address_type
		// members, here (spanning m_handle to m_pData below)

		// Handle of the low-level Qorvo NVM look-up table
		uint8_t m_handle;

		// Size of the data stored in m_pData
		uint8_t m_cbData;

		// Length of the search mask
		CPersistentStorage1Item::size_type m_cbItem;

		// Data contents of the current match
		void *m_pData;

		// Optional predicate to inspect potential matches
		const PREDICATE m_pfnPredicate;

		// An item variable fully identifying the current match
		CPersistentStorage1ItemGeneric m_thisItem;

		// Reference to an item variable with the search key
		const CPersistentStorage1Item &m_item;
};
