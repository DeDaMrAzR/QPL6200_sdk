// MemoryDevice.h : header file
//
// Copyright(C) 2006-2020 ubisys technologies, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This file is part of the CompactStorage(TM) library, which provides the
// functionality of malloc() and free() for use with internal/external flash
// memories.
//
// CMemoryDevice provides an abstraction layer for different types of memory,
// including volatile and non-volatile memories. It is an abstract base class
// with pure virtual functions. Another abstract base class,
// CPagedFlashDevice, provides a write cache for use with common flash
// memories

/////////////////////////////////////////////////////////////////////////////
// CMemoryDevice

class CMemoryDevice
{
	// typedefs & enums
	public:
		typedef uint32_t address_type;
		typedef uint16_t size_type;

	// Construction
	public:
		CMemoryDevice();

	// Overridables
	public:
		// Read cbData bytes from flash device beginning at nAddress. Copy
		// data to local buffer at pData
		virtual bool Read(void *pData, const size_type cbData,
			const address_type nAddress) = 0;

		// Write cbData bytes to flash device beginning at nAddress. Copy
		// data from local buffer at pData
		virtual bool Write(const void *pData, const size_type cbData,
			const address_type nAddress) = 0;

		// For devices with an internal buffer implementation, this method
		// flushes the internal buffer to the physical flash device
		virtual bool Flush(const bool bErase = true);

	// Implementation
	public:
		virtual ~CMemoryDevice();
};


/////////////////////////////////////////////////////////////////////////////
// CMemoryPage

class CMemoryPage
{
	// Construction
	public:
		CMemoryPage(const CMemoryDevice::size_type nSize);

	// Attributes
	public:
		// Contains the contents of the currently mirrored flash page
		unsigned char *m_pbData;

		// Size of a physical flash page. Defined as the smallest amount of
		// flash memory that can be written in a single operation
		const CMemoryDevice::size_type m_nSize;

	// Operations
	public:
		// Allocates memory for mirrored page contents
		void Allocate();

		// Releases allocated memory and clears the pointer
		void Release();

		// Returns true, if the cache is in use (i.e. buffer exists)
		operator bool() const;

	// Implementation
	public:
		~CMemoryPage();
};

inline CMemoryPage::operator bool() const
	{ return m_pbData ? true : false; }


/////////////////////////////////////////////////////////////////////////////
// CPagedFlashDevice

class CPagedFlashDevice : public CMemoryDevice
{
	// Construction
	public:
		CPagedFlashDevice(const uint_fast8_t nPageSizeShift,
			const uint_fast8_t nBlockSizeShift = 0,
			const uintptr_t nBaseAddress = 0);

	// Attributes
	public:
		// Indicates the page index of the currently mirrored page
		address_type m_nCurrentPage;

		// Contains the contents of the currently mirrored flash page
		CMemoryPage m_cache;

		// Base address, e.g. start of internal flash ROM in address space
		const uintptr_t m_nBaseAddress;

		// Page size as shift value, e.g. 8 for 256 bytes per page
		const uint8_t m_nPageSizeShift;

		// Block size as shift value, e.g. 12 for 4KB flash blocks
		const uint8_t m_nBlockSizeShift;

		// Returns the size of a single page
		size_type GetPageSize() const;

		// Returns the size of a block (the smallest erasable unit)
		size_type GetBlockSize() const;

	// Overrides
	public:
		// Read cbData bytes from flash device beginning at nAddress
		virtual bool Read(void *pData, const size_type cbData,
			const address_type nAddress);

		// Write cbData bytes to flash device beginning at nAddress. This
		// class implements a write cache for repetitive writes to the
		// same flash page. Beware that using Write() will result in the
		// sectors affected by the write operation being erased, including
		// pages in those sectors that have not been written
		virtual bool Write(const void *pData, const size_type cbData,
			const address_type nAddress);

		// Emits write buffer to actual flash device. Also releases the
		// memory block used to cache write operations
		virtual bool Flush(const bool bErase = true);

	// Operations
	public:
		// Modifies data in non-volatile memory by reading erase sectors into a
		// sector buffer, modifying the buffer, and writing all the pages in
		// the buffer after having erased the sector. Notice that calling this
		// function can result in the sector being corrupted when the write
		// process is interrupted due to a power failure, for example. This
		// routine is just a convenience function to allow modifying smaller
		// memory portions of internal flash memory at runtime
		bool WriteEx(const void *pData, const size_type cbData,
			const address_type nAddress);

	// Overridables
	public:
		// Actually reads from the physical flash device
		virtual bool DoRead(void *pData, const size_type cbData,
			const address_type nAddress) = 0;

		// Actually writes to the physical flash device
		virtual bool DoWrite(const void *pData, const size_type cbData,
			const address_type nAddress) = 0;

		// Actually erases a single sector in the physical flash device. The
		// start address of the pages is specified, and m_nPageSize bytes
		// are erased
		virtual bool DoErase(const address_type nAddress) = 0;

	// Implementation
	public:
		virtual ~CPagedFlashDevice();
};

inline CMemoryDevice::size_type CPagedFlashDevice::GetPageSize() const
	{ return 1u << m_nPageSizeShift; }

inline CMemoryDevice::size_type CPagedFlashDevice::GetBlockSize() const
	{ return 1u << m_nBlockSizeShift; }
