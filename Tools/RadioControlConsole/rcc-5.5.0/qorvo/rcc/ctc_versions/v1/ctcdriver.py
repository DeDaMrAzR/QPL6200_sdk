"""CTC gpAPI Wrapper Class
Expects that required paths are already present in os.path
"""

import codecs
import datetime
import os
import struct
import sys
from functools import partial

from ...transport.gpAPI_2 import gpAPI, int2Bytelist, list2int
from . import gpCTCAPI


def gpCTC_FormatSpecial(parName, paramValue, cmdDef):
    return ""


import logging

logger = logging.getLogger(__name__)


# =============================================================================
#  CTC Logger
# =============================================================================
class CTCLogger:
    """CTC logger overrides a few functions from jadelogger to control the
    verbosity of the logging messages.
    """

    def __init__(self, peak_connector, logger, debug_level=0):
        ...

    def _LoggingSeen(self, module_id, data):  # noqa N802
        ...

    def _TxSeen(self, module_id, data):  # noqa N802
        ...


# =============================================================================
#  CTC Driver
# =============================================================================
class CTC(object):
    def __init__(self, connection_str=None, debug_level=0):
        self.device = None
        self.connection_str = connection_str
        self.debug_level = debug_level
        if self.connection_str:
            self.device = self._create_connection(self.connection_str)

        # Dynamically create set and get methods for attributes
        # Using set_attribute and get_attribute as templates
        # Skips attributes on skip_attribs list
        # Methods are created as set_antenna, get_antenna
        skip_attribs = ["CTC_ATTRIBUTES_MAX_NUMBER", "gpCTC_AttributeSetTxData"]
        for attribute_key, attribute_name in gpCTCAPI.gpCTC_AttributeId_t.items():
            if attribute_name in skip_attribs:
                continue
            set_method_name = "set_{}".format(attribute_name.replace("gpCTC_Attribute", "").lower())
            get_method_name = "get_{}".format(attribute_name.replace("gpCTC_Attribute", "").lower())
            set_method = partial(self.set_attribute, attribute_key)
            get_method = partial(self.get_attribute, attribute_key)
            setattr(self, set_method_name, set_method)
            setattr(self, get_method_name, get_method)
            getattr(self, set_method_name).__name__ = set_method_name
            getattr(self, get_method_name).__name__ = get_method_name

        for mode_key, mode_name in gpCTCAPI.gpCTC_ModeId_t.items():
            if mode_name in skip_attribs:
                continue
            get_method_name = "get_{}".format(mode_name.replace("gpCTC_Mode_Set_", "mode_").lower())
            get_method = partial(self.get_mode_state, mode_key)
            setattr(self, get_method_name, get_method)

        # documentation for attributes
        self.set_priority.__doc__ = "Set the priority of the Coex mode."
        self.set_request.__doc__ = "Set the request enable for the coex mode."
        self.set_attlna_low.__doc__ = "No documentation available."
        self.set_attlna_high.__doc__ = "No documentation available."
        self.set_txnotgrantedaction.__doc__ = "Mask that sets the MAC packet TX actions upon not having grant."
        self.set_txacknotgrantedaction.__doc__ = "Mask that sets the MAC ACK TX actions upon not having grant."
        self.set_gaincontrolmode.__doc__ = "Enumeration specifying the gain control mode."
        self.set_rxreqexttrigger.__doc__ = "Mask that sets request extension triggers."
        self.set_ble_notgrantedaction.__doc__ = "Mask that sets the BLE actions upon not having grant."
        self.set_ble_rx_window_prio.__doc__ = "Set the Priority coex of RX window during RX."
        self.set_ble_rx_window_req.__doc__ = "Set the Request enable coex of RX window."
        self.set_ble_rx_packet_prio.__doc__ = "Set the Priority applied on RX."
        self.set_ble_rx_packet_req.__doc__ = "Set the Request enable for applied when actual RX is busy."
        self.set_ble_tx_packet_prio.__doc__ = "Set the Priority for transmitting a packet in BLE."
        self.set_ble_tx_packet_req.__doc__ = "Set the Request enable for transmitting BLE packet."
        self.set_ble_txrx_packet_prio.__doc__ = "Set the Priority coex of TX/RX packet."
        self.set_ble_txrx_packet_req.__doc__ = "Set the Request enable of TX/RX packet."

        self.set_count.__doc__ = "No documentation available."
        self.set_exttimeout.__doc__ = "No documentation available."
        self.set_rxreqextpriority.__doc__ = "No documentation available."

    def connect(self, connection_str):
        """Creates a connection with the DUT."""
        if self.device:
            self.disconnect()
        self.device = self._create_connection(connection_str)

    def disconnect(self):
        self.device.stopInput()
        self.device.logger.Stop()

    def _create_connection(self, connection_str):
        self.connection_str = connection_str
        device = gpAPI(
            gpCTCAPI.gpCTC_Commands,
            formatParseSpecial=[gpCTC_FormatSpecial, None],
            parList=[connection_str],
            processCallback=self._indication_cb,
        )
        device.startInput()
        device.verbose = False
        device.commChannel.verbose = False

        # Add jadeLogger
        device.logger = CTCLogger(device.peakConnector, logging.getLogger("CTC_DUT"), self.debug_level)
        device.logger.Start()

        return device

    def _indication_cb(self, dev, ind):
        """DUT indication callback function."""
        logger.debug("DUT indication received.")
        pass

    def set_attribute(self, attrib, value, is_bytedata=False):
        if is_bytedata:
            cmdid = gpCTCAPI.gpCTC_SetByteDataForAttributeRequest_CmdId
            datastr = self.padded_hex_str((value))
            ret = self.device.Request(
                cmdid,
                [
                    0x00,  # client id
                    "0x" + self.padded_hex_str(attrib),  # attrib ID
                    int(len(datastr) / 2),  # data_len
                    False,  # is null ptr
                    "0x" + datastr,
                ],
            )
        else:
            cmdid = gpCTCAPI.gpCTC_SetAttributeRequest_CmdId
            parsed_value = (
                "0x"
                + self.padded_hex_str(self.fix_signedness_and_mask(value, width=32, signed=False, mask=0xFFFFFFFF))
                + self.padded_hex_str(self.fix_signedness_and_mask(attrib, width=8, mask=0xFF))
            )

            ret = self.device.Request(
                cmdid,
                [0x00, 1, False, parsed_value],  # client id  # number of attrs  # is null ptr
            )

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != cmdid
            or ret.cmdDef.parValues["result"] != gpCTCAPI.gpCTC_ResultSuccess
        ):
            # pdb.set_trace()
            raise ValueError(
                "Wrong reply on set_attribute method: {}.\n\
                Attribute: {}\n Value: {}".format(
                    ret, gpCTCAPI.gpCTC_AttributeId_t[attrib], datastr if is_bytedata else parsed_value
                )
            )

        return ret

    def get_attribute(self, attrib):
        ret = self.device.Request(
            gpCTCAPI.gpCTC_GetAttributeRequest_CmdId,
            [
                0x00,  # client id
                1,  # number of attrs
                False,  # is null ptr
                "0x" + self.padded_hex_str(0) + self.padded_hex_str(attrib),
            ],
        )

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != gpCTCAPI.gpCTC_GetAttributeRequest_CmdId
            or ret.cmdDef.parValues["result"] != gpCTCAPI.gpCTC_ResultSuccess
        ):
            # pdb.set_trace()
            raise ValueError("Wrong reply on get_attribute method")

        return ret.cmdDef.parValues["result"]

    def set_mode(self, mode, etime, mtype, extra_params=0, *params):
        # Avoids turning on a mode multiple times, regardless of execution time
        # Single execution of a mode still passes!

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

        ret = self.device.Request(gpCTCAPI.gpCTC_SetModeRequest_CmdId, args)

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != gpCTCAPI.gpCTC_SetModeRequest_CmdId
            or ret.cmdDef.parValues["result"] != gpCTCAPI.gpCTC_ResultSuccess
        ):
            # pdb.set_trace()
            logger.error(ret)
            raise ValueError("Wrong reply on set_mode method")

        return ret

    def get_mode_state(self, mode):
        """Returns current state of chip's mode."""
        args = [0x00, mode]
        res = self.device.Request(gpCTCAPI.gpCTC_GetModeRequest_CmdId, args)
        return res.cmdDef.parValues["OnOff"]

    def execute_custom_command(self, cc_id, arg_list=[]):
        lparam = len(arg_list)
        cc_args = [cc_id]
        cc_args.extend(arg_list)
        cc_args.append(lparam + 2)

        args = [
            0x00,  # client id
            lparam + 2,  # data len in
            False,  # is nullptr
            list2int(cc_args),
        ]

        ret = self.device.Request(gpCTCAPI.gpCTC_ExecuteCustomCommand_CmdId, args)

        if (
            ret.cmdDef.parValues["AckResult"] != gpAPI.AckStatus_Succes
            or ret.cmdDef.parValues["cmdId"] != gpCTCAPI.gpCTC_ExecuteCustomCommand_CmdId
            or ret.cmdDef.parValues["result"] != gpCTCAPI.gpCTC_ResultSuccess
        ):
            raise ValueError("Wrong reply on execute_custom_command method")

        return int2Bytelist(ret.parList[6], ret.parList[4])

    def _read_register(self, address, width):
        """Recursively iterates register addresses if width > 8."""
        ret = self.set_attribute(gpCTCAPI.gpCTC_AttributeReadRegisterAddress, address)
        ret = self.set_mode(gpCTCAPI.gpCTC_ModeReadRegister, 0, gpCTCAPI.gpCTC_ModeExecution_Single, 1, 0x00)

        rval = ret.parList[5]

        if width > 8:
            return (self._read_register(address + 1, width - 8) << 8) + rval

        return rval

    def read_register(self, address, width=8, signed=False, mask=None):
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

    def _set_write_register_value(self, value):
        """Sets the value to write to a register."""
        self.set_attribute(gpCTCAPI.gpCTC_AttributeWriteRegisterValue, self.fix_signedness_and_mask(value, signed=True))

    def write_register(self, address, value):
        """Writes byte value to register."""
        # Address
        self.set_attribute(gpCTCAPI.gpCTC_AttributeWriteRegisterAddress, address)
        # Value
        self._set_write_register_value(value)
        # Mode
        self.set_mode(gpCTCAPI.gpCTC_ModeWriteRegister, 0, gpCTCAPI.gpCTC_ModeExecution_Single, 1, 0x00)

    # This method is really slow! Need to find a better method to request
    # all attributes directly on the request command
    def get_device_attributes(self):
        """Returns a dict with raw dump of device attributes."""
        attributes = {}
        for key, value in gpCTCAPI.gpCTC_AttributeId_t.items():
            attributes[value] = self.get_attribute(key)

        # Skip CTC_ATTRIBUTES_MAX_NUMBER
        del attributes["CTC_ATTRIBUTES_MAX_NUMBER"]

        return attributes

    def get_device_info(self):
        """Returns a dict with raw dump of device info."""
        cmdid = gpCTCAPI.gpCTC_getDUTInfoRequest_CmdId
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

        mac_address = self.parse_mac_addr(list_)
        ble_address = self.parse_ble(list_)
        ctc_version = self.parse_ctc_version(list_)
        cl = "CL:{}".format(str(dval["appVersion_changelist"]))
        chip = self.parse_chip_version(list_)
        product_id = self.hex_to_str(dval["productID_name"])
        product_name = self.hex_to_str(dval["productName_name"])

        info = {
            "mac_addr": mac_address,
            "ble_addr": ble_address,
            "ctc_version": ctc_version,
            "app_version": f"{ctc_version} {cl}",
            "chip": chip,
            "product_id": product_id,
            "product_name": product_name,
        }

        return info

    @staticmethod
    def padded_hex_str(value):
        if isinstance(value, str) and value.startswith("0x"):
            return value.lstrip("0x").rstrip("L").zfill(len(value) + len(value) % 2)
        aval = hex(value).lstrip("0x").rstrip("L")
        alen = len(aval)
        aval = aval.zfill(alen + alen % 2)
        return aval

    @staticmethod
    def fix_signedness_and_mask(value, width=8, signed=False, mask=None):
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

    @staticmethod
    def parse_mac_addr(list_):
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
    def parse_ble(list_):
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
    def parse_ctc_version(list_):
        """Parses CTC version obtained via gpAPI's parBytes list."""
        assert isinstance(list_, list), "Parameter must be a list"

        # unpacks ctc version (4 bytes long, little endian)
        ctc_ver_unpacked = struct.unpack("<4B", bytearray(list_[22 : 22 + 4]))

        # builds list of characters
        ctc_ver_raw = [format(item) for item in ctc_ver_unpacked]

        # returns ble address string with proper formatting
        ctc_ver = ".".join(ctc_ver_raw).upper()
        return ctc_ver

    @staticmethod
    def parse_chip_version(list_):
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
    def hex_to_str(value):
        value_str = str(hex(value)).lstrip("0x").rstrip("L")
        return codecs.getdecoder("hex")(value_str)[0][::-1].decode("ascii")
