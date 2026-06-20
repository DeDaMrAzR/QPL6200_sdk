// ZigBeeClusterIASZone.h : header file
//
// Copyright(C) 2016-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the IAS zone server cluster

/////////////////////////////////////////////////////////////////////////////
// Forward referenced classes

class CZigBeeServiceLocator;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIASZone

class CZigBeeClusterIASZone : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the IAS Zone cluster
		enum { id = 0x0500 };

		// Cluster revision of this implementation
		enum { revision = 2 };

		// Attribute identifiers of the IAS Zone server cluster
		enum { attributeState, attributeType, attributeStatus,
			attributeCIEAddress = 0x0010, attributeIdentifier,
			attributeNumberOfSensitivityLevelsSupported,
			attributeCurrentSensitivityLevel };

		// Command identifiers of the IAS Zone cluster, client to server
		enum { commandEnrollResponse, commandInitiateNormalOperationMode,
			commandInitiateTestMode };

		// Command identifiers of the IAS Zone cluster, server to client
		enum { commandStatusChange, commandEnrollRequest };

		// Zone status bitmap fields
		enum { statusAlarm1 = 0x0001, statusAlarm2 = 0x0002,
			statusTampered = 0x0004, statusBatteryLow = 0x0008,
			statusRegularNotifications = 0x0010,
			statusRestoreNotifications = 0x0020, statusTrouble = 0x0040,
			statusMainsFault = 0x0080, statusTestMode = 0x0100,
			statusBatteryDefect = 0x0200 };

		// Zone type enumeration values
		enum { typeStandardCIE, typeMotionSensor = 0x000d,
			typeContactSwitch = 0x0015, typeDoorWindowHandle = 0x0016,
			typeFireSensor = 0x0028, typeWaterSensor = 0x002a,
			typeCarbonMonoxideSensor, typePersonalEmergencyDevice,
			typeVibrationOrMovement, typeRemoteControl = 0x010f,
			typeKeyFob = 0x0115, typeKeypad = 0x021d,
			typeStandardWarningDevice = 0x0225, typeGlassBreakSensor = 0x0226,
			typeSecurityRepeater = 0x0229 };

		// Cluster behavior configuration
		enum { configurationAutoEnrollRequest = 0x01,
			configurationTripToPair = 0x02,
			configurationRequestDefaultResponse = 0x04,
			configurationAutoDetectCIE = 0x08 };

	// Construction
	public:
		CZigBeeClusterIASZone(CZigBeeFoundationApplication &application,
			const unsigned short nType,
			const bool bAutoEnrollRequest = true,
			const bool bRequestDefaultResponse = true,
			const unsigned short nStatus = 0,
			const unsigned int nAutoStartupFlags =
				CZigBeeApplicationSupportManagementBase::startupAllowResume |
				CZigBeeApplicationSupportManagementBase::startupAllowRejoining);

	// Overrides
	public:
		// This override performs additional checks in order to decide whether
		// or not writing the CIE address is permissible
		virtual unsigned char DoWriteAttribute
			(const CZigBeeAttribute &attribute,
			const void *const pValue, const size_t cbValue);

		// This method is called when an attribute's value has changed. Calls
		// the applications notification handlers and manages attribute
		// reporting, if applicable. For the time cluster, this will enforce
		// correct behavior of the time status and update real-time clocks etc.
		virtual void OnAttributeChanged(const unsigned short wAttributeID,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0);

		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// We have special requirements for the IAS zone cluster regarding
		// bindings. This overrides the default behavior, which is used by
		// most other clusters
		virtual unsigned char ProcessBindOrUnbindRequest
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const CZigBeeAddress &originator);

	// Overridables
	public:
		// Called after the IAS zone server has been enrolled. The base class
		// implementation starts to send regular supervision notifications
		virtual void AfterEnrolled();

	// Operations
	public:
		// Called by the application when a status change needs to be reported
		// to the IAS zone client (the CIE)
		virtual void OnStatusChange(const unsigned short nAdd,
			const unsigned short nRemove = 0);

	// Implementation
	public:
		// Revision of the IAS Zone cluster
		static const unsigned short nClusterRevision;

		// A regular notification aggregation window for start-up. A regular
		// notification is scheduled to occur after this window, which can be
		// postponed by an event-driven notification
		enum { startupNotificationTicks = 3750000 };

		// Interval of regular IAS zone notifications, in timer ticks
		static const unsigned int nRegularNotificationTicks;

		// Default value of the enhanced transmission options used for
		// transmitting status change notification frames
		static const uint8_t bStatusChangeEnhancedOptions;

		// Address of the CIE
		uint64_t m_qwCIEAddress;

		// IAS Zone client locator
		CZigBeeServiceLocator *m_locator;

		// Timer for regular supervision notifications
		CCompactTimer *m_pRegularNotificationTimer;

		// Either zero (no network steering triggered by status changes) or any
		// combination of CZigBeeApplicationSupportManagement::startupAllow...
		// flags to instigate a network startup
		unsigned int m_nAutoStartupFlags;

		// Indicates the type of sensor and consequently the meaning of the
		// alarm bits in the zone status attribute
		const unsigned short m_nType;

		// Indicates the current zone status, in particular the alarm bits and
		// other device health indicators (battery, tamper, etc.)
		unsigned short m_nStatus;

		// Indicates whether the sensor is already enrolled or not
		bool m_bEnrolled;

		// Zone identifier
		unsigned char m_nIdentifier;

		// IAS zone server cluster behavior configuration
		uint8_t m_nConfiguration;

		// Set preferred enrollment method for all supported IAS Zone cluster
		// instances that have not been enrolled yet
		static bool SetEnrollmentMethod(CZigBeeApplicationSupportBase &aps,
			const uint_fast8_t method);

		// Locates the endpoint on the CIE given its EUI-64 in m_qwCIEAddress.
		// The supplied argument is passed to the service locator used to find
		// the matching peer endpoint
		void LocateClient(void *pArgument = 0);

		// Called when service discovery has completed for the CIE
		bool OnServiceLocationComplete(CZigBeeServiceLocator &locator,
			const unsigned char nStatus);

		// Transmits an enroll request to the CIE, for which an appropriate
		// binding must exist
		void TransmitEnrollRequest();

		// Implements common status change behavior. Returns true if the caller
		// should proceed with its normal status change behavior, false if the
		// status change has been used as a trigger to initiate a trip-to-pair
		// sequence and the caller should not send or queue a notification
		bool OnStatusChangeCommon(const unsigned short nAdd,
			const unsigned short nRemove);

		// Updates the zone status attribute by setting the bits in the "add"
		// argument, and clearing those in the "remove" argument
		void UpdateStatus(const unsigned short nAdd,
			const unsigned short nRemove);

		// Transmit a status change. Returns the ZCL sequence number for the
		// frame created and passed to the APS layer
		unsigned char TransmitStatusChange(const CZigBeeAddress &destination,
			const unsigned short nZoneStatus,
			const unsigned short nDelay = 0, void *const pCookie = 0,
			const bool bDisableDefaultResponse = false,
			const unsigned int nTimeout = defaultTimeout,
			const CZigBeeClusterRequest::CALLBACK onDefaultResponse = 0);

		// Processes unsolicited incoming zone enroll response during an
		// ongoing "Auto-Enroll-Response" enroll process
		void OnEnrollResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Process an anticipated zone enroll response during an ongoing
		// "Trip-to-Pair" or "Auto-Enroll-Request" enroll process
		void OnPendingEnrollResponse(const uint_fast8_t status,
			CZigBeeClusterRequest &request);

		// Implement common enroll procedure that applies to all enroll methods
		void OnEnrollResponseCommon(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const uint8_t code, const uint8_t identifier);

		// Called when a regular supervision notification is due
		void OnRegularNotificationDue(void *pArgument = 0);

		// Set allowed enrollment method if not yet enrolled
		bool SetEnrollmentMethod(const uint_fast8_t method);

	protected:
		// Should be called when a status notification has been sent in order
		// to post-pone the next regular notification
		void ScheduleRegularNotification(const unsigned int nTicks =
			nRegularNotificationTicks);

		// Returns true of the specified address is the CIE. If only a short
		// address is provided, the address map is considered, but no active
		// address-resolution will be attempted
		bool IsCIE(const CZigBeeAddress &device) const;
};

#define __COMPACT7B_IASZONE_ZONESTATE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterIASZone::attributeState, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_IASZONE_ZONETYPE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterIASZone::attributeType, \
		CZigBeeAttribute::typeEnumeration16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_IASZONE_ZONESTATUS(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterIASZone::attributeStatus, \
		CZigBeeAttribute::typeBitmap16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_IASZONE_CIEADDRESS(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterIASZone::attributeCIEAddress, \
		CZigBeeAttribute::typeIEEEAddress, 0, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_IASZONE_ZONEID(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterIASZone::attributeIdentifier, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_IASZONE_CLUSTER_ATTRIBUTES(cls) \
	__COMPACT7B_IASZONE_ZONESTATE(cls, m_bEnrolled) \
	__COMPACT7B_IASZONE_ZONETYPE(cls, m_nType) \
	__COMPACT7B_IASZONE_ZONESTATUS(cls, m_nStatus) \
	__COMPACT7B_IASZONE_CIEADDRESS(cls, m_qwCIEAddress) \
	__COMPACT7B_IASZONE_ZONEID(cls, m_nIdentifier)
