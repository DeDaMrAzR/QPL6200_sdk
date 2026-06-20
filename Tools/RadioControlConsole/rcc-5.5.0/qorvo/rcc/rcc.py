"""This module provides the top-level CLI parsing interface."""

# flake8: noqa
# pylint: skip-file
import logging
import os
import sys
from cmd import Cmd
from functools import partial
from pathlib import Path
from typing import Any, Callable, Dict, List, Optional, TextIO, Tuple, Union

from lxml import etree
from serial.tools import list_ports

from .rcc_types import connection
from .rcc_types.console_command import CommandAttribute, CommandMode
from .rcc_types.deviceinfo import API_VERSION_UNKNOWN, DeviceInfo
from .rcc_types.generic import AppGeneric, CommandModeAction, RCCDriverGeneric
from .rcc_types.rcc_exceptions import InvalidStateException
from .transport.gpAPI_2 import gpAPIConfirmIndication
from .transport.peakconnect import (
    SerialConnectionException,
    peakConnector,
    peakConnectorSerial,
    peakConnectorSocket,
    create_peak_connector,
)

logger = logging.getLogger(__name__)

# Import rccdriver modules
from .ptc_versions import manager as ptc_manager

# from . ctc_versions.v1 import rccdriver as rccdriver_ctc_v1

try:
    from msvcrt import getch as msvcrt_getch  # type: ignore [attr-defined]
    from msvcrt import putch as msvcrt_putch  # type: ignore [attr-defined]

    _msvcrt_is_imported = True
except ModuleNotFoundError:

    def msvcrt_getch() -> bytes:
        raise NotImplementedError(f"The 'msvcrt' module is not available for {os.name}")

    def msvcrt_putch(data: Union[bytes, bytearray], /) -> None:
        raise NotImplementedError(f"The 'msvcrt' module is not available for {os.name}")

    _msvcrt_is_imported = False


class ConcurrentIo:
    def __init__(self, special_cb: Callable[[int, int], None]):
        self.special_cb = special_cb

    def readline(self, limit: int = -1) -> str:
        precode = None
        msg = ""
        while True:
            capture = msvcrt_getch()
            if precode is not None:
                self.special_cb(int.from_bytes(precode, "little"), int.from_bytes(capture, "little"))
                precode = None
            elif capture in (b"\x00", b"\xe0"):
                precode = capture
            elif capture == b"\x03":
                sys.exit(0)
            else:
                msvcrt_putch(capture)
                msg += capture.decode("UTF-8")
                # For some reason Windows in a PowerShell, cmd.exe and Windows Terminal window only echo "\r"
                # as end of line, and not \r\n as expected by os.linesep
                if msg.endswith("\r"):
                    # This is required as, since we didn"t receive it, we just echoed the "\r" character
                    msvcrt_putch(b"\n")
                    return msg


def make_connection(app: AppGeneric, conn_args: connection.ConnectionArguments) -> peakConnector:
    # PTC/CTC connection
    try:
        conn = create_peak_connector(conn_args)
    except SerialConnectionException:
        app.show_user_error(f"Failed to connect to {conn_args.com}")
        app.show_user_message("These are the available ports:")
        for port in list_ports.comports():
            app.show_user_message(port.name)
            details = f" -- {port.description}"
            if port.manufacturer:
                details += f" by {port.manufacturer}"
            app.show_user_message(details)
        raise
    return conn


def connect_rcc_dut(
    app: AppGeneric,
    conn_args: connection.ConnectionArguments,
    is_coex: bool,
    forced_api_version: Optional[int] = None,
    debug_level: int = 0,
) -> Tuple[RCCDriverGeneric, DeviceInfo]:
    """Connect to PTC/CTC by providing appropriate connection parameters."""
    connector = make_connection(app, conn_args)
    try:
        if is_coex:
            raise NotImplementedError("Coex functionality is not supported in this version.")
        rcc_driver = ptc_manager.connect_to_rcc_driver(connector, app, forced_api_version)

        # Check PTC/CTC:
        device_info = rcc_driver.get_device_info()
        if is_coex and "ctc" not in device_info.product_name.lower():
            msg = "The DUT is not flashed with a CTC firmware."
            logger.error(msg)
            raise ValueError(msg)
        elif not is_coex and "ptc" not in device_info.product_name.lower():
            msg = "The DUT is not flashed with a PTC firmware."
            logger.error(msg)
            raise ValueError(msg)

        return rcc_driver, device_info

    # pylint: disable-next=too-broad-exception
    except Exception:
        connector.Stop()
        raise


# =============================================================================
# RCC Interactive Shell
class RCCShell(Cmd, AppGeneric):
    """This class provides a CLI REPL for RCC commands."""

    prompt = "rcc> "
    intro = "Welcome to RCC! Type ? to list commands."
    file = None
    pad = "   "

    def execute_cli(self, scriptfile: Optional[Union[str, Path]]) -> None:
        # Run script
        if scriptfile:
            self._print_user(f"== Running {scriptfile} ==")
            self.do_run(scriptfile)
        self.cmdloop()

    def __init__(
        self,
        conn_args: connection.ConnectionArguments,
        coex: bool,
        addrmap: Optional[str],
        settingfile: Optional[str],
        extfolder: Optional[str] = None,
        debuglevel: int = 0,
        forced_api_version: Optional[int] = None,
        completekey: str = "Tab",
        stdin: TextIO = sys.stdin,
        stdout: TextIO = sys.stdout,
    ) -> None:
        super().__init__(completekey=completekey, stdin=stdin, stdout=stdout)
        self._is_coex = coex
        self._ljust = 18
        self._script_command_echo = False

        self.forced_api_version = forced_api_version

        self._rcc_driver, self._device_info = connect_rcc_dut(
            self,
            conn_args,
            is_coex=self._is_coex,
            forced_api_version=self.forced_api_version,
            debug_level=debuglevel,
        )

        # Load rccdriver
        self._show_device_info(self._rcc_driver.get_device_info())

        # Load commands
        for cmd in self._rcc_driver.commands.keys():
            setattr(RCCShell, f"do_{cmd.lower()}", partial(self._do_command, cmd))
            setattr(
                RCCShell,
                f"help_{cmd.lower()}",
                partial(self._help_command, cmd),
            )
            self._ljust = max(self._ljust, len(cmd) + 4)

        # Hotkeys
        self._hotkeys: Dict[str, List[str]] = {}
        if settingfile:
            self.configure_hotkeys(settingfile)

    def configure_hotkeys(self, settingfile: str) -> None:
        with open(settingfile) as fobj:
            xml = fobj.read()

        self._hotkeys = {}
        root = etree.fromstring(xml)
        for kb in root.find("KeyBindings").getchildren():  # type: ignore[union-attr]
            hot_key_str = kb.find("Key").text.lower()
            commandlist = kb.findall("Command")
            # Filter out empty entries
            cmd_list = [cmd.text.strip() for cmd in commandlist if cmd.text]
            self._hotkeys[hot_key_str] = [cmd for cmd in cmd_list if cmd]

        _special_key_map: Dict[Tuple[int, int], str] = {
            (0x00, 59): "f1",
            (0x00, 60): "f2",
            (0x00, 61): "f3",
            (0x00, 62): "f4",
            (0x00, 63): "f5",
            (0x00, 64): "f6",
            (0x00, 65): "f7",
            (0x00, 66): "f8",
            (0x00, 67): "f9",
            (0x00, 68): "f10",
            (0xE0, 133): "f11",
            (0xE0, 134): "f12",
        }

        def special_callback(precode: int, capture: int) -> None:
            hotkey = _special_key_map.get((precode, capture), "")
            for cmd in self._hotkeys.get(hotkey, []):
                # Cascade calls as in the cmd module
                self.stdout.write(cmd + os.linesep + self.prompt)
                self.stdout.flush()
                self.postcmd(self.onecmd(self.precmd(cmd)), cmd)

        if not _msvcrt_is_imported:
            # msvcrt is a standard python module in Windows but does not exist in Linux/Mac
            # Our work-around for typing correctness is to have a dummy class; here we check that
            raise RuntimeError("Hotkeys are disabled for this operating system.")

        self.stdin = ConcurrentIo(special_callback)  # type: ignore [assignment]
        self.use_rawinput = False

    def _print_user(self, *args: Any, **kwargs: Any) -> None:
        print(*args, file=self.stdout, **kwargs)

    def show_user_message(self, message: str) -> None:
        print(message, file=self.stdout)

    def show_user_warning(self, warning: str) -> None:
        print(warning, file=self.stdout)

    def show_user_error(self, error: str) -> None:
        print(error, file=self.stdout)

    def _do_command(self, cmd: str, arg: Optional[str] = None) -> None:
        """Template method for each command."""
        args = arg.upper().split() if arg is not None else []
        retval = None
        cmdobj = self._rcc_driver.commands.get(cmd, None)
        if cmdobj is None:
            self.show_user_error(f"Command {cmd} does not exist.")
            return
        try:
            retval = cmdobj.execute_console_line(args)
        except InvalidStateException as ex:
            self.show_user_error(f"Failed to set {cmd}, reason: {ex}")
            if ex.action:
                for line in ex.action:
                    self.show_user_message(line)
            return
        except ValueError as ex:
            logger.debug("Failed to execute command.", exc_info=ex)
            self.show_user_error(f"Failed to set {cmd}, reason: {ex}")
            for line in cmdobj.get_syntax():
                self.show_user_message(line)
            return
        except Exception as ex:
            self.show_user_error(f"Failed to set {cmd}, reason")
            for line in cmdobj.get_syntax():
                self.show_user_message(line)
            return
        if retval is None:
            return
        for line in retval:
            self._print_result(line)

    def _help_command(self, cmd: str) -> None:
        """Template method to generate the help message for each command."""
        cmd_obj = self._rcc_driver.commands[cmd]
        self._print_user(cmd_obj.get_help())
        self._print_user("Usage: " + " ".join(cmd_obj.get_syntax()))

    def do_help(self, arg: str) -> None:
        """Prints the help message."""
        if arg:
            # Print the help message for a specific attribute/method
            cmdobj = self._rcc_driver.commands.get(arg.upper(), None)
            if cmdobj is None:
                self.show_user_error(f"Command {arg} not found")
            else:
                self.show_user_message(os.linesep.join(cmdobj.get_help()))
                for line in cmdobj.get_syntax():
                    self.show_user_message(line)
            return

        entries_attrib: List[List[str]] = []
        entries_mode: List[List[str]] = []
        entries_commands: List[List[str]] = []
        for cmdobj in self._rcc_driver.commands.values():
            if isinstance(cmdobj, CommandModeAction):
                entries_commands.append([cmdobj.name] + cmdobj.get_help())
            elif isinstance(cmdobj, CommandMode):
                entries_mode.append([cmdobj.name] + cmdobj.get_help())
            else:
                entries_attrib.append([cmdobj.name] + cmdobj.get_help())

        self._print_user("General commands:")
        self._print_user("=================")
        entries_general = [
            ["I", "Get all settings"],
            ["H", "Returns this help message (also '?' or 'HELP')"],
            ["H <cmd>", "Returns the detailed help message for a command"],
            # ["VERSION", "Shows the used DLL version and ProductName"],
            ["RUN", "Loads and run a script"],
            ["RUNECHO <true/false>", "Enables or disables echoing of commands from script file"],
            ["PRINTLOG <true/false>", "Asynchronously print all device logging statements"],
            ["Q", "Quits the application (also 'QUIT' or 'EXIT')"],
        ]
        for entry in entries_general:
            self._print_help(entry, ljust=self._ljust)
        self._print_user()

        self._print_user("Attributes:")
        self._print_user("===========")
        for entry in sorted(entries_attrib, key=lambda x: x[0]):
            self._print_help(entry, ljust=self._ljust)
        self._print_user()

        self._print_user("Modes:")
        self._print_user("======")
        for entry in sorted(entries_mode, key=lambda x: x[0]):
            self._print_help(entry, ljust=self._ljust)
        self._print_user()

        self._print_user("Commands:")
        self._print_user("=========")
        for entry in sorted(entries_commands, key=lambda x: x[0]):
            self._print_help(entry, ljust=self._ljust)
        self._print_user()

    def do_exit(self, arg: str) -> None:
        """Exits the application."""
        self._print_user("Exiting the application.")
        self.stop_record()
        sys.exit()

    do_EOF = do_exit  # (Ctrl-D)

    def default(self, line: str) -> None:
        """Method called on an input line when the command prefix is not recognized."""
        inp_l = line.lower() + " "
        if inp_l == "q " or inp_l == "quit ":
            return self.do_exit(inp_l)
        elif inp_l[0] == "h" and inp_l[1] == " ":
            return self.do_help(inp_l[1:].strip())
        elif inp_l[0] == "i" and inp_l[1] == " ":
            self._print_status()
        elif inp_l[0] == "#":
            pass
        else:  # attempts to execute the command "as is"
            try:
                inp_l_split = inp_l.split()
                func = getattr(self, f"do_{inp_l_split[0]}")
            except AttributeError:
                inp_split = line.split()
                self._print_user(f"Error: Unknown command: {inp_split[0]}.")
            else:
                func(" ".join(inp_l_split[1:]))

    def completenames(self, text: str, *ignored: Any) -> List[str]:
        """Method called when the autocomplete key is pressed."""
        return [cmd for cmd in self._rcc_driver.commands.keys() if cmd.lower().startswith(text.lower())]

    def _show_device_info(self, info: DeviceInfo) -> None:
        self._print_user(f"802.15.4 MAC Address: {info.mac_address}")
        self._print_user(f"BLE Address: {info.ble_address}")
        self._print_user(f"APP version: {info.app_version} - Commit: {int(info.changelist):x}")
        self._print_user(f"PTC Version: {info.ptc_version}")
        self._print_user(f"API Version: {info.api_version if info.api_version != API_VERSION_UNKNOWN else '?'}")
        self._print_user(f"Target: {info.product_name} - {info.product_id}")
        self._print_user(f"Chip Version: {info.chip_version}")

    def _print_result(self, data: Union[Dict[str, Any], int, str]) -> None:
        """Prints RCC command results."""
        if isinstance(data, dict):
            for key, value in data.items():
                self._print_user(self.pad + f"{key} -> {value}")
        elif isinstance(data, gpAPIConfirmIndication):
            # The assumption here is that any non-successful command will raise and caught afterwards.
            # There should be no result if it was an error!
            self._print_user("Success")
        else:
            self._print_user(data)

    def _print_help(
        self,
        entries: List[str],
        ljust: int = 0,
    ) -> None:
        """Helper method to print help messages."""
        self._print_user(f"{entries[0].ljust(ljust)}: {entries[1]}")
        if len(entries) > 2:
            for entry in entries[2:]:
                self._print_user(f"{entry}")

    def _print_status(self) -> None:
        self._print_user("Attributes:")
        self._print_user("===========")
        for cmd_attrs in sorted(
            (x for x in self._rcc_driver.commands.values() if isinstance(x, CommandAttribute)),
            key=lambda x: x.name,
        ):
            try:
                val = cmd_attrs.get_current_device_value()
            except (RuntimeError, ValueError) as e:
                logger.debug(e, exc_info=True)
                val = ("?", "(Unknown)")
            self._print_user(f"{cmd_attrs.name.ljust(self._ljust)}={val[0].rjust(self._ljust)} {val[1]}".strip())
        self._print_user()

        self._print_user("Modes:")
        self._print_user("======")
        for cmd_modes in sorted(
            (x for x in self._rcc_driver.commands.values() if isinstance(x, CommandMode) and x.has_state),
            key=lambda x: x.name,
        ):
            try:
                state = cmd_modes.get_current_device_value()
            except (RuntimeError, ValueError) as e:
                logger.debug(e, exc_info=True)
                state = ("?", "(Unknown)")
            if state[0] or state[1]:
                self._print_user(
                    f"{cmd_modes.name.ljust(self._ljust)}={state[0].rjust(self._ljust)} {state[1]}".strip()
                )
        self._print_user()

    # -------------------------------------------------------------------------
    # Record and playback command script
    def do_record(self, filepath: str) -> None:
        """Save future commands to a file."""
        if not filepath:
            self._print_user("Error: Missing output file. Please provide an output file (ex: record script.txt).")
        elif os.path.exists(filepath):
            self._print_user(f"Error: The file '{filepath}' already exists. Please provide another file.")
        else:
            self.file = open(filepath, "w")

    def do_runecho(self, runecho: Union[str, bool]) -> None:
        if isinstance(runecho, bool):
            self._script_command_echo = runecho
        elif runecho.lower() == "true":
            self._script_command_echo = True
        elif runecho.lower() == "false":
            self._script_command_echo = False
        else:
            self._print_user('Error: invalid value, please use "true" or "false"')

    def do_printlog(self, printlog: Union[str, bool]) -> None:
        def _print_device_log(timestamp: float, message: str) -> None:
            self._print_user(f"Device: ({timestamp:0>12.4f}) {message}")

        match printlog:
            case "true" | True:
                self._rcc_driver.set_gplog_callback(_print_device_log)
            case "false" | False:
                self._rcc_driver.set_gplog_callback(None)
            case _:
                self._print_user('Error: invalid value, please use "true" or "false"')

    def do_run(self, filepath: Union[str, Path]) -> None:
        """Run commands from a file."""
        if not filepath:
            self._print_user("Error: Missing input file. Please provide an input file (ex: run script.txt).")
            return
        fpath = Path(filepath)
        if not fpath.exists():
            self._print_user(f"Error: The file '{filepath}' does not exist.")
            return
        self.stop_record()
        lines = fpath.read_text().splitlines()
        for line in lines:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if self._script_command_echo:
                self._print_user(self.pad + line)
            self.onecmd(line)
        self._print_user()

    def precmd(self, line: str) -> str:
        """Method called before executing commands."""
        line = line.upper()
        if self.file:
            self._print_user(line, file=self.file)
        return line

    def stop_record(self) -> None:
        """Close file."""
        if self.file:
            self._print_user("Recording has stopped")
            self.file.close()
            self.file = None
