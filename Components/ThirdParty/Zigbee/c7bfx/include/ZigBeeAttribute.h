// ZigBeeAttribute.h : header file
//
// Copyright(C) 2009-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Provides an application foundation that suites ZigBee Cluster Library
// paradigms and provides support for attributes with global scope,
// application class scope, or application instance scope. Supports multiple
// instances of the same application (for example multi-channel load switch
// providing multiple endpoints), and a single application serving multiple
// end-points

/////////////////////////////////////////////////////////////////////////////
// forward referenced classes

class CZigBeeCluster;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeTime

class CZigBeeAttributeTime
{
	// Attributes
	public:
		uint8_t m_nHours;
		uint8_t m_nMinutes;
		uint8_t m_nSeconds;
		uint8_t m_nHundredths;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeDate

class CZigBeeAttributeDate
{
	// Attributes
	public:
		uint8_t m_nYearMinus1900;
		uint8_t m_nMonth;
		uint8_t m_nDay;
		uint8_t m_nWeekDay;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeArray

#pragma pack(push, 1)

class CZigBeeAttributeArray
{
	// Attributes
	public:
		// Data type of elements
		uint8_t m_nType;

		// Number of elements, 0xffff means the array has not been initialized
		// and is empty
		uint16_t m_nElements;

		// Variable length element data follows

	// Operations
	public:
		// Returns a pointer to an element, starting with index 0 for the first
		// element. For simple types (fixed length) the operation uses pointer
		// arithmetic. For complex types (raw data, strings, arrays, sets,
		// bags, structures) each call results in all previous elements being
		// parsed
		void *GetElementAt(const uint_fast16_t nIndex) const;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeBag

typedef CZigBeeAttributeArray CZigBeeAttributeBag;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeSet

typedef CZigBeeAttributeBag CZigBeeAttributeSet;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeStructure

#pragma pack(push, 1)

class CZigBeeAttributeStructure
{
	// Attributes
	public:
		uint16_t m_nElements;

		// Variable length element data follows
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttribute

class CZigBeeAttribute
{
	// typedefs & enums
	public:
		// Attribute types
		enum { typeNull, typeData8 = 0x08, typeData16, typeData24, typeData32,
			typeData40, typeData48, typeData56, typeData64, typeBoolean = 0x10,
			typeBitmap8 = 0x18, typeBitmap16, typeBitmap24, typeBitmap32,
			typeBitmap40, typeBitmap48, typeBitmap56, typeBitmap64,
			typeUnsigned8 = 0x20, typeUnsigned16, typeUnsigned24,
			typeUnsigned32, typeUnsigned40, typeUnsigned48, typeUnsigned56,
			typeUnsigned64, typeSigned8 = 0x28, typeSigned16, typeSigned24,
			typeSigned32, typeSigned40, typeSigned48, typeSigned56,
			typeSigned64, typeEnumeration8 = 0x30, typeEnumeration16,
			typeFloatingPoint16 = 0x38, typeFloatingPoint32,
			typeFloatingPoint64, typeRawData = 0x41, typeString,
			typeLongRawData, typeLongString, typeArray = 0x48,
			typeStructure = 0x4c, typeSet = 0x50, typeBag = 0x51,
			typeTime = 0xe0, typeDate, typeTimeUTC, typeClusterID = 0xe8,
			typeAttributeID, typeBACnetOID, typeIEEEAddress = 0xf0,
			typeSecurityKey = 0xf1, typeUnknown = 0xff };

		// Attribute flags (attribute scope, access type, manufacturer-specific
		// vs. standard attribute, attribute preserved across simple factory
		// resets, dynamically allocated vs. statically allocated value space,
		// reportable attribute?, send gratuitous reports for this attribute?,
		// persistent attributes and if so a frequently changing one). Notice
		// that volatile options were designed for Persistent Storage 1.0;
		// Persistent Storage 2.0 does not require this information any longer
		enum { scopeGlobal = 0x00, scopeClass = 0x01, scopeCluster = 0x02,
			scopeMask = 0x03, accessReadOnly = 0x04,
			manufacturerSpecific = 0x08, preserve = 0x10, dynamic = 0x20,
			persistent = 0x40, reportable = 0x80, volatileOptionMask = 0x1f00,
			volatileOptionShift = 8, storageVolatile = 0x2000,
			storageVolatile4x1 = storageVolatile | 0x0400,
			storageVolatile4x2 = storageVolatile | 0x0500,
			storageVolatile4x4 = storageVolatile | 0x0600,
			storageVolatile4x8 = storageVolatile | 0x0700,
			storageVolatile8x1 = storageVolatile | 0x0800,
			storageVolatile8x2 = storageVolatile | 0x0900,
			storageVolatile8x4 = storageVolatile | 0x0a00,
			storageVolatile8x8 = storageVolatile | 0x0b00,
			storageVolatile16x1 = storageVolatile | 0x0c00,
			storageVolatile16x2 = storageVolatile | 0x0d00,
			storageVolatile16x4 = storageVolatile | 0x0e00,
			storageVolatile16x8 = storageVolatile | 0x0f00,
			storageVolatile32x1 = storageVolatile | 0x1000,
			storageVolatile32x2 = storageVolatile | 0x1100,
			storageVolatile32x4 = storageVolatile | 0x1200,
			storageVolatile32x8 = storageVolatile | 0x1300,
			storageVolatile64x1 = storageVolatile | 0x1400,
			storageVolatile64x2 = storageVolatile | 0x1500,
			storageVolatile64x4 = storageVolatile | 0x1600,
			storageVolatile64x8 = storageVolatile | 0x1700,
			persistentVolatile4x1 = persistent | storageVolatile4x1,
			persistentVolatile4x2 = persistent | storageVolatile4x2,
			persistentVolatile4x4 = persistent | storageVolatile4x4,
			persistentVolatile4x8 = persistent | storageVolatile4x8,
			persistentVolatile8x1 = persistent | storageVolatile8x1,
			persistentVolatile8x2 = persistent | storageVolatile8x2,
			persistentVolatile8x4 = persistent | storageVolatile8x4,
			persistentVolatile8x8 = persistent | storageVolatile8x8,
			persistentVolatile16x1 = persistent | storageVolatile16x1,
			persistentVolatile16x2 = persistent | storageVolatile16x2,
			persistentVolatile16x4 = persistent | storageVolatile16x4,
			persistentVolatile16x8 = persistent | storageVolatile16x8,
			persistentVolatile32x1 = persistent | storageVolatile32x1,
			persistentVolatile32x2 = persistent | storageVolatile32x2,
			persistentVolatile32x4 = persistent | storageVolatile32x4,
			persistentVolatile32x8 = persistent | storageVolatile32x8,
			persistentVolatile64x1 = persistent | storageVolatile64x1,
			persistentVolatile64x2 = persistent | storageVolatile64x2,
			persistentVolatile64x4 = persistent | storageVolatile64x4,
			persistentVolatile64x8 = persistent | storageVolatile64x8,
			gratuitousReports = 0x4000
		};

		// Attribute access control policy (same as overall application and
		// cluster policies)
		enum { accessUnrestricted, accessPeerOnly, accessTrustCenterOnly,
			accessInherit };

		// Attribute access types
		enum { accessWrite, accessRead, accessReporting };

		// Attribute read, write and reporting access restrictions (could add
		// discovery access restrictions in the future if required)
		enum { accessMask = 3, accessWriteShift = accessWrite << 1,
			accessWriteMask = accessMask << accessWriteShift,
			accessWriteUnrestricted = accessUnrestricted << accessWriteShift,
			accessWritePeerOnly = accessPeerOnly << accessWriteShift,
			accessWriteTrustCenterOnly =
				accessTrustCenterOnly << accessWriteShift,
			accessWriteInherit = accessInherit << accessWriteShift,
			accessReadShift = accessRead << 1,
			accessReadMask = accessMask << accessReadShift,
			accessReadUnrestricted = accessUnrestricted << accessReadShift,
			accessReadPeerOnly = accessPeerOnly << accessReadShift,
			accessReadTrustCenterOnly =
				accessTrustCenterOnly << accessReadShift,
			accessReadInherit = accessInherit << accessReadShift,
			accessReportingShift = accessReporting << 1,
			accessReportingMask = accessMask << accessReportingShift,
			accessReportingUnrestricted =
				accessUnrestricted << accessReportingShift,
			accessReportingPeerOnly =
				accessPeerOnly << accessReportingShift,
			accessReportingTrustCenterOnly =
				accessTrustCenterOnly << accessReportingShift,
			accessReportingInherit = accessInherit << accessReportingShift,
			accessDefault = accessWriteInherit | accessReadInherit |
				accessReportingInherit };

	// Construction
	// Objects of this class are constructed from initializer lists. Therefore
	// no constructor must be declared at all

	// Attributes
	public:
		// Identifier of this attribute
		uint16_t m_wAttributeID;

		// In case of a manufacturer-specific attribute, this member holds the
		// manufacturer ID
		uint16_t m_wManufacturerCode;

		// Options (a combination of above values)
		uint16_t m_nOptions;

		// Access control options
		uint8_t m_nAccessControl;

		// Type of this attribute
		uint8_t m_nType;

		// Pointer to the attribute value storage location. The exact type
		// depends on the scope option (absolute address or offset within
		// particular cluster object). Declared as const here to support
		// in-flash attributes, but might be const_cast<>, if not declared as
		// read only. If the "dynamic" option is set, this pointer addresses
		// a pointer to a std::pair<void *, ptrdiff_t> identifying a block of
		// memory dynamically allocated from the heap with operator new[],
		// which might also be null (empty value)
		const void *m_pStorage;

		// Pointer to an attribute value range structure (minimum and maximum
		// values, bit masks, octet or character string size limits, etc. May
		// be null (no range limits beyond those inherent to the attribute
		// type)
		const void *m_pRangeInfo;

	// Operations
	public:
		// Returns a pointer to the actual attribute storage space (for all
		// kinds of attributes). Might return null for dynamic attributes, if
		// no storage has been allocated
		void *GetData(const CZigBeeCluster &cluster) const;

		// Returns a pointer to the actual attribute storage space (for global
		// and class-scope attributes only). Might return null for dynamic
		// attributes, if no storage has been allocated
		void *GetData() const;

		// Same as above, with an optional CZigBeeCluster instance
		void *GetData(const CZigBeeCluster *) const;

		// Determines the size, in bytes, required to store a value of the
		// given attribute type. For variable-length attributes, the minimum
		// size is returned unless a pointer to the value storage is provided,
		// together with the minimum size of accessible bytes, in which case
		// the actual size is returned
		static bool GetSize(CPacketData::size_type &nSize,
			const uint_fast8_t nType, const void *const pValue = 0,
			const CPacketData::size_type cbValue = -1);

		// Returns the size, in bytes, of the storage space required to store
		// this attribute's value
		bool GetSize(const CZigBeeCluster &cluster,
			CPacketData::size_type &nSize) const;

		// Determine the data type and length in bytes for a structured element,
		// in accordance to the given selector index. Return false if no valid
		// element can be found.
		static bool GetStructuredElement(const void *& element, uint8_t &type,
			CPacketData::size_type &size, const uint_fast8_t attributeType,
			const void *const pIndex, const CPacketData::size_type cbIndex,
			const void *const pValue);

		// Checks if the specified value would be a valid value for the given
		// data type. Performs range checks, if range info is present
		static bool IsValid(const uint8_t type, const uint16_t options,
			const void *const value, const CPacketData::size_type length,
			const void *rangeInfo = 0);

		// Checks if the specified value would be a valid value for this
		// attribute. Performs range checks, if range info is present
		bool IsValid(const void *const pValue,
			const CPacketData::size_type cbValue) const;

		// Assigns new data to this attribute. This data is typically obtained
		// as a result of an incoming write attributes command. No checks are
		// performed, so IsValid() should be called before assigning new data
		// Optionally inhibits updates of persistent storage. Returns true,
		// only if the attribute data has indeed changed.
		bool SetData(const CZigBeeCluster &cluster, const void *const pValue,
			const CPacketData::size_type cbValue,
			const bool bAllowStore = true) const;

		// Same as above, but for static/global attributes when no cluster
		// instance is available
		bool SetData(const void *const pValue,
			const CPacketData::size_type cbValue) const;

		// Allocates or reallocates heap storage (only applicable to attributes
		//	with the "dynamic" flag set)
		void AllocateStorage(const CZigBeeCluster &cluster,
			const CPacketData::size_type cbValue) const;

		// Releases allocated heap storage (only applicable to attributes with
		// the "dynamic" flag set). Do not call GetData() afterwards. You must
		// first call SetData() or Load() to reallocate storage
		void ReleaseStorage(const CZigBeeCluster &cluster) const;

		// Extends a signed or unsigned integer of the specified width to an
		// integer of broader width. Can be used to store a 48-bit value in
		// a 64-bit integer, for example. The value must have been copied
		// to the data storage location before
		static void Extend(void *const pValue,
			const CPacketData::size_type cbValue, const bool bSigned);

		// Returns true, if the specified data type is an integer (signed or
		// unsigned)
		static bool IsInteger(const uint_fast8_t nType);

		// Returns true, if the specified data type is a signed integer
		static bool IsSignedInteger(const uint_fast8_t nType);

		// Returns true, if the specified data type is a floating point value
		static bool IsFloatingPoint(const uint_fast8_t nType);

		// Returns true, if the specified data type is "analog". Otherwise it
		// is "discrete"
		static bool IsAnalog(const uint_fast8_t nType);

		// Returns true, if the specified data type is variable length, else
		// all possible values occupy the same amount of storage space
		static bool IsVariableLength(const uint_fast8_t nType);

		// Returns true, if the attribute is an integer (signed or unsigned)
		bool IsInteger() const;

		// Returns true, if the attribute is a signed integer
		bool IsSignedInteger() const;

		// Returns true, if the attribute is a floating point value
		bool IsFloatingPoint() const;

		// Returns true, if the attribute is an "analog" value. Otherwise it
		// is "discrete"
		bool IsAnalog() const;

		// Returns true, if the specified data type is variable length, else
		// all possible values occupy the same amount of storage space
		bool IsVariableLength() const;

		// Returns true, if an analog value has changed at least a certain
		// amount compared to a reference value. Can be used to implement
		// reporting for analog values ("reportable change")
		bool HasChanged(const CZigBeeCluster &cluster,
			const void *const pReferenceValue,
			const CPacketData::size_type cbReferenceValue,
			const void *const pThreshold,
			const CPacketData::size_type cbThreshold) const;

		// Returns the size of a buffer, in bytes, required to store any value
		// of the given attribute type. For variable-length attributes, the
		// maximum size is returned
		static bool GetRequiredCapacity(CPacketData::size_type &nCapacity,
			const uint_fast8_t nType);

		// Returns the size of a storage space, in bytes, required to store any
		// value of the given attribute type. For variable-length attributes,
		// the maximum size is returned. For integers the natural size is
		// returned
		static bool GetRequiredCapacityEx(CPacketData::size_type &nCapacity,
			const uint_fast8_t nType);

		// Returns the size of a buffer, in bytes, required to store an allowed
		// value, i.e. regarding the specific range information associated with
		// the particular attribute. If no range information is available, the
		// generally required (worst-case) size is returned
		bool GetRequiredCapacity(CPacketData::size_type &nCapacity) const;

		// Returns the size of a buffer, in bytes, required to store an allowed
		// value, i.e. regarding the specific range information associated with
		// the particular attribute. If no range information is available, the
		// size required to store its current value is returned
		bool GetRequiredCapacity(CPacketData::size_type &nCapacity,
			const CZigBeeCluster &cluster) const;

		// Returns the size of a integer suitable to store a packed integer of
		// the specified width in a CPU friendly format, i.e. a 24-bit integer
		// would be stored in 32-bits, a 56-bit integer int 64-bits etc. The
		// resulting size is the next power of two greater than or equal to
		// the specified original size
		static CPacketData::size_type GetIntegerCapacity
			(const CPacketData::size_type cbPacked);

		// Creates an array on the heap and returns a pointer to the first
		// element. The original array must contain valid data. The attribute
		// must have the "dynamic" option flag set
		void *CreateArray(const CZigBeeCluster &cluster,
			const CZigBeeAttributeArray &array) const;

		// Creates an array on the heap and returns a pointer to the first
		// element. If the original array has elements specify cbElements as
		// the total number of bytes occupied by the elements. The storage for
		// this array is capable of storing cbCapacity bytes of overall element
		// data. The attribute must have the "dynamic" option flag set
		void *CreateArray(const CZigBeeCluster &cluster,
			const CZigBeeAttributeArray &array,
			const CPacketData::size_type cbElements,
			const CPacketData::size_type cbCapacity) const;

		// Creates a simple array on the heap and returns a pointer to the
		// first element. The attribute must have the "dynamic" option flag set
		void *CreateArray(const CZigBeeCluster &cluster,
			const uint_fast8_t nType = typeUnknown,
			const uint_fast16_t nElements = 0,
			const CPacketData::size_type cbElement = 0) const;

		// Writes a value for the attribute to persistent storage (non-volatile
		// memory). If the attribute type is of variable length (string, raw
		// binary), the capacity reserved for future value changes can be
		// specified as well. The value might be different than the current
		// value for this attribute
		bool StoreEx(const CZigBeeCluster &cluster,
			const void *const pValue,
			const CPacketData::size_type cbCapacity =
				static_cast<CPacketData::size_type>(-1)) const;

		// Writes this attribute to persistent storage (non-volatile memory).
		// If the attribute type is of variable length (string, raw binary),
		// the capacity reserved for future value changes can be specified as
		// well. This is equivalent to calling StoreEx() for the attribute's
		// current value
		bool Store(const CZigBeeCluster &cluster,
			const CPacketData::size_type cbCapacity =
				static_cast<CPacketData::size_type>(-1)) const;

		// Loads this attribute from persistent storage (non-volatile memory).
		// If you want potential attribute changes to be reported, call
		// OnAttributeChanged() afterwards
		bool Load(const CZigBeeCluster &cluster) const;

		// Get the default value for a given data type
		static CPacket GetDefault(const uint_fast8_t type);

	// Implementation
	public:
		// Granularity for dynamic attributes. This should match the actual
		// granularity of heap blocks on the platform and is typically 2^3 = 8,
		// or 2^4 = 16 bytes
		enum { granularity = 3 };

		// Template function that checks if an attribute value is in its
		// specified range. Can be used to query if a write operation can
		// proceed or should be denied
		template<class T>
		static bool ValidateRange(const void *const pValue,
			const CPacketData::size_type cbValue,
			const void *const pRangeInfo);

		// Template function that checks if a bit-field type attribute value is
		// in its specified range. Can be used to query if a write operation
		// can proceed or should be denied
		template<class T>
		static bool ValidateMask(const void *const pValue,
			const CPacketData::size_type cbValue,
			const void *const pRangeInfo);

		// Template function that checks if a bit-field type attribute value is
		// in its specified range. Can be used to query if a write operation
		// can proceed or should be denied
		template<class T>
		static bool ValidateCapacity(const uint16_t options,
			const void *const pValue, const CPacketData::size_type cbValue,
			const void *const pRangeInfo);

		// Template function that checks if an analog attribute value differs
		// a certain minimum amount when compared to another value
		template<class T>
		static bool HasChanged(const void *const pValue1,
			const void *const pValue2, const void *const pThreshold);

#ifdef _DEBUG
		// Dumps this attribute (for per-instance attributes)
		void Dump(const CZigBeeCluster &cluster) const;

		// Dumps this attribute (only for static/global attributes)
		void Dump() const;

		// Dumps this attribute (common for per-instance and static/global)
		void CommonDump() const;
#endif

	// private implementation
	private:
		// Shared implementation for SetData() (with and without cluster instance)
		bool SetData(const CZigBeeCluster *cluster, const void *const pValue,
			const CPacketData::size_type cbValue,
			const bool bAllowStore = true) const;

		// Same as AllocateStorage(const CZigBeeCluster &,
		//     const CPacketData::size_type ) const, but with an optional
		// cluster instance
		void AllocateStorage(const CZigBeeCluster *cluster,
			const CPacketData::size_type cbValue) const;
};


inline bool CZigBeeAttribute::GetSize(const CZigBeeCluster &cluster,
	CPacketData::size_type &nSize) const
{
	return GetSize(nSize, m_nType, GetData(cluster));
}


inline bool CZigBeeAttribute::IsInteger() const
{
	return IsInteger(m_nType);
}


inline bool CZigBeeAttribute::IsSignedInteger() const
{
	return IsSignedInteger(m_nType);
}


inline bool CZigBeeAttribute::IsFloatingPoint() const
{
	return IsFloatingPoint(m_nType);
}


inline bool CZigBeeAttribute::IsAnalog() const
{
	return IsAnalog(m_nType);
}


inline bool CZigBeeAttribute::IsVariableLength() const
{
	return IsVariableLength(m_nType);
}
