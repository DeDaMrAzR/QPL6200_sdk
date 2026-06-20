// ISO13239.h : header file
//
// Copyright(C) 2005-2018 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Implements a 16-bit cyclic redundancy check based on the ISO/IEC 13239
// international standard:
// Information technology -- Telecommunications and information exchange
// between systems -- High-level data link control (HDLC) procedures

/////////////////////////////////////////////////////////////////////////////
// CStandardISO13239CRC

class CStandardISO13239CRC
{
	// Operations
	public:
		// Given a block of data (and its length), this method calculates a
		// 16-bit CRC according to ISO/IEC 13239
		static unsigned short Calculate(const void *const pData,
			const unsigned int nLength);

		// Given a block of data (and its length), this method appends two
		// bytes of CRC (little endian) as calculated by above method. The
		// data buffer must have enough room to store nLength + 2 bytes!
		static void Append(void *pData, const unsigned int nLength);

		// Given a block of data followed by a 16-bit CRC, this method checks
		// whether the CRC is correct. Notice that - in contrast to Append() -
		// nLength is the length of the data block including the 16-bit CRC
		static bool Check(const void *const pData,
			const unsigned int nLength);

	// Implementation
	protected:
		enum { polynomial = 0x8408, presetValue = 0xffff, residue = 0xf0b8 };

		// This method performs the actual calculation and is called by
		// CalculateCRC(). Usually, it will only be used internally
		static unsigned short Update(const unsigned short nCurrentValue,
			const unsigned char bData);
};
