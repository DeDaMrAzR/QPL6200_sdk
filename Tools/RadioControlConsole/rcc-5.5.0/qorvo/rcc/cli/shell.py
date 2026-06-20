# -*- coding: utf-8 -*-
# type:ignore
"""Entrypoint for Radio Control Console."""
# Ignoring mypy because it is complaining about click's untyped decorators

import datetime
import logging
import logging.handlers
import os
import sys

import click

from qorvo.rcc import __version__ as version
from qorvo.rcc.rcc import RCCShell
from qorvo.rcc.rcc_types.connection import ConnectionArguments


def setup_logging(log_folder: str, console_level: int) -> None:
    """Sets up logging for RCC."""
    if not os.path.exists(log_folder):
        os.mkdir(log_folder)

    file = logging.FileHandler(
        f"{log_folder}/{str(datetime.date.today()).replace('-', '')}_{str(datetime.datetime.now().time()).split('.', maxsplit=1)[0].replace(':', '')}.log"  # noqa: E501 pylint: disable=line-too-long
    )
    file.setLevel(logging.DEBUG)
    file.setFormatter(logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s"))

    out = logging.StreamHandler(sys.stdout)

    # set the logging level for the console output
    out.setLevel(console_level)
    out.setFormatter(logging.Formatter("%(name)s - %(levelname)s - %(message)s"))

    root_logger = logging.getLogger()

    # set the minimum logging level for all registered handlers
    root_logger.setLevel(logging.DEBUG)

    root_logger.addHandler(file)
    root_logger.addHandler(out)


@click.command(
    no_args_is_help=True,
    context_settings=dict(help_option_names=["-h", "--help"]),  # noqa: C408 pylint: disable=use-dict-literal
)
@click.option("-c", "--COM", "com", type=str, default="", help="The COM port that is used to connect to the target.")
@click.option("-b", "--BaudRate", "baud", type=int, default=57600, help="[reserved] The baudrate of the connection.")
@click.option("-e", "--ETH", "eth", type=str, default="", help="The ethernet host name of the target.")
@click.option("-p", "--port", type=int, default=9192, help="The ethernet port used to connect to the target.")
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
    "-u",
    "--UserConfiguration",
    "userconfig",
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
@click.option("--api-version", "apiversion", type=int, default=None, help="Set the RCC API version.")
@click.option("--version", "showversion", is_flag=True, help="Show application version and exit.")
def rcc_click_wrapper(  # pylint: disable=too-many-arguments
    com: str,
    baud: int,
    eth: str,
    port: int,
    coex: bool,
    addrmap: str,
    scriptfile: str,
    userconfig: str,
    extfolder: str,
    debuglevel: int,
    apiversion: int | None,
    showversion: bool,
) -> None:
    """Exposes RCC as Click CLI."""
    if showversion:
        print(f"Radio Control Console {version}")
        return
    try:
        obj = RCCShell(
            ConnectionArguments(com, baud, eth, port),
            coex,
            addrmap,
            userconfig,
            extfolder,
            debuglevel,
            apiversion,
        )
        obj.execute_cli(scriptfile)
    except Exception as ex:  # noqa B902, pylint: disable=W0718
        # Suppress default backtrace print in liew of abetter user experience;
        # he/she does not need to know where the error was.
        print(f"Fatal error: {ex}")
        if os.getenv("RCC_DEBUG") == "true":
            raise


def main():
    """Main entrypoint function."""
    setup_logging("./log", console_level=logging.WARNING)
    rcc_click_wrapper()  # pylint: disable=E1120


if __name__ == "__main__":
    main()
