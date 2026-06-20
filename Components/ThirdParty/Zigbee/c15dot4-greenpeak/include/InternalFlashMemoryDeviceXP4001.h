// InternalFlashMemoryDeviceXP4001.h : header file
//
// Copyright(C) 2016 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// CInternalFlashMemoryDeviceK8A implements the CMemoryDevice interface for
// Qorvo chips, in particular the XP4001

/////////////////////////////////////////////////////////////////////////////
// CInternalFlashMemoryDeviceK8A

class CInternalFlashMemoryDeviceXP4001 : public CPagedFlashDevice
{
	// Construction
	public:
		CInternalFlashMemoryDeviceXP4001(const uint_fast8_t nPageSizeShift,
			const uint_fast8_t nBlockSizeShift, const uint_fast8_t startAddress);

	// Overrides
	public:
		// Actually reads from the physical flash device
		virtual bool DoRead(void *pData, const size_type cbData,
			const address_type nAddress);

		// Actually writes to the physical flash device
		virtual bool DoWrite(const void *pData, const size_type cbData,
			const address_type nAddress);

		// Actually erases a single page in the physical flash device. The
		// start address of the pages is specified, and m_nPageSize bytes
		// are erased
		virtual bool DoErase(const address_type nAddress);

	// Implementation
	public:
		virtual ~CInternalFlashMemoryDeviceXP4001();
};

