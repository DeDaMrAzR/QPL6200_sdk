// ZigBeeClusterOTABase.h : header file
//
// Copyright(C) 2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of ubisys ZigBee Cluster Library. Provides the OTA upgrade cluster
// frame definitions

/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImageNotify

#pragma pack(push, 1)

class CZigBeeOTAImageNotify
{
	// typedefs & enums
	public:
		enum { typeQueryJitter, typeSpanIncludingManufacturerID,
			typeSpanIncludingImageType, typeSpanIncludingFileVersion };

	// Attributes
	public:
		// Payload type (frame format)
		unsigned char m_nType;

		// Query jitter
		unsigned char m_nQueryJitter;

		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type
		unsigned short m_wImageType;

		// New file version
		unsigned int m_dwFileVersion;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAQueryImageRequest

#pragma pack(push, 1)

class CZigBeeOTAQueryImageRequest
{
	// typedefs & enums
	public:
		enum { fieldHardwareVersionPresent = 0x01 };

	// Attributes
	public:
		// Field control flags. Indicates presence of optional fields
		unsigned char m_nFieldControl;

		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. The value shall be between 0x0000 - 0xffbf (manufacturer
		// specific value range)
		unsigned short m_wImageType;

		// Current file version. The file version included in the payload
		// represents the device's current running image version
		unsigned int m_dwFileVersion;

		// Hardware version (optional). The hardware version if included in the
		// payload represents the device's current running hardware version
		unsigned short m_wHardwareVersion;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAQueryImageResponse

#pragma pack(push, 1)

class CZigBeeOTAQueryImageResponse
{
	// Attributes
	public:
		// Status code. Remaining fields are present only if status is SUCCESS
		unsigned char m_nStatus;

		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. Same value as in the query next image request frame
		unsigned short m_wImageType;

		// The file version indicates the image version that the client is
		// required to install. The version value may be lower than the current
		// image version on the client if the server decides to perform a
		// downgrade. The version value may be the same as the client's current
		// version if the server decides to perform a reinstall. However, in
		// general, the version value should be higher than the current image
		// version on the client to indicate an upgrade.
		unsigned int m_dwFileVersion;

		// The value represents the total size of the image (in bytes)
		// including header and all sub-elements
		unsigned int m_dwImageSize;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImageBlockRequest

#pragma pack(push, 1)

class CZigBeeOTAImageBlockRequest
{
	// typedefs & enums
	public:
		enum { fieldExtendedAddressPresent = 0x01,
			fieldBlockRequestDelayPresent = 0x02 };

	// Attributes
	public:
		// Field control. Remaining fields are present depending on the
		// settings here
		unsigned char m_nFieldControl;

		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. Same value as in the query next image request frame
		unsigned short m_wImageType;

		// The file version included in the payload represents the OTA upgrade
		// image file version that is being requested
		unsigned int m_dwFileVersion;

		// The value indicates number of bytes of data offset from the
		// beginning of the file. It essentially points to the location in the
		// OTA upgrade image file that the client is requesting the data from
		unsigned int m_dwFileOffset;

		// The value indicates the largest possible length of data (in bytes)
		// that the client can receive at once. The server shall respect the
		// value and not send the data that is larger than the maximum data
		// size. The server may send the data that is smaller than the maximum
		// data size value, for example, to account for source routing payload
		// overhead if the client is multiple hops away. By having the client
		// send both file offset and maximum data size in every command, it
		// eliminates the burden on the server for having to remember the
		// information for each client
		unsigned char m_nMaximumChunkSize;

		// Requesting node address, optional. This is the IEEE address of the
		// client device sending the Image Block Request command
		bool GetAddress(unsigned long long &qwAddress);

		// This is the current value of the BlockRequestDelay attribute of the
		// device that is making the request as set by the server. If the
		// device supports the attribute then it SHALL include this field in
		// the request. The value is in milliseconds
		bool GetBlockRequestDelay(unsigned short &wBlockRequestDelay);
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImagePageRequest

#pragma pack(push, 1)

class CZigBeeOTAImagePageRequest
{
	// typedefs & enums
	public:
		enum { fieldExtendedAddressPresent = 0x01,
			fieldBlockRequestDelayPresent = 0x02 };

	// Attributes
	public:
		// Status code. Remaining fields are present only if status is SUCCESS
		unsigned char m_nFieldControl;

		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. Same value as in the query next image request frame
		unsigned short m_wImageType;

		// The file version included in the payload represents the OTA upgrade
		// image file version that is being requested
		unsigned int m_dwFileVersion;

		// The value indicates number of bytes of data offset from the
		// beginning of the file. It essentially points to the location in the
		// OTA upgrade image file that the client is requesting the data from
		unsigned int m_dwFileOffset;

		// The value indicates the largest possible length of data (in bytes)
		// that the client can receive at once. The server shall respect the
		// value and not send the data that is larger than the maximum data
		// size. The server may send the data that is smaller than the maximum
		// data size value, for example, to account for source routing payload
		// overhead if the client is multiple hops away. By having the client
		// send both file offset and maximum data size in every command, it
		// eliminates the burden on the server for having to remember the
		// information for each client
		unsigned char m_nMaximumChunkSize;

		// The value indicates the number of bytes to be sent by the server
		// before the client sends another Image Page Request command. In
		// general, page size value shall be larger than the maximum data size
		// value
		unsigned short m_wPageSize;

		// The value indicates how fast the server shall send the data (via
		// Image Block Response command) to the client. The value is determined
		// by the client. The server shall wait at the minimum the (response)
		// spacing value before sending more data to the client. The value is
		// in milliseconds
		unsigned short m_wBlockDelay;

		// Requesting node address, optional. This is the IEEE address of the
		// client device sending the Image Block Request command
		bool GetAddress(unsigned long long &qwAddress);

		// This is the current value of the BlockRequestDelay attribute of the
		// device that is making the request as set by the server. If the
		// device supports the attribute then it SHALL include this field in
		// the request. The value is in milliseconds
		bool GetBlockRequestDelay(unsigned short &wBlockRequestDelay);
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImageBlockResponse

#pragma pack(push, 1)

class CZigBeeOTAImageBlockResponse
{
	// Attributes
	public:
		// Status code. Remaining fields are present only if status is SUCCESS
		unsigned char m_nStatus;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImageBlockResponseData

#pragma pack(push, 1)

class CZigBeeOTAImageBlockResponseSuccess :
	public CZigBeeOTAImageBlockResponse
{
	// Attributes
	public:
		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. Same value as in the query next image request frame
		unsigned short m_wImageType;

		// The file version indicates the image version that the client is
		// required to install
		unsigned int m_dwFileVersion;

		// The value represents the location of the data requested by the
		// client. For most cases, the file offset value included in the
		// (Image Block) response should be the same as the value requested
		// by the client. For (unsolicited) Image Block responses generated as
		// a result of Image Page Request, the file offset value shall be
		// incremented to indicate the next data location
		unsigned int m_dwFileOffset;

		// The value indicates the length of the image data (in bytes) that is
		// being included in the command. The value may be equal or smaller
		// than the maximum data size value requested by the client
		unsigned char m_nChunkSize;

		// Variable length data (m_nChunkSize bytes) follows...

};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImageBlockResponseDefer

#pragma pack(push, 1)

class CZigBeeOTAImageBlockResponseDefer :
	public CZigBeeOTAImageBlockResponse
{
	// Attributes
	public:
		// Current time. If the current time value is zero that means the
		// server does not support UTC time and the client shall treat the
		// request time value as offset time. Otherwise this is the server's
		// current time in seconds since 2000/01/01, 00:00:00 UTC
		unsigned int m_dwCurrentTime;

		// Request time. The UTC time indicates the actual time moment that
		// needs to pass before the client should try again. If current time is
		// nonzero, this is in seconds since 2000/01/01, 00:00:00 UTC, else
		// this is an offset value in seconds from now
		unsigned int m_dwRequestTime;

		// Minimum block period field is relocated to the extended version of
		// the frame definition as this field is generally optional and might
		// be omitted if a server does not support rate limiting
		// "The client SHALL check the existence of this field by looking at
		// the length of the message. If the field does not exist, then the
		// field SHALL have the value of zero."
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAImageBlockResponseDeferEx

#pragma pack(push, 1)

class CZigBeeOTAImageBlockResponseDeferEx :
	public CZigBeeOTAImageBlockResponseDefer
{
	// Attributes
	public:
		// Minimum block period. This value is only included if the status is
		// WAIT_FOR_DATA and the server supports rate limiting. This is the
		// minimum delay that the server wants the client to wait between
		// subsequent block requests. The client shall update its
		// BlockRequestDelay attribute to this value. The BlockRequestDelay
		// value SHALL be observed in all future Image Block Request messages
		// for the duration of the firmware image download, or until updated by
		// the server
		unsigned short m_wMinimumBlockPeriod;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAUpgradeEndRequest

#pragma pack(push, 1)

class CZigBeeOTAUpgradeEndRequest
{
	// Attributes
	public:
		// The status value of the Upgrade End Request command shall be
		// SUCCESS, INVALID_IMAGE, REQUIRE_MORE_IMAGE, or ABORT
		unsigned char m_nStatus;

		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. Same value as in the query next image request frame
		unsigned short m_wImageType;

		// The file version included in the payload represents the newly
		// downloaded OTA upgrade image file version
		unsigned int m_dwFileVersion;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAUpgradeEndResponse

#pragma pack(push, 1)

class CZigBeeOTAUpgradeEndResponse
{
	// Attributes
	public:
		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. Same value as in the query next image request frame
		unsigned short m_wImageType;

		// The file version included in the payload represents the newly
		// downloaded OTA upgrade image file version
		unsigned int m_dwFileVersion;

		// Current time. If the current time value is zero that means the
		// server does not support UTC time and the client shall treat the
		// request time value as offset time. Otherwise this is the server's
		// current time in seconds since 2000/01/01, 00:00:00 UTC
		unsigned int m_dwCurrentTime;

		// Upgrade time. The UTC time indicates the actual time moment that
		// needs to pass before the client should try again. If current time is
		// nonzero, this is in seconds since 2000/01/01, 00:00:00 UTC, else
		// this is an offset value in seconds from now
		unsigned int m_dwUpgradeTime;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAQuerySpecificImageRequest

#pragma pack(push, 1)

class CZigBeeOTAQuerySpecificFileRequest
{
	// typedefs & enums
	public:
		enum { fieldHardwareVersionPresent = 0x01 };

	// Attributes
	public:
		// Requesting node address
		unsigned long long m_qwAddress;

		// Manufacturer code
		unsigned short m_wManufacturerID;

		// Image type. The value shall be between 0x0000 - 0xffbf (manufacturer
		// specific value range)
		unsigned short m_wImageType;

		// Current file version. The file version included in the payload
		// represents the device's current running image version
		unsigned int m_dwFileVersion;

		// The value may represent the current running ZigBee stack version on
		// the device or the ZigBee stack version of the OTA upgrade image
		// being stored in additional memory space. The decision of which value
		// to include depends on which device specific file being requested.
		// For example, if the client is requesting a new security credential
		// file in order to be able to run the newly downloaded image (ex.
		// SE 2.0), then it should include the ZigBee stack version value of
		// the new image.
		unsigned short m_wZigBeeStackVersion;
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTAQuerySpecificImageResponse

typedef CZigBeeOTAQueryImageResponse CZigBeeOTAQuerySpecificImageResponse;
