# -*- coding: utf-8 -*-
"""This module provides base abstract CLI interaction commands."""

import abc
from enum import Enum
from typing import List, Optional, Sequence, Tuple, TypeVar

T = TypeVar("T")
Tenum = TypeVar("Tenum", bound=Enum)


class ConsoleCommand(abc.ABC):
    """This class represents a command on the CLI."""

    def __init__(self, name: str) -> None:
        """Interface class that represents a console command. A command may have arguments.

        :param name: The which this command is registered as.
        """
        self.name = name

    @abc.abstractmethod
    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """

    @abc.abstractmethod
    def execute_console_line(self, args: List[str]) -> Optional[List[str]]:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """

    @abc.abstractmethod
    def get_help(self) -> List[str]:
        """Get the help text for this command.

        :return: A list of help text lines.
        """

    @abc.abstractmethod
    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """


class CommandAttribute(ConsoleCommand):
    """Generic interface for PTC/CTC attributes."""


class CommandMode(ConsoleCommand):
    """Class that represents a mode that behaves as a command."""

    def __init__(
        self,
        name: str,
        has_state: bool,
        attribute_list: Optional[List[CommandAttribute]] = None,
    ) -> None:
        """Generic interface for PTC/CTC commands.

        :param name: The name of this mode.
        :param has_state: If this mode has state (e.g. RX) or behaves like a command (e.g. RR)
        :param attribute_list: Optional attributes linked to this mode, defaults to None.
        :param kwargs: Extra arguments used by the child classes.
        """
        super().__init__(name)
        self.has_state = has_state
        self._attr_list = attribute_list or []

    def _execute_sub_attributes(self, args: Sequence[str]) -> None:
        """Parse and process the attributes tied to this mode.

        :param args: A list of supplied argument (attribute) values.
        :raises ValueError: If too many argument values were supplied.
        """
        # Execute the extra attribute values, if any
        if len(args) > len(self._attr_list):
            raise ValueError("Too many extra attributes provided")

        for attribute, value in zip(self._attr_list, args):
            attribute.execute_console_line([value])
