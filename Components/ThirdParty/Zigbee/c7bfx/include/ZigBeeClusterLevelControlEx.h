// ZigBeeClusterLevelControlEx.h : header file
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
// Provides extensions to the level control cluster for Lighting & Occupancy

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterLevelControlEx

class CZigBeeClusterLevelControlEx : public CZigBeeClusterLevelControl
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()

	// typedefs & enums
	public:
		// Additional attribute identifiers of the level control server cluster
		enum { attributeStartupLevel = 0x4000 };

	// Construction
	public:
		CZigBeeClusterLevelControlEx(CZigBeeFoundationApplication &application,
			const unsigned short nOnOffTransistionTime = 0,
			const unsigned char nOnLevel = 0xfe,
			const unsigned char bOptions = optionExecuteIfOff,
			const unsigned char nMinOnLevel = 0xff);

	// Attributes
	public:
		// Start-up level of this device
		unsigned char m_nStartupLevel;

	// Overrides
	public:
		// This method is called when an attribute's value has changed. Calls
		// the applications notification handlers and manages attribute
		// reporting, if applicable
		virtual void OnAttributeChanged(const unsigned short wAttributeID,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0);

		// Called after power-up. Evaluates the ballast configuration settings
		// to set current level and stored level appropriately
		virtual void AfterPowerup();

		// Called when the current level has settled, i.e. the value has been
		// changed and the transition has completed. This is used here to
		// store the current level persistently
		virtual void OnCurrentLevelSettled(const bool bStored = false) const;

		// Request to stop the current ongoing level transition. This might
		// be necessary when the level transition is correlated with other
		// cluster's functionality, e.g. coupling color control cluster's
		// color temperature to the current level. When color temperature
		// change is actively instigated from the color control cluster,
		// current ongoing level transition needs to be stopped at first.
		virtual void RequestStop();

	// Implementation
	public:
		// Indicate whether a color control cluster on the same endpoint
		// should be notified of local level change. This is normally
		// necessary when color temperature should be coupled to level or
		// color composition requires brightness information.
		// Note: When this is set to true, the companion color control
		// cluster on the same endpoint MUST be an instance of
		// CZigBeeClusterColorControl or a class derived from
		// CZigBeeClusterColorControl.
		bool m_bNotifyCompanionColorControl;

		// Stores the current level persistently
		void StoreCurrentLevel(const unsigned char nValue) const;

		// Loads the current level from persistent storage
		bool LoadCurrentLevel();
};

#define __COMPACT7B_LEVELCONTROLEX_CURRENTLEVEL(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterLevelControl::attributeCurrentLevel, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable | \
			CZigBeeAttribute::storageVolatile4x1, member)

#define __COMPACT7B_LEVELCONTROLEX_STARTUPLEVEL(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterLevelControlEx::attributeStartupLevel, \
		CZigBeeAttribute::typeUnsigned8, 0, \
		CZigBeeAttribute::persistent, member)
