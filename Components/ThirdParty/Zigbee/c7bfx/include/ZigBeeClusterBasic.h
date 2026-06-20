// ZigBeeClusterBasic.h : header file
//
// Copyright(C) 2009-2016 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of ubisys ZigBee Cluster Library. Provides the ZCL basic cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterBasic

class CZigBeeClusterBasic : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the basic cluster
		enum { id = 0x0000 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// This is the value of the ZCL version attribute of the basic cluster.
		// For ZCLv6 (ZigBee 3.0) this has been set to 2. A value of 0 or 1
		// indicates ZCL version 4 or below. Notice that version 5 was purely
		// editorial. This is not to be confused with the cluster revision of
		// the Basic cluster, which is e.g. 1 for ZCLv6 and 2 for ZCLv7 (which
		// also sets the ZCL version to 3).
		// Starting with ZCL8, the value of the ZCL version shall be aligned
		// with the release version of the ZCL.
		enum { version4 = 1, version6, version7, version8 = 8,
			version = version8 };

		// Cluster command identifiers
		enum { commandResetToFactoryDefaults };

		// Identifiers of the basic device information attribute set
		enum { attributeClusterLibraryVersion, attributeApplicationVersion,
			attributeStackVersion, attributeHardwareVersion,
			attributeManufacturerName, attributeModelIdentifier,
			attributeDateCode, attributePowerSource };

		// Identifiers of the basic device configuration attribute set
		enum { attributeLocationDescription = 0x0010,
			attributePhysicalEnvironment, attributeDeviceEnabled,
			attributeAlarmMask };

		// Power sources
		enum { sourceUnknown, sourceMains1, sourceMains3, sourceBattery,
			sourceDC, sourceEmergencyMainsConstantlyPowered,
			sourceEmergencyMainsTransferSwitch, sourceHasBackup = 0x80 };

	// Construction
	public:
		CZigBeeClusterBasic(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Attribute storage for attribute 0x0000, global scope, read-only
		static const unsigned char bClusterLibraryVersion;

		// Attribute storage for attribute 0x0001, global scope, read-only
		static __COMPACT_STOCK unsigned char bApplicationVersion;

		// Attribute storage for attribute 0x0002, global scope, read-only
		static __COMPACT_STOCK unsigned char bStackVersion;

		// Attribute storage for attribute 0x0003, global scope, read-only
		static __COMPACT_STOCK unsigned char bHardwareVersion;

		// Attribute storage for attribute 0x0004, global scope, read-only
		static __COMPACT_STOCK char achManufacturerName[];

		// Attribute storage for attribute 0x0005, global scope, read-only
		static __COMPACT_STOCK char achModelIdentifier[];

		// Attribute storage for attribute 0x0006, global scope, read-only,
		// ISO 8601 notation (YYYYMMDD)
		static __COMPACT_STOCK char achDateCode[];

		// Attribute storage for attribute 0x0007, global scope, read-only
		static __COMPACT_STOCK unsigned char bPowerSource;

		// Attribute storage for attribute 0x0010, global scope
		static char achLocationDescription[];

		// Attribute range information for attribute 0x0010, global scope
		static const unsigned char abLocationDescriptionRange[2];

		// Attribute storage for attribute 0x0011, global scope
		static unsigned char bPhysicalEnvironment;

	// Implementation
	public:
		// Revision of the Basic cluster
		static const unsigned short nClusterRevision;

		// Processes incoming cluster command frames
		void OnFactoryDefaults(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);
};

#define __COMPACT7B_BASIC_ZCL_VERSION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeClusterLibraryVersion, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bClusterLibraryVersion)

#define __COMPACT7B_BASIC_APPLICATION_VERSION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeApplicationVersion, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bApplicationVersion)

#define __COMPACT7B_BASIC_STACK_VERSION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeStackVersion, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bStackVersion)

#define __COMPACT7B_BASIC_HARDWARE_VERSION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeHardwareVersion, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bHardwareVersion)

#define __COMPACT7B_BASIC_MANUFACTURER_NAME() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeManufacturerName, \
		CZigBeeAttribute::typeString, 0, \
		CZigBeeAttribute::accessReadOnly, \
		achManufacturerName)

#define __COMPACT7B_BASIC_MODEL_IDENTIFIER() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeModelIdentifier, \
		CZigBeeAttribute::typeString, 0, \
		CZigBeeAttribute::accessReadOnly, \
		achModelIdentifier)

#define __COMPACT7B_BASIC_DATE_CODE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeDateCode, \
		CZigBeeAttribute::typeString, 0, \
		CZigBeeAttribute::accessReadOnly, \
		achDateCode)

#define __COMPACT7B_BASIC_POWER_SOURCE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributePowerSource, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bPowerSource)

#define __COMPACT7B_BASIC_LOCATION_DESCRIPTION() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributeLocationDescription, \
		CZigBeeAttribute::typeString, &abLocationDescriptionRange, \
		CZigBeeAttribute::persistent, \
		achLocationDescription)

#define __COMPACT7B_BASIC_PHYSICAL_ENVIRONMENT() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasic, \
		CZigBeeClusterBasic::attributePhysicalEnvironment, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::persistent, \
		bPhysicalEnvironment)

#define __COMPACT7B_BASIC_CLUSTER_ATTRIBUTES() \
	__COMPACT7B_BASIC_ZCL_VERSION() \
	__COMPACT7B_BASIC_APPLICATION_VERSION() \
	__COMPACT7B_BASIC_STACK_VERSION() \
	__COMPACT7B_BASIC_HARDWARE_VERSION() \
	__COMPACT7B_BASIC_MANUFACTURER_NAME() \
	__COMPACT7B_BASIC_MODEL_IDENTIFIER() \
	__COMPACT7B_BASIC_DATE_CODE() \
	__COMPACT7B_BASIC_POWER_SOURCE() \
	__COMPACT7B_BASIC_LOCATION_DESCRIPTION() \
	__COMPACT7B_BASIC_PHYSICAL_ENVIRONMENT()
