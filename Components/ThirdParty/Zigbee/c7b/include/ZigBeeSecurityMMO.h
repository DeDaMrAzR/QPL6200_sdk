// ZigBeeSecurityMMO.h : header file
//
// Copyright(C) 2011-2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This is an implementation of the Matyas-Meyer-Oseas cryptographic hash
// with E = AES-128, and the initialization vector = 0, which is the ZigBee
// MMO cryptogaphic hash, based on Rjindael AES and suitable for embedded
// applications

/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CRijndaelAES;

/////////////////////////////////////////////////////////////////////////////
// CZigBeeSecurityMMO

class CZigBeeSecurityMMO
{
	// typedefs & enums
	public:
		enum { blockSize = 16 };

	// Operations
	public:
		// Calculates the Matyas-Meyer-Oseas hash with E = AES-128, and the
		// initialization vector = 0		
		static void CalculateHash(unsigned char abHash[blockSize],
			const void *const pData, const size_t cbData);
		
	// Implementation
	public:
		static void InitializeHash(CRijndaelAES &aes,
			unsigned char abHash[blockSize], size_t &cbProcessed);
		
		static void UpdateBlock(CRijndaelAES &aes,
			unsigned char abHash[blockSize], const bool bFinalBlock = false);

		static size_t UpdateHash(CRijndaelAES &aes,
			unsigned char abHash[blockSize], size_t &cbData,
			const void *const pChunk, const size_t cbChunk);

		static void FinalizeHash(CRijndaelAES &aes,
			unsigned char abHash[blockSize], const size_t cbData);
};
