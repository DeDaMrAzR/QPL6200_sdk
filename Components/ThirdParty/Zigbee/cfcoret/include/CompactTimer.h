// CompactTimer.h : header file
//
// Copyright(C) 2008-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved. 
//
// www.ubisys.de
// support@ubisys.de
//
// Part of ubisys(R) Compact15.4(TM) IEEE 802.15.4 implementation
//
// Provides a timer service which has been particularly designed to function
// as a symbol timer for IEEE 802.15.4 devices. Therefore, the basic timer 
// unit is an integer multiple of the particular physical layer's symbol
// duration (16us corresponding to 62.5kHz for the 2.4GHz flavor). The 
// integer factor depends on the clock frequency used for the timer 
// peripheral

/////////////////////////////////////////////////////////////////////////////
// CCompactTimerClient

class CCompactTimerClient
{
};


/////////////////////////////////////////////////////////////////////////////
// CCompactTimer

class CCompactTimer
{
	// typedefs & enums
	public:
		// A non-static member function pointer to method of a 
		// CCompactTimerClient derived class
		typedef void (CCompactTimerClient::*MEMBERCALLBACK)(void *pArgument);
		
		// A global function or static member function pointer
		typedef void (*STATICCALLBACK)(void *pArgument);
		
	// Construction
	public:
		CCompactTimer(CCompactTimerClient &client, 
			const MEMBERCALLBACK pfnCallback, void *const pArgument, 
			const uint_fast64_t startTime, const uint_fast64_t timeSpan,
			CCompactTimer **const ppReference = 0);

		CCompactTimer(const STATICCALLBACK pfnCallback, 
			void *const pArgument, const uint_fast64_t startTime,
			const uint_fast64_t timeSpan, CCompactTimer **const ppReference = 0);
		
	// Attributes
	public:
		// Reference to the timer service client instance whose member function
		// is to be called. If null, a static member or global function is to 
		// be called
		CCompactTimerClient *const m_pClient;

		// Pointer to callback. Must be a non-static member function of a 
		// CIEEE802154TimerServiceClient derived class (m_pfnCallback), or a 
		// pointer to a global function or static member (m_pfnStaticCallback)
		union
		{
			MEMBERCALLBACK m_pfnCallback;
			STATICCALLBACK m_pfnStaticCallback;
		};

		// A user-defined argument that is passed to the timer callback
		void *const m_pArgument;
		
		// If the timer can be cancelled, this is a link to the instance 
		// pointer referencing this object, or null otherwise
		CCompactTimer **const m_ppReference;

		// Time when timer has been created (facilitates overflow-safe 
		// operation)
		const uint64_t m_startTime;
		
		// Time when timer is to be executed (elapsed time, relative to start)
		const uint64_t m_timeSpan;
		
	// Implementation
	public:

#ifdef _DEBUG
		// Dump timer object members
		void Dump(const int_fast64_t remainingTime) const;
#endif
		
};
