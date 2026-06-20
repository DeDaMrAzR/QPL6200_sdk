// ZigBeeCluster.h : header file
//
// Copyright(C) 2009-2024 ubisys technologies GmbH, Duesseldorf, Germany.
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

class CZigBeeApplicationSupportBase;
class CZigBeeApplication;
class CZigBeeFoundationApplication;
class CZigBeeReportingStorageItem;
class CZigBeeCluster;
class CZigBeeStorage;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterLibraryHeader

#pragma pack(push, 1)

class CZigBeeClusterLibraryHeader
{
	// typedefs & enums
	public:
		// Frame control field values
		enum { typeProfile, typeCluster, maskType = 0x03,
			manufacturerSpecific = 0x04, directionServerToClient = 0x08,
			disableDefaultResponse = 0x10 };

	// Attributes
	public:
		// Frame Control Field, 2.3.1.1
		unsigned char m_bFrameControl;

		// Manufacturer code, optional, 2.3.1.2
		// Transaction sequence number, 2.3.1.3
		// Command identifier, 2.3.1.4

	// Operations
	public:
		// Returns the size of this ZCL header
		CPacketData::size_type GetSize() const;

		// Returns the frame type
		unsigned char GetType() const;

		// Returns true if general or cluster command is manufacturer-specific
		bool IsManufacturerSpecific() const;

		// Returns Manufacturer Code field
		bool GetManufacturerCode(unsigned short &wManufacturerCode) const;

		// Returns Transaction Sequence number
		unsigned char GetSequence() const;

		// Returns Command identifier number
		unsigned char GetCommand() const;
};

#pragma pack(pop)

inline CPacketData::size_type CZigBeeClusterLibraryHeader::GetSize() const
	{ return (m_bFrameControl & manufacturerSpecific) ? 5 : 3; }

inline unsigned char CZigBeeClusterLibraryHeader::GetType() const
	{ return m_bFrameControl & maskType; }

inline bool CZigBeeClusterLibraryHeader::IsManufacturerSpecific() const
	{ return (m_bFrameControl & manufacturerSpecific) ? true : false; }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterLibraryDefaultResponse

#pragma pack(push, 1)

class CZigBeeClusterLibraryDefaultResponse
{
	// Attributes
	public:
		// Command identifier field, 2.4.12.1.2
		unsigned char m_bCommand;

		// Status code field, 2.4.12.1.3
		unsigned char m_bStatus;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeReadStatusRecord

#pragma pack(push, 1)

class CZigBeeAttributeReadStatusRecord
{
	// Attributes
	public:
		// Attribute identifier field, 2.4.2.1.2
		unsigned short m_wAttributeID;

		// Status field, 2.4.2.1.3
		unsigned char m_bStatus;

		// Optional attribute data type field, 2.4.2.1.4
		// Optional/variable length attribute data field, 2.4.2.1.5
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeWriteRecord

#pragma pack(push, 1)

class CZigBeeAttributeWriteRecord
{
	// Attributes
	public:
		// Attribute identifier field, 2.4.3.1.2
		unsigned short m_wAttributeID;

		// Attribute data type field, 2.4.3.1.3
		unsigned char m_bType;

		// Variable length attribute data field, 2.4.3.1.4
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeWriteStatusRecord

#pragma pack(push, 1)

class CZigBeeAttributeWriteStatusRecord
{
	// Attributes
	public:
		// Status field, 2.4.5.1.2
		unsigned char m_bStatus;

		// Attribute identifier field, 2.4.3.1.3, might be omitted in case of
		// success
		unsigned short m_wAttributeID;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeStructuredRecord

#pragma pack(push, 1)

class CZigBeeAttributeStructuredRecord
{
	// Attributes
	public:
		// Attribute identifier field, 2.5.15.1.2
		uint16_t m_attributeID;

		// Selector field, indicator sub-field, 2.5.15.1.3
		uint8_t m_indicator;

		// Selector field, optional index sub-field(s), 2.5.15.1.3
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeReportRecord

typedef CZigBeeAttributeWriteRecord CZigBeeAttributeReportRecord;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeReportingConfigurationRecord

#pragma pack(push, 1)

class CZigBeeAttributeReportingConfigurationRecord
{
	// Attributes
	public:
		// Direction field, 2.4.7.1.2
		unsigned char m_bDirection;

		// Attribute identifier field, 2.4.7.1.3
		unsigned short m_wAttributeID;

		// Variable length attribute data type, minimum/maximum reporting
		// interval, reportable change (direction = 0) OR timeout period
		// (direction = 1)
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeReportingConfigurationRecord0 - direction = 0/2

#pragma pack(push, 1)

class CZigBeeAttributeReportingConfigurationRecord0 :
	public CZigBeeAttributeReportingConfigurationRecord
{
	// Attributes
	public:
		// Contains the data type of the attribute that is to be reported
		unsigned char m_bType;

		// Contains the minimum interval, in seconds, between issuing reports
		// of the specified attribute. If this value is set to 0x0000, then
		// there is no minimum limit, unless one is imposed out-of-band
		unsigned short m_wMinimumInterval;

		// Contains the maximum interval, in seconds, between issuing reports
		// of the specified attribute. If this value is set to 0xffff, then
		// the device shall not issue reports for the specified attribute, and
		// the configuration information for that attribute need not be
		// maintained, i.e. cancel reporting
		unsigned short m_wMaximumInterval;

		// Variable length reportable change field. Contain the minimum change
		// to the attribute that will result in a report being issued. For
		// attributes with "analog" data type the field has the same data type
		// as the attribute. For attributes of "discrete" data type this field
		// is omitted
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeReportingConfigurationRecord1 - direction = 1/3

#pragma pack(push, 1)

class CZigBeeAttributeReportingConfigurationRecord1 :
	public CZigBeeAttributeReportingConfigurationRecord
{
	// Attributes
	public:
		// Contains the maximum expected time, in seconds, between received
		// reports for the attribute specified in he attribute identifier
		// field. If more time than this elapses between reports, this may be
		// an indication that there is a problem with reporting. If this value
		// is set to 0x0000, reports of the attribute are not subject to
		// timeout
		unsigned short m_wTimeout;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeStatusRecord

#pragma pack(push, 1)

class CZigBeeAttributeStatusRecord
{
	// Attributes
	public:
		// Status field, 2.4.8.1.3
		unsigned char m_bStatus;

		// Optional in case of success: Direction field, 2.4.8.1.2
		unsigned char m_bDirection;

		// Optional in case of success: Attribute identifier field,
		// 2.4.3.1.3
		unsigned short m_wAttributeID;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeDiscoveryRecord

#pragma pack(push, 1)

class CZigBeeAttributeDiscoveryRecord
{
	// Attributes
	public:
		// Attribute ID
		unsigned short m_wAttributeID;

		// ZCL data type
		unsigned char m_nType;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeePersistentReportingInfo

#pragma pack(push, 4)

class CZigBeePersistentReportingInfo
{
	// typedefs & enums
	public:
		// Options
		enum { manufacturerSpecific = CZigBeeAttribute::manufacturerSpecific,
			client = 0x80 };

		// Offset for valid data members behind the obsolete target address
		enum { offset = 12 };

	// Construction
	private:
		CZigBeePersistentReportingInfo();

	protected:
		// This constructor variant with embedded attribute value storage must
		// only be invoked via Create()
		CZigBeePersistentReportingInfo(const unsigned short wAttributeID,
			const unsigned short wManufacturerCode, const unsigned char nOptions,
			const unsigned short wMinimumInterval,
			const unsigned short wMaximumInterval,
			const CPacketData::size_type cbReportableChange = 0);

	private:
		// This constructor variant with embedded attribute value storage must
		// only be invoked via Create()
		CZigBeePersistentReportingInfo(const unsigned short wAttributeID,
			const unsigned short wManufacturerCode, const unsigned char nOptions,
			const unsigned short wMinimumInterval,
			const unsigned short wMaximumInterval,
			const void *const pReportableChange,
			const CPacketData::size_type cbReportableChange);

	// Attributes
	public:
		// Associated attribute ID
		uint16_t m_wAttributeID;

		// Manufacturer ID, if applicable (determined by options)
		uint16_t m_wManufacturerCode;

		// Minimum reporting interval
		uint16_t m_wMinimumInterval;

		// Maximum reporting interval
		uint16_t m_wMaximumInterval;

		// The "manufacturer specific" option flag and a distinction between
		// client and server side of a cluster. These options will be taken
		// into consideration as key elements when comparing/sorting reporting
		// configuration/information records like this
		uint8_t m_nOptions;

		// Size of the embedded "reportable change" value data
		uint8_t m_cbReportableChange;

		// Reserved for future use. Also provides alignment padding for the
		// reportable change value immediately following...
		uint16_t m_wReserved;

	// Operations
	public:
		// Creates a new reporting configuration object on the heap. Objects
		// of this type can only be created through this method because the
		// size of the object is variable, depending on m_cbReportableChange
		static CZigBeePersistentReportingInfo *Create
			(const unsigned short wAttributeID,
			const unsigned short wManufacturerCode,
			const unsigned char nOptions,
			const unsigned short wMinimumInterval,
			const unsigned short wMaximumInterval,
			const void *const pReportableChange = 0,
			const CPacketData::size_type cbReportableChange = 0);

		// Creates a new reporting configuration object on the heap. Objects
		// of this type can only be created through this method because the
		// size of the object is variable, depending on m_cbReportableChange
		static CZigBeePersistentReportingInfo *Create
			(const CPacketData::size_type cbRawSize);

		// Returns the raw storage location which can be read from or written to
		// non-volatile storage
		void *GetRawStorage() const;

		// Returns the size of the storage item required to store the entry
		CPacketData::size_type GetStorageSize() const;

		// Saves a reporting configuration to persistent storage
		unsigned char Store(const CZigBeeCluster &cluster) const;

		// Returns a pointer to the reportable change attribute value. This
		// value is guaranteed of the same type as the associated attribute
		// and is aligned on an 8-byte boundary. Notice that derived classes
		// adding data members must overload and provide appropriate storage
		// space
		virtual const void *GetReportableChangeValue() const;

	// Implementation
	public:
		virtual ~CZigBeePersistentReportingInfo();

#ifdef _DEBUG
		// Dump persistent reporting configuration
		void Dump() const;
#endif

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeReportingInfo

class CZigBeeReportingInfo : public CZigBeePersistentReportingInfo
{
	// Construction
	public:
		CZigBeeReportingInfo(const CZigBeeCluster &cluster,
			const CZigBeeAttribute &attribute,
			const unsigned short wMinimumInterval = 0,
			const unsigned short wMaximumInterval = 0);

	protected:
		// This constructor variant with embedded attribute value storage must
		// only be invoked via Create()
		CZigBeeReportingInfo(const CZigBeeCluster &cluster,
			const CZigBeeAttribute &attribute,
			const unsigned short wMinimumInterval,
			const unsigned short wMaximumInterval,
			const void *const pReportableChange = 0,
			const CPacketData::size_type cbReportableChange = 0,
			const CPacketData::size_type cbReportedValueCapacity = 0);

	// Attributes
	public:
		// Last time an attribute report has been sent
		CCompactTimerService::ticks_t m_lastReportTime;

		// Size of the embedded "last reported" value data
		CPacketData::size_type m_cbReportedValue;

		// Size of the embedded "last reported" value data
		CPacketData::size_type m_cbReportedValueCapacity;

		// Returns the current transfer state for this attribute report
		uint_fast8_t GetState() const;

		// Sets the current transfer state for this attribute report
		void SetState(const uint_fast8_t nState);

		// Returns true if a report is pending, i.e. needs to be queued and
		// sent out
		bool IsPending() const;

		// Returns true if the last report time and reported value fields are
		// valid
		bool IsLastReportValid() const;

		// Returns true if change or time-based reporting is paused for this
		// attribute. It is still possible to send reports programmatically,
		// i.e. requesting reports for the attribute explicitly
		bool IsPaused() const;

		// Returns a pointer to the reportable change attribute value. This
		// value is guaranteed of the same type as the associated attribute
		// and is aligned on an 8-byte boundary
		virtual const void *GetReportableChangeValue() const;

		// Returns a pointer to the last reported attribute value. This
		// value is guaranteed of the same type as the associated attribute
		// and is aligned on an 4 byte boundary (read/write)
		void *GetReportedValueStorage();

		// Returns a pointer to the last reported attribute value. This
		// value is guaranteed of the same type as the associated attribute
		// and is aligned on an 4 byte boundary (read-only)
		const void *GetReportedValueStorage() const;

	// Operations
	public:
		// Creates a new reporting configuration object on the heap. Objects
		// of this type can only be created through this method because the
		// size of the object is variable, depending on m_cbReportableChange
		static CZigBeeReportingInfo *Create(const CZigBeeCluster &cluster,
			const CZigBeeAttribute &attribute,
			const unsigned short wMinimumInterval,
			const unsigned short wMaximumInterval,
			const void *const pReportableChange = 0,
			const CPacketData::size_type cbReportableChange = 0);

	// Implementation
	public:
		virtual ~CZigBeeReportingInfo();

		// Flags, which describe state as per below bitmask definition
		uint16_t m_flags;

		// Cluster library sequence number of the attribute report, when one is
		// currently in progress. Where in progress means that a ZCL report has
		// been assembled and is awaiting transmission or an APS acknowledgment
		// and/or a ZCL default response, depending on configuration
		uint8_t m_nSequence;

		// Reporting info flags. 'Last report valid' when set to '1' means
		// that the last report time and reported value fields are valid. If
		// 'pending' is '1' a report should be sent out at the next occasion.
		// For instance, pending is set to '1' when the attribute should be
		// reported due to a change of the attribute value that is considered
		// significant. This is the case after reboot, or whenever the value
		// changes and the change exceeds the significant value configured for
		// the attribute, or the maximum reporting interval has expired.
		// The states 'transferred' and 'transfer failed' are ephemeral states,
		// which are only used in attribute reporting progress notifications.
		// 'Last report failed' when set to '1' means that the last report
		// could not be created, e.g. due to frame length restriction
		enum { stateIdle, stateQueued, stateTransferring, stateTransferred,
			stateTransferFailed, stateMask = 0x07, attemptMask = 0x18,
			attemptShift = 3, pending = 0x40, lastReportValid = 0x80,
			paused = 0x0100, lastReportFailed = 0x0200 };

#ifdef _DEBUG
		// Dump attribute reporting configuration
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeReportingInfoLess

class CZigBeeReportingInfoLess
{
	public:
		// Compares two CZigBeeReportingInfo objects, given their pointers.
		// Enables using a STL set of dynamically allocated
		// CZigBeeReportingInfo objects
		bool operator()(const CZigBeeReportingInfo *const pLeft,
			const CZigBeeReportingInfo *const pRight) const;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeReportingInfoSet

typedef std::set<CZigBeeReportingInfo *, CZigBeeReportingInfoLess>
	CZigBeeReportingInfoSet;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeLess

class CZigBeeAttributeLess
{
	public:
		// Compares two CZigBeeAttribute objects, given their pointers, and
		// returns true if left->m_wAttributeID < right->m_wAttributeID. Used
		// by CZigBeeCluster to create a sorted list of attributes during
		// attribute discovery
		bool operator()(const CZigBeeAttribute *const pLeft,
			const CZigBeeAttribute *const pRight) const;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAttributeDiscoverySet

typedef std::set<const CZigBeeAttribute *, CZigBeeAttributeLess>
	CZigBeeAttributeDiscoverySet;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeCluster attribute and command map support

#define __COMPACT7B_DECLARE_ATTRIBUTE_MAP() \
	protected: \
	static const CZigBeeAttribute attributeMap[]; \
	virtual void AfterPowerupEx(const bool bTopLevel); \
	public: \
	virtual const CZigBeeAttribute * \
	WalkAttributeMaps(const CZigBeeAttribute *const pMap) const;

#define __COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(cls, parent) \
	void cls::AfterPowerupEx(const bool bTopLevel) \
	{ \
		parent::AfterPowerupEx(false); \
		CZigBeeCluster::AfterPowerup(cls::attributeMap); \
		if (bTopLevel) \
			AfterPowerup(); \
	} \
	const CZigBeeAttribute * \
	cls::WalkAttributeMaps(const CZigBeeAttribute *const pMap) const \
	{ \
		return !pMap ? attributeMap : \
			parent::WalkAttributeMaps(pMap == attributeMap ? 0 : pMap); \
	}

#define __COMPACT7B_BEGIN_ATTRIBUTE_MAP(cls) \
	const CZigBeeAttribute cls::attributeMap[] = {

#define __COMPACT7B_END_ATTRIBUTE_MAP() \
		{ 0, 0, 0, 0, CZigBeeAttribute::typeUnknown, 0, 0 } \
	};

#define __COMPACT7B_ATTRIBUTE_CLUSTER(cls, attribute, type, range, options, \
	member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER_EX(cls, attribute, type, range, options, \
		CZigBeeAttribute::accessDefault, member)

#define __COMPACT7B_ATTRIBUTE_CLUSTER_EX(cls, attribute, type, range, \
	options, access, member) \
	{ \
		attribute, 0, (options) | CZigBeeAttribute::scopeCluster, \
		access, CZigBeeAttribute::type, \
		reinterpret_cast<const void *>(__c_offsetof(cls, member)), range \
	},

#define __COMPACT7B_ATTRIBUTE_CLASS(cls, attribute, type, range, options, \
	member) \
	__COMPACT7B_ATTRIBUTE_CLASS_EX(cls, attribute, type, range, options, \
		CZigBeeAttribute::accessDefault, member)

#define __COMPACT7B_ATTRIBUTE_CLASS_EX(cls, attribute, type, range, \
	options, access, member) \
	{ \
		attribute, 0, (options) | CZigBeeAttribute::scopeClass, \
		access, CZigBeeAttribute::type, &cls::member, range \
	},

#define __COMPACT7B_ATTRIBUTE_GLOBAL(attribute, type, range, options, \
	storage) \
	__COMPACT7B_ATTRIBUTE_GLOBAL_EX(attribute, type, range, options, \
	CZigBeeAttribute::accessDefault, storage)

#define __COMPACT7B_ATTRIBUTE_GLOBAL_EX(attribute, type, range, options, \
	access, storage) \
	{ \
		attribute, 0, (options) | CZigBeeAttribute::scopeGlobal, \
		access, CZigBeeAttribute::type, storage, range \
	},

#define __COMPACT7B_ATTRIBUTE_CLUSTER_M(cls, manufacturerID, attribute, \
	type, range, options, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER_M_EX(cls, manufacturerID, attribute, \
	type, range, options, CZigBeeAttribute::accessDefault, member)

#define __COMPACT7B_ATTRIBUTE_CLUSTER_M_EX(cls, manufacturerID, attribute, \
	type, range, options, access, member) \
	{ \
		attribute, manufacturerID, (options) | \
			CZigBeeAttribute::scopeCluster | \
			CZigBeeAttribute::manufacturerSpecific, \
		access, CZigBeeAttribute::type, \
		reinterpret_cast<const void *>(__c_offsetof(cls, member)), range  \
	},

#define __COMPACT7B_ATTRIBUTE_CLASS_M(cls, manufacturerID, attribute, type, \
	range, options, member) \
	__COMPACT7B_ATTRIBUTE_CLASS_M_EX(cls, manufacturerID, attribute, type, \
	range, options, CZigBeeAttribute::accessDefault, member)

#define __COMPACT7B_ATTRIBUTE_CLASS_M_EX(cls, manufacturerID, attribute, \
	type, range, options, access, member) \
	{ \
		attribute, manufacturerID, (options) | CZigBeeAttribute::scopeClass | \
			CZigBeeAttribute::manufacturerSpecific, \
		access, CZigBeeAttribute::type, &cls::member, range  \
	},

#define __COMPACT7B_ATTRIBUTE_GLOBAL_M(manufacturerID, attribute, type, \
	range, options, storage) \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M_EX(manufacturerID, attribute, type, \
	range, options, CZigBeeAttribute::accessDefault, storage)

#define __COMPACT7B_ATTRIBUTE_GLOBAL_M_EX(manufacturerID, attribute, type, \
	range, options, access, storage) \
	{ \
		attribute, manufacturerID, \
			(options) | CZigBeeAttribute::scopeGlobal | \
			CZigBeeAttribute::manufacturerSpecific, \
		access, CZigBeeAttribute::type, storage, range \
	},

#define __COMPACT7B_DECLARE_COMMAND_MAP() \
	protected: \
	static const CZigBeeClusterCommand commandMap[]; \
	public: \
	virtual const CZigBeeClusterCommand *FindCommand \
		(const unsigned char nCommand, \
		const unsigned char nOptions = 0, \
		const unsigned short wManufacturerCode = 0) const;

#define __COMPACT7B_IMPLEMENT_COMMAND_MAP(cls, parent) \
	const CZigBeeClusterCommand *cls::FindCommand \
		(const unsigned char nCommand, const unsigned char nOptions, \
		const unsigned short wManufacturerCode) const \
	{ \
		const CZigBeeClusterCommand *pCommand = \
			CZigBeeCluster::FindCommand(nCommand, nOptions, \
				wManufacturerCode, commandMap); \
		return pCommand ? pCommand : \
			parent::FindCommand(nCommand, nOptions, wManufacturerCode); \
	}

#define __COMPACT7B_BEGIN_COMMAND_MAP(cls) \
	const CZigBeeClusterCommand cls::commandMap[] = {

#define __COMPACT7B_COMMAND_C2S(command, pfn) \
		{ command, CZigBeeClusterCommand::typeCluster | \
			CZigBeeClusterCommand::accessInherit, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_COMMAND_C2S_EX(command, pfn, options) \
		{ command, CZigBeeClusterCommand::typeCluster | options, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_COMMAND_S2C(command, pfn) \
		{ command, CZigBeeClusterCommand::typeCluster | \
			CZigBeeClusterCommand::directionServerToClient | \
			CZigBeeClusterCommand::accessInherit, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_COMMAND_S2C_EX(command, pfn, options) \
		{ command, CZigBeeClusterCommand::typeCluster | \
			CZigBeeClusterCommand::directionServerToClient | options, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_COMMAND_C2S_M(manufacturerID, command, pfn) \
		{ command, CZigBeeClusterCommand::typeCluster | \
			CZigBeeClusterCommand::manufacturerSpecific | \
			CZigBeeClusterCommand::accessInherit, manufacturerID, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_COMMAND_C2S_M_EX(manufacturerID, command, pfn, options) \
		{ command, CZigBeeClusterCommand::typeCluster | \
			CZigBeeClusterCommand::manufacturerSpecific | options, \
			manufacturerID, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_COMMAND_S2C_M(manufacturerID, command, pfn) \
		{ command, CZigBeeClusterCommand::typeCluster | \
			CZigBeeClusterCommand::directionServerToClient | \
			CZigBeeClusterCommand::manufacturerSpecific | \
			CZigBeeClusterCommand::accessInherit, manufacturerID, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_COMMAND_S2C_M_EX(manufacturerID, command, pfn, options) \
		{ command, CZigBeeClusterCommand::typeCluster | \
			CZigBeeClusterCommand::directionServerToClient | \
			CZigBeeClusterCommand::manufacturerSpecific | options, \
			manufacturerID, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_PROFILECOMMAND_C2S(command, pfn) \
		{ command, CZigBeeClusterCommand::typeProfile | \
			CZigBeeClusterCommand::accessInherit, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_PROFILECOMMAND_C2S_EX(command, pfn, options) \
		{ command, CZigBeeClusterCommand::typeProfile | options, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_PROFILECOMMAND_S2C(command, pfn) \
		{ command, CZigBeeClusterCommand::typeProfile | \
			CZigBeeClusterCommand::directionServerToClient | \
			CZigBeeClusterCommand::accessInherit, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_PROFILECOMMAND_S2C_EX(command, pfn, options) \
		{ command, CZigBeeClusterCommand::typeProfile | \
			CZigBeeClusterCommand::directionServerToClient | options, 0, \
			static_cast<CZigBeeClusterCommand::HANDLER>(pfn) },

#define __COMPACT7B_END_COMMAND_MAP() \
		{ 0, 0, 0, 0 } \
	};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterCommand

class CZigBeeClusterCommand
{
	// typedefs & enums
	public:
		// Command handler template
		typedef void (CZigBeeCluster::*HANDLER)
			(const CZigBeeAddress &destination, const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &payload,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Options
		enum { typeProfile = CZigBeeClusterLibraryHeader::typeProfile,
			typeCluster = CZigBeeClusterLibraryHeader::typeCluster, typeStub,
			typeMask = 0x03,
			manufacturerSpecific =
				CZigBeeClusterLibraryHeader::manufacturerSpecific,
			directionServerToClient =
				CZigBeeClusterLibraryHeader::directionServerToClient,
			accessUnrestricted = 0x00, accessPeerOnly = 0x10,
			accessTrustCenterOnly = 0x20, accessInherit = 0x30,
			accessMask = 0x30, accessShift = 4,
			matchingMask = typeMask | manufacturerSpecific |
				directionServerToClient };

	// Attributes
	public:
		// Command identifier
		unsigned char m_nCommand;

		// Command options, including whether this is a manufacturer-specific
		// command
		unsigned char m_nOptions;

		// Manufacturer code (in case of a manufacturer-specific command)
		unsigned short m_wManufacturerCode;

		// Command handler routine
		HANDLER m_pfnHandler;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterRequest

class CZigBeeClusterRequest
{
	// typedefs & enums
	public:
		typedef void (CZigBeeCluster::*CALLBACK)(const uint_fast8_t nStatus,
			CZigBeeClusterRequest &request);

	// Construction
	public:
		CZigBeeClusterRequest(const uint_fast8_t nSequence,
			const uint_fast8_t nRequestCommand,
			const uint_fast8_t nResponseCommand,
			const CALLBACK m_pfnComplete, void *const pArgument,
			const unsigned int nTimeout,
			const bool bMultipleResponses,
			CZigBeeClusterRequest **const reference = 0);

	// Attributes
	public:
		// Argument to the service routine
		void *m_pArgument;

		// Called when the request has been completed
		CALLBACK m_pfnComplete;

		// Timeout for requests that require a response
		CCompactTimer *m_pTimer;

		// For sleeping end-devices, this is a responsive mode poll timer
		CCompactTimer *m_pResponsiveModeTimer;

		// Timeout allowed for the request (in symbols)
		unsigned int m_nTimeout;

		// Timestamp (of transmitted request frame)
		unsigned int m_nTimestampTX;

		// Timestamp (of received response frame)
		unsigned int m_nTimestampRX;

		// Destination address (of response frame)
		CZigBeeAddress m_destination;

		// Source address (of response frame)
		CZigBeeAddress m_source;

		// Response frame (might be empty)
		CPacket m_response;

		// Sequence number for this request, managed by ZDO instance below
		uint8_t m_nSequence;

		// Original command sent
		uint8_t m_nRequestCommand;

		// Expected response command
		uint8_t m_nResponseCommand;

		// State (progress that the request has made)
		uint8_t m_nState;

		// Status (of APSDE-DATA.request)
		uint8_t m_nStatusTX;

		// Security status (of received response frame)
		bool m_bSecurityUsed;

		// Link quality indicator (for received response frame)
		uint8_t m_nLinkQuality;

		// Offset to the ZCL header within the response frame
		CPacketData::size_type m_headerOffset;

		// If the request transmission can be aborted, this refers to the
		// instance pointer that references this request, or null otherwise
		CZigBeeClusterRequest **const m_reference;

	// Implementation
	public:
		// Possible states the request might be in
		enum { stateCreated = 0x01, stateQueued = 0x02, stateConfirmed = 0x04,
			stateResponseReceived = 0x08, stateTimeout = 0x10,
			stateMultipleResponses = 0x20, stateComplete = 0x80 };

#ifdef _DEBUG
		// Dumps pending ZCL request
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeCluster

class CZigBeeCluster : public CCompactTimerClient
{
	friend class CZigBeeFoundationApplication;

	__COMPACT7B_DECLARE_COMMAND_MAP()
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster library status codes
		enum { statusSuccess, statusFailure, statusNotAuthorized = 0x7e,
			statusReservedFieldNotZero, statusMalformedCommand,
			statusUnsupportedClusterCommand, statusUnsupportedGeneralCommand,
			statusUnsupportedManufacturerClusterCommand,
			statusUnsupportedManufacturerGeneralCommand, statusInvalidField,
			statusUnsupportedAttribute, statusInvalidValue, statusReadOnly,
			statusInsufficientSpace, statusDuplicateExists, statusNotFound,
			statusUnreportableAttribute, statusInvalidDataType,
			statusInvalidSelector, statusWriteOnly,
			statusInconsistentStartupState, statusDefinedOutOfBand,
			statusInconsistent, statusActionDenied, statusTimeout,
			statusHardwareFailure = 0xc0, statusSoftwareFailure,
			statusCalibrationError, statusUnsupportedCluster };

		// General ZCL command identifiers
		enum { commandReadAttributes, commandReadAttributesResponse,
			commandWriteAttributes, commandWriteAttributesUndivided,
			commandWriteAttributesResponse, commandWriteAttributesNoResponse,
			commandConfigureReporting, commandConfigureReportingResponse,
			commandReadReportingConfiguration,
			commandReadReportingConfigurationResponse, commandReportAttributes,
			commandDefaultResponse, commandDiscoverAttributes,
			commandDiscoverAttributesResponse, commandReadAttributesStructured,
			commandWriteAttributesStructured,
			commandWriteAttributesStructuredResponse };

		// General ZCL global cluster attributes
		enum { attributeClusterRevision = 0xfffd };

		// Frame overhead (ZCL command frame header)
		enum { frameOverhead = 3, frameOverheadEx = 5 };

		// Flags for cluster instances. A single instance can be both, inbound
		// (server) and outbound (client), although this might not be good
		// design practice (due to attribute reporting and binding table
		// restrictions, for example). The notion of "operational" clusters
		// distinguishes clusters that shall be bound during push-button
		// commissioning (for example on/off, level control) from those, which
		// are rather management clusters like basic, groups, identify, etc.
		// Notice that in this regard the scenes cluster should be marked as
		// non-operational cluster, because scenes cannot be commissioned with
		// simple bindings. The "accept reports" flag indicates whether a
		// cluster instance accepts inbound attribute reports. The "concealed"
		// flag, when set, will prevent the cluster from being listed in the
		// simple descriptor, or resulting in a cluster match. The cluster
		// remains operational, i.e. it can send and receive messages. The
		// "inactive" flag, when set, completely deactivates a cluster, i.e. it
		// prevents messages from being forwarded to the cluster and it
		// also implies the "concealed" flag behavior.The "inbound match"
		// flag, when set, instructs the framework to create a binding between
		// a local inbound cluster and a remote outbound cluster of the same
		// type, in the course of finding and binding; the "outbound match"
		// does the same for a local outbound cluster and a remote inbound
		// cluster.
		enum { flagInbound = 0x01, flagOutbound = 0x02,
			flagOperational = 0x04, flagAcceptReports = 0x08,
			flagInboundMatch = 0x10, flagOutboundMatch = 0x20,
			flagConcealed = 0x40, flagInactive = 0x80 };

		// Transmit options for ZCL messages sent via APS. This is a replica
		// of the same values maintained at the APS layer, so these must
		// always match. Added here for convenience in order not having to
		// include the stack in all clusters.
		// The non-APS "NoImplicitSecurity" option provides means to not
		// consider the implicit cluster security for a particular
		// transmission.
		enum { transmitEnableSecurity = 0x01, transmitAcknowledged = 0x04,
			transmitExtendedNonce = 0x10, transmitNoImplicitSecurity = 0x20,
			useIncomingSecurityMaterial = 0x80 };

		// Application access control privilege levels (mirrors the same values
		// as in CZigBeeFoundationApplication)
		enum { privilegeNone, privilegePeer, privilegeTrustCenter };

		// Policy overrides: default, always, never, inherit
		enum { policyDefault, policyAlways, policyNever, policyInherit };

		// Cluster access restriction models: unrestricted access, only talk to
		// peer devices with an application link-key, only talk to the trust
		// center (require trust center link-key), or inherit the settings from
		// the parent application endpoint
		enum { accessUnrestricted = privilegeNone,
			accessPeerOnly = privilegePeer,
			accessTrustCenterOnly = privilegeTrustCenter, accessInherit };

		// Attribute access types (used for access verification)
		enum { attributeAccessWrite = 1 << CZigBeeAttribute::accessWrite,
			attributeAccessRead = 1 << CZigBeeAttribute::accessRead,
			attributeAccessReporting = 1 << CZigBeeAttribute::accessReporting,
			attributeAccessAll = attributeAccessWrite | attributeAccessRead |
				attributeAccessReporting };

		// Configuration options. Security model: "no security required" will
		// accept any commands, including restricted ones, even if these are
		// not APS encrypted, "peer security" will accept restricted commands
		// only when they are APS encrypted, "trust center security" will
		// accept restricted commands only if they are encrypted with the trust
		// center link key (not any peer APS data link key). Similarly, all
		// messages originating in the device will use APS security when the
		// security policy is set to peer or trust center security. In addition
		// the APS acknowledgment request policy is configurable to either a
		// default policy (which typically means that most commands will be
		// soliciting APS acknowledgment), a force on (always request ACKs),
		// or force off (never request ACKs) policy.
		enum { optionAccessUnrestricted = accessUnrestricted,
			optionAccessPeerOnly = accessPeerOnly,
			optionAccessTrustCenterOnly = accessTrustCenterOnly,
			optionAccessInherit, optionAccessMask = 0x0003,
			optionAccessShift = 0,
			optionAcknowledgmentPolicyShift = 4,
			optionAcknowledgmentPolicyMask =
				3 << optionAcknowledgmentPolicyShift,
			optionAcknowledgmentPolicyDefault =
				policyDefault << optionAcknowledgmentPolicyShift,
			optionAcknowledgmentPolicyAlways =
				policyAlways << optionAcknowledgmentPolicyShift,
			optionAcknowledgmentPolicyNever =
				policyNever << optionAcknowledgmentPolicyShift,
			optionAcknowledgmentPolicyInherit =
				policyInherit << optionAcknowledgmentPolicyShift,
			optionDefaultResponsePolicyShift = 6,
			optionDefaultResponsePolicyMask =
				3 << optionDefaultResponsePolicyShift,
			optionDefaultResponsePolicyDefault =
				policyDefault << optionDefaultResponsePolicyShift,
			optionDefaultResponsePolicyAlways =
				policyAlways << optionDefaultResponsePolicyShift,
			optionDefaultResponsePolicyNever =
				policyNever << optionDefaultResponsePolicyShift,
			optionDefaultResponsePolicyInherit =
				policyInherit << optionDefaultResponsePolicyShift,
			optionBindToTrustCenterOnly = 0x0100 };

	// Construction
	public:
		CZigBeeCluster(CZigBeeFoundationApplication &application,
			const uint_fast16_t wClusterID,
			uint_fast8_t nFlags = flagInbound);

	// Attributes
	public:
		// Link to foundation application instance
		CZigBeeFoundationApplication &m_application;

		// Set of attribute reporting configuration records
		CZigBeeReportingInfoSet m_reporting;

		// Unique 16-bit identifier of this cluster
		const uint16_t m_wClusterID;

		// Server (in-bound), client (out-bound) or both sides of the cluster.
		// Also indicates whether the cluster should participate in matching
		// during finding & binding, or if the cluster is visible and active.
		// These flags typically don't change during the life-time of the
		// cluster
		uint8_t m_nFlags;

		// Configuration options. Determines whether the cluster requires APS
		// acknowledgment frames on typical messages, on all messages or on
		// none of the messages, and whether it should mirror the behavior
		// of request messages when generating response frames. Also determines
		// the overall cluster security policy, i.e. whether or not a set of
		// restricted commands and attributes exist in the cluster, which
		// require (i) an application link key or (ii) a trust-center link key
		// for authorization. Notice that by default all profile-wide commands
		// inherit the cluster security settings. A cluster that wants to use
		// attribute-level granularity for access control should override the
		// command handler map for profile-wide commands specifying new policy
		// (unrestricted) for the respective command handlers
		uint16_t m_nOptions;

	// Operations
	public:
		// Creates a ZCL request command frame (public) of the specified type
		static void CreateRequest(CPacket &asdu, const unsigned char bCommand,
			const unsigned char bType = CZigBeeClusterLibraryHeader::typeCluster,
			const CPacketData::size_type nPayload = 0);

		// Creates a ZCL request command frame (manufacturer specific) of the
		// specified type
		static void CreateRequest(CPacket &asdu,
			const unsigned short wManufacturerCode, const unsigned char bCommand,
			const unsigned char bType = CZigBeeClusterLibraryHeader::typeCluster,
			const CPacketData::size_type nPayload = 0);

		// Creates a ZCL response command frame, either public or manufacturer
		// specific, based on the header of the original request command frame
		// and the type parameter
		static void CreateResponse(CPacket &asdu,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned char bCommand,
			const CPacketData::size_type nPayload = 0,
			unsigned char bType = CZigBeeClusterLibraryHeader::typeProfile);

		// Default cluster command frame processing (handles general commands),
		// in fact checks the header, only
		static bool DefaultCommandProcessing(const CPacket &asdu);

		// Preprocesses a frame in order to help distinguish the cluster flags,
		// i.e. help decide which cluster instance should receive this message
		static bool Preprocess(const CPacket &asdu, unsigned char &nFlags);

		// Determines if an incoming frame should be rejected upfront in the
		// dispatch stage due to a lack of privilege (no security at all or
		// insufficient privilege level based on link key type) and whether a
		// default response with NOT_AUTHORIZED should be generated
		bool VerifyPrivilege(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const CZigBeeClusterCommand &command,
			const bool bGenerateDefaultResponse = true);

		// Determines if an incoming attempt to access an attribute should be
		// rejected due to a lack of privilege (no security at all or
		// insufficient privilege level based on link key type)
		bool VerifyPrivilege(const CZigBeeAttribute &attribute,
			const uint_fast8_t access = attributeAccessAll) const;

		// Creates a default response frame and sends it to the requester using
		// the specified application object. This allows an instance of the
		// CZigBeeFoundationApplication class to transmit default responses,
		// in particular the "UNSUPPORTED_CLUSTER" default response
		static void TransmitDefaultResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned char bStatus,
			CZigBeeFoundationApplication &application,
			const unsigned short wClusterID,
			const unsigned char nTransmitOptions = transmitAcknowledged);

		// Creates a default response frame and sends it to the requester.
		// Notice that the addressing material is that of the original request
		// frame, as it is typically supplied to a command handler. This method
		// will look at addressing mode, header flags, and status code to
		// determine whether or not a default response is actually sent, so in
		// most cases it is enough to invoke TransmitDefaultResponse()
		// unconditionally. Bear in mind that a default response should not be
		// sent whenever a more suitable, dedicated response is specified
		void TransmitDefaultResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned char bStatus = statusSuccess,
			const unsigned char nTransmitOptions = transmitAcknowledged);

		// Looks at the APS incoming security material to determine whether the
		// APS transmit options should include security and if so uses the
		// incoming security material for outgoing messages
		unsigned char MirrorIncomingSecurity() const;

		// Creates a configure reporting command frame with cbRecords space for
		// configuration records, which can be added using the two
		// AppendReportingConfiguration() methods
		static void CreateConfigureReportingCommand(CPacket &asdu,
			const CPacketData::size_type cbRecords = 16);

		// Appends a direction = 0 ("report to bindings") configuration record
		static void AppendReportingConfiguration(CPacket &asdu,
			const unsigned short wAttributeID,
			const unsigned char nType = CZigBeeAttribute::typeNull,
			const unsigned short wMinimumInterval = 0,
			const unsigned short wMaximumInterval = 0,
			const void *const pReportableChange = 0,
			const CPacketData::size_type cbReportableChange = 0);

		// Appends a direction = 1 ("I report to you") configuration record
		static void AppendReportingConfiguration(CPacket &asdu,
			const unsigned short wAttributeID,
			const unsigned short wTimeout);

		// Finds a particular attribute specified by cluster and attribute
		// identifiers. Notice that cluster and attribute identifiers are
		// related to a particular application profile identifier. Each
		// endpoint supports only one profile identifier
		const CZigBeeAttribute *
		FindAttribute(const unsigned short wAttributeID,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0) const;

		// Creates a sorted set of all attributes supported by the cluster,
		// subject to the specified options and manufacturer code. The
		// attribute set should initially be empty. Each derived class adds its
		// attributes to the supplied set
		void DiscoverAttributes(CZigBeeAttributeDiscoverySet &attributes,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0) const;

		// Sets the value of an attribute and takes appropriate measures. These
		// include sending a reporting if need be, and notifying the derived
		// cluster implementations and the parent application endpoint
		bool SetAttributeValue(const CZigBeeAttribute &attribute,
			const void *const pData,
			const CPacketData::size_type cbValue);

		// Similar to SetAttributeValue(), but does not notify derived cluster
		// implementations via OnAttributeChanged() or the parent application
		// endpoint via attribute change notifications. This is useful, if the
		// local device wants to change the attribute value and send a report
		// over the air without triggering application or cluster attribute
		// change handlers
		bool SetAttributeValueEx(const CZigBeeAttribute &attribute,
			const void *const pData,
			const CPacketData::size_type cbValue);

		// Save reporting configuration and start reporting...
		unsigned char SetupReporting(const CZigBeeAttribute &attribute,
			const unsigned short wMinimumInterval,
			const unsigned short wMaximumInterval,
			const void *const pReportableChange,
			const CPacketData::size_type cbReportableChange);

		// Remove reporting configuration and stop reporting
		unsigned char CancelReporting(const CZigBeeAttribute &attribute);

		// Revert reporting configuration to factory-fresh settings
		unsigned char RevertReporting(const CZigBeeAttribute &attribute,
			const uint_fast8_t endpoint = UINT8_MAX);

		// Process the configure reporting command (start reporting). Since
		// reporting may start after device start-up, it is possible to add
		// a delay and a random jitter for the first report
		unsigned char StartReporting(const CZigBeeAttribute &attribute,
			const unsigned short wMinimumInterval,
			const unsigned short wMaximumInterval,
			const void *const pReportableChange,
			const CPacketData::size_type cbReportableChange,
			CZigBeeReportingInfo **ppInfo = 0,
			const unsigned int nDelay = 1000000,
			const unsigned int nJitter = 15000000);

		// Process the configure reporting command (stop reporting)
		unsigned char StopReporting(const CZigBeeAttribute &attribute);

		// Restart reporting after it has been stopped by calling StopReporting()
		uint_fast8_t RestartReporting(const CZigBeeAttribute &attribute);

		// Pauses reporting of the specified attribute
		bool PauseReporting(const CZigBeeAttribute &attribute);

		// Resumes reporting of an attribute whose reporting has been paused
		bool ResumeReporting(const CZigBeeAttribute &attribute);

		// Triggers reporting of a particular attribute, at most after the
		// specified timeout. The report for this attribute is marked pending
		// and it is made sure that an attribute reporting cycle occurs after
		// the specified duration.
		bool ReportAttribute(const CZigBeeAttribute &attribute,
			const CCompactTimerService::ticks_diff_t defaultTimeout = 0);

		// Should be called after an attribute value has potentially changed.
		// Checks if the specified attribute should be reported, and if so,
		// triggers reporting of the specified attribute subject to reporting
		// configuration settings
		void ReportAttributeEx(const CZigBeeAttribute &attribute);

		// Performs range checks (and perhaps logical checks) in response to
		// an write attributes command. The status code returned by this method
		// is forwarded to the requestor. Notice that this method does the
		// actual change to the attribute value, if it meets all applicable
		// conditions. This also allows implementations to tweak attribute
		// values, e.g. round, clamp etc. DoWriteAttribute() will never be
		// called for attribute values, which are invalid. Notice that this was
		// previously called CanWriteAttributes() and was non-invasive, i.e.
		// the actual write was performed afterwards. The new approach is more
		// versatile
		virtual unsigned char DoWriteAttribute
			(const CZigBeeAttribute &attribute,
			const void *const pValue, const size_t cbValue);

		// Performs range checks (and perhaps logical checks) in response to
		// an write attributes command. If this method returns false, report
		// configuration must fail and an attribute status of
		// UNREPORTABLE_ATTRIBUTE shall be returned to the requestor
		/*virtual */bool CanReportAttribute
			(const CZigBeeAttribute &attribute) const;

		// Allows runtime configuration of the cluster options. Clears the set
		// of options specified in the second argument and sets the ones in
		// the first argument. Then stores configuration option changes
		// persistently
		virtual void ModifyOptions(const uint_fast16_t nSet,
			const uint_fast16_t nClear);

		// Sends a command frame via ZCL and waits for one or more suitable
		// response frames, including specific responses and default response
		// frames that match the sequence
		void QueueRequest(const CZigBeeAddress &destination,
			CPacket &asdu, const uint_fast8_t nResponseCommand,
			const unsigned char bTransmitOptions,
			const unsigned char bRadius = 0, void *const pArgument = 0,
			const unsigned char bEnhancedOptions = 0,
			const unsigned int nTimeout = defaultTimeout,
			const CZigBeeClusterRequest::CALLBACK pfnComplete = 0,
			const bool bMultipleResponses = false,
			CZigBeeClusterRequest **const reference = 0);

		// Cancel a particular pending ZCL request
		bool CancelRequest(CZigBeeClusterRequest *&request);

	// Overridables
	public:
		// Sends a command frame via ZCL and might perform pre-processing of
		// command templates to generate the final command frame. Might be
		// called by other clusters within the same or a different application
		// endpoint to transmit a ZCL command frame
		virtual void OnRequestDataEx(const CZigBeeAddress &destination,
			CPacket &asdu, const unsigned char bTransmitOptions,
			const unsigned char bRadius = 0, void *const pCookie = 0,
			const unsigned char bEnhancedOptions = 0);

		// Sends a command frame via ZCL
		virtual void OnRequestData(const CZigBeeAddress &destination,
			CPacket &asdu, const unsigned char bTransmitOptions,
			const unsigned char bRadius = 0, void *const pCookie = 0,
			const unsigned char bEnhancedOptions = 0);

		// Called by the application framework when a command frame has been
		// received for this particular cluster
		virtual void OnIndicateData(const CZigBeeAddress &destination,
			const CZigBeeAddress &source, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called by the application framework when a data transmission has
		// completed: APSDE-DATA.confirm(DstAddrMode, DstAddress, DstEndpoint,
		// SrcEndpoint, Status, TxTime), clause 2.2.4.1.2
		virtual void OnConfirmData(const CZigBeeAddress &destination,
			const unsigned char bSourceEndpoint, const unsigned char bStatus,
			const unsigned int nTimestamp, void *const pCookie);

		// Called at an early stage when the device is powered-up. May be
		// overridden to provide automatic loading of conditionally persistent
		// attributes or perform similar start-up initialization
		virtual void AfterPowerup();

		// Called at an early stage when the device is powered-up. Provides
		// code for automatic loading of persistent attributes (provided by
		// __COMPACT7B_DECLARE_ATTRIBUTE_MAP)
		// virtual void AfterPowerupEx(const bool bTopLevel);

		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// This notification is invoked when a device left the network
		virtual void OnNotifyNetworkLeave(const uint8_t nStatus,
			const bool bRejoin, uint8_t &actionFlags);

		// This notification provides network status notifications, which might
		// either have been generated by the local NLME or were received over
		// the air in a network status command
		virtual void OnNotifyNetworkStatus(const unsigned char nStatus,
			const CZigBeeAddress &address);

		// Called twice during factory reset: Once before persistent storage is
		// reverted to factory fresh settings (stage bit false), and once after
		// the storage has been reverted (stage bit true). Additional bits may
		// be set, which define the scope of the reset, e.g. if this is a full
		// factory reset. A full factory reset wipes security frame counters as
		// well as all attributes which are not marked as "preserved". It is
		// possible to define additional reset behavior, which is specific to a
		// particular application and sets a new operating mode, for example
		virtual void OnFactoryReset(const unsigned int nOptions);

		// Deletes all persistent storage items for the cluster. This is useful
		// for implementing partial factory resets or removing clusters at
		// run-time
		virtual void RestoreDefaults();

		// Handles a ZDO bind or unbind request for the cluster
		virtual unsigned char ProcessBindOrUnbindRequest
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const CZigBeeAddress &originator);

		// This method is called when an attribute's value has changed. Calls
		// the applications notification handlers and manages attribute
		// reporting, if applicable
		virtual void OnAttributeChanged(const unsigned short wAttributeID,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0);

		// This method is called when an attribute's value needs to be
		// refreshed. Calls the applications notification handlers. Can also be
		// overloaded in derived classes to perform the refresh inside the
		// cluster implementation
		virtual uint_fast8_t RefreshAttribute(const unsigned short wAttributeID,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0) const;

		// Stores relevant attributes as a scene extension field set in the
		// supplied packet object
		virtual bool StoreSceneAttributes(CPacket &scene) const;

		// Applies a pre-stored extension field set to the appropriate
		// attributes. Transition time is specified in tenths of a second
		virtual bool LoadSceneAttributes(const CPacket &scene,
			const unsigned int nTransitionTime);

		// Provides a progress indication on an attribute report, specifically
		// when a report is about to be transmitted, has been transferred
		// successfully, or the transfer failed
		virtual void OnNotifyReportingProgress(CZigBeeReportingInfo &info);

	// Implementation
	public:
		virtual ~CZigBeeCluster();

		// Replicates the same network status code. Will be used when the queue
		// of ZCL requests is at capacity while a new request is made
		enum { statusFrameNotBuffered = 0xd3 };

		// High-level (cluster scope) report generator engine states.
		// The state 'Startup' indicates that shortly after startup, pending
		// report transmission should be postponed until expiration of a
		// pre-defined delay period. This aims to increase the reliability
		// of report transmission on a stable communication link.
		enum { stateStartup, stateIdle, stateTransferInProgress };

		// Called when the attribute value has been modified by a write access
		// or a cluster command
		typedef void (CZigBeeCluster::*PARSECALLBACK)
			(const CZigBeeAttribute &attribute, void *pArgument);

		// This is the default cluster revision attribute. When introduced in
		// ZCL version 6, the initial value was set to one. A non-existing
		// attribute shall be treated as version 0 and indicates a ZCL v1..v4
		// implementation (notice ZCLv5 was only an interim paper-only release
		// without any implementations for profile interoperability). Specific
		// cluster revisions can be set by redeclaring the global attribute in
		// the derived cluster's attribute map
		static const uint16_t nClusterRevision;

		// Maximum number of attribute reporting configurations per cluster
		static const uint8_t nMaxReportingInfos;

		// Maximum number of attribute informations in a discovery response
		static const uint8_t nAttributeInformationLimit;

		// Maximum number of ZCL requests pending (per endpoint, cluster)
		static const uint8_t nQueueLimit;

		// Sequence counter for outbound ZCL command frames
		static unsigned char nSequence;

		// Default ZCL transaction timeout for cluster requests
		static const uint32_t defaultTimeout;

	protected:
		// Set of cluster requests pending a response
		std::set<CZigBeeClusterRequest *> m_requests;

		// Pending scene extension field set as to be restored in an imminent
		// recall scene
		CPacket m_pendingScene;

		// A timer for managing attribute reports. A single timer manages the
		// reports for all attributes across all clusters across all endpoints
		static CCompactTimer *pReportingTimer;

		// A delay to be taken into account when scheduling the next report.
		// This allows startup delays after reboot, for example. Also allows
		// aggregating, consolidating and collating reports
		static uint_fast32_t nReportingDelay;

		// A jitter to be taken into account when scheduling the next report.
		// This allows jitter after start-up or multicast requests, for example
		static uint_fast32_t nReportingJitter;

		// High-level (cluster scope) state of the report generator engine
		static uint8_t nReportingState;

		// The reporting delay to apply by default
		static const unsigned int nDefaultReportingDelay;

		// The reporting jitter to apply by default
		static const unsigned int nDefaultReportingJitter;

		// The reporting delay to apply after starting up (reboot)
		static const unsigned int nStartupReportingDelay;

		// The reporting jitter to apply after starting up (reboot)
		static const unsigned int nStartupReportingJitter;

		// The reporting delay to apply after receipt of a multicast command
		static const unsigned int nMulticastReportingDelay;

		// The reporting jitter to apply after receipt of a multicast command
		static const unsigned int nMulticastReportingJitter;

		// Creates a report attributes command frame either appending as many
		// attribute reports as fit or containing only one single reportable
		// attribute
		static const bool bCollateAttributeReports;

	public:
		// Walks the given attribute map after power-up. Loads persistent
		// attribute values
		void AfterPowerup(const CZigBeeAttribute *const pMap) const;

		// Searches the given attribute map for the specified attribute
		static const CZigBeeAttribute *
		FindAttribute(const unsigned short wAttributeID,
			const unsigned char nOptions, const unsigned short wManufacturerCode,
			const CZigBeeAttribute *const pMap);

		// Searches the given attribute map for the specified attribute
		static void DiscoverAttributes(CZigBeeAttributeDiscoverySet &attributes,
			const unsigned char nOptions, const unsigned short wManufacturerCode,
			const CZigBeeAttribute *const pMap);

		// Searches the given command map for the specified command
		static const CZigBeeClusterCommand *
		FindCommand(const unsigned char nCommand,
			const unsigned char nOptions, const unsigned short wManufacturerCode,
			const CZigBeeClusterCommand *const pMap);

		// Creates a ZCL command frame (public) of the specified type
		static void CreateCommand(CPacket &asdu, const unsigned char bSequence,
			const unsigned char bCommand,
			const unsigned char bType = CZigBeeClusterLibraryHeader::typeCluster,
			const CPacketData::size_type nPayload = 0);

		// Creates a ZCL command frame (manufacturer specific) of the specified
		// type
		static void CreateCommand(CPacket &asdu, const unsigned char bSequence,
			const unsigned short wManufacturerCode, const unsigned char bCommand,
			const unsigned char bType = CZigBeeClusterLibraryHeader::typeCluster,
			const CPacketData::size_type nPayload = 0);

		// Processes the read attributes command
		void OnReadAttributes(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes the write attributes command
		void OnWriteAttributes(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes the configure reporting command
		void OnConfigureReporting(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes the read reporting configuration command
		void OnReadReportingConfiguration
			(const CZigBeeAddress &destination, const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes the discover attributes command
		void OnDiscoverAttributes(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes the read attributes structured command
		void OnReadAttributesStructured(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes the write attributes structured command
		void OnWriteAttributesStructured(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Loads reporting configurations that have been previously stored
		void LoadReportingConfigurations();

		// Reverts reporting to the persistent configuration stored in the
		// supplied persistent storage, which may be working of factory-fresh
		uint_fast8_t RevertReporting(const CZigBeeAttribute &attribute,
			CZigBeeStorage &storage, const bool bStore = true,
			const uint_fast8_t endpoint = UINT8_MAX);

		// Reactivates a report given its persistent configuration
		uint_fast8_t ReactivateReporting
			(const CZigBeeReportingStorageItem item,
			CZigBeePersistentReportingInfo &info);

		// Pauses (or resumes) reporting of the specified attribute
		bool PauseOrResumeReporting(const CZigBeeAttribute &attribute,
			const bool bPause);

		// Schedules the next report. Determines the instant when the next
		// report from this device is due
		static void ScheduleReport(CZigBeeApplicationSupportBase &aps,
			const CCompactTimerService::ticks_diff_t defaultTimeout = 0,
			const bool bForceReport = false);

		// Fixup the timeout for the reporting timer given the current time,
		// and the specified minimum or maximum interval
		static void FixupTimeout(std::pair<CCompactTimerService::ticks_diff_t,
			bool> &timeout, const CZigBeeReportingInfo &info,
			const CCompactTimerService::ticks_t currentTime,
			const CCompactTimerService::ticks_diff_t interval);

		// Timer call-back that sends collated attribute reports when due
		static void OnReportDue(void *pArgument);

		// Finds a particular reporting configuration item
		bool FindReportingItem(const CZigBeeAttribute &attribute,
			CZigBeeReportingStorageItem &item);

		// Finds a particular reporting configuration item. Same as the default
		// version above, with the ability to specify a different storage than
		// the default persistent storage
		bool FindReportingItem(const CZigBeeAttribute &attribute,
			CZigBeeReportingStorageItem &item, CZigBeeStorage &storage,
			CZigBeePersistentReportingInfo **ppInfo = 0,
			const uint_fast8_t endpoint = UINT8_MAX);

		// Sends an attribute report immediately or schedules a report
		void ReportAttribute(CZigBeeReportingInfo &info,
			const CZigBeeAttribute &attribute) const;

		// Appends a single attribute report to a report attributes command
		// frame
		bool AppendAttributeReport(CPacket &asdu, CZigBeeReportingInfo &info,
			const CZigBeeAttribute &attribute) const;

		// Creates a report attributes command frame, appends as many attribute
		// reports as fit and queues a cluster library request. Keeps track of
		// each processed report, such that subsequent calls will progressively
		// process all pending attribute reports
		bool CollateAttributeReports();

		// Appends attribute records for gratuitously reported attributes to an
		// attribute report that has been prepared by CollateAttributeReports()
		// and will be sent alongside the attributes reported due to periodic
		// or event based reporting. Gratuitous reports are a best effort
		// service, no guaranteed delivery, retries or reframing will occur. It
		// is the product manufacturer's responsibility to carefully choose the
		// attributes that go alongside "normal" reports; it is recommended to
		// not use gratuitous attribute reports where possible, and only do so
		// where it is required, e.g. to comply with particular requirements of
		// certain eco-systems
		void AppendGratuitousReports(CPacket &asdu,
			std::set<uint16_t> &reportedAttributes) const;

		// Done transmitting the report
		void OnReportComplete(const uint_fast8_t nStatus,
			CZigBeeClusterRequest &request);

		// Can be called by derived classes to parse an attribute report
		void ParseReport(const CZigBeeClusterLibraryHeader &header,
			CPacket &asdu, const PARSECALLBACK pfnCallback,
			void *pArgument = 0);

		// Can be called by derived classes to parse a read attributes response
		// and provide a pointer to the read attribute status record for the
		// attribute of interest; in case of parsing errors or no matching
		// record, the return value is null. When the return value is not null,
		// the caller should look at the status field in order to determine
		// whether or not attribute data type and value are included. This is
		// the case only if the status equals SUCCESS
		static const CZigBeeAttributeReadStatusRecord *
		GetAttributeValue(const CZigBeeClusterLibraryHeader &header,
			const CPacket &asdu, const uint_fast16_t attributeID,
			const uint_fast8_t options = 0,
			const uint_fast16_t manufacturerCode = 0);

		// Stores preserved attributes
		void StorePreservedAttributes() const;

		// Removes attribute values from persistent storage
		void DeletePersistentAttributes(const unsigned char nEndpoint,
			const bool bManufacturerSpecific);

		// Returns the security level of the incoming frame that is currently
		// being processed. Any of the security level enumerations above. Note
		// this is looking at the APS incoming security material and must not
		// be called out of context
		unsigned int GetIncomingPrivilegeLevel() const;

		// Returns the effective level of security for a particular command by
		// looking at its meta-data and resolving potential references to the
		// inherited security level
		unsigned int GetRequiredPrivilegeLevel
			(const CZigBeeClusterCommand &command) const;

		// Returns the effective level of security for the cluster instance by
		// looking at the per instance and per cluster default settings as well
		// as resolving any inheritance relationships
		unsigned int GetRequiredPrivilegeLevel() const;

		// Returns the effective level of security for the cluster instance by
		// looking at the per instance and per cluster default settings as well
		// as resolving any inheritance relationships
		unsigned int GetAcknowledgmentPolicy() const;

		// Returns the effective level of security for the cluster instance by
		// looking at the per instance and per cluster default settings as well
		// as resolving any inheritance relationships
		unsigned int GetDefaultResponsePolicy() const;

		// Stores cluster options for (1) all clusters, (2) all clusters on a
		// specific endpoint, or (3) a specific cluster instance
		static bool StoreOptions(const uint_fast16_t nOptions,
			const uint_fast8_t nEndpoint = static_cast<uint_fast8_t>(-1),
			const uint_fast16_t wClusterID = static_cast<uint_fast16_t>(-1));

		// Restores the cluster options for all clusters, taking into account
		// global, per end-point and per cluster instance values. Globals are
		// loaded first, then per end-point, then per cluster instance
		static bool LoadOptions(uint16_t &nOptions,
			const uint_fast8_t nEndpoint = static_cast<uint_fast8_t>(-1),
			const uint_fast16_t wClusterID = static_cast<uint_fast16_t>(-1));

		// Called when a request has been processed. For requests that require
		// a response, the request is considered complete, when a mating
		// response or confirmation frame has been received, or the request
		// has timed out
		void CompleteRequest(CZigBeeClusterRequest *pRequest,
			const uint_fast8_t nStatus, CPacket *const pResponse = 0,
			const CPacketData::size_type headerOffset = 0);

		// Called when a request has timed-out, i.e. there was no response to
		// the original request within the specified timeout period
		void OnRequestTimeout(void *const pArgument);

		// Checks the header of a received frame in order to determine if this
		// is an anticipated response
		CZigBeeClusterRequest *IsAnticipatedResponse
			(const CZigBeeAddress &destination, const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu);

		// Loads scene extension field sets of clusters on the same endpoint
		bool ApplySceneAttributes(CPacket &packet,
			const unsigned int nTransitionTime);

		// Get the pending scene extension field set as a const reference. This
		// would prevent inadvertent change in the course of applying scenes
		// which could lead to unwanted effects.
		const CPacket &GetSceneFieldSet() const;

		// Performs value change to a structured attribute element in response
		// to a Write Attributes Structured command. The status code returned
		// by this method is forwarded to the requestor. If the element count
		// is to be altered, insertion of default element(s) or truncation of
		// existing element(s) may occur.
		uint_fast8_t DoWriteAttributeElement(const CZigBeeAttribute &attribute,
			uint16_t *const selectorIndex, const uint_fast8_t selectorIndicator,
			const void *const elementValue,
			const CPacketData::size_type elementSize,
			const void *const currentValue,
			const CPacketData::size_type currentSize);

#ifdef _DEBUG
		// Dump cluster information, e.g. attribute and reporting details
		virtual void Dump() const;
#endif

};

#define __COMPACT7B_CLUSTERREVISION(cls) \
	__COMPACT7B_ATTRIBUTE_CLASS(cls, \
		CZigBeeCluster::attributeClusterRevision, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nClusterRevision)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterNotification

class CZigBeeClusterNotification
{
	// typedefs & enums
	public:
		// Called when the attribute value has been modified by a write access
		// or a cluster command or needs refreshing
		typedef void (CZigBeeApplication::*MEMBERCALLBACK)
			(const unsigned char nOptions);

		// Options
		enum { manufacturerSpecific =
			CZigBeeClusterLibraryHeader::manufacturerSpecific,
			client = CZigBeeClusterLibraryHeader::directionServerToClient,
			dueToReadAttributes = 0x80, dueToReportAttributes = 0x40,
			subscriptionMask = 0x0f };

	// Attributes
	public:
		// Cluster ID to subscribe to
		unsigned short m_wClusterID;

		// Attribute ID to subscribe to
		unsigned short m_wAttributeID;

		// Manufacturer code (only valid, if indicated in option flags below)
		unsigned short m_wManufacturerCode;

		// Options (only the classification part, i.e. manufacturer-specific
		// and client flags, currently)
		unsigned char m_nOptions;

		// Call-back, invoked when the value has been touched (by a write
		// access or other command)
		MEMBERCALLBACK m_pfnUpdate;

		// Call-back, invoked when the value of an attribute is about to be
		// retrieved (by a read attributes command or a scheduled report)
		MEMBERCALLBACK m_pfnRefresh;
};
