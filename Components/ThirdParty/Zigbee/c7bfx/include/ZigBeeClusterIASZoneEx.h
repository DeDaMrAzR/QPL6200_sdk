// ZigBeeClusterIASZoneEx.h : header file
//
// Copyright(C) 2017 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the IAS zone server cluster extensions, in particular a queue,
// which complies with iControl requirements

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIASZoneEvent

class CZigBeeClusterIASZoneEvent
{
	// typedefs & enums
	public:
		// Queue element states: queued (status change detected), transmitting
		// (datagram queued towards the APS layer), transmitted (tranmission
		// confirmed from the APS layer), acknowledged (matching default
		// response with status = SUCCESS received), no response received, or
		// failed (response with status different than SUCCESS received). The
		// "discarded" and "transferred" states are for information purposes in
		// notifications only
		enum { stateQueued, stateTransmitting, stateTransmitted,
			stateTransmissionFailed, stateAcknowledged, stateNoResponse,
			stateFailed, stateDiscarded, stateTransferred };

	// Construction
	public:
		CZigBeeClusterIASZoneEvent(const unsigned int nTimestamp,
			const unsigned short nStatus,
			const unsigned char nState = stateQueued,
			const unsigned char nSequence = -1);

	// Attributes
	public:
		// Time when the event ocurred
		unsigned int m_nTimestamp;

		// Resulting zone status change triggered by the event
		unsigned short m_nStatus;

		// State of the event
		unsigned char m_nState;

		// The most recent ZCL sequence number used to send this event and we
		// want to match incoming default responses against. Only valid if we
		// passed the transmitted state
		unsigned char m_nSequence;

		// Returns true, if this event needs to be transmitted or retransmitted
		bool ShouldTransmit() const;

		// Returns true, if transmission of this event has failed
		bool HasFailed() const;

	// Implementation
	public:
#ifdef _DEBUG
		// Dumps an IAS zone status change event
		void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIASZoneEx

class CZigBeeClusterIASZoneEx : public CZigBeeClusterIASZone
{
	// Construction
	public:
		CZigBeeClusterIASZoneEx(CZigBeeFoundationApplication &application,
			const unsigned short nType,
			const bool bAutoEnrollRequest = true,
			const bool bRequestDefaultResponse = true,
			const unsigned short nStatus = 0,
			const unsigned int nAutoStartupFlags =
				CZigBeeApplicationSupportManagementBase::startupAllowResume |
				CZigBeeApplicationSupportManagementBase::startupAllowRejoining);

	// Operations
	public:
		// Flushes (i.e. clears) the event queue. Notice, this will not issue
		// a progress notification
		void Flush();

	// Overrides
	public:
		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

		// Called by the application when a status change needs to be reported
		// to the IAS zone client (the CIE)
		virtual void OnStatusChange(const unsigned short nAdd,
			const unsigned short nRemove = 0);

		// Called after the IAS zone server has been enrolled. Attempts to send
		// any queued events
		virtual void AfterEnrolled();

	// Overridables
	protected:
		// Called for the topmost event in the queue whenever an event is ready
		// for transmission and updated as it transitions through the various
		// states. A null pointer indicates the queue just became empty again
		virtual void OnNotifyProgress
			(const CZigBeeClusterIASZoneEvent *const pEvent = 0);

		// Schedules transmission of the next event. The base class
		// implementation transmits immediately if the previous transmission
		// attempt was successful, and back-offs using a delay, which increases
		// exponentially with the number of failed attempts. May be overridden
		// to implement customized back-off and retransmission schemes
		virtual void ScheduleNextTransmission();

	// Operations
	public:
		// Attempts to transmit pending events now. This function may be called
		// when an external trigger causes a new event to be queued and you do
		// not want to wait for a back-off period to expire
		void TransmitNextEventNow();

	// Implementation
	public:
		// Capacity of the queue (maximum number of events stored in the queue)
		static const size_t nEventLimit;

		// Default base duration for back-off delay, in timer ticks
		static const CCompactTimerService::ticks_t defaultBaseBackoffTicks;

		// Working base duration for back-off delay, in timer ticks
		static CCompactTimerService::ticks_t baseBackoffTicks;

		// Default maximum back-off time (upper bound), in timer ticks
		static const CCompactTimerService::ticks_t defaultMaxBackoffTicks;

		// Working maximum back-off time (upper bound), in timer ticks
		static CCompactTimerService::ticks_t maxBackoffTicks;

		// Default maximum back-off exponent for back-off delay
		static const uint8_t defaultBackoffCommonRatio;

		// Working maximum back-off exponent for back-off delay
		static uint8_t backoffCommonRatio;

		// Default attempt limit for delivery of each event.
		// If 0xff, delivery will be attempted forever (default)
		static const uint8_t defaultAttemptLimit;

		// Working attempt limit for delivery of each event.
		// If 0xff, delivery will be attempted forever (default)
		static uint8_t attemptLimit;

		// Event queue. Each status change (if it is really a change) is queued
		// and sent to the client. This allows for buffering messages during
		// times when the CIE is temporarily offline
		std::list<CZigBeeClusterIASZoneEvent> m_events;

		// Retransmission back-off timer
		CCompactTimer *m_pBackoffTimer;

		// Next backoff duration in timer ticks
		CCompactTimerService::ticks_t m_backoffTicks;

		// Number of consecutive transmission attempts that have failed. This
		// might be reset at different occasions, when immediate transmission
		// of pending requests is forced by e.g. TransmitPendingEventsNow()
		uint8_t m_nFailedAttempts;

		// Set transmission retry settings for stalled status change indication
		static void SetEventRetrySettings(const uint_fast32_t maxBackoffTime,
			const uint_fast8_t baseBackoffTime,
			const uint_fast8_t newBackoffCommonRatio,
			const uint_fast8_t newAttemptLimit);

		// Revert transmission retry settings for stalled status change
		// indication to the default values
		static void RevertEventRetrySettings();

		// Transmits the next event
		void TransmitNextEvent();

		// Progresses the topmost pending event to its next state
		void ProgressEvent(const unsigned char nState);

		// Processes incoming default response commands
		void OnDefaultResponse(const uint_fast8_t status,
			CZigBeeClusterRequest &request);

		// Timer call-back, which is invoked when the next transmission is due
		void OnNextTransmissionDue(void *pArgument);

#ifdef _DEBUG
		// Dumps extended IAS zone cluster information
		virtual void Dump() const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIASZoneDispatch

class CZigBeeClusterIASZoneDispatch
{
	// typedefs & enums
	public:

	// Attributes
	public:
		// Callback for setting preferred enrollment method
		bool (*m_setEnrollmentMethod)
			(CZigBeeApplicationSupportBase &aps, const uint_fast8_t method);

		// Callback for configuring transmission retry settings in case of
		// stalled IAS Zone status change notifications
		void (*m_setEventRetrySettings)(const uint_fast32_t maxBackoffTime,
			const uint_fast8_t baseBackoffTime,
			const uint_fast8_t backoffCommonRatio,
			const uint_fast8_t attemptLimit);

		// Callback for reverting transmission retry settings to the defaults
		void (*m_revertEventRetrySettings)();

	// Operations
	public:
		// Request setting preferred enrollment method
		static uint_fast8_t SetEnrollmentMethod
			(CZigBeeApplicationSupportBase &aps, const uint_fast8_t method);

		// Request configuring transmission retry settings in case of
		// stalled IAS Zone status change notifications
		static void SetEventRetrySettings(const uint_fast32_t maxBackoffTime,
			const uint_fast8_t baseBackoffTime,
			const uint_fast8_t backoffCommonRatio,
			const uint_fast8_t attemptLimit);

		// Request reverting transmission retry settings to the defaults
		static void RevertEventRetrySettings();
};


#define __C7BFX_IAS_ZONE_DELEGATE_IMPLEMENT_EX(id, ...) \
	__COMPACT_SECTION_IMPLEMENT(CZigBeeClusterIASZoneDispatch, \
		c7bfx_iaszd, id, __VA_ARGS__)

#define __C7BFX_IAS_ZONE_DELEGATE_IMPLEMENT(identifier, ...) \
	__C7BFX_IAS_ZONE_ENTITY_IMPLEMENT_EX( \
		__c7bfx_iaszd##identifier, identifier, \
		__VA_ARGS__)
