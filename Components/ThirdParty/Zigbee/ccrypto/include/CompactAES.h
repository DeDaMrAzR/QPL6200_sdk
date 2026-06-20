// CompactAES.h : header file
//
// Copyright(C) 2005-2009 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This is a tiny implementation of AES Rjindael suitable for embedded
// applications. The CBC mode of operation is used.
//
// Specify the desired key length (#define AES_KEYLENGTH 128/192/256)
//
// If you make use of encryption, #define AES_SUPPORT_ENCRYPTION
// If you make use of decryption, #define AES_SUPPORT_DECRYPTION

/////////////////////////////////////////////////////////////////////////////
// Options for CompactAES

#define AES_KEYLENGTH 128	// 128, 192 or 256
#define AES_SUPPORT_ENCRYPTION
#define AES_SUPPORT_DECRYPTION

/////////////////////////////////////////////////////////////////////////////
// CRijndaelAES

class CRijndaelAES
{
	// Construction
	public:
		// Key, if known at construction time
		CRijndaelAES(const unsigned char *pbKey);

		// This version can be used to create an instance and later call
		// expand key, when the key is known
		CRijndaelAES();

	// Operations
	public:

#ifdef AES_SUPPORT_DECRYPTION
		void Decrypt(unsigned char *pbData, unsigned char *pbChainBlock) const;
#endif

#ifdef AES_SUPPORT_ENCRYPTION
		void Encrypt(unsigned char *pbData, unsigned char *pbChainBlock) const;
#endif

	// Implementation
	public:
		// Lower 8 bits of (x^8+x^4+x^3+x+1), ie. (x^4+x^3+x+1).
		enum { polynom = 0x1b };

#if (AES_KEYLENGTH == 128)
		enum { blockSize = 16, rounds = 10, keyLength = 16 };
#elif (AES_KEYLENGTH == 192)
		enum { blockSize = 16, rounds = 12, keyLength = 24 };
#elif (AES_KEYLENGTH == 256)
		enum { blockSize = 16, rounds = 14, keyLength = 32 };
#else
#error Use AES_KEYLENGTH of 128, 192 or 256
#endif

		// Expanded key size: 176, 208 or 240 bytes
		enum { expandedKeySize = blockSize * (rounds + 1) } ;

		// Expanded key
		unsigned char m_abExpandedKey[256];

		// s-box
		static const unsigned char abSBox[256];

#ifdef AES_SUPPORT_DECRYPTION
		// Inverseerse s-box.
		static const unsigned char abSBoxInverse[256];
#endif

		// Cycle 4 bytes in an array left once.
		static void CycleLeft(unsigned char *pbRow);

		// Substitute every byte in state
		static void SubstituteBytes(unsigned char *pbData, unsigned char bCount);

		// Perform exclusive-or on data block
		static void AddBytes(unsigned char *pbData1,
			const unsigned char *pbData2, unsigned char bCount);

		void ExpandKey(const unsigned char *pbKey);

#ifdef AES_SUPPORT_DECRYPTION
		static void InverseMixColumn(unsigned char *pbColumn);

		static void InverseSubstituteBytesAndAdd(unsigned char *pbData,
			const unsigned char *pbKey, unsigned char bCount);

		static void InverseShiftRows(unsigned char *pbState);

		static void InverseMixColumns(unsigned char *pbState);

		void InverseCipher(unsigned char *pbBlock) const;
#endif

#ifdef AES_SUPPORT_ENCRYPTION
		static unsigned char Multiply(unsigned char bNumber,
			unsigned char bFactor);

		static unsigned char DotProduct(const unsigned char *pbVector1,
			const unsigned char *pbVector2);

		static void MixColumn(unsigned char *pbColumn);

		static void ShiftRows(unsigned char *pbState);
		static void MixColumns(unsigned char *pbState);

		void Cipher(unsigned char *pbBlock) const;
#endif

};
