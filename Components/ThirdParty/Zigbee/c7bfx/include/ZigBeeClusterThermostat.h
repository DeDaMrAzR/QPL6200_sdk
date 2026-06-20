// ZigBeeClusterThermostat.h : header file
//
// Copyright(C) 2015-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the thermostat cluster (server side)

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterThermostat

class CZigBeeClusterThermostat : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the thermostat cluster
		enum { id = 0x0201 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Attribute identifiers of the thermostat cluster
		enum { attributeLocalTemperature, attributeOutdoorTemperature,
			attributeOccupancy, attributeAbsoluteMinimumHeatingSetpoint,
			attributeAbsoluteMaximumHeatingSetpoint,
			attributeAbsoluteMinimumCoolingSetpoint,
			attributeAbsoluteMaximumCoolingSetpoint, attributeCoolingDemand,
			attributeHeatingDemand, attributeSystemTypeConfiguration,
			attributeLocalTemperatureCalibration = 0x0010,
			attributeOccupiedCoolingSetpoint, attributeOccupiedHeatingSetpoint,
			attributeUnoccupiedCoolingSetpoint,
			attributeUnoccupiedHeatingSetpoint, attributeMinimumHeatingSetpoint,
			attributeMaximumHeatingSetpoint, attributeMinimumCoolingSetpoint,
			attributeMaximumCoolingSetpoint, attributeMinimumSetpointDeadband,
			attributeRemoteSensing, attributeControlSequence,
			attributeMode, attributeAlarmMask, attributeRunningMode,
			attributeStartOfWeek = 0x0020, attributeNumberOfWeeklyTransitions,
			attributeNumberOfDailyTransitions, attributeTemperatureSetpointHold,
			attributeTemperatureSetpointHoldDuration,
			attributeProgrammingOperationMode, attributeRunningState = 0x0029,
			attributeSetPointChangeSource = 0x0030,
			attributeSetPointChangeAmount, attributeSetpointChangeTimestamp,
			attributeAirConditioningType = 0x0040, attributeAirConditioningCapacity,
			attributeAirConditioningRefrigerantType,
			attributeAirConditioningCompressorType,
			attributeAirConditioningErrorCode,
			attributeAirConditioningLouverPosition,
			attributeAirConditioningCoilTemperature,
			attributeAirConditioningCapacityFormat
		};

		// Command IDs for the thermostat cluster (client to server)
		enum { commandStepSetpoint, commandSetWeeklySchedule,
			commandGetWeeklySchedule, commandClearWeeklySchedule,
			commandGetRelayStatusLog };

		// Command IDs for the thermostat cluster (server to client)
		enum { commandGetWeeklyScheduleResponse,
			commandGetRelayStatusLogResponse };

		// Thermostat system modes
		enum { modeOff, modeAuto, modeCool = 3, modeHeat, modeEmergencyHeat,
			modePrecool, modeFanOnly, modeDry, modeSleep };

		// Thermostat system modes supported by this device
		enum { supportsOff = 1 << modeOff, supportsAuto = 1 << modeAuto,
			supportsCool = 1 << modeCool, supportsHeat = 1 << modeHeat,
			supportsEmergencyHeat = 1 << modeEmergencyHeat,
			supportsPrecool = 1 << modePrecool,
			supportsFanOnly = 1 << modeFanOnly, supportsDry = 1 << modeDry,
			supportsSleep = 1 << modeSleep };

		// Thermostat running states
		enum { operationHeatStage1, operationCoolStage1, operationFanStage1,
			operationHeatStage2, operationCoolStage2, operationFanStage2,
			operationFanStage3 };

		// Thermostat control sequence of operation
		enum { controlCoolingOnly, controlCoolingWithReheat,
			controlHeatingOnly, controlHeatingWithReheat,
			controlCoolingAndHeating, controlCoolingAndHeatingWithReheat };

		// Thermostat control sequences of operation supported by this device
		enum { supportsCoolingOnly = 1 << controlCoolingOnly,
			supportsCoolingWithReheat = 1 << controlCoolingWithReheat,
			supportsHeatingOnly = 1 << controlHeatingOnly,
			supportsHeatingWithReheat = 1 << controlHeatingWithReheat,
			supportsCoolingAndHeating = 1 << controlCoolingAndHeating,
			supportsCoolingAndHeatingWithReheat =
				1 << controlCoolingAndHeatingWithReheat,
			supportsAllControlSequences = supportsCoolingOnly |
				supportsCoolingWithReheat | supportsHeatingOnly |
				supportsHeatingWithReheat | supportsCoolingAndHeating |
				supportsCoolingAndHeatingWithReheat };

		// Thermostat HVAC system types
		enum { systemCoolStage1, systemCoolStage2, systemCoolStage3,
			systemCoolStageMask = 0x03, systemCoolStageShift = 0,
			systemHeatStage1 = 0x00, systemHeatStage2 = 0x04,
			systemHeatStage3 = 0x08, systemHeatStageMask = 0x0c,
			systemHeatStageShift = 2, systemTypeHeatPump = 0x10,
			systemEnergySourceGas = 0x20 };

	// Construction
	public:
		CZigBeeClusterThermostat(CZigBeeFoundationApplication &application,
			const unsigned char nControlSequence = controlHeatingOnly,
			const unsigned char nSupportedControlSequences = supportsHeatingOnly,
			const unsigned short nSupportedModes = supportsOff | supportsAuto |
				supportsHeat);

	// Attributes
	public:
		// Attribute storage for attribute 0x0001, read-only (outdoor
		// temperature). Notice that the storage space for this attribute is
		// shared by all instances of CZigBeeClusterThermostat
		static short nOutdoorTemperature;

		// Attribute storage for attribute 0x0000, read-only (local temperature)
		short m_nLocalTemperature;

		// Attribute storage for attribute 0x0011 (occupied cooling set-point)
		short m_nOccupiedCoolingSetpoint;

		// Attribute storage for attribute 0x0012 (occupied heating set-point)
		short m_nOccupiedHeatingSetpoint;

		// Attribute storage for attribute 0x0013 (unoccupied cooling set-point)
		short m_nUnoccupiedCoolingSetpoint;

		// Attribute storage for attribute 0x0014 (unoccupied heating set-point)
		short m_nUnoccupiedHeatingSetpoint;

		// Attribute storage for attribute 0x001b (control sequence of
		// operation)
		unsigned char m_nControlSequence;

		// Specifies the generally supported system modes for this thermostat
		unsigned char m_nSupportedControlSequences;

		// Attribute storage for attribute 0x001c (system mode)
		unsigned char m_nMode;

		// Specifies the generally supported system modes for this thermostat
		unsigned short m_nSupportedModes;

		// Attribute storage for attribute 0x0002 (occupancy)
		bool m_bOccupancy;

		// Returns true if the thermostat generally allows heating
		bool AllowsHeating() const;

		// Returns true if the thermostat generally allows cooling
		bool AllowsCooling() const;

		// Returns true if the thermostat generally allows reheating
		bool AllowsReheating() const;

		// Returns true of the specified control sequence of operation allows
		// reheating
		static bool AllowsReheating(const uint_fast8_t controlSequence);

		// Returns true if the supplied mode is a heating-related mode
		bool IsHeatingMode(const unsigned char nMode) const;

		// Returns true if the supplied mode is a cooling-related mode
		bool IsCoolingMode(const unsigned char nMode) const;

		// Returns true if the thermostat is currently able to heat
		bool CanHeat() const;

		// Returns true if the thermostat is currently able to cool
		bool CanCool() const;

		// Returns the minimum setpoint deadband in units of 0.01°C
		int16_t GetMinimumSetpointDeadband() const;

	// Operations
	public:


	// Overrides
	public:
		// Performs additional logical checks when attempts are made to write
		// the system mode attribute
		virtual unsigned char DoWriteAttribute
			(const CZigBeeAttribute &attribute,
			const void *const pValue, const size_t cbValue);

		// Stores relevant attributes as a scene extension field set in the
		// supplied packet object
		virtual bool StoreSceneAttributes(CPacket &scene) const;

		// Applies a pre-stored extension field set to the appropriate
		// attributes. Transition time is specified in tenths of a second
		virtual bool LoadSceneAttributes(const CPacket &scene,
			const unsigned int nTransitionTime);

	// Implementation
	public:
		// Revision of the Thermostat cluster
		static const unsigned short nClusterRevision;

		// Heating setpoint bounds (absolute limits)
		static const std::pair<int16_t, int16_t> heatingRange;

		// Cooling setpoint bounds (absolute limits)
		static const std::pair<int16_t, int16_t> coolingRange;

		// Minimum setpoint dead-band bounds
		static const std::pair<int8_t, int8_t> deadbandRange;

		// Returns true if the supplied mode is permissible, taking the current
		// control sequence of operation into account
		bool IsModeAllowed(const unsigned char nMode) const;

		// Returns true if the supplied mode is supported in general
		bool IsModeSupported(const unsigned char nMode) const;

		// Returns true if the control sequence of operation is supported
		bool IsControlSequenceSupported(const unsigned char nMode) const;

		// Processes a "setpoint raise/lower" command
		void OnStepSetpoint(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);
};

#define __COMPACT7B_THERMOSTAT_LOCALTEMPERATURE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeLocalTemperature, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_OUTDOORTEMPERATURE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLASS(cls, \
		CZigBeeClusterThermostat::attributeOutdoorTemperature, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_OCCUPANCY(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeOccupancy, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_ABSOLUTEMINIMUMHEATINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLASS(cls, \
		CZigBeeClusterThermostat::attributeAbsoluteMinimumHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_THERMOSTAT_ABSOLUTEMAXIMUMHEATINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLASS(cls, \
		CZigBeeClusterThermostat::attributeAbsoluteMaximumHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_THERMOSTAT_ABSOLUTEMINIMUMCOOLINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLASS(cls, \
		CZigBeeClusterThermostat::attributeAbsoluteMinimumCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_THERMOSTAT_ABSOLUTEMAXIMUMCOOLINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLASS(cls, \
		CZigBeeClusterThermostat::attributeAbsoluteMaximumCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::accessReadOnly, \
		member)

#define __COMPACT7B_THERMOSTAT_COOLINGDEMAND(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeCoolingDemand, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_HEATINGDEMAND(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeHeatingDemand, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_OCCUPIEDCOOLINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeOccupiedCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_OCCUPIEDCOOLINGSETPOINT_EX(cls, member, \
	range) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeOccupiedCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, range, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_OCCUPIEDHEATINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeOccupiedHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_OCCUPIEDHEATINGSETPOINT_EX(cls, member, \
	range) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeOccupiedHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, range, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_UNOCCUPIEDCOOLINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeUnoccupiedCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_UNOCCUPIEDCOOLINGSETPOINT_EX(cls, member, \
	range) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeUnoccupiedCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, range, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_UNOCCUPIEDHEATINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeUnoccupiedHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, 0, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_UNOCCUPIEDHEATINGSETPOINT_EX(cls, member, \
	range) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeUnoccupiedHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, range, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_MINIMUMHEATINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeMinimumHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, &heatingRange, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_THERMOSTAT_MAXIMUMHEATINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeMaximumHeatingSetpoint, \
		CZigBeeAttribute::typeSigned16, &heatingRange, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_THERMOSTAT_MINIMUMCOOLINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeMinimumCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, &coolingRange, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_THERMOSTAT_MAXIMUMCOOLINGSETPOINT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeMaximumCoolingSetpoint, \
		CZigBeeAttribute::typeSigned16, &coolingRange, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_THERMOSTAT_MINIMUMSETPOINTDEADBAND(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeMinimumSetpointDeadband, \
		CZigBeeAttribute::typeSigned8, &deadbandRange, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_THERMOSTAT_CONTROLSEQUENCE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeControlSequence, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::persistent, \
		member)

#define __COMPACT7B_THERMOSTAT_MODE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeMode, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::persistent | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_THERMOSTAT_RUNNINGMODE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterThermostat::attributeRunningMode, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)
