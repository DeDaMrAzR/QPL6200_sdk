// ZigBeeClusterMetering.h : header file
//
// Copyright(C) 2009-2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the metering cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterMetering

class CZigBeeClusterMetering : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of metering cluster
		enum { id = 0x0702 };

		// Cluster revision of this implementation
		enum { revision = 2 };

		// Status bitmap
		enum { statusCheckMeter = 0x01, statusLowBattery = 0x02,
			statusTamperDetect = 0x04, statusPowerFailure = 0x08,
			statusPowerQuality = 0x10, statusLeakDetect = 0x20,
			statusServiceDisconnectOpen = 0x40 };

		// Units of measure, e.g. unitKiloWatts | unitFlagBinaryCodedDecimal
		// "Please Note: When using BCD for meter reads, the values A to F are
		// special values or indicators denoting "Opens", "Shorts", and etc.
		// conditions when reading meter register hardware. Any SE device
		// displaying the BCD based values to end users should use a
		// non-decimal value to replace the A to F. In other words, a device
		// could use an "*" in place of the special values or indicators."
		enum { unitKiloWatts, unitCubicMeters, unitCubicFeet,
			unitCentumCubicFeet, unitUSGallons, unitImperialGallons,
			unitBTU, unitLiters, unitKiloPascal, unitKiloPascalAbsolute,
			unitFlagBinaryCodedDecimal = 0x80 };

		// Metering device type
		enum { typeElectricMetering, typeGasMetering, typeWaterMetering,
			typeThermalMetering, typePressureMetering, typeHeatMetering,
			typeCoolingMetering, typeFlagMirroredMetering = 0x80 };

		// Identifiers of the metering cluster
		enum { attributeCurrentSummationDelivered,
			attributeCurrentSummationReceived,
			attributeCurrentMaxDemandDelivered,
			attributeCurrentMaxDemandReceived,
			attributeDailyFreezeTimeSummation, attributeDailyFreezeTime,
			attributePowerFactor, attributeStatus = 0x0200,
			attributeUnitOfMeasure = 0x0300, attributeMultiplier,
			attributeDivisor, attributeSummationFormatting,
			attributeDemandFormatting, attributeHistoricalConsumptionFormatting,
			attributeDeviceType, attributeInstantaneousDemand = 0x0400 };

	// Construction
	public:
		CZigBeeClusterMetering(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Represents the most recent summed value of energy, gas, or water
		// delivered and consumed in the premise. Mandatory. This is a 48-bit
		// value stored in 64-bit here.
		unsigned long long m_nCurrentSummationDelivered;

		// Represents the most recent summed value of energy, gas, or water
		// generated and delivered by the premise. Optional. This is a 48-bit
		// value stored in 64-bit here.
		unsigned long long m_nCurrentSummationReceived;

		// Represents the maximum demand or rate of delivered value of energy,
		// gas, or water being utilized at the premise. If optionally provided,
		// the value is updated continuously as new measurements are made.
		// This is a 48-bit value stored in 64-bit here.
		unsigned long long m_nCurrentMaxDemandDelivered;

		// Represents the maximum demand or rate of received value of energy,
		// gas, or water being utilized by the utility. If optionally provided,
		// the value is updated continuously as new measurements are made.
		// This is a 48-bit value stored in 64-bit here.
		unsigned long long m_nCurrentMaxDemandReceived;

		// Represents the current demand of energy, gas, or water delivered or
		// received at the premise. Positive values indicate demand delivered
		// to the premise where negative values indicate demand received from
		// the premise. The value is updated continuously as new measurements
		// are made. The frequency of updates to this field is specific to the
		// metering device, but should be within the range of once every second
		// to once every 5 seconds. This is a 24-bit value stored in 32-bit
		// here.
		int m_nInstantaneousDemand;

		// Contains the average power factor ratio in 1/100ths. Valid values
		// are -100 to 100
		signed char m_nPowerFactor;

		// Provides indicators reflecting the current error conditions found
		// by the metering device
		unsigned char m_bStatus;

		// Provides a label for the Energy, Gas, or Water being measured by
		// the metering device. The unit of measure apply to all summations,
		// consumptions/profile interval and demand/rate supported by this
		// cluster
		unsigned char m_nUnitOfMeasure;

		// Provides a value to be multiplied against a raw or uncompensated
		// sensor count of Energy, Gas, or Water being measured by the metering
		// device. If present, this attribute must be applied against all
		// summation, consumption and demand values to derive the delivered and
		// received values expressed in the unit of measure specified. This
		// attribute must be used in conjunction with the Divisor attribute.
		// This is a 24-bit value stored in 32-bit here
		unsigned int m_nMultiplier;

		// Provides a value to divide the results of applying the Multiplier
		// Attribute against a raw or uncompensated sensor count of Energy, Gas,
		// or Water being measured by the metering device. If present, this
		// attribute must be applied against all summation, consumption and
		// demand values to derive the delivered and received values expressed
		// in the unit of measure specified. This attribute must be used in
		// conjunction with the Multiplier attribute.
		// This is a 24-bit value stored in 32-bit here
		unsigned int m_nDivisor;

		// Provides indicators reflecting the current error conditions found by
		// the metering device
		unsigned char m_nStatus;

	// Operations
	public:
		void OnNewInstantaneusDemandMeasurement(const int nInstantaneousDemand);

	// Implementation
	public:
		// Revision of the Metering cluster
		static const unsigned short nClusterRevision;
};


#define __COMPACT7B_METERING_CURRENTSUMMATIONDELIVERED(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeCurrentSummationDelivered, \
		CZigBeeAttribute::typeUnsigned48, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_CURRENTSUMMATIONRECEIVED(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeCurrentSummationReceived, \
		CZigBeeAttribute::typeUnsigned48, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_CURRENTMAXDEMANDDELIVERED(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeCurrentMaxDemandDelivered, \
		CZigBeeAttribute::typeUnsigned48, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_CURRENTMAXDEMANDRECEIVED(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeCurrentMaxDemandReceived, \
		CZigBeeAttribute::typeUnsigned48, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_POWERFACTOR(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributePowerFactor, \
		CZigBeeAttribute::typeSigned8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_INSTANTANEOUSDEMAND(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeInstantaneousDemand, \
		CZigBeeAttribute::typeSigned24, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_METERING_STATUS(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeStatus, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_UNITOFMEASURE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeUnitOfMeasure, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_MULTIPLIER(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeMultiplier, \
		CZigBeeAttribute::typeUnsigned24, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_METERING_DIVISOR(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterMetering::attributeDivisor, \
		CZigBeeAttribute::typeUnsigned24, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)
