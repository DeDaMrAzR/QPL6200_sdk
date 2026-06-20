// ZigBeeClusterWindowCovering.h : header file
//
// Copyright(C) 2009-2018 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Part of the ubisys ZigBee Cluster Library.
// Provides the window covering cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterWindowCovering

class CZigBeeClusterWindowCovering : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of window covering cluster
		enum { id = 0x0102 };

		// Cluster revision of this implementation
		enum { revision = 3 };

		// Identifiers of the window covering information attribute set
		enum { attributeType, attributePhysicalClosedLimitLift,
			attributePhysicalClosedLimitTilt, attributeCurrentPositionLift,
			attributeCurrentPositionTilt, attributeActuationsLift,
			attributeActuationsTilt, attributeStatus,
			attributeCurrentPositionLiftPercentage,
			attributeCurrentPositionTiltPercentage,
			attributeOperationalStatus };

		// Identifiers of the window covering settings attribute set
		enum { attributeInstalledOpenLimitLift = 0x0010,
			attributeInstalledClosedLimitLift, attributeInstalledOpenLimitTilt,
			attributeInstalledClosedLimitTilt, attributeVelocity,
			attributeAccelerationTime, attributeDecelerationTime,
			attributeMode, attributeSetpointsLift, attributeSetpointsTilt };

		// Command identifiers
		enum { commandUpOpen, commandDownClose, commandStop,
			commandGotoLiftValue = 4, commandGotoLiftPercentage,
			commandGotoTiltValue = 7, commandGotoTiltPercentage };

		// Window covering types
		enum { typeRollershade, typeRollershade2Motor, typeRollershadeExterior,
			typeRollershadeExterior2Motor, typeDrapery, typeAwning, typeShutter,
			typeTiltBlindTiltOnly, typeTiltBlindLiftAndTilt,
			typeProjectorScreen };

		// Config/status bits
		enum { statusOperational = 0x01, statusOnline = 0x02,
			statusReversal = 0x04, statusLiftClosedLoop = 0x08,
			statusTiltClosedLoop = 0x10, statusLiftHasEncoder = 0x20,
			statusTiltHasEncoder = 0x40 };

		// Operational status bits
		enum { statusMovingUp = 0x01, statusMovingDown = 0x02 };

		// Mode bits
		enum { modeReversed = 0x01, modeCalibration = 0x02,
			modeMaintenance = 0x04, modeOpticalFeedback = 0x08 };

	// Construction
	public:
		CZigBeeClusterWindowCovering(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// WindowCoveringType. Identifies the type of window covering being
		// controlled. One of the typeXyz constants above
		unsigned char m_nType;

		// Config/Status. Makes configuration and status information available.
		// To change settings, devices shall write to the Mode attribute of the
		// window covering settings attribute set
		unsigned char m_nStatus;

		// PhysicalClosedLimit - Lift. Identifies the maximum possible encoder
		// position possible (in centimeters) to position the height of the
		// window covering - this is ignored if the device is running in Open
		// Loop Control
		unsigned short m_nPhysicalClosedLimitLift;

		// PhysicalClosedLimit - Tilt. Identifies the maximum possible encoder
		// position possible (tenth of a degrees) to position the angle of the
		// window covering - this is ignored if the device is running in Open
		// Loop Control
		unsigned short m_nPhysicalClosedLimitTilt;

		// CurrentPosition - Lift. Identifies the actual position (in
		// centimeters) of the window covering from the top of the shade if
		// Closed Loop Control is enabled. This attribute is ignored if the
		// device is running in Open Loop Control
		unsigned short m_nCurrentPositionLift;

		// Current Position - Tilt. Identifies the actual tilt position (in
		// tenth of a degree) of the window covering from Open if Closed Loop
		// Control is enabled. This attribute is ignored if the device is
		// running in Open Loop Control
		unsigned short m_nCurrentPositionTilt;

		// Number of Actuations - Lift. Identifies the total number of lift
		// actuations applied to the Window Covering since the device was
		// installed
		unsigned short m_nActuationsLift;

		// Number of Actuations - Tilt. Identifies the total number of tilt
		// actuations applied to the Window Covering since the device was
		// installed
		unsigned short m_nActuationsTilt;

		// Current Position Lift Percentage. Identifies the actual position as
		// a percentage between the PhysicalOpenLimitLift Attribute and the
		// PhysicalClosedLimitLift attribute of the window covering from the
		// top of the shade if Closed Loop Control is enabled
		unsigned char m_nCurrentPositionLiftPercentage;

		// Current Position Tilt Percentage. Identifies the actual position as
		// a percentage between the PhysicalOpenLimitTilt attribute and the
		// PhysicalClosedLimitTilt attribute of the window covering from the
		// top of the shade if Closed Loop Control is enabled
		unsigned char m_nCurrentPositionTiltPercentage;

		// Current operational status. Moving up, moving down or no motion
		unsigned char m_nOperationalStatus;

		// InstalledOpenLimit - Lift. Identifies the Open Limit for Lifting the
		// Window Covering whether position (in centimeters) is encoded or
		// timed. This attribute is ignored if the device is running in Open
		// Loop Control
		unsigned short m_nInstalledOpenLimitLift;

		// InstalledClosedLimit - Lift. Identifies the Closed Limit for Lifting
		// the Window Covering whether position (in centimeters) is encoded or
		// timed. This attribute is ignored if the device is running in Open
		// Loop Control
		unsigned short m_nInstalledClosedLimitLift;

		// InstalledOpenLimit - Tilt. Identifies the Open Limit for Tilting the
		// Window Covering whether position (in tenth of a degree) is encoded
		// or timed. This attribute is ignored if the device is running in Open
		// Loop Control
		unsigned short m_nInstalledOpenLimitTilt;

		// InstalledClosedLimit - Tilt. Identifies the Open Limit for Tilting
		// the Window Covering whether position (in tenth of a degree) is
		// encoded or timed. This attribute is ignored if the device is running
		// in Open Loop Control
		unsigned short m_nInstalledClosedLimitTilt;

		// Velocity - Lift. Identifies the velocity (in centimeters per second)
		// associated with Lifting the Window Covering
		unsigned short m_nVelocity;

		// Mode. Allows configuration of the Window Covering, such as reversing
		// the motor direction, placing the Window Covering into calibration
		// mode, placing the motor into maintenance mode, disabling the ZigBee
		// network, and disabling status LEDs
		unsigned char m_nMode;

/*
		// Intermediate Setpoints - Lift. Identifies the number of Intermediate
		// Setpoints supported by the Window Covering for Lift and then
		// identifies the position settings for those Intermediate Setpoints if
		// Closed Loop Control is supported
		void *m_pSetpointsLift;

		// Intermediate Setpoints - Tilt. Identifies the number of Intermediate
		// Setpoints supported by the Window Covering for Tilt and then
		// identifies the position settings for those Intermediate Setpoints if
		// Closed Loop Control is supported
		void *m_pSetpointsTilt;
*/

		// Returns true, if the device supports "lift" commands/attributes
		bool SupportsLift() const;

		// Returns true, if the device supports "tilt" commands/attributes
		bool SupportsTilt() const;

	// Operations
	public:
		// Creates a stop command (client side)
		static void CreateStopCommand(CPacket &asdu);

		// Assembles and transmits a stop command (client to server)
		static void RequestStop(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius);

		// Creates an up/open command (client side)
		static void CreateUpOpenCommand(CPacket &asdu);

		// Assembles and transmits an up/open command (client to server)
		static void RequestUpOpen(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius);

		// Creates a down/close command (client side)
		static void CreateDownCloseCommand(CPacket &asdu);

		// Assembles and transmits a down/close command (client to server)
		static void RequestDownClose(CZigBeeApplicationSupportBase &aps,
			const CZigBeeAddress &destination,
			const unsigned short wProfileID,
			const unsigned char bSourceEndpoint,
			const unsigned char nTransmitOptions, const unsigned char bRadius);

		// Called by the application framework when an "up/open" command has
		// been received
		void OnUpOpen(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called by the application framework when a "down/close" command has
		// been received
		void OnDownClose(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called by the application framework when a "stop" command has been
		// received
		void OnStop(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called by the application framework when a move to lift value
		// command has been received
		void OnGotoLiftValue(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called by the application framework when a move to lift percentage
		// command has been received
		void OnGotoLiftPercentage(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called by the application framework when a move to tilt value
		// command has been received
		void OnGotoTiltValue(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Called by the application framework when a move to tilt percentage
		// command has been received
		void OnGotoTiltPercentage(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Sets the operational status in a way that makes sure that a report
		// is sent to bound targets, if required. The new operational status
		// is obtained by first clearing the specified bits in nClear and then
		// setting the bits in nSet. All other bits remain unmodified
		void SetOperationalStatus(const unsigned char nSet = 0,
			const unsigned char nClear = statusMovingUp | statusMovingDown);

		// To be invoked by derived classes when a command finished execution
		void OnCommandExecutionCompleted();

	// Overrides
	public:
		// Stores relevant attributes as a scene extension field set in the
		// supplied packet object
		virtual bool StoreSceneAttributes(CPacket &scene) const;

		// Applies a pre-stored extension field set to the appropriate
		// attributes. Transition time is specified in tenths of a second
		virtual bool LoadSceneAttributes(const CPacket &scene,
			const unsigned int nTransitionTime);

	// Overridables
	public:
		// Moves up or down
		virtual void Move(const bool bUp) = 0;

		// Stops movement
		virtual void Stop() = 0;

		// Moves to a particular lift/tilt position. If the value is void (-1),
		// the current lift/tilt value is not modified, such that it is
		// possible to go to a certain lift value and/or tilt value
		virtual void Goto(const unsigned short nLiftPosition,
			const unsigned short nTiltPosition) = 0;

	// Implementation
	protected:
		// Revision of the Window Covering cluster
		static const unsigned short nClusterRevision;

		// Saved tilt position at the time a move-to-lift command was received.
		// This is needed to properly restore the tilt value on subsequent
		// move-to-lift value.  Only valid when a move-to-lift command is executing
		unsigned short m_nSavedPositionTilt;

		// Indicates if a Go To Lift command is currently executing
		bool m_bGotoLiftActive;

		// Used by the command handlers for absolute (centimeters) and relative
		// (percentage) lift positioning
		void OnGotoLiftValue(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned short nLiftValue);

		// Used by the command handlers for absolute (degrees) and relative
		// (percentage) tilt positioning
		void OnGotoTiltValue(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header,
			const unsigned short nTiltValue);

		// Converts lift percentage to an absolute lift value
		unsigned short ToLiftValue(const unsigned char nPercentage) const;

		// Converts tilt percentage to an absolute tilt value
		unsigned short ToTiltValue(const unsigned char nPercentage) const;
};

#define __COMPACT7B_WINDOWCOVERING_TYPE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeType, \
		CZigBeeAttribute::typeEnumeration8, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_STATUS(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeStatus, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_PHYSICALCLOSEDLIMITLIFT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributePhysicalClosedLimitLift, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_PHYSICALCLOSEDLIMITTILT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributePhysicalClosedLimitTilt, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_CURRENTPOSITIONLIFT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeCurrentPositionLift, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, member)

#define __COMPACT7B_WINDOWCOVERING_CURRENTPOSITIONTILT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeCurrentPositionTilt, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, member)

#define __COMPACT7B_WINDOWCOVERING_ACTUATIONSLIFT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeActuationsLift, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_ACTUATIONSTILT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeActuationsTilt, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_CURRENTPOSITIONLIFTPERCENTAGE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeCurrentPositionLiftPercentage, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, member)

#define __COMPACT7B_WINDOWCOVERING_CURRENTPOSITIONTILTPERCENTAGE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeCurrentPositionTiltPercentage, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, member)

#define __COMPACT7B_WINDOWCOVERING_OPERATIONALSTATUS(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeOperationalStatus, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, member)

#define __COMPACT7B_WINDOWCOVERING_INSTALLEDOPENLIMITLIFT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeInstalledOpenLimitLift, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_INSTALLEDCLOSEDLIMITLIFT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeInstalledClosedLimitLift, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_INSTALLEDOPENLIMITTILT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeInstalledOpenLimitTilt, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_INSTALLEDCLOSEDLIMITTILT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeInstalledClosedLimitTilt, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_VELOCITY(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeVelocity, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		CZigBeeAttribute::accessReadOnly, member)

#define __COMPACT7B_WINDOWCOVERING_MODE(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeMode, \
		CZigBeeAttribute::typeBitmap8, 0, \
		CZigBeeAttribute::persistent, member)

/*
#define __COMPACT7B_WINDOWCOVERING_SETPOINTSLIFT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeSetpointsLift, \
		CZigBeeAttribute::typeRawData, 0, \
		0, member)

#define __COMPACT7B_WINDOWCOVERING_SETPOINTSTILT(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterWindowCovering::attributeSetpointsTilt, \
		CZigBeeAttribute::typeRawData, 0, \
		0, member)
*/
