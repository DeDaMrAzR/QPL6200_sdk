// ZigBeeApplicationSecurity.h : header file
//
// Copyright(C) 2008-2022 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This implementation is based on the ZigBee Alliance's ZigBee Specification
// as of January 17, 2008
//
// This module is provides the security service for the application support
// sublayer (APS) according to ZigBee 2007

/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSecurityMaterial

class CZigBeeApplicationSecurityMaterial : public CZigBeeSecurityMaterial
{
	// typedefs & enums
	public:
		// The "derived" flag is used on a trust center only and allows a derived
		// trust-center link key to the device to be stored efficiently. If the
		// key is derived from the TC master key, the actual key is not stored.
		// A key with the "explicit" flag has been created by application logic,
		// for example by adding an installation code, and should not be erased
		// when the device attempts to join from scratch. The "pre-configured"
		// flag is used in two instances. If the local device is not the trust
		// center, then an entry for the trust center with this flag set means
		// that the install code has been used to join the network; in case the
		// local device is the trust center, an entry with the "pre-configured"
		// flag means that this is the pre-configured key (installation code)
		// for an anticipated device (i.e. provisional key); "agreed" means
		// that the key is the outcome of a key agreement process; "pending
		// agreement" means that key agreement has been started, but is pending
		// a reply, "pending confirmation" means this is a key that is still
		// pending confirmation (trust center link key exchange or key
		// agreement in progress), "pending erasure" means that this key is
		// about to be erased. The "pendingXyz" flags are ephemeral, whereas
		// the bits in the lower nibble are stationary. When the 'low-entropy'
		// flag is set, the entry stores a passcode, which is padded to
		// 128-bits. The "virtual" flag denotes an entry for a Zigbee Virtual
		// Device (ZVD) according to the Zigbee Direct specification
		enum { flagGlobal = 0x0001, flagLowEntropy = 0x0002,
			flagAuthenticationTokenDelivered = 0x0004, flagExchanged = 0x0008,
			flagVirtual = 0x0010, flagDerived = 0x0100, flagExplicit = 0x0200,
			flagPreconfigured = 0x0400, flagAgreed = 0x0800,
			flagPendingAgreement = 0x1000, flagPendingConfirmation = 0x2000,
			flagCounterVerified = 0x4000, flagPendingErasure = 0x8000,
			flagPersistentMask = 0x0fff };

		// We use 16 bits to store feature and fragmentation support data about
		// a remote device. This includes 4 bits of capabilities and 12 bits of
		// ASDU size
		enum { fragmentationSizeMask = 0x0fff,
			featureFlagsMask = 0xf000, featureFlagsShift = 12 };

		// Device capabilities, e.g. fragmentation support of the core stack
		// (ZDO endpoint #); fragmentation support on all other application
		// endpoints. A total of 4 feature flag bits is supported. The first
		// two bits are copied verbatim from the fragmentation support TLV
		enum { featureCoreFragmentation = 0x01,
			featureApplicationFragmentation = 0x02,
			featureSynchronization = 0x08 };

		// Security material initial and subsequent (post-join) device
		// authentication options. Initial authentication method in the two
		// LSBs of the lower nibble and updated authentication method in the
		// 3 LSBs of the upper nibble
		enum { initialAnonymousSimple = 0,
			initialAuthenticatedSimple = 1,
			initialAnonymousAgreement = 2,
			initialAuthenticatedAgreement = 3,
			initialAuthenticationMask = 0x03,
			initialAuthenticationShift = 0,
			updatedViaKeyRequest = 1,
			updatedViaAnonymousAgreement = 2,
			updatedViaAuthenticatedAgreement = 3,
			updatedViaApplicationCertificate = 4,
			updatedAuthenticationMask = 0x70,
			updatedAuthenticationShift = 4 };

	// Construction
	public:
		CZigBeeApplicationSecurityMaterial();

		CZigBeeApplicationSecurityMaterial(const uint8_t key[16],
			const uint_fast16_t flags,
			const uint_fast8_t type = typeTrustCenter);

	// Attributes
	public:
		// Flags, including link key type
		uint16_t m_nFlags;

		// Identifies the address of the entity with which this key-pair is
		// shared
		uint64_t m_qwDeviceAddress;

		// Incoming frame counter value corresponding to DeviceAddress
		uint32_t m_nIncomingFrameCounter;

		// Short address (might be invalid, 0xfffe = 'only extended address')
		uint16_t m_shortAddress;

		// Maximum ASDU transfer size in the lower 12 bits (up to 8KB ASDU),
		// and feature support flags in the upper 4 bits. If more is required,
		// some of the remaining main flags above might be used before adding
		// more data fields or extending the field size
		uint16_t m_features;

		// Device authentication information. This is a combination of the
		// initial join authentication method in the lower nibble (two bits
		// in use, currently), and the update method used for the most recent
		// (active) key in the upper nibble (3 bits in use, currently)
		uint8_t m_authentication;

		// True for a unique link key, false for a global link key
		bool IsUnique() const;

		// True for a pre-configured link key, i.e. an install-code derived
		// key or low-entry passcode
		bool IsPreconfigured() const;

		// Returns true if the global key is valid (non-zero)
		static bool IsGlobalKeyValid();

		// Returns the initial authentication method
		uint_fast8_t GetInitialAuthentication() const;

		// Returns the authentication method used for the most recent update
		uint_fast8_t GetUpdatedAuthentication() const;

		// Returns true if the device supports fragmentation for ZDO
		bool SupportsCoreFragmentation() const;

		// Returns true if the device supports fragmentation for applications
		bool SupportsApplicationFragmentation() const;

		// Returns the size of the device's fragmentation reassembly buffer
		uint_fast16_t GetFragmentationSize() const;

		// Returns true if the device supports frame counter synchronization
		bool SupportsSynchronization() const;

	// Operations
	public:
		// Generates a new global trust center link key. Must be called once
		// when a new trust center instance is formed
		static void GenerateGlobalKey();

		// Derives a key for two devices, where one might be the trust center;
		// If one device is the trust center, qwAddress2 shall be void
		void DeriveKey(const uint_fast64_t qwAddress1,
			const uint_fast64_t qwAddress2 = CZigBeeAddress::qwExtendedVoid);

		// Derives a provisional trust-center swap-out key
		void DeriveSwapoutKey(uint8_t key[16]) const;

		// Given a variable-length installation code, calculates the resulting
		// 128-bit pre-configured link key. Does not validate the install code;
		// if needed, use CStandardISO13239CRC::Check() to validate the code
		// before calling this method
		void DerivePreconfiguredLinkKey(const void *const code,
			const uint_fast8_t size);

		// Advances the frame counter to the next value and returns the new
		// value, which can be used to construct an APS-encrypted frame
		static uint32_t AdvanceOutgoingFrameCounter();

		// Returns the device address. At least the extended address is valid,
		// and potentially both the extended address and the network short
		// address are valid
		CZigBeeAddress GetAddress() const;

	// Implementation
	public:
		// The default trust center link-key as used by some ZigBee application
		// profiles, e.g. the home automation profile. This is a global and
		// publicly well-known key ("ZigBeeAlliance09")
		static const uint8_t abDefaultTrustCenterLinkKey[16];

		// The distributed security link key is used by the ZigBee Light Link
		// Profile, for example. This is a global, typically secret key, which
		// is made available to companies after certification of products
		static const uint8_t abDistributedSecurityLinkKey[16];

		// The trust center master key is used to create unique link keys for
		// both trust center link key usage and peer-to-peer usage
		static uint8_t abGlobalKey[16];

		// One outgoing APS frame counter for all APS link-keys in order to
		// save flash memory
		static uint32_t nOutgoingFrameCounter;

		// Outgoing frame counter non-volatile storage store-ahead interval.
		// This must be a power of two and allows the device to not store all
		// frame counter increments, but store the counter only when the pre-
		// reserved counters have been consumed
		enum { storageInterval = 0x1000 };

		// Sets the global key and stores it persistently
		static void SetGlobalKey(const uint8_t key[16]);

		// Derives a key for two devices, where one might be the trust center;
		// If one device is the trust center, qwAddress2 shall be all 'F's;
		// An optional salt can be used to derive other key material than
		// partner link keys, for example R23+ symmetric authentication tokens
		static void DeriveKey(uint8_t abKey[16],
			const uint_fast64_t qwAddress1,
			const uint_fast64_t qwAddress2,
			const uint_fast8_t salt = 0);

		// Given a variable-length installation code, calculates the resulting
		// 128-bit pre-configured link key. Does not validate the install code;
		// if needed, use CStandardISO13239CRC::Check() to validate the code
		// before calling this method
		static void DerivePreconfiguredLinkKey(uint8_t key[16],
			const void *const code, const uint_fast8_t size);

		// Stores the outgoing frame counter of the given security material by
		// first adding the grace sequence number period the the frame counter,
		// indexed by key type and key sequence number
		static void StoreFrameCounter
			(const uint_fast32_t nAhead = storageInterval);

		// Loads the outgoing frame counter of the given security material
		static bool LoadFrameCounter();

		// Sets the distributed security link-key
		static void SetDistributedSecurityLinkKey(const uint8_t key[16]);

		// Gets the distributed security global link key
		static const uint8_t *GetDistributedSecurityLinkKey();

		// Gets the default trust center global link key
		static const uint8_t *GetDefaultTrustCenterLinkKey();

#ifdef _DEBUG
		// Dumps information stored in this security material set
		void Dump() const;
#endif

};

inline bool CZigBeeApplicationSecurityMaterial::IsUnique() const
	{ return !(m_nFlags & flagGlobal); }

inline bool CZigBeeApplicationSecurityMaterial::IsPreconfigured() const
	{ return m_nFlags & flagPreconfigured ? true : false; }

inline CZigBeeAddress CZigBeeApplicationSecurityMaterial::GetAddress() const
	{ return CZigBeeAddress(m_qwDeviceAddress, m_shortAddress); }

inline uint_fast8_t
CZigBeeApplicationSecurityMaterial::GetInitialAuthentication() const
{
	return (m_authentication & initialAuthenticationMask) >>
		initialAuthenticationShift;
}

inline uint_fast8_t
CZigBeeApplicationSecurityMaterial::GetUpdatedAuthentication() const
{
	return (m_authentication & updatedAuthenticationMask) >>
		updatedAuthenticationShift;
}


inline uint_fast16_t CZigBeeApplicationSecurityMaterial::
GetFragmentationSize() const
{
	return m_features & fragmentationSizeMask;
}


inline bool CZigBeeApplicationSecurityMaterial::
SupportsCoreFragmentation() const
{
	return m_features & (featureCoreFragmentation << featureFlagsShift);
}


inline bool CZigBeeApplicationSecurityMaterial::
SupportsApplicationFragmentation() const
{
	return m_features &
		(featureApplicationFragmentation << featureFlagsShift);
}


inline bool CZigBeeApplicationSecurityMaterial::
SupportsSynchronization() const
{
	return m_features & (featureSynchronization << featureFlagsShift);
}


inline void CZigBeeApplicationSecurityMaterial::DerivePreconfiguredLinkKey
	(const void *const code, const uint_fast8_t size)
{
	DerivePreconfiguredLinkKey(m_abKey, code, size);
}
