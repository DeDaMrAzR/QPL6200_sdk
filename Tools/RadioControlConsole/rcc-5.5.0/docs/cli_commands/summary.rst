Commands - CLI
==============

Data Model
**********

.. The user interaction with RCC is based on `Attributes`_ and `Modes`_.
.. Attributes represent a device's configuration state with static values.
.. Modes also represent state, but are mutually-exclusive between them.

Commands are split up into 3 categories: general commands, attributes and modes.
General commands are commands providing the user extra information and features such as executing script containing a number of commands.
Attributes define the configuration of the radio.
Modes allow to change the radio state of the chip.
Only one mode can be selected at a time.

.. note::
    The set of supported attributes and modes is defined by the used PTC software.
    The radio console application dynamically loads the attributes and modes supported by the PTC software running on the DUT.
    The HELP command provides an overview of the supported attributes and modes of the used PTC component.

The Help command will show you what attributes belong to what modes. The order in which the attributes are
supplied to the modes are based on alphabetical order and is also shown in this overview.

The Help command will also show what the possible values a certain attribute can have.


General Commands
****************

.. csv-table:: General Commands
    :header: "Command", "Summary", "Comments"

    "INFO", "Specific information such as MAC address and version of the firmware on the chip", ""
    "I", "Print the current settings/state of the chip", ""
    "H/HELP", "Show help on all known commands", ""
    "RUN", "Execute a script that contains a sequence of commands", ""
    "Q/QUIT/EXIT", "Quit the application", ""


Attributes
**********

Attributes define the configuration of the radio.
These can be set before or during execution of modes, except the attributes to change the RX mode which can only be set while RX is off.
When there is no execution mode active, changing the attributes will have no effect.
If there is an execution mode active, changing these attributes will have a direct impact on the currently executing mode.

The possible and valid values for each attribute are described by the HELP command.

.. note::
    Some attributes can also be set when entering a mode command.
    This will have the same effect as setting the attributes separately.
    The HELP command gives an overview of what attributes are being used by a specific command.


The following table shows the known attributes (for all API versions) along with their short names.

.. csv-table:: Known attributes
    :header: "Short Name", "Description", "Reference"

    "AD", "Switches antenna diversity on or off", `Antenna Diversity`_
    "AN\ :sup:`[1]`", "Sets the antenna configuration", `Antenna`_
    "BLEDATARATE\ :sup:`[2]`", "Selects the PHY BLE mode for Rx and Tx", `BLE Data Rate`_
    "BLERXDATARATE\ :sup:`[2]`", "Selects the PHY BLE mode for Rx", `BLE Rx Data Rate`_
    "BLETXDATARATE\ :sup:`[2]`", "Selects the PHY BLE mode for Tx", `BLE Tx Data Rate`_
    "BLETESTPACKET\ :sup:`[2]`", "Defines the BLE TestPacket Type to be used in Direct Test Mode Transmit ", `BLE Test Packet Payload Type`_
    "CH", "Sets the main channel. An alias for *SRCID0* attribute", `Channel`_
    "CSMA\ :sup:`[1]`", "Sets the CSMA-CA mode", `CSMA Mode`_
    "CW", "Sets the type of the Continuous Wave Mode", `Continuous Wave Type`_
    "DCDC", "Sets the DCDC Mode", `DCDC Mode`_
    "MAPCLKIOPIN", "Sets the GPIO number to be used to map the clock", `Clock IO pin`_
    "MAPCLKTYPE", "Sets the clock type to be used to map the clock", `Clock type`_
    "MAXBE\ :sup:`[1]`", "Sets the maximum MAC Backoff exponent", `Maximum Backoff exponent`_
    "MC\ :sup:`[1]`", "Sets the maximum number of CSMA Backoffs", `Maximum Backoffs number`_
    "MIDLEVELAGCATT", "Switches the RX LNA AGC attenuation during timeout", `Attenuation during timeout`_
    "MINBE\ :sup:`[1]`", "Sets the minimum MAC Backoff exponent", `Minimum Backoff exponent`_
    "MR\ :sup:`[1]`", "Sets the maximum number of MAC retries", `Maximum MAC retries`_
    "PACKETCOUNT", "Sets the number of packets to transmit", `Packet count`_
    "PACKETINTERVAL\ :sup:`[1]`", "Sets the interval between packets, in ms", `Packet interval`_
    "PACKETLENGTH\ :sup:`[1]`", "Sets the packet length in bytes", `Packet length`_
    "PCHSTATCHANNEL\ :sup:`[1]`", "Sets channel for packet statistic", `Statistic channel`_
    "PDMCLKFREQ", "Sets the PDM clock output frequency", `PDM clock frequency`_
    "PDMCLKOUT", "Selects the PDM clock output pin", `PDM clock output pin`_
    "PDMCLKSRC", "Selects the clock source for the PDM", `PDM clock source`_
    "PHY", "Sets the PHY access mode", `PHY access mode`_
    "PI", "Sets Packet-In-Packet mode ON or OFF", `Packet-In-Packet`_
    "PM\ :sup:`[1]`", "Sets Promiscuous mode", `Promiscuous mode`_
    "RRADDRESS", "Sets the address of the register to read", `Register address`_
    "RXMS", "Sets the Multi Standard RX mode selector", `Multi Standard RX`_
    "RXHS\ :sup:`[1]`", "Sets the High Sensitivity RX mode selector", `High Sensitivity RX`_
    "RXMC\ :sup:`[1]`", "Sets the Multi Channel RX mode selector", `Multi Channel RX`_
    "SCANCOUNT\ :sup:`[1]`", "Sets the number of Energy Detect scans", `ED scan number`_
    "SCANINTERVAL\ :sup:`[1]`", "Sets the interval between Energy Detect scans, in ms", `ED scan interval`_
    "SETTXDATA\ :sup:`[1]`", "Sets the transmit packet payload", `TX packet payload`_
    "SPAN\ :sup:`[1]`", "Sets the Pan ID", `Pan ID`_
    "SRCID0", "Sets the main channel. Aliased by *CH* attribute", `Source ID 0`_
    "SRCID1\ :sup:`[1]`", "Sets the additional Rx channel with ID 1", `Source ID 1`_
    "SRCID2\ :sup:`[1]`", "Sets the additional Rx channel with ID 2", `Source ID 2`_
    "SSA\ :sup:`[1]`", "Sets the short MAC address", `Short MAC address`_
    "SLP", "Sets the sleep mode configuration", `Sleep mode configuration`_
    "STATSDUMP_P", "Sets the dump of the P command output", `Dump P command output`_
    "STATSDUMP_RXP\ :sup:`[1]`", "Sets the dump of the RXP command output", `Dump RXP command output`_
    "TXMC_CHANNEL", "Sets the channel in MultiChannel TX mode", `TXMC channel`_
    "TXMC_PACKETCOUNT", "Sets the packet count in MultiChannel TX mode", `TXMC packet count`_
    "TXMC_PACKETINTERVAL", "Sets the packet interval in MultiChannel TX mode", `TXMC packet interval`_
    "TXMC_SETTXDATA", "Sets the packet payload in MultiChannel TX mode", `TXMC packet payload`_
    "TXMC_CHSEQ", "Sets the channel sequence in MultiChannel TX mode", `TXMC channel sequence`_
    "W", "Sets the TX Power", `Transmit power`_
    "WGET_ARG_CHANNEL\ :sup:`[1]`", "Sets the channel from which the tx power will be read", `Read tx power channel`_
    "WRADDRESS", "Sets the address of the register to write to", `Write register address`_
    "WRBITMASK", "Sets the bit mask for the write register value", `Write register bitmask`_
    "WRVALUE", "Sets the value to write to a register", `Write register value`_
    "WSET_ARG_CHANNEL", "Sets the channel to which the tx power will be set", `Write tx power channel`_
    "WSET_ARG_POWER", "Sets the Tx power to be set on a specific channel", `Write tx power`_
    "XTALTRIMTEMP1...6", "[Cache-only] Sets the XTAL32M trimming zone max temperature", `Trimming zone max temp`_
    "XTALTRIMSET1...7", "[Cache-only] Sets the XTAL32M trimming zone setting", `Trimming zone setting`_


\ :sup:`[1]` 802.15.4 only

\ :sup:`[2]` BLE only


Antenna Diversity
-----------------

.. panels::

    **Command: AD ON/OFF**

    Available for: 802.15.4

This command turns the antenna diversity ON or OFF.
If antenna diversity is disabled, the antenna that was set via the AN command is used.
If no value was set via AN, the default value is used (antenna 0).
(Not available for differential antenna configurations.)


Antenna
-------

.. panels::

    **Command: AN number**

    Available for: 802.15.4 and BLE

This command allows to select the antenna used during transmit (continuous wave or packets).
If antenna diversity is switched off, this also specifies which antenna to use to receive packets.
If this command is used, the antenna diversity is turned off implicitly.
To turn the antenna diversity back on use the AD command.
(Not available for differential antenna configurations.)


BLE Data Rate
-------------

.. panels::

    Command: BLEDATARATE type
        type

            = 1M: Normal (1 Mbit) mode

            = 2M: High (2 Mbit) mode

            = LR125: Long Range (125 kbit) mode

            = LR500: Long Range (500 kbit) mode

    Available for: BLE

This command defines the mode of the BLE radio for both Tx and Rx.

.. note::
    With this command both Tx and Rx are set to the same mode. If user wants to set
    BLE mode for Rx and Tx separately, he can use BLERXDATARATE and BLETXDATARATE commands.


BLE Rx Data Rate
-------------

.. panels::

    Command: BLERXDATARATE type
        type

            = 1M: Normal (1 Mbit) receiver mode

            = 2M: High (2 Mbit) receiver mode

            = LR: Long Range receiver mode

            = LR+1M: Concurrent (1 Mbit + Long Range) receiver mode

    Available for: BLE

This command defines the mode of the BLE radio for Rx.


BLE Tx Data Rate
-------------

.. panels::

    Command: BLETXDATARATE type
        type

            = 1M: Normal (1 Mbit) data rate

            = 2M: High (2 Mbit) data rate

            = LR125: Long Range (125 kbit) data rate

            = LR500: Long Range (500 kbit) data rate

    Available for: BLE

This command defines the mode of the BLE radio for Tx.


BLE Test Packet Payload Type
----------------------------

.. panels::

    **Command: BLETESTPACKET type**

    Available for: BLE

This command defines the payload type of the BLE test packet as defined in :ref:`[2] <[2]>`.

.. csv-table:: BLE test packet types
    :header: "Type", "Summary"

    "0", "PRBS9 sequence ‘11111111100000111101…’ (in transmission order)"
    "1", "Repeated ‘11110000’ (in transmission order)"
    "2", "Repeated ‘10101010’ (in transmission order)"
    "3", "PRBS15 sequence"
    "4", "Repeated ‘11111111’ (in transmission order) sequence"
    "5", "Repeated ‘00000000’ (in transmission order) sequence"
    "6", "Repeated ‘00001111’ (in transmission order) sequence"
    "7", "Repeated ‘01010101’ (in transmission order) sequence"


Channel
-------

.. panels::

    Command: CH channel
        channel = channel number

    Available for: 802.15.4 and BLE

This command sets the RF Tx/Rx channel to the specified channel number.
This is an alias for the SRCID0 attribute.

.. note::
    RF channel does not equal the channel index used on higher layers of the BLE specification.
    The RF channel linearly maps on frequency (2402+ 2 x RF Channel) MHz.
    For details please check Vol 6 1.4.1 Advertising and Data Channel of :ref:`[2] <[2]>`.


CSMA Mode
---------

.. panels::

    Command: CSMA mode
        mode

            = NOCCA: no Clear Channel Assessment

            = CCA: CCA only; no retries if channel not clear

            = CSMA: CSMA-CA as defined in [1]

    Available for: 802.15.4

This command sets the clear channel assessment (CCA) mode to be used when transmitting a packet (see :ref:`[1] <[1]>`).

By default, the application backoff periods or failing channel access.


Continuous Wave Type
--------------------

.. panels::

    Command: CW type
        type

            = U: unmodulated continuous wave mode

            = M: modulated continuous wave mode

    Available for: 802.15.4 and BLE

This command sets the attribute of Continuous Wave Mode. This is just an attribute and using it does not start
the CW mode.

.. note::
    When CW mode is on, changing this attribute will not affect current mode.
    Using “CW M” command and then “SETCW ON” command is equal to “SETCW M ON”


DCDC Mode
---------

.. panels::

    **Command: DCDC ON/OFF**

    Available for: 802.15.4 and BLE

This command turns the DCDC mode ON or OFF.


Clock IO pin
------------

.. panels::

    Command : MAPCLKIOPIN gpio
        gpio = the GPIO number

    Available for: 802.15.4 and BLE


This command sets the GPIO that will be used to put the clock out. Running the help command of the
RadioControlConsole will show a list of GPIOs that can be used.


Clock type
----------

.. panels::

    **Command : MAPCLKTYPE type**

    Available for: 802.15.4 and BLE

This command sets the clock that will be mapped onto a defined GPIO. Running the help command of the
RadioControlConsole will show a list of clocks that can be defined.


Maximum Backoff exponent
------------------------

.. panels::

    Command : MAXBE number
        number = the maximum value (1…5) of the backoff exponent
        (0 will be accepted as 1)

    Available for: 802.15.4

This command sets the maximum value of the backoff exponent used in the CSMA-CA algorithm described in
the IEEE 802.15.4 specification :ref:`[1] <[1]>`.


Maximum Backoffs number
-----------------------

.. panels::

    Command : MC number
        number = the maximum number of back-offs (0…5)

    Available for: 802.15.4

This command sets the maximum number of back-off procedures during the CSMA-CA algorithm as explained
in the IEEE 802.15.4 specification :ref:`[1] <[1]>`.


Attenuation during timeout
--------------------------

.. panels::

    **Command : MIDLEVELAGCATT ON/OFF**

    Available for: 802.15.4 and BLE

This command enables automatic mid-level AGC (adaptive gain control) attenuation for RX if “noise” is
received for a fixed time, when set to ON. This feature is disabled by default as it only shows benefits to counter
high Wi-Fi interference. The other levels (not mid-level) of automatic RX AGC attenuation are always enabled.
The default value if OFF.


Minimum Backoff exponent
------------------------

.. panels::

    Command : MINBE number
        number = the minimum value (0…3) of the backoff exponent

    Available for: 802.15.4

This command sets the minimum value of the backoff exponent used in the CSMA-CA algorithm described in
the IEEE 802.15.4 specification :ref:`[1] <[1]>`.


Maximum MAC retries
-------------------

.. panels::

    Command: MR number
        number = the maximum number of retries (0...5)

    Available for: 802.15.4

This command sets the number of times a frame will be retransmitted by the MAC if an acknowledgement was
requested but not received.


Packet count
------------

.. panels::

    Command: PACKETCOUNT number
        number = number of packets to transmit

    Available for: 802.15.4 and BLE

This command sets the number of times a packet will be transmitted.


Packet interval
---------------

.. panels::

    Command: PACKETINTERVAL interval
        interval = interval between packets, in ms

    Available for: 802.15.4

This command sets the interval between transmitted packets.


Packet length
-------------

.. panels::

    Command: PACKETLENGTH number
        number =

                - for 802.15.4: total number of bytes in the packet
                - for BLE: number of bytes in the payload

    Available for: 802.15.4 and BLE

This command sets the length of the packet to transmit.

.. note::
    In BLE mode the maximum packet payload is 241 bytes.


Statistic channel
-----------------

.. panels::

    Command: PCHSTATCHANNEL channel
        channel = channel number

    Available for: 802.15.4

This command sets the channel for statistic printing.


PDM clock frequency
-------------------

.. panels::

    Command: PDMCLKFREQ frequency
        frequency = frequency in Hz from 375733 to 192000000

    Available for: 802.15.4 and BLE

This command configures the output frequency of the ASP module.

.. note::
    For PDMCLKSRC = CLK_2M, this value should be equal to 2000000. Other values result in error
    while enabling the SETPDMCLK mode. When using CLK_PLL, the output frequency may differ from the
    desired value due to the nature of the fractional clock (limited reference clock dividers). For more information
    about this clock, refer to the “Fractional Clock Divider” section in the User Manual.


PDM clock output pin
--------------------

.. panels::

    Command: PDMCLKUOUT gpio
        gpio = GPIOx (where x is a pin number)

    Available for: 802.15.4 and BLE

This command selects the PDM clock output pin.

.. note::
    Pin numbers available as PDM clock output depends on the chip model. In the Radio Control Console,
    the available pins are shown in the PDMCLKOUT attribute description. For more information,
    refer to the “Pin Configuration” section in the User Manual.


PDM clock source
----------------

.. panels::

    Command: PDMCLKSRC src
        src = CLK_PLL or CLK_2M

    Available for: 802.15.4 and BLE

This command selects the clock source for the ASP module (which uses PDM).

.. note::
    Not all chip models support the PLL clock source – please refer to the “PDM Microphone Interface”
    section in the User Manual.


PHY access mode
---------------

.. panels::

    Command: PHY mode
        mode

            = 802.15.4

            = BLE

    Available for: 802.15.4 and BLE

This command sets the PHY access mode. It is only changeable if RX is set to OFF.


Packet-In-Packet
----------------

.. panels::

    **Command: PI ON/OFF**

    Available for: 802.15.4

This command sets the Packet-In-Packet (PIP) resynchronization ON or OFF.


Promiscuous mode
----------------

.. panels::

    **Command: PM ON/OFF**

    Available for: 802.15.4

This command turns the Promiscuous Mode ON or OFF. In promiscuous mode the transceiver will not perform
any address filtering on incoming packets; any packet will be received. In normal mode received packets will
be filtered on Destination Address and PanID.
Default, the Promiscuous Mode is ON.


Register address
-------------

.. panels::

    Command: RRADDRESS addr
        addr = the register address used by the Register Read command

    Available for: 802.15.4 and BLE


Multi Standard RX
-----------------

.. panels::

    **Command: RXMS ON/OFF**

    Available for: 802.15.4 and BLE

This command turns the RX Mode selector to Multi Standard. If set to ON, the attributes RXHS (High Sensitivity
RX mode selector) and RXMC (Multi Channel RX mode selector) will be set to OFF. The attribute can only be
changed if RX mode is set to OFF. The default value is OFF.


High Sensitivity RX
-----------------

.. panels::

    **Command: RXHS ON/OFF**

    Available for: 802.15.4

This command turns the RX Mode selector to High Sensitivity. If set to ON, the attributes RXMS (Multi Standard
RX mode selector) and RXMC (Multi Channel RX mode selector) will be set to OFF. The attribute can only be
changed if RX mode is set to OFF. The default value is OFF.


Multi Channel RX
----------------

.. panels::

    **Command: RXMC ON/OFF**

    Available for: 802.15.4

This command configures the RX Mode selector as Multi Channel. If set to ON, the attributes RXMS (Multi
Standard RX mode selector) and RXHS (High Sensitivity RX mode selector) will be set to OFF. The attribute
can only be changed if RX mode is set to OFF. The default value is OFF.

.. note::
    This command is related to the SRCID1 and the SRCID2 attributes that control the channels used to
    listen the 802.15.4 packets.


ED scan number
--------------

.. panels::

    Command: SCANCOUNT number
        number = number of scans during Energy Detect (ED)

    Available for: 802.15.4

This command sets the number of scans during Energy Detect.


ED scan interval
----------------

.. panels::

    Command: SCANINTERVAL interval
        interval = interval between scans during Energy Detect (ED), in ms

    Available for: 802.15.4

This command sets the interval between scans during Energy Detect.


TX packet payload
-----------------

.. panels::

    Command: SETTXDATA data
        data = 6...max Character string where each character (0…9, a...f) is interpreted as a
        hexadecimal value for a nibble.
        Should not be prepended with “0x”!
        The maximum length is application/platform specific.

    Available for: 802.15.4

This command specifies the data to be used as payload when transmitting a packet using the TX command.
The data set using the SETTXDATA command is interpreted as the MAC Header and MAC Payload of the
IEEE 802.15.4 data frame. This means the first 2 bytes (4 characters) will be interpreted as the MAC Frame
Control field, the third byte (next 2 characters) will be interpreted as the MAC Sequence Number and the
following bytes will be interpreted as Addressing fields (as the MAC Frame Control field specifies these to be
present). All remaining bytes will be interpreted as the MAC Payload. Please refer to the general MAC frame
format as specified in the IEEE 802.15.4 specification [1] for additional information on the format of a data
packet.

.. note::
    If no data is set using the SETTXDATA command, the transmit command will use a default data packet
    (see 4.12 below).
    The maximum length that can be configured is application specific.

..  hint::
    Example 1:
        (input:) SETTXDATA 01234567890ABCDEFabcdef

        (output:) Failed to set SETTXDATA, reason: Value string doesn't have even length

    Example 2:
        (input:) SETTXDATA 00112233445566778899

        (output:) *Data set*


Pan ID
------

.. panels::

    Command: SPAN panId
        panId = the 16-bit PAN ID to be used (default 0xCAFE)

    Available for: 802.15.4

This command sets the PAN ID that, in normal mode (i.e. Promiscuous Mode is off), is used in combination
with the Short Address to filter incoming packets, and to decide whether an acknowledgement for an incoming
packet needs to be generated or not.


Source ID 0
-----------

.. panels::

    Command: SRCID0 channel
        channel = channel number

    Available for: 802.15.4 and BLE

This command sets the RF Tx/Rx channel to the specified channel number. More details in the `Channel`_
paragraph.


Source ID 1
-----------

.. panels::

    Command: SRCID1 channel
        channel = channel number

    Available for: 802.15.4

This command sets the additional RF Rx channel to the specified channel number. When configured, allows
to receive packets on multiple channels simultaneously in the MultiChannel (RXMC) mode.


Source ID 2
-----------

.. panels::

    Command: SRCID2 channel
        channel = channel number

    Available for: 802.15.4

This command sets the additional RF Rx channel to the specified channel number. When configured, allows
to receive packets on multiple channels simultaneously in the MultiChannel (RXMC) mode.


Short MAC address
-----------------

.. panels::

    Command: SSA address
        address = the 16-bit Short Address to be used (default 0xBEEF)

    Available for: 802.15.4

This command sets the Short Address that, in normal mode (i.e. Promiscuous Mode is off), is used in
combination with the PAN ID to filter incoming packets, and to decide whether an acknowledgement for an
incoming packet needs to be generated or not.


Sleep mode configuration
------------------------

.. panels::

    Command: SLP mode
        mode

            = RC

            = 32KHZ

            = 32MHZ

    Available for: 802.15.4 and BLE

This command sets the dumping of the P command output to a file.

Dump P command output
---------------------

.. panels::

    Command: STATSDUMP_P mode
        mode

            = DUMP_OFF

            = DUMP_CSV

    Available for: 802.15.4 and BLE

This command sets the dumping of the P command output to a file.

.. note::
    The CSV file is stored in the *csv* folder, and its name is the P_<phy_mode>_<timestamp>.csv, where
    the *phy_mode* equals RF4CE or BLE and the timestamp is the moment of the program was launched.


Dump RXP command output
-----------------------

.. panels::

    Command: STATSDUMP_RXP mode
        mode

            = DUMP_OFF

            = DUMP_CSV

    Available for: 802.15.4

This command sets the dumping of the RXP command output to a file.

.. note::
    The CSV file is stored in the *csv* folder, and its name is the RXP_RF4CE_<timestamp>.csv, where the
    timestamp is the moment of the program was launched.


TXMC channel
------------

.. panels::

    Command: TXMC_CHANNEL channel
        channel = channel number

    Available for: 802.15.4

This command sets the RF channel in MultiChannel TX mode.

.. note::
    Channel is set for the index configured with the TXMC_CHSEQ command. The allowed channels range from 11 to 26
    (see `Transmit packets in MultiChannel mode`_ for example configuration).


TXMC packet count
-----------------

.. panels::

    Command: TXMC_PACKETCOUNT number
        number = number of packets to transmit

    Available for: 802.15.4

This command sets the number of times a packet will be transmitted in MultiChannel TX mode.

.. note::
    Packet count is set for the index configured with the TXMC_CHSEQ command
    (see `Transmit packets in MultiChannel mode`_ for example configuration).


TXMC packet interval
--------------------

.. panels::

    Command: TXMC_PACKETINTERVAL interval
        interval = interval between packets, in ms

    Available for: 802.15.4

This command sets the interval between transmitted packets in MultiChannel TX mode.

.. note::
    Packet interval is set for the index configured with the TXMC_CHSEQ command
    (see `Transmit packets in MultiChannel mode`_ for example configuration).


TXMC packet payload
-------------------

.. panels::

    Command: TXMC_SETTXDATA data
        data = 6...max Character string where each character (0…9, a...f) is interpreted as a
        hexadecimal value for a nibble.
        Should not start with “0x”!
        The maximum length is application/platform specific.

    Available for: 802.15.4

This command specifies the data to be used as the payload when transmitting a packet using the TXMC
command.

See `TX packet payload`_ for details about frame format.

.. note::
    Packet payload is set for the index configured with the TXMC_CHSEQ command
    (see `Transmit packets in MultiChannel mode`_ for example configuration).


TXMC channel sequence
---------------------

.. panels::

    Command: TXMC_CHSEQ idx
        idx = index to configure in channel sequence

    Available for: 802.15.4

This command sets the index to be configured in channel sequence for MultiChannel TX mode.
The allowed indexes range from 0 to 2.

.. note::
    Commands: TXMC_CHANNEL, TXMC_PACKETCOUNT, TXMC_SETTXDATA, TXMC_PACKETINTERVAL operates on
    index set in this command (see `Transmit packets in MultiChannel mode`_ for example configuration).


Transmit power
--------------

.. panels::

    Command: W power
        power = transmit output power in dBm

    Available for: 802.15.4 and BLE

This command sets the transmit power of the chip. Please refer to the chip’s Data Sheet for the supported TX
power control range.


Read tx power channel
---------------------

.. panels::

    Command: WGET_ARG_CHANNEL channel
        channel = the channel whose tx power will be returned

    Available for: 802.15.4

This command sets the channel from which the tx power will be read with WGET command (see `Get Tx power for channel`_).


Write register address
----------------------

.. panels::

    Command: WRADDRESS addr
        addr = the register address

    Available for: 802.15.4 and BLE

This command sets the register address that will be used by the Register Write commands.


Write register bitmask
----------------------

.. panels::

    Command: WRBITMASK mask
        mask = a bitmask to use when setting a write register value

    Available for: 802.15.4 and BLE

This command Sets the bit mask for the write register value that will be used by the Register Write Bits command (see `Write bits to register`_).


Write register value
--------------------

.. panels::

    Command: WRVALUE value
        value = byte-value to write to a register

    Available for: 802.15.4 and BLE

This command sets the register value that will be used by the Register Write commands.


Write tx power channel
----------------------

.. panels::

    Command: WSET_ARG_CHANNEL channel
        channel = the channel whose tx power will be set

    Available for: 802.15.4 and BLE

This command sets the channel to which the tx power will be set with WGET (see `Get Tx power for channel`_).


Write tx power
--------------

.. panels::

    Command: WSET_ARG_POWER power
        power = the power to be set

    Available for: 802.15.4 and BLE

This command sets the Tx power to be set on a specific channel with WSET ( see `Apply power configuration`_).


Trimming zone max temp
----------------------


.. panels::

    Command: XTALTRIMTEMP1...6 maxTemp
        maxTemp = the upper border of the temperature zone

    Available for: 802.15.4 and BLE

.. note::
    Please use the SETXTALTRIMCAP to configure the 32MHz crystal temperature calibration.


The maxTemp values set individually (using this command) will only be cached locally and the I command will
overwrite them with the current settings. To commit the configuration, one has to use the SETXTALTRIMCAP
with the full set of the arguments or set the parameters individually without calling the I command and then
use the SETXTALTRIMCAP without any arguments.


The maxTemp value is the upper border of the zone and should not overlap other zones, including a hysteresis
(a margin between zones). The 7th zone has a hardcoded max temperature set to 125°C. The 1st zone has no
lower border, but its value should be more or equal to -40°C.


Trimming zone setting
---------------------


.. panels::

    Command: XTALTRIMSET1...7 setting
        setting = the temperature zone setting for the trimming algorithm

    Available for: 802.15.4 and BLE

.. note::
    Please use the SETXTALTRIMCAP to configure the 32MHz crystal temperature calibration.


The setting values set individually (using this command) will only be cached locally and the I command will
overwrite them with the current settings. To commit the configuration, one must use the SETXTALTRIMCAP
with the full set of the arguments or set the parameters individually without calling the I command and then
use the SETXTALTRIMCAP without any arguments.


Modes
*****

.. |_| unicode:: 0xA0
   :trim:


Modes define the execution state of the chip. The modes can be switched on or off. The modes that are
switched on remain on until the off command has been issued. The On/Off switch should always be the last
argument in case any arguments are given.


The following table shows the known modes (for all API versions) along with their short names.


.. csv-table:: Known modes
    :header: "Short Name", "Description", "Reference"

    "CHIPTEMP", "Prints current chip temperature in °C", `Chip temperature`_
    "CLOAD_TABLE", "Prints load capacitor coefficients table", `Load capacitors table`_
    "ED\ :sup:`[1]`", "Does an energy detect scan", `Energy detect`_
    "P", "Prints packet statistics", `Packet Statistics`_
    "PANT\ :sup:`[1]`", "Prints antenna statistics", `Antenna statistics`_
    "PCH\ :sup:`[1]`", "Prints packet statistics for specified channel", `Packet statistics for channel`_
    "PCHA\ :sup:`[1]`", "Prints packet statistics for all channel", `Packet statistics for all channels`_
    "R", "Resets packet statistics", `Reset packet statistics`_
    "RANT\ :sup:`[1]`", "Resets antenna statistics", `Reset antenna statistics`_
    "RCH\ :sup:`[1]`", "Resets statistics packet count for specified channel", `Reset packet statistics for channel`_
    "RR", "Reads register", `Read register`_
    "RX", "Sets the chip in Receive Mode", `Receive Mode`_
    "RXP\ :sup:`[1]`", "Prints the received packets", `Print received packets`_
    "SETCW", "Turns on the continuous wave mode", `Continuous wave mode`_
    "SETCLCKONGPIO", "Maps a clock on a certain GPIO", `Set clock on gpio`_
    "SETPDMCLK", "Configures and enable the PDM clock output", `Set PDM clock`_
    "SETXTALTRIMCAP", "Configures the XTAL32M trimming feature", `Set XTAL trimming`_
    "SL", "Puts the radio in sleep mode", `Radio sleep mode`_
    "TX", "Transmits packets with payload from SETTXDATA command", `Transmit packets`_
    "TXMC\ :sup:`[1]`", "Transmits data on MultiChannel TX mode", `Transmit packets in MultiChannel mode`_
    "TXR\ :sup:`[1]`", "Transmits packets with random payload", `Transmit packets with random payload`_
    "WGET", "Gets Tx power of a specific channel", `Get Tx power for channel`_
    "WR", "Writes byte value to register", `Write byte to register`_
    "WRB", "Writes bits value to register", `Write bits to register`_
    "WSET\ :sup:`[1]`", "Apply channel specific power configuration", `Apply power configuration`_


\ :sup:`[1]` 802.15.4 only


Chip temperature
----------------

.. panels::

    **Command: CHIPTEMP**

    Available for: 802.15.4 and BLE

This command shows the current chip temperature in °C.

..  hint::
    Example:
        (input:) CHIPTEMP

        (output:) Chip temperature: 27,5°C


Load capacitors table
---------------------

.. panels::

    **Command: CLOAD_TABLE**

    Available for: 802.15.4 and BLE

This command shows the table of load capacitor frequency error in ppm for various temperature range in °C.
This table currently only applies to the QPG6200
..  hint::
    Example:
        (input:) CLOAD_TABLE

        (output:)
    Parameter     Temperature range Frequency Error [ppm]
    ============= ================= =====================
    cload_comp_0  T < -30°C         15
    cload_comp_1  -30°C < T < -20°C 15
    cload_comp_2  -20°C < T < -10°C 15
    cload_comp_3  -10°C < T < 0°C   15
    cload_comp_4  0°C < T < 10°C    0
    cload_comp_5  10°C < T < 20°C   0
    cload_comp_6  20°C < T < 30°C   0
    cload_comp_7  30°C < T < 40°C   0
    cload_comp_8  40°C < T < 50°C   -15
    cload_comp_9  50°C < T < 60°C   -15
    cload_comp_10 60°C < T < 70°C   -20
    cload_comp_11 70°C < T < 80°C   -20
    cload_comp_12 80°C < T < 90°C   -15
    cload_comp_13 90°C < T < 100°C  -15
    cload_comp_14 100°C < T < 110°C -5
    cload_comp_15 110°C < T < 120°C 10
    cload_comp_16 130°C < T         25


Energy detect
-------------

.. panels::

    Command: scancount scaninterval ON/OFF
        scancount = number of scans

        scaninterval = interval between scans, in ms

    Available for: 802.15.4

This command performs *scancount* Energy Detect (ED) scans, with a specified *scaninterval*. An indication of
the amount of energy (linear scale) is shown for each ED scan. The ED scan is done on the channel currently
set using the CH command.

..  hint::
    Example:
        (input:)  ED 3 7 ON

        (output:)
            ED 2C

            ED 2E

            ED 30


Packet Statistics
-----------------

.. panels::

    Command: P STATSDUMP_P
        STATSDUMP_P

            = DUMP_OFF: disables command output dumping to a file

            = DUMP_CSV: enables command output dumping to a CSV file

    Available for: 802.15.4 and BLE

This command shows the following packet statistics:
    RX Packet Count - The number of packets received with correct FCS. This can be used to determine the
    Packet Error Rate (PER).

    TX OK - The number of packets transmitted successfully (triggered via the TX or TXR command).

    TX FAIL - The number of transmission failures (triggered via the TX or TXR command).

    Avg RSSI, LQI -The average RSSI and LQI measured over the received packets.

These statistics can be reset with the R command (see [FIXME add link]4.6 below).

.. note::
    There is an optional possibility to dump command output to a CSV file.

..  hint::
    Example:
        (input:) P

        (output:)
            Avg RSSI : -34

            Avg LQI : 237

            RX Packet Count : 84

            TX OK : 100

            TX FAIL : 0


Antenna statistics
------------------

.. panels::

    **Command: PANT**

    Available for: 802.15.4

This command shows the following antenna statistics for both antennas:
    Number of RX packets - The number of received packets.

    Avg RSSI - The average RSSI measured over the received packets.

..  hint::
    Example:
        (input:) PANT

        (output:)
            Antenna 0:

                Number of RX packets: 2

                Avg RSSI: -52

            Antenna 1:

                Number of RX packets: 0

                Avg RSSI: 0

.. note::
    In BLE mode, it is not possible to get the antenna and the RSSI of the received packets.


Packet statistics for channel
-----------------------------

.. panels::

    Command: PCH pchstatchannel
        pchstatchannel = channel for which statistics will be shown

    Available for: 802.15.4

This command shows the following packet statistics for the selected receiver channel:
    RX - The number of packets received with correct FCS. This can be used to determine the
    Packet Error Rate (PER).

    TX OK - The number of packets transmitted successfully (triggered via the TX or TXR command).

    TX FAIL - The number of transmission failures (triggered via the TX or TXR command).

    RSSI, LQI - The RSSI and LQI measured over the received packets

..  hint::
    Example:
        (input:) PCH 20

        (output:)
            Channel |_| |_| |_| RSSI  |_| |_| |_| LQI |_| |_| |_| RX Packet Cnt |_| |_| |_| TX OK |_|  |_| |_| TX FAIL

            -- - - - - - |_| |_| |_| - - - |_| |_| |_| - - - |_| |_| |_| |_| - - - - - - - - - -|_| |_| |_| |_| - - - - -|_| |_| |_| - - - - - -

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 20 |_| |_| |_| |_|  -54 |_| |_| |_| 255 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| 1 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0


Packet statistics for all channels
----------------------------------

.. panels::

    **Command: PCHA**

    Available for: 802.15.4

This command shows shows the following packet statistics for all 16 channels of the receiver.
    RX - The number of packets received with correct FCS. This can be used to determine the
    Packet Error Rate (PER).

    TX OK - The number of packets transmitted successfully (triggered via the TX or TXR command).

    TX FAIL - The number of transmission failures (triggered via the TX or TXR command).

    RSSI, LQI - The RSSI and LQI measured over the received packets

..  hint::
    Example:
        (input:) PCHA

        (output:)
            Channel |_| |_| |_| RSSI  |_| |_| |_| LQI |_| |_| |_| RX Packet Cnt |_| |_| |_| TX OK |_|  |_| |_| TX FAIL

            -- - - - - - |_| |_| |_| - - - |_| |_| |_| - - - |_| |_| |_| |_| - - - - - - - - - -|_| |_| |_| |_| - - - - -|_| |_| |_| - - - - - -

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 11 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 12 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 13 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 14 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 15 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 16 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 17 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 18 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 19 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 20 |_| |_| |_| -54 |_| |_| |_| 255 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| 1 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 21 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 22 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 23 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 24 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 25 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0

            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 26 |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_|
            |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0 |_| |_| |_| |_| |_| |_| |_| |_| |_|  |_| |_| |_| |_| |_| |_| 0


.. note::
    To collect signal quality statistics, you must set the required payload: **SETTXDATA 0100AA0102030405060708090A0B0C**


Reset packet statistics
-----------------------

.. panels::

    **Command: R**

    Available for: 802.15.4 and BLE

This command resets the packet statistics (see `Packet Statistics`_).


Reset antenna statistics
------------------------

.. panels::

    **Command: RANT**

    Available for: 802.15.4

This command resets the antenna statistics (see `Antenna Statistics`_).


Reset packet statistics for channel
-----------------------------------

.. panels::

    Command: RCH pchstatchannel
        pchstatchannel = channel for which statistics will be reset

    Available for: 802.15.4

This command resets the packet statistics for the selected receiver channel (see `Packet statistics for channel`_).


Read register
-------------

.. panels::

    Command: RR rraddress
        raddress = the address of the register to be read

    Available for: 802.15.4 and BLE

This command returns the content of a register.


Receive Mode
------------

.. panels::

    **Command: RX ON/OFF**

    Available for: 802.15.4 and BLE

This command turns the receiver ON or OFF.

If an invalid combination of RX mode selector (RXMS, RXHS, RXMC) and PHY is selected, RX won’t turn ON.

.. note::
    Default, the application starts with the receiver OFF.


Print received packets
----------------------

.. panels::

    Command: RXP STATSDUMP_RXP ON/OFF
        STATSDUMP_RXP

            = DUMP_OFF: disables command output dumping to a file

            = DUMP_CSV: enables command output dumping to a CSV file

    Available for: 802.15.4

This command turns the display of received packets ON or OFF. The maximum packet length that can be
displayed is application specific. Larger packets will be received but are not displayed.

There is an optional possibility to dump command output to a CSV file.

.. note::
    Default, this setting is turned OFF.

If mode is set to ON, following information are displayed:
    • Antenna used (0/1 – antenna number, 2 – automatic)
    • RSSI
    • LQI
    • Channel
    • Payload
    • Length of payload

..  hint::
    Example:
        (output:) RX Packet: antenna: 2, rssi: -52, lqi: 255, channel: 21, payload: 00 01 66 77 88 99, len: 6


Continuous wave mode
--------------------

.. panels::

    Command: SETCW mode ON/OFF
        mode
            = U: unmodulated continuous wave mode

            = M: modulated continuous wave mode

    Available for: 802.15.4 and BLE

This command sets the chip in the continuous wave mode. It is possible to switch from modulated to unmodulated
mode and back.

.. note::
    Continuous wave mode is disabled implicitly the device is power-cycled.
    During continuous mode, the receiver will be disabled. When continuous mode is disabled, the state as
    it was before continuous wave was started will be set again. Also, during continuous wave mode, some
    commands (i.e., transmitting data) will be disabled until the user disables the continuous wave mode.


Set clock on gpio
-----------------

.. panels::

    Command: SETCLKONGPIO mapclkiopin mapclktype ON/OFF
        mapclkiopin = GPIO on which the clock will be mapped

        mapclktype = type of clock that will be mapped

    Available for: 802.15.4 and BLE

This command brings the clock to GPIO defined by the MapClkiopin attribute.


Set PDM clock
-------------

.. panels::

    Command: SETPDMCLK pdmclksrc pdmclkfreq pdmclkout ON/OFF
        pdmclksrc = the PDM clock source

        pdmclkfreq = the PDM clock output frequency

        pdmclkout = the PDM clock output pin

    Available for: 802.15.4 and BLE

This command configures and enables/disables the PDM clock output.

..  hint::
    Example for fixed 2MHz:
        (input:) SETPDMCLK CLK_2M 2000000 GPIO3 ON

        (output:) *SETPDMCLK set to ON*

    Example for configurable clock:
        (input:) SETPDMCLK CLK_PLL 2048000 GPIO17 ON

        (output:) *SETPDMCLK set to ON*

.. note::
    Changing the related attributes independently while SETPDMCLK mode is enabled has no effect until
    the next call to the SETPDMCLK command. If you do not use the ON command, only attributes will be set or
    updated when already ON.


Set XTAL trimming
-----------------

.. panels::

    Command: SETXTALTRIMCAP maxTemp_1 setting_1 ... maxTemp_6 setting_6 setting_7
        maxTemp_1 = upper border of the temperature zone [-40°C to 125°C]

        setting_1 = setting used to trim the capacitor in the corresponding zone

        …
    Available for: 802.15.4 and BLE

This command configures the 32 MHz crystal frequency correction algorithm used to compensate for the
temperature dependent frequency offset of the 32 MHz crystal. Please use Qorvo’s Crystal Procurement Tool
to generate the parameters for this command that are most optimal for your crystal. The command requires 7
zones to be configured with a switch capacitor mode (setting_x) and a temperature range where this setting
is applicable (maxTemp_x).

..  hint::
    Example:
        (input:) SETXTALTRIMCAP 17 3 27 2 41 1 97 0 112 1 118 2 3

To understand the temperature zones, please refer to either the Qorvo Crystal Procurement Tool, the
application note “GP_P008_AN_20182_Configuring_Frequency_Offset_Correction_Algorithm“ or the picture
below. The zones are marked in red, the trimming settings [0-3] in black (at the top).

.. image:: ../assets/Optimized_Multiple_Zone_Compensation.png

.. note::
    There is no lower border of the first zone and no upper border of the last zone, but the penultimate zone
    max temperature should not be higher than 125°C, including the hysteresis.

.. note::
    The algorithm uses hysteresis to switch between zones. All adjacent zones should be at least the width
    of 2x the hysteresis value (GP_BSP_TRIMCAP_HYSTERESIS).


Radio sleep mode
----------------

.. panels::

    Command: SL mode ON / SL mode ON time
        mode = the supported sleep modes by the radio

        time = number of milliseconds (ms) that the radio will sleep

    Available for: 802.15.4 and BLE

This command will put the radio in sleep mode. The time is optional. When specified, the radio will put itself
into sleep for that amount of time and will wake up and restore its attributes after the time has expired. If no
time is specified, the chip will remain in sleep mode and some external action is required to wake the chip up.
During the time that the chip is in sleep, no communication is possible.


Transmit packets
----------------

.. panels::

    Command: TX packetcount packetinterval ON/OFF
        packetcount = number of transmit attempts (≥0)

        packetinterval = interval between transmission attempts, in ms (≥0)

    Available for: 802.15.4 and BLE

In case of 802.15.4, this command sends *packetcount* packets with the data that was specified by the last
SETTXDATA command with a specified *packetinterval*.

In case of BLE, the command sends *packetcount* BLE test packets with a payload type specified by the
BLETESTPACKET command and the last PACKETLENGTH command. The *packetinterval* parameter is not
available for BLE.

..  hint::
    Example 1:
        (input:) TX 10 0 ON

        (output:) *TX set to ON*

    Example 2:
        (input:) TX 0xF1 0xA ON

        (output:) TX set to ON

.. note::
    If you do not use the ON command, no packets will be transmitted.

.. note::
    The *packetinterval* parameter (only 802.15.4) is used to define the time in milliseconds between the
    reception of the dataconfirm primitive of the previous packet and the trigger to send the new packet. The
    minimum value of the interval should be 5 ms. The software does not take into account the time required
    to write the packet over the serial bus to the chip and the actual transmit time. Therefore, the actual inter
    packet delay will always be larger than interval.

    When *packetcount* equals 0xffff, the chip will start a continuous transmit loop. When *packetcount* equals
    0, a previously started continuous transmission session will be stopped.


Transmit packets in MultiChannel mode
-------------------------------------

.. panels::

    Command: TXMC txmc_chseq txmc_channel txmc_packetcount txmc_settxdata txmc_packetinterval ON/OFF
        txmc_chseq = channel sequence index (0 ≤ txmc_chseq ≤ 2)

        txmc_channel = channel number (11 ≤ txmc_channel ≤ 26)

        txmc_packetcount = number of packets to transmit (≥0)

        txmc_settxdata = packet payload

        txmc_packetinterval = interval between transmission attempts, in ms (≥0)

    Available for: 802.15.4

This command sends the *txmc_packetcount* packets on the *txmc_channel* channel with the *txmc_settxdata*
payload and the *txmc_packetinterval* interval. With the *txmc_chseq* it is possible to configure up to 3 sequential
transmissions.

..  hint::
    Example 1:
        (input:) TXMC 0 20 10 0001AA55 1000

        (input:) TXMC 1 21 5 0001BBCC 100

        (input:) TXMC 2 22 15 0001DDEE 5000 ON

        After setting mode to ON, the transmission with sequence: ch20 → ch21 → ch22 → ch20 → ch21 →
        ch22… will be triggered.

    Example 2:
        (input:) TXMC 0x0 0x14 0xA 0001AA55 0x3E8

        (input:) TXMC 0x1 0xFF

        (input:) TXMC 0x2 0xFF

        (input:) TXMC ON

        To stop further transmissions for already configured channel sequence index, set its channel to invalid
        value (e.g. 255).

    Example 3:
        (input:) TXMC 0

        (output:)
            channel: 20

            packet_count: 10

            data_length: 4

            packet_interval: 1000

        Executing the TXMC command with only the *txmc_chseq* parameter will result in retrieving the
        configuration for this channel sequence index.


Transmit packets with random payload
------------------------------------

.. panels::

    Command: TXR packetcount packetinterval packetlength ON/OFF
        packetcount = number of transmit attempts (>0)

        packetinterval = interval between trdansmit attempts, in ms (≥0)

        packetlength = length of the random packet

    Available for: 802.15.4

This command sends *packetcount* packets with *packetlength* random data bytes with a specified
*packetinterval*.

..  hint::
    Example 1:
        (input:) TXR 10 0 15 ON

        (output:) *TXR set to ON*

    Example 2:
        (input:) TXR 0xF1 0xA 0x5 ON

        (output:) *TXR set to ON*

.. note::
    The *packetinterval* parameter is used in the application to define the time in milliseconds between the
    transmit confirmation of the previous packet and the trigger to send a new packet. The minimum value
    of the interval should be 5 ms. The software does not take into account any bus transfers and medium
    access delays. Therefore, the actual inter packet delay will always be larger than interval.

    When number equals 0xffff, the chip will start a continuous transmit loop. When number equals 0, a
    previously started continuous transmission session will be stopped.


Get Tx power for channel
------------------------

.. panels::

    Command: WGET wgetchannel
        wgetchannel = the channel whose power will be returned

    Available for: 802.15.4

This command retrieves the transmit power for a specific channel.

..  hint::
    Example:
        (input:) WGET 11

        (output:) Channel power value : 10


Write byte to register
----------------------

.. panels::

    Command: WR wraddress wrvalue
        wraddress = the address of the register to write to

        wrvalue = the byte value that needs to be written to the register

    Available for: 802.15.4 and BLE

This command writes a value to a register.


Write bits to register
----------------------

.. panels::

    Command: WRB wraddress wrbitmask wrvalue
        wraddress = the address of the register to write to

        wrbitmask = the mask to be used over the byte value

        wrvalue = the byte value that needs to be written to the

    Available for: 802.15.4 and BLE

This command writes a bit value to a register.


Apply power configuration
-------------------------

.. panels::

    Command: WSET wsetchannel wsetpower
        wsetchannel = the channel for which the power is to be set

        wsetpower = the power for a specific channel

    Available for: 802.15.4

This command sets the transmit power for a specific channel.

..  hint::
    Example:
        (input:) WSET 11


References
**********

.. _[1]:

.. [1] IEEE Standard for Low-Rate Wireless Personal Area Networks (WPANs);

    IEEE Std 802.15.4 – 2015

.. _[2]:

.. [2] Bluetooth 4.2 Specification

.. _[3]:

.. [3] GP_P330_AN_13550_Integrate_PTC_into_SDK.pdf
