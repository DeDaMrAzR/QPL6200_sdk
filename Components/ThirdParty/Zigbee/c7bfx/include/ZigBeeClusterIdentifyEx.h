// ZigBeeClusterIdentifyEx.h : header file
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
// Provides extensions to the identify cluster for Lighting & Occupancy

/////////////////////////////////////////////////////////////////////////////
// forward references

class CZigBeeClusterIdentifyEx;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIdentifyEffect

class CZigBeeClusterIdentifyEffect : public CCompactTimerClient
{
	friend class CZigBeeClusterIdentifyEx;

	// typedefs & enums
	public:
		// Context the effect is run from. Currently either the on/off cluster
		// "Off with effect" command, or the identify cluster "Trigger effect"
		// command
		enum { contextIdentify, contextOnOff };

		// Type of the effect
		enum { typeExFinish = 254, typeExStop = 255 };

		// Option mask: field masks
		enum { optionContextMask = 0x00ff0000, optionTypeMask = 0x0000ff00,
			optionVariantMask = 0x000000ff };

		// Called when the effect has finished
		typedef void (*ONCOMPLETE)(CZigBeeClusterIdentifyEffect &effect);

	// Construction
	public:
		CZigBeeClusterIdentifyEffect(CZigBeeClusterIdentifyEx &cluster,
			const unsigned int nOptions = 0,
			const ONCOMPLETE pfnComplete = 0,
			void *const pArgument = 0);

	// Attributes
	public:
		CZigBeeClusterIdentifyEx &m_cluster;

		// Returns the effect context
		unsigned char GetContext() const;

		// Returns the effect type
		unsigned char GetType() const;

		// Returns the effect variant
		unsigned char GetVariant() const;

	// Operations
	public:
		// Creates an effect option description
		static unsigned int MakeOptions(const unsigned char nOptions,
			const unsigned char nType, const unsigned char nVariant = 0);

		// Returns the context of an effect, given its option mask
		static unsigned char GetContext(const unsigned int nOptions);

		// Returns the type of an effect, given its option mask
		static unsigned char GetType(const unsigned int nOptions);

		// Returns the variant of an effect, given its option mask
		static unsigned char GetVariant(const unsigned int nOptions);

	// Overridables
	protected:
		// Restore the output to its expected state, e.g. sets the brightness
		// of a light to the level as defined by the values of the relevant
		// on/off and level control cluster attributes
		virtual void Revert() = 0;

		// Advances to the next state. If there are more states for this effect
		// then return true, else return false to finish the effect
		virtual bool AdvanceState() = 0;

	// Implementation
	public:
		virtual ~CZigBeeClusterIdentifyEffect();

		// Completion handler for the effect
		const ONCOMPLETE m_pfnComplete;

		// Completion handler argument for the effect
		void *const m_pArgument;

		// Options include the effect type, effect variant and effect context,
		// e.g. whether the effect is triggered by an "Off with effect" command
		// of the on/off cluster, or a "Trigger effect" command of the identify
		// cluster
		unsigned int m_nOptions;

		// Timer instance for the effect
		CCompactTimer *m_pTimer;

		// Possible effect states; add more in specialized effect classes
		enum { stateIdle };

		// True, if the current effect should finish gracefully
		bool m_bFinish;

		// The timer for the current effect state has expired and the effect
		// state machine needs to progress to its next state
		void OnTimerExpired(void *pArgument);
};

inline unsigned int CZigBeeClusterIdentifyEffect::MakeOptions
	(const unsigned char nContext, const unsigned char nType,
	const unsigned char nVariant)
{
	return static_cast<unsigned int>(nContext << 16) |
		static_cast<unsigned int>(nType << 8) | nVariant;
}

inline unsigned char CZigBeeClusterIdentifyEffect::GetContext
	(const unsigned int nOptions)
{
	return nOptions >> 16;
}

inline unsigned char CZigBeeClusterIdentifyEffect::GetType
	(const unsigned int nOptions)
{
	return nOptions >> 8;
}

inline unsigned char CZigBeeClusterIdentifyEffect::GetVariant
	(const unsigned int nOptions)
{
	return static_cast<unsigned char>(nOptions);
}

inline unsigned char CZigBeeClusterIdentifyEffect::GetContext() const
	{ return GetContext(m_nOptions); }

inline unsigned char CZigBeeClusterIdentifyEffect::GetType() const
	{ return GetType(m_nOptions); }

inline unsigned char CZigBeeClusterIdentifyEffect::GetVariant() const
	{ return GetVariant(m_nOptions); }


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIdentifyEx

class CZigBeeClusterIdentifyEx : public CZigBeeClusterIdentify
{
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster command identifiers (client to server)
		enum { commandTriggerEffect = 0x40 };

	// Construction
	public:
		CZigBeeClusterIdentifyEx(CZigBeeFoundationApplication &application);

	// Attributes
	public:
		// Effect currently being executed (if any)
		CZigBeeClusterIdentifyEffect *m_pEffect;

	// Operations
	public:
		// Starts executing an effect
		void ExecuteEffect(unsigned int nOptions,
			const CZigBeeClusterIdentifyEffect::ONCOMPLETE pfnComplete = 0,
			void *const pArgument = 0);

	// Overrides
	public:
		// Handles trigger effect command
		void OnTriggerEffect(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

	// Overridables
	public:
		// Creates a CZigBeeClusterIdentifyEffect object for the specific
		// options
		virtual CZigBeeClusterIdentifyEffect *
		CreateEffect(unsigned int nOptions,
			const CZigBeeClusterIdentifyEffect::ONCOMPLETE pfnComplete = 0,
			void *const pArgument = 0);
};
