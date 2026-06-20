import abc
import enum
import logging
import select
import socket
import time
import traceback
from typing import Callable, Dict, List, Optional, Type

from serial import Serial

from qorvo.rcc.rcc_types import connection

from .threading import ProcessThread

CONNECTION_TIMEOUT = 3
logger = logging.getLogger(__name__)


class SerialConnectionException(Exception):
    ...


class Protocol(enum.Enum):
    PEAK = 0
    RAW = 1


def crc16(crc: int, byte: int) -> int:
    crc = crc ^ byte
    for _ in range(0, 8):
        if (crc & 0x0001) == 0x0001:
            crc = (crc >> 1) ^ 0xA001
        else:
            crc = crc >> 1
    return crc


class GenericConnection(abc.ABC):
    def __init__(self, message_callback: Callable[[List[int]], None] | None = None, logging_suffix: str = ""):
        self.message_callback = message_callback or (lambda x: None)
        self.logger = logger.getChild(logging_suffix)

    @abc.abstractmethod
    def write(self, data: List[int]):
        ...

    @abc.abstractmethod
    def read(self):
        ...

    @abc.abstractmethod
    def start(self):
        ...

    @abc.abstractmethod
    def stop(self):
        ...


############################
# Protocol Connector types #
############################
class GenericConnector(abc.ABC):
    def __init__(self, connection: GenericConnection, logging_suffix: str = ""):
        self.logger = logger.getChild(logging_suffix)
        self.name = "GENERIC"

        self.IsStarted = False
        self.connection = connection
        self.connection.message_callback = self._parse_protocol
        self.thread = ProcessThread(self.connection.read, name=self.__class__.__name__)

        self.indicateCallbacks: List[Callable[[int, List[int]], None]] = []
        self.writeCallbacks: List[Callable[[int, List[int]], None]] = []

        # Debug
        self.droppedBuffer: List[int] = []

        # Connection params
        self.conn_param = ""

    @abc.abstractmethod
    def _parse_protocol(self, data: List[int]) -> None:
        ...

    @abc.abstractmethod
    def Start(self):
        ...

    @abc.abstractmethod
    def Stop(self):
        ...

    def __str__(self):
        s = "%s" % self.name
        return s


class peakConnector(GenericConnector):
    def __init__(self, connection: GenericConnection, wakeup: bool = False):
        super().__init__(connection)
        self.name = "PEAK"
        # Add Wake up using extra pre-amble byte
        self.wakeup = wakeup
        self.packetData: List[int] = []
        self.enforce_crc = True

    def Start(self):
        if not self.IsStarted:
            self._ResetProtocol()
            self._ResetPacket()
            self.connection.start()
            self.thread.Start()
            self.IsStarted = True
        else:
            self.logger.info("Already running")

    def Stop(self):
        if self.IsStarted:
            self.thread.Stop()
            try:
                while not self.thread.IsDone():
                    pass
            except Exception as e:
                print("Ignoring exception %s" % str(e))
            self.IsStarted = False
            self._ResetProtocol()
            self._ResetPacket()
            self.connection.stop()
        else:
            self.logger.info("Already stopped")

    def WriteData(self, moduleId: int, data: List[int]) -> None:
        if len(data) >= (1 << 12):  # 8 + 4 bit length field
            raise Exception("Packet too long %d >= %d" % (len(data), (1 << 12)))
        # SYN|len|res|modId|data
        # SYN = 0x53, 0x59, 0x4E

        if self.wakeup:
            # wakeup on RX
            # Send pre-amble byte first, wait and let the chip wake-up, then send the actual data (eg: wa 0x94 dbgu_app)
            preamble = [0xA5]
            self.connection.write(preamble)
            time.sleep(0.002)

        lenLSB = len(data) & 0xFF
        lenMSB = (len(data) >> 8) & 0x0F  # Only using lower 4 bits

        message: List[int] = []
        message += [ord("S"), ord("Y"), ord("N"), lenLSB, lenMSB, moduleId]
        message += data

        crc = 0
        for byte in message:
            crc = self._updateCRC(crc, byte)
        message += [crc & 0xFF, (crc >> 8) & 0xFF]

        # self.log(message)
        self.connection.write(message)

        for writeCb in self.writeCallbacks:
            writeCb(moduleId, data)

    def _parse_protocol(self, data: List[int]) -> None:
        for byte in data:
            if self.protocolState == 0:
                if self.protocolCount == 0:
                    if byte == ord("S"):  # gpCom_ProtocolHeaderS
                        # Reset packet
                        self._ResetPacket()
                        self.droppedBuffer.clear()
                        self.protocolCount += 1
                    else:
                        if byte != 0:
                            self.droppedBuffer.append(byte)
                        self._ResetProtocol()
                        continue
                elif self.protocolCount == 1:  # gpCom_ProtocolHeaderY
                    if byte == ord("Y"):
                        self.protocolCount += 1
                    else:
                        self._ResetProtocol()
                        continue
                elif self.protocolCount == 2:  # gpCom_ProtocolHeaderN
                    if byte == ord("N"):
                        self.protocolCount += 1
                    else:
                        self._ResetProtocol()
                        continue
                elif self.protocolCount == 3:  # gpCom_ProtocolHeaderLength
                    self.packetLength = byte
                    self.protocolCount += 1
                elif self.protocolCount == 4:  # gpCom_ProtocolHeaderFrameControl
                    # top 4 bits length
                    self.packetLength += (byte & 0x0F) << 8
                    self.protocolCount += 1
                elif self.protocolCount == 5:  # gpCom_ProtocolHeaderModule
                    self.packetModuleID = byte
                    self.protocolCount = 0
                    self.protocolState += 1
                else:
                    self._ResetProtocol()
                    continue
                # Update CRC calculation
                self.crcCalculated = self._updateCRC(self.crcCalculated, byte)
            elif self.protocolState == 1:  # data
                if self.packetLength:
                    self.packetData.append(byte)
                    self.packetLength -= 1
                if self.packetLength == 0:
                    self.protocolState += 1
                    self.protocolCount = 0
                # Update CRC calculation
                self.crcCalculated = self._updateCRC(self.crcCalculated, byte)
            elif self.protocolState == 2:  # CRC
                # Skip 2 bytes
                if self.protocolCount == 0:
                    self.protocolCount += 1
                    if byte != (self.crcCalculated & 0xFF) and self.enforce_crc:
                        self._ResetProtocol()
                        continue
                elif self.protocolCount == 1:
                    self.protocolCount += 1
                    if byte != ((self.crcCalculated >> 8) & 0xFF):
                        self.logger.debug("CRC fail (MSB) %02X <> %04X" % (byte, self.crcCalculated))
                        if self.enforce_crc:
                            self._ResetProtocol()
                            continue
                if self.protocolCount == 2:
                    # Finish off
                    for ind_cb in self.indicateCallbacks:
                        try:
                            ind_cb(self.packetModuleID, self.packetData)
                        except Exception:
                            traceback.print_exc()
                    # Start again
                    self._ResetProtocol()
                    continue

    def _ResetProtocol(self):
        self.protocolCount = 0
        self.protocolState = 0
        self.crcCalculated = 0
        self._ResetPacket()

    def _updateCRC(self, crc: int, byte: int) -> int:
        crc = crc16(crc, byte)
        return crc

    def _ResetPacket(self):
        self.packetLength = 0
        self.packetModuleID = 0xFF
        self.packetData.clear()


########################
# HW Connection types
########################
class serialConnection(GenericConnection):
    def __init__(self, port: str = "COM3", baudrate: int = 57600):
        super().__init__()
        self.comport = port
        self.baudrate = baudrate
        self.name = self.comport
        self.port = Serial(timeout=0.001)

    def __open(self) -> None:
        try:
            self.port.port = self.comport
            self.port.baudrate = self.baudrate
            self.port.open()
        except Exception as exc:
            raise SerialConnectionException(f"Connection to {self.comport} failed.") from exc

    def read(self) -> None:
        # Timeout set in init
        rx = self.port.read(64)
        if len(rx):
            self.message_callback(list(rx))

    def write(self, data: List[int]):
        # make sure we don't write while another thread is re-initing this connection
        self.port.write(bytes(data))

    def start(self):
        if not self.port.is_open:
            self.__open()

    def stop(self):
        if self.port.is_open:
            self.port.close()


class socketConnection(GenericConnection):
    def __init__(
        self,
        path: str | None = None,
        host: str | None = None,
        port: int | None = None,
        type: str | None = None,
    ):
        super().__init__()
        self.path = path
        self.host = host
        self.port = port
        self.type = type
        # Keep a local buffer of all data - to avoid TCP nacking and buffer overflows on transmitter side
        self.rx_buffer: List[int] = []
        self.socket: socket.socket

    def _Connect(self):
        if self.host is not None and self.port is not None:
            self.name = "%s:%s" % (self.host, self.port)
            if self.type is None or self.type.lower() == "tcp":
                self.name += ":tcp"
                self.socket = self._ConnectTcp(self.host, self.port)
            else:
                raise Exception("Unknown type for %s - %s" % (self.name, self.type))
        else:
            raise Exception("Need something, host:port<:udp/tcp> or path")
        self.socket.setblocking(False)

    def ReConnect(self):
        self.stop()
        self._Connect()

    def _ConnectTcp(self, host: str, port: int, connectionTimeout: Optional[int] = None):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # Disable Nagle
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        s.settimeout(connectionTimeout or CONNECTION_TIMEOUT)
        s.connect((host, port))
        return s

    def read(self):
        readable, _, _ = select.select([self.socket], [], [], 0)  # timeout of 0 means just a 'peek'
        for s in readable:
            try:
                # Append received bytes to buffer (use large value to be able to effciently receive any TCP packet size)
                self.rx_buffer += s.recv(65536)
            except Exception:
                # ignore closed socket
                pass

        # Distribute data to framework when there are still bytes in the buffer
        if len(self.rx_buffer):
            # Pass data to the test framework in chunks (to avoid long delays before reading again)
            chunk_size = min(2000, len(self.rx_buffer))

            # Get the chunk and update the buffered data
            rx = self.rx_buffer[0:chunk_size]
            self.rx_buffer = self.rx_buffer[chunk_size:]
            self.logger.debug(f"Received packet: {rx}")
            self.message_callback(rx)

    def write(self, data: List[int]):
        self.socket.send(bytes(data))

    def start(self):
        self._Connect()

    def stop(self):
        self.socket.shutdown(socket.SHUT_RDWR)
        self.socket.close()


###########################
# Peak Connector flavors
###########################
class peakConnectorSerial(peakConnector):
    def __init__(self, port: str, baudrate: int):
        super().__init__(serialConnection(port, baudrate))


class peakConnectorSocket(peakConnector):
    def __init__(
        self,
        path: str | None = None,
        host: str | None = None,
        port: int | None = None,
        type: str | None = None,
    ):
        super().__init__(socketConnection(path, host, port, type))


def ConnectAndStart(
    conninfo: str,
    protocol: Protocol = Protocol.PEAK,
    start: bool = True,
) -> GenericConnector:
    """Returns a PeakConnector object based on commandline style type selection

    :param conninfo: contains info on connection to set up. Parameters given by : split list
        syntax dependent on different types:
        tcp/udp:
            'host:port'
            'host:port:type'
        serial:
            'COMx:baudrate'
            '/dev/ttySx:baudrate'
        usb:
            'usbt:path'
    :type conninfo: str
    :param protocol: Protocol to be parsed on connection - PeakProtocol, BLE or raw stream
    :type protocol: number
    :param start: 'True' if returned connector object needs to be started, default True
    :type start: bool, optional
    """
    connectors: Dict[Protocol, Dict[str, Type[GenericConnector]]] = {
        Protocol.PEAK: {
            "Serial": peakConnectorSerial,
            "Socket": peakConnectorSocket,
        },
    }

    if protocol not in connectors:
        raise Exception("Unknown protocol %s - options %s" % (protocol, connectors.keys()))

    parameters = conninfo.split(":")
    type_connection = parameters[0]

    if any([conninfo.find(substr) != -1 for substr in ["COM", "tty", "pts", "ptmx", "/dev/serial"]]):
        port = type_connection
        if len(parameters) > 1:
            baudrate = parameters[1]
        else:
            baudrate = 57600
        if "Serial" not in connectors[protocol]:
            raise Exception("Serial connector not supported in protocol %s" % protocol)
        peakConnector = connectors[protocol]["Serial"](port=port, baudrate=baudrate)
    else:
        host = type_connection
        port = 9190
        type = None
        if len(parameters) > 1:
            port = parameters[1]
            port = int(port)
        if len(parameters) > 2:
            type = parameters[2]
        if "Socket" not in connectors[protocol]:
            raise Exception("Socket connector not supported in protocol %s" % protocol)
        peakConnector = connectors[protocol]["Socket"](host=host, port=port, type=type)

    if start:
        peakConnector.Start()

    peakConnector.conn_param = conninfo

    return peakConnector


def create_peak_connector(conn_args: connection.ConnectionArguments) -> peakConnector:
    """Attempt to create a `peakConnector` from the supplied arguments. Does not auto-start.

    :param conn_args: The connection arguments.
    :raises ValueError: When no coehssive connection information was provided.
    :return: The created connector.
    """
    if conn_args.eth:
        if not conn_args.port:
            conn_args.port = connection.DEFAULT_TCP_IP_PORT
        conn = peakConnectorSocket(host=conn_args.eth, port=conn_args.port)
    elif conn_args.com:
        if not conn_args.baud:
            conn_args.baud = connection.DEFAULT_SERIAL_BAUDRATE
        conn = peakConnectorSerial(port=conn_args.com, baudrate=conn_args.baud)
    else:
        raise ValueError("No cohesive connection information.")
    conn.conn_param = str(conn_args)
    return conn
