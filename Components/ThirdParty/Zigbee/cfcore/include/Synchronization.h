// CriticalSection.h : header file
//
// Copyright(C) 2008-2015 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Implements a critical section for thread synchronization and a plain, i.e.
// single-level, interrupt lock.
//
// Plain interrupt locks - unlike critical sections - cannot be nested and
// should be used very carefully and with limited scope; for example,
// disabling interrupts while on-chip flash memory is being erased and
// programmed is a case where a CPlainInterruptLock() is required, instead of
// a CCriticalSectionLock, depending on the target platform (e.g. Cortex-M)


/////////////////////////////////////////////////////////////////////////////
// CCriticalSection

class CCriticalSection
{
	// Attributes
	public:
		// Returns true, if the critical section is currently locked (by the
		// calling thread)
		static bool IsLocked();

	// Operations
	public:
		// Enters the critical section. This method blocks until the thread is
		// able to acquire a lock on the critical section object. On Cortex-M3,
		// M4 and M7 devices, this sets the base priority such that exceptions
		// where the MSB in the priority is not set, won't occur. On Cortex M0,
		// Cortex M0+, ARM7TDMI and ARM926EJ-S this will disable interrupts
		static void Lock();

		// Leaves the critical section, allowing other threads to enter
		static void Unlock();

		// Enables interrupts without affecting the spin counter. Can be used
		// to initially enable interrupts after startup
		static void Enable();

		// Disables interrupts without affecting the spin counter
		static void Disable();

		// Returns true, if interrupts are currently enabled
		static bool IsInterruptEnabled();

	protected:
		// Spin counter (supports multiple entries from within the same thread)
		static volatile unsigned int nCount;

		static volatile bool bEnabled;
};

inline bool CCriticalSection::IsLocked()
	{ return bEnabled ? (nCount ? true : false) : true; }


/////////////////////////////////////////////////////////////////////////////
// CCriticalSectionLock

class CCriticalSectionLock
{
	// Construction
	public:
		CCriticalSectionLock();

	// Implementation
	public:
		~CCriticalSectionLock();
};

inline CCriticalSectionLock::CCriticalSectionLock()
	{ CCriticalSection::Lock(); }

inline CCriticalSectionLock::~CCriticalSectionLock()
	{ CCriticalSection::Unlock(); }


/////////////////////////////////////////////////////////////////////////////
// CPlainInterruptLock

class CPlainInterruptLock
{
	// Construction
	public:
		CPlainInterruptLock();

	// Implementation
	public:
		~CPlainInterruptLock();

	// Attributes
	protected:
		// Stores the interrupt lock state before applying the lock
		const unsigned long m_originalState;
};
