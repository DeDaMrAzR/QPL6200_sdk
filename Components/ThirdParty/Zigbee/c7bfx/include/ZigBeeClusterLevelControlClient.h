// ZigBeeClusterLevelControlClient.h : header file
//
// Copyright(C) 2009-2016 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the level control client cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterLevelControlClient

class CZigBeeClusterLevelControlClient : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the level control configuration cluster
		enum { id = CZigBeeClusterLevelControl::id };

	// Construction
	public:
		CZigBeeClusterLevelControlClient
			(CZigBeeFoundationApplication &application);

	// Operations
	public:
		// Creates a move to level command (client side)
		static void CreateMoveToLevelCommand(CPacket &asdu,
			const unsigned char nLevel, const unsigned short wTransitionTime,
			const bool bOnOff = false);

		// Assembles and transmits a move to level command (client to server)
		static void RequestMoveToLevel(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius,
			const unsigned char nLevel, const unsigned short wTransitionTime,
			const bool bOnOff = true);

		// Creates a move command (client side)
		static void CreateMoveCommand(CPacket &asdu, const unsigned char bMode,
			const unsigned char bRate, const bool bOnOff = false);

		// Assembles and transmits a move command (client to server)
		static void RequestMove(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius,
			const unsigned char bMode, const unsigned char bRate,
			const bool bOnOff = true);

		// Creates a step command (client side)
		static void CreateStepCommand(CPacket &asdu, const unsigned char bMode,
			const unsigned char bStepSize,
			const unsigned short wTransitionTime,
			const bool bOnOff = false);

		// Assembles and transmits a step command (client to server)
		static void RequestStep(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius,
			const unsigned char bMode, const unsigned char bStepSize,
			const unsigned short wTransitionTime,
			const bool bOnOff = true);

		// Creates a stop command (client side)
		static void CreateStopCommand(CPacket &asdu,
			const bool bOnOff = false);

		// Assembles and transmits a stop command (client to server)
		static void RequestStop(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius,
			const bool bOnOff = true);

	// Implementation
	public:
		// Revision of the LevelControl client cluster
		static const unsigned short nClusterRevision;
};
