// CompactDebug.h : header file
//
// Copyright(C) 2006-2022 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Implements cross-platform debug input and output, heap and stack info,
// etc. Feature support varies depending on platform, toolchain and runtime
// library
//
// For IAR C++: Add --no_path_in_file_macros to compiler command line
// options, to remove path names from __FILE__ macro

#ifdef _DEBUG

/////////////////////////////////////////////////////////////////////////////
// forward references

class CHeapCell;


/////////////////////////////////////////////////////////////////////////////
// CHeapDebug

class CHeapDebug
{
	// typedefs & enums
	public:
		enum { validate = 0, updateStatistics = 0x01, dumpCells = 0x02,
			dumpFree = 0x04, dumpUsed = 0x08, dumpData = 0x10,
			assertValid = 0x80 };

		typedef unsigned int size_type;

	// Construction
	public:
		CHeapDebug();

	// Attributes
	public:
		// Number of used blocks
		unsigned int m_nBlocksUsed;

		// Size of largest used block
		unsigned int m_nLargestUsed;

		// Total amount of used heap memory
		unsigned int m_nTotalUsed;

		// Number of free blocks
		unsigned int m_nBlocksFree;

		// Size of largest free block
		unsigned int m_nLargestFree;

		// Total amount of available memory
		unsigned int m_nTotalFree;

	// Operations
	public:
		// Walks the heap and updates statistics, dumps free and/or used
		// blocks. Call once with updateStatistics set to true before using
		// Dump()
		bool Walk(const unsigned int nFlags = updateStatistics | assertValid);

		// Dumps previously collected statistics
		void Dump() const;

		// Returns the heap size
		static unsigned int GetHeapSize();

		// Checks if the specified pointer corresponds to a valid heap block
		// of at least the specified size
		static bool IsValid(const void *pBlock, const size_type = 0,
			const bool bAdvanced = true);

		// Checks if the specified pointer corresponds to a valid heap block
		// of at least the specified size
		static bool IsValid(const CHeapCell *const pCell);

		// Returns true, if the heap has not been used, and hence, has not been
		// initialized yet
		static bool IsEmpty();

	// Implementation
	public:

#ifdef __COMPACT_DEBUG_HEAP
		// Magic value (advanced debugging)
		enum { magic = 0xfacecafe };

		// Offset (in terms of heap cell sizes) from cell to user block
		enum { blockOffset = 2 };
#else
		// Offset (in terms of heap cell sizes) from cell to user block
		enum { blockOffset = 1 };
#endif

		// Returns true if the specified cell is available
		static bool IsAvailable(const CHeapCell *const pQuery);
};


/////////////////////////////////////////////////////////////////////////////
// CStackDebug
//
// Add this to your startup file, for example before/after stack pointer
// initialization:
// #ifdef _DEBUG
//		extern	_ZN11CStackDebug16InitializeStacksEv
//		bl			_ZN11CStackDebug16InitializeStacksEv
// #endif

class CStackDebug
{
	// typedefs & enums
	public:
#ifdef __ARM_PROFILE_M__
		// Stacks used by the present firmware application (Cortex-M versions
		// without separate process stack)
		enum { stackMain, stackCount };
#else
		// Stacks used by the present firmware application (ARM7/ARM9 versions
		// without FIQ stack)
		enum { stackSVC, stackIRQ, stackCount };
#endif

	// Operations
	public:
		// Called by startup code to initialize stacks with the magic value
		static void InitializeStacks();

		// Returns the highest number of bytes ever used on the specified stack
		// since program startup
		static unsigned int DetermineMaximumUsage(const unsigned int nStack);

		// Returns the highest number of bytes ever used on the specified stack
		// since program startup
		static unsigned int DetermineCurrentUsage(const unsigned int nStack);

		// Returns the size of the specified stack
		static unsigned int GetStackSize(const unsigned int nStack);

	// Implementation
	public:
		// Magic value used for comparisons. Memory locations that have
		// different content will be regarded as "once used"
		enum { magic = 0xa55a1fee };

		// Indexes into stack info array
		enum { end, begin };

		// Stacks that have been used. Currently SVC and IRQ stack
		static const unsigned int *const aapStacks[stackCount][2];

		// Initializes a single stack
		static void InitializeStack(unsigned int *pEnd, unsigned int *pBegin);

		// Returns current stack pointer value
		static unsigned int *GetStackPointer();
};


/////////////////////////////////////////////////////////////////////////////
// CDebugUnitCommand

class CDebugUnitCommand
{
	// typedefs & enums
	public:
		// Command handler prototype
		typedef void (*HANDLER)(const unsigned int nArguments,
			const char *const apszArguments[]);

	// Attributes
	public:
		// Command mnemonic
		const char *m_pszCommand;

		// Command handler
		HANDLER m_pfnHandler;

		// Execution context (system interrupt or application)
		bool m_bApplicationContext;
};


/////////////////////////////////////////////////////////////////////////////
// CDebugUnitCommand macros

#ifdef __COMPACT_LINKER_IAR__
#define __DECLARE_DEBUG_COMMAND_EX(id, command, handler, context) \
	_Pragma("location=\".cfcore.dbg.cmds\"") \
	_Pragma("language=extended") \
	__root static const CDebugUnitCommand id = { command, handler, context }; \
	_Pragma("language=default")
#define __DEBUG_COMMANDS_BEGIN() \
	_Pragma("section=\".cfcore.dbg.cmds\"") \
	_Pragma("language=extended") \
	reinterpret_cast<const CDebugUnitCommand *> \
	(__section_begin(".cfcore.dbg.cmds")) \
	_Pragma("language=default")
#define __DEBUG_COMMANDS_END() \
	_Pragma("section=\".cfcore.dbg.cmds\"") \
	_Pragma("language=extended") \
	reinterpret_cast<const CDebugUnitCommand *> \
	(__section_end(".cfcore.dbg.cmds")) \
	_Pragma("language=default")
#elif (defined __COMPACT_LINKER_LLVM__)
extern const CDebugUnitCommand __start_cfcore_dbg_cmds
	__asm("section$start$cfcore_dbg_cmds$CFCORE_DBG_CMDS");
extern const CDebugUnitCommand __stop_cfcore_dbg_cmds
	__asm("section$end$cfcore_dbg_cmds$CFCORE_DBG_CMDS");
__attribute__((weak)) extern const CDebugUnitCommand __start_cfcore_dbg_cmds, __stop_cfcore_dbg_cmds;
#define __DECLARE_DEBUG_COMMAND_EX(id, command, handler, context) \
	static const __attribute__((section("cfcore_dbg_cmds,CFCORE_DBG_CMDS"))) \
	__attribute__((used, retain)) \
	CDebugUnitCommand id = { command, handler, context };
#define __DEBUG_COMMANDS_BEGIN() \
	&__start_cfcore_dbg_cmds
#define __DEBUG_COMMANDS_END() \
	&__stop_cfcore_dbg_cmds
#elif (defined __COMPACT_LINKER_GOLD__)
extern const CDebugUnitCommand __start_cfcore_dbg_cmds;
extern const CDebugUnitCommand __stop_cfcore_dbg_cmds;
__attribute__((weak)) extern const CDebugUnitCommand __start_cfcore_dbg_cmds, __stop_cfcore_dbg_cmds;
#define __DECLARE_DEBUG_COMMAND_EX(id, command, handler, context) \
	static const __attribute__((used, retain, section("cfcore_dbg_cmds"))) \
	CDebugUnitCommand id = { command, handler, context };
#define __DEBUG_COMMANDS_BEGIN() \
	&__start_cfcore_dbg_cmds
#define __DEBUG_COMMANDS_END() \
	&__stop_cfcore_dbg_cmds
#elif (defined __COMPACT_LINKER_SEGGER__)
extern const CDebugUnitCommand cfcore_dbg_cmds_start__;
extern const CDebugUnitCommand cfcore_dbg_cmds_end__;
__attribute__((weak)) extern const CDebugUnitCommand cfcore_dbg_cmds_start__, cfcore_dbg_cmds_end__;
#define __DECLARE_DEBUG_COMMAND_EX(id, command, handler, context) \
	static const __attribute__((used, retain, section("cfcore_dbg_cmds"))) \
	CDebugUnitCommand id = { command, handler, context };
#define __DEBUG_COMMANDS_BEGIN() \
	&cfcore_dbg_cmds_start__
#define __DEBUG_COMMANDS_END() \
	&cfcore_dbg_cmds_end__
#else
#error Unsupported toolchain/linker
#endif

#define __DECLARE_DEBUG_COMMAND(command, handler, context) \
	__DECLARE_DEBUG_COMMAND_EX(__cmd##handler, command, &handler, context)


/////////////////////////////////////////////////////////////////////////////
// CDebugUnitPromptItem

class CDebugUnitPromptItem
{
	// typedefs & enums
	public:
		// Command handler prototype
		typedef void (*HANDLER)(const bool bPriority);

	// Attributes
	public:
		// Command handler
		HANDLER m_pfnHandler;

		// Prompt
		const char *m_pszPrompt;

		// Command prompt key
		char m_chKey;

		// Command prompt accelerator
		char m_chPriorityKey;
};


/////////////////////////////////////////////////////////////////////////////
// CDebugUnitPromptItem macros

#ifdef __COMPACT_LINKER_IAR__
#define __DECLARE_DEBUG_PROMPTITEM_EX(id, key, priorityKey, prompt, handler) \
	_Pragma("location=\".cfcore.dbg.items\"") \
	_Pragma("language=extended") \
	__root static const CDebugUnitPromptItem id = { handler, prompt, key, \
		priorityKey }; \
	_Pragma("language=default")
#define __DEBUG_ITEMS_BEGIN() \
	_Pragma("section=\".cfcore.dbg.items\"") \
	_Pragma("language=extended") \
	reinterpret_cast<const CDebugUnitPromptItem *> \
	(__section_begin(".cfcore.dbg.items")) \
	_Pragma("language=default")
#define __DEBUG_ITEMS_END() \
	_Pragma("section=\".cfcore.dbg.items\"") \
	_Pragma("language=extended") \
	reinterpret_cast<const CDebugUnitPromptItem *> \
	(__section_end(".cfcore.dbg.items")) \
	_Pragma("language=default")
#elif (defined __COMPACT_LINKER_LLVM__)
extern const CDebugUnitPromptItem __start_cfcore_dbg_items
	__asm("section$start$cfcore_dbg_items$CFCORE_DBG_ITEMS");
extern const CDebugUnitPromptItem __stop_cfcore_dbg_items
	__asm("section$end$cfcore_dbg_items$CFCORE_DBG_ITEMS");
__attribute__((weak)) extern const CDebugUnitPromptItem __start_cfcore_dbg_items, __stop_cfcore_dbg_items;
#define __DECLARE_DEBUG_PROMPTITEM_EX(id, key, priorityKey, prompt, handler) \
	static const __attribute__((section("cfcore_dbg_items,CFCORE_DBG_ITEMS"))) \
	__attribute__((used, retain)) \
	CDebugUnitPromptItem id = { handler, prompt, key, priorityKey };
#define __DEBUG_ITEMS_BEGIN() \
	&__start_cfcore_dbg_items
#define __DEBUG_ITEMS_END() \
	&__stop_cfcore_dbg_items
#elif (defined __COMPACT_LINKER_GOLD__)
extern const CDebugUnitPromptItem __start_cfcore_dbg_items;
extern const CDebugUnitPromptItem __stop_cfcore_dbg_items;
__attribute__((weak)) extern const CDebugUnitPromptItem __start_cfcore_dbg_items, __stop_cfcore_dbg_items;
#define __DECLARE_DEBUG_PROMPTITEM_EX(id, key, priorityKey, prompt, handler) \
	static const __attribute__((used, retain, section("cfcore_dbg_items"))) \
	CDebugUnitPromptItem id = { handler, prompt, key, priorityKey };
#define __DEBUG_ITEMS_BEGIN() \
	&__start_cfcore_dbg_items
#define __DEBUG_ITEMS_END() \
	&__stop_cfcore_dbg_items
#elif (defined __COMPACT_LINKER_SEGGER__)
extern const CDebugUnitPromptItem cfcore_dbg_items_start__;
extern const CDebugUnitPromptItem cfcore_dbg_items_end__;
__attribute__((weak)) extern const CDebugUnitPromptItem cfcore_dbg_items_start__, cfcore_dbg_items_end__;
#define __DECLARE_DEBUG_PROMPTITEM_EX(id, key, priorityKey, prompt, handler) \
	static const __attribute__((used, retain, section("cfcore_dbg_items"))) \
	CDebugUnitPromptItem id = { handler, prompt, key, priorityKey };
#define __DEBUG_ITEMS_BEGIN() \
	&cfcore_dbg_items_start__
#define __DEBUG_ITEMS_END() \
	&cfcore_dbg_items_end__
#else
#error Unsupported toolchain/linker
#endif

#define __DECLARE_DEBUG_PROMPTITEM(key, priorityKey, prompt, handler) \
	__DECLARE_DEBUG_PROMPTITEM_EX(__item##handler, key, priorityKey, prompt, \
		&handler)


/////////////////////////////////////////////////////////////////////////////
// CDebugUnit default buffer sizes

#ifndef __COMPACT_DEBUG_MESSAGE_SIZE
#define __COMPACT_DEBUG_MESSAGE_SIZE 512
#endif

#ifndef __COMPACT_DEBUG_COMMAND_SIZE
#define __COMPACT_DEBUG_COMMAND_SIZE 80
#endif


/////////////////////////////////////////////////////////////////////////////
// CDebugUnit

class CDebugUnit
{
	// typedefs & enums
	public:
		// Default mask value for trace messages
		enum { maskFacility = 0xfffffff8, maskLevel = 0x00000007,
			facilityAll = maskFacility, facilityException = 0x80000000,
			facilityAssertion = 0x40000000, facilityCore = 0x20000000,
			facilityApplication = 0x00000008,
			maskDefault = facilityApplication | 1 };

	// Construction
	public:
		CDebugUnit();

	// Attributes
	public:
		// If this is set to 1, echo received characters
		static const bool bEcho;

	// Operations
	public:
		// Causes a system reset
		static void SystemReset();

		// Displays the reason for a recent reset
		static void DumpResetCause();

		// Dumps interrupt priorities, source modes, and vectors
		static void DumpInterrupts();

		// Displays heap information
		static void DumpHeapInfo(const bool bDetails = true,
			const bool bWithData = false);

		// Displays stack information
		static void DumpStackInfo();

		// Causes a system halt. If reset is allowed, pressing the "r" key on
		// the terminal program causes a reset. If continue is allowed,
		// pressing a "c" lets normal program flow continue. Any other input
		// re-prints the most recent output message
		static void SystemHalt(const bool bAllowContinue = true,
			const bool bAllowReset = true);

		// Called when an assertion has failed
		static void OnAssertionFailed(const char *const pszFile,
			const unsigned int nLine);

		// Configure SAM7 peripherals (acquire PIO lines, setup USART)
		static void Configure();

		// Output a string, character by character
		static void OutputString(const char *pszMessage);

		// Returns true, if a particular output should appear (based on
		// facility and level)
		static bool IsOutputSelected(const unsigned int nMask);

		// Output a string (use as printf)
#ifdef __IAR_SYSTEMS_ICC__
#pragma __printf_args
#endif
		static void OutputStringEx(const unsigned int nMask,
			const char *pszMessage, ...);

		// Read a character from the DBGU USART
		static bool GetChar(char &chValue);

		// Write a character to the DBGU USART
		static void PutChar(const char chValue);

		// Executes a debug command, both default commands (reset, stack and
		// heap dump, etc.), and application-specific commands. If commands
		// that execute from application context have been defined, the
		// application must call ExecutePendingCommand() when idle to execute
		// pending commands
		static void ExecutePendingCommand(const bool bApplicationContext = true);

	// Overridables
	public:
		// Called by ISR when a character has been received
		static void OnReceive(const char ch);

		// Called to dump the application state. Must be overloaded by
		// applications that provide application dumps
		static void DumpApplication();

		// Called to validate the application state, i.e. perform regular
		// sanity checks. The default checks are limited to heap sanity checks;
		//	can be overloaded by applications to provide exhaustive checking
		static void ValidateApplication();

	// Implementation
	public:
		// Maximum number of command line arguments
		enum { maxMessageLength = 128, maxCommandLength = 80,
			maxArguments = 16 };

		// Pointer to the one and only CDebugUnit instance
		static CDebugUnit *pInstance;

		// Message buffer for OutputStringEx(). Also used to re-print the last
		// message upon request
		static char szMessage[__COMPACT_DEBUG_MESSAGE_SIZE];

		// Command buffer
		static char szCommand[__COMPACT_DEBUG_COMMAND_SIZE];

		// Trace mask. Allows to turn on/off individual groups of trace messages
		static unsigned int nOutputMask;

		// Heap validation switch. Allows do enable/disable heap checks made
		// with ASSERT_VALIDHEAPBLOCK() and VALIDATE_HEAP() on-the-fly in case
		//	timing is critical
		static bool bValidateHeap;

		// True, if ExecutePendingCommand() must run from application context
		// to execute a pending command
		static bool bExecutePending;

		// Returns a string representing the active interrupt source
		static const char *GetInterruptSource(const unsigned int nInterruptStatus);

		// Registered as new handler and called when operator new was unable to
		// allocate a memory block
		static void OnOutOfMemory();

		// Returns a prompt item handler that matches the specified key or
		// priority key
		static const CDebugUnitPromptItem *FindPromptItem(const char chKey,
			const bool bPriority = false);

		// Command handler for dbgu_app
		static void HandleDumpApplication(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_trace
		static void HandleSetOutputMask(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_reset
		static void HandleReset(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_ints
		static void HandleDumpInterrupts(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_io
		static void HandleDumpPackagePins(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_rmem
		static void HandleReadMemory(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_wmem
		static void HandleWriteMemory(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_heap
		static void HandleDumpHeapInfo(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_heap_validation
		static void HandleHeapValidation(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_stack
		static void HandleDumpStackInfo(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_help
		static void HandleHelp(const unsigned int nArguments,
			const char *const apszArguments[]);

		// Command handler for dbgu_clock
		static void HandleDumpClockInfo(const unsigned int nArguments,
			const char *const apszArguments[]);
};

#endif


/////////////////////////////////////////////////////////////////////////////
// Compile-time selectable maximum trace level
// Can be used to limit tracing at compile-time, e.g. in order to reduce
// the binary size.

#ifndef TRACE_LEVEL_MAX
#define TRACE_LEVEL_MAX 7
#endif

/////////////////////////////////////////////////////////////////////////////
// TRACE macros

#ifdef _DEBUG
#define _TRACE0(mask, arg) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg); \
	} while (0)
#define _TRACE1(mask, arg1, arg2) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2); \
	} while (0)
#define _TRACE2(mask, arg1, arg2, arg3) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3); \
	} while (0)
#define _TRACE3(mask, arg1, arg2, arg3, arg4) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3, arg4); \
	} while (0)
#define _TRACE4(mask, arg1, arg2, arg3, arg4, arg5) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3, \
				arg4, arg5); \
	} while (0)
#define _TRACE5(mask, arg1, arg2, arg3, arg4, arg5, arg6) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3, \
				arg4, arg5, arg6); \
	} while (0)
#define _TRACE6(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3, \
				arg4, arg5, arg6, arg7); \
	} while (0)
#define _TRACE7(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3, \
				arg4, arg5, arg6, arg7, arg8); \
	} while (0)
#define _TRACE8(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3, \
				arg4, arg5, arg6, arg7, arg8, arg9); \
	} while (0)
#define _TRACE9(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
	do { \
		if (((mask) & 0x07) <= TRACE_LEVEL_MAX) \
			CDebugUnit::OutputStringEx(mask, arg1, arg2, arg3, \
				arg4, arg5, arg6, arg7, arg8, arg9, arg10); \
	} while (0)
#else
#define _TRACE0(mask, arg)
#define _TRACE1(mask, arg1, arg2)
#define _TRACE2(mask, arg1, arg2, arg3)
#define _TRACE3(mask, arg1, arg2, arg3, arg4)
#define _TRACE4(mask, arg1, arg2, arg3, arg4, arg5)
#define _TRACE5(mask, arg1, arg2, arg3, arg4, arg5, arg6)
#define _TRACE6(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define _TRACE7(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define _TRACE8(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#define _TRACE9(mask, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)
#endif

#define TRACE0(arg) _TRACE0(CDebugUnit::maskDefault, arg)
#define TRACE1(arg1, arg2) \
	_TRACE1(CDebugUnit::maskDefault, arg1, arg2)
#define TRACE2(arg1, arg2, arg3) \
	_TRACE2(CDebugUnit::maskDefault, arg1, arg2, arg3)
#define TRACE3(arg1, arg2, arg3, arg4) \
	_TRACE3(CDebugUnit::maskDefault, arg1, arg2, arg3, arg4)
#define TRACE4(arg1, arg2, arg3, arg4, arg5) \
	_TRACE4(CDebugUnit::maskDefault, arg1, arg2, arg3, \
		arg4, arg5)
#define TRACE5(arg1, arg2, arg3, arg4, arg5, arg6) \
	_TRACE5(CDebugUnit::maskDefault, arg1, arg2, arg3, \
		arg4, arg5, arg6)
#define TRACE6(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
	_TRACE6(CDebugUnit::maskDefault, arg1, arg2, arg3, \
		arg4, arg5, arg6, arg7)
#define TRACE7(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
	_TRACE7(CDebugUnit::maskDefault, arg1, arg2, arg3, \
		arg4, arg5, arg6, arg7, arg8)
#define TRACE8(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
	_TRACE8(CDebugUnit::maskDefault, arg1, arg2, arg3, \
		arg4, arg5, arg6, arg7, arg8, arg9)
#define TRACE9(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
	_TRACE9(CDebugUnit::maskDefault, arg1, arg2, arg3, \
		arg4, arg5, arg6, arg7, arg8, arg9, arg10)


/////////////////////////////////////////////////////////////////////////////
// TRACE with variable argument count (requires C99 __VA_ARGS__ support)

#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended
#endif
#ifdef _DEBUG
#define _TRACE(mask, ...) \
	CDebugUnit::OutputStringEx(mask, __VA_ARGS__)
#else
#define _TRACE(mask, ...)
#endif
#define TRACE(...) _TRACE(CDebugUnit::maskDefault, __VA_ARGS__)
#ifdef __IAR_SYSTEMS_ICC__
#pragma language=default
#endif

		
/////////////////////////////////////////////////////////////////////////////
// ASSERT_SIMPLE and VERIFY_SIMPLE macros for flash constraint devices

#ifdef _DEBUG
#define ASSERT_SIMPLE(e)	\
	do { \
		if (!(e)) \
			exit(EXIT_FAILURE); \
	} while (false)
#define VERIFY_SIMPLE(e) \
	do { \
		if (!(e)) \
			exit(EXIT_FAILURE); \
	} while (false)
#else
#define ASSERT_SIMPLE(e)
#define VERIFY_SIMPLE(e)	(e)
#endif


/////////////////////////////////////////////////////////////////////////////
// ASSERT and VERIFY macros

#ifdef __COMPACT_SIMPLE_ASSERT
#define ASSERT(e) ASSERT_SIMPLE(e)
#define VERIFY(e) VERIFY_SIMPLE(e)
#else
#ifdef _DEBUG
#define ASSERT(e)	\
	do { \
		if (!(e)) \
			CDebugUnit::OnAssertionFailed(__FILE__, __LINE__); \
	} while (false)
#define VERIFY(e) \
	do { \
		if (!(e)) \
			CDebugUnit::OnAssertionFailed(__FILE__, __LINE__); \
	} while (false)
#else
#define ASSERT(e)
#define VERIFY(e)	(e)
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// ASSERT_VALIDHEAPBLOCK

#if (defined _DEBUG) && ((defined __IAR_SYSTEMS_ICC__) || \
	(defined __GNUC__ && defined __linux__))
#define ASSERT_VALIDHEAPBLOCKEX(p, n, e) \
	{ \
		if (CDebugUnit::bValidateHeap && (e) && !CHeapDebug::IsValid(p, n)) \
		{ \
			CDebugUnit::OutputStringEx(CDebugUnit::facilityAssertion, \
				"\r\nNOT A VALID HEAP BLOCK: pointer = %08p, size = %u.\r\n", \
				static_cast<const void *>(p), n); \
			CDebugUnit::OnAssertionFailed(__FILE__, __LINE__); \
		} \
	}
#else
#define ASSERT_VALIDHEAPBLOCKEX(p, n, e)
#endif

#define ASSERT_VALIDHEAPBLOCK(p, n) ASSERT_VALIDHEAPBLOCKEX(p, n, true)

#if (defined _DEBUG) && (defined __IAR_SYSTEMS_ICC__)
#define ASSERT_HEAPVALID() \
	{ \
		CHeapDebug hd; \
		if (CDebugUnit::bValidateHeap && !hd.Walk(CHeapDebug::validate)) \
		{ \
			CDebugUnit::OutputStringEx(CDebugUnit::facilityAssertion, \
				"\r\nHEAP CORRUPT!\r\n"); \
			CDebugUnit::OnAssertionFailed(__FILE__, __LINE__); \
		} \
	}
#else
#define ASSERT_HEAPVALID()
#endif
