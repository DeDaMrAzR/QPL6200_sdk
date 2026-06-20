// Compact15dot4.h : header file
//
// Copyright(C) 2008-2017 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact15.4(TM) IEEE 802.15.4 compliant MAC and PHY layer HAL
// class library.
//
// This file is specific to the Compact15.4 Adapter for the Qorvo MAC, for
// example the GP712 and GP570/GP695

#define __COMPACT15DOT4_GREENPEAK__
#define __COMPACT15DOT4_HARDWARE_FCS__
#define __COMPACT15DOT4_HARDWARE_ADDRESSING__
#define __COMPACT15DOT4_HARDWARE_RANDOM__
#define __COMPACT15DOT4_FFD__

/////////////////////////////////////////////////////////////////////////////
// 802.15.4 stack trace masks

#ifdef _DEBUG

enum { c15dot4mac = 0x00008000, c15dot4mlme = 0x00004000,
	c15dot4phy = 0x00002000 };

#endif
