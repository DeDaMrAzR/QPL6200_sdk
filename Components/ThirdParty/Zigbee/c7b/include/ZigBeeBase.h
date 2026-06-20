// ZigBeeBase.h : header file
//
// Copyright(C) 2008-2021 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This implementation is based on the ZigBee Alliance's ZigBee Specification
// as of January 17, 2007
//
// This header allows configuration of the entire ZigBee stack in a single
// place

/////////////////////////////////////////////////////////////////////////////
// ZigBee core stack revision

#define __C7B_CORE_STACK_REVISION 23


/////////////////////////////////////////////////////////////////////////////
// Start of namespace c7b

namespace c7b
{

/////////////////////////////////////////////////////////////////////////////
// ZigBee stack version

// This is the ZigBee protocol version, or more precisely the stack type;
// The only currently supported value is 2 = ZigBee PRO
extern const unsigned short nStackVersion;

// This is the ZigBee core stack revision, i.e. the revision of the published
// specification document, e.g. 21 for ZigBee Stack R21 ("ZigBee 3.0")
extern const unsigned char nCoreStackRelease;


/////////////////////////////////////////////////////////////////////////////
// __C7BFB_BASEEX, extended factory block default location

#ifndef __C7BFB_BASEEX
#define __C7BFB_BASEEX (__C7BFB_BASE - 0x0100)
#endif


/////////////////////////////////////////////////////////////////////////////
// __C7B_FACTORYBLOCK_ELEMENT_EX() - convenient factory block elements

#ifdef __IAR_SYSTEMS_ICC__
#define __C7B_FACTORYBLOCK_ELEMENTEX(base, offset) \
	_Pragma("language=save") \
	_Pragma("language=extended") \
	_Pragma("diag_suppress=Pe1665") \
	_Pragma(__COMPACT_LITERALIZE(location = base + ## offset)) \
	__no_init __root const \
	_Pragma("diag_default=Pe1665") \
	_Pragma("language=restore")
#else
#define __C7B_FACTORYBLOCK_ELEMENTEX(base, offset) \
	/* __attribute__((address((base + offset)))) */ \
	const
#endif

#ifdef __IAR_SYSTEMS_ICC__
#define __C7B_FACTORYBLOCK_ELEMENT_STATICEX(base, offset) \
	_Pragma("language=save") \
	_Pragma("language=extended") \
	_Pragma("diag_suppress=Pe1665") \
	_Pragma(__COMPACT_LITERALIZE(location = base + ## offset)) \
	__no_init __root static const \
	_Pragma("diag_default=Pe1665") \
	_Pragma("language=restore")
#else
#define __C7B_FACTORYBLOCK_ELEMENT_STATICEX(base, offset) \
	/* __attribute__((address((base + offset)))) */ \
	static const
#endif


/////////////////////////////////////////////////////////////////////////////
// __C7B_FACTORYBLOCK_ELEMENT() - convenient factory block elements

#define __C7B_FACTORYBLOCK_ELEMENT(offset) \
	__C7B_FACTORYBLOCK_ELEMENTEX(__C7BFB_BASE, offset)

#define __C7B_FACTORYBLOCK_ELEMENT_STATIC(offset) \
	__C7B_FACTORYBLOCK_ELEMENT_STATICEX(__C7BFB_BASE, offset)

#define __C7B_FACTORYBLOCKEX_ELEMENT(offset) \
	__C7B_FACTORYBLOCK_ELEMENTEX(__C7BFB_BASEEX, offset)

#define __C7B_FACTORYBLOCKEX_ELEMENT_STATIC(offset) \
	__C7B_FACTORYBLOCK_ELEMENT_STATICEX(__C7BFB_BASEEX, offset)


/////////////////////////////////////////////////////////////////////////////
// ZigBee stack trace masks

#ifdef _DEBUG

enum { nwk = 0x00800000, nlme = 0x00400000, aps = 0x00200000,
	apsme = 0x00100000, app = 0x00080000, zdo = 0x00040000,
	zcl = 0x00020000 };

#endif


/////////////////////////////////////////////////////////////////////////////
// ZigBee stack status code lookup

#ifdef _DEBUG

const char *GetStatusString(const unsigned char nStatus);

#endif


/////////////////////////////////////////////////////////////////////////////
// End of namespace c7b

}
