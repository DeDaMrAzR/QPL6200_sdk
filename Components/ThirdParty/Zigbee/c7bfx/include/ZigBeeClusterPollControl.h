// ZigBeeClusterPollControl.h : header file
//
// Copyright(C) 2017-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the poll control server cluster

/////////////////////////////////////////////////////////////////////////////
// forward referenced classes

class CZigBeeServiceLocator;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterPollControl

class CZigBeeClusterPollControl : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the poll control cluster
		enum { id = 0x0020 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Attribute identifiers of the poll control server cluster
		enum { attributeCheckinInterval, attributeNormalPollInterval,
			attributeResponsivePollInterval, attributeResponsiveModeDuration,
			attributeMinimumCheckinInterval, attributeMinimumNormalPollInterval,
			attributeMaximumResponsiveModeDuration };

		// Command identifiers of the poll control server cluster,
		// client to server
		enum { commandCheckinResponse, commandLeaveResponsiveMode,
			commandSetNormalPollInterval, commandSetResponsivePollInterval };

		// Command identifiers of the poll control server cluster, server to
		// client
		enum { commandCheckin };

	// Construction
	public:
		CZigBeeClusterPollControl(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Attribute storage for attribute 0x0000, global scope
		static unsigned int nCheckinInterval;

		// Attribute storage for attribute 0x0001, global scope
		static unsigned int nNormalPollInterval;

		// Attribute storage for attribute 0x0002, global scope
		static unsigned short nResponsivePollInterval;

		// Attribute storage for attribute 0x0003, global scope
		static unsigned short nResponsiveModeDuration;

		// Attribute storage for attribute 0x0004, global scope
		static const unsigned int nMinimumCheckinInterval;

		// Attribute storage for attribute 0x0005, global scope
		static const unsigned int nMinimumNormalPollInterval;

		// Attribute storage for attribute 0x0006, global scope
		static const unsigned short nMaximumResponsiveModeDuration;

		// Limit of absent check-in responses, in a row, upon which
		// advanced parent link maintenance will be triggered
		static const uint8_t absentCheckinResponseLimit;

	// Operations
	public:
		// Creates a new service locator instance and searches for a suitable
		// endpoint on the trust center, if operating in All Hubs restricted mode
		void LocatePollControlClient(void *pArgument = 0);

		// Called when the service location process is complete
		bool OnServiceLocationComplete(CZigBeeServiceLocator &locator,
			const unsigned char nStatus);

	// Overrides
	public:
		// This override performs additional checks in order to make sure the
		// relationship between responsive mode poll intervals, normal mode
		// poll intervals and check-in intervals is always maintained
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

		// Called in an early stage when the device is powered-up. Loads
		// conditionally persistent attributes
		virtual void AfterPowerup();

		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// This override ensures that the only binding target is set to the
		// trust center, when operating in All Hubs restricted mode
		virtual void ModifyOptions(const uint_fast16_t nSet,
			const uint_fast16_t nClear);

		// This notification is invoked when a device left the network
		virtual void OnNotifyNetworkLeave(const uint8_t nStatus,
			const bool bRejoin, uint8_t &actionFlags);

		// Handles a ZDO bind or unbind request for the cluster
		virtual unsigned char ProcessBindOrUnbindRequest
			(const CZigBeeBindingSource &source,
			const CZigBeeBindingTarget &target, const bool bBind,
			const CZigBeeAddress &originator);

	// Implementation
	public:
		// Revision of the PollControl cluster
		static const unsigned short nClusterRevision;

		// Active responsive mode locks. Each short address corresponds to a
		// client, which keeps the server in responsive mode until the related
		// lock time has elapsed or the client requests to leave responsive
		// mode preemptively
		std::map<CZigBeeAddress, CCompactTimerService::ticks_diff_t> m_locks;

		// Poll control client locator that is only used in All Hubs
		// restricted mode to discover the poll control client end-point
		// on the hub.
		CZigBeeServiceLocator *m_locator;

		// Regular check-in interval timer
		CCompactTimer *m_pCheckinTimer;

		// Responsive mode lock life-time maintenance timer
		CCompactTimer *m_pLockMaintenanceTimer;

		// Responsive mode lock cookie assigned to the poll control cluster
		unsigned int m_nCookie;

		// Number of absent check-in responses in a row
		uint8_t m_absentCheckinResponseCount;

		// Initial delay for check-in, in ticks. This is applied after the
		// stack has started and also after a new client is bound
		enum { initialDelay = 15625 };

		// Attempts to augment an address such that it comprises both short and
		// extended addresses
		CZigBeeAddress AugmentAddress(const CZigBeeAddress &address) const;

		// Schedules the next regular check-in. The time is specified in ticks
		void ScheduleNextCheckin(const unsigned int nTicks = initialDelay);

		// Timer call-back. Fires at the regular check-in interval
		void OnCheckinIntervalElapsed(void *pArgument);

		// Schedules or reschedules the lock maintenance timer. Also enters
		// responsive mode if appropriate
		void ScheduleLockMaintenanceTimer();

		// Refreshes the lock life-time; either when the timer is rescheduled,
		// or the timer fires, or a lock is pre-emptively removed
		void RefreshLockLifetimes(const CCompactTimer *const pTimer = 0);

		// Timer call-back for maintenance of responsive mode locks. Basically,
		// implements a down-counter for the individual responsive mode locks
		// imposed by clients for potentially different durations
		void OnLockMaintenanceDue(void *pArgument);

		// Checks whether the specified application (as determined by either
		// the 16-bit network short address or 64-bit extended address and the
		// application endpoint) is bound to this cluster instance. Here, bound
		// means that the local device has an matching entry in its binding
		// table
		bool IsBoundDevice(const CZigBeeAddress &address) const;

		// Verifies that the source address is listed as a binding target for
		// the poll-control cluster, and transmits a default response with
		// status code ACTION_DENIED if not
		bool VerifyBoundDevice(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header);

		// Processes a received "check-in response" command that is triggered
		// by a transmitted check-in request command. In case of multiple
		// binding targets as bound clients, this method could be invoked
		// multiple times for a single transmitted request command.
		void OnCheckinResponse(const uint_fast8_t status,
			CZigBeeClusterRequest &request);

		// Processes a "fast-poll stop" command
		void OnLeaveResponsiveMode(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "set long poll interval" command
		void OnSetNormalPollInterval(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "set fast poll interval" command
		void OnSetResponsivePollInterval(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

#ifdef _DEBUG
		void Dump() const;
#endif

};

#define __COMPACT7B_POLLCONTROL_CHECKININTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterPollControl, \
		CZigBeeClusterPollControl::attributeCheckinInterval, \
		CZigBeeAttribute::typeUnsigned32, 0, CZigBeeAttribute::persistent, \
		nCheckinInterval)

#define __COMPACT7B_POLLCONTROL_NORMALPOLLINTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterPollControl, \
		CZigBeeClusterPollControl::attributeNormalPollInterval, \
		CZigBeeAttribute::typeUnsigned32, 0, CZigBeeAttribute::accessReadOnly, \
		nNormalPollInterval)

#define __COMPACT7B_POLLCONTROL_RESPONSIVEPOLLINTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterPollControl, \
		CZigBeeClusterPollControl::attributeResponsivePollInterval, \
		CZigBeeAttribute::typeUnsigned16, 0, CZigBeeAttribute::accessReadOnly, \
		nResponsivePollInterval)

#define __COMPACT7B_POLLCONTROL_RESPONSIVEMODEDURATION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterPollControl, \
		CZigBeeClusterPollControl::attributeResponsiveModeDuration, \
		CZigBeeAttribute::typeUnsigned16, 0, CZigBeeAttribute::persistent, \
		nResponsiveModeDuration)

#define __COMPACT7B_POLLCONTROL_MINIMUMCHECKININTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterPollControl, \
		CZigBeeClusterPollControl::attributeMinimumCheckinInterval, \
		CZigBeeAttribute::typeUnsigned32, 0, CZigBeeAttribute::accessReadOnly, \
		nMinimumCheckinInterval)

#define __COMPACT7B_POLLCONTROL_MINIMUMNORMALPOLLINTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterPollControl, \
		CZigBeeClusterPollControl::attributeMinimumNormalPollInterval, \
		CZigBeeAttribute::typeUnsigned32, 0, CZigBeeAttribute::accessReadOnly, \
		nMinimumNormalPollInterval)

#define __COMPACT7B_POLLCONTROL_MAXIMUMRESPONSIVEMODEDURATION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterPollControl, \
		CZigBeeClusterPollControl::attributeMaximumResponsiveModeDuration, \
		CZigBeeAttribute::typeUnsigned16, 0, CZigBeeAttribute::accessReadOnly, \
		nMaximumResponsiveModeDuration)

#define __COMPACT7B_POLLCONTROL_CLUSTER_ATTRIBUTES() \
	__COMPACT7B_POLLCONTROL_CHECKININTERVAL() \
	__COMPACT7B_POLLCONTROL_NORMALPOLLINTERVAL() \
	__COMPACT7B_POLLCONTROL_RESPONSIVEPOLLINTERVAL() \
	__COMPACT7B_POLLCONTROL_RESPONSIVEMODEDURATION() \
	__COMPACT7B_POLLCONTROL_MINIMUMCHECKININTERVAL() \
	__COMPACT7B_POLLCONTROL_MINIMUMNORMALPOLLINTERVAL() \
	__COMPACT7B_POLLCONTROL_MAXIMUMRESPONSIVEMODEDURATION()
