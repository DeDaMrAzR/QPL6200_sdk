// ZigBeeCommissioningAttributes.h : header file
//
// Copyright(C) 2012-2018 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// Commissioning cluster attributes (single persistent storage item)


/////////////////////////////////////////////////////////////////////////////
// CZigBeeCommissioningAttributes

class CZigBeeCommissioningAttributes
{
	// typedefs & enums
	public:
		enum { startupCommissioned, startupFormNetwork, startupRejoinNetwork,
			startupJoinNetwork };

		// The 'always announce' option results in a device announcement 
		// after reboot, not just when joining or rejoining a network. It
		// is useful for certain eco-systems, which require a device 
		// announcement after power-up. In general, it is recommended to
		// disable this feature, though. After power cycling a large number
		// of devices this would otherwise result in a broadcast storm
		enum { optionAlwaysAnnounce = 0x01 };

	// Attributes
	public:
		// Attribute storage for attribute 0x0011, global scope. Notice: With
		// the removal of high security mode and SKKE commands from the R21
		// specification, this usage is obsolete. Instead, R21+ trust centers
		// store their global link key here. In addition, R23+ devices store
		// the network's symmetric passphrase here (in which case the value
		// is different from its all-zeros default value)
		uint8_t m_abTrustCenterMasterKey[16];

		// Attribute storage for attribute 0x0012, global scope
		uint8_t m_abNetworkKey[16];

		// Attribute storage for attribute 0x0014, global scope
		uint8_t m_abPreconfiguredLinkKey[16];

		// Attribute storage for attribute 0x0001, global scope
		uint64_t m_qwExtendedPANID;

		// Attribute storage for attribute 0x0010, global scope
		uint64_t m_qwTrustCenterAddress;

		// Attribute storage for attribute 0x0003, global scope
		uint32_t m_dwChannelMask;

		// Attribute storage for attribute 0x0002, global scope
		uint16_t m_wPANID;

		// Attribute storage for attribute 0x0000, global scope
		uint16_t m_wShortAddress;

		// Attribute storage for attribute 0x0017, global scope
		uint16_t m_wNetworkManagerAddress;

		// Attribute storage for attribute 0x0006, global scope
		uint8_t m_bStartupControl;

		// Attribute storage for attribute 0x0013, global scope
		bool m_bUseInsecureJoin;

		// Attribute storage for attribute 0x0015, global scope
		uint8_t m_bNetworkKeySequence;

		// Attribute storage for attribute 0x0016, global scope
		uint8_t m_bNetworkKeyType;

		// Attribute storage for attribute 0x0021, global scope
		uint16_t m_wTimeBetweenScans;

		// Attribute storage for attribute 0x0022, global scope
		uint16_t m_wRejoinInterval;

		// Attribute storage for attribute 0x0023, global scope
		uint16_t m_wMaxRejoinInterval;

		// Attribute storage for attribute 0x0020, global scope
		uint8_t m_bScanAttempts;

		// Attribute storage for attribute 0x0031, global scope
		uint8_t m_bParentRetryThreshold;

		// Attribute storage for attribute 0x0030, global scope
		uint16_t m_wIndirectPollRate;

		// Attribute storage for attribute 0x0040, global scope
		bool m_bIsConcentrator;

		// Attribute storage for attribute 0x0041, global scope
		uint8_t m_bConcentratorRadius;

		// Attribute storage for attribute 0x0042, global scope
		uint8_t m_bConcentratorDiscoveryTime;

		// Attribute storage for attribute 0x1f02:0x0000, global scope
		int8_t m_bTransmitPower;

		// Attribute storage for attribute 0x1f02:0x0001, global scope
		// Additional start-up options, e.g. broadcast device announcement
		// after each reboot?
		uint8_t m_nOptions;

		// Attribute storage for attribute 0x1f02:0x0002, global scope
		// Additional out-of-band commissioning item: channel
		uint8_t m_nChannel;

		// Attribute storage for attribute 0x1f02:0x0003, global scope
		// Additional out-of-band commissioning item: network update identifier
		uint8_t m_nNetworkUpdateID;

		// Reserved (padding)
		uint8_t m_nReserved;
};
