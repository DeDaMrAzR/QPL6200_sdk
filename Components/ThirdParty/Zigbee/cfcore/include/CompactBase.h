// CompactBase.h : header file
//
// Copyright(C) 2005 - 2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) generic Compact Series framework
//
// This file encapsulates compiler adaptations, as far as required
//
// If __COMPACT_NO_CXXNAMESPACES__ is defined, this indicates that the C++
// compiler does not support namespaces. Some embedded C++ compilers do not
// support namespaces. In this case std is defined as nothing
//
// If __COMPACT_NO_CXXEXCEPTIONS__ is defined, this indicates that the C++
// compiler does not support exception handling. This may be due to the fact
// that exception handling has been disabled, or is simply not supported. In
// fact, some EC++ compilers, such as IAR ARM C++ currently don't support
// C++ exception handling
//
// For some use-cases we need to know which linker is going to be used to
// link the final executable. While clang being the compiler could be assumed
// to hint to LLVM being the linker, this is not always true. For example,
// in recent Android NDKs, clang is used as the compiler and GNU GOLD is used
// as the linker, whereas on iOS the combination of clang and LLVM linker is
// used

/////////////////////////////////////////////////////////////////////////////
// embedded C++ support

#ifdef __CC_ARM
#ifndef __embedded_cplusplus
#define __embedded_cplusplus
#endif
#endif

#ifdef __embedded_cplusplus
#define __COMPACT_NO_CXXNAMESPACES__
#define __COMPACT_NO_CXXEXCEPTIONS__
#endif


/////////////////////////////////////////////////////////////////////////////
// C++ namespace supported?

#ifdef __COMPACT_NO_CXXNAMESPACES__
#define std
#endif


/////////////////////////////////////////////////////////////////////////////
// STLport defines std as something else, breaking basic C++ functionality

#ifdef _STLPORT_VERSION
#define __c_nothrow __std_alias::nothrow
#define __c_set_new_handler __std_alias::set_new_handler
#else
#define __c_nothrow std::nothrow
#define __c_set_new_handler std::set_new_handler
#endif


/////////////////////////////////////////////////////////////////////////////
// std::binary_function required for C++98, deprecated in C++ 17

#if (__cplusplus < 201703L)
#define __COMPACT_DECLARE_BFBASE(type) \
	: public std::binary_function<type, type, bool>
#else
#define __COMPACT_DECLARE_BFBASE(type)
#endif


/////////////////////////////////////////////////////////////////////////////
// C++ linker

#ifdef __IAR_SYSTEMS_ICC__
#define __COMPACT_LINKER_IAR__
#elif __SEGGER_RTL_VERSION
#define __COMPACT_LINKER_SEGGER__
#elif (defined __clang__ && defined __ANDROID__)
#define __COMPACT_LINKER_GOLD__
#elif (defined __clang__)
#define __COMPACT_LINKER_LLVM__
#elif (defined __GNUC__)
#define __COMPACT_LINKER_GOLD__
#else
#error Unsupported toolchain
#endif
