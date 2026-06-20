// Compact15dot4Ex.h : header file
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
// This file encapsulates support for various physical layer chips

/////////////////////////////////////////////////////////////////////////////
// Common PHY and MAC parts

#include "IEEE802154PHY.h"


/////////////////////////////////////////////////////////////////////////////
// Support for CC2420

#ifdef __COMPACT15DOT4_CC2420__
#include "RadioCC2420.h"
#include "PhysicalCC2420.h"
typedef CPhysicalCC2420 CPhysicalRadio;
#endif


/////////////////////////////////////////////////////////////////////////////
// Support for CC2520

#ifdef __COMPACT15DOT4_CC2520__
#include "RadioCC2520.h"
#include "PhysicalCC2520.h"
typedef CPhysicalCC2520 CPhysicalRadio;
#endif


/////////////////////////////////////////////////////////////////////////////
// Support for GreenPeak transceivers (generic GreenPeak MAC API)

#ifdef __COMPACT15DOT4_GREENPEAK__
#include "RadioGreenPeak.h"
#include "PhysicalGreenPeak.h"
typedef CPhysicalGreenPeak CPhysicalRadio;
#endif
