// ZigBeeClusterIdentify.h : header file
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
// Provides the identify cluster

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterIdentify

class CZigBeeClusterIdentify : public CZigBeeCluster
{
	__COMPACT7B_DECLARE_ATTRIBUTE_MAP()
	__COMPACT7B_DECLARE_COMMAND_MAP()

	// typedefs & enums
	public:
		// Cluster identifier of the identify cluster
		enum { id = 0x0003 };

		// Cluster revision of this implementation
		enum { revision = 2 };

		// Cluster command identifiers (client to server)
		enum { commandIdentify, commandQueryIdentify,
			commandInvokeCommissioning, commandUpdateCommissioningState };

		// Cluster command identifiers (server to client)
		enum { commandQueryIdentifyResponse };

		// Attribute identifiers of the identify cluster
		enum { attributeIdentifyTime, attributeCommissioningState };

	// Construction
	public:
		CZigBeeClusterIdentify(CZigBeeFoundationApplication &application);

	// Operations
	public:
		// Lets the specified application endpoint identify itself using the
		// identify cluster on that endpoint (if any)
		static void LetIdentify(CZigBeeApplicationSupportBase &aps,
			const uint_fast8_t nEndpoint, const uint16_t nDuration = 3);

	// Overrides
	public:
		// Handles identify command
		void OnIdentify(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// Handles query identify command
		void OnQueryIdentify(const CZigBeeAddress &destination,
			const CZigBeeAddress &source,
			const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
			const unsigned char nLinkQuality, const unsigned int nTimestamp);

		// This method is called when an attribute's value has changed. Calls
		// the applications notification handlers and manages attribute
		// reporting, if applicable. In addition creates a count-down timer,
		// if required
		virtual void OnAttributeChanged(const unsigned short wAttributeID,
			const unsigned char nOptions = 0,
			const unsigned short wManufacturerCode = 0);

	// Implementation
	public:
		// Revision of the Identify cluster
		static const unsigned short nClusterRevision;

		// Timer that is non-zero during identify. Decrements the identify
		// time once every second
		CCompactTimer *m_pTimer;

		// IdentifyTime attribute. Specifies the remaining length of time,
		// in seconds, that the device will continue to identify itself.
		// If this attribute is set to a value other than 0x0000 then the
		// device shall enter its identification procedure
		unsigned short m_wIdentifyTime;

		// Responsive mode lock cookie assigned to the identify cluster
		unsigned int m_nCookie;

		// Called once a second to decrement the identify timer
		void OnTimerExpired(void *pArgument);

		// Enter identify mode for the specified duration. Also enter
		// into responsive mode if required.
		void StartIdentify(const uint16_t nDuration,
			const bool bEnterResponsiveMode = false);
};

#define __COMPACT7B_IDENTIFY_IDENTIFYTIME(cls, member) \
	__COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
		CZigBeeClusterIdentify::attributeIdentifyTime, \
		CZigBeeAttribute::typeUnsigned16, 0, \
		0, member)
