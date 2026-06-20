// ZigBeeClusterTemperatureMeasurement.h : header file
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
// Provides the temperature measurement server cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterTemperatureMeasurement

class CZigBeeClusterTemperatureMeasurement : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of temperature measurement cluster
		enum { id = 0x0402 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Identifiers of the temperature measurement cluster
		enum { attributeMeasuredValue, attributeMinMeasurableValue,
			attributeMaxMeasurableValue, attributeTolerance };

	// Construction
	public:
		CZigBeeClusterTemperatureMeasurement
			(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Represents the current temperature. Mandatory. This is a 16-bit
		// signed integer in units of 0.01°C
		short m_nTemperature;

		// Range of temperature measurements supported by the device. Notice
		// this should not change. Rather it is the full range the device is
		// capable of measuring
		std::pair<short, short> m_range;

		// Tolerance. This also should not change (but it could, in theory and
		// it could also be reportable)
		unsigned short m_nTolerance;

	// Implementation
	public:
		// Revision of the TemperatureMeasurement cluster
		static const unsigned short nClusterRevision;
};

#define __COMPACT7B_TEMPERATUREMEASUREMENT_MEASUREDVALUE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterTemperatureMeasurement::attributeMeasuredValue, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_TEMPERATUREMEASUREMENT_MINMEASURABLEVALUE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterTemperatureMeasurement::attributeMinMeasurableValue, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_TEMPERATUREMEASUREMENT_MAXMEASURABLEVALUE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterTemperatureMeasurement::attributeMaxMeasurableValue, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_TEMPERATUREMEASUREMENT_TOLERANCE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterTemperatureMeasurement::attributeTolerance, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)
