// ZigBeeClusterElectricalMeasurement.h : header file
//
// Copyright(C) 2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library. Provides the Home Automation
// Electrical Measurement Cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterElectricalMeasurement

class CZigBeeClusterElectricalMeasurement : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of simple metering cluster
		enum { id = 0x0b04 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		enum { typeActiveAC = 0x0000001, typeReactiveAC = 0x00000002,
			typeApparentAC = 0x00000004, typePhaseA = 0x00000008,
			typePhaseB = 0x00000010, typePhaseC = 0x00000020,
			typeDC = 0x00000040, typeHarmonics = 0x00000080,
			typeQuality = 0x00000100 };

		// Identifiers of the simple metering cluster
		enum { attributeMeasurementType, attributeFrequency = 0x0300,
			attributeFrequencyMin, attributeFrequencyMax,
			attributeNeutralCurrent, attributeTotalActivePower,
			attributeTotalReactivePower, attributeTotalApparentPower,

			attributeFrequencyMultiplier = 0x0400, attributeFrequencyDivisor,
			attributePowerMultiplier, attributePowerDivisor,

			attributeInstantaneousVoltage = 0x0500,
			attributeInstantaneousCurrent,
			attributeActiveCurrent, attributeReactiveCurrent,
			attributeInstantaneousPower, attributeRMSVoltage,
			attributeRMSVoltageMin, attributeRMSVoltageMax,
			attributeRMSCurrent, attributeRMSCurrentMin, attributeRMSCurrentMax,
			attributeActivePower, attributeActivePowerMin,
			attributeActivePowerMax, attributeReactivePower,
			attributeApparentPower, attributePowerFactor,

			attributeVoltageMultiplier = 0x0600, attributeVoltageDivisor,
			attributeCurrentMultiplier, attributeCurrentDivisor,
			attributePowerMultiplier2, attributePowerDivisor2
		};

	// Construction
	public:
		CZigBeeClusterElectricalMeasurement
			(CZigBeeFoundationApplication &application,
			 const unsigned int nType = typeActiveAC | typeReactiveAC |
				 typeApparentAC | typePhaseA);

	// Attributes
	public:
		// Attribute storage for attribute 0x0000, read-only (measurement type)
		unsigned int m_nType;

		unsigned short m_nFrequency;
		unsigned short m_nFrequencyMin;
		unsigned short m_nFrequencyMax;
		short m_nVoltage;
		short m_nCurrent;
		int m_nTotalActivePower;
		int m_nTotalReactivePower;
		unsigned int m_nTotalApparentPower;

		short m_nActivePower;
		short m_nReactivePower;
		unsigned short m_nApparentPower;

		unsigned short m_nVoltageRMS;
		unsigned short m_nCurrentRMS;
		unsigned short m_nPowerMultiplier;
		unsigned short m_nPowerDivisor;
		unsigned short m_nVoltageMultiplier;
		unsigned short m_nVoltageDivisor;
		unsigned short m_nCurrentMultiplier;
		unsigned short m_nCurrentDivisor;
		signed char m_nPowerFactor;

	// Operations
	public:

	// Implementation
	public:
		// Revision of the ElectricalMeasurement cluster
		static const unsigned short nClusterRevision;
};

#define __COMPACT7B_ELECTRICALMEASUREMENT_TYPE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeMeasurementType, \
		CZigBeeAttribute::typeBitmap32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_FREQUENCY(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeFrequency, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_FREQUENCYMIN(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeFrequencyMin, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_FREQUENCYMAX(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeFrequencyMax, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_INSTANTANEOUSVOLTAGE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeInstantaneousVoltage, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_INSTANTANEOUSCURRENT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeInstantaneousCurrent, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_ACTIVECURRENT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeActiveCurrent, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_REACTIVECURRENT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeReactiveCurrent, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_INSTANTANEOUSPOWER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeInstantaneousPower, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_RMSVOLTAGE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeRMSVoltage, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_RMSVOLTAGEMIN(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeRMSVoltageMin, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_RMSVOLTAGEMAX(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeRMSVoltageMax, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_RMSCURRENT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeRMSCurrent, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_RMSCURRENTMIN(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeRMSCurrentMin, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_RMSCURRENTMAX(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeRMSCurrentMax, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_POWERFACTOR(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributePowerFactor, \
		CZigBeeAttribute::typeSigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_ACTIVEPOWER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeActivePower, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_REACTIVEPOWER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeReactivePower, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_APPARENTPOWER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeApparentPower, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_TOTALACTIVEPOWER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeTotalActivePower, \
		CZigBeeAttribute::typeSigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_TOTALREACTIVEPOWER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeTotalReactivePower, \
		CZigBeeAttribute::typeSigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_ELECTRICALMEASUREMENT_TOTALAPPARENTPOWER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterElectricalMeasurement::attributeTotalApparentPower, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)
