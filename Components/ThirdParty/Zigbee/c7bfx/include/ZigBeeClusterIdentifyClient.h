// ZigBeeClusterIdentifyClient.h : header file
//
// Copyright(C) 2020 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the Identify client cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIdentifyClient

class CZigBeeClusterIdentifyClient : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the Identify client cluster
		enum { id = CZigBeeClusterIdentify::id };

	// Construction
	public:
		CZigBeeClusterIdentifyClient
			(CZigBeeFoundationApplication &application);

	// Operations
	public:
		// Issues an identify command to a remote device
		static void RequestIdentify(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID, const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius,
			const unsigned short wIdentifyTime);

		// Issues an identify command via the zcl queue to a remote device
		static void QueueIdentify(CZigBeeCluster &cluster,
			const CZigBeeAddress &destination,
			const unsigned short wIdentifyTime,
			const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged,
			const unsigned char bRadius = 0,
			const unsigned int nTimeout = 0,
			const CZigBeeClusterRequest::CALLBACK pfnComplete = 0);

		// Issues an identify query command to a remote device
		static void RequestQueryIdentify(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID, const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius);

		// Issues an identify query command via the zcl queue to a remote device
		static void QueueQueryIdentify(CZigBeeCluster &cluster,
			const CZigBeeClusterRequest::CALLBACK pfnComplete = 0,
			const unsigned int nTimeout = defaultTimeout,
			const CZigBeeAddress &destination =
				CZigBeeAddress(CZigBeeAddress::qwExtendedVoid,
				CZigBeeAddress::wBroadcast, CZigBeeAddress::endpointBroadcast),
			const unsigned char nTransmitOptions = 0,
			const unsigned char bRadius = 0,
			const bool bMultipleResponses = true);

	// Implementation
	public:
		// Revision of the Identify client cluster
		static const unsigned short nClusterRevision;
};
