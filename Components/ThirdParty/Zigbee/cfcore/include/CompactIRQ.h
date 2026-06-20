// CompactIRQ.h : header file
//
// Copyright(C) 2008 - 2015 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Provides an easy-to-use interface to the AT91SAM external interrupt request
// peripheral. Use CCompactIO to configure the appropriate pin in peripheral
// mode


/////////////////////////////////////////////////////////////////////////////
// CCompactIRQ

class CCompactIRQ
{
	// typedefs & enums
	public:
		// Service routine called through a wrapper
		typedef void(*SERVICEROUTINEEX)(void *pArgument);

		// Service routine called directly by interrupt logic
		typedef void(*SERVICEROUTINE)();

		// Configuration flags
		enum { defaultFlags, autoEnable = 0x01 };

	// Attributes
	public:
#ifndef __ARM_PROFILE_M__
		// Start-up/default service routine (not required on Cortex-M3/M4)
		SERVICEROUTINE m_pfnServiceRoutine;
#endif

		// IRQ peripheral ID. A signed value facilitates CMSIS integration
		signed char m_nPeripheralID;

		// IRQ priority
		unsigned char m_nPriority;

		// IRQ source type (edge/level sensitivity)
		unsigned char m_nType;

		// Additional flags
		unsigned char m_bFlags;

	// Operations
	public:
		// Configures the interrupt according to its default start-up
		// configuration
		void Configure() const;

		// Configures the interrupt with an arbitrary service routine and
		// priority. On Cortex-M cores, only the priority can be changed
		// dynamically
#ifdef __ARM_PROFILE_M__
		void Configure(const unsigned int nPriority) const;
#else
		// Default type: 0 = internal, level sensitive, i.e.
		// AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE (SAM9263) or
		// AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL (SAM7S, SAM7X)
		void Configure(const SERVICEROUTINE pfnServiceRoutine,
			const unsigned int nPriority,
			const unsigned int nType = 0) const;
#endif

		// Configures all interrupts that have been declared via the
		// __COMPACTIRQ_EX macro and configures a default handler for all
		// others. Should be called by start-up code.
		static void ConfigureAll();

		// Enables the interrupt
		void Enable() const;

		// Disables (and clears) the interrupt
		void Disable() const;

		// Enables the interrupt
		bool IsEnabled() const;

		// Returns true if the interrupt is pending
		bool IsPending() const;

		// Allows software to trigger an AIC interrupt
		void Trigger() const;

		// Allows software to clear an AIC interrupt
		void Clear() const;

		// Might be called from within an interrupt service routine to
		// determine the currently active interrupt source (AIC channel)
		static unsigned char GetActiveSource();

		// Returns true, if code is currently excecuted from interrupt context
		static bool IsInterruptContext();

		// Configures the specified interrupt channel
#ifdef __ARM_PROFILE_M__
		static void Configure(const unsigned int nID,
			const unsigned int nPriority);
#else
		static void Configure(const unsigned int nID,
			const unsigned int nPriority, const unsigned int nSourceType,
			const SERVICEROUTINE pfnService);
#endif

#ifndef __ARM_PROFILE_M__
		// Interrupt service routine for the shared system peripheral interrupt
		static void OnSystemInterrupt(void *pArgument);

		// Default handler for spurious interrupts
		static void OnSpuriousInterrupt();
#endif

	// Implementation
	public:
		// Number of interrupt sources. Notice: these are only meant to cover
		// peripheral interrupts, exlcuding core interrupts
#ifdef __COMPACT_SAM3S
		enum { peripherals = 35, core = 16, interrupts = peripherals + core };
#elif (defined __COMPACT_SAM4S)
		enum { peripherals = 35, core = 16, interrupts = peripherals + core };
#elif (defined __COMPACT_SAM)
		enum { peripherals = 32, core = 0, interrupts = peripherals + core };
#elif (defined __COMPACT_K8A)
		enum { peripherals = 31, core = 16, interrupts = peripherals + core };
#elif (defined __COMPACT_K8E)
		enum { peripherals = 33, core = 16, interrupts = peripherals + core };
#elif (defined __COMPACT_XP4001)
		enum { peripherals = 37, core = 16, interrupts = peripherals + core };
#else
#error Unsupported device
#endif

#ifdef __ARM_PROFILE_M__
		// Exception numbers of Cortex-M processors. While fully defined by ARM
		// some chip manufacturers do not include all definitions in their 
		// CMSIS deliveries, so we define the core ones here (again)
		enum { irqnNonMaskable = -14, irqnHardFault = -13,
			irqnMemoryManagement = -12, irqnBusFault = -11, irqnUsageFault = -10,
			irqnSecureFault = -9, irqnSVC = -5, irqnDebugMonitor = -4,
			irqnPendSV = -2, irqnSysTick = -1 };
#endif
		
		// Default handler for otherwise unconfigured interrupts
		static void OnInterrupt();

#ifdef __ARM_PROFILE_M__
		// Default handler for otherwise unconfigured interrupts
		static void OnInterruptEx(const unsigned int *const pStackFrame);
#endif

};


inline bool CCompactIRQ::IsPending() const
{
#ifdef __ARM_PROFILE_M__
	// Notice: NVIC_GetPendingIRQ() can only be used for periheral interrupts,
	// not for Cortex-M3/M4 core exceptions
	return NVIC_GetPendingIRQ(static_cast<IRQn_Type>(m_nPeripheralID)) ?
		true : false;
#else
	return (AT91C_BASE_AIC->AIC_IPR & (1u << m_nPeripheralID)) ?
		true : false;
#endif
}


inline void CCompactIRQ::Trigger() const
{
#ifdef __ARM_PROFILE_M__
	NVIC->STIR = m_nPeripheralID;
#else
	AT91C_BASE_AIC->AIC_ISCR = 1u << m_nPeripheralID;
#endif
}


inline void CCompactIRQ::Clear() const
{
#ifdef __ARM_PROFILE_M__
	NVIC_ClearPendingIRQ(static_cast<IRQn_Type>(m_nPeripheralID));
#else
	AT91C_BASE_AIC->AIC_ICCR = 1u << m_nPeripheralID;
#endif
}


inline void CCompactIRQ::Enable() const
{
#ifdef __ARM_PROFILE_M__
	NVIC_EnableIRQ(static_cast<IRQn_Type>(m_nPeripheralID));
#else
	AT91C_BASE_AIC->AIC_IECR = 1u << m_nPeripheralID;
#endif
}


inline void CCompactIRQ::Disable() const
{
	// Disable...
#ifdef __ARM_PROFILE_M__
	NVIC_DisableIRQ(static_cast<IRQn_Type>(m_nPeripheralID));
#else
	AT91C_BASE_AIC->AIC_IDCR = 1u << m_nPeripheralID;
#endif

	// ... and clear pending
   Clear();

	ASSERT(!IsPending());
}


inline bool CCompactIRQ::IsEnabled() const
{
#ifdef __ARM_PROFILE_M__
	return (NVIC->ISER[m_nPeripheralID >> 5] & (1 << m_nPeripheralID)) ?
		true : false;
#else
	return (AT91C_BASE_AIC->AIC_IMR & (1u << m_nPeripheralID)) ? true : false;
#endif
}


inline unsigned char CCompactIRQ::GetActiveSource()
{
#ifdef __ARM_PROFILE_M__
	return __get_IPSR();
#else
	return AT91C_BASE_AIC->AIC_ISR & 0x1f;
#endif
}


inline bool CCompactIRQ::IsInterruptContext()
{
#ifdef __ARM_PROFILE_M__
	return __get_IPSR() ? true : false;
#else
	const unsigned int nStatus = AT91C_BASE_AIC->AIC_ISR;
	const unsigned int nPending = AT91C_BASE_AIC->AIC_IPR;

	return (nStatus & 0x1f) || (nPending & (1 << AT91C_ID_FIQ));
#endif
}


#ifdef __ARM_PROFILE_M__

inline void CCompactIRQ::Configure(const unsigned int nID,
	const unsigned int nPriority)
{
	// Disable interrupt
	// NVIC_DisableIRQ(static_cast<IRQn_Type>(nID));

	// The priority for Cortex-M NVIC is defined such that lower numbers have
	// higher priority, e.g. 0 = highest user-configurable priority, and
	// (1 << __NVIC_PRIO_BITS) - 1 is the lowest. Atmel's AIC uses different
	// the inverse scheme: lower value, meaning lower priority, so we have to
	// unify the priority level and map to a common scheme
	NVIC_SetPriority(static_cast<IRQn_Type>(nID),
		(1 << __NVIC_PRIO_BITS) - 1 - nPriority);

	// Clear interrupt
	// NVIC_ClearPendingIRQ(static_cast<IRQn_Type>(nID));
}

#else

inline void CCompactIRQ::Configure(const unsigned int nID,
	const unsigned int nPriority, const unsigned int nSourceType,
	const SERVICEROUTINE pfnService)
{
	const unsigned int nMask = 1u << nID;

	// Disable interrupt
	// AT91C_BASE_AIC->AIC_IDCR = nMask;

	// Store vector, source type and priority
	AT91C_BASE_AIC->AIC_SVR[nID] = reinterpret_cast<unsigned int>(pfnService);
	AT91C_BASE_AIC->AIC_SMR[nID] = nSourceType | nPriority;

	// Clear interrupt
	AT91C_BASE_AIC->AIC_ICCR = nMask;
}

#endif


/////////////////////////////////////////////////////////////////////////////
// CCompactIRQ macros

#ifdef __ARM_PROFILE_M__
#define __COMPACTIRQ_DECLARE(id, priority, type, flags, service) \
	{ id, priority, 0, flags }
#else
#define __COMPACTIRQ_DECLARE(id, priority, type, flags, service) \
	{ service, id, priority, type, flags }
#endif

#define __COMPACTIRQ_EX(obj, id, priority, type, flags, service, argument) \
	const CCompactIRQ::SERVICEROUTINEEX __cirq_pfnServiceEx_##id = service; \
	void *const __cirq_pArgument_##id = argument; \
	void __cirq_OnInterrupt_##id() \
	{ \
		__cirq_pfnServiceEx_##id(__cirq_pArgument_##id); \
	} \
	_Pragma("location=\".cfcore.cirq.init\"") \
	_Pragma("language=extended") \
	__root const CCompactIRQ obj = __COMPACTIRQ_DECLARE(__CFCORE_ID(id), \
		priority, type, flags, &__cirq_OnInterrupt_##id); \
	_Pragma("language=default")

#ifdef __ARM_PROFILE_M__
#define __COMPACTIRQ_HANDLER(obj, irq, id, priority, flags, service, argument) \
	const CCompactIRQ::SERVICEROUTINEEX __cfhandler_pfnServiceEx_##irq = service; \
	void *const __cfhandler_pArgument_##irq = argument; \
	void __cfhandler_##irq() \
	{ \
		__cfhandler_pfnServiceEx_##irq(__cfhandler_pArgument_##irq); \
	} \
	_Pragma("location=\".cfcore.cirq.init\"") \
	_Pragma("language=extended") \
	__root const CCompactIRQ obj = \
		__COMPACTIRQ_DECLARE(id, priority, 0, flags, \
			&__cfhandler_##irq); \
	_Pragma("language=default")
#endif


/////////////////////////////////////////////////////////////////////////////
// CCompactSystemIRQ

class CCompactSystemIRQ
{
	// typedefs & enums
	public:
		// Service routine called through a wrapper
		typedef void(*SERVICEROUTINE)(void *pArgument);

	// Attributes
	public:
		// Start-up/default service routine
		SERVICEROUTINE m_pfnServiceRoutine;

		// Argument to the service routine
		void *m_pArgument;
};


/////////////////////////////////////////////////////////////////////////////
// CCompactSystemIRQ macros

#define __COMPACTIRQ_SYSTEM(obj, service, argument) \
	_Pragma("location=\".cfcore.cirq.sys\"") \
	_Pragma("language=extended") \
	__root const CCompactSystemIRQ obj = { service, argument }; \
	_Pragma("language=default")

#if (defined __COMPACT_SAM3S || defined __COMPACT_SAM4S)
#define __COMPACTIRQ_RTT(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_EX(obj, RTT, priority, type, flags, service, argument)
#define __COMPACTIRQ_RSTC(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_EX(obj, RSTC, priority, type, flags, service, argument)
#define __COMPACTIRQ_WDT(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_EX(obj, WDT, priority, type, flags, service, argument)
#define __COMPACTIRQ_PMC(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_EX(obj, PMC, priority, type, flags, service, argument)
#define __COMPACTIRQ_DBGU(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_EX(obj, UART0, priority, type, flags, service, argument)
#else
#define __COMPACTIRQ_RTT(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_SYSTEM(obj, service, argument)
#define __COMPACTIRQ_RSTC(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_SYSTEM(obj, service, argument)
#define __COMPACTIRQ_WDT(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_SYSTEM(obj, service, argument)
#define __COMPACTIRQ_PMC(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_SYSTEM(obj, service, argument)
#define __COMPACTIRQ_DBGU(obj, priority, type, flags, service, argument) \
	__COMPACTIRQ_SYSTEM(obj, service, argument)
#endif
