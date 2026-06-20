// CompactDevice.h : header file
//
// Copyright(C) 2009 - 2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) generic Compact series framework - device identification

#if defined (__COMPACT_SAM3S)
#include "atmel/ATSAM3S4_cfcore.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) BASE_##peripheral1
#define __CFCORE_IDEX(peripheral1, peripheral2) ID_##peripheral1
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type1 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix1##_##suffix1
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix1##_##suffix1
#elif defined (__COMPACT_SAM4S)
#include "atmel/ATSAM4S8_cfcore.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) BASE_##peripheral1
#define __CFCORE_IDEX(peripheral1, peripheral2) ID_##peripheral1
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type1 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix1##_##suffix1
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix1##_##suffix1
#elif defined (__COMPACT_SAM7S)
#include "atmel/ioat91sam7s512.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) AT91C_BASE_##peripheral2
#define __CFCORE_IDEX(peripheral1, peripheral2) AT91C_ID_##peripheral2
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#elif defined (__COMPACT_SAM7X)
#include "atmel/ioat91sam7x512.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) AT91C_BASE_##peripheral2
#define __CFCORE_IDEX(peripheral1, peripheral2) AT91C_ID_##peripheral2
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#elif defined (__COMPACT_SAM9263)
#include "atmel/ioat91sam9263.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) AT91C_BASE_##peripheral2
#define __CFCORE_IDEX(peripheral1, peripheral2) AT91C_ID_##peripheral2
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#elif defined (__COMPACT_SAM9XE)
#include "atmel/ioat91sam9xe512.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) AT91C_BASE_##peripheral2
#define __CFCORE_IDEX(peripheral1, peripheral2) AT91C_ID_##peripheral2
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#elif defined (__COMPACT_SAM9G45)
#include "atmel/ioat91sam9g45.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) AT91C_BASE_##peripheral2
#define __CFCORE_IDEX(peripheral1, peripheral2) AT91C_ID_##peripheral2
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL AT91C_AIC_SRCTYPE_INT_LEVEL_SENSITIVE
#elif defined (__COMPACT_K8A)
#include "qorvo/QPG6095.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) QPG6095_BASE_##peripheral2
#define __CFCORE_IDEX(peripheral1, peripheral2) QPG6095_ID_##peripheral2
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#elif defined (__COMPACT_K8E)
#include "qorvo/QPG6105.h"
#define __CFCORE_BASEEX(peripheral1, peripheral2) QPG6105_BASE_##peripheral2
#define __CFCORE_IDEX(peripheral1, peripheral2) QPG6105_ID_##peripheral2
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) prefix2##_##suffix2
#elif defined (__COMPACT_XP4001)
// Nothing to define here yet
#elif defined (__COMPACT_BL702) || defined (__COMPACT_BL707)
#define __CFCORE_BASEEX(peripheral1, peripheral2) 0
#define __CFCORE_IDEX(peripheral1, peripheral2) 0
#define __CFCORE_PERIPHERAL(common, type1, type2) typedef type2 common
#define __CFCORE_REGISTEREX(suffix1, prefix1, suffix2, prefix2) 0
#define __CFCORE_CONSTANTEX(suffix1, prefix1, suffix2, prefix2) 0
#else
#error Unsupported device type
#endif


#if defined(__GNUC__) && ( \
	defined(__ARM_ARCH_6M__) || \
	defined(__ARM_ARCH_7M__) || \
	defined(__ARM_ARCH_7EM__))
#define __ARM_PROFILE_M__
#ifndef __ASM
#define __ASM asm
#endif
#ifndef __STATIC_INLINE
#define __STATIC_INLINE static inline
#endif
#include "cmsis_gcc.h"
#endif

#define __CFCORE_BASE(peripheral) \
	__CFCORE_BASEEX(peripheral, peripheral)

#define __CFCORE_ID(peripheral) \
	__CFCORE_IDEX(peripheral, peripheral)

#define __CFCORE_CONSTANT(common, prefix1, prefix2) \
	__CFCORE_CONSTANTEX(common, prefix1, common, prefix2)

#define __CFCORE_REGISTER(common, prefix1, prefix2) \
	__CFCORE_REGISTEREX(common, prefix1, common, prefix2)

#define __CFCORE_MAKEVALUE(field, value) \
	(field##_MASK & ((value) << field##_SHIFT))

#define __CFCORE_GETVALUE(field, value) \
	((field##_MASK & (value)) >> field##_SHIFT)


#ifndef __IAR_SYSTEMS_ASM__

/////////////////////////////////////////////////////////////////////////////
// CCompactDevice

class CCompactDevice
{
	// typedefs & enums
	public:
		// Embedded ARM processor cores
		enum { processorARM946ES = 1, processorARM7TDMI, processorCortexM3,
			processorARM920T, processorARM926EJS, processorCortexA5,
			processorCortexM4 };

		// Architectures
		enum { architectureAT91SAM9xx = 0x19,
			architectureAT91SAM9XExx = 0x29,
			architectureAT91x34 = 0x34,
			architectureCAP7 = 0x37,
			architectureCAP9 = 0x39,
			architectureCAP11 = 0x3b,
			architectureAT91x40 = 0x40,
			architectureAT91x42 = 0x42,
			architectureAT91x55 = 0x55,
			architectureAT91SAM7Axx = 0x60,
			architectureAT91SAM7AQxx = 0x61,
			architectureAT91x63 = 0x63,
			architectureAT91SAM7Sxx = 0x70,
			architectureAT91SAM7XCxx = 0x71,
			architectureAT91SAM7SExx = 0x72,
			architectureAT91SAM7Lxx = 0x73,
			architectureAT91SAM7Xxx = 0x75,
			architectureAT91SAM7SLxx = 0x76,
			architectureATSAM3UxC = 0x80,
			architectureATSAM3UxE = 0x81,
			architectureATSAM3AxC = 0x83,
			architectureATSAM3XxC = 0x84,
			architectureATSAM3XxE = 0x85,
			architectureATSAM3XxG = 0x86,
			architectureATSAMxSxA = 0x88,
			architectureATSAMxSxB = 0x89,
			architectureATSAMxSxC = 0x8a,
			architectureAT91x92 = 0x92,
			architectureATSAM3NxA = 0x93,
			architectureATSAM3NxB = 0x94,
			architectureATSAM3NxC = 0x95,
			architectureATSAM3SDxA = 0x98,
			architectureATSAM3SDxB = 0x99,
			architectureATSAM3SDxC = 0x9a,
			architectureATSAM5A = 0xa5,
			architectureAT75Cxx = 0xf0 };

		// Memory options
		enum { nvpROM = 0, nvpROMlessOrFlash = 0x01,
			nvpEmbeddedFlash = 0x02, nvpROMAndFlash = 0x03,
			nvpSRAMEmulatingROM = 0x04 };

		// Memory sizes (for SRAM)
		enum { sram48K, sram1K, sram2K, sram6K,
			sram24K, sram4K, sram80K, sram160K, sram8K,
			sram16K, sram32K, sram64K, sram128K, sram256K,
			sram96K, sram512K };

		// Memory sizes (non-volatile program memory)
		enum { nvpNone, nvp8K, nvp16K, nvp32K, nvpReserved4,
			nvp64K, nvpReserved6, nvp128K, nvpReserved8, nvp256K,
			nvp512K, nvpReserved11, nvp1024K, nvpReserved13, nvp2048K,
			nvpReserved15 };

		// Complete chip IDs
		enum {
			chipAT91SAM7S512A = 0x270b0a40, chipAT91SAM7S512B = 0x270b0a4f,
			chipAT91SAM7S256A = 0x270d0940, chipAT91SAM7S256B = 0x270b0941,
			chipAT91SAM7S256C = 0x270b0942, chipAT91SAM7S128A = 0x270a0740,
			chipAT91SAM7S128B = 0x270a0741, chipAT91SAM7S128C = 0x270a0742,
			chipAT91SAM7S64A = 0x27090540, chipAT91SAM7S64B = 0x27090543,
			chipAT91SAM7S64C = 0x27090544, chipAT91SAM7S321A = 0x27080342,
			chipAT91SAM7S32A = 0x27080340, chipAT91SAM7S32B = 0x27080341,
			chipAT9SAM7S161A = 0x27050241, chipAT91SAM7S16 = 0x27050240,
			chipAT91SAM7X512 = 0x275c0a40, chipAT91SAM7X256 = 0x275b0940,
			chipAT91SAM7X128 = 0x275a0740,
			chipATSAM3S4A = 0x28800960, chipATSAM3S2A = 0x28800760,
			chipATSAM3S1A = 0x28800560, chipATSAM3S4B = 0x28900960,
			chipATSAM3S2B = 0x28900760, chipATSAM3S1B = 0x28900560,
			chipATSAM3S4C = 0x28a00960, chipATSAM3S2C = 0x28a00760,
			chipATSAM3S1C = 0x28a00560,
			chipATSAM4SD32C = 0x29a70ee0, chipATSAM4SD32B = 0x29970ee0,
			chipATSAM4SD16C = 0x29a70ce0, chipATSAM4SD16B = 0x29970ce0,
			chipATSAM4SA16C = 0x28a70ce0, chipATSAM4SA16B = 0x28970ce0,
			chipATSAM4S16B = 0x289c0ce0, chipATSAM4S16C = 0x28ac0ce0,
			chipATSAM4S8B = 0x289c0ae0, chipATSAM4S8C = 0x28ac0ae0,
			chipATSAM4S4C = 0x28ab09e0, chipATSAM4S4B = 0x289b09e0,
			chipATSAM4S4A = 0x288b09e0, chipATSAM4S2C = 0x28ab07e0,
			chipATSAM4S2B = 0x289b07e0, chipATSAM4S2A = 0x288b07e0,
			chipAT91SAM9XE512 = 0x329aa3a0, chipATSAM9XE256 = 0x329a93a0,
			chipATSAM9XE128 = 0x329973a0,
			chipAT91SAM9260 = 0x019803a2, chipAT91SAM9261 = 0x019703a0,
			chipAT91SAM9261S = 0x019703a0, chipAT91SAM9263 = 0x019607a0,
			chipAT91SAM9G45 = 0x819b05a2 };

	// Attributes
	public:
		unsigned m_nVersion : 5;
		unsigned m_nProcessor : 3;
		unsigned m_nNVPSize : 4;
		unsigned m_nNVPSize2 : 4;
		unsigned m_nSRAMSize : 4;
		unsigned m_nArchitecture : 8;
		unsigned m_nNVPType : 3;
		unsigned m_nExt : 1;

	// Operations
	public:
		// Returns the 32-bit chip ID, e.g. for comparison with one of the
		// chipXyz constants above
		unsigned int GetChipID() const;

		// Compares against one of the chipXyz constants above, ignoring the
		// version subfield value
		bool IsType(const unsigned int nType) const;

		// Returns true, if device has two flash banks
		bool HasDualPlaneFlash() const;

	// Implementation
	public:

#ifdef _DEBUG
		// Prints device information on debug unit
		void Dump() const;
#endif

};

inline unsigned int CCompactDevice::GetChipID() const
	{ return *reinterpret_cast<const unsigned int *>(this); }

inline bool CCompactDevice::IsType(const unsigned int nType) const
	{ return (GetChipID() &~ 0x1f) == (nType &~ 0x1f); }


/////////////////////////////////////////////////////////////////////////////
// The one and only CCompactDevice instance

extern CCompactDevice theDevice;

#endif
