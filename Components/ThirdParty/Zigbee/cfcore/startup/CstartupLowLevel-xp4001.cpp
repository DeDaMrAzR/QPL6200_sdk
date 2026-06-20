// CstartupLowLevel.cpp : implementation file
//
// Copyright(C) 2006-2015 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Low level hardware initialization. Called by startup code. Performs early
// initialization of peripherals, i.e. setup of oscillators, PLL and clock
// generator. Also disables the watch-dog and provides initial vectors for
// the interrupt controller
//
// Be careful when calling C runtime library functions, since the library has
// not been initialized when entering InitializeLowLevel()

#include "CompactFramework.h"

/////////////////////////////////////////////////////////////////////////////
// Application defined function that sets up the main clock frequency and is
// also responsible for selecting an appropriate number of flash wait states,
// etc.

void SetupMainClock();


/////////////////////////////////////////////////////////////////////////////
// __low_level_init()
//
// Overrides the default IAR __low_level_init() function. Note that this
// function is called before the data segments are initialized, meaning that
// this function cannot rely on the values of global or static variables,
// except for those that are stored in ROM.
// When this function returns zero, the startup code will inhibit the
// initialization of data segments

extern "C" int __cfcore_low_level_init()
{
	// This functions is called by the start-up code (assembler) before
	// initialization of the C library. A temporary stack has been created
	// at the end of internal RAM

	// This is very early board initialization, which is meant to put certain
	// exernal peripherals into a state, which allows the start-up process to
	// continue (e.g. to make sure an external clock source is running at a
	// suitable frequency)
	CBoard::Preinitialize();

	// Call application defined function that sets up the main clock frequency
	// and is also responsible for selecting an appropriate number of flash
	// wait states, etc.
	SetupMainClock();

#ifdef _DEBUG
	// In debug builds, configure debug unit, enabling early trace messages
	CDebugUnit::Configure();
#endif

	// Configures interrupts to pre-defined extended service routines or
	// default interrupt handlers
	CCompactIRQ::ConfigureAll();

	// Return true, i.e. do perform data segment initialization hereafter
	return true;
}
