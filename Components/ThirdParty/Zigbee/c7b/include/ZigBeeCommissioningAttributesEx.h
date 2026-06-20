// ZigBeeCommissioningAttributesEx.h : header file
//
// Copyright(C) 2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// Commissioning cluster attributes, extensions providing initialization etc.


/////////////////////////////////////////////////////////////////////////////
// Forward declarations

class CZigBeeApplicationSupport;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeCommissioningStorageItem

#pragma pack(push, 1)

class CZigBeeCommissioningStorageItem : public CZigBeeStorageItem
{
	// typedefs & enums
	public:
		// ZigBee joint commissioning attribute set
		enum { domain = 0x40 };

		// Supported items
		enum { itemCommissioningAttributes };

	// Construction
	public:
		CZigBeeCommissioningStorageItem
			(const uint_fast8_t nItemID = itemCommissioningAttributes);

	// Attributes
	public:
		// Item identifier for network storage items
		uint8_t m_nItemID;
};

#pragma pack(pop)

inline CZigBeeCommissioningStorageItem::CZigBeeCommissioningStorageItem
	(const uint_fast8_t nItemID) :
CZigBeeStorageItem(domain), m_nItemID(nItemID)
{
}


/////////////////////////////////////////////////////////////////////////////
// CZigBeeCommissioningAttributesEx

class CZigBeeCommissioningAttributesEx : public CZigBeeCommissioningAttributes
{
	// Operations
	public:
		// Sets default values of commissioning attributes based on current
		// operational settings
		void SetCurrentValues(const CZigBeeApplicationSupport &aps,
			const unsigned char bStartupControl = startupCommissioned,
			const bool bPartial = false);

		// Loads the attribute values in the commissioning start-up attribute
		// set. The network and application support layer attributes remain
		// unchanged
		bool Load();

		// Loads the attribute values in the commissioning start-up attribute
		// set and sets the according network and application support layer
		// attributes appropriately
		bool Load(CZigBeeApplicationSupport &aps);

		// Stores the current attribute values in the commissioning start-up
		// attribute set. The network and application support layer attributes
		// remain unchanged, so the network is still operational until Commit()
		// is called
		void Store();

		// Must be called after initial network start-up. Takes care of
		// switching the device to "commissioned" once network formation or
		// joining was successful
		static void AfterInitialStartupComplete
			(const CZigBeeApplicationSupport &aps, const unsigned char nStatus);

		// Must be called after each network start-up. Takes care of setting up
		// certain stack features once the stack is running
		static void AfterStartupComplete(const CZigBeeApplicationSupport &aps,
			const unsigned char nStatus);

		// Must be called after the network has been left. Takes care of
		// switching the device to "scratch from join" once the device has
		// left the network
		static void AfterLeave(const CZigBeeApplicationSupport &aps,
			const unsigned char nStatus);

		// Commits, i.e. applies, the current commissioning attributes by
		// storing them persistently and updating the appropriate network
		// and application support layer attributes
		void Commit(CZigBeeApplicationSupport &aps);

	// Implementation
	public:

#ifdef _DEBUG
		void Dump() const;
#endif

};
