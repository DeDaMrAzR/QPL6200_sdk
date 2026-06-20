# -*- coding: utf-8 -*-
"""Radio control console package entrypoint."""

import sys

if __name__ == "__main__":
    from .cli.shell import rcc_click_wrapper

    sys.exit(rcc_click_wrapper())  # pylint: disable=E1120
