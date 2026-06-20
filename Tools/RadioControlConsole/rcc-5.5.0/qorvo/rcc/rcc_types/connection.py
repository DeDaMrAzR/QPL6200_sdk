# -*- coding: utf-8 -*-
"""This module provides connection types."""

from dataclasses import dataclass
from typing import Optional

DEFAULT_TCP_IP_PORT = 9192
"""The default TCP/IP port used for connection."""
DEFAULT_SERIAL_BAUDRATE = 57600
"""The default serial baudrate used for connection."""


@dataclass
class ConnectionArguments:
    """This container holds the possible connection arguments for a peakConnect device."""

    com: Optional[str]
    """The COM port or /dev/tty serial port to connect to."""
    baud: Optional[int]
    """The baudrate to talk with the device."""
    eth: Optional[str]
    """The hostname or IP of the device."""
    port: Optional[int]
    """The remote TCP port of the device."""
