// ZigBeeClusterOnOff.h : header file
//
// Copyright(C) 2009-2015 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the on/off server cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterOnOff

class CZigBeeClusterOnOff : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the on/off cluster
		enum { id = 0x0006 };

		// Cluster revision of this implementation
		enum { revision = 2 };

		// Attribute identifiers of the on/off cluster
		enum { attributeOnOff };

		// Command IDs for the on/off cluster
		enum { commandOff, commandOn, commandToggle };

	// Construction
	public:
		CZigBeeClusterOnOff(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Attribute storage for attribute 0x0000, read-only (on/off)
		bool m_bOnOff;

	// Operations
	public:
		// Can be used to set the value programmatically (e.g. from another
		// application on the same device).
		// When invoked during the power-up phase, setting the value will
		// occur locally without interaction with other clusters such as
		// level or color control on the same endpoint.
		virtual void SetValue(const bool bOnOff, const bool bPowerup = false);

	// Overrides
	public:
		// Stores relevant attributes as a scene extension field set in the
		// supplied packet object
		virtual bool StoreSceneAttributes(CPacket &scene) const;

		// Applies a pre-stored extension field set to the appropriate
		// attributes. Transition time is specified in tenths of a second
		virtual bool LoadSceneAttributes(const CPacket &scene,
			const unsigned int nTransitionTime);

	// Implementation
	public:
		// Revision of the OnOff cluster
		static const unsigned short nClusterRevision;

		// Processes an "on" command
		void OnTurnOn(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes an "off" command
		void OnTurnOff(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes a "toggle" command
		void OnToggle(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);
};

#define __COMPACT7B_ONOFF_ONOFF(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOnOff::attributeOnOff, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)
