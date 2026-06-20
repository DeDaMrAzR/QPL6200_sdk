// PersistentStorage.h : header file
//
// Copyright(C) 2018-2023 ubisys technologies, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This file is part of the CompactStorage(TM) library and allows for a smooth
// transitioning from CPersistentStorage1 to CPersistentStorage2

/////////////////////////////////////////////////////////////////////////////
// __COMPACT_STORAGE defines the version to use (defaults to 1)

#ifndef __COMPACT_STORAGE
#define __COMPACT_STORAGE 1
#endif


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage1

#if (__COMPACT_STORAGE == 1)

#include "PersistentStorage1.h"
#include "PersistentVolatile1.h"

typedef CPersistentStorage1 CPersistentStorageBase;
typedef CPersistentStorage1Item CPersistentStorageItemBase;
typedef CPersistentStorage1ItemGeneric CPersistentStorageItemGeneric;
typedef CPersistentStorage1Search CPersistentStorageSearch;
typedef CPersistentVolatile1 CPersistentVolatile;

#endif


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage2

#if (__COMPACT_STORAGE == 2)

#include "PersistentStorage2.h"

typedef CPersistentStorage2 CPersistentStorageBase;
typedef CPersistentStorage2ItemCanonical CPersistentStorageItemBase;
typedef CPersistentStorage2ItemGeneric CPersistentStorageItemGeneric;
typedef CPersistentStorage2Search CPersistentStorageSearch;


/////////////////////////////////////////////////////////////////////////////
// CPersistentVolatile (unused, only for source code compatibility)

class CPersistentVolatile
{
	// typedefs & enums
	public:
		enum { size8, size16, size32, size64, sizeMask = 0x03,
			slots4 = 0x04, slots8 = 0x08, slots16 = 0x0c, slots32 = 0x10,
			slots64 = 0x14, slotCountShift = 2, slotCountMask = 0x1c };
};

#endif


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorageItem

class CPersistentStorageItem : public CPersistentStorageItemBase
{
	// typedefs & enums
	public:

#if (__COMPACT_STORAGE == 2)
		enum { flagVersionShift = flagexVersionShift };
#endif

	// Construction
	public:
		CPersistentStorageItem(const uint_fast8_t nDomain);

	// Attributes
	public:
		// Domain identifier. Allows modular management of storage items
		uint8_t m_nDomain;

	// Operations
	public:
		// Declares this item as being "volatile", i.e. frequently changing
		void DeclareVolatile();
};


/////////////////////////////////////////////////////////////////////////////
// CPersistentStorage

class CPersistentStorage : public CPersistentStorageBase
{
	// Construction
	public:
		CPersistentStorage(CPagedFlashDevice &device,
			const CMemoryDevice::address_type address,
			const CPersistentStorageItem::size_type cbStorage,
			const CPersistentStorageItem::size_type cbItem =
				sizeof(CPersistentStorageItem),
			const unsigned char nGranularityShift = 0,
			const bool factoryArea = false,
			const bool backupArea = false);

	// Operations
	public:
		// Loads a volatile (frequently changing) persistent storage item
		bool LoadVolatile(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem,
			void *const pData,
			const CPersistentStorageItem::size_type cbData,
			const uint_fast8_t nOptions = 0);

		// Stores a volatile (frequently changing) persistent storage item
		bool StoreVolatile(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem,
			const void *const pData,
			const CPersistentStorageItem::size_type cbData,
			const uint_fast8_t nOptions = 0);

		// Restores the persistent storage working area to factory defaults,
		// as provided by the caller
		void RevertToFactoryFresh(const void *const pFactory,
			const size_t cbFactory);

#if (__COMPACT_STORAGE == 2)

		// Stores a specific item, which may have been allocated before or will
		// be allocated automatically. Make sure to set the item's requested
		// capacity correctly before invoking Store(). Attempts to grow the
		// item if it has not currently enough storage capacity to hold the new
		// data. Finds the item based on the unique part of the storage item
		// directory entry
		bool StoreEx(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem,
			const void *const pData,
			const CPersistentStorageItem::size_type cbData,
			const uint8_t nFlagsEx = 0,
			const CPersistentStorageItem::size_type cbCapacity = 0);

#endif

};
