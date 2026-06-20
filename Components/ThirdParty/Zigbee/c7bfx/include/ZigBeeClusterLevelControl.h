// ZigBeeClusterLevelControl.h : header file
//
// Copyright(C) 2009-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the level control cluster

/////////////////////////////////////////////////////////////////////////////
// forward referenced classes

class CZigBeeValueTransition;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterLevelControl

class CZigBeeClusterLevelControl : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the level control configuration cluster
		enum { id = 0x0008 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Identifiers of the level control server cluster
		enum { attributeCurrentLevel, attributeRemainingTime,
			attributeOptions = 0x0f, attributeOnOffTransitionTime,
			attributeOnLevel, attributeOnTransitionTime,
			attributeOffTransitionTime, attributeDefaultMoveRate };

		// Identifiers for manufacturer-specific attributes of the level
		// control server cluster
		enum { attributeMinimumOnLevel };

		// Command IDs for the level control cluster
		enum { commandMoveToLevel, commandMove, commandStep, commandStop,
			commandMoveToLevelOnOff, commandMoveOnOff, commandStepOnOff,
			commandStopOnOff };

		// Minimum and maximum allowed levels
		enum { levelOff = 0, levelMinimum = 1, levelMaximum = 254,
			levelReserved = 255 };

		// Instant rates and transition times
		enum { instantRate = 0xff, zeroTransitionTime = 0,
			instantTransitionTime = 0xffff };

		// Generic Level Control cluster options
		enum { optionExecuteIfOff = 0x01, optionReserved1 = 0x02 };

		// Specific Level Control for Lighting cluster options. Couple
		// color temperature to (light intensity) level provides an option
		// to mimic the behavior of incandescent bulbs, where lower light
		// intensities are warm white, and brighter light is cool white
		enum { optionCoupleColorTemperatureToLevel = optionReserved1 };

	// Construction
	public:
		CZigBeeClusterLevelControl(CZigBeeFoundationApplication &application,
			const unsigned short nOnOffTransistionTime = 0,
			const unsigned char nOnLevel = 0xfe,
			const unsigned char nMinOnLevel = 0xff);

	// Attributes
	public:
		// Time remaining until the current command is complete (100ms units)
		unsigned short m_remainingTime;

		// Time taken to move to or from the target level when on or off
		// commands are received by an on/off cluster on the same endpoint
		// (100ms units)
		unsigned short m_nOnOffTransitionTime;

		// Current level of this device
		unsigned char m_nCurrentLevel;

		// Determines the value that the CurrentLevel attribute is set to when
		// the OnOff attribute of an on/off cluster on the same endpoint is set
		// to on. If the OnLevel attribute is not implemented, or is set to ff,
		// it has no effect
		unsigned char m_nOnLevel;

		// Default command options
		unsigned char m_bOptions;

		// Determines the minimum value that the CurrentLevel attribute should
		// be set to when the OnOff attribute of an on/off cluster on the same
		// endpoint is set to on. If the OnLevel attribute is implemented and
		// not set to 0xff, or the minimum value itself is set to 0xff, it has
		// no effect.
		unsigned char m_nMinimumOnLevel;

	// Overrides
	public:
		// Stores relevant attributes as a scene extension field set in the
		// supplied packet object
		virtual bool StoreSceneAttributes(CPacket &scene) const;

		// Applies a pre-stored extension field set to the appropriate
		// attributes. Transition time is specified in tenths of a second
		virtual bool LoadSceneAttributes(const CPacket &scene,
			const unsigned int nTransitionTime);

		// Called after power-up. Evaluates the ballast configuration settings
		// to set current level and stored level appropriately
		virtual void AfterPowerup();

	// Operations
	public:
		// Force on/off. The MSB signifies whether the on/off state is forced
		// to on or off and the target on/off value is encoded in the LSB in
		// this case
		enum { forceNone = 0, forceOnOffMask = 0x01, forceFlag = 0x80,
			forceOn = 0x81, forceOff = 0x80 };

		// Implements the move to level command (with and without on/off)
		void MoveToLevel(const unsigned char nLevel,
			const unsigned short nTransitionTime, const bool bOnOff = false,
			const uint_fast8_t forceOnOff = forceNone,
			const unsigned char rate = instantRate);

		// Implements the move command (with and without on/off)
		void Move(const bool bMode, const unsigned char nRate,
			const bool bOnOff = false);

		// Implements the step command (with and without on/off)
		void Step(const bool bMode, const unsigned char nStepSize,
			const unsigned short nTransitionTime, const bool bOnOff = false);

		// Implements the stop command (identical for both variants). Also
		// sets the target level to the current level after the current
		// transition operation has been stopped. Returns true if there was a
		// transition in progress, which has actually been stopped; false
		// otherwise
		bool Stop();

		// Must be called by an on/off cluster on the same endpoint, if an on,
		// off, or toggle command has been received. The argument bOnOff
		// expresses the new state of the OnOff attribute
		void OnOnOff(const bool bOnOff);

		// Confines the specified level to the minimum and maximum bounds as
		// supported on the device (taking into account ballast configuration)
		unsigned char ConfineLevel(const unsigned char nLevel) const;

	// Overridables
	public:
		// Returns the minimum allowed level (takes ballast configuration into
		// account, if applicable)
		virtual unsigned char GetMinimumLevel() const;

		// Returns the maximum allowed level (takes ballast configuration into
		// account, if applicable)
		virtual unsigned char GetMaximumLevel() const;

		// Called when the current level has settled, i.e. the value has been
		// changed and the transition has completed. The parameter bStored
		// indicates whether the m_nStoredLevel attribute is valid
		virtual void OnCurrentLevelSettled(const bool bStored = false) const;

		// Request to stop the current ongoing level transition. This might
		// be necessary when the level transition is correlated with other
		// cluster's functionality and needs to be stopped at first prior to
		// starting action on other cluster
		virtual void RequestStop();

	// Implementation
	public:
		// Revision of the LevelControl cluster
		static const unsigned short nClusterRevision;

		// Determines if the level control cluster sets the current level to
		// the stored level, after the device has been turned off
		static const bool bRevertToStoredLevel;

		// Timer that is used to implement fading effects
		CCompactTimer *m_pTimer;

		// Current transition, if any
		CZigBeeValueTransition *m_transition;

		// Target level. Fading will stop, when the target level is reached
		unsigned char m_nTargetLevel;

		// If true, the state of the OnOff attribute of the on/off cluster on
		// the same endpoint will be maintained (with on/off command options)
		bool m_bMaintainOnOff;

		// When an off command is received, the current level is stored here,
		// before it is moved to the minimum level
		unsigned char m_nStoredLevel;

		// Sets the value of the current level attribute, maintains a related
		// OnOff attribute and notifies the application
		void SetCurrentLevel(const unsigned char nLevel);

		// Timer call-back that does the actual fading
		void OnTimerExpired(void *pArgument);

		// Retrieves the command options by taking the payload and default into
		// account
		bool RetrieveCommandOptions(CPacket &asdu,
			unsigned char &bOptions) const;

		// Retrieves the command options and determines whether the command is
		// to be ignored (execute if off is false and the on/off attribute is
		// also false), or processing should continue.
		bool RetrieveCommandOptionsEx(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			unsigned char &bOptions);

		// Change to the on state upon receipt of an On or Toggle command by
		// an OnOff cluster on the same endpoint
		void SwitchOn();

		// Move to level (with or without on/off)
		void OnMoveToLevel(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Move (with or without on/off)
		void OnMove(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Step (with or without on/off)
		void OnStep(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Stop (with or without on/off)
		void OnStop(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Applies a new target level during level control transitions
		void Apply(CZigBeeValueTransition &transition, const uint8_t level);

		// If a level transition is currently in progress, returns information
		// about the target level, start time and end time. Notice that the
		// time is updated only if it starts sooner or ends later, i.e. the
		// caller must set startTime to UINT64_MAX, and endTime to 0 on entry
		bool GetTarget(uint8_t &level,
			CCompactTimerService::ticks_t &startTime,
			CCompactTimerService::ticks_t &endTime) const;

		// Returns true if a level control transition is currently in progress
		bool IsTransitionInProgress() const;
};

#define __COMPACT7B_LEVELCONTROL_CURRENTLEVEL(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterLevelControl::attributeCurrentLevel, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
		member)

#define __COMPACT7B_LEVELCONTROL_REMAININGTIME(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterLevelControl::attributeRemainingTime, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_LEVELCONTROL_ONOFFTRANSITIONTIME(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterLevelControl::attributeOnOffTransitionTime, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::persistent, member)

#define __COMPACT7B_LEVELCONTROL_ONLEVEL(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterLevelControl::attributeOnLevel, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::persistent, member)

#define __COMPACT7B_LEVELCONTROL_OPTIONS(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterLevelControl::attributeOptions, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::persistent, member)

#define __COMPACT7B_LEVELCONTROL_MINIMUMONLEVEL(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER_M(cls, \
		CZigBeeDeviceObject::wManufacturerID, \
		CZigBeeClusterLevelControl::attributeMinimumOnLevel, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::persistent, member)
