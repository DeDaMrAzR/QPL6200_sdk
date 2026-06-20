// IEEE802154Security.h : header file
//
// Copyright(C) 2011-2022 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact15.4(TM) IEEE 802.15.4 compliant MAC and PHY layer HAL
// class library.
//
// This implementation is based on the IEEE Standard for Information
// Technology - Telecommunications and information exchange between systems -
// Local and metropolitan area networks - specific requirements - Part 15.4:
// Wireless Medium Access Control (MAC) and Physical Layer (PHY)
// Specifications for Low-Rate Wireless Personal Area Networks (WPANs), as of
// September 8, 2006
//
// Security features. Implementation of AES-CCM* authenticating stream cipher

/////////////////////////////////////////////////////////////////////////////
// forward referenced classes

class CRijndaelAES;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154SecurityInfo

class CIEEE802154SecurityInfo;


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154SecurityMaterial

class CIEEE802154SecurityMaterial
{
	// Construction
	public:
		CIEEE802154SecurityMaterial();
		CIEEE802154SecurityMaterial(const CIEEE802154SecurityMaterial &material);

	// Attributes
	public:
		// Symmetric key. This is either a MAC layer security key, or a network
		// or application layer key in ZigBee networks, for example
		uint8_t m_abKey[16];

	// Operations
	public:
		// Clears the AES instance. This is required after a key update, for
		// example
		void OnKeyModified();

		// Returns an AES instance that can be used for encryption/decryption
		operator CRijndaelAES &() const;

		// Makes a copy of the security material (no AES engine, initially)
		CIEEE802154SecurityMaterial &operator=
			(const CIEEE802154SecurityMaterial &material);

		// Returns true if the key is null, i.e. consists of all zeros
		bool IsNull() const;

	// Implementation
	public:
		~CIEEE802154SecurityMaterial();

	protected:
		// AES engine. This is created upon request, as soon as the security
		// material is actually required for cryptographic operations. Notice
		// that
		mutable CRijndaelAES *m_pInstance;
};


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154AESCCMNonce

#pragma pack(push, 1)

class CIEEE802154AESCCMNonce
{
	// typedefs & enums
	public:
		// Generic security levels available to the MAC, ZigBee NWK and ZigBee
		// APS layers
		enum { securityNone, securityIntegrity32, securityIntegrity64,
			securityIntegrity128, securityEncryption,
			securityEncryptionAndIntegrity32, securityEncryptionAndIntegrity64,
			securityEncryptionAndIntegrity128, securityLevelMask = 0x7,
			securityIntegrityCodeSizeMask = 0x3	};

	// Attributes
	public:
		// Source address (64-bit extended IEEE address)
		uint64_t m_qwSource;

		// Frame counter
		uint32_t m_nFrameCounter;

		// Security control (this is the service level in 802.15.4, but has
		// additional flags in higher ZigBee layers)
		uint8_t m_nSecurityControl;

	// Operations
	public:
		// Returns true, if the security level implies data encryption
		static bool HasEncryption(const uint_fast8_t nSecurityLevel);

		// Return M, the number of bytes used for the message integrity code
		static CPacketData::size_type GetIntegrityCodeSize
			(const uint_fast8_t nSecurityLevel);

		// Returns true, if the security level implies data encryption
		bool HasEncryption() const;

		// Return M, the number of bytes used for the message integrity code
		CPacketData::size_type GetIntegrityCodeSize() const;
};

#pragma pack(pop)

inline bool CIEEE802154AESCCMNonce::HasEncryption() const
	{ return HasEncryption(m_nSecurityControl & securityLevelMask); }

inline CPacketData::size_type CIEEE802154AESCCMNonce::GetIntegrityCodeSize() const
	{ return GetIntegrityCodeSize(m_nSecurityControl & securityLevelMask); }


/////////////////////////////////////////////////////////////////////////////
// CIEEE802154AESCCM

class CIEEE802154AESCCM
{
	// typedefs & enums
	public:
		enum { flagAuthenticationData = 0x40 };

	// Operations
	public:
		// Encrypts and/or authenticates a message using AES-CCM*
		static bool EncryptAndAuthenticate(CPacket &message,
			const CPacketData::size_type cbHeaders,
			const CIEEE802154AESCCMNonce &nonce,
			const CIEEE802154SecurityMaterial &material);

		// Decrypts and/or authenticates a message created using AES-CCM*
		static bool DecryptAndAuthenticate(CPacket &message,
			const CPacketData::size_type cbHeaders,
			const CIEEE802154AESCCMNonce &nonce,
			const CIEEE802154SecurityMaterial &material);

	// Implementation
	public:
		// Makes a nonce block for AES-CCM*
		static CPacketData::size_type MakeNonceBlock(uint8_t abBlock[16],
			const CIEEE802154AESCCMNonce &nonce, const uint_fast16_t nCount = 0,
			const bool bEncryption = true,
			const void *const pAuthenticationData = 0,
			const CPacketData::size_type cbAuthenticationData = 0);

		// Stream cipher (symmetric, does encryption and decryption)
		static void CipherStream(CRijndaelAES &aes, void *const pData,
			const CPacketData::size_type cbData, uint8_t abChainBlock[16],
			const uint8_t abBlock[16]);

		// Updates the authentication tag with a chunk of data to be
		// authenticated
		static void UpdateAuthenticationTag(CRijndaelAES &aes,
			uint8_t abChainBlock[16], uint8_t abBlock[16],
			CPacketData::size_type &cbCapacity, CPacketData::size_type &nOffset,
			const void *const pChunk, const CPacketData::size_type cbChunk);

		// Calculates the authentication tag
		static void CalculateAuthenticationTag(CRijndaelAES &aes,
			uint8_t abChainBlock[16], uint8_t abBlock[16],
			const CPacket &message, const CPacketData::size_type cbHeaders);

#ifdef __COMPACT15DOT4_HARDWARE_AESCCM__
		// Encrypts or decrypts a frame using the radio's hardware AES engine
		static bool EncryptOrDecryptAndAuthenticate(CPacket &message,
			const CPacketData::size_type cbHeaders,
			const CIEEE802154AESCCMNonce &nonce,
			const CIEEE802154SecurityMaterial &material, const bool bEncrypt);
#endif

};
