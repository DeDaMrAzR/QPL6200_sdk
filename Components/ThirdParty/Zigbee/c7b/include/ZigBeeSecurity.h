// ZigBeeSecurity.h : header file
//
// Copyright(C) 2011-2022 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This implementation is based on the ZigBee Alliance's ZigBee Specification
// as of October 12, 2010
//
// This module implements ZigBee security features

/////////////////////////////////////////////////////////////////////////////
// Cryptographic functions

#include "ZigBeeSecurityMMO.h"
#include "ZigBeeSecurityHMAC.h"


/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeNetwork;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeAuxiliaryHeader

#pragma pack(push, 1)

class CZigBeeAuxiliaryHeader
{
	// typedefs & enums
	public:
		enum { controlSecurityLevelMask = 0x07,
			controlKeyIdentifierMask = 0x18, controlKeyIdentifierShift = 3,
			controlExtendedNonce = 0x20 };

		enum { minSize = 5, maxSize = 14 };

	// Attributes
	public:
		// Consist of a security level, a key identifier, and an extended nonce
		// sub-field
		uint8_t m_nSecurityControl;

		// The counter field is used to provide frame freshness and to prevent
		// processing of duplicate frames
		uint32_t m_nFrameCounter;

	// Operations
	public:
		// Returns the key identifier
		uint8_t GetKeyIdentifier() const;

		// Indicates the extended 64-bit address of the device responsible for
		// securing the frame. Returns false, if not present in the frame
		CZigBeeAddress GetSourceAddress() const;

		// Indicates the key sequence number of the network key used to secure
		// the frame. Returns false, if not applicable
		bool GetKeySequenceNumber(uint8_t &nKeySequenceNumber) const;

		// Returns the size of the auxiliary header
		CPacketData::size_type GetSize() const;

		// Returns the size of an auxiliary header with the specified control
		// field and key identifier
		static CPacketData::size_type GetSize
			(const uint_fast8_t nSecurityControl);

		// Prepends an auxiliary header in front of the packet
		static bool Prepend(CPacket &packet, CPacketData::size_type &cbHeaders,
			const uint_fast8_t nSecurityControl,
			const uint_fast32_t nFrameCounter,
			const uint_fast8_t nKeySequenceNumber,
			const uint_fast64_t qwSourceAddress = 0);
};

inline CPacketData::size_type CZigBeeAuxiliaryHeader::GetSize() const
	{ return GetSize(m_nSecurityControl); }

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeSecurityMaterial

class CZigBeeSecurityMaterial : public CIEEE802154SecurityMaterial
{
	// typedefs & enums
	public:
		// Zigbee key types. Notice: This are not necessarily the same values
		// as in transport key messages. For example, peer application keys
		// have value 2 here, as opposed to 3 in transport key messages
		enum { typeNetworkKey = 1, typeApplication = 2, typeTrustCenter = 4,
			typeEphemeralGlobalAuthorization = 0xb0,
			typeEphemeralUniqueAuthorization, typeBasicAuthorization,
			typeAdministrativeAuthorization, typeStaleNetworkKey = 0xfe,
			typeVoid = static_cast<uint8_t>(-1) };

	// Construction
	public:
		CZigBeeSecurityMaterial(const uint_fast8_t nKeyType = typeVoid);

	// Attributes
	public:
		// Outgoing frame counter
		uint32_t m_nOutgoingFrameCounter;

		// A sequence number assigned to a network key by the Trust Center and
		// used to distinguish network keys for purposes of key updates, and
		// incoming frame security operations
		uint8_t m_nKeySequenceNumber;

		// The type of the key. As of ZigBee 2012, 0x01 = network key,
		// 0x02 = application link key, 0x04 = trust center link key. With
		// the advent of Zigbee Direct, further key types are available, e.g.
		// basic authorization keys and administrative authorization keys
		uint8_t m_nKeyType;

	// Operations
	public:

	// Implementation
	public:
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeSecurityNonce

class CZigBeeSecurityNonce : public CIEEE802154AESCCMNonce
{
	// typedefs & enums
	public:
		// Security control field values (in addition to security level field
		// defined in IEEE 802.15.4)
		enum { securityDataKey, securityNetworkKey = 0x08,
			securityKeyTransportKey = 0x10, securityKeyLoadKey = 0x18,
			securityKeyIdentifierMask = 0x18, securityExtendedNonce = 0x20 };

		// Key identifiers
		enum { keyData, keyNetwork, keyKeyTransport, keyKeyLoad };

	// Construction
	public:
		CZigBeeSecurityNonce(const uint_fast8_t nSecurityLevel,
			const uint_fast8_t nKeyIdentifier,
			const CIEEE802154Address &source, const bool bExtendedNonce,
			const uint_fast32_t nFrameCounter);
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeSecurityServiceProvider

class CZigBeeSecurityServiceProvider : public CIEEE802154AESCCM,
	public CZigBeeSecurityHMAC
{
	// typedefs & enums
	public:
		// Key types
		enum { deriveKeyTransportKey, deriveKeyLoadKey = 2,
			deriveKeyConfirmationHash };

		// Authorization levels. Used to derive authorization keys
		enum { authorizationBasic = 3, authorizationAdministrative };

	// Operations
	public:
		// Encrypts and/or authenticates a message using AES-CCM*
		static bool EncryptAndAuthenticate(CPacket &message,
			const uint_fast8_t nSecurityLevel,
			const uint_fast8_t nKeyIdentifier,
			const CIEEE802154Address &source, const bool bExtendedNonce,
			const uint_fast32_t nFrameCounter,
			const CZigBeeSecurityMaterial &material,
			const CPacketData::size_type cbHeaders);

		// Decrypts and/or authenticates a message created using AES-CCM*
		static bool DecryptAndAuthenticate(CPacket &message,
			const uint_fast8_t nSecurityLevel,
			const uint_fast8_t nKeyIdentifier,
			const CIEEE802154Address &source, const bool bExtendedNonce,
			const uint_fast32_t nFrameCounter,
			const CZigBeeSecurityMaterial &material,
			const CPacketData::size_type cbAuthenticationData);

		// Derives a key-load or key-transport key from a link key
		static void DeriveKey(uint8_t abKey[blockSize],
			const uint8_t *const pbKey,
			const uint_fast8_t nKeyType = deriveKeyTransportKey);

		// Derives a basic or administrative authorization key from a network
		// or trust-center link key and a target device address (EUI-64)
		static void DeriveAuthorizationKey(uint8_t authorizationKey[16],
			const uint_fast64_t address, const uint8_t key[16],
			const uint_fast8_t type = authorizationBasic);
};
