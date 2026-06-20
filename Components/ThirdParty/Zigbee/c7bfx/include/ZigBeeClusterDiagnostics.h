// ZigBeeClusterDiagnostics.h : header file
//
// Copyright(C) 2017 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of ubisys ZigBee Cluster Library. Provides the diagnostics cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterDiagnostics

class CZigBeeClusterDiagnostics : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the diagnostics cluster
		enum { id = 0x0b05 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Identifiers of the hardware information attribute set
		enum { attributeDeviceResets = 0x0000,
			attributePersistentStorageWrites };

		// Identifiers of the stack/network information attribute set
		enum { attributeL2BroadcastsReceived = 0x0100,
			attributeL2BroadcastsTransmitted, attributeL2UnicastsReceived,
			attributeL2UnicastsTransmitted, attributeL2UnicastsRetried,
			attributeL2UnicastsFailed, attributeL7BroadcastsReceived,
			attributeL7BroadcastsTransmitted, attributeL7UnicastsReceived,
			attributeL7UnicastsSuccessful, attributeL7UnicastsRetried,
			attributeL7UnicastsFailed, attributeRouteDiscoveriesInitiated,
			attributeNeighborAdditions, attributeNeighborRemovals,
			attributeNeighborExpirations, attributeJoinIndications,
			attributeChildHandovers, attributeL4FrameCounterFailures,
         attributeL7FrameCounterFailures, attributeUnauthorizedKeys,
			attributeL4DecryptionFailures, attributeL7DecryptionFailures,
			attributePacketBufferAllocationFailures, attributeUnicastsRelayed,
			attributeL2InboundQueueOverflows, attributePacketValidationErrors,
			attributeL2AverageRetries, attributeLastMessageLQI,
			attributeLastMessageRSSI };

	// Construction
	public:
		CZigBeeClusterDiagnostics(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Number of times the device has been reset
		static uint16_t nDeviceResets;

		// Number of store operations to persistent storage
		static uint16_t nPersistentStorageWrites;

		// Link quality value for the last message received, minus 1. Note that
		// for a device reading the LastMessageLQI the returned value SHALL be
		// the LQI for the read attribute message used to read the attribute
		// itself
		static uint8_t nLastMessageLQI;

		// This is the receive signal strength indication for the last message
		// received. As with LastMessageLQI, a device reading the
		// LastMessageRSSI, the returned value SHALL be the RSSI of the read
		// attribute message used to read the attribute itself
		static int8_t nLastMessageRSSI;

	// Overrides
	public:
		// Processes the read attributes command. This is an overloaded handler
		// used to enforce an update of last message LQI and last message RSSI
		void OnReadAttributes(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

	// Implementation
	public:
		// Revision of the Diagnostics cluster
		static const unsigned short nClusterRevision;
};

#define __COMPACT7B_DIAGNOSTICS_DEVICERESETS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterDiagnostics, \
		CZigBeeClusterDiagnostics::attributeDeviceResets, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nDeviceResets)

#define __COMPACT7B_DIAGNOSTICS_PERSISTENTSTORAGEWRITES() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterDiagnostics, \
		CZigBeeClusterDiagnostics::attributePersistentStorageWrites, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nPersistentStorageWrites)

#define __COMPACT7B_DIAGNOSTICS_L2BROADCASTSRECEIVED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2BroadcastsReceived, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nBroadcastsReceived)

#define __COMPACT7B_DIAGNOSTICS_L2BROADCASTSTRANSMITTED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2BroadcastsTransmitted, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nBroadcastsTransmitted)

#define __COMPACT7B_DIAGNOSTICS_L2UNICASTSRECEIVED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2UnicastsReceived, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nUnicastsReceived)

#define __COMPACT7B_DIAGNOSTICS_L2UNICASTSTRANSMITTED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2UnicastsTransmitted, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nUnicastsTransmitted)

#define __COMPACT7B_DIAGNOSTICS_L2UNICASTSRETRIED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2UnicastsRetried, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nUnicastsRetried)

#define __COMPACT7B_DIAGNOSTICS_L2UNICASTSFAILED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2UnicastsFailed, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nUnicastsFailed)

#define __COMPACT7B_DIAGNOSTICS_L7BROADCASTSRECEIVED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7BroadcastsReceived, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nBroadcastsReceived)

#define __COMPACT7B_DIAGNOSTICS_L7BROADCASTSTRANSMITTED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7BroadcastsTransmitted, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nBroadcastsTransmitted)

#define __COMPACT7B_DIAGNOSTICS_L7UNICASTSRECEIVED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7UnicastsReceived, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nUnicastsReceived)

#define __COMPACT7B_DIAGNOSTICS_L7UNICASTSSUCCESSFUL() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7UnicastsSuccessful, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nUnicastsSuccessful)

#define __COMPACT7B_DIAGNOSTICS_L7UNICASTSRETRIED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7UnicastsRetried, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nUnicastsRetried)

#define __COMPACT7B_DIAGNOSTICS_L7UNICASTSFAILED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7UnicastsFailed, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nUnicastsFailed)

#define __COMPACT7B_DIAGNOSTICS_ROUTEDISCOVERIESINITIATED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeRouteDiscoveriesInitiated, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nRouteDiscoveriesInitiated)

#define __COMPACT7B_DIAGNOSTICS_NEIGHBORADDITIONS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeNeighborAdditions, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nNeighborAdditions)

#define __COMPACT7B_DIAGNOSTICS_NEIGHBORREMOVALS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeNeighborRemovals, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nNeighborRemovals)

#define __COMPACT7B_DIAGNOSTICS_NEIGHBOREXPIRATIONS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeNeighborExpirations, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nNeighborExpirations)

#define __COMPACT7B_DIAGNOSTICS_JOININDICATIONS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeJoinIndications, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nJoinIndications)

#define __COMPACT7B_DIAGNOSTICS_CHILDHANDOVERS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeChildHandovers, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nChildHandovers)

#define __COMPACT7B_DIAGNOSTICS_L4FRAMECOUNTERFAILURES() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL4FrameCounterFailures, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nFrameCounterFailures)

#define __COMPACT7B_DIAGNOSTICS_L7FRAMECOUNTERFAILURES() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7FrameCounterFailures, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nFrameCounterFailures)

#define __COMPACT7B_DIAGNOSTICS_UNAUTHORIZEDKEYS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeUnauthorizedKeys, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nUnauthorizedKeys)

#define __COMPACT7B_DIAGNOSTICS_L4DECRYPTIONFAILURES() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL4DecryptionFailures, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nDecryptionFailures)

#define __COMPACT7B_DIAGNOSTICS_L7DECRYPTIONFAILURES() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL7DecryptionFailures, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_aps.m_nDecryptionFailures)

#define __COMPACT7B_DIAGNOSTICS_PACKETBUFFERALLOCATIONFAILURES() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributePacketBufferAllocationFailures, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nPacketBufferAllocationFailures)

#define __COMPACT7B_DIAGNOSTICS_UNICASTSRELAYED() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeUnicastsRelayed, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nwk.m_nUnicastsRelayed)

#define __COMPACT7B_DIAGNOSTICS_L2INBOUNDQUEUEOVERFLOWS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2InboundQueueOverflows, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nInboundQueueOverflows)

#define __COMPACT7B_DIAGNOSTICS_PACKETVALIDATIONERRORS() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributePacketValidationErrors, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_nPacketValidationErrors)

#define __COMPACT7B_DIAGNOSTICS_L2AVERAGERETRIES() \
	__COMPACT7B_ATTRIBUTE_GLOBAL \
		(CZigBeeClusterDiagnostics::attributeL2AverageRetries, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		&theZigBeeStatistics.m_mac.m_nAverageRetries)

#define __COMPACT7B_DIAGNOSTICS_LASTMESSAGELQI() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterDiagnostics, \
		CZigBeeClusterDiagnostics::attributeLastMessageLQI, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nLastMessageLQI)

#define __COMPACT7B_DIAGNOSTICS_LASTMESSAGERSSI() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterDiagnostics, \
		CZigBeeClusterDiagnostics::attributeLastMessageRSSI, \
		CZigBeeAttribute::typeSigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		nLastMessageRSSI)

#define __COMPACT7B_DIAGNOSTICS_CLUSTER_ATTRIBUTES() \
	__COMPACT7B_DIAGNOSTICS_L2AVERAGERETRIES() \
	__COMPACT7B_DIAGNOSTICS_LASTMESSAGELQI() \
	__COMPACT7B_DIAGNOSTICS_LASTMESSAGERSSI()
