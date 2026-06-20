// PersistentVolatile1.h : header file
//
// Copyright(C) 2013-2019 ubisys technologies, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This file is part of the CompactStorage(TM) 1.0 library and complements the
// persistent storage classes by providing a class featuring wear-leveling
// for non-volatile memories with limited erase/write cycles, like typical
// on-chip flash memories

/////////////////////////////////////////////////////////////////////////////
// CPersistentVolatile1

class CPersistentVolatile1
{
	// typedefs & enums
	public:
		// Size of sequence ring buffer entries
		typedef unsigned char sequence_t;

		// Ring buffer entry format. Lower six bit are the sequence number,
		// the upper two bits are reserved for future use. These could be used
		// to distinguish a differential bitmap instead of an absolute value.
		// Each bit in the bitmap, if cleared, indicates that the previous
		// absolute value is incremented by one. At least one absolute value
		// would be required in the complete sequence. The location of which
		// should alternate equally within the storage space
		enum { sequenceNumberMask = 0x3f };

		// Notice: The actual slot count is 2^(n+1), where n is the value
		// stored in the sub-field of options. This implementation supports
		// 4, 8, 16, 32 and 64 slots, which can be encoded using 5 bit sequence
		// numbers, leaving three bits for additional per-slot options
		enum { size8, size16, size32, size64, sizeMask = 0x03,
			slots4 = 0x04, slots8 = 0x08, slots16 = 0x0c, slots32 = 0x10,
			slots64 = 0x14, slotCountShift = 2, slotCountMask = 0x1c };

	// Construction
	public:
		CPersistentVolatile1(const unsigned char nOptions);

	// Attributes
	public:
		// Option flags for the volatile value storage
		unsigned char m_nOptions;

	// Operations
	public:
		// Returns the number of bytes required to store a value of the size
		// and wear-leveling depth specified in m_nOptions
		CPersistentStorage1Item::size_type GetRequiredCapacity() const;

		// Stores the volatile value using the item in the supplied storage.
		// Any 8/16/32/64-bit wide data type can be stored using the template
		// function
		template<class T>
		bool Store(CPersistentStorage1 &storage, CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem, const T value) const;

		// Loads the volatile value using the item in the supplied storage.
		// Any 8/16/32/64-bit wide data type can be retrieved using the
		// template function
		template<class T>
		bool Load(CPersistentStorage1 &storage, CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem, T &value) const;

	// Implementation
	public:
		// Returns the number of bytes required to store a value of the size
		// and wear-leveling depth specified in nOptions
		static CPersistentStorage1Item::size_type
			GetRequiredCapacity(const unsigned char nOptions);

		// Stores the volatile value using the item in the supplied storage
		bool Store(CPersistentStorage1 &storage, CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			const void *const pValue,
			const CPersistentStorage1Item::size_type cbValue) const;

		// Loads the volatile value using the item in the supplied storage
		bool Load(CPersistentStorage1 &storage, CPersistentStorage1Item &item,
			const CPersistentStorage1Item::size_type cbItem,
			void *const pValue,
			const CPersistentStorage1Item::size_type cbValue) const;

		// Finds the most recently used (current) sequence number. Increment
		// by one to obtain the next one that is available for new items.
		bool GetSequence(const unsigned char *const pbValue,
			const CPersistentStorage1Item::size_type cbValue,
			sequence_t &nCurrentSequence) const;

		// Returns the current value of the persistent volatile, given the
		// raw data block that is stored persistently
		bool GetValue(const unsigned char *const pbRawValue,
			const CPersistentStorage1Item::size_type cbRawValue,
			void *const pValue,
			const CPersistentStorage1Item::size_type cbValue) const;

		// Sets the current value of the persistent volatile, given the
		// raw data block that is stored persistently and the new value
		bool SetValue(unsigned char *const pbRawValue,
			const CPersistentStorage1Item::size_type cbRawValue,
			const void *const pValue,
			const CPersistentStorage1Item::size_type cbValue) const;

		// Returns an offset to the slot value storage space within the raw
		// data block given its slot index
		CPersistentStorage1Item::size_type
		CalculateSequenceOffset(const sequence_t nSequence) const;

		// Returns an offset to the slot value storage space within the raw
		// data block given its slot index
		CPersistentStorage1Item::size_type
		CalculateValueOffset(const sequence_t nSequence) const;

		// Returns the number of slots maintained in the raw data block
		static sequence_t GetSlotCount(const unsigned char nOptions);
};

inline CPersistentStorage1Item::size_type
CPersistentVolatile1::GetRequiredCapacity() const
	{ return GetRequiredCapacity(m_nOptions); }

template<class T>
inline bool CPersistentVolatile1::Store(CPersistentStorage1 &storage,
	CPersistentStorage1Item &item,
	const CPersistentStorage1Item::size_type cbItem, const T value) const
{
	STATIC_ASSERT((sizeof(T) == sizeof(unsigned char)) ||
		(sizeof(T) == sizeof(unsigned short)) ||
		(sizeof(T) == sizeof(unsigned long)) ||
		(sizeof(T) == sizeof(unsigned long long)),
		"CPersistentVolatile1::Store<T>() can write 8/16/32/64 bit only.");

	return Store(storage, item, cbItem, &value, sizeof(value));
}

template<class T>
inline bool CPersistentVolatile1::Load(CPersistentStorage1 &storage,
	CPersistentStorage1Item &item,
	const CPersistentStorage1Item::size_type cbItem, T &value) const
{
	STATIC_ASSERT((sizeof(T) == sizeof(unsigned char)) ||
		(sizeof(T) == sizeof(unsigned short)) ||
		(sizeof(T) == sizeof(unsigned long)) ||
		(sizeof(T) == sizeof(unsigned long long)),
		"CPersistentVolatile1::Load<T>() can read 8/16/32/64 bit only.");

	return Load(storage, item, cbItem, &value, sizeof(value));
}
