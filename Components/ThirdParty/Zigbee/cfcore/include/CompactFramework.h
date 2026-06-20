// CompactFramework.h : header file
//
// Copyright(C) 2005-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) generic Compact Series framework
//
// This file encapsulates compiler and device adaptations, as far as required

/////////////////////////////////////////////////////////////////////////////
// Helps to detect availability of cfcore library in multi-platform projects

#define __COMPACT_FRAMEWORK__		0x0200


/////////////////////////////////////////////////////////////////////////////
// Intel x86 and x86-64 processors are always little-endian

#ifdef _M_IX86
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#endif

#ifdef _M_X64
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// GNU C compiler indicates endian-ness

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// Intrinsics (on IAR 8.30.2 intrinsics.h clashes with CMSIS)

#ifdef __IAR_SYSTEMS_ICC__
#if __IAR_SYSTEMS_ICC__ < 9
#include <intrinsics.h>
typedef unsigned long ldrexw_arg_t;
#else
#include <iccarm_builtin.h>
#define __no_operation __iar_builtin_no_operation
#define __disable_interrupt __iar_builtin_disable_interrupt
#define __enable_interrupt __iar_builtin_enable_interrupt
#define __get_interrupt_state __iar_builtin_get_interrupt_state
#define __set_interrupt_state __iar_builtin_set_interrupt_state
typedef unsigned int ldrexw_arg_t;
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// Header files, standard C++ types, fixed-size integers

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS
#define __STDC_FORMAT_MACROS

#include <cstddef>
#include <cstdint>
#include <ctime>

#ifdef __SEGGER_RTL_VERSION
#include <inttypes.h>
#else
#include <cinttypes>
#endif

#include "CompactBase.h"


/////////////////////////////////////////////////////////////////////////////
// Static assertions

#ifdef __IAR_SYSTEMS_ICC__
#define STATIC_ASSERT(expression, message) \
	_Pragma("language=extended") \
	static_assert(expression, message); \
	_Pragma("language=default")
#else
#define STATIC_ASSERT(expression, message) \
	static_assert(expression, message);
#endif


/////////////////////////////////////////////////////////////////////////////
// __COMPACT_LITERALIZE() and __COMPACT_PRAGMA()

#define __COMPACT_LITERALIZE(s) #s
#define __COMPACT_LITERALIZE2(x, y) __COMPACT_LITERALIZE(x ## y)
#define __COMPACT_LITERALIZE2L(x, y) __COMPACT_LITERALIZE(x ## #y)
#define __COMPACT_LITERALIZE2C(x, y) #x "," #y
#define __COMPACT_PRAGMA(s) _Pragma(s)


/////////////////////////////////////////////////////////////////////////////
// Compiler warning control (for GCC)

#ifdef __GNUC__

#define __GCC_DIAGNOSTIC_PRAGMA(s) \
	__COMPACT_PRAGMA(GCC diagnostic s)

#define __GCC_DIAGNOSTIC_DISABLE(x) \
	__GCC_DIAGNOSTIC_PRAGMA(push) \
	__GCC_DIAGNOSTIC_PRAGMA(ignored __COMPACT_JOIN(-W,x))

#define __GCC_DIAGNOSTIC_RESTORE \
	__GCC_DIAGNOSTIC_PRAGMA(pop)

#endif


/////////////////////////////////////////////////////////////////////////////
// Sections

#ifdef __COMPACT_LINKER_IAR__

// Variadic macro arguments require IAR Language Extensions for IAR 7.80.4
// and earlier revisions, which are not C11-compliant
#pragma language=extended

// Below macros result in unjustified warnings related to the resulting token
// not being an identifier (which is not the intention)
#pragma diag_suppress=Pe1665

#define __COMPACT_SECTION_DECLARE(type, name)
#define __COMPACT_SECTION_IMPLEMENT(type, name, id, ...) \
	__COMPACT_PRAGMA(__COMPACT_LITERALIZE2L(location=,.name)) \
	_Pragma("language=extended") \
	__root static const type id = { __VA_ARGS__ }; \
	_Pragma("language=default")
#define __COMPACT_SECTION_IMPLEMENT_ONDEMAND(type, name, id, ...) \
	__COMPACT_PRAGMA(__COMPACT_LITERALIZE2L(location=,.name)) \
	_Pragma("language=extended") \
	static const type id = { __VA_ARGS__ }; \
	_Pragma("language=default")
#define __COMPACT_SECTION_BEGIN(type, name) \
	__COMPACT_PRAGMA(__COMPACT_LITERALIZE2L(section=,.name)) \
	_Pragma("language=extended") \
	reinterpret_cast<const type *>(__section_begin("." #name)) \
	_Pragma("language=default")
#define __COMPACT_SECTION_END(type, name) \
	__COMPACT_PRAGMA(__COMPACT_LITERALIZE2L(section=,.name)) \
	_Pragma("language=extended") \
	reinterpret_cast<const type *>(__section_end("." #name)) \
	_Pragma("language=default")
#elif (defined __COMPACT_LINKER_LLVM__)
#define __COMPACT_SECTION_DECLARE(type, name) \
	extern const type __start_ ## name[] \
		__asm(__COMPACT_LITERALIZE(section$start$ ## name ## $ ## name)); \
	extern const type __stop_ ## name[] \
		__asm(__COMPACT_LITERALIZE(section$end$ ## name ## $ ## name)); \
	__attribute__((weak)) extern const type __start_ ## name[], __stop_ ## name[];
#define __COMPACT_SECTION_IMPLEMENT(type, name, id, ...) \
	static const __attribute__((section(__COMPACT_LITERALIZE2C(name, name)))) \
	__attribute__((used, retain)) \
	type id = { __VA_ARGS__ };
#define __COMPACT_SECTION_IMPLEMENT_ONDEMAND(type, name, id, ...) \
	static const __attribute__((section(__COMPACT_LITERALIZE2C(name, name)))) \
	type id = { __VA_ARGS__ };
#define __COMPACT_SECTION_BEGIN(type, name) \
	__start_ ## name
#define __COMPACT_SECTION_END(type, name) \
	__stop_ ## name
#elif (defined __COMPACT_LINKER_GOLD__)
#define __COMPACT_SECTION_DECLARE(type, name) \
	extern const type __start_ ## name[]; \
	extern const type __stop_ ## name[]; \
	__attribute__((weak)) extern const type __start_ ## name[], __stop_ ## name[];
#define __COMPACT_SECTION_IMPLEMENT(type, name, id, ...) \
	static const __attribute__((section(__COMPACT_LITERALIZE(name)))) \
	__attribute__((used, retain)) \
	type id = { __VA_ARGS__ };
#define __COMPACT_SECTION_IMPLEMENT_ONDEMAND(type, name, id, ...) \
	static const __attribute__((section(__COMPACT_LITERALIZE(name)))) \
	type id = { __VA_ARGS__ };
#define __COMPACT_SECTION_BEGIN(type, name) \
	__start_ ## name
#define __COMPACT_SECTION_END(type, name) \
	__stop_ ## name
#elif (defined __COMPACT_LINKER_SEGGER__)
#define __COMPACT_SECTION_DECLARE(type, name) \
	extern const type __ ## name ## _start__[]; \
	extern const type __ ## name ## _end__[]; \
	__attribute__((weak)) extern const type __ ## name ## _start__[]; \
	__attribute__((weak)) extern const type __ ## name ## _stop__[];
#define __COMPACT_SECTION_IMPLEMENT(type, name, id, ...) \
	static const __attribute__((section(__COMPACT_LITERALIZE(name)))) \
	__attribute__((used, retain)) \
	type id = { __VA_ARGS__ };
#define __COMPACT_SECTION_IMPLEMENT_ONDEMAND(type, name, id, ...) \
	static const __attribute__((section(__COMPACT_LITERALIZE(name)))) \
	type id = { __VA_ARGS__ };
#define __COMPACT_SECTION_BEGIN(type, name) \
	__ ## name ## _start__
#define __COMPACT_SECTION_END(type, name) \
	__ ## name ## _end__
		
#pragma diag_default=Pe1665		
#else
#error Unsupported toolchain/linker
#endif


/////////////////////////////////////////////////////////////////////////////
// Support for 32-bit and 64-bit time_t implementations

#ifdef _MSC_VER
#ifdef _USE_32BIT_TIME_T
#define __COMPACT_TIME_T 32
typedef int32_t __compact_time_t;
#else
#define __COMPACT_TIME_T 64
typedef int64_t __compact_time_t;
#endif
#endif

#ifdef __IAR_SYSTEMS_ICC__
#if _DLIB_TIME_USES_64
#define __COMPACT_TIME_T 64
typedef int64_t __compact_time_t;
#else
#define __COMPACT_TIME_T 32
typedef int32_t __compact_time_t;
#endif
#endif

#ifndef __COMPACT_TIME_T
#if INTPTR_MAX == INT32_MAX
#define __COMPACT_TIME_T 32
typedef int32_t __compact_time_t;
#else
#define __COMPACT_TIME_T 64
typedef int64_t __compact_time_t;
#endif
#endif

STATIC_ASSERT(sizeof(__compact_time_t) == sizeof(time_t),
	"__compact_time_t size mismatch")


/////////////////////////////////////////////////////////////////////////////
// Debugging support, available on all platforms

#include "CompactDebug.h"


/////////////////////////////////////////////////////////////////////////////
// Device specific header files for Atmel SAM

#if defined (__COMPACT_SAM3S) || defined (__COMPACT_SAM4S) || \
	defined (__COMPACT_SAM7S) || defined (__COMPACT_SAM7X) || \
	defined (__COMPACT_SAM9263) || defined (__COMPACT_SAM9XE) || \
	defined (__COMPACT_SAM9G45)

#define __COMPACT_BAREMETAL
#define __COMPACT_SAM
#include "Board.h"
#include "CompactDevice.h"
#include "CompactIRQ.h"
#include "atmel/CompactPMC.h"
#include "atmel/CompactUSART.h"
#include "atmel/CompactIO.h"
#include "atmel/CompactTC.h"
#include "atmel/CompactSPI.h"
#include "atmel/CompactRTT.h"
#include "atmel/CompactVREG.h"
#include "atmel/CompactCKGR.h"
#include "atmel/CompactRSTC.h"
#include "atmel/CompactWDT.h"
#include "atmel/CompactTWI.h"
#include "atmel/CompactDebugSAM.h"
#endif

#if defined (__COMPACT_SAM7S) || defined (__COMPACT_SAM7X) || \
	defined (__COMPACT_SAM9263) || defined (__COMPACT_SAM9G45)

#include "atmel/CompactPWM.h"

#endif


/////////////////////////////////////////////////////////////////////////////
// Device specific header files for Qorvo K8A (e.g. GP570, QPG6095, UE878)

#ifdef __COMPACT_K8A
#define __COMPACT_BAREMETAL
#include "gp_kx.h"
#include "Board.h"
#include "CompactDevice.h"
#include "CompactIRQ.h"
#include "qorvo/CompactIOK8A.h"
#include "qorvo/CompactSPI.h"
#include "qorvo/CompactADC.h"
#include "qorvo/CompactTWI.h"
#include "qorvo/CompactPWM.h"
#include "qorvo/CompactPWMLegacy.h"
#include "qorvo/CompactUSART.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// Device specific header files for Qorvo K8E (QPG6105)

#ifdef __COMPACT_K8E
#define __COMPACT_BAREMETAL
#include "gp_kx.h"
#include "Board.h"
#include "CompactDevice.h"
#include "CompactIRQ.h"
#include "qorvo/CompactIOK8E.h"
#include "qorvo/CompactDMAK8E.h"
#include "qorvo/CompactSPI.h"
#include "qorvo/CompactPWMK8E.h"
#include "qorvo/CompactTMR.h"
#include "qorvo/CompactADCK8E.h"
#include "qorvo/CompactLEDK8E.h"
#include "qorvo/CompactIRK8E.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// Device specific header files for Qorvo XP4001 (QPG6200)

#ifdef __COMPACT_XP4001
#define __COMPACT_BAREMETAL
#include "gp_kx.h"
#include "Board.h"
#include "CompactDevice.h"
#include "CompactIRQ.h"
#include "qorvo/CompactIOXP4001.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// Device specific header files for Bouffalo Lab BL702, BL707

#if defined (__COMPACT_BL702) || defined (__COMPACT_BL707)
#define __COMPACT_BAREMETAL
#include "CompactDevice.h"
#include "Board.h"
#include "bl/CompactIO.h"
#include "bl/CompactTC.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// Support for declaring weak symbols

#ifdef __IAR_SYSTEMS_ICC__
#define __COMPACT_WEAK \
	_Pragma("language=extended") \
	__weak \
	_Pragma("language=default")
#elif defined (__GNUC__)
#define __COMPACT_WEAK \
	__attribute__((weak))
#endif


/////////////////////////////////////////////////////////////////////////////
// Compact framework, bare metal vs. host OS support

#ifdef __COMPACT_BAREMETAL
#define __COMPACT_STOCK const
#else
#define __COMPACT_STOCK
#endif


/////////////////////////////////////////////////////////////////////////////
// Compact framework, bare metal vs. host OS support

#ifndef __COMPACT_DEFAULT_STOCK
#define __COMPACT_DEFAULT_STOCK __COMPACT_STOCK
#endif

#ifndef __COMPACT_DEFAULT2_STOCK
#define __COMPACT_DEFAULT2_STOCK __COMPACT_STOCK
#endif


/////////////////////////////////////////////////////////////////////////////
// Support for Microsoft Windows

#ifdef _WIN32
#ifdef __COMPACT_BAREMETAL
#error Windows is not a bare-metal OS
#endif
#include "stdafx.h"
#undef GetCurrentTime
#undef min
#undef max
#endif


/////////////////////////////////////////////////////////////////////////////
// Support for Linux and macOS

#if defined (__linux__) || defined (__APPLE__)
#ifdef __COMPACT_BAREMETAL
#error Linux/macOS not a bare-metal OS
#endif
#include "Board.h"
#include "posix/CompactIO.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// Support for output via serial port to terminal application

#if defined (__IAR_SYSTEMS_ICC__) && defined (_OSTREAM_)
inline std::ostream &__c_endl(std::ostream &s)
{
	s.put('\r');
	s.put('\n');
	s.flush();

	return (s);
}
/*
inline std::ostream &__c_endl(std::ostream &s)
	{ return std::endl(s); }
*/
#else
#define __c_endl std::endl
#endif


/////////////////////////////////////////////////////////////////////////////
// Inline functions

#include "CompactFramework.inl"


/////////////////////////////////////////////////////////////////////////////
// General utility functions

#include "CompactRandom.h"


/////////////////////////////////////////////////////////////////////////////
// Mathematical functions

#include "CompactMath.h"
