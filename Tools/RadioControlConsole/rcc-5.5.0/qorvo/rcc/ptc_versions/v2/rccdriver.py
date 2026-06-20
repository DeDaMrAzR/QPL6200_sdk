"""RCC driver API"""

import logging
from dataclasses import dataclass
from typing import Callable, List, Optional, TypeVar
from tabulate import tabulate
from ...rcc_types.deviceinfo import API_VERSION_UNKNOWN, DeviceInfo
from ...rcc_types.generic import (
    AppGeneric,
    CommandAttributeBytes,
    CommandAttributeEnum,
    CommandAttributeGpioNumber,
    CommandAttributeInt,
    CommandAttributeIntAndOff,
    CommandAttributeOnOff,
    CommandModeAction,
    CommandModeOnOff,
    ConsoleCommand,
    IndicationMessageCallback,
    RCCDriverGeneric,
)
from ...transport.peakconnect import peakConnector
from .ptcdriver import PTC, BaseIndication, EdScanIndication, RxPacketIndication

logger = logging.getLogger(__name__)


T = TypeVar("T")


@dataclass
class ProtoModeOnOff:
    name: str
    help_text: List[str]
    on_func: Callable[[Optional[int]], None]
    off_func: Callable[[], None]
    getvalue_func: Optional[Callable[[], bool]]
    attribute_list: Optional[List[str]] = None
    extra_action_on_args: Callable[[], Optional[List[str]]] = None


@dataclass
class ProtoModeAction:
    name: str
    help_text: List[str]
    action: Callable[[], Optional[List[str]]]
    attribute_list: Optional[List[str]] = None


class PTCDriver(RCCDriverGeneric):
    """RCC driver class for PTC devices."""

    api_version = 2

    def __init__(
        self,
        app: AppGeneric,
        connector: peakConnector,
        user_commands: Optional[List[ConsoleCommand]] = None,
        debug_level: int = 0,
    ):
        self.api = PTC(connector, debug_level, self._indication_message_cb)

        ptc_commands: List[ConsoleCommand] = [
            CommandAttributeInt(
                "CH", ["This is an alias for the SRCID0 to keep backward compatibility"], self.api, "channel"
            ),
            CommandAttributeInt(
                "SRCID0",
                ["Set main logic channel for the 802.15.4 (11 to 26) or the RF channel index for the BLE (0 to 39)"],
                self.api,
                "channel_srcid0",
            ),
            CommandAttributeIntAndOff(
                "SRCID1",
                ["Set second logic channel for the 802.15.4 (11 to 26). Works ony when the RXMC is enabled"],
                self.api,
                "channel_srcid1",
            ),
            CommandAttributeIntAndOff(
                "SRCID2",
                ["Set third logic channel for the 802.15.4 (11 to 26). Works ony when the RXMC is enabled"],
                self.api,
                "channel_srcid2",
            ),
            CommandAttributeEnum[PTC.AntennaValues](
                "AN",
                ["Select the active antenna"],
                self.api,
                "antenna",
                {
                    "0": PTC.AntennaValues.A0,
                    "1": PTC.AntennaValues.A1,
                    "AUTO": PTC.AntennaValues.AUTO,
                },
            ),
            CommandAttributeOnOff(
                "AD",
                ["Set antenna diversity enabled flag"],
                self.api,
                "antenna_diversity",
            ),
            CommandAttributeEnum[PTC.SleepModes](
                "SLP",
                ["Set the device sleep mode"],
                self.api,
                "sleep_mode",
                {
                    "RC": PTC.SleepModes.SleepRC,
                    "32KHZ": PTC.SleepModes.Sleep32kHz,
                    "32MHZ": PTC.SleepModes.Sleep32MHz,
                },
            ),
            CommandAttributeEnum[PTC.CSMAModes](
                "CSMA",
                ["Set the CMSA mode"],
                self.api,
                "csma_mode",
                {
                    "NOCCA": PTC.CSMAModes.NoCCA,
                    "CCA": PTC.CSMAModes.CCA,
                    "CSMA": PTC.CSMAModes.CSMA,
                },
            ),
            CommandAttributeInt(
                "MAXBE",
                ["Sets the maximum MAC backoff exponent"],
                self.api,
                "max_mac_backoff_exponent",
            ),
            CommandAttributeInt(
                "MINBE",
                ["Sets the minimum MAC backoff exponent"],
                self.api,
                "min_mac_backoff_exponent",
            ),
            CommandAttributeInt(
                "MC",
                ["Sets the maximum CSMA backoff exponent"],
                self.api,
                "max_csma_backoff_exponent",
            ),
            CommandAttributeInt("MR", ["Sets the maximum MAC retries"], self.api, "max_mac_retries"),
            CommandAttributeInt("W", ["Sets the transmit power"], self.api, "power"),
            CommandAttributeInt("SPAN", ["Sets the PAN ID"], self.api, "pan_id"),
            CommandAttributeInt("SSA", ["Sets the short address"], self.api, "short_address"),
            CommandAttributeEnum[PTC.ContinuousWaveMode](
                "CW",
                ["Sets the continuous wave mode"],
                self.api,
                "continuous_wave_mode",
                {
                    "U": PTC.ContinuousWaveMode.Unmodulated,
                    "M": PTC.ContinuousWaveMode.Modulated,
                },
            ),
            CommandAttributeOnOff(
                "PCL",
                ["Set the power control loop flag"],
                self.api,
                "power_control_loop_flag",
            ),
            CommandAttributeInt(
                "RRADDRESS",
                ["Sets address of the register to be read"],
                self.api,
                "read_register_address",
            ),
            CommandAttributeInt(
                "WRADDRESS",
                ["Sets address of the register to be written"],
                self.api,
                "write_register_address",
            ),
            CommandAttributeInt(
                "WRVALUE",
                ["Sets value to be written to the register"],
                self.api,
                "write_register_value",
            ),
            CommandAttributeInt(
                "WRBITMASK",
                ["Sets write register bitmask value"],
                self.api,
                "write_register_bitmask",
            ),
            CommandAttributeInt(
                "PACKETINTERVAL",
                ["Packet interval in ms"],
                self.api,
                "packet_interval_ms",
            ),
            CommandAttributeInt("SCANINTERVAL", ["Scan interval in ms"], self.api, "scan_interval_ms"),
            CommandAttributeInt("PACKETLENGTH", ["Packet length, in bytes"], self.api, "packet_length"),
            CommandAttributeInt("PACKETCOUNT", ["Number of packets"], self.api, "packet_count"),
            CommandAttributeInt("SCANCOUNT", ["Number of scans"], self.api, "scan_count"),
            CommandAttributeOnOff("PI", ["Packet in packet mode"], self.api, "packet_in_packet_flag"),
            CommandAttributeOnOff("PM", ["Promiscuous"], self.api, "promiscuous_mode_flag"),
            CommandAttributeBytes("SETTXDATA", ["Transmit packet payload"], self.api, "txdata"),
            CommandAttributeBytes(
                "TXMC_SETTXDATA", ["Transmit packet payload for multi channel tx"], self.api, "txmc_txdata"
            ),
            CommandAttributeInt(
                "TXMC_PACKETCOUNT", ["Number of packets for multi channel tx"], self.api, "txmc_packet_count"
            ),
            CommandAttributeInt("TXMC_PACKETINTERVAL", ["Packet interval in ms"], self.api, "txmc_packet_interval_ms"),
            CommandAttributeInt("TXMC_CHANNEL", ["Channel selection for multi channel tx"], self.api, "txmc_channel"),
            CommandAttributeInt(
                "TXMC_CHSEQ", ["Set channel sequence index for multi channel tx"], self.api, "txmc_chseq"
            ),
            CommandAttributeEnum[PTC.PhyMode](
                "PHY",
                ["Set the Phy mode"],
                self.api,
                "phy",
                {"RF4CE": PTC.PhyMode.RF4CE, "BLE": PTC.PhyMode.BLE},
            ),
            CommandAttributeInt(
                "BLETESTPACKET",
                ["Defines the BLE TestPacket Type to be used in Direct Test Mode Transmit"],
                self.api,
                "ble_dtm_packet_type",
            ),
            CommandAttributeEnum[PTC.BleDataRate](
                "BLEDATARATE",
                ["Defines the BLE Data Rate that will be used for Tx and Rx"],
                self.api,
                "ble_datarate",
                {
                    "1M": PTC.BleDataRate.Ble1M,
                    "2M": PTC.BleDataRate.Ble2M,
                    "LR125": PTC.BleDataRate.Ble125LR,
                    "LR500": PTC.BleDataRate.Ble500LR,
                },
            ),
            CommandAttributeInt(
                "MAPCLKIOPIN",
                ["Defines the GPIO number to map the clock to"],
                self.api,
                "gpio_clock_map_pin",
            ),
            CommandAttributeEnum[PTC.GpioClockMapType](
                "MAPCLKTYPE",
                ["Define the clock type to map to a GPIO"],
                self.api,
                "gpio_clock_map_type",
                {"32KHZ": PTC.GpioClockMapType.Map32kHz},
            ),
            CommandAttributeOnOff(
                "MIDLEVELAGCATT",
                ["Enable mid-level Rx LNA AGC attenuation during timeout"],
                self.api,
                "rx_lna_during_timeout",
            ),
            CommandAttributeOnOff("RXMS", ["Enable multi-standard Rx mode"], self.api, "rx_multi_standard"),
            CommandAttributeOnOff(
                "RXHS",
                ["Enable high-sensitivity Rx mode"],
                self.api,
                "rx_high_sensitivity",
            ),
            CommandAttributeOnOff("RXMC", ["Enable multi-channel Rx mode"], self.api, "rx_multi_channel"),
            CommandAttributeEnum[PTC.PdmClockSource](
                "PDMCLKSRC",
                ["Select the PDM clock source"],
                self.api,
                "pdm_map_clock_source",
                {
                    "CLK_2M": PTC.PdmClockSource.Pdm2M,
                    "CLK_PLL": PTC.PdmClockSource.PdmPLL,
                },
            ),
            CommandAttributeInt(
                "PDMCLKFREQ",
                [
                    "Select the PDM clock frequency in Hz",
                    "375733 to 192000000 for CLK_PLL and 2000000 for CLK_2M",
                ],
                self.api,
                "pdm_map_clock_frequency",
            ),
            CommandAttributeGpioNumber(
                "PDMCLKOUT",
                ["Select PDM clock output pin"],
                self.api,
                "pdm_map_pin",
            ),
            CommandAttributeInt(
                "XTALTRIMTEMP1",
                [
                    "The max temperature of the Xtal32M capacitor trimming algorithm's zone number 1.",
                    "Value range is from -40 to +125 (check the hysteresis) and should not overlap other zones",
                ],
                self.api,
                "xtaltrimtemp1",
            ),
            CommandAttributeInt("XTALTRIMTEMP2", ["... zone 2 max temperature"], self.api, "xtaltrimtemp2"),
            CommandAttributeInt("XTALTRIMTEMP3", ["... zone 3 max temperature"], self.api, "xtaltrimtemp3"),
            CommandAttributeInt("XTALTRIMTEMP4", ["... zone 4 max temperature"], self.api, "xtaltrimtemp4"),
            CommandAttributeInt("XTALTRIMTEMP5", ["... zone 5 max temperature"], self.api, "xtaltrimtemp5"),
            CommandAttributeInt("XTALTRIMTEMP6", ["... zone 6 max temperature"], self.api, "xtaltrimtemp6"),
            CommandAttributeInt(
                "XTALTRIMSET1",
                ["Setting for the capacitance register corresponding to the temperature zone number 1"],
                self.api,
                "xtaltrimset1",
            ),
            CommandAttributeInt("XTALTRIMSET2", ["... zone 2 setting"], self.api, "xtaltrimset2"),
            CommandAttributeInt("XTALTRIMSET3", ["... zone 3 setting"], self.api, "xtaltrimset3"),
            CommandAttributeInt("XTALTRIMSET4", ["... zone 4 setting"], self.api, "xtaltrimset4"),
            CommandAttributeInt("XTALTRIMSET5", ["... zone 5 setting"], self.api, "xtaltrimset5"),
            CommandAttributeInt("XTALTRIMSET6", ["... zone 6 setting"], self.api, "xtaltrimset6"),
            CommandAttributeInt("XTALTRIMSET7", ["... zone 7 setting"], self.api, "xtaltrimset7"),
            CommandAttributeInt(
                "WSETCHANNEL", ["Set the channel to which the power will be set"], self.api, "wsetchannel"
            ),
            CommandAttributeInt("WSETPOWER", ["Set Tx power of a specific channel"], self.api, "wsetpower"),
            CommandAttributeInt("WGETCHANNEL", ["Get Tx power of a specific channel"], self.api, "wgetchannel"),
            CommandAttributeInt("PCHSTATCHANNEL", ["Set channel of statistic print"], self.api, "pchstatchannel"),
        ]

        proto_modes = [
            ProtoModeOnOff(
                "SETCW",
                ["Transmit a continuous wave"],
                lambda x: self.api.start_tx_carrier_wave(duration_ms=x),
                self.api.stop_tx_carrier_wave,
                self.api.get_tx_carrier_wave,
                ["CW"],
            ),
            ProtoModeAction("INFO", ["Display device information"], self._console_device_info),
            ProtoModeAction(
                "RR",
                ["Read register value @ a certain address"],
                self._console_read_register,
                ["RRADDRESS"],
            ),
            ProtoModeAction(
                "WR",
                ["Write value to register"],
                self._console_write_register,
                ["WRADDRESS", "WRVALUE"],
            ),
            ProtoModeAction(
                "WSET",
                ["Apply channel specific power configuration"],
                self._console_set_power_to_channel,
                ["WSETCHANNEL", "WSETPOWER"],
            ),
            ProtoModeAction(
                "WGET",
                ["Get Tx power of a specific channel"],
                self._console_get_power_of_channel,
                ["WGETCHANNEL"],
            ),
            ProtoModeAction(
                "WRB",
                ["Write bits value to register"],
                self._console_write_register,
                ["WRADDRESS", "WRBITMASK", "WRVALUE"],
            ),
            ProtoModeOnOff(
                "SL",
                ["Put the DUT to sleep (time in ms)"],
                lambda x: self.api.sleep(time_ms=x),
                lambda: None,
                None,
            ),
            ProtoModeAction("WU", ["Wake up the device"], self._console_wakeup),
            ProtoModeOnOff(
                "ED",
                ["Do energy detect"],
                lambda x: self.api.do_energy_detect(None, None),
                self.api.stop_energy_detect,
                None,
                ["SCANCOUNT", "SCANINTERVAL"],
            ),
            ProtoModeOnOff(
                "TX",
                ["Transmit predefined packet"],
                lambda x: self.api.start_tx_packets(),
                self.api.stop_tx_packets,
                None,
                ["PACKETCOUNT", "PACKETINTERVAL"],
            ),
            ProtoModeOnOff(
                "TXR",
                ["Transmit random packets"],
                lambda x: self.api.start_tx_rf4ce_random_data(duration_ms=x),
                lambda: None,
                None,
                ["PACKETCOUNT", "PACKETINTERVAL", "PACKETLENGTH"],
            ),
            ProtoModeOnOff(
                "RX",
                ["Set the receive mode"],
                lambda x: self.api.timed_rx_mode(duration_ms=x or 0),
                self.api.disable_rx_mode,
                self.api.get_rx_mode,
            ),
            ProtoModeOnOff(
                "RXP",
                ["Print received packets"],
                lambda x: self.api.indicate_received_packets(True),
                lambda: self.api.indicate_received_packets(False),
                None,
            ),
            ProtoModeAction("R", ["Reset Rx/Tx counters"], self.api.reset_packet_counters),
            ProtoModeAction("P", ["Print Rx/Tx packet count"], self._console_print_statistics),
            ProtoModeAction(
                "PCH",
                ["Print Rx/Tx packet count for specified channel"],
                self._console_print_statistics_for_channel,
                ["PCHSTATCHANNEL"],
            ),
            ProtoModeAction(
                "PCHA", ["Print Rx/Tx packet count for all channel"], self._console_print_statistics_for_all_channel
            ),
            ProtoModeAction(
                "RCH",
                ["Reset statistics packet count for specified channel"],
                self._console_reset_statistics_for_channel,
                ["PCHSTATCHANNEL"],
            ),
            ProtoModeOnOff(
                "SETCLKONGPIO",
                ["Map a clock type to a GPIO"],
                lambda x: self.api.start_map_clock_to_gpio(),
                self.api.stop_map_clock_to_gpio,
                None,
                ["MAPCLKIOPIN", "MAPCLKTYPE"],
            ),
            ProtoModeOnOff(
                "XTALSELFTEST",
                ["Runs a sleep mode xtal self test (32kHz)"],
                lambda x: self.api.start_xtal_self_test(),
                self.api.stop_xtal_self_test,
                None,
            ),
            ProtoModeOnOff(
                "SETPDMCLK",
                ["Enable the PDM clock at a given frequency on the selected pin"],
                lambda x: self.api.start_pdm_clock(),
                self.api.stop_pdm_clock,
                None,
                ["PDMCLKSRC", "PDMCLKFREQ", "PDMCLKOUT"],
            ),
            ProtoModeAction(
                "SETXTALTRIMCAP",
                ["Configure the Xtal32M capacitor trimming algorithm"],
                self.api.set_xtal_trim_cap,
                [
                    "XTALTRIMTEMP1",
                    "XTALTRIMSET1",
                    "XTALTRIMTEMP2",
                    "XTALTRIMSET2",
                    "XTALTRIMTEMP3",
                    "XTALTRIMSET3",
                    "XTALTRIMTEMP4",
                    "XTALTRIMSET4",
                    "XTALTRIMTEMP5",
                    "XTALTRIMSET5",
                    "XTALTRIMTEMP6",
                    "XTALTRIMSET6",
                    "XTALTRIMSET7",
                ],
            ),
            ProtoModeOnOff(
                "TXMC",
                ["Transmit on multiple channels"],
                lambda x: self.api.enable_tx_multi_channels(),
                self.api.disable_tx_multi_channels,
                None,
                ["TXMC_CHSEQ", "TXMC_CHANNEL", "TXMC_PACKETCOUNT", "TXMC_SETTXDATA", "TXMC_PACKETINTERVAL"],
                self._console_print_txmc_configuration,
            ),
        ]

        ptc_modes: List[ConsoleCommand] = []
        for pmode in proto_modes:
            if isinstance(pmode, ProtoModeOnOff):
                ptc_modes.append(
                    CommandModeOnOff(
                        pmode.name,
                        pmode.help_text,
                        pmode.on_func,
                        pmode.off_func,
                        pmode.getvalue_func,
                        [next(cmd for cmd in ptc_commands if cmd.name == attr) for attr in pmode.attribute_list or []],
                        pmode.extra_action_on_args,
                    )
                )
            elif isinstance(pmode, ProtoModeAction):
                ptc_modes.append(
                    CommandModeAction(
                        pmode.name,
                        pmode.help_text,
                        pmode.action,
                        [next(cmd for cmd in ptc_commands if cmd.name == attr) for attr in pmode.attribute_list or []],
                    )
                )
            else:
                raise ValueError(f"Cannot convert type {type(pmode)} to {ConsoleCommand.__name__}")
        ptc_commands.extend(ptc_modes)

        if user_commands:
            ptc_commands.extend(user_commands)

        super().__init__(app, ptc_commands)

    def promote_to_connection_owner(self) -> None:
        self.api.own_connector()

    def get_device_info(self) -> DeviceInfo:
        return self.api.get_device_info()

    def start(self) -> ...:
        self.api.connect()

    def stop(self) -> ...:
        self.api.disconnect()

    def set_gplog_callback(self, callback: Callable[[float, str], None] | None) -> None:
        self._printf_logger.stop()
        self._printf_logger.log_action = callback
        if callback is not None:
            self._printf_logger.start(self.api.device.peakConnector)

    def set_indication_message_callback(self, callback: IndicationMessageCallback) -> None:
        self.api.set_indication_message_callback(callback)

    def _indication_message_cb(self, message: BaseIndication) -> None:
        if isinstance(message, RxPacketIndication):
            payload_list = [f"{x:02X}" for x in message.payload]
            self._app.show_user_message(
                f"RX Packet: {' '.join(payload_list)}, len: {len(payload_list)} and {len(message.payload)}"
            )
        elif isinstance(message, EdScanIndication):
            self._app.show_user_message(f"ED {message.value}")
        else:
            self._app.show_user_message(f"Indication: {message}!")

    # Mode Actions
    def _console_device_info(self) -> List[str]:
        info = self.get_device_info()
        return [
            f"802.15.4 MAC Address: {info.mac_address}",
            f"BLE Address: {info.ble_address}",
            f"APP version: {info.app_version} - Commit: {int(info.changelist):x}",
            f"PTC Version: {info.ptc_version}",
            f"API Version: {info.api_version if info.api_version != API_VERSION_UNKNOWN else '?'}",
            f"Target: {info.product_name} - {info.product_id}",
            f"Chip Version: {info.chip_version}",
        ]

    def _console_read_register(self) -> List[str]:
        value = self.api.read_register(None)
        # \Components\Qorvo\Test\vlatest\test\regcheck_wrapper.py -> prefetch_regdump
        return [f"ReadRegister value : {value} - 0x{value:02x} - 0b{value:08b}"]

    def _console_write_register(self) -> None:
        self.api.write_register(None, None)

    def _console_set_power_to_channel(self) -> None:
        self.api.set_power_to_channel(None, None)

    def _console_get_power_of_channel(self) -> List[str]:
        value = self.api.get_power_of_channel(None)
        return [f"Channel power value : {value}"]

    def _console_wakeup(self) -> None:
        self.api.wakeup()

    def _console_print_statistics(self) -> List[str]:
        value = self.api.get_packet_statistics()
        return [
            f"Avg RSSI        : {value.avg_rssi}",
            f"Avg LQI         : {value.avg_lqi}",
            f"RX Packet Count : {value.rx_packet_count}",
            f"TX OK           : {value.tx_ok}",
            f"TX FAIL         : {value.tx_fail}",
        ]

    def _console_print_statistics_for_channel(self) -> List[str]:
        stat_list = self.api.get_packet_statistics_for_channel(None)
        return [tabulate(stat_list, headers=["Channel", "RSSI", "LQI", "RX Packet Cnt", "TX OK", "TX FAIL"])]

    def _console_print_statistics_for_all_channel(self) -> List[str]:
        all_channel_list = list(range(11, 27))
        stat_list = self.api.get_packet_statistics_for_channel(all_channel_list)
        return [tabulate(stat_list, headers=["Channel", "RSSI", "LQI", "RX Packet Cnt", "TX OK", "TX FAIL"])]

    def _console_reset_statistics_for_channel(self) -> None:
        self.api.reset_packet_statistics_for_channel(None)

    def _console_print_txmc_configuration(self, args: List[str]) -> Optional[List[str]]:
        value = self.api.get_txmc_configuration(args)
        if value is not None:
            return [
                f"channel: {value.channel}",
                f"packet_count: {value.packet_count}",
                f"data_length: {value.data_length}",
                f"packet_interval: {value.packet_interval}",
            ]
        return None
