// ZigBeeOTAUpgrade.h : header file
//
// Copyright(C) 2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This is part of the ZigBee Over-the-Air Upgrading Cluster used by ZigBee
// Smart Energy (1.1.2 draft) and Home Automation (1.2 draft) Profiles


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImageHeader

#pragma pack(push, 1)

class CZigBeeOTAImageHeader
{
	// typedefs & enums
	public:
		// OTA image magic number
		enum { magic = 0x0beef11e };

		// Initial header version
		enum { version = 0x0100 };

		// Header Field Control flags
		enum { flagHasSecurityCredentialVersion = 0x0001, 
			flagDeviceSpecific = 0x0002, flagHasHardwareVersions = 0x0004 };

		// Predefined image types. All other values in the range 0x0000 - 0xffbf
		// are manufacturer specific
		enum { typeClientSecurityCredentials = 0xffc0, typeClientConfiguration,
			typeServerLog, typeWildcard = 0xffff };

		// ZigBee stack versions
		enum { stackZigBee2006, stackZigBee2007, stackZigBeePRO, stackZigBeeIP };

		// Security Credential Version
		enum { securitySmartEnergy10, securitySmartEnergy11, 
			securitySmartEnergy20 };

	// Attributes
	public:
		// The value is a unique 4-byte value (magic number) that is included 
		// at the beginning of all ZigBee OTA upgrade image files. Always equal
		// to 0x0beef11e
		uint32_t m_dwFileIdentifier;

		// The value enumerates the version of the header and provides 
		// compatibility information. The value is composed of a major and 
		// minor version number (one byte each). The high byte (or the most 
		// significant byte) represents the major version and the low byte 
		// (or the least significant byte) represents the minor version number
		uint16_t m_wHeaderVersion;

		// This value indicates full length of the OTA header in bytes, 
		// including the OTA upgrade file identifier, OTA header length itself
		// to any optional fields
		uint16_t m_wHeaderSize;

		// The bit mask indicates whether additional information such as Image 
		// Signature or Signing Certificate are included as part of the OTA 
		// Upgrade Image
		uint16_t m_wHeaderFieldControl;

		// This is the ZigBee assigned identifier for each member company. When 
		// used during the OTA upgrade process, manufacturer code value of 
		// 0xffff has a special meaning of a wild card ("match all")
		uint16_t m_wManufacturerID;

		// The manufacturer should assign an appropriate and unique image type 
		// value to each of its devices in order to distinguish the products. 
		// This is a manufacturer specific value. However, the OTA Upgrade 
		// cluster has reserved the last 64 values of image type value to 
		// indicate specific file types such as security credential, log, and 
		// configuration
		uint16_t m_wImageType;

		// For firmware image, the file version represents the release and 
		// build number of the image's application and stack. The application 
		// release and build numbers are manufacturer specific. 
		// Recommendation regarding how the file version should be defined:
		// Application release, application build, stack release, stack build,
		// e.g. 0x10053519 represents application release 1.0 build 05 with 
		// stack release 3.5 b19, 0x10103701 represents application release 
		// 1.0 build 10 with stack release 3.7 b01
		uint32_t m_dwFileVersion;

		// This information indicates the ZigBee stack version that is used by 
		// the application. This provides the upgrade server an ability to 
		// coordinate the distribution of images to devices when the upgrades 
		// will cause a major jump that usually breaks the over-the-air 
		// compatibility, for example, from ZigBee Pro to upcoming ZigBee IP
		uint16_t m_wZigBeeStackVersion;

		// This is a manufacturer-specific string that may be used to store 
		// other necessary information as seen fit by each manufacturer. The 
		// idea is to have a human readable string that can prove helpful 
		// during development cycle
		char m_achDescription[32];

		// The value represents the total image size in bytes. This is the 
		// total of data in bytes that shall be transferred over-the-air from 
		// the server to the client. In most cases, the total image size of an 
		// OTA upgrade image file is the sum of the OTA header and the actual 
		// file data (along with its tag) lengths. If the image is a signed 
		// image and contains a certificate of the signer, then the total image 
		// size shall also include the signer certificate and the signature 
		// (along with their tags) in bytes
		uint32_t m_dwImageSize;

		// The following fields are optional. Their presence is determined by
		// evaluating the header control fields
		/*
		// Optional: Security credential version. This information indicates 
		// security credential version type, such as SE1.0 or SE2.0 that the 
		// client is required to have, before it shall install the image. One 
		// use case for this is so that after the client has downloaded a new 
		// image from the server, it should check if the value of security 
		// credential version allows for running the image. If the client's 
		// existing security credential version does not match or is outdated 
		// from what specified in the OTA header, it should obtain new security 
		// credentials before upgrading to running the new image
		uint8_t m_bSecurityCredentialVersion;

		// Optional: Target IEEE EUI-64. If the device specific bit is set, it
		// indicates that this OTA file contains security credential/certificate
		// data or other type of information that is specific to a particular 
		// device. The IEEE address of the target client device is stored here
		// in that case
		uint64_t m_qwUpgradeFileDestination;

		// Optional: Minimum hardware version. The value represents the earliest
		// hardware platform version this image should be used on. This field is
		// defined as follows: The high byte represents the version and the low 
		// byte represents the revision
		uint16_t m_wMinimumHardwareVersion;
		
		// Optional: Maximum hardware version. The value represents the latest 
		// hardware platform this image should be used on. The field is defined 
		// the same as the Minimum Hardware Version (above)
		uint16_t m_wMaximumHardwareVersion;
		*/

	// Operations
	public:
		// Total size of all optional fields
		enum { cbOptionalFields = 13 };

		// Returns the size of a header with the specified header control flags
		static size_t GetSize(unsigned short wHeaderFieldControl);

		// Returns the size of the header, including optional fields
		size_t GetSize() const;

		// Returns true if the image header is valid
		bool IsValid() const;

		// Optional: Security credential version. This information indicates 
		// security credential version type, such as SE1.0 or SE2.0 that the 
		// client is required to have, before it shall install the image. One 
		// use case for this is so that after the client has downloaded a new 
		// image from the server, it should check if the value of security 
		// credential version allows for running the image. If the client's 
		// existing security credential version does not match or is outdated 
		// from what specified in the OTA header, it should obtain new security 
		// credentials before upgrading to running the new image.
		uint8_t &GetSecurityCredentialVersion();

		// Optional: Target IEEE EUI-64. If the device specific bit is set, it
		// indicates that this OTA file contains security credential/certificate
		// data or other type of information that is specific to a particular 
		// device. The IEEE address of the target client device is stored here
		// in that case. Notice: Might not be properly aligned for 64-bit
		uint64_t &GetUpgradeFileDestination();

		// Optional: Minimum hardware version. The value represents the earliest
		// hardware platform version this image should be used on. This field is
		// defined as follows: The high byte represents the version and the low 
		// byte represents the revision. Notice: Might not be properly aligned 
		// for 16-bit
		uint16_t &GetMinimumHardwareVersion();

		// Optional: Maximum hardware version. The value represents the latest 
		// hardware platform this image should be used on. The field is defined 
		// the same as the Minimum Hardware Version (above). Notice: Might not be 
		// properly aligned for 16-bit
		uint16_t &GetMaximumHardwareVersion();
		
	// Implementation
	public:
};

#pragma pack(pop)

inline size_t CZigBeeOTAImageHeader::GetSize() const
	{ return GetSize(m_wHeaderFieldControl); }

inline bool CZigBeeOTAImageHeader::IsValid() const
	{ return (m_dwFileIdentifier == magic) && 
		(m_wHeaderSize >= sizeof(CZigBeeOTAImageHeader)) && 
		(m_wHeaderSize >= GetSize()) && (m_dwImageSize >= m_wHeaderSize) &&
		(m_wHeaderVersion == version); }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAElementHeader

#pragma pack(push, 1)

class CZigBeeOTAElementHeader
{
	// typedefs & enums
	public:
		// Predefined tag identifiers. Manfucaturer-specific tags in the range
		// 0xf000 - 0xffff. Tag 0xf7bd is reserved for use by ubisys and must
		// not be used by product manufacturers; it contains a proprietary
		// extension to the standard OTA file format, the Secure Directory
		enum { tagUpgradeImage, tagECDSASignature, tagECDSASigningCertificate,
			tagAESMMOHash, tagManufacturerSpecific = 0xf000,
			tagSecureDirectory = 0xf7bd };

	// Attributes
	public:
		// The tag identifier denotes the type and format of the data contained
		// within the sub-element.
		uint16_t m_wTagID;

		// The length of the data in the sub-element must be equal to the value
		// of the Length Field in bytes
		uint32_t m_dwSize;

		// Variable length data of the specified type and size follows here...

	// Operations
	public:
		
	// Implementation
	public:
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTADirectoryEntry

#pragma pack(push, 4)

class CZigBeeOTADirectoryEntry
{
	// typedefs & enums
	public:
		// The "hash valid" flag denotes that the hash field is valid, i.e.
		// conatains the hash of the original data (prior to encryption, if
		// applicable). The "encrypted" flag is set to '1' if the content is
		// encrypted
		enum { hashValid = 0x0001, encrypted = 0x0002 };

	// Attributes
	public:
		// File offset of the sub-element. The offset points to the 
		// sub-element header within the OTA file such that a client can 
		// quickly access the data it is interested in without having to
		// visit each sub-element header
		uint32_t m_dwOffset;

		// The tag identifier denotes the type and format of the data contained
		// within the sub-element.
		uint16_t m_wTagID;

		// Flags. A combination of the flags above
		uint16_t m_wFlags;

		// Hash over the entire contents of the sub-element. Notice that it is
		// sufficient for the client to verify the hash of only those elements
		// it is interested in, and verify the signature of this directory
		uint8_t m_abHash[16];

	// Operations
	public:

	// Implementation
	public:
};

#pragma pack(pop)
