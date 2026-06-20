#!/usr/bin/env python3

import argparse
import sys
import os
import logging
import shutil
import subprocess
from dataclasses import dataclass
from typing import Tuple

DESCRIPTION = """\
Turn the application build hex-file into a bootable image and generate an ota image
"""


@dataclass
class GenerateOtaImageArguments:
    """helper to enforce type checking on argparse output"""
    input: str
    base_path: str
    output: str
    sign: bool
    compression: str
    flash_app_start_offset: int
    prune_only: bool


DEFAULT_FLASH_APP_START_OFFSET = 0x6000
LICENSE_SIZE = 0x100

SCRIPT_PATH = os.path.dirname(__file__)
HEXTOOLS_SCRIPT_PATH = os.path.join(SCRIPT_PATH, "..", "Hex")
CRCFIRMWARE_PATH = f"{SCRIPT_PATH}/crcFirmware.py"
HEX2BIN_PATH = f"{HEXTOOLS_SCRIPT_PATH}/hex2bin.py"
COMPRESSFIRMWARE_PATH = f"{SCRIPT_PATH}/compressFirmware.py"
OTAGEN_PATH = f"{SCRIPT_PATH}/ota_gen.py"

if not os.path.isfile(OTAGEN_PATH):
    CRCFIRMWARE_PATH = os.getenv("QORVO_CRCFIRMWARE_PATH", CRCFIRMWARE_PATH)
    HEX2BIN_PATH = os.getenv("QORVO_HEX2BIN_PATH", HEX2BIN_PATH)
    COMPRESSFIRMWARE_PATH = os.getenv("QORVO_COMPRESSFIRMWARE_PATH", COMPRESSFIRMWARE_PATH)
    OTAGEN_PATH = os.getenv("QORVO_OTAGEN_PATH", OTAGEN_PATH)


def parse_command_line_arguments() -> GenerateOtaImageArguments:
    """Parse command-line arguments"""
    def any_base_int(string):
        return int(string, 0)
    parser = argparse.ArgumentParser(description=DESCRIPTION)

    parser.add_argument("--input",
                        help="Path to input file")
    parser.add_argument('--sign', help='Sign firmware', action='store_true')
    parser.add_argument("--compression",
                        choices=['none', 'lzma'],
                        default="lzma",
                        help="compression type (default to lzma)")
    parser.add_argument('-p', '--flash_app_start_offset',
                        type=any_base_int,
                        help='Offset of the application in program flash',
                        default=DEFAULT_FLASH_APP_START_OFFSET)
    parser.add_argument("--prune_only",
                        help="prune unneeded sections; don't add an upgrade user license (external storage scenario)",
                        action='store_true')

    args = parser.parse_args()

    args.base_path = os.path.splitext(args.input)[0]

    args.output = args.base_path

    return GenerateOtaImageArguments(**vars(args))


def validate_arguments(args: GenerateOtaImageArguments):
    if not args.input:
        logging.error("Supply an input file")
        sys.exit(-1)
    else:
        assert os.path.isfile(args.input), f"The path specified as input file is not a file: {args.input}"

    if args.sign:
        logging.error("Secure boot not ready for alpha release yet")
        sys.exit(-1)


def run_script(command: str):
    """ run a python script using the current interpreter """
    assert command != ""
    logging.info("%s", command)
    subprocess.check_output(f"{sys.executable} {command}", shell=True)


def post_process_image(args: GenerateOtaImageArguments):
    """Run Qorvo image post-processing steps

    WARNING: THIS FUNCTION MODIFIES THE INPUT FILE!

    Add necessary metadata for the bootloader to process
    * crc/sign, set application loaded by bootloader flag
    """

    copy_of_unmodified_input = f"{args.base_path}.input.hex"

    # we modify in place, keep a copy of the input for reference
    shutil.copyfile(args.input, copy_of_unmodified_input)

    common_arguments = ("--set_bootloader_loaded "
                        f"--hex {args.input} "
                        f"--license_offset {args.flash_app_start_offset:#x} "
                        f"--section1 {args.flash_app_start_offset+LICENSE_SIZE:#x}:0xffffffff "
                        "--section2 0x800:0x1000 "
                        f"--start_addr_area 0x10020000 "
                        )

    if args.sign:
        # TODO: sign application
        args.output += ".sign"
        return
    else:
        run_script(f"{CRCFIRMWARE_PATH} --add_crc "
                   f"{common_arguments}")


def compress_ota_payload(args: GenerateOtaImageArguments):
    """Apply compression and add metadata for the Qorvo bootloader"""
    intermediate_hash_added_binary = f"{args.base_path}.hash.bin"
    intermediate_compressed_binary_path = f"{args.base_path}.compressed.bin"
    run_script(f"{HEX2BIN_PATH} {args.input} {intermediate_hash_added_binary}")
    run_script(f"{COMPRESSFIRMWARE_PATH} "
               f"{'--add_crc ' if not args.sign else ''}"
               f"{'--prune_only ' if args.prune_only else ''}"
               f"--compression={args.compression} "
               f"--license_offset {args.flash_app_start_offset-0x10:#x} --ota_offset 0xc6000 "
               f"--input {intermediate_hash_added_binary} "
               f"--output {intermediate_compressed_binary_path} "
               "--page_size 0x200 --sector_size 0x400 "
               )
    if args.compression != "none":
        args.output += ".compress"
    return intermediate_compressed_binary_path


def main():
    """ Main """

    logging.basicConfig(level=logging.INFO)

    args = parse_command_line_arguments()

    validate_arguments(args)

    # Bootable image preparation
    post_process_image(args)

    # Qorvo specific OTA preparation
    intermediate_compressed_binary_path = compress_ota_payload(args)

    if not args.sign:
        run_script(f"{OTAGEN_PATH} "
                   f"--binary_file {intermediate_compressed_binary_path} "
                   f"--jumptable_offset 0x0 "
                   f"--jumptable_size 0x1000 "
                   f"--application_offset 0x1000 "
                   f"--output_file {args.output}.ota "
                   )
    else:
        return


if __name__ == "__main__":
    main()
