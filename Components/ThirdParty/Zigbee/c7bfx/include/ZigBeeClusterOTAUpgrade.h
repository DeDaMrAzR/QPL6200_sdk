// ZigBeeClusterOTAUpgrade.h : header file
//
// Copyright(C) 2012-2020 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of ubisys ZigBee Cluster Library. Provides the OTA upgrade cluster

/////////////////////////////////////////////////////////////////////////////
// forward referenced classes

class CZigBeeServiceLocator;
class CZigBeeOTAImageHeader;
class CZigBeeOTAElementHeader;
class CZigBeeClusterOTAUpgrade;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTATransferBlock

class CZigBeeOTATransferBlock
{
	// Construction
	public:
		CZigBeeOTATransferBlock(const unsigned int nAddress,
			const unsigned char nSize = 0, const unsigned char nAttempts = 0);

	// Attributes
	public:
		unsigned int m_nAddress;

		unsigned char m_nSize;

		unsigned char m_nAttempts;

		// True, if the block has been successfully received
		bool m_bReceived;

	// Operations
	public:
		bool operator<(const CZigBeeOTATransferBlock &block) const;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeOTATransfer

class CZigBeeOTATransfer
{
	// typedefs & enums
	public:
		// Called when a transfer is complete (either completed successfully or
		// failed)
		typedef void (CZigBeeClusterOTAUpgrade::*CALLBACK)
			(unsigned char nStatus, unsigned int nAddress, void *pData,
			unsigned short cbData);

	// Construction
	public:
		CZigBeeOTATransfer();

	// Attributes
	public:
		// Raw OTA data buffer
		void *m_pData;

		// Start address
		unsigned int m_nAddress;

		// Size of raw OTA data buffer
		unsigned short m_cbData;

		// Pending block responses: address, chunk size, attempts made
		std::set<CZigBeeOTATransferBlock> m_blocks;

		// Completion handler
		CALLBACK m_pfnComplete;

	// Implementation
	public:
		~CZigBeeOTATransfer();
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterOTAUpgrade

class CZigBeeClusterOTAUpgrade : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the OTA upgrade cluster
		enum { id = 0x0019 };

		// Cluster revision of this implementation
		enum { revision = 4 };

		// Cluster command identifiers
		enum { commandNotifyImage, commandQueryNextImageRequest,
			commandQueryNextImageResponse, commandImageBlockRequest,
			commandImagePageRequest, commandImageBlockResponse,
			commandUpgradeEndRequest, commandUpgradeEndResponse,
			commandQuerySpecificFileRequest, commandQuerySpecificFileResponse };

		// Identifiers of the attribute set
		enum { attributeUpgradeServerID, attributeFileOffset,
			attributeCurrentFileVersion, attributeCurrentZigBeeStackVersion,
			attributeDownloadedFileVersion,
			attributeDownloadedZigBeeStackVersion, attributeImageUpgradeStatus,
			attributeManufacturerID, attributeImageTypeID,
			attributeMinimumBlockRequestDelay };

		// Image upgrade status values
		enum { upgradeIdle, upgradeDownloadInProgress, upgradeDownloadComplete,
			upgradeWaitingToUpgrade, upgradeCountDown, upgradeWaitForMore };

		// Status codes (OTA). Additional codes from ZCL: NOT_AUTHORIZED,
		// SUCCESS, MALFORMED_COMMAND, UNSUP_CLUSTER_COMMAND
		enum { statusAbort = 0x95, statusInvalidImage, statusWaitForData,
			statusNoImageAvailable,	statusRequireMoreImage };

		// Default chunk size for all blocks except initial header block. Must
		// be small enough such that it can be accommodated (accounting for NWK
		// and potentially APS security)
		enum { defaultChunkSize = 64 };

	// Construction
	public:
		CZigBeeClusterOTAUpgrade(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// OTA image header
		CZigBeeOTAImageHeader *m_pHeader;

		// Optional hardware revision to be used in queries
		unsigned short m_nHardwareVersion;

		// Network address, hardware address and endpoint number of the server
		CZigBeeAddress m_serverAddress;

		// True, if the server generally supports page requests
		bool m_bSupportsPageRequests;

		// The file version of the running firmware image on the device. This
		// version is sent in query next image requests and is also checked in
		// notify image messages
		static const unsigned int dwCurrentFileVersion;

		// The application's image type ID. This type is sent in query next
		// image requests and is also checked in notify image messages
		static const unsigned short wImageTypeID;

		// True if the OTA client shall prohibit downgrades to an earlier version
		static bool bProhibitDowngrades;

		// True if the OTA client should deliberately allow reinstallation of
		// firmware having the same file version as the currently installed one.
		// Note: Per ZCL specification, reinstallation of the same software
		// version shall be prohibited. The application might deliberately allow
		// it by setting this variable to TRUE. However, 'bProhibitDowngrades'
		// takes priority over 'bAllowReinstallation', i.e. if
		// 'bProhibitDowngrades' is set to TRUE the value of
		// 'bAllowReinstallation' is irrelevant and attempts to install a lower
		// or the same version will be rejected.
		static const bool bAllowReinstallation;

	// Operations
	protected:
		// Requests a chunk of image data from the OTA server. Splits the
		// request into several requests and reattempts transfers until they
		// succeed or the number of attempt limits has been reached
		void RequestDataTransfer(const unsigned int nAddress,
			const unsigned short cbTransfer,
			const CZigBeeOTATransfer::CALLBACK pfnComplete,
			const CPacketData::size_type cbChunk = defaultChunkSize);

		// Given an offset within the current sub-element, this method returns
		// an offset than can be used as an argument to RequestDataTransfer()
		unsigned int MakeOffset(unsigned int nElementOffset);

		// Must be called once after ProcessElement() has been called to
		// continue parsing the OTA image
		void EndElement();

	// Overridables
	protected:
		// Called when the complete OTA header, including optional fields, is
		// available for processing. The image is available in m_pHeader
		virtual bool OnImageHeaderAvailable();

		// Called to process an element contained in the OTA image. The element
		// data starts at m_nFileOffset, type and length are available in the
		// m_wElementTagID and m_dwElementSize data members. In overrides of
		// this method, make sure that after all required data blocks have been
		// requested, EndElement() is called to continue with the next element
		// or end the upgrade process properly
		virtual void OnBeginElement();

		// Called when the last element has been processed. The method must
		// make a state transition to either "download complete" by returning
		// true or "wait for more image" by returning false. Typically, this
		// comprises checking hash values etc.
		virtual bool OnImageComplete();

		// Called when the download or upgrade has been aborted by the server
		// or client. Can be used in derived classes to perform clean-up
		virtual void OnAbort();

		// Called after the download has completed successfully and the server
		// instructed to apply the image now
		virtual void OnApply();

	// Overrides
	public:
		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// Called by the application framework when a data transmission has
		// completed: APSDE-DATA.confirm(DstAddrMode, DstAddress, DstEndpoint,
		// SrcEndpoint, Status, TxTime), clause 2.2.4.1.2
		virtual void OnConfirmData(const CZigBeeAddress &destination,
			const unsigned char bSourceEndpoint, const unsigned char bStatus,
			const unsigned int nTimestamp, void *const pCookie);

		// This override ensures that the upgrade server address is set to the
		// trust center, as soon as this security policy is becoming effective
		virtual void ModifyOptions(const uint_fast16_t nSet,
			const uint_fast16_t nClear);

		// This notification is invoked when a device left the network
		virtual void OnNotifyNetworkLeave(const uint8_t status,
			const bool rejoin, uint8_t &actionFlags);

	// Implementation
	public:
		// Revision of the OTA Upgrade cluster
		static const unsigned short nClusterRevision;

		// Maximum image size supported by the device (512KB by default).
		// Notice that this is a limit on the overall OTA file size, which can
		// be greater than the size the device can store if it is processed
		// sequentially, for example. Also includes overhead introduced by the
		// OTA file and tag headers, for example
		static const unsigned int nMaximumImageSize;

		// Randomizes query next image requests. Time span in timer ticks over
		// which the requests are to be spread
		static const unsigned int nJitterTicksShort;
		static const unsigned int nJitterTicksNormal;
		static const unsigned int nJitterTicksExtended;
		static const unsigned int nJitterTicksExtendedBroadcast;

		// Regular query interval. Should be in the region of hours or days.
		// Notice that we choose 8 hours (three times a day), which yields
		// 28.800.000.000 microseconds or 1.800.000.000 ticks of 16usecs.
		static const unsigned int nRegularQueryTicks;

		// Query delay duration in ticks, for starting an image query after
		// OTA server localization has been successfully completed.
		static const unsigned int nStartupQueryTicks;

		// Query delay duration in ticks, for starting an image query retry
		// upon error being indicated for the current query request.
		static const unsigned int nRetryQueryTicks;

		// Query delay duration in ticks, for starting an image query upon
		// receipt of an unsolicited image notify request as unicast.
		static const unsigned int nNotifyQueryTicksIdle;

		// Query delay duration in ticks, for starting an image query upon
		// receipt of an unsolicited image notify request as multicast.
		static const unsigned int nNotifyQueryTicksIdleBroadcast;

		// Startup delay duration in ticks, for starting initial OTA server
		// localization procedure after startup.
		static const unsigned int nStartupDelayTicks;

		// Default block request delay for block requests and response spacing
		// for page requests (in milliseconds). Recommended value: 0.25 seconds
		static const unsigned int nDefaultBlockRequestDelay;
		static const unsigned int nRetryBlockRequestDelay;
		static const unsigned int nDefaultResponseSpacing;

		// Timeout per expected block response. Recommended value: 10 seconds
		static const unsigned int nBlockResponseTimeout;

		// Number of block attempts made before a transfer fails
		static const unsigned char nBlockAttemptLimit;

		// Timeout for upgrade end response in timer ticks of 16us,
		// recommended value: 10 seconds
		static const CCompactTimerService::ticks_diff_t upgradeEndResponseTimeout;

		// Interval for regular queries (repeated upgrade end request frames)
		// in timer ticks of 16us, shall not be less than 60 minutes
		static const CCompactTimerService::ticks_diff_t upgradeEndRequestInterval;

		// Indicates whether the client should accept Image Block response
		// frames carrying more data than it requested. By default, accept
		// such Image Block response frames and only take the actually
		// requested data chunk into account, silently dropping the
		// superfluous part
		static const bool acceptOverlongBlocks;

		// Currently active transfer
		CZigBeeOTATransfer m_transfer;

		// Timer for the next query
		CCompactTimer *m_pTimer;

		// OTA server locator with the boolean flag indicating if the start
		// of a localization process has been scheduled
		std::pair<CZigBeeServiceLocator *, bool> m_locator;

		// Indicates if the upgrade server has to be the trust center
		bool m_bTrustCenter;

		// Used to store the IEEE address of the upgrade server resulted from
		// the discovery of the upgrade server's identity
		unsigned long long m_qwUpgradeServerID;

		// Size of the image currently being downloaded
		unsigned int m_nImageSize;

		// Indicates the current location in the OTA upgrade image. It is
		// essentially the (start of the) address of the image data that is
		// being transferred from the OTA server to the client
		unsigned int m_nFileOffset;

		// The ZigBee stack version of the downloaded image on additional
		// memory space on the device
		unsigned short m_nDownloadedZigBeeStackVersion;

		// The file version of the downloaded image on additional memory space
		// on the device
		unsigned int m_nDownloadedFileVersion;

		// This attribute shall reflect the ZigBee assigned value for the
		// manufacturer of the device
		unsigned short m_nManufacturerID;

		// This attribute shall indicate the image type identifier of the file
		// that the client is currently downloading, or a file that has been
		// completely downloaded but not upgraded to yet. The value of this
		// attribute shall be 0xFFFF when the client is not downloading a file
		// or is not waiting to apply an upgrade
		unsigned short m_nImageTypeID;

		// This attribute acts as a rate limiting feature for the server to
		// slow down the client download and prevent saturating the network
		// with block requests. The attribute lives on the client but can be
		// changed during a download if rate limiting is supported by both
		// devices. This attribute shall reflect the minimum delay between
		// Image Block Request commands generated by the client in
		// milliseconds. The value of this attribute shall be updated when the
		// rate is changed by the server, but should reflect the client default
		// when an upgrade is not in progress or a server does not support this
		// feature
		unsigned short m_nMinimumBlockRequestDelay;

		// The upgrade status of the client device. The status indicates where
		// the client device is at in terms of the download and upgrade
		// process. The status helps to indicate whether the client has
		// completed the download process and whether it is ready to upgrade to
		// the new image
		unsigned char m_nImageUpgradeStatus;

		// Chunk size of the next scheduled image block/page query
		unsigned char m_nScheduledChunkSize;

		// File offset of the next scheduled image block/page query
		unsigned int m_dwScheduledFileOffset;

		// Page size of the next scheduled page query. Zero for block queries
		unsigned short m_wScheduledPageSize;

		// Desired response spacing of the next scheduled page query
		unsigned short m_wScheduledResponseSpacing;

		// Offset of the element currently being processed (start of header)
		unsigned int m_dwElementOffset;

		// Size of the element currently being processed (excluding the header)
		unsigned int m_dwElementSize;

		// Tag of the element currently being processed
		unsigned short m_wElementTagID;

		// Number of upgrade-end attempts without response from the server
		unsigned short m_nUpgradeEndAttempts;

		// Responsive mode timer
		CCompactTimer *m_pResponsiveModeTimer;

		// Sets the downloaded (or current) file version attribute
		void SetFileVersion(const unsigned int dwVersion);

		// Sets the downloaded (or current) ZigBee stack version attribute
		void SetZigBeeStackVersion(const unsigned short wVersion);

		// Sets the image type attribute
		void SetImageType(const unsigned short wImageType);

		// Sets the image upgrade status attribute
		void SetImageUpgradeStatus(const unsigned char bImageUpgradeStatus);

		// Sets the file offset attribute
		void SetFileOffset(const unsigned int dwOffset);

		// Sets the minimum block request delay attribute
		void SetMinimumBlockRequestDelay(const unsigned short wDelay);

		// Sets the upgrade server address
		void SetUpgradeServerID(const CZigBeeAddress &address);

		// Attempts to locate the server after a specified delay
		void LocateServerDeferred(const unsigned int nTicks);

		// Creates a new service locator instance and searches for a suitable
		// OTA server
		void LocateServer(void *pArgument = 0);

		// Called by the service locator as soon as the mating service-provider
		// is found or service discovery has failed
		bool OnServiceLocationComplete(CZigBeeServiceLocator &locator,
			const unsigned char nStatus);

		// Convenience function to sends a request to the server using default
		// transmit options. Optionally enables responsive mode on sleep
		// end-devices, in case the caller expects a response
		void RequestData(CPacket &asdu,
			const bool bEnterResponsiveMode = false);

		// Handler for OTA image notify command. Queries the next image
		void OnNotifyImage(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Aborts a download and enters idle state. Performs regular queries
		void Abort(const unsigned int nTicks = nRetryQueryTicks,
			const unsigned int nJitterTicks = nJitterTicksNormal);

		// Schedules the query next image command to occur with the specified
		// base timeout (plus jitter)
		void ScheduleNextQuery(const unsigned int nTicks = nRegularQueryTicks,
			const unsigned int nJitterTicks = nJitterTicksNormal);

		// Timer callback, which is invoked when the query period has expired
		void OnQueryJitterExpired(void *pArgument);

		// Checks whether a new image is available
		void QueryNextImage();

		// Handler for OTA query next image response command, starts the
		// download if necessary. Received Query Next Image response frame
		// will only be processed if a corresponding request frame has been
		// previously initiated. Unsolicited ones without a prior request
		// frame will be dropped.
		void OnQueryNextImageResponse(const uint_fast8_t nStatus,
			CZigBeeClusterRequest &request);

		// Called when the header transfer (fixed portion) is complete
		void OnHeaderTransferComplete1(const unsigned char nStatus,
			const unsigned int nAddress, void *const pData,
			const unsigned short cbData);

		// Called when the header transfer (including optional fields) is
		// complete
		void OnHeaderTransferComplete2(const unsigned char nStatus,
			const unsigned int nAddress, void *const pData,
			const unsigned short cbData);

		// Called when a sub-element header transfer is complete. Called as
		// often as the number of sub-elements stored in the OTA container
		void OnElementHeaderTransferComplete(const unsigned char nStatus,
			const unsigned int nAddress, void *const pData,
			const unsigned short cbData);

		// Creates a block or page request
		void RequestImageData(const unsigned int dwOffset,
			const unsigned char cbChunk, const unsigned short wPageSize,
			const unsigned short wResponseSpacing);

		// Schedules the next block or page request
		void RequestImageDataThrottled(const unsigned int dwFileOffset,
			const unsigned char cbChunk, const unsigned short wPageSize = 0,
			const unsigned short wResponseSpacing = nDefaultResponseSpacing);

		// Request the next block of data (or a number of blocks if page
		// requests are allowed)
		void RequestNextBlock(const bool bAllowPageRequest = true);

		// Schedules the next block or page request
		void ScheduleRequest(const CCompactTimerService::ticks_diff_t nTicks,
			const unsigned int dwFileOffset, const unsigned char cbChunk,
			const unsigned short wPageSize = 0,
			const unsigned short wResponseSpacing = nDefaultResponseSpacing);

		// Timer callback that fires when the next block or page request may
		// be issued to the server (respecting the minimum block request delay)
		void OnDataRequestDelayExpired(void *pArgument);

		// Handler for OTA image block response command
		void OnImageBlockResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Handler for default response command
		void OnDefaultResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called when no block response was received for the specified timeout
		void OnImageBlockResponseTimeout(void *pArgument);

		// Completes a pending transfer
		void CompleteTransfer(const unsigned char nStatus);

		// Requests the end of the upgrade process (either upon success or
		// failure)
		void RequestUpgradeEnd(const unsigned char nStatus);

		// Handler for OTA upgrade end response command
		void OnUpgradeEndResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called when no upgrade end response was received
		void OnUpgradeEndResponseTimeout(void *pArgument);

		// Called when the update time is reached (count-down reached zero)
		void OnCountdownExpired(void *pArgument);

		// Cancel the current download procedure and schedule next query
		void CancelDownload(const uint_fast8_t status,
			const uint_fast32_t ticks = nRetryQueryTicks);

#ifdef _DEBUG
		// Shows overall OTA image and current sub-element progress
		void DumpProgress(const unsigned int nTotal = 0,
			const unsigned int nCurrent = 0,
			const char *const pszDescription = "", const char chMark = '=');

		// Prints the set of currently pending blocks
		void DumpPendingBlocks();
#endif

};

#define __COMPACT7B_OTA_UPGRADESERVERID(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeUpgradeServerID, \
		CZigBeeAttribute::typeIEEEAddress, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_OTA_FILEOFFSET(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeFileOffset, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_OTA_CURRENTFILEVERSION(cls) \
	__COMPACT7B_ATTRIBUTE_CLASS(cls, \
		CZigBeeClusterOTAUpgrade::attributeCurrentFileVersion, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		dwCurrentFileVersion)

#define __COMPACT7B_OTA_CURRENTZIGBEESTACKVERSION(cls) \
	__COMPACT7B_ATTRIBUTE_GLOBAL( \
		CZigBeeClusterOTAUpgrade::attributeCurrentZigBeeStackVersion, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&c7b::nStackVersion)

#define __COMPACT7B_OTA_DOWNLOADEDFILEVERSION(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeDownloadedFileVersion, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_OTA_DOWNLOADEDZIGBEESTACKVERSION(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeDownloadedZigBeeStackVersion, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_OTA_MANUFACTURERID(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeManufacturerID, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_OTA_IMAGETYPEID(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeImageTypeID, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_OTA_MINIMUMBLOCKREQUESTDELAY(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeMinimumBlockRequestDelay, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_OTA_IMAGEUPGRADESTATUS(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOTAUpgrade::attributeImageUpgradeStatus, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)
