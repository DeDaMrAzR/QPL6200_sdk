"""PTC gpAPI Wrapper Class"""

import abc
import codecs
import logging
import struct
from dataclasses import dataclass
from enum import Enum
from typing import Any, List, Optional, Type, TypeVar, Union

from ...rcc_types.deviceinfo import API_VERSION_UNKNOWN, DeviceInfo
from ...rcc_types.generic import IndicationMessageCallback
from ...rcc_types.property_attribute import (
    PropertyAttributeDriver,
    property_attribute_bool,
    property_attribute_bytes,
    property_attribute_enum,
    property_attribute_int,
)
from ...transport.gpAPI_2 import (
    gpAPI,
    gpAPI_Generic,
    gpAPIConfirmIndication,
    int2Bytelist,
    list2int,
)
from ...transport.peakconnect import ConnectAndStart, peakConnector
from . import gpPTCAPI


def gpPTC_FormatSpecial(parName: Any, paramValue: Any, cmdDef: Any) -> str:
    return ""


T = TypeVar("T")

# Logging
logger = logging.getLogger(__name__)


class BaseIndication(abc.ABC):
    cmd_id: int
    _IndicationT = TypeVar("_IndicationT", bound="BaseIndication")

    @classmethod
    def factory(cls: Type["BaseIndication"], value: gpAPIConfirmIndication) -> Optional["BaseIndication"]:
        ind_class = next(
            (klass for klass in cls.__subclasses__() if klass.cmd_id == value.cmdId),
            None,
        )
        if ind_class is None:
            return None
        return ind_class.from_api(value)

    @classmethod
    @abc.abstractmethod
    def from_api(cls: Type[_IndicationT], value: gpAPIConfirmIndication) -> _IndicationT:
        ...


class RxPacketIndication(BaseIndication):
    cmd_id = gpPTCAPI.gpPTC_RXPacketIndication_CmdId

    @classmethod
    def from_api(cls: Type[BaseIndication._IndicationT], value: gpAPIConfirmIndication) -> BaseIndication._IndicationT:
        val = value.parList[2]
        return cls(val.to_bytes(length=(max(val.bit_length(), 1) + 7) // 8, byteorder="little"))

    def __init__(self, payload: bytes):
        self.payload = payload


class EdScanIndication(BaseIndication):
    cmd_id = gpPTCAPI.gpPTC_EDConfirm_CmdId

    @classmethod
    def from_api(cls: Type[BaseIndication._IndicationT], value: gpAPIConfirmIndication) -> BaseIndication._IndicationT:
        val = value.parList[2]
        return cls(val)

    def __init__(self, value: int):
        self.value = value


@dataclass
class PacketStatistics:
    avg_rssi: int
    avg_lqi: int
    rx_packet_count: int
    tx_ok: int
    tx_fail: int


# =============================================================================
#  Base PTC Driver
# =============================================================================
class _BasePTC(abc.ABC):
    @classmethod
    def new_connection(cls, connection_str: str, debug_level: int = logging.DEBUG) -> "PTC":
        conn = ConnectAndStart(connection_str)
        obj = cls(conn, debug_level)
        return obj

    @abc.abstractmethod
    def _indication_cb(self, dev: gpAPI_Generic, ind: gpAPIConfirmIndication) -> None:
        """DUT indication callback function."""
        ...

    def __init__(self, connector: peakConnector, debug_level: int = logging.DEBUG) -> None:
        self.device = gpAPI(
            connector,
            gpPTCAPI.gpPTC_Commands,
            formatParseSpecial=(gpPTC_FormatSpecial, None),
            processCallback=self._indication_cb,
        )
        self.debug_level = debug_level
        self.device.verbose = False
        if self.device.commChannel:
            self.device.commChannel.verbose = False
        self.connect()

    def own_connector(self) -> None:
        """Capture ownership of the connector."""
        self.device.own_connector()

    def connect(self) -> None:
        """Creates a connection with the DUT."""
        self.device.startInput()

    def disconnect(self) -> None:
        self.device.stopInput()

    def clear_state(self) -> None:
        ...

    def set_attribute(
        self,
        attrib: int,
        value: int,
        is_bytedata: bool = False,
    ) -> Union[gpAPIConfirmIndication, None]:
        if is_bytedata:
            cmdid = gpPTCAPI.gpPTC_SetByteDataForAttributeRequest_CmdId
            datastr = self._padded_hex_str(value)
            ret = self.device.Request(
                cmdid,
                [
                    0x00,  # client id
                    "0x" + self._padded_hex_str(attrib),  # attrib ID
                    len(datastr),  # data_len
                    False,  # is null ptr
                    "0x" + datastr,
                ],
            )
        else:
            cmdid = gpPTCAPI.gpPTC_SetAttributeRequest_CmdId
            parsed_value = (
                "0x"
                + self._padded_hex_str(self._fix_signedness_and_mask(value, width=32, signed=False, mask=0xFFFFFFFF))
                + self._padded_hex_str(self._fix_signedness_and_mask(attrib, width=8, mask=0xFF))
            )

            ret = self.device.Request(
                cmdid,
                [
                    0x00,  # client id
                    1,  # number of attrs
                    False,  # is null ptr
                    parsed_value,
                ],
            )

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != cmdid
            or ret.cmdDef.parValues["result"] != gpPTCAPI.gpPTC_ResultSuccess
        ):
            code_str = gpPTCAPI.gpPTC_Result_t[ret.cmdDef.parValues["result"]]
            raise ValueError(
                "Wrong reply on set_attribute method: {}. Attribute: {} Value: {}".format(
                    code_str,
                    gpPTCAPI.gpPTC_AttributeId_t[attrib],
                    datastr if is_bytedata else parsed_value,
                )
            )

        return ret

    def get_attribute(self, attrib: int) -> int:
        ret = self.device.Request(
            gpPTCAPI.gpPTC_GetAttributeRequest_CmdId,
            [
                0x00,  # client id
                1,  # number of attrs
                False,  # is null ptr
                "0x" + self._padded_hex_str(0) + self._padded_hex_str(attrib),
            ],
        )

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != gpPTCAPI.gpPTC_GetAttributeRequest_CmdId
            or ret.cmdDef.parValues["result"] != gpPTCAPI.gpPTC_ResultSuccess
        ):
            code_str = gpPTCAPI.gpPTC_Result_t[ret.cmdDef.parValues["result"]]
            raise ValueError(f"Wrong reply on get_attribute method ({code_str})")

        # Unpacks attribute as signed int32, little endian
        list_ = ret.parBytes
        val = struct.unpack("<i", bytearray(list_[6 : 6 + 4]))[0]
        return val

    def set_mode(
        self, mode: int, etime: int, mtype: int, extra_params: int = 0, *params: int
    ) -> gpAPIConfirmIndication:
        lparam = len(params)
        params = [0x00]
        args = [
            0x00,  # client id
            mode,
            etime,  # exec time
            mtype,
            extra_params,  # num of extra params
            lparam == 0,  # is null
        ]
        args.extend(params)

        ret = self.device.Request(gpPTCAPI.gpPTC_SetModeRequest_CmdId, args)

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != gpPTCAPI.gpPTC_SetModeRequest_CmdId
            or ret.cmdDef.parValues["result"] != gpPTCAPI.gpPTC_ResultSuccess
        ):
            logger.debug(ret)
            code_str = gpPTCAPI.gpPTC_Result_t[ret.cmdDef.parValues["result"]]
            raise ValueError(f"Wrong reply on set_mode method ({code_str})")

        return ret

    def get_mode_state(self, mode: int) -> int:
        """Returns current state of given chip's mode."""
        args = [0x00, mode]
        res = self.device.Request(gpPTCAPI.gpPTC_GetModeRequest_CmdId, args)
        return res.cmdDef.parValues["OnOff"]

    @staticmethod
    def _padded_hex_str(value: Union[int, str]) -> str:
        if isinstance(value, str):
            if value.startswith("0x"):
                return value.lstrip("0x").rstrip("L").zfill(len(value) + len(value) % 2)
            else:
                raise ValueError("oops.")
        aval = hex(value).lstrip("0x").rstrip("L")
        alen = len(aval) or 8
        aval = aval.zfill(alen + alen % 2)
        return aval

    @staticmethod
    def _fix_signedness_and_mask(value: int, width: int = 8, signed: bool = False, mask: Optional[int] = None) -> int:
        """Assumes that mask is contiguous (ie. not 0x12, but 0x03)"""
        if width not in [8, 16, 32, 64]:
            raise ValueError("Incorrect width.")
        if not mask:
            mask = pow(2, width) - 1
        value &= mask
        if not signed:
            return value

        # Signing integer (using 2's complement)
        if value > (mask / 2):
            value ^= mask
            value += 1
            value *= -1
        return value


# =============================================================================
#  PTC Driver
# =============================================================================
class PTC(_BasePTC, PropertyAttributeDriver):
    def _indication_message_cb(self, message: BaseIndication) -> None:
        pass

    def _indication_cb(self, dev: gpAPI_Generic, ind: gpAPIConfirmIndication) -> None:
        """DUT indication callback function."""
        ind_obj = BaseIndication.factory(ind)
        if ind_obj is not None:
            self._indication_message_cb(ind_obj)

    def __init__(
        self,
        connector: peakConnector,
        debug_level: int = logging.DEBUG,
        indication_message_cb: Optional[IndicationMessageCallback] = None,
    ) -> None:
        super().__init__(connector, debug_level)
        if indication_message_cb is not None:
            self.set_indication_message_callback(indication_message_cb)
        # State cache
        self._txdata: Optional[int] = None

    def set_indication_message_callback(self, callback: IndicationMessageCallback) -> None:
        self._indication_message_cb = callback

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeChannel)
    def channel(self):
        """Device active channel."""

    class AntennaValues(Enum):
        A0 = 0
        A1 = 1
        AUTO = 2

    @property_attribute_enum(gpPTCAPI.gpPTC_AttributeAntenna, AntennaValues)
    def antenna(self):
        """Device active antenna."""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributeAntennaDiversity)
    def antenna_diversity(self):
        """Antenna diversity enabled flag."""

    class SleepModes(Enum):
        SleepRC = gpPTCAPI.gpPTC_SleepModeRC
        Sleep32kHz = gpPTCAPI.gpPTC_SleepMode32KHz
        Sleep32MHz = gpPTCAPI.gpPTC_SleepMode32MHz

    @property_attribute_enum(gpPTCAPI.gpPTC_AttributeSleepMode, SleepModes)
    def sleep_mode(self):
        """Device sleep mode."""

    class CSMAModes(Enum):
        NoCCA = gpPTCAPI.gpPTC_CSMAMode_NoCCA
        CCA = gpPTCAPI.gpPTC_CSMAMode_CCA
        CSMA = gpPTCAPI.gpPTC_CSMAMode_CSMA

    @property_attribute_enum(gpPTCAPI.gpPTC_AttributeCSMAMode, CSMAModes)
    def csma_mode(self):
        """Device CSMA mode."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeMAXBE)
    def max_mac_backoff_exponent(self):
        """Maximum MAC backoff exponent."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeMINBE)
    def min_mac_backoff_exponent(self):
        """Minimum MAC backoff exponent."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeMaxCSMABackoff)
    def max_csma_backoff_exponent(self):
        """Maximum CSMA backoff exponent."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeMaxMACRetries)
    def max_mac_retries(self):
        """Maximum number of MAC retries."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeTXPower)
    def power(self):
        """Device transmit power."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributePanID)
    def pan_id(self):
        """PAN ID."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeShortAddress)
    def short_address(self):
        """Device short address."""

    class ContinuousWaveMode(Enum):
        Modulated = gpPTCAPI.gpPTC_CWModulated
        Unmodulated = gpPTCAPI.gpPTC_CWUnModulated

    @property_attribute_enum(gpPTCAPI.gpPTC_AttributeCWMode, ContinuousWaveMode)
    def continuous_wave_mode(self):
        """Continuows wave mode."""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributePowerControlLoop)
    def power_control_loop_flag(self):
        """Output power control loop enabled."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeReadRegisterAddress)
    def read_register_address(self):
        """Address to read register value from."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeWriteRegisterAddress)
    def write_register_address(self):
        """Address to write register value to."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeWriteRegisterValue)
    def write_register_value(self):
        """Value to write to register."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeWriteRegisterBitsMask)
    def write_register_bitmask(self):
        """Register write bitmask to use."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributePacketIntervalInMS)
    def packet_interval_ms(self):
        """Packet interval in ms."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeScanIntervalInMS)
    def scan_interval_ms(self):
        """Scan interval in ms."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributePacketLength)
    def packet_length(self):
        """Packet length, in bytes."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributePacketCount)
    def packet_count(self):
        """Number of packets."""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeScanCount)
    def scan_count(self):
        """Number of scans during energy detect."""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributePacketInPacket)
    def packet_in_packet_flag(self):
        """Packet-in-packet mode enabled flag."""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributePromiscuousMode)
    def promiscuous_mode_flag(self):
        """Promiscuous mode enabled flag."""

    @property_attribute_bytes(gpPTCAPI.gpPTC_AttributeSetTxData, cached=True)
    def txdata(self):
        """Transmit packet payload."""

    class PhyMode(Enum):
        RF4CE = gpPTCAPI.gpPTC_RF4CE
        BLE = gpPTCAPI.gpPTC_BLE

    @property_attribute_enum(gpPTCAPI.gpPTC_AttributePhyMode, PhyMode)
    def phy(self):
        """Phy operational mode"""

    @property_attribute_int(gpPTCAPI.gpPTC_BLE_DTM_PacketType)
    def ble_dtm_packet_type(self):
        """BLE test packet type used in DTM Transmit"""

    class BleDataRate(Enum):
        Ble1M = 1
        Ble2M = 2
        Ble125LR = 3
        Ble500LR = 4

    @property_attribute_enum(gpPTCAPI.gpPTC_BLE_DataRate, BleDataRate)
    def ble_datarate(self):
        """BLE Data Rate used for Rx and Tx"""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributeMapClockIOPin)
    def gpio_clock_map_pin(self):
        """GPIO number which the CLOCK will be mapped to"""

    class GpioClockMapType(Enum):
        Map32kHz = gpPTCAPI.gpPTC_SleepMode32KHz

    @property_attribute_enum(gpPTCAPI.gpPTC_AttributeMapClockType, GpioClockMapType)
    def gpio_clock_map_type(self):
        """Clock to map to the GPIO"""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributeRxLnaAttDuringTimeout)
    def rx_lna_during_timeout(self):
        """Enable Mid-level Rx LNA AGC attenunation during timeout"""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributeRxMultiStandard)
    def rx_multi_standard(self):
        """Multi-standard listening mode enable flag"""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributeRxHighSensitivity)
    def rx_high_sensitivity(self):
        """High sensitivity listening mode enable flag"""

    @property_attribute_bool(gpPTCAPI.gpPTC_AttributeRxMultiChannel)
    def rx_multi_channel(self):
        """Multi-channel listening mode enable flag"""

    class PdmClockSource(Enum):
        Pdm2M = gpPTCAPI.gpPTC_PDMClkSrc_2M
        PdmPLL = gpPTCAPI.gpPTC_PDMClkSrc_PLL

    @property_attribute_enum(gpPTCAPI.gpPTC_AttributePDMClkSrc, PdmClockSource)
    def pdm_map_clock_source(self):
        """PDM clock output source"""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributePDMClkFreq)
    def pdm_map_clock_frequency(self):
        """PDM clock output frequency"""

    @property_attribute_int(gpPTCAPI.gpPTC_AttributePDMClkOutPin)
    def pdm_map_pin(self):
        """PDM clock output pin"""

    class ExecutionMode(Enum):
        On = gpPTCAPI.gpPTC_ModeExecution_On
        Off = gpPTCAPI.gpPTC_ModeExecution_Off
        Single = gpPTCAPI.gpPTC_ModeExecution_Single

    def execute_custom_command(self, cc_id: int, arg_list: List[int] = []) -> List[int]:
        cc_args = [cc_id]
        cc_args.extend(arg_list)
        args = [
            0x00,  # client id
            # https://stackoverflow.com/a/14329943
            len(cc_args),  # data len in
            False,  # is nullptr
            list2int(cc_args),
        ]

        ret = self.device.Request(gpPTCAPI.gpPTC_ExecuteCustomCommand_CmdId, args)

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != gpPTCAPI.gpPTC_ExecuteCustomCommand_CmdId
            or ret.cmdDef.parValues["result"] != gpPTCAPI.gpPTC_ResultSuccess
        ):
            raise ValueError("Wrong reply on execute_custom_command method")

        return int2Bytelist(ret.parList[6], ret.parList[4])

    def _read_register(self, address: Optional[int], width: int) -> int:
        """Recursively iterates register addresses if width > 8."""
        if address is not None:
            self.set_attribute(gpPTCAPI.gpPTC_AttributeReadRegisterAddress, address)
        ret = self.set_mode(
            gpPTCAPI.gpPTC_ModeReadRegister,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Single,
            1,
            0x00,
        )

        rval = ret.parList[5]

        if width > 8:
            return (self._read_register(address + 1, width - 8) << 8) + rval

        return rval

    def read_register(
        self,
        address: Optional[int],
        width: int = 8,
        signed: bool = False,
        mask: Optional[int] = None,
    ) -> int:
        """Reads a register.
        Assumes that mask is contiguous (ie. not 0x12, but 0x03)
        """
        if width not in [8, 16, 32]:
            raise ValueError("Incorrect width.")

        if mask is None:
            if width == 8:
                mask = 0xFF
            elif width == 16:
                mask = 0xFFFF
            elif width == 32:
                mask = 0xFFFFFFFF

        rval = self._read_register(address, width)

        rval &= mask

        # now, on the topmost call apply the corrections
        if not signed:
            return rval

        # apply 2's complement
        if rval > (mask / 2):
            rval ^= mask
            rval += 1
            rval *= -1

        return rval

    def write_register(self, address: Optional[int], value: Optional[int]) -> None:
        """Writes byte value to register."""
        if address is not None:
            self.write_register_address = address
        if value is not None:
            self.write_register_value = value
        # Mode
        self.set_mode(
            gpPTCAPI.gpPTC_ModeWriteRegister,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Single,
            1,
            0x00,
        )

    def write_register_with_bitmask(self, address: Optional[int], value: Optional[int], bitmask: Optional[int]) -> None:
        if address is not None:
            self.write_register_address = address
        if value is not None:
            self.write_register_value = value
        if bitmask is not None:
            self.write_register_bitmask = bitmask
        # Mode
        self.set_mode(
            gpPTCAPI.gpPTC_ModeWriteRegisterBits,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Single,
            1,
            0x00,
        )

    def sleep(self, mode: Optional[SleepModes] = None, time_ms: Optional[int] = None) -> None:
        if mode is not None:
            self.sleep_mode = mode
        self.set_mode(
            gpPTCAPI.gpPTC_ModeSleep,
            time_ms or 0,
            gpPTCAPI.gpPTC_ModeExecution_On,
            0,
            0x00,
        )

    def wakeup(self) -> None:
        self.set_mode(gpPTCAPI.gpPTC_ModeWakeUp, 0, gpPTCAPI.gpPTC_ModeExecution_On, 0, 0x00)

    def do_energy_detect(self, scan_count: Optional[int], scan_interval_ms: Optional[int]) -> None:
        if scan_count is not None:
            self.scan_count = scan_count
        if scan_interval_ms is not None:
            self.scan_interval_ms = scan_interval_ms
        self.set_mode(
            gpPTCAPI.gpPTC_ModeEDScan,
            0,
            gpPTCAPI.gpPTC_ModeExecution_On,
            0,
            0x00,
        )

    def stop_energy_detect(self) -> None:
        self.set_mode(
            gpPTCAPI.gpPTC_ModeEDScan,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Off,
            0,
            0x00,
        )

    def reset_device(self) -> None:
        """Request device to execute a software reset."""
        cmdid = gpPTCAPI.gpPTC_ResetDUT_CmdId
        ret = self.device.Request(cmdid, [])
        if ret.parList[0] != gpAPI.AckStatus_Succes or ret.parList[1] != cmdid:
            raise ValueError(f"Wrong reply on reset_device method: {ret}")

    def get_device_api_version(self) -> int:
        """Get device API version

        :return: API version reported by device.
        """
        cmdid = gpPTCAPI.gpPTC_GetDUTApiVersion_CmdId
        ret = self.device.Request(cmdid, [])
        if ret.parList[0] != gpAPI.AckStatus_Succes or ret.parList[1] != cmdid:
            return API_VERSION_UNKNOWN

        return ret.cmdDef.parValues["version"]

    def get_device_info(self) -> DeviceInfo:
        """Returns a dict with raw dump of device info."""
        cmdid: int = gpPTCAPI.gpPTC_GetDUTInfoRequest_CmdId
        ret = self.device.Request(
            cmdid,
            [
                0x00,  # client id
            ],
        )

        if ret.parList[0] != gpAPI.AckStatus_Succes or ret.parList[1] != cmdid:
            raise ValueError(f"Wrong reply on get_device_info method: {ret}")

        list_ = ret.parBytes
        dval = ret.cmdDef.parValues

        mac_address = self._parse_mac_addr(list_)
        ble_address = self._parse_ble(list_)
        ptc_version = ".".join(str(dval[f"ptcVersion_{x}"]) for x in ("major", "minor", "revision", "patch"))
        app_version = ".".join(str(dval[f"appVersion_{x}"]) for x in ("major", "minor", "revision", "patch"))
        cl = str(dval["appVersion_changelist"])
        chip = self._parse_chip_version(list_)
        product_id = self._hex_to_str(dval["productID_name"])
        product_name = self._hex_to_str(dval["productName_name"])
        api_version = self.get_device_api_version()

        return DeviceInfo(
            mac_address,
            ble_address,
            ptc_version,
            app_version,
            cl,
            chip,
            product_id,
            product_name,
            api_version,
        )

    def enable_rx_mode(self) -> None:
        self.timed_rx_mode(duration_ms=0)

    def disable_rx_mode(self) -> None:
        self.set_mode(gpPTCAPI.gpPTC_ModeRx, 0, gpPTCAPI.gpPTC_ModeExecution_Off, 0, 0x00)

    def timed_rx_mode(self, duration_ms: int = 0) -> None:
        """Sets chip to Rx mode for "duration_sec" seconds. If set to 0, mode lasts indefinitely."""
        self.set_mode(
            gpPTCAPI.gpPTC_ModeRx,
            duration_ms,  # Mode duration in ms
            gpPTCAPI.gpPTC_ModeExecution_On,
            0,
            0x00,
        )

    def get_rx_mode(self) -> bool:
        return self.get_mode_state(gpPTCAPI.gpPTC_ModeRx) == gpPTCAPI.gpPTC_ModeExecution_On

    def indicate_received_packets(self, enable: bool) -> None:
        self.set_mode(
            gpPTCAPI.gpPTC_ModePrintReceivedPackets,
            0,
            gpPTCAPI.gpPTC_ModeExecution_On if enable else gpPTCAPI.gpPTC_ModeExecution_Off,
            0,
            0x00,
        )

    def start_tx_carrier_wave(
        self,
        phy: Optional[PhyMode] = None,
        channel: Optional[int] = None,
        antenna: Optional[AntennaValues] = None,
        power: Optional[int] = None,
        duration_ms: Optional[int] = None,
        modulated: Optional[ContinuousWaveMode] = None,
    ) -> None:
        """Starts transmiting carrier wave. If duration_ms == 0, transmission lasts indefinitely.
        Defaults to continuous un-modulated transmission.
        """
        if phy is not None:
            if self.phy != phy:
                self.phy = phy
        # Setting up required attributes
        if channel is not None:
            self.channel = channel
        if antenna is not None:
            self.antenna = antenna
        if power is not None:
            self.power = power
        if modulated is not None:
            self.continuous_wave_mode = modulated

        # Sets mode to transmit carrier wave
        self.set_mode(
            gpPTCAPI.gpPTC_ModeCarrierWave,
            (duration_ms or 0),  # Mode duration in ms
            gpPTCAPI.gpPTC_ModeExecution_On,
            0,
            0x00,
        )
        if duration_ms:
            logger.debug(f"Transmitting carrier wave for {duration_ms / 1000} seconds")
        else:
            logger.debug("Transmitting CW indefinately")

    def stop_tx_carrier_wave(self) -> None:
        """Stops carrier wave transmission."""
        logger.debug("Stopping carrier wave transmission.")
        self.set_mode(gpPTCAPI.gpPTC_ModeCarrierWave, 0, gpPTCAPI.gpPTC_ModeExecution_Off, 0, 0x00)

    def get_tx_carrier_wave(self) -> bool:
        """Get sthe current state of carrier wave transmition"""
        value = self.get_mode_state(gpPTCAPI.gpPTC_ModeCarrierWave)
        return value == gpPTCAPI.gpPTC_ModeExecution_On

    def start_tx_rf4ce_random_data(
        self,
        channel: Optional[int] = None,
        antenna: Optional[AntennaValues] = None,
        power: Optional[int] = None,
        length: Optional[int] = None,
        count: Optional[int] = None,
        interval_ms: Optional[int] = None,
        duration_ms: Optional[int] = None,
    ) -> None:
        """Transmits random packets on RFCE channel."""
        # Setting up required attributes
        if antenna is not None:
            self.antenna = antenna
        self.antenna_diversity = False
        if channel is not None:
            self.channel = channel
        self.disable_rx_mode()
        if power is not None:
            self.power = power

        if length is not None:
            self.packet_length = length
        if count is not None:
            self.packet_count = count
        if interval_ms is not None:
            if interval_ms < 5:
                logger.info("Minimum value for TXRF4CE interval should be 5 ms. Setting 5 ms.")
                interval_ms = 5
            self.packet_interval_ms = interval_ms

        # Sets mode to transmit random packets
        self.set_mode(
            gpPTCAPI.gpPTC_ModeTransmitRandomPackets,
            duration_ms or 0,
            gpPTCAPI.gpPTC_ModeExecution_On,
            0,
            0x00,
        )

        if (duration_ms or 0) > 0:
            logger.info(
                f"Transmitting packets {count} packets of length {length} and interval of {interval_ms} ms for {duration_ms} seconds."
            )
        else:
            logger.info(f"Transmitting packets {count} packets of length {length} and interval of {interval_ms} ms.")

    def start_tx_packets(
        self,
        phy: Optional[PhyMode] = None,
        channel: Optional[int] = None,
        antenna: Optional[AntennaValues] = None,
        power: Optional[int] = None,
        count: Optional[int] = None,
        interval_ms: Optional[int] = None,
    ) -> None:
        """Transmit packets on selected phy channel."""
        if phy is not None:
            if self.phy != phy:
                self.phy = phy
        # Setting up required attributes
        if channel is not None:
            self.channel = channel
        if antenna is not None:
            self.antenna = antenna
        if power is not None:
            self.power = power

        if count is not None:
            self.packet_count = count
        if interval_ms is not None:
            if interval_ms < 5:
                logger.info("Minimum value for TXRF4CE interval should be 5 ms. Setting 5 ms.")
                interval_ms = 5
            self.packet_interval_ms = interval_ms

        self.set_mode(
            gpPTCAPI.gpPTC_ModeTransmitPacket,
            0,
            gpPTCAPI.gpPTC_ModeExecution_On,
            0,
            0x00,
        )

    def stop_tx_packets(self) -> None:
        """Stops packet transmission."""
        self.set_mode(
            gpPTCAPI.gpPTC_ModeTransmitPacket,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Off,
            0,
            0x00,
        )

    def get_packet_statistics(self) -> PacketStatistics:
        """Returns a dict with raw dump of packet statistics."""
        # Setting mode for printing packet count with 5 extra parameters
        ret = self.set_mode(
            gpPTCAPI.gpPTC_ModePrintPacketCount,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Single,
            5,
            0x00,
        )

        # Unpacking data as signed int32, little endian
        list_ = ret.parBytes
        avg_rssi = struct.unpack("<i", bytearray(list_[5 : 5 + 4]))[0]
        avg_lqi = struct.unpack("<i", bytearray(list_[9 : 9 + 4]))[0]
        rx_packet_count = struct.unpack("<i", bytearray(list_[13 : 13 + 4]))[0]
        tx_ok = struct.unpack("<i", bytearray(list_[17 : 17 + 4]))[0]
        tx_fail = struct.unpack("<i", bytearray(list_[21 : 21 + 4]))[0]

        return PacketStatistics(avg_rssi, avg_lqi, rx_packet_count, tx_ok, tx_fail)

    def reset_packet_counters(self) -> None:
        """Resets PTC packet counters."""
        self.set_mode(
            gpPTCAPI.gpPTC_ModeResetCounters,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Single,
            0,
            0x00,
        )

    def start_map_clock_to_gpio(
        self, map_pin: Optional[int] = None, map_type: Optional[GpioClockMapType] = None
    ) -> None:
        """Maps a clock type to a GPIO."""
        if map_pin is not None:
            self.gpio_clock_map_pin = map_pin
        if map_type is not None:
            self.gpio_clock_map_type = map_type
        self.set_mode(
            gpPTCAPI.gpPTC_ModeSetClockToGPIO,
            0,
            gpPTCAPI.gpPTC_ModeExecution_On,
            1,
            0x00,
        )

    def stop_map_clock_to_gpio(self) -> None:
        self.set_mode(
            gpPTCAPI.gpPTC_ModeSetClockToGPIO,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Off,
            1,
            0x00,
        )

    def start_xtal_self_test(self, sleep_mode: Optional[SleepModes] = None) -> None:
        """Runs a sleep mode xtal self test (32Khz)."""
        if sleep_mode is not None:
            self.sleep_mode = sleep_mode
        self.set_mode(gpPTCAPI.gpPTC_ModeXtalSelfTest, 0, gpPTCAPI.gpPTC_ModeExecution_On, 1, 0x00)

    def stop_xtal_self_test(self) -> None:
        self.set_mode(
            gpPTCAPI.gpPTC_ModeXtalSelfTest,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Off,
            1,
            0x00,
        )

    def start_pdm_clock(self) -> None:
        """Enable the PDM clock at a given frequency on the selected pin"""
        self.set_mode(
            gpPTCAPI.gpPTC_ModeSetPdmClock,
            0,
            gpPTCAPI.gpPTC_ModeExecution_On,
            1,
            0x00,
        )

    def stop_pdm_clock(self) -> None:
        self.set_mode(
            gpPTCAPI.gpPTC_ModeSetPdmClock,
            0,
            gpPTCAPI.gpPTC_ModeExecution_Off,
            1,
            0x00,
        )

    @staticmethod
    def _parse_mac_addr(list_: List[int]) -> str:
        """Parses MAC address obtained via gpAPI's parBytes list."""
        assert isinstance(list_, list), "Parameter must be a list"

        # unpacks mac address (8 bytes long, little endian)
        mac_addr_unpacked = struct.unpack("<8B", bytearray(list_[6 : 6 + 8]))

        # reverses raw list of mac address' bytes, removes hex header and fills with two 0's
        mac_addr_raw = [hex(item).lstrip("0x").zfill(2) for item in mac_addr_unpacked][::-1]

        # returns mac address string with proper formatting
        mac_addr = ":".join(mac_addr_raw).upper()
        return mac_addr

    @staticmethod
    def _parse_ble(list_: List[int]) -> str:
        """Parses BLE address obtained via gpAPI's parBytes list."""
        assert isinstance(list_, list), "Parameter must be a list"

        # unpacks ble address (6 bytes long, little endian)
        ble_addr_unpacked = struct.unpack("<6B", bytearray(list_[15 : 15 + 6]))

        # reverses raw list of ble address' bytes, removes hex header and fills with two 0's
        ble_addr_raw = [hex(item).lstrip("0x").zfill(2) for item in ble_addr_unpacked][::-1]

        # returns ble address string with proper formatting
        ble_addr = ":".join(ble_addr_raw).upper()
        return ble_addr

    @staticmethod
    def _parse_chip_version(list_: List[int]) -> str:
        """Parses chip version obtained via gpAPI's parBytes list."""
        assert isinstance(list_, list), "Parameter must be a list"

        # unpacks chip version (2 byte long, little endian)
        chip_ver_unpacked = struct.unpack("<2B", bytearray(list_[31 : 31 + 2]))

        # builds list of characters
        chip_ver_raw = (chip_ver_unpacked[0] << 8) + chip_ver_unpacked[1]

        # returns chip version with proper formatting
        chip_ver = hex(chip_ver_raw).lstrip("0x").zfill(4).upper()

        return chip_ver

    @staticmethod
    def _hex_to_str(value: int) -> str:
        value_str = str(hex(value)).lstrip("0x").rstrip("L")
        return codecs.getdecoder("hex")(value_str)[0][::-1].decode("ascii")
