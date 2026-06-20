// InternalFlashMemoryDeviceXP4001.cpp : implementation file
//
// Copyright(C) 2016-2020 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// CInternalFlashMemoryDeviceK8x implements the CMemoryDevice interface for
// Qorvo chips, in particular the XP4001 based silicon.

#include <algorithm>
#include "CompactFramework.h"
#include "MemoryDevice.h"
#include "InternalFlashMemoryDeviceXP4001.h"

#ifndef __ARM_PROFILE_M__
#error This implementation is for Cortex-M devices only
#endif

#include "global.h"
#define GP_COMPONENT_ID     1
#include "gpLog.h"
#include "gpHal_kx_mm.h"
#include "hal_kx_Flash.h"

/////////////////////////////////////////////////////////////////////////////
// CInternalFlashMemoryDeviceXP4001

CInternalFlashMemoryDeviceXP4001::CInternalFlashMemoryDeviceXP4001(const uint_fast8_t nPageSizeShift,
			const uint_fast8_t nBlockSizeShift, const uint_fast8_t startAddress) :
CPagedFlashDevice(nPageSizeShift, nBlockSizeShift, startAddress)
{
	ASSERT(m_cache.m_nSize == FLASH_PAGE_SIZE);
	ASSERT(GetBlockSize() == FLASH_SECTOR_SIZE);

}


CInternalFlashMemoryDeviceXP4001::~CInternalFlashMemoryDeviceXP4001()
{
	// Need to call Flush() here, prior to virtual method table being restored
	// to that of the base class
	Flush();
}


bool CInternalFlashMemoryDeviceXP4001::DoRead(void *pData,
	const size_type cbData, const address_type nAddress)
{
	return gpHal_FlashRead(m_nBaseAddress + nAddress, cbData,
		static_cast<uint8_t *>(pData)) == gpHal_FlashError_Success;
}


bool CInternalFlashMemoryDeviceXP4001::DoWrite(const void *pData,
	const size_type cbData, const address_type nAddress)
{
	// Use gpHal_FlashWritePage to write to flash memory
	return gpHal_FlashWrite(m_nBaseAddress + nAddress, cbData >> 2,
		const_cast<uint32_t *>(static_cast<const uint32_t *>(pData)))
		== gpHal_FlashError_Success;
}


bool CInternalFlashMemoryDeviceXP4001::DoErase(const address_type nAddress)
{
	// Address must be aligned on an erase sector boundary
	ASSERT(!(nAddress & (GetBlockSize() - 1)));

	return gpHal_FlashEraseSectorNoVerify(m_nBaseAddress + nAddress) ==
		gpHal_FlashError_Success;
}
