// ZigBeeClusterRelativeHumidityMeasurement.h : header file
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
// Provides the server side of the relative humidity measurement cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterRelativeHumidityMeasurement

class CZigBeeClusterRelativeHumidityMeasurement : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of temperature measurement cluster
		enum { id = 0x0405 };

		// Cluster revision of this implementation
		enum { revision = 2 };

		// Identifiers of the temperature measurement cluster
		enum { attributeMeasuredValue, attributeMinMeasurableValue,
			attributeMaxMeasurableValue, attributeTolerance };

	// Construction
	public:
		CZigBeeClusterRelativeHumidityMeasurement
			(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Represents the most recent summed value of energy, gas, or water
		// delivered and consumed in the premise. Mandatory. This is a 48-bit
		// value stored in 64-bit here.
		unsigned short m_nRelativeHumidity;

		// Range of temperature measurements supported by the device. Notice
		// this should not change. Rather it is the full range the device is
		// capable of measuring
		std::pair<unsigned short, unsigned short> m_range;

		// Tolerance. This also should not change (but it could, in theory and
		// it could also be reportable)
		unsigned short m_nTolerance;

	// Implementation
	public:
		// Revision of the RelativeHumidityMeasurement cluster
		static const unsigned short nClusterRevision;
};

#define __COMPACT7B_RELATVEHUMIDITYMEASUREMENT_MEASUREDVALUE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterRelativeHumidityMeasurement::attributeMeasuredValue, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_RELATVEHUMIDITYMEASUREMENT_MINMEASURABLEVALUE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterRelativeHumidityMeasurement::attributeMinMeasurableValue, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_RELATVEHUMIDITYMEASUREMENT_MAXMEASURABLEVALUE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterRelativeHumidityMeasurement::attributeMaxMeasurableValue, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_RELATVEHUMIDITYMEASUREMENT_TOLERANCE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterRelativeHumidityMeasurement::attributeTolerance, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)
