// CompactTimerService.h : header file
//
// Copyright(C) 2008-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Part of ubisys(R) CompactFramework(TM) library
//
// Provides a timer service with cancellable timers based on the AT91SAM
// periodic interval timer (PIT) peripheral, or the Cortex-M3 SysTick timer
// embedded into the core


/////////////////////////////////////////////////////////////////////////////
// CCompactTimerLess

class CCompactTimerLess
{
	public:
		// Compares two CCompactTimer objects, given their pointers, and
		// returns true if left->m_nTime < right->m_nTime. Used in the timer
		// service class in a multiset to achieve sorting according to due
		// time
		bool operator()(const CCompactTimer *const pLeft,
			const CCompactTimer *const pRight) const;
};


/////////////////////////////////////////////////////////////////////////////
// CCompactTimerService

class CCompactTimerService
{
	// typedefs & enums
	public:
		// Data type for absolute tick counts. In the present implementation
		// of the timer service this is always 64-bits
		typedef uint_fast64_t ticks_t;

		// Data type for difference between two tick counts. In the present
		// implementation of the timer service this is always 64-bits
		typedef int_fast64_t ticks_diff_t;

	// Construction
	public:
		CCompactTimerService(const unsigned int nInterval = 48,
			const bool bConfigureInterrupt = false,
			const unsigned int nPriority = 1);

	// Attributes
	public:
		// Number of ticks per second (62500), as a 32-bit value
		static const uint_fast32_t ticksPerSecond;

		// Maximum tick difference that can be expressed (2^63 - 1)
		static const CCompactTimerService::ticks_diff_t defaultTimeout;

	// Operations
	public:
		// Configures SAM7 peripherals for use within the timer service class.
		// If you do not currently make use of the system interrupt, call the
		// method with bConfigureInterrupt set to true. Otherwise you must call
		// OnInterrupt from within your existing service routine.
		// The interval is specified in multiples of 16 master clock cycles
		static void Configure(const unsigned int nInterval = 48);

		// Returns the current time with best accuracy. Notice that the value
		// returned by GetCurrenTime() does not change, if interrupts are
		// disabled, or the system interrupt is not executed due to other
		// interrupts with higher priority being active
		static ticks_t GetCurrentTime();

		// Returns the current time with best accuracy. Notice that the value
		// returned by PollCurrenTime() does change, if interrupts are
		// disabled. Does not cause the execution of due timers from within the
		// calling thread's execution context. It will also not update the
		// ticks returned by GetCurrentTime()
		static ticks_t PollCurrentTime();

		// Polls the current time and sets
		static void UpdateCurrentTime();

		// Sets the current time. Does not modify pending timers, i.e. the
		// caller should cancel all pending timers prior to changing the
		// absolute time
		static void SetCurrentTime(const ticks_t time);

		// Creates a new timer. The specified timer callback is executed from
		// interrupt context when the specified timeout has expired. If a
		// reference pointer is provided, once the timer callback has been
		// invoked, the reference is nulled. Until then, the timer might be
		// cancelled. The location of the reference must persist for the
		// lifetime of the timer.
		// This version is for non-static member functions
		void CreateTimer(const ticks_diff_t timeSpan,
			CCompactTimerClient &client,
			CCompactTimer::MEMBERCALLBACK pfnCallback,
			void *pArgument = 0, CCompactTimer **const ppReference = 0);

		// Creates a new timer. The specified timer callback is executed from
		// interrupt context when the specified timeout has expired. If a
		// reference pointer is provided, once the timer callback has been
		// invoked, the reference is nulled. Until then, the timer might be
		// cancelled. The location of the reference must persist for the
		// lifetime of the timer.
		// This version is for global functions or static member functions
		void CreateTimer(const ticks_diff_t timeSpan,
			CCompactTimer::STATICCALLBACK pfnCallback, void *pArgument = 0,
			CCompactTimer **const ppReference = 0);

		// Cancels a pending timer. Returns true, if the timer was still
		// pending and canceled, false otherwise
		bool CancelTimer(CCompactTimer *&pTimer);

		// Returns the time remaining before the timer is fired. A negative
		// value indicates that the timer is overdue
		ticks_diff_t GetRemainingTime(const CCompactTimer &timer) const;

		// Returns the time that elapsed since the timer was started
		ticks_diff_t GetElapsedTime(const CCompactTimer &timer) const;

		// Returns the absolute time when the timer is fired.
		// Note that this might include an overflow, i.e. the returned
		// value might be lower than the current time.
		ticks_t GetDueTime(const CCompactTimer &timer) const;

		// Returns the remaining time of the first timer due, if any. A safe
		// duration to wait as a fall-back otherwise; this is half the period
		// that can be handled without causing an arithmetic overflow
		ticks_diff_t GetRemainingTime() const;

		// Returns the absolute due time of the first timer due, if any. A safe
		// duration to wait as a fall-back otherwise; this is half the period
		// that can be handled without causing an arithmetic overflow
		ticks_t GetNextDueTime() const;

		// Calls GetCurrentTime() repeatedly until the specified delay has
		// elapsed. Accounts for counter overflow. The PIT timer must be able
		// to execute and update the major timer
		static void Sleep(const uint_fast32_t delay);

		// Calls PollCurrentTime() repeatedly until the specified delay has
		// elapsed. Accounts for counter overflow. The PIT timer need not be
		// able to execute, but timer call-backs might be invoked from the
		// current thread of execution
		static void SleepEx(const uint_fast32_t delay);

		// Returns true the specified delay has elapsed. Accounts for counter
		// overflows. Can be used to check for timeouts in loops, for example
		static bool HasElapsed(const ticks_t start,
			const ticks_diff_t timeSpan, const ticks_t currentTime);

		// Calculates timer service ticks corresponding to the specified time
		// in microseconds. Notice that this calculation employs a division
		static ticks_t CalculateTicks(const uint_fast64_t nMicroseconds);

		// Returns the current timer object. Must only be called by the timer
		// callback that is currently running
		const CCompactTimer &GetActiveTimer() const;

	// Implementation
	protected:
		// Timer value. This value stores the most recent tick count and is
		// maintained by a clock source, e.g. a hardware timer and associated
		// interrupt service routine, or an operating system timer. For the
		// accurate current time value, the ticks elapsed between the time
		// this value was stored/updated and the present time need to be taken
		// into account as well, e.g. by adding the current value of the
		// hardware timer since the update
		static ticks_t ticks;

		// Current time value. This value is captured from the raw ticks value
		// above when UpdateCurrentTime() is used. It is the value that will be
		// returned by subsequent calls to GetCurrentTime(). Allows to freeze
		// the current time within runs of an event-loop, for example
		static ticks_t currentTime;

		// A pointer to the timer which is currently being executed, or null.
		// Can be used by the timer callback function to retrieve information
		// about the timer, e.g. the start time
		const CCompactTimer *m_pActiveTimer;

		// A set of timer objects holding information about timers, callbacks
		// their due time and argument
		std::multiset<CCompactTimer *, CCompactTimerLess> m_timers;

		// Called by CreateTimer() variants to insert the newly created timer
		// into the set of pending timers
		void AddTimer(CCompactTimer *pTimer);

		// Called to remove a timer from the set of pending timers; does not
		// delete the CCompactTimer object. By default, will also reschedule
		// the underlying system timer, if necessary
		void RemoveTimer(std::multiset<CCompactTimer *,
			CCompactTimerLess>::iterator t, const bool reschedule = true);

		// Called to remove a timer from the set of pending timers
		void DeleteTimer(std::multiset<CCompactTimer *,
			CCompactTimerLess>::iterator t);

	public:
		// Invoked whenever a new timer is added which happens to be the first
		// timer due. May be used to re-schedule a hardware timer etc.
		// Return true if there is at least one timer due now.
		bool Reschedule();

		// Checks if any timer is due and ready for execution. If so, executes
		// the corresponding callback(s)
		void CheckAndRunTimerCallbacks();

	public:
		// Handles the system interrupt, a shared interrupt that is used by the
		// PIT, DBGU and other peripherals
		static void OnInterrupt(void *pArgument);

#ifdef _DEBUG
		// Shows a list of pending timers
		void Dump() const;

		// Dumps a timer (calculates remaining time)
		void Dump(const CCompactTimer *const pTimer) const;
#endif

};


/////////////////////////////////////////////////////////////////////////////
// One and only instance of CTimerService class

extern CCompactTimerService theTimerService;
