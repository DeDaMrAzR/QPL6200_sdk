#!/usr/bin/env python3
"""
    jadelogger.py <connection> <options>
    - connection:
        tcp/udp:
            'host:port'        - TCP connection to 'host' on 'port'
            'host:port:type'   - choose the type of connection using tcp/udp
        serial:
            'COMx:baudrate'    - serial connection to COM# with baudrate
                                baudrate optional - defaults to 57600
        usb: 'usb:path'       - targetted HID usb device (use COM for usb2serial connections)
                                empty path gives a list of possible HID devices
        pc: 'pc:host:baudrate' - TCP connection to platform controller
                                connected to UART of device
        testbed: 'testbed'     - log all the available COM# which are mbed (QPG6095 Evaluation boards)
    - options:
        'raw' - prints without any SYN protocol
        'ble' - uses the BLE-UART protocol
        'file=<file_prefix>'- route output to a logfile - <file_prefix>_YYMMDD_HHMMSS.txt or log_YYMMDD_HHMMSS.txt when
            nothing is specified
        'reconnect' - keeps trying to connect until successful. Useful in case a USB connection is only available after
            some delay.
    Examples:

    To use jadelogger on COM1 - 57600 baudrate:
        'jadelogger.py COM1'
    To use jadelogger on COM1 - 57600 baudrate in combination with BLE:
        'jadelogger.py COM1 ble'
    To use jadelogger on a USB HID device - unique path ab3245ef:
        'jadelogger.py usb:ab3245ef'
    To use jadelogger on a network board:
        'jadelogger.py db05pc100:9190'
    To use jadelogger as logger.py:
        'jadelogger.py db05pc100:9240:udp raw'
    To use jadelogger with platform controller - baudrate 115200:
        'jadelogger.py pc:db09pc62:115200'
    To use jadelogger with platform controller - baudrate 57600 - PLATFCTRL setting:
        'jadelogger.py pc'
    To use jadelogger within the test environment:
        'jadelogger.py testbed'
"""
import datetime
import logging
import sys
import time
from typing import Callable, List, Optional, Tuple

from .peakconnect import peakConnector

logger = logging.getLogger(__name__)


def _parse_gplog_message(log_packet: List[int]) -> str:
    """Parse gpLog format string.

    Format string and parameters to format
    Example in code:
        GP_LOG_SYSTEM_PRINTF("Some numbers:%u, %lu", 0, 100, 256);
    Incoming string:
        "Some numbers:%u, %lu", /0, |100, 0|, |1, 1, 0, 0|

    :param logPacket: incoming gpLog string
    :type logPacket: str
    """
    format_string = bytes(log_packet[0 : log_packet.index(0)]).decode("ASCII")

    # Get parameters
    parameters = log_packet[log_packet.index(0) + 1 :]
    params: List[int] = []
    specifier_index = format_string.find("%")
    while len(parameters):
        if specifier_index != -1 and len(parameters) > 1:
            param = parameters.pop(0)
            param += parameters.pop(0) << 8
            spec = format_string[specifier_index : specifier_index + 2]
            if spec == "%l" and len(parameters) > 1:
                param += parameters.pop(0) << 16
                param += parameters.pop(0) << 24
                spec = format_string[specifier_index : specifier_index + 3]
            # Create negative complement number in python for signed prints if needed
            if spec in ("%d", "%i"):
                if param & (1 << 15):
                    param -= 1 << 16
            elif spec == "%li":
                if param & (1 << 31):
                    param -= 1 << 32
            params.append(param)
        specifier_index = format_string.find("%", specifier_index + 1)
    try:
        if params:
            format_params = [f"params[{i}]" for i in range(len(params))]
            formatted = eval("'''" + format_string + "'''") % (eval(",".join(format_params)))
        else:
            formatted = eval("'''" + format_string + "'''")
    except Exception:
        formatted = format_string + "\n [%s]" % params

    return formatted


def process_gplog_packet(data: List[int]) -> Optional[Tuple[int, int, float, str]]:
    """Parse a gLog printf indication

    :param data: incoming gpCom level data
    :type data: str
    """

    # Parse Log header
    # 0x02 | module ID | endianness | reserved | count | time0 | time1 | time2 | time3 |
    log_packet = data[:]
    packet_type, comp_id, _, _, cnt, _t1, _t2, _t3, _t4, *message = log_packet
    if packet_type != 0x02:
        logger.debug("Received a non-printf packet")
        return None

    timestamp = _t1 + (_t2 << 8) + (_t3 << 16) + (_t4 << 24)
    timestamp = (timestamp * 32.0) / 1000000

    # Format formatstring
    formatted = _parse_gplog_message(message)

    return (comp_id, cnt, timestamp, formatted)


class PrintfLogger:
    """
    Class to handle GP logging
    - peakconnector = connection to accept logging from
    """

    def __init__(self, log_action: Optional[Callable[[float, str], None]]):
        self.peakconnector = None
        self.log_action = log_action

        # Functions to be called when logging comes in
        self.log_string_indication: List[Callable[[str], None]] = []

    def start(self, peakconnector: peakConnector):
        self.peakconnector = peakconnector
        logger.info("== Connecting PrintfLogger to %s ==", self.peakconnector)
        self.peakconnector.indicateCallbacks.append(self._logging_seen)

    def stop(self):
        if self.peakconnector is not None and self._logging_seen in self.peakconnector.indicateCallbacks:
            self.peakconnector.indicateCallbacks.remove(self._logging_seen)

    def _logging_seen(self, module_id: int, data: List[int]) -> None:
        """Parse incoming data from connector used with this logger.
        Non-logging data will be logged as raw hex values.
        Registered with connector as indication callback.

        :param moduleId: Id of SW component that is sending the data.
        :param data: list of incoming bytes, to be interpreted as logging
        """
        # The gpLog component sends data with module id 0x0B
        if module_id != 0x0B:
            return

        # Logging output
        log_packet = process_gplog_packet(data)

        if log_packet is None:
            return

        [_, _, rx_time, log] = log_packet

        # Signal any registered log monitors
        if self.log_action:
            self.log_action(rx_time, log)


class Jadelogger:
    """
    Class to handle GP logging
    - peakconnector = connection to accept logging from
    - logfile = send logging to file iso stdout
    - .LogStringIndication - add functions to perform further handling
    """

    def __init__(self, peakconnector: peakConnector, logfile: Optional[str] = None):
        self.peakconnector = peakconnector
        # File to log to
        self.logfile = logfile

        # Functions to be called when logging comes in
        self.log_string_indication: List[Callable[[str], None]] = []

        # Output handle - stdout/file
        self.out = None
        logger.info("== Connecting to %s - logfile: %s ==", peakconnector, logfile)

        self.starttime = time.time()

    def HoldLog(self):
        pass

    def start(self):
        if self.logfile is not None:
            # Open file
            self.out = open(self.logfile, "w")
        else:
            self.out = sys.stdout
        if not self.peakconnector.IsStarted:
            self.peakconnector.Start()
        self.peakconnector.indicateCallbacks.append(self._logging_seen)
        self.peakconnector.writeCallbacks.append(self._tx_seen)

    def stop(self):
        if self._logging_seen in self.peakconnector.indicateCallbacks:
            self.peakconnector.indicateCallbacks.remove(self._logging_seen)
        if self._tx_seen in self.peakconnector.writeCallbacks:
            self.peakconnector.writeCallbacks.remove(self._tx_seen)

        if self.out is not None and self.logfile is not None:
            # Close file
            self.out.close()
        self.out = None

    def _get_timestamp_str(self) -> str:
        x = datetime.datetime.now()
        return "%02u-%02u-%02u-%02u:%02u:%02u.%03u " % (
            x.year,
            x.month,
            x.day,
            x.hour,
            x.minute,
            x.second,
            x.microsecond / 1000,
        )

    def _logging_seen(self, module_id: int, data: List[int]) -> None:
        """Parse incoming data from connector used with this logger.
        Non-logging data will be logged as raw hex values.
        Registered with connector as indication callback.

        :param moduleId: Id of SW component that is sending the data.
        :type moduleId: digit
        :param data: list of incoming bytes, to be interpreted as logging
        :type data: list
        """
        rx_time = time.time() - self.starttime

        # gpCom parsing
        if module_id == 0xA and len(data) >= 2 and data[0] == 0x02:
            # Overflow occurred
            log = f"!Overflow:{data[1]} mess missed"
        elif module_id == 0xB:  # gpLog parsing
            # Logging output
            log_packet = process_gplog_packet(data)
            if log_packet is None:
                return
            [comp_id, _, rx_time, log] = log_packet

            module_id = comp_id
            log = " " + log
        elif module_id == 0x0:  # Raw connector - no module Id present
            # Raw ASCII output
            # Convert any non-printable characters with repr()
            log = " " + bytes(data).decode("ASCII").strip("'")

            # Control characters made explicit by repr() call
            # Strip again for actual print
            log = log.replace("\\r", "", 1)
        # HCI traffic
        # (expecting at least 5 bytes: packet_type, event_id, length, subevent_id, one or more payload bytes)
        elif module_id == 0x9C and len(data) >= 5:
            packet_type = data[0]
            # HCI event
            if packet_type == 0x04:
                # events
                event_id = data[1]
                # VSD subevent for logging (data[3] is subevent code, which is 0 for logging)
                if event_id == 0xFF and data[3] == 0:
                    log_packet = process_gplog_packet(data[4:])
                    if log_packet is None:
                        return
                    [comp_id, _, rx_time, log] = log_packet
                    module_id = comp_id
                    log = " " + log
                else:
                    log = "<[" + ",".join((f"{x:0>2X}" for x in data)) + "]"
                    module_id = data[0]
            else:
                log = "<[" + ",".join((f"{x:0>2X}" for x in data)) + "]"
        else:
            # Raw hex data output
            log = "<[" + ",".join((f"{x:0>2X}" for x in data)) + "]"

        logging_str = "%02X %04.6f%s" % (module_id, rx_time, log)

        output = self._get_timestamp_str() + " " + logging_str.strip("\r") + "\n"

        if self.out:
            self.out.write(output)
            self.out.flush()

        # Signal any registered log monitors
        for ind in self.log_string_indication:
            ind(output)

    def _tx_seen(self, module_id: int, data: List[int]) -> None:
        """Log any data sent over connector.
        Registered with connector as write callback.

        :param moduleId: Id of SW component that data is sent to.
        :type moduleId: digit
        :param data: list of outgoing bytes, to be logged as raw hex bytes
        :type data: list
        """
        tx_time = time.time() - self.starttime
        logstr = "%02X %04.6f>[%s]" % (
            module_id,
            tx_time,
            ",".join(map(lambda x: "%02X" % x, data)),
        )

        output = self._get_timestamp_str() + " " + logstr + "\n"

        if self.out:
            self.out.write(output)
            self.out.flush()

        # Signal any registered log monitors
        for ind in self.log_string_indication:
            ind(output)
