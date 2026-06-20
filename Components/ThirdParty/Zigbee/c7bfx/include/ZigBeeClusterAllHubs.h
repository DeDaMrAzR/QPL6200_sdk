// ZigBeeClusterAllHubs.h : header file
//
// Copyright(C) 2019-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the proprietary, manufacturer-specific Amazon Works With All Hubs
// (WWAH) server cluster. This is an early-adopter version of the "All Hubs
// Initiative" covering a very narrow hub-centric use case.
//
// Usage of this cluster is only supported on pre-standards R23 and later
// stack revisions of the ubisys stack as it requires features only available
// in these versions. Standard R22 and earlier stacks only support a subset of
// the required features

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterAllHubs

class CZigBeeClusterAllHubs : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the Works With All Hubs cluster, within the
		// Amazon namespace
		enum { id = 0xfc57 };

		// Manufacturer IDs of Amazon Lab 126 and ubisys (custom extensions)
		enum { manufacturer = 0x1217 };

		// Attribute identifiers of the WWAH cluster
		enum { attributeDisableOTADowngrades = 2,
			attributeAcceptManagementLeaveWithoutRejoin,
			attributeNetworkRetryLimit,
			attributeMACRetryLimit,
			attributeRegularRouterCheckins,
			attributeAllowTouchlink,
			attributeUseParentClassification,
			attributeEventRetries,
			attributeEventQueueSize,
			attributeApplyPrescribedRejoinStrategy,
			attributeDataPollRetryInterval,
			attributeAllowConfiguration,
			attributeCurrentDebugReportID,
			attributeEnforceSecureNetworkKeyRotation,
			attributeAdvancedParentLinkMaintenance,
			attributePendingNetworkUpdateChannel,
			attributePendingNetworkUpdatePANID,
			attributeOTAMaxOfflineDuration };

		// Additional attribute identifiers allocated by ubisys
		enum { attributeRegularRouterCheckinInterval = 0x8000,
			attributeTrustCenterConnectivity };

		// Command IDs of the WWAH cluster (client-to-server)
		enum { commandEnableLinkKeyAuthorization,
			commandDisableLinkKeyAuthorization,
			commandQueryLinkKeyAuthorization,
			commandRequestNewLinkKey,
			commandEnableEventRetries,
			commandDisableEventRetries,
			commandRequestTime,
			commandEnableRejoinAlgorithm,
			commandDisableRejoinAlgorithm,
			commandSetIASZoneEnrollmentMethod,
			commandClearBindingTable,
			commandEnableRouterCheckins,
			commandDisableRouterCheckins,
			commandSetDataPollRetryInterval,
			commandSetPendingNetworkUpdate,
			commandMustRequestAcknowledgments,
			commandNeedNotRequestAcknowledgments,
			commandQueryMustRequestAcknowledgments,
			commandQueryDebugReport,
			commandSiteSurvey,
			commandDisableOTADowngrades,
			commandDisallowManagementLeaveWithoutRejoin,
			commandDisableTouchlink,
			commandEnableParentClassification,
			commandDisableParentClassification,
			commandEnableSecureNetworkKeyRotation,
			commandEnableAdvancedParentLinkMaintenance,
			commandDisableAdvancedParentLinkMaintenance,
			commandAllowConfiguration, commandDisallowConfiguration,
			commandUseTrustCenterForClusterServer,
			commandQueryUseTrustCenterForClusterServer };

		// Command IDs of the WWAH cluster (server-to-client)
		enum { commandQueryLinkKeyAuthorizationResponse,
			commandNotifyPoweringOff,
			commandNotifyPoweringOn,
			commandNotifyAddressChange,
			commandQueryMustRequestAcknowledgmentsResponse,
			commandNotifyPowerDescriptorChange,
			commandNotifyDebugReport,
			commandQueryDebugReportResponse,
			commandQueryUseTrustCenterForClusterServerResponse,
			commandSiteSurveyResponse };

		// Powering on and powering off notification reasons
		enum { reasonUnknown, reasonBattery, reasonBrownout, reasonWatchdog,
			reasonHardwareReset, reasonHardwareFault, reasonSoftwareException,
			reasonBootloadSuccess, reasonSoftwareReset, reasonPowerButton,
			reasonTemperature, reasonBootloadFailure };

	// Construction
	public:
		CZigBeeClusterAllHubs(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// This is the cluster revision attribute for the All Hubs cluster
		static const uint16_t nClusterRevision;

		// This attribute is a fixed value containing the number of MAC Level
		// Retries the device performs when sending a unicast command, not
		// including APS retries
		static const uint8_t nMACRetryLimit;

		// This attribute determines whether the device is connected to the
		// trust center or not, according to the rules determined by the trust
		// center connectivity monitoring feature
		static bool bTrustCenterConnectivity;

		// This attribute is set to TRUE if the router's check-in algorithm
		// has been enabled through the 'Enable Periodic Router Check-Ins'
		// command
		static bool bRegularRouterCheckins;

		// This attribute is set to the value of the check-in interval field
		// of am enable router check-in command, or null if disable router
		// check-ins has been invoked
		static uint16_t nRegularRouterCheckinInterval;

		// This attribute is set to FALSE if support for Touchlink Interpan
		// messages has been disabled on the device
		static bool bAllowTouchlink;

		// If WWAHParentClassificationEnabled is set to TRUE, the device
		// SHALL enable the WWAH Parent Classification Advertisements feature
		static bool bUseParentClassification;

		// This attribute is TRUE if the WWAH Application Event Retry Algorithm
		// is enabled
		static bool bEventRetries;

		// This attribute is the queue size for re-delivery attempts in the
		// WWAH Application Event Retry Algorithm
		static const uint8_t nEventQueueSize;

		// This attribute is TRUE if the WWAH Rejoin Algorithm is enabled
		static bool bApplyPrescribedRejoinStrategy;

		// This attribute is the time in seconds the device waits before
		// retrying a data poll when a MAC level data poll fails for any
		// reason
		static uint8_t nDataPollRetryInterval;

		// This attribute contains the ID of the current debug report stored
		// on the node. A value of 0x00 indicates that no debug report is
		// available
		static uint8_t nCurrentDebugReportID;

		// If WWAHBadParentRecoveryEnabled Attribute is set to TRUE, the
		// end-device SHALL enable the WWAH Bad Parent Recovery feature
		// described as follows
		static bool bAdvancedParentLinkMaintenance;

		// This attribute contains the maximum time in seconds the device may
		// be unavailable after completing its OTA download (e.g. while
		// restarting)
		static const uint16_t nOTAMaxOfflineDuration;

		// This attribute contains the maximum time in seconds the device may
		// be unavailable after completing its OTA download (e.g. while
		// restarting)
		static const bool bTrustCenterForClusterServer;

	// Operations
	public:
		// Returns the global All Hubs cluster singleton instance. Must not be
		// called prior to all global objects having been constructed
		static CZigBeeClusterAllHubs &GetInstance();

		// Sends a powering on or powering off notification, with optional
		// manufacturer-specific data
		void SendPoweringNotification(const bool bOn = true,
			const uint_fast8_t nReason = reasonUnknown,
			const void *const pInfo = 0,
			const uint_fast8_t cbInfo = 0);

		// Store debug report in flash and notify the client that a new report
		// is available. Notice that debug reports should not exceed the upper
		// bound defined below
		void StoreDebugReport(const void *const pReport,
			const CPacketData::size_type cbReport);

		// Get the power-on notification reason enum code add additional
		// manufacturer-specific information
		static uint_fast8_t GetPowerOnReason(CPacket &reason);

		// Preprocess an inbound frame that has passed security and
		// duplicate filters at the network layer
		static bool PreprocessNetworkData(CZigBeeNetwork &nwk,
			const CIEEE802154Address &source,
			const CIEEE802154Address &destination, CPacket &msdu,
			const unsigned char nLinkQuality, const unsigned char nDataSequence,
			const unsigned int nTimestamp);

	// Overrides
	public:
		// Called at an early stage when the device is powered-up. This
		// override configures the core stack and other clusters like
		// touch-link and OTA in accordance with configuration settings
		// stored persistently
		virtual void AfterPowerup();

		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational. Used to send powering on
		// notification
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// This notification provides network status notifications, which might
		// either have been generated by the local NLME or were received over
		// the air in a network status command
		virtual void OnNotifyNetworkStatus(const unsigned char nStatus,
			const CZigBeeAddress &address);

	// Implementation
	public:
		// Initial back-off (delay) and jitter for powering on notification
		enum { poweringOnBackoff = 0, poweringOnJitter = 312500 };

		// Long up-time threshold (in timer ticks). This should be set to 24h
		// in production, but will use a lower setting here (5 minutes) to
		// facilitate testing
		enum { longUpTimeThresholdTicks = 46875000 };

		// Upper bound on the size of debug reports, in bytes
		enum { maximumDebugReportSize = 64 };

		// All hubs cluster instance (singleton)
		static CZigBeeClusterAllHubs *pInstance;

		// Trust center connectivity timer for routers and non-sleepy
		// end-devices
		CCompactTimer *m_pConnectivityTimer;

		// Processes an "Enable link key authorization" command
		void OnEnableLinkKeyAuthorization(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "Disable link key authorization" command
		void OnDisableLinkKeyAuthorization(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "Query link key authorization" command
		void OnQueryLinkKeyAuthorization(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "Request new link key" command
		void OnRequestNewLinkKey(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnEnableEventRetries(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableEventRetries(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnRequestTime(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnEnableRejoinAlgorithm(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableRejoinAlgorithm(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnSetIASZoneEnrollmentMethod(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnClearBindingTable(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnEnableRouterCheckins(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableRouterCheckins(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnSetDataPollRetryInterval(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnSetPendingNetworkUpdate(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnMustRequestAcknowledgments(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnNeedNotRequestAcknowledgments(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnQueryMustRequestAcknowledgments(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnQueryDebugReport(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnSiteSurvey(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableOTADowngrades(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableManagementLeaveWithoutRejoin(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableTouchlink(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnEnableParentClassification(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableParentClassification(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnEnableSecureNetworkKeyRotation(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnEnableAdvancedParentLinkMaintenance
			(const CZigBeeAddress &destination, const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisableAdvancedParentLinkMaintenance
			(const CZigBeeAddress &destination, const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnAllowConfiguration(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnDisallowConfiguration(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnUseTrustCenterForClusterServer(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "" command
		void OnQueryUseTrustCenterForClusterServer
			(const CZigBeeAddress &destination, const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Parses a cluster list, e.g. an exemption list
		static bool ParseClusters(CPacket &asdu, std::set<uint16_t> &clusters);

		// Appends a cluster list, e.g. an exemption list. The caller must
		// make sure there is enough room in the packet to store the cluster
		// list
		static void AppendClusters(CPacket &asdu,
			const std::set<uint16_t> &clusters);

		// Walks all matching clusters on all endpoints and configures the
		// cluster options as specified. Notice this might result in writes
		// to persistent storage, so this method should only be used sparingly
		void ModifyAllClusterOptions(const std::set<uint16_t> &clusters,
			const uint_fast16_t nSet, const uint_fast16_t nClear = 0,
			const bool bInverse = false);

		// Probes all cluster instances on the device and compiles a set of
		// cluster IDs of those instances where the options match
		void FindClustersWithOptions(std::set<uint16_t> &clusters,
			const uint_fast16_t nValue, const uint_fast16_t nMask,
			const bool bInverse = false);

		// Returns true if any cluster instance that matches the specified
		// cluster identifier has been configured with the specified set of
		// options, e.g. to use and enforce link key authorization or to request
		// APS acknowledgments. The value referenced by the bAnyMatch parameter
		// will be set to true if any cluster (including those with a different
		// cluster identifier) matches (and thus requires the global application
		// flag be set to true if it was using inheritance)
		bool QueryClusterOptions(bool &bAnyMatch,
			const uint_fast16_t nValue, const uint_fast16_t nMask,
			const uint_fast16_t wClusterID = static_cast<uint_fast16_t>(-1));

		// Timer call-back that fires when the powering-on notification is due,
		// i.e. at a random instant within five seconds after power-up
		void OnPoweringOnNotificationDue(void *pArgument);

		// Set the value of the router check-in interval (ubisys custom)
		void SetRouterCheckinInterval(const uint_fast16_t nInterval = 0);

		// Set the value of trust center connectivity (ubisys custom). This
		// will also make sure the beacon payload is updated accordingly
		void SetTrustCenterConnectivity(const bool bConnected = true);

		// Schedule or reschedule the connectivity timer based on the check-in
		// interval that has been configured
		void ScheduleConnectivityTimer();

		// Timer call-back that fires when trust center connectivity is
		// potentially lost and needs to be checked explicitly
		void OnTrustCenterConnectivityTimeout(void *pArgument);

		// Called in the course of probing trust center connectivity, when the
		// query for the trust center's node descriptor has completed
		static void OnQueryComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Called when connectivity to trust center has been confirmed as lost
		static void OnTrustCenterConnectivityLoss();

		// Sends a notification to the client, notifying it of a new debug
		// report
		void SendDebugReportNotification(const uint_fast32_t cbReport,
			const uint_fast8_t nReportID = nCurrentDebugReportID);

		// Loads the most recent debug report, if any and sets the current
		// debug report identifier. If the call succeeded, ownership of the
		// underlying heap block is transferred to the caller and the
		// caller is responsible for calling delete[] on this pointer. The
		// memory block contains the report identifier followed by actual
		// report data. The returned size is the total size (i.e. report data
		// size plus one)
		bool LoadDebugReport(uint8_t *&pbReport,
			CPacketData::size_type &cbReport);

		// Sends a notification to the hub informing it about an address
		// change
		void SendAddressChangeNotification(const CZigBeeAddress &address);

		// Completion handler for site survey requests
		static void OnSiteSurveyComplete(const unsigned char nStatus,
			const std::list<CZigBeeNetworkDescriptor> &networks,
			void *pArgument);
};

#define __COMPACT7B_ALLHUBS_CLUSTERREVISION() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeClusterRevision, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nClusterRevision)

#define __COMPACT7B_ALLHUBS_DISABLEOTADOWNGRADES() \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M(CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeDisableOTADowngrades, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		&CZigBeeClusterOTAUpgrade::bProhibitDowngrades)

#define __COMPACT7B_ALLHUBS_ACCEPTMANAGEMENTLEAVEWITHOUTREJOIN() \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M(CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeAcceptManagementLeaveWithoutRejoin, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		&aps.m_apsme.m_aib.m_bAcceptLeaveRequest)

#define __COMPACT7B_ALLHUBS_NETWORKRETRYLIMIT() \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M(CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeNetworkRetryLimit, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&CZigBeeNetworkInformationBase::dataAttemptLimit)

#define __COMPACT7B_ALLHUBS_MACRETRYLIMIT() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeMACRetryLimit, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nMACRetryLimit)

#define __COMPACT7B_ALLHUBS_REGULARROUTERCHECKINS() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeRegularRouterCheckins, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		bRegularRouterCheckins)

#define __COMPACT7B_ALLHUBS_ALLOWTOUCHLINK() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeAllowTouchlink, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		bAllowTouchlink)

#define __COMPACT7B_ALLHUBS_USEPARENTCLASSIFICATION() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeUseParentClassification, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		bUseParentClassification)

#define __COMPACT7B_ALLHUBS_EVENTRETRIES() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeEventRetries, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		bEventRetries)

#define __COMPACT7B_ALLHUBS_EVENTQUEUESIZE() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeEventQueueSize, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		nEventQueueSize)

#define __COMPACT7B_ALLHUBS_APPLYPRESCRIBEDREJOINSTRATEGY() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeApplyPrescribedRejoinStrategy, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		bApplyPrescribedRejoinStrategy)

#define __COMPACT7B_ALLHUBS_DATAPOLLRETRYINTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeDataPollRetryInterval, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		nDataPollRetryInterval)

#define __COMPACT7B_ALLHUBS_ALLOWCONFIGURATION() \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M(CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeAllowConfiguration, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		&zdo.m_bAllowConfiguration)

#define __COMPACT7B_ALLHUBS_CURRENTDEBUGREPORTID() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeCurrentDebugReportID, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nCurrentDebugReportID)

#define __COMPACT7B_ALLHUBS_ENFORCESECURENETWORKKEYROTATION() \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M(CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeEnforceSecureNetworkKeyRotation, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		&aps.m_apsme.m_aib.m_bEnforceSecureNetworkKeyRotation)

#define __COMPACT7B_ALLHUBS_ADVANCEDPARENTLINKMAINTENANCE() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeAdvancedParentLinkMaintenance, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		bAdvancedParentLinkMaintenance)

#define __COMPACT7B_ALLHUBS_PENDINGNETWORKUPDATECHANNEL() \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M(CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributePendingNetworkUpdateChannel, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		&nwk.m_nlme.m_nib.m_nAcceptableChannel)

#define __COMPACT7B_ALLHUBS_PENDINGNETWORKUPDATEPANID() \
	__COMPACT7B_ATTRIBUTE_GLOBAL_M(CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributePendingNetworkUpdatePANID, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		&nwk.m_nlme.m_nib.m_wAcceptablePANID)

#define __COMPACT7B_ALLHUBS_MAXOFFLINEDURATION() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeOTAMaxOfflineDuration, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		nOTAMaxOfflineDuration)

#define __COMPACT7B_ALLHUBS_REGULARROUTERCHECKININTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeRegularRouterCheckinInterval, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		nRegularRouterCheckinInterval)

#define __COMPACT7B_ALLHUBS_TRUSTCENTERCONNECTIVITY() \
	__COMPACT7B_ATTRIBUTE_CLASS_M(CZigBeeClusterAllHubs, \
		CZigBeeClusterAllHubs::manufacturer, \
		CZigBeeClusterAllHubs::attributeTrustCenterConnectivity, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		bTrustCenterConnectivity)
