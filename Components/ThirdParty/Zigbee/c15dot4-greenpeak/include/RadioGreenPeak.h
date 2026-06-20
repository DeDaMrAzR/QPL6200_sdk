// RadioGreenPeak.h : header file
//
// Copyright(c) 2008-2017 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Provides a low level interface to the Qorvo GP712 radio

#ifdef __COMPACT15DOT4_GP712__

/////////////////////////////////////////////////////////////////////////////
// CRadioGreenPeak

class CRadioGreenPeak
{
	// typedefs & enums
	public:

	// Construction
	public:
		CRadioGreenPeak();

	// Attributes
	public:
/*
		// According to section 23 of the radio data sheet, the RSSI_OFFSET
		// must be found empirically during system development. An approximate
		// value of -45 is also specified. Determination of actual value
		// remains TBD.
		static const short nRSSIOffset;
*/

	// Operations
	public:
		// Configures SAM7S peripherals to interface with x. The first
		// stage does a minimal setup to configure the output clock divider
		// such that the micro-controller can run off the x output clock
		// signal
		static void ConfigureStage1(const unsigned int nTicks = 64);

		// The second stage prepares everything for stack start-up and ensures
		// it does impact the already running and configured ouput clock
		static void ConfigureStage2();

		static void SsnHigh();
		static void WriteRstn(const bool value);
		static void WriteWkup(const bool value);

		// Configures SAM7S peripherals to interface with radio
		void Configure();

		// Enables or disables the radio (voltage regulator)
		static void Enable(const bool bEnable = true);

		// Enters low power mode by turning the radio on-chip oscillator off
		static void EnterLowPowerMode();

		// Leaves low power mode by turning the radio on-chip oscillator on.
		// Does not return to the calling thread until the oscillator has
		// become stable
		bool LeaveLowPowerMode();

		// Performs a hardware reset (through the reset pin)
		static void EnableReset(const bool bEnable = true);

		// Sets the 64-bit IEEE extended address; used for address recognition
		static void SetExtendedAddress(const unsigned long long qwAddress);

		// Sets the 16-bit short address; used for address recognition
		static void SetShortAddress(const unsigned short wAddress);

		// Sets the 16-bit PAN identifier; used for address recognition
		static void SetNetworkIdentifier(const unsigned short wPANID);

		// Enables/disables radio hardware address recognition. If address
		// recognition is enabled, bCoordinator must be set to true for
		// coordinators, false otherwise. To accept reserved frame types, pass
		// bAcceptReserved as true
		static void EnableAddressRecognition(const bool bEnable = true,
			const bool bCoordinator = false,
			const bool bAcceptReserved = false);

		// Enables/disables radio automatic generation and transmission of
		// acknowledgment frames. Should be used together with address
		// recognition
		static void EnableAutoAcknowledge(const bool bEnable = true);

		// Sets the clear channel assessment threshold value (-32 is the reset
		// value of radio)
		static void SetCCAThreshold(const signed char nThreshold = -32);

		// Sets the FIFO threshold interrupt level, i.e. FIFOP_THR in IOCFG0
		static void SetFIFOThreshold(const unsigned char nThreshold);

		// Returns the current RSSI estimate (logarithmic scale), without
		// having applied any offset fix-up. The caller should check if the
		// "RSSI valid" status bit is set, indicating that the receiver has
		// been turned on long enough to produce a valid RSSI measurement
		static signed char GetRSSIValueEx();

		// Returns the current RSSI estimate (logarithmic scale), fixed-up by
		// nRSSIOffset
		static short GetRSSIValue();

		// Returns true, if the clear channel assessment indicates a clear
		// channel
		static bool IsChannelClear();

		// Sets the RF operating frequency according to the desired channel,
		// where channels are numbered according to IEEE 802.15.4. In the
		// 2.4 GHz band, channels 11-26 are specified, with a channel spacing
		// of 5 MHz
		static void SetFrequency(const unsigned char nChannel = 11);

		// Sets the RF operating frequency with 1 MHz resolution. Supported
		// range is from 2048 MHz to 3071 MHz, with reset value equalling
		// 2405 MHz
		static void SetFrequencyEx(const unsigned short nFrequency = 2405);

		// Returns the current frequency in MHz
		static unsigned short GetFrequencyEx();

		// Sets the PA_LEVEL, i.e. the output level of the power amplifier.
		// Refer to section 28 of the radio data sheet for details
		static void SetTransmitPower(const unsigned short nPowerSettings);

		// Enables the battery monitor included in radio. As soon as the
		// unregulated voltage drops below the specified voltage level, the
		// BATT_OK flag toggles.
		// Toggle voltage = 1.25V * (72 - nVoltageLevel) / 27. Use the
		// CALCULATE_VOLTAGE_LEVEL() macro to determine the value
		static void EnableBatteryMonitor(const bool bEnable = true,
			const unsigned char nVoltageLevel = 0);

		// Returns true, if battery voltage level is OK. A setup time of 5us
		// must be allowed after enabling the battery monitor, until the
		// BATT_OK flag is valid
		static bool IsBatteryOK();

		// Returns information about manufacturer, part number and revision.
		// Besides being useful for debugging purposes, this information can
		// be used to provide work-arounds for chip bugs, for example. Returns
		// the values of MANFIDL and MANFIDH registers
		static void GetDeviceInfo(unsigned short &nManufacturerID,
			unsigned short &nPartNumber, unsigned char &nVersion);

		// Gives the current state of the FIFO and frame control (FFCTRL)
		// finite state machine
		static unsigned char GetCurrentState();

		// Enables transmitter test mode. Generates an unmodulated carrier, if
		// bModulatedSpectrum is false, or a pseudo-random sequence using the
		// CRC generator otherwise
		void EnableTransmitterTestMode(const bool bEnable = true,
			const bool bModulatedSpectrum = true) const;

	// Implementation
	public:
		~CRadioGreenPeak();

		// Package pin connected to radio's reset signal
		static const CCompactIO reset;

		// Package pin connected to radio's int pin
		static const CCompactIO intoutn;

		// Package pin connected to radio's wake-the-radio-up
		static const CCompactIO wakeup;

		// SPI interface to radio (bus, chip and mode pre-configured)
		static const CCompactSPIDevice spi;

		// IRQ peripheral monitoring FIFOP pin activity
		static const CCompactIRQ irq;

		// Transmits a single byte to the radio and receives a response byte at
		// the same time
		static unsigned char Transceive(const unsigned char bData);

		// Interrupt service routine that handles the radio FIFOP interrupt
		// request
		static void OnDataAvailableInterrupt(void *pArgument);

		void OnDataAvailable();

		// Interrupt service routine that handles the timer's interrupt request
		static void OnTimerInterrupt(unsigned int nStatus, void *pInfo);

		// Detects an overflow by examining the FIFO and FIFOP signal states
		static bool DetectReceiveOverflow();

		// Handles an receiver overflow condition. Must be called when an RX
		// overflow has occurred. Notice that the chip remains in RX_OVERFLOW
		// state, until the overflow condition has been acknowledged and reset.
		// Account for this fact in any loop that waits for a status to avoid
		// dead-locks
		void HandleReceiveOverflow();

		// Handles an transmitter underflow condition. Must be called when n
		// TX underflow has occurred
		static void HandleTransmitUnderflow();

#ifdef _DEBUG
	public:
		// Returns a human-readable string for the specified status bit
		static const char *GetChipStatusString(const unsigned char bIndex);

		// Returns a human-readable string identifying the current FSM state
		static const char *GetStateString(const unsigned char bState);

		// Given a status byte, this method outputs a descriptive string on the
		// debug console
		static void DumpStatus(const unsigned char bStatus,
			const char *const pszMessage = 0);

		// Retrieves the current status and FSM state and dumps other
		// information, such as the last received packet
		void Dump() const;
#endif

};

#else

/////////////////////////////////////////////////////////////////////////////
// CRadioGreenPeak, when the ubisys stack does not implement the Qorvo HAL

class CRadioGreenPeak
{
};

#endif
