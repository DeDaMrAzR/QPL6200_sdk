// ZigBeeClusterOnOffEx.h : header file
//
// Copyright(C) 2015 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides enhancements to the on/off cluster for Lighting & Occupancy

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterOnOffEx

class CZigBeeClusterOnOffEx : public CZigBeeClusterOnOff
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Attribute identifiers of the on/off cluster
		enum { attributeGlobalSceneControl = 0x4000, attributeOnTime,
			attributeOffWaitTime, attributeStartupOnOff };

		// Command IDs for the on/off cluster
		enum { commandOffWithEffect = 0x40, commandOnAndRecallGlobalScene,
			commandOnWithTimedOff };

		// Start-up behavior
		enum { startupOff, startupOn, startupToggle, startupRestore = 0xff };

		// On with timed off control options
		enum { acceptOnlyWhenOn = 0x01 };

	// Construction
	public:
		CZigBeeClusterOnOffEx(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Attribute storage for attribute 0x4000, read-only
		bool m_bGlobalSceneControl;

		// Attribute storage for attribute 0x4001
		unsigned short m_nOnTime;

		// Attribute storage for attribute 0x4002
		unsigned short m_nOffWaitTime;

		// Attribute storage for attribute 0x4003
		unsigned char m_nStartupOnOff;

	// Operations
	public:

	// Overrides
	public:
		// This method is called when an attribute's value has changed. Calls
		// the applications notification handlers and manages attribute
		// reporting, if applicable
		virtual void OnAttributeChanged(const unsigned short wAttributeID,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0);

		// Called in an early stage when the device is powered-up. Applies the
		// configured start-up behavior
		virtual void AfterPowerup();

	// Implementation
	public:
		// Timer for "on with timed off" behavior
		CCompactTimer *m_pTimer;

		// Processes an "off with effect" command
		void OnTurnOffWithEffect(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes an "on with recall global scene" command
		void OnTurnOnAndRecallGlobalScene(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Processes an "on with timed off" command
		void OnTurnOnWithTimedOff(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// This timer fires every 100ms as long as the OnTime or OffWaitTime
		// attributes are non-zero (and decrements both)
		void OnTimerExpired(void *pArgument);
};

#define __COMPACT7B_ONOFFEX_GLOBALSCENECONTROL(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOnOffEx::attributeGlobalSceneControl, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ONOFFEX_ONTIME(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOnOffEx::attributeOnTime, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, member)

#define __COMPACT7B_ONOFFEX_OFFWAITTIME(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOnOffEx::attributeOffWaitTime, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, member)

#define __COMPACT7B_ONOFFEX_STARTUPONOFF(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOnOffEx::attributeStartupOnOff, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_ONOFFEX_ONOFF(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOnOff::attributeOnOff, \
		CZigBeeAttribute::typeBoolean, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable | \
			CZigBeeAttribute::persistentVolatile4x1, \
		member)
