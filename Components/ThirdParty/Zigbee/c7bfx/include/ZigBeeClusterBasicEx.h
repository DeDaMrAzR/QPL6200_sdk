// ZigBeeClusterBasicEx.h : header file
//
// Copyright(C) 2015-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of ubisys ZigBee Cluster Library. Provides extensions to the the ZCL
// basic cluster, which are required for Lighting & Occupancy Devices, for
// example

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterBasicEx

class CZigBeeClusterBasicEx : public CZigBeeClusterBasic
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Identifiers of the basic device information attribute set
		enum { attributeGenericDeviceClass = 8, attributeGenericDeviceType,
			attributeProductCode, attributeProductURL,
			attributeFirmwareBuildID = 0x4000 };

	// Construction
	public:
		CZigBeeClusterBasicEx(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Attribute storage for attribute 0x0008, global scope, read-only
		static __COMPACT_STOCK unsigned char bGenericDeviceClass;

		// Attribute storage for attribute 0x0009, global scope, read-only
		static __COMPACT_STOCK unsigned char bGenericDeviceType;

		// Attribute storage for attribute 0x000a, global scope, read-only
		static __COMPACT_STOCK unsigned char abProductCode[];

		// Attribute storage for attribute 0x000b, global scope, read-only
		static __COMPACT_STOCK char achProductURL[];

		// Attribute storage for attribute 0x4000, global scope, read-only
		static __COMPACT_STOCK char achFirmwareBuildID[];
};

#define __COMPACT7B_BASIC_GENERIC_DEVICE_CLASS() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasicEx, \
		CZigBeeClusterBasicEx::attributeGenericDeviceClass, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bGenericDeviceClass)

#define __COMPACT7B_BASIC_GENERIC_DEVICE_TYPE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasicEx, \
		CZigBeeClusterBasicEx::attributeGenericDeviceType, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		bGenericDeviceType)

#define __COMPACT7B_BASIC_GENERIC_PRODUCT_CODE() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasicEx, \
		CZigBeeClusterBasicEx::attributeProductCode, \
		CZigBeeAttribute::typeRawData, 0, \
		CZigBeeAttribute::accessReadOnly, \
		abProductCode)

#define __COMPACT7B_BASIC_GENERIC_PRODUCT_URL() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasicEx, \
		CZigBeeClusterBasicEx::attributeProductURL, \
		CZigBeeAttribute::typeString, 0, \
		CZigBeeAttribute::accessReadOnly, \
		achProductURL)

#define __COMPACT7B_BASIC_FIRMWARE_BUILD_ID() \
	__COMPACT7B_ATTRIBUTE_CLASS(CZigBeeClusterBasicEx, \
		CZigBeeClusterBasicEx::attributeFirmwareBuildID, \
		CZigBeeAttribute::typeString, 0, \
		CZigBeeAttribute::accessReadOnly, \
		achFirmwareBuildID)

#define __COMPACT7B_BASIC_CLUSTER_ATTRIBUTES_EX() \
	__COMPACT7B_BASIC_GENERIC_DEVICE_CLASS() \
	__COMPACT7B_BASIC_GENERIC_DEVICE_TYPE() \
	__COMPACT7B_BASIC_GENERIC_PRODUCT_CODE() \
	__COMPACT7B_BASIC_GENERIC_PRODUCT_URL() \
	__COMPACT7B_BASIC_FIRMWARE_BUILD_ID()
