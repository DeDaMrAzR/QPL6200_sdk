// ZigBeeStorage.h : header file
//
// Copyright(C) 2008-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This implementation is based on the ZigBee Alliance's ZigBee Specification
// as of January 17, 2008
//
// This module implements non-volatile storage

/////////////////////////////////////////////////////////////////////////////
// CZigBeeStorageItem

#pragma pack(push, 1)

class CZigBeeStorageItem : public CPersistentStorageItem
{
	// Construction
	public:
		CZigBeeStorageItem(const uint8_t nDomain);

	// typedefs & enums
	public:
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeStorage

class CZigBeeStorage
{
	// Construction
	public:
		CZigBeeStorage(CPagedFlashDevice &device,
			const CMemoryDevice::address_type address,
			const CPersistentStorageItem::size_type cbStorage = 1024,
			const CPersistentStorageItem::size_type cbItem =
				sizeof(CPersistentStorageItemGeneric),
			const unsigned char nGranularityShift = 0,
			const bool factoryArea = false, const bool backupArea = false);

	// Operations
	public:
		// Reverts storage to factory fresh settings
		void RevertToFactoryFresh();

		// Complete a previously interrupted factory reset, i.e., apart from
		// reverting to the factory default settings, additionally restore the
		// retained data from a temporary retain storage to the working storage
		void CompleteFactoryReset(CPersistentStorage &storage);

		// Returns a storage that is supplied from factory-fresh settings;
		// this is useful for selective factory resets, including, but not
		// limited to, reverting to a default reporting configuration
		static CZigBeeStorage &GetFactoryStorage();

	// Implementation
	public:
		// Address of working storage area within non-volatile storage. Notice,
		// that the actual interpretation of this value is specific to the
		// particular CPersistentStorage flavor being used
		static __COMPACT_STOCK uint32_t dwWorkingAreaAddress;

		// Size of working storage area within non-volatile storage. Notice,
		// that the actual interpretation of this value is specific to the
		// particular CPersistentStorage flavor being used
		static __COMPACT_STOCK uint16_t wWorkingAreaSize;

		// Size of factory storage area within non-volatile storage. Notice,
		// that the actual interpretation of this value is specific to the
		// particular CPersistentStorage flavor being used
		static __COMPACT_STOCK uint16_t wFactoryAreaSize;

		// Address of factory storage area within non-volatile storage. Notice,
		// that the actual interpretation of this value is specific to the
		// particular CPersistentStorage flavor being used
		static __COMPACT_STOCK uint32_t dwFactoryAreaAddress;

		// Address of retain area within non-volatile storage, which is used
		// as a temporary storage during safe factory resets to guard against
		// potential power failures during the process
		static __COMPACT_DEFAULT_STOCK uint32_t backupAreaAddress;

		// Size of retain area within non-volatile storage, which is used
		// as a temporary storage during safe factory resets to guard against
		// potential power failures during the process
		static __COMPACT_DEFAULT_STOCK uint16_t backupAreaSize;

		// Default storage instance, i.e. the working storage
		CPersistentStorage m_default;

		// Pointer to the currently active storage instance, which enables
		// switching between storage instances, e.g. during factory resets
		CPersistentStorage *m_instance;

	public:
		// Must be called once after start-up or e.g. after a factory reset.
		bool Initialize();

		// Updates page information, including statistics.
		bool RefreshPageInfo(const bool includeVolatile = true,
			const bool entire = true);

		// Returns true, if the storage is valid
		bool IsValid() const;

		// Clears the storage and prepares it for adding entries
		bool Format();

		// Makes a unique item
		bool MakeUniqueItem(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbCommon,
			const CPersistentStorageItem::size_type cbUnique = 1,
			bool *const existing = 0,
			const CPersistentStorage::PREDICATE predicate = 0,
			void *const argument = 0);

		// Attempts to locate the specified item and updates the size and
		// capacity members of the item record, if an item was found
		bool Lookup(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem);

		// Allocates storage for a new data item.
		bool Allocate(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem,
			const void *const pData = 0,
			const CPersistentStorageItem::size_type cbData = 0,
			const uint_fast8_t nFlagsEx = 0);

		// Deletes a particular item from the item storage
		bool Free(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem);

		// Deletes matching items from the item storage.
		bool FreeEx(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem);

		// Retrieves the contents of the specified data item
		bool Load(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem,
			void *const pData, const CMemoryDevice::size_type cbData,
			CMemoryDevice::size_type *const pcbData = 0);

		// Stores a specific item with optional extended flags
		bool Store(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem,
			const void *const pData,
			const CPersistentStorageItem::size_type cbData,
			const uint_fast8_t nFlagsEx = 0);

		// Stores a specific item, which may have been allocated before or
		// will be allocated automatically.
		bool StoreEx(CPersistentStorageItem &item,
			const CPersistentStorageItem::size_type cbItem,
			const void *const pData,
			const CPersistentStorageItem::size_type cbData,
			const uint8_t nFlagsEx = 0,
			const CPersistentStorageItem::size_type cbCapacity = 0);

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

		// Convert a zigbee storage object to its underlying persistent
		// storage object
		operator CPersistentStorage &() const;

		// Persistently stores factory reset progress during a factory reset
		// sequence. This guards the factory reset sequence against power-
		// failures and allows the device to detect an incomplete factory
		// reset and recover without loss of data that is expected to be
		// preserved across factory resets.
		void StoreFactoryResetProgress(const uint8_t state = 0);

		// Returns true if a factory reset is in progress, i.e. has been
		// started but not completed. For example, the process might have been
		// interrupted due to a power cycle
		bool IsFactoryResetInProgress();

#ifdef _DEBUG
	public:
		// Dumps storage info
		void Dump(const bool dumpItems = true);
#endif

};


inline CZigBeeStorage::operator CPersistentStorage &() const
{
	ASSERT(m_instance);
	return *m_instance;
}


inline bool CZigBeeStorage::Initialize()
{
	return m_instance->Initialize();
}


inline bool CZigBeeStorage::RefreshPageInfo(const bool includeVolatile,
	const bool entire)
{
	return m_instance->RefreshPageInfo(includeVolatile, entire);
}


inline bool CZigBeeStorage::IsValid() const
{
	return m_instance->IsValid();
}


inline bool CZigBeeStorage::Format()
{
	return m_instance->Format();
}


inline bool CZigBeeStorage::MakeUniqueItem(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbCommon,
	const CPersistentStorageItem::size_type cbUnique,
	bool *const existing, const CPersistentStorage::PREDICATE predicate,
	void *const argument)
{
	return m_instance->MakeUniqueItem(item, cbCommon, cbUnique, existing,
		predicate, argument);
}


inline bool CZigBeeStorage::Lookup(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem)
{
	return m_instance->Lookup(item, cbItem);
}


inline bool CZigBeeStorage::Allocate(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem, const void *const pData,
	const CPersistentStorageItem::size_type cbData,
	const uint_fast8_t nFlagsEx)
{
	return m_instance->Allocate(item, cbItem, pData, cbData, nFlagsEx);
}


inline bool CZigBeeStorage::Free(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem)
{
	return m_instance->Free(item, cbItem);
}


inline bool CZigBeeStorage::FreeEx(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem)
{
	return m_instance->FreeEx(item, cbItem);
}


inline bool CZigBeeStorage::Load(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem,
	void *const pData, const CMemoryDevice::size_type cbData,
	CMemoryDevice::size_type *const pcbData)
{
	return m_instance->Load(item, cbItem, pData, cbData, pcbData);
}


inline bool CZigBeeStorage::Store(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem, const void *const pData,
	const CPersistentStorageItem::size_type cbData,
	const uint_fast8_t nFlagsEx)
{
	return m_instance->Store(item, cbItem, pData, cbData, nFlagsEx);
}


inline bool CZigBeeStorage::StoreEx(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem,
	const void *const pData, const CPersistentStorageItem::size_type cbData,
	const uint8_t nFlagsEx, const CPersistentStorageItem::size_type cbCapacity)
{
	return m_instance->StoreEx(item, cbItem, pData, cbData, nFlagsEx,
		cbCapacity);
}


inline bool CZigBeeStorage::LoadVolatile(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem, void *const pData,
	const CPersistentStorageItem::size_type cbData,
	const uint_fast8_t nOptions)
{
	return m_instance->LoadVolatile(item, cbItem, pData, cbData, nOptions);
}


inline bool CZigBeeStorage::StoreVolatile(CPersistentStorageItem &item,
	const CPersistentStorageItem::size_type cbItem, const void *const pData,
	const CPersistentStorageItem::size_type cbData,
	const uint_fast8_t nOptions)
{
	return m_instance->StoreVolatile(item, cbItem, pData, cbData, nOptions);
}


#ifdef _DEBUG

inline void CZigBeeStorage::Dump(const bool dumpItems)
{
	m_instance->Dump(dumpItems);
}

#endif


/////////////////////////////////////////////////////////////////////////////
// The one and only CZigBeeStorage instance

extern CZigBeeStorage theStorage;
