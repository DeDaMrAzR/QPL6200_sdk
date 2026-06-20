# type: ignore
# -*- coding: utf-8 -*-
"""Radio control console package system test entrypoint."""

import logging

import click

from qorvo.rcc.rcc import RCCShell

logger = logging.getLogger(__name__)


# =============================================================================
# RCC CLI
@click.command(
    no_args_is_help=True,
    context_settings=dict(help_option_names=["-h", "--help"]),  # noqa: C408 pylint: disable=R1735
)
@click.option("-c", "--COM", "com", type=str, default="", help="The COM port that is used to connect to the target.")
@click.option("-b", "--BaudRate", "baud", type=str, default="", help="[reserved] The baudrate of the connection.")
@click.option("-e", "--ETH", "eth", type=str, default="", help="The ethernet host name of the target.")
@click.option("-p", "--port", type=str, default="", help="The ethernet port used to connect to the target.")
@click.option("-", "--coex", "coex", is_flag=True, help="Enable COEX functionality.")
@click.option(
    "-a",
    "--addressmap",
    "addrmap",
    type=str,
    default="",
    help="[reserved] Filename containing Addressmap with register definitions.",
)
@click.option(
    "-s",
    "--scriptFileName",
    "scriptfile",
    type=str,
    default="",
    help="The file name that contains a list of commands to execute in sequence.",
)
@click.option(
    "-S",
    "--settingFileName",
    "settingfile",
    type=str,
    default="",
    help="The file name that contains a list of hotkey settings.",
)
@click.option(
    "-f",
    "--Extensionsfolder",
    "extfolder",
    type=str,
    default="",
    hidden=True,
    help="Directory that contains the testcomps ptc dlls.",
)
@click.option("-d", "--debuglevel", "debuglevel", type=int, default=0, help="Set the debug level.")
@click.option("-v", "--version", "version", type=int, default=1, help="Set the RCC API version.")
def rcc_click_wrapper(  # pylint: disable=too-many-arguments
    com: str,
    baud: int,
    eth: str,
    port: int,
    coex: bool,
    addrmap: str,
    scriptfile: str,
    settingfile: str,
    extfolder: str,
    debuglevel: int,
    version: int,
):
    """Exposes RCC as Click CLI."""
    obj = RCCShell(com, baud, eth, port, coex, addrmap, settingfile, extfolder, debuglevel, version)
    obj.execute_cli(scriptfile)


if __name__ == "__main__":
    rcc_click_wrapper()  # pylint: disable=E1120
