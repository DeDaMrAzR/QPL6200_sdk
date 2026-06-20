// Board.h : header file
//
// Copyright(C) 2007-2022 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Board-specific settings

/////////////////////////////////////////////////////////////////////////////
// CBoard

class CBoard
{
	// typedefs & enums
	public:
		// Master clock frequency in Hz
		static const unsigned int nMasterClockFrequency;

	// Operations
	public:
		// Low level initialization of external clock sources, if applicable.
		// This method is called at a very early stage (before switching to the
		// external clock) and can be used to condition the external clock such
		// that it aligns with e.g. minimum and maximum frequency requirements
		// of on-chip PLL blocks. Notice this is even called before any static
		// object is initialized (including static C variables)
		static void Preinitialize();

		// Low level initialization for platforms with a dynamic loader and the
		// ability to pass command line arguments and environment strings. This
		// needs to be called from main(), passing the main arguments; on some
		// platforms that support such parameters before main(), it may also be
		// invoked from there, e.g. from an entry in the .preinit_array section
		// of a program built with GCC and glibc
		static void PreinitializeEx(int argc, char **argv, char **envp);

		// Second stage low-level initialization. Called after Preinitialize(),
		// after setting-up the clock source. Same restrictions apply as for
		// Preinitialize(). Implementations of Preinitialize2 shall return true
		// to perform data segment initialization, or false if that has already
		// occurred in Preinitialize2()
		static bool Preinitialize2();

		// Low level initialization of IOs, peripherals, etc. called from main,
		// i.e. after construction of global C++ objects. Might fail if for
		// example persistent storage failed to initialize, or a external device
		// that is vital to the device's operation failed to initialize. When
		// this method return false, the device should not start and instead
		// indicate a power-on self-test (POST) failure
		static bool Initialize();

		// Reboots the device
		static void Reboot();

		// Low-level deinitialization for releasing allocated resources,
		// e.g., release the persistent storage and its underlying memory
		// device, disable global interrupts etc.
		static void PostShutdown();

		// Returns true, if the device has rebooted due to a software-triggered
		// reset, false otherwise (watch-dog, power-cycle, etc.)
		static bool WasResetCausedBySoftware();

		// Runs the main application loop. This function must never return
		static void Run();

		// Provides a mechanism for modifying the value of otherwise constant
		// variables at runtime.
		// CAUTION: The use of this function shall be restricted to laboratory
		// environments only! It is not meant for normal in-field usage. For
		// example, when used on typical flash-based microcontrollers, this
		// has the effect of writing to internal flash memory without
		// particular precautions related to power failures during erase and
		// write operations. Platform-specific implementations must ensure
		// that writing to a flash page that contains the WriteReadOnlyValue()
		// implementation or any dependencies (e.g. memcpy) is either not
		// possible, or does not result in busfaults or other unexpected
		// behavior. The intended usage of this function is limited to test
		// setups, e.g. to configure specific Golden Unit or Device Under
		// Test behavior for certification compliance testing.
		static bool WriteReadOnlyValue(const void *pLocation,
			const void *pValue, size_t cbSize);
};
