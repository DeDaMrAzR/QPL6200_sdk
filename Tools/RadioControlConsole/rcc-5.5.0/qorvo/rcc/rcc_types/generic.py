# -*- coding: utf-8 -*-
"""This module provides generic types for RCC UI."""

# Pylint complains about the __set__ and __get__ property method calls.
# It suggest the usage of the "get/set" methods, but these do not apply to the property.
# pylint: disable=unnecessary-dunder-call
import abc
import re as regex
from enum import Enum
from typing import (
    Any,
    Callable,
    Dict,
    Generic,
    List,
    Optional,
    Protocol,
    Tuple,
    TypeVar,
    Union,
)

from ..transport.jadelogger import PrintfLogger
from .console_command import CommandAttribute, CommandMode, ConsoleCommand
from .deviceinfo import DeviceInfo
from .property_attribute import PropertyAttributeBase, PropertyAttributeDriver

T = TypeVar("T")
Tenum = TypeVar("Tenum", bound=Enum)


class AppGeneric(Protocol):
    """This class provides an interface for an application shell."""

    def show_user_message(self, message: str) -> None:
        """Show a message to the user.

        :param message: The message to be shown.
        """

    def show_user_warning(self, warning: str) -> None:
        """Show a warning to the user.

        :param warning: The warning text to be shown.
        """

    def show_user_error(self, error: str) -> None:
        """Show an error to the user.

        :param error: The error text to be shown.
        """


class RccArgumentTypeDictGeneric(Generic[T]):
    """This class represents a CLI command that can behave like an argument."""

    def __init__(self, values: Dict[str, T]) -> None:
        """This class represents a CLI argument which is based on an option dictionary.

        :param values: The supported options for the argument.
        """
        self._dict = {k.upper(): v for k, v in values.items()}

    def value_to_option(self, value: T) -> str:
        """Convert a CLI argument to a supported dictionary entry.

        :param value: The received argument value.
        :raises ValueError: When the value does not exist in the options.
        :return: The corresponding dictionary entry.
        """
        try:
            return next((k for k, v in self._dict.items() if value == v))
        except StopIteration as ex:
            raise ValueError(f"Unable to find entry for value {value}") from ex

    @property
    def options(self) -> List[str]:
        """The argument values of the supported options.

        :return: A list with the values.
        """
        return list(self._dict.keys())

    def to_value(self, value: str) -> T:
        """Convert an option to a CLI argument (str).

        :param value: The option.
        :return: An uppercase string.
        """
        return self._dict[value.upper()]


RccArgumentTypeDict = RccArgumentTypeDictGeneric[int]


class IndicationMessageCallback(Protocol):  # pylint: disable=too-few-public-methods
    """Interface that represents indications received from the device."""

    def __call__(self, message: Any) -> None:
        """Call operator.

        :param message: The message object received from the device.
        """


class CommandAttributeInt(CommandAttribute):
    """Class for integer attributes."""

    def __init__(
        self,
        name: str,
        help_text: List[str],
        instance: PropertyAttributeDriver,
        propname: str,
    ):
        """Create an Int attribute for CLI interaction.

        :param name: Attribute's name.
        :param help_text: Attribute's help text.
        :param instance: API driver instance.
        :param propname: Property name in the API driver.
        """
        self._help_text = help_text
        self.instance = instance
        self.prop: PropertyAttributeBase[int] = vars(type(instance))[propname]
        super().__init__(name)

    def get_help(self) -> List[str]:
        """Get this command's user presentable help text.

        :return: A multi-line help text.
        """
        return self._help_text

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        val = self.prop.__get__(self.instance)
        return (f"{val}", f"(0x{val:X})")

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        return [
            f"{self.name} <VALUE>",
            "<VALUE> is a decimal value",
        ]

    def execute_console_line(self, args: List[str]) -> Any:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        if not args:
            raise ValueError("No value provided")
        if len(args) > 1:
            raise ValueError("Too many values provided")
        try:
            value = int(args[0], 0)
        except Exception as ex:  # noqa B902
            raise ValueError(f"Value '{args[0]}' cannot be converted to an integer") from ex
        return self.prop.__set__(self.instance, value)


class CommandAttributeGpioNumber(CommandAttribute):
    """Class for integer attributes."""

    def __init__(
        self,
        name: str,
        help_text: List[str],
        instance: PropertyAttributeDriver,
        propname: str,
    ):
        """Create GPIO number attribute.

        :param name: Attribute's name.
        :param help_text: Attribute's help text.
        :param instance: API driver instance.
        :param propname: Property name in the API driver.
        """
        self._help_text = help_text
        self.instance = instance
        self.prop: PropertyAttributeBase[int] = vars(type(instance))[propname]
        super().__init__(name)

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        val = self.prop.__get__(self.instance)
        return (f"GPIO{val}", f"(0x{val:X})")

    def get_help(self) -> List[str]:
        """Get this command's user presentable help text.

        :return: A multi-line help text.
        """
        return self._help_text

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        return [
            f"{self.name} <VALUE>",
            "<VALUE> is a GPIO identifier (e.g. GPIO3)",
        ]

    def execute_console_line(self, args: List[str]) -> Any:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        if not args:
            raise ValueError("No value provided")
        if len(args) > 1:
            raise ValueError("Too many values provided")
        res = regex.fullmatch(r"^GPIO(\d*)$", args[0])
        if res is None:
            raise ValueError(f"Value '{args[0]}' does not match GPIOx pattern.")
        return self.prop.__set__(self.instance, int(res.groups()[0]))


class CommandAttributeIntAndOff(CommandAttribute):
    """Class for integer attributes with encoded Off state as 0xFF."""

    def __init__(
        self,
        name: str,
        help_text: List[str],
        instance: PropertyAttributeDriver,
        propname: str,
    ):
        """Create Integer+Off attribute.

        :param name: Attribute's name.
        :param help_text: Attribute's help text.
        :param instance: API driver instance.
        :param propname: Property name in the API driver.
        """
        self._help_text = help_text
        self.instance = instance
        self.prop: PropertyAttributeBase[int] = vars(type(instance))[propname]
        super().__init__(name)

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        val = self.prop.__get__(self.instance)
        if val == 0xFF:
            str_out = "OFF"
        else:
            str_out = f"{val}"
        return str_out, f"(0x{val:X})"

    def get_help(self) -> List[str]:
        """Get this command's user presentable help text.

        :return: A multi-line help text.
        """
        return self._help_text

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        return [
            f"{self.name} <VALUE>",
            "<VALUE> is a decimal value",
        ]

    def execute_console_line(self, args: List[str]) -> Any:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        if not args:
            raise ValueError("No value provided")
        if len(args) > 1:
            raise ValueError("Too many values provided")
        if args[0].upper() == "OFF":
            value = 0xFF
        else:
            try:
                value = int(args[0], 0)
            except Exception as ex:  # noqa B902
                raise ValueError(f"Value '{args[0]}' cannot be converted to an integer") from ex
        return self.prop.__set__(self.instance, value)


class CommandAttributeOnOff(CommandAttribute):
    """This class represents an attribute which has two states, ON or OFF."""

    def __init__(
        self,
        name: str,
        help_text: List[str],
        instance: PropertyAttributeDriver,
        propname: str,
    ):
        """Create bistable (On-Off) attribute.

        :param name: Attribute's name.
        :param help_text: Attribute's help text.
        :param instance: API driver instance.
        :param propname: Property name in the API driver.
        """
        self._help_text = help_text
        self.instance = instance
        self.prop: PropertyAttributeBase[bool] = vars(type(instance))[propname]
        super().__init__(name)

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        val = self.prop.__get__(self.instance)
        return ("ON" if val else "OFF", "")

    def get_help(self) -> List[str]:
        """Get this command's user presentable help text.

        :return: A multi-line help text.
        """
        return self._help_text

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        return [
            f"{self.name} <VALUE>",
            "<VALUE> is a 'ON' or 'OFF'",
        ]

    def execute_console_line(self, args: List[str]) -> Any:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        if not args:
            raise ValueError("No value provided")
        if len(args) > 1:
            raise ValueError("Too many values provided")
        try:
            value = args[0].lower() == "on"
        except Exception as ex:  # noqa B902
            raise ValueError(f"Value '{args[0]}' cannot be converted to an integer") from ex
        return self.prop.__set__(self.instance, value)


class CommandAttributeEnum(CommandAttribute, Generic[Tenum]):
    """This class represents an attribute with a limited set of accpeted values."""

    # pylint: disable-next=too-many-arguments
    def __init__(
        self,
        name: str,
        help_text: List[str],
        instance: PropertyAttributeDriver,
        propname: str,
        value_dict: Dict[str, Tenum],
    ):
        """Create GPIO number attribute.

        :param name: Attribute's name.
        :param help_text: Attribute's help text.
        :param instance: API driver instance.
        :param propname: Property name in the API driver.
        :param value_dict: CLI value to python enumeration object mapping.
        """
        self._help_text = help_text
        self.instance = instance
        self.prop: PropertyAttributeBase[Tenum] = vars(type(instance))[propname]
        self.value_dict = RccArgumentTypeDictGeneric(value_dict)
        super().__init__(name)

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        val = self.prop.__get__(self.instance)
        return (self.value_dict.value_to_option(val), f"(0x{val.value:X})")

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        return [
            f"{self.name} <VALUE>",
            f"<VALUE> is one of {', '.join(self.value_dict.options)}",
        ]

    def get_help(self) -> List[str]:
        """Get this command's user presentable help text.

        :return: A multi-line help text.
        """
        return self._help_text

    def execute_console_line(self, args: List[str]) -> Any:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        if not args:
            raise ValueError("No value provided")
        if len(args) > 1:
            raise ValueError("Too many values provided")
        try:
            value = self.value_dict.to_value(args[0])
        except Exception as ex:  # noqa B902
            raise ValueError(f"Value '{args[0]}' not in the known options") from ex
        return self.prop.__set__(self.instance, value)


class CommandAttributeBytes(CommandAttribute):
    """This class represents an attribute which's data type is `bytes`."""

    def __init__(
        self,
        name: str,
        help_text: List[str],
        instance: PropertyAttributeDriver,
        propname: str,
    ):
        """Create a byte array (bytes) attribute.

        :param name: Attribute's name.
        :param help_text: Attribute's help text.
        :param instance: API driver instance.
        :param propname: Property name in the API driver.
        """
        self._help_text = help_text
        self.instance = instance
        self.prop: PropertyAttributeBase[bytes] = vars(type(instance))[propname]
        super().__init__(name)

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        val = "".join((f"{bval:X}" for bval in self.prop.__get__(self.instance)))
        if len(val) % 2:
            return ("0" + val, "")
        return (val, "")

    def get_help(self) -> List[str]:
        """Get this command's user presentable help text.

        :return: A multi-line help text.
        """
        return self._help_text

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        return [
            f"{self.name} <VALUE>",
            "<VALUE> is an HEX number string (e.g. [0x]00110C0BFF)",
        ]

    def execute_console_line(self, args: List[str]) -> Any:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        if not args:
            raise ValueError("No value provided")
        if len(args) > 1:
            raise ValueError("Too many values provided")
        if len(args[0]) % 2 != 0:
            raise ValueError("Value string doesn't have even length")
        ret = bytearray()
        for idx in range(0, len(args[0]), 2):
            ret.append(int(args[0][idx : (idx + 2)], 16))
        return self.prop.__set__(self.instance, bytes(ret))


class CommandModeAction(CommandMode):
    """This class represents a mode which behaves as a command (e.g. RR)."""

    def __init__(
        self,
        name: str,
        help_text: List[str],
        action: Callable[[], Optional[List[str]]],
        attribute_list: Optional[List[CommandAttribute]] = None,
    ) -> None:
        """This class represents a mode which behaves as a command (e.g. RR).

        :param name: The name of this command.
        :param help_text: The help text of this command.
        :param action: A callback to be executed when this command is invoked.
        :param attribute_list: Optional arguments supplied to the command, defaults to None.
        """
        super().__init__(name, False, attribute_list)
        self.help_text = help_text
        self.action = action

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        raise RuntimeError("An invalid program state called this function.")

    def execute_console_line(self, args: List[str]) -> Optional[List[str]]:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        self._execute_sub_attributes(args)
        return self.action()

    def get_help(self) -> List[str]:
        """Get the help text for this command.

        :return: A list of help text lines.
        """
        return self.help_text

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        args = " ".join(f"(`{x.name}`)" for x in self._attr_list)
        return [
            f"This command follows the console format: [{self.name}] {args}",
            "Where values in `[]` are mandatory and in `()` are optional",
        ]


class CommandModeOnOff(CommandMode):
    """This class represents a mode which can be turned on and off (e..g RX)."""

    def __init__(  # pylint: disable=too-many-arguments
        self,
        name: str,
        help_text: List[str],
        on_func: Callable[[Optional[int]], None],
        off_func: Callable[[], None],
        getvalue_func: Optional[Callable[[], bool]],
        attribute_list: Optional[List[CommandAttribute]] = None,
        extra_action_on_args: Optional[Callable[[Optional[List[str]]], Optional[List[str]]]] = None,
    ):
        """This class represents a mode which can be turned on and off (e.g. RX).

        :param name: The mode's name.
        :param help_text: The mode's help text.
        :param on_func: A callback that's executed when the mode is turned on.
        :param off_func: A callback that's executed when the mode is turned off.
        :param getvalue_func: A callback to query the device's current mode status.
        :param attribute_list: Optional parameters to the mode, defaults to None.
        """
        super().__init__(name, True, attribute_list)
        self.help = help_text
        self._on_func = on_func
        self._off_func = off_func
        self._getvalue_func = getvalue_func
        self._extra_action_on_args = extra_action_on_args

    def get_current_device_value(self) -> Tuple[str, str]:
        """Query the device for the current value for this command (attribute or mode).

        :return: A tuple of a short and extended value representation of the value.
        """
        if self._getvalue_func is not None:
            return ("ON" if self._getvalue_func() else "OFF", "")
        return ("", "")

    def get_help(self) -> List[str]:
        """Get the help text for this command.

        :return: A list of help text lines.
        """
        return self.help

    def get_syntax(self) -> List[str]:
        """Get the command execution syntax.

        :return: A list of syntax text lines.
        """
        args = " ".join(f"(`{x.name}`)" for x in self._attr_list)
        return [
            f"This command follows the console format: [{self.name}] {args} [ON/OFF] (TIME)",
            "Where values in `[]` are mandatory and in `()` are optional, TIME in ms",
        ]

    def execute_console_line(self, args: List[str]) -> Optional[List[str]]:
        """Parse the console arguments for this command.

        :param args: The supplied string arguments.
        :return: Optional user-facing messages related to the command execution.
        """
        # Find the "sentence break" (on/off)
        on_off_idx = None
        for idx, val in enumerate(args):
            if val in ("ON", "OFF"):
                on_off_idx = idx
                break
        if on_off_idx is None:
            # Execute all arguments as setting attributes
            self._execute_sub_attributes(args)
            if self._extra_action_on_args is not None:
                return self._extra_action_on_args(args)
            return None

        # Check if an execution time was provided
        if len(args) - on_off_idx > 2:
            raise ValueError("Too many trailing arguments after (ON/OFF)")
        if (len(args) - on_off_idx) == 2:
            exec_time_ms = int(args[-1])
        else:
            exec_time_ms = None

        self._execute_sub_attributes(args[:on_off_idx])

        # Lastly execute the command mode set
        if args[on_off_idx] == "ON":
            self._on_func(exec_time_ms)
        else:
            self._off_func()
        return None


# =============================================================================
# RCC DRIVER
# =============================================================================
class RCCDriverGeneric(abc.ABC):
    """Generic RCC driver class."""

    api_version: int
    """The API's communication version."""

    def __init__(self, app: AppGeneric, commands: List[ConsoleCommand]) -> None:
        """This class represents a generic RCC driver component.

        The top-level RCC UI interfacts with the device drivers through this layer.

        :param app: The app UI which this layer is bound to.
        :param commands: The supported commands for this layer.
        """
        self._app = app
        self.commands = {x.name: x for x in commands}
        self._printf_logger = PrintfLogger(None)

    @abc.abstractmethod
    def promote_to_connection_owner(self) -> None:
        """Promote lower communication layer to own the transport object.

        This function is required because, during initialization, the driver is created with an existing transport layer
        connection.
        """

    @abc.abstractmethod
    def get_device_info(self) -> DeviceInfo:
        """Request information about the connected DUT.

        :return: The connected DUT's info.
        """

    @abc.abstractmethod
    def start(self) -> None:
        """Start the DUT connection of this driver."""

    @abc.abstractmethod
    def stop(self) -> None:
        """Stops the DUT connection of this driver."""

    @abc.abstractmethod
    def set_gplog_callback(self, callback: Optional[Callable[[float, str], None]]) -> None:
        """Set the callback for received gpLog messages. If None, disables it."""

    def _indication_message_cb(self, message: Any) -> None:
        """Callback of received DUT indications.

        :param message: Indication message object received.
        """
        self._app.show_user_message(f"Received indication: {message}")

    @abc.abstractmethod
    def set_indication_message_callback(self, callback: IndicationMessageCallback) -> None:
        """Set the callback to execute when a new indication is received. Defaults to printing a string.

        :param callback: The new callable to use.
        """

    def delete_all_user_commands(self) -> None:
        """Deletes all registered commands."""
        self.commands = {}

    def delete_user_command(self, user_cmd: Union[str, List[str]]) -> None:
        """Deletes an user command or a list of commands.

        :param user_cmd: The command (or commands) to delete, identified by name.
        """
        if not isinstance(user_cmd, list):
            user_cmd = [user_cmd]
        for cmd in user_cmd:
            cmd = cmd.upper()
            self.commands.pop(cmd)

    def bind_user_command(self, func: ConsoleCommand) -> None:
        """Binds an RCC user command to a PTC/CTC command (setter and getter [optional]).

        :param func: The command to bind.
        """
        # Collect functions to bind
        self.commands[func.name] = func
