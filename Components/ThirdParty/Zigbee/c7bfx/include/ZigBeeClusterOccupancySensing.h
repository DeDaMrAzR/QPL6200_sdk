// ZigBeeClusterOccupancySensing.h : header file
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
// Provides the server side of the occupancy sensing cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterOccupancySensing

class CZigBeeClusterOccupancySensing : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of occupancy sensing cluster
		enum { id = 0x0406 };

		// Cluster revision of this implementation
		enum { revision = 2 };

		// Identifiers of the temperature measurement cluster
		enum { attributeOccupancy, attributeSensorType,
			attributeSensorTypeEx,
			attributePIROccupiedToUnoccupiedDelay = 0x0010,
			attributePIRUnoccupiedToOccupiedDelay,
			attributePIRUnoccupiedToOccupiedThreshold,
			attributeUltrasonicOccupiedToUnoccupiedDelay = 0x0020,
			attributeUltrasonicUnoccupiedToOccupiedDelay,
			attributeUltrasonicUnoccupiedToOccupiedThreshold};

		// Sensor types (legacy enumeration value)
		enum { typePIR, typeUltrasonic, typePIRAndUltrasonic,
			typePhysicalContact };

		// Sensor types (bitmap)
		enum { typeexPIR = 0x01, typeexUltrasonic = 0x02,
			typeexPhysicalContact = 0x04 };

	// Construction
	public:
		CZigBeeClusterOccupancySensing
			(CZigBeeFoundationApplication &application,
			const uint_fast8_t nSensorType = typePIR,
			const uint_fast8_t nSensorTypeEx = typeexPIR);

	// Attributes
	public:
		// Sensor value: either occupied or unoccupied encoded in the LSB
		bool m_bOccupancy;

		// Sensor type (legacy enumeration value): either PIR, ultrasonic,
		// PIR and ultrasonic, or physical contact
		uint8_t m_nSensorType;

		// Sensor type (new bitmap representation): arbitrary combination of
		// PIR, ultrasonic, and physical contact
		uint8_t m_nSensorTypeEx;

	// Implementation
	public:
		// Revision of the OccupancySensing cluster
		static const unsigned short nClusterRevision;
};

#define __COMPACT7B_OCCUPANCYSENSING_OCCUPANCY(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOccupancySensing::attributeOccupancy, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_OCCUPANCYSENSING_SENSORTYPE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOccupancySensing::attributeSensorType, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_OCCUPANCYSENSING_SENSORTYPEEX(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterOccupancySensing::attributeSensorTypeEx, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)
