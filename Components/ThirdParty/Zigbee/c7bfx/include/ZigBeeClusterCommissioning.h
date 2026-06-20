// ZigBeeClusterCommissioning.h : header file
//
// Copyright(C) 2009-2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the commissioning cluster


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterCommissioning

class CZigBeeClusterCommissioning : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of commissioning cluster
		enum { id = 0x0015 };

		// Cluster revision of this implementation
		enum { revision = 2 };

		// Cluster command identifiers
		enum { commandRestartDevice, commandSaveStartupParameters,
			commandRestoreStartupParameters, commandResetStartupParameters };

		// Identifiers of the start-up attribute attribute set
		enum { attributeShortAddress, attributeExtendedPANID, attributePANID,
			attributeChannelMask, attributeProtocolVersion,
			attributeStackProfile, attributeStartupControl,
			attributeTrustCenterAddress = 0x10, attributeTrustCenterMasterKey,
			attributeNetworkKey, attributeUseInsecureJoin,
			attributePreconfiguredLinkKey, attributeNetworkKeySequence,
			attributeNetworkKeyType, attributeNetworkManagerAddress,
			attributeScanAttempts = 0x20, attributeTimeBetweenScans,
			attributeRejoinInterval, attributeMaxRejoinInterval,
			attributeIndirectPollRate = 0x30, attributeParentRetryThreshold,
			attributeIsConcentrator = 0x40, attributeConcentratorRadius,
			attributeConcentratorDiscoveryTime };

		// This is a ubisys extension
		enum { attributeTransmitPower = 0x07 };

	// Attributes
	public:
		// ZigBee Alliance global commissioning Extended PANID. Additional 64K
		// addresses immediately following are also reserved for commissioning
		static const unsigned long long qwCommissioningEPID;

	// Construction
	public:
		CZigBeeClusterCommissioning(CZigBeeFoundationApplication &application);

	// Operations
	public:
		// Stores the current set of commissioning attributes in persistent
		// storage
		static void Store();

	// Overrides
	public:
		// Called when the ZigBee network start-up procedure is complete. When
		// successful, networking is operational
		virtual void OnNotifyStartupComplete(const unsigned char nStatus,
			const unsigned short nFlags);

	// Implementation
	public:
		// Revision of the Commissioning cluster
		static const unsigned short nClusterRevision;

		// Joint attribute storage for all attributes
		static CZigBeeCommissioningAttributes attributes;

		// Attribute storage for attribute 0x0004, global scope, read-only
		static const unsigned char bProtocolVersion;

		// Attribute storage for attribute 0x0005, global scope, read-only
		static const unsigned char bStackProfile;

		// Send a commissioning cluster response frame
		void TransmitResponse(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned char bStatus);

		// Restart the device
		void OnRestartDevice(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Reset the start-up parameters
		void OnResetStartupParameters(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// If this timer fires, the local station shall restart either
		// installing a new startup attribute set or reverting to the previous
		// one. The argument is non-zero if new attributes shall be used, false
		// otherwise
		void OnRestartDeviceTimeout(void *pArgument);
};

#define __COMPACT7B_COMMISSIONING_SHORT_ADDRESS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeShortAddress, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, \
		attributes.m_wShortAddress)

#define __COMPACT7B_COMMISSIONING_EXTENDED_PANID() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeExtendedPANID, \
		CZigBeeAttribute::typeIEEEAddress, 0, \
		0, \
		attributes.m_qwExtendedPANID)

#define __COMPACT7B_COMMISSIONING_PANID() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributePANID, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, \
		attributes.m_wPANID)

#define __COMPACT7B_COMMISSIONING_CHANNEL_MASK() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeChannelMask, \
		CZigBeeAttribute::typeBitmap32, 0, \
		0, \
		attributes.m_dwChannelMask)

#define __COMPACT7B_COMMISSIONING_PROTOCOL_VERSION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeProtocolVersion, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bProtocolVersion)

#define __COMPACT7B_COMMISSIONING_STACK_PROFILE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeStackProfile, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bStackProfile)

#define __COMPACT7B_COMMISSIONING_STARTUP_CONTROL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeStartupControl, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		0, \
		attributes.m_bStartupControl)

#define __COMPACT7B_COMMISSIONING_TRUSTCENTER_ADDRESS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeTrustCenterAddress, \
		CZigBeeAttribute::typeIEEEAddress, 0, \
		0, \
		attributes.m_qwTrustCenterAddress)

#define __COMPACT7B_COMMISSIONING_TRUSTCENTER_MASTERKEY() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeTrustCenterMasterKey, \
		CZigBeeAttribute::typeSecurityKey, 0, \
		0, \
		attributes.m_abTrustCenterMasterKey[0])

#define __COMPACT7B_COMMISSIONING_NETWORK_KEY() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeNetworkKey, \
		CZigBeeAttribute::typeSecurityKey, 0, \
		0, \
		attributes.m_abNetworkKey[0])

#define __COMPACT7B_COMMISSIONING_USE_INSECURE_JOIN() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeUseInsecureJoin, \
		CZigBeeAttribute::typeBoolean, 0, \
		0, \
		attributes.m_bUseInsecureJoin)

#define __COMPACT7B_COMMISSIONING_PRECONFIGURED_LINK_KEY() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributePreconfiguredLinkKey, \
		CZigBeeAttribute::typeSecurityKey, 0, \
		0, \
		attributes.m_abPreconfiguredLinkKey[0])

#define __COMPACT7B_COMMISSIONING_NETWORK_KEY_SEQUENCE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeNetworkKeySequence, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		0, \
		attributes.m_bNetworkKeySequence)

#define __COMPACT7B_COMMISSIONING_NETWORK_KEY_TYPE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeNetworkKeyType, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		0, \
		attributes.m_bNetworkKeyType)

#define __COMPACT7B_COMMISSIONING_NETWORK_MANAGER_ADDRESS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeNetworkManagerAddress, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, \
		attributes.m_wNetworkManagerAddress)

#define __COMPACT7B_COMMISSIONING_SCAN_ATTEMPTS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeScanAttempts, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		0, \
		attributes.m_bScanAttempts)

#define __COMPACT7B_COMMISSIONING_TIME_BETWEEN_SCANS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeTimeBetweenScans, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, \
		attributes.m_wTimeBetweenScans)

#define __COMPACT7B_COMMISSIONING_REJOIN_INTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeRejoinInterval, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, \
		attributes.m_wRejoinInterval)

#define __COMPACT7B_COMMISSIONING_MAXREJOIN_INTERVAL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeMaxRejoinInterval, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, \
		attributes.m_wMaxRejoinInterval)

#define __COMPACT7B_COMMISSIONING_INDIRECT_POLL_RATE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeIndirectPollRate, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, \
		attributes.m_wIndirectPollRate)

#define __COMPACT7B_COMMISSIONING_PARENT_RETRY_THRESHOLD() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeParentRetryThreshold, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		0, \
		attributes.m_bParentRetryThreshold)

#define __COMPACT7B_COMMISSIONING_IS_CONCENTRATOR() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeIsConcentrator, \
		CZigBeeAttribute::typeBoolean, 0, \
		0, \
		attributes.m_bIsConcentrator)

#define __COMPACT7B_COMMISSIONING_CONCENTRATOR_RADIUS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeConcentratorRadius, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		0, \
		attributes.m_bConcentratorRadius)

#define __COMPACT7B_COMMISSIONING_CONCENTRATOR_DISCOVERY_TIME() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeConcentratorDiscoveryTime, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		0, \
		attributes.m_bConcentratorDiscoveryTime)

#define __COMPACT7B_COMMISSIONING_TRANSMIT_POWER() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterCommissioning, \
		CZigBeeClusterCommissioning::attributeTransmitPower, \
		CZigBeeAttribute::typeSigned8, 0, \
		0, \
		attributes.m_bTransmitPower)

#define __COMPACT7B_COMMISSIONING_CLUSTER_ATTRIBUTES() \
	__COMPACT7B_COMMISSIONING_SHORT_ADDRESS() \
	__COMPACT7B_COMMISSIONING_EXTENDED_PANID() \
	__COMPACT7B_COMMISSIONING_PANID() \
	__COMPACT7B_COMMISSIONING_CHANNEL_MASK() \
	__COMPACT7B_COMMISSIONING_PROTOCOL_VERSION() \
	__COMPACT7B_COMMISSIONING_STACK_PROFILE() \
	__COMPACT7B_COMMISSIONING_STARTUP_CONTROL() \
	__COMPACT7B_COMMISSIONING_TRUSTCENTER_ADDRESS() \
	__COMPACT7B_COMMISSIONING_TRUSTCENTER_MASTERKEY() \
	__COMPACT7B_COMMISSIONING_NETWORK_KEY() \
	__COMPACT7B_COMMISSIONING_USE_INSECURE_JOIN() \
	__COMPACT7B_COMMISSIONING_PRECONFIGURED_LINK_KEY() \
	__COMPACT7B_COMMISSIONING_NETWORK_KEY_SEQUENCE() \
	__COMPACT7B_COMMISSIONING_NETWORK_KEY_TYPE() \
	__COMPACT7B_COMMISSIONING_NETWORK_MANAGER_ADDRESS() \
	__COMPACT7B_COMMISSIONING_SCAN_ATTEMPTS() \
	__COMPACT7B_COMMISSIONING_TIME_BETWEEN_SCANS() \
	__COMPACT7B_COMMISSIONING_REJOIN_INTERVAL() \
	__COMPACT7B_COMMISSIONING_MAXREJOIN_INTERVAL() \
	__COMPACT7B_COMMISSIONING_INDIRECT_POLL_RATE() \
	__COMPACT7B_COMMISSIONING_PARENT_RETRY_THRESHOLD() \
	__COMPACT7B_COMMISSIONING_IS_CONCENTRATOR() \
	__COMPACT7B_COMMISSIONING_CONCENTRATOR_RADIUS() \
	__COMPACT7B_COMMISSIONING_CONCENTRATOR_DISCOVERY_TIME() \
	__COMPACT7B_COMMISSIONING_TRANSMIT_POWER()
