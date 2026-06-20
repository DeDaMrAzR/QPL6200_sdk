// ZigBeeClusterOnOffClient.h : header file
//
// Copyright(C) 2009-2019 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the on/off client cluster


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterOnOffClient

class CZigBeeClusterOnOffClient : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the on/off cluster
		enum { id = CZigBeeClusterOnOff::id };

	// Construction
	public:
		CZigBeeClusterOnOffClient(CZigBeeFoundationApplication &application);

	// Operations
	public:
		// Creates a packet object holding a valid ZCL frame that can be passed
		// to the APS for delivery to a peer entity. This method is called on
		// the client-side to create a frame that is delivered to the server
		// via the APSDE-DATA.request service primitive
		static void CreateCommand(CPacket &asdu, const unsigned char bCommand);

		// Creates a on/off command frame with the specified command code and
		// transmits it via APSDE-DATA.request primitive
		static void RequestCommand(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius,
			const unsigned char bCommand = CZigBeeClusterOnOff::commandToggle,
			const unsigned char bEnhancedOptions =
				CZigBeeApplicationSupportBase::priorityMedium);

	// Implementation
	public:
		// Revision of the OnOff client cluster
		static const unsigned short nClusterRevision;
};
