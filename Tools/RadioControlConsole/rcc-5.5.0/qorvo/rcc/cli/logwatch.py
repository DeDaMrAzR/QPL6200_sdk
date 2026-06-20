# -*- coding: utf-8 -*-
# type:ignore
"""Entrypoint for Radio Control Console - LogWatch."""

# Ignoring mypy because it is complaining about click's untyped decorators
import logging
import time

import click

from qorvo.rcc.rcc_types.connection import ConnectionArguments
from qorvo.rcc.transport.jadelogger import Jadelogger
from qorvo.rcc.transport.peakconnect import create_peak_connector


@click.command(
    no_args_is_help=True,
    context_settings=dict(help_option_names=["-h", "--help"]),  # noqa: C408 pylint: disable=use-dict-literal
)
@click.option("-c", "--COM", "com", type=str, default="", help="The COM port that is used to connect to the target.")
@click.option("-b", "--BaudRate", "baud", type=int, default=57600, help="[reserved] The baudrate of the connection.")
@click.option("-e", "--ETH", "eth", type=str, default="", help="The ethernet host name of the target.")
@click.option("-p", "--port", type=int, default=9192, help="The ethernet port used to connect to the target.")
@click.option("-d", "--debuglevel", "debuglevel", type=int, default=None, help="Set the debug level.")
@click.option(
    "--ignore-crc-errors", is_flag=True, help="Ignore CRC errors in received messages and attempt to print them anyway."
)
def log_watch_click_wrapper(  # pylint: disable=too-many-arguments
    com: str,
    baud: int,
    eth: str,
    port: int,
    debuglevel: int | None,
    ignore_crc_errors: bool = False,
) -> None:
    """Exposes RCC as Click CLI."""
    logging.basicConfig(level=logging.INFO if debuglevel is None else debuglevel)

    conn = create_peak_connector(ConnectionArguments(com, baud, eth, port))
    if debuglevel is not None:
        conn.logger.setLevel(debuglevel)

    if ignore_crc_errors:
        conn.enforce_crc = False

    conn.Start()
    logger = Jadelogger(conn, None)
    logger.start()

    while True:
        time.sleep(0.5)


def main():
    log_watch_click_wrapper()  # pylint: disable=E1120


if __name__ == "__main__":
    main()
