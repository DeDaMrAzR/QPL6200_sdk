#!/usr/bin/env python3

import argparse
import getpass
import sys
import os
import logging
import re
import shutil
import subprocess
from intelhex import IntelHex
from dataclasses import dataclass
from typing import Tuple, Optional

from ota_multi_image_tool import pack_images

DESCRIPTION = """\
Turn a Matter application build hex-file into a bootable image and generate an ota image
"""


@dataclass
class GenerateOtaImageArguments:
    """helper to enforce type checking on argparse output"""
    chip_config_header: str
    factory_data_config: str
    se_image_file: str
    bl_image_file: str
    chip_root: str
    in_file: str
    out_file: str
    version: int
    min_version: int
    max_version: int
    version_str: str
    test_update_image: bool
    vendor_id: str
    product_id: str
    pem_file_path: str
    pem_password: str
    header_version: int
    flash_app_start_offset: int
    flash_start: int
    compression: str
    prune_only: bool
    ota_offset: int
    no_extended_user_license: bool
    jumptables: bool
    xp400x: bool
    developer_key: str
    secondary_input_file: str
    flash_secondary_start_offset: int


# QPG6105 values:
QPG6105_FLASH_START = 0x4000000
QPG6105_FLASH_APP_START_OFFSET = 0x8000
QPG6200_FLASH_SECONDARY_START_OFFSET = 0x176800
OTA_ADDRESS_OFFSET = 0xa0000
UPGRADE_SECUREBOOT_PUBLICKEY_OFFSET = 0x1800
LICENSE_SIZE = 0x100

SCRIPT_PATH = os.path.dirname(__file__)
HEXTOOLS_SCRIPT_PATH = os.path.join(SCRIPT_PATH, "..", "Hex")
HEX2BIN_PATH = f"{HEXTOOLS_SCRIPT_PATH}/hex2bin.py"
COMPRESSFIRMWARE_PATH = f"{SCRIPT_PATH}/compressFirmware.py"
BUNDLEOTAIMAGE_PATH = f"{SCRIPT_PATH}/bundle_ota_img.py"

# Attempt to get the paths from environment variables if it looks like they don't reside at the expected address
if not os.path.isfile(os.path.join(SCRIPT_PATH, "crypto_utils.py")):
    HEX2BIN_PATH = os.getenv("QORVO_HEX2BIN_PATH", HEX2BIN_PATH)
    COMPRESSFIRMWARE_PATH = os.getenv("QORVO_COMPRESSFIRMWARE_PATH", COMPRESSFIRMWARE_PATH)
    BUNDLEOTAIMAGE_PATH = os.getenv("QORVO_BUNDLEOTAIMAGE_PATH", BUNDLEOTAIMAGE_PATH)


def parse_command_line_arguments() -> GenerateOtaImageArguments:
    """Parse command-line arguments"""
    def any_base_int(string):
        return int(string, 0)

    parser = argparse.ArgumentParser(description=DESCRIPTION)

    parser.add_argument("--chip_config_header",
                        help="path to Matter config header file", default="")
    parser.add_argument("--factory_data_config", default="",
                        help="path to application factory data configuration file")
    parser.add_argument("--se_image_file", default="",
                        help="path to SE OTA signed image file")
    parser.add_argument("--bl_image_file", default="",
                        help="path to Bootloader OTA signed image file")
    parser.add_argument('-vn', '--version', type=any_base_int, help='Software version (numeric)', default=None)
    parser.add_argument('-mi', '--min_version', type=any_base_int,
                        help='Minimum applicable DUT software version for OTA image (numeric)', default=None)
    parser.add_argument('-ma', '--max_version', type=any_base_int,
                        help='Maximum applicable DUT software version for OTA image (numeric)', default=None)
    parser.add_argument('-vs', '--version-str', help='Software version (string)', default=None)
    parser.add_argument('-tui', '--test-update-image', action='store_true',
                        help='Extract incremented versions for OTA update test', default=False)
    parser.add_argument('-vid', '--vendor-id', help='Vendor ID (string)', default=None)
    parser.add_argument('-pid', '--product-id', help='Product ID (string)', default=None)
    parser.add_argument('--pem_file_path', help='PEM file path (string)', default=None)
    parser.add_argument('--pem_password', help='PEM file password (string)', default=None)
    parser.add_argument('--header_version', type=any_base_int, help='Qorvo Image Header version (numeric)', default=1)
    parser.add_argument('--ota_offset',
                        type=any_base_int,
                        help='OTA offset address',
                        default=OTA_ADDRESS_OFFSET)
    parser.add_argument('--flash_app_start_offset',
                        type=any_base_int,
                        help='Offset of the application in program flash',
                        default=QPG6105_FLASH_APP_START_OFFSET)
    parser.add_argument('--flash-secondary-start-offset',
                        type=any_base_int,
                        help='Offset of the secondary application in program flash',
                        default=QPG6200_FLASH_SECONDARY_START_OFFSET)
    parser.add_argument('--flash_start',
                        type=any_base_int,
                        help='Offset of the program flash in the memory map',
                        default=QPG6105_FLASH_START)
    parser.add_argument("--compression",
                        choices=['none', 'lzma'],
                        default="lzma",
                        help="compression type (default to none)")
    parser.add_argument("--prune_only",
                        help="prune unneeded sections; don't add an upgrade user license (external storage scenario)",
                        default=False,
                        action='store_true')
    parser.add_argument("--no-extended-user-license",
                        help="no extended user license in use",
                        default=False,
                        action='store_true')
    parser.add_argument("--jumptables",
                        help="jumptables are part of the OTA",
                        default=False,
                        action='store_true')
    parser.add_argument("--xp400x",
                        help="Use XP400x data structures",
                        default=False,
                        action='store_true')
    parser.add_argument("--developer-key",
                        help="sign firmware using developer's signing key",
                        metavar="FILE.sk")
    parser.add_argument("--secondary-input-file",
                        help="Path to input file for secondary application to format to Matter OTA fileformat")
    requiredArgGroup = parser.add_argument_group('required arguments')
    requiredArgGroup.add_argument("--out_file",
                                  help="Path to output file (.ota file)", required=True)
    requiredArgGroup.add_argument("--in_file",
                                  help="Path to input file to format to Matter OTA fileformat", required=True)
    requiredArgGroup.add_argument("--chip_root",
                                  help="Path to root Matter directory", required=True)

    logging.info(f"{sys.argv[0]} -> provided args : {sys.argv[1:]}")
    args = parser.parse_args()

    return GenerateOtaImageArguments(**vars(args))


def run_script(command: str):
    """ run a python script using the current interpreter """
    assert command != ""
    logging.info("%s", command)
    subprocess.check_output(f"{sys.executable} {command}", shell=True)


def get_hexfile_content_offset(args: GenerateOtaImageArguments, hex_file: str):
    """ return the difference between start of flash and first data in the intel hex """
    intel_hex_file = IntelHex(hex_file)
    offset = intel_hex_file.minaddr() - args.flash_start
    logging.info("Offset of first data in hex file: 0x%08x", offset)
    return offset


def compress_firmware(args: GenerateOtaImageArguments, intermediate_binary: str, intermediat_compressed_binary_path: str, hexfile_content_offset: str, flash_app_start_offset: int):
    run_script(f"{COMPRESSFIRMWARE_PATH} "
               f" --compression={args.compression}"
               f" {'--no-extended-user-license' if args.no_extended_user_license else ''}"
               f" --input {intermediate_binary}"
               f" --license_offset {flash_app_start_offset - hexfile_content_offset:#x}"
               + f" --output {intermediat_compressed_binary_path}"
               " --page_size 0x200 --sector_size 0x400"
               + (f" --pem {args.pem_file_path} "
                  f" --pem_password {args.pem_password}" if args.pem_file_path else "")
               + (f" --developer-key {args.developer_key}" if args.developer_key else "")
               + (" --xp400x" if args.xp400x else f" --ota_offset {args.ota_offset:#x}")
               + (" --prune_only" if args.prune_only else ""))


def compress_ota_payload(args: GenerateOtaImageArguments):
    """Apply compression and add metadata for the Qorvo bootloader"""
    intermediate_secondary_compressed_binary_path = None

    input_base_path = os.path.splitext(args.in_file)[0]
    intermediate_primary_binary = f"{input_base_path}.bin"
    intermediate_primary_compressed_binary_path = f"{input_base_path}.compressed.bin"

    run_script(f"{HEX2BIN_PATH} {args.in_file} {intermediate_primary_binary}")
    hexfile_content_offset = get_hexfile_content_offset(args, args.in_file)
    compress_firmware(args, intermediate_primary_binary,
                      intermediate_primary_compressed_binary_path, hexfile_content_offset, args.flash_app_start_offset)
    if args.secondary_input_file:
        secondary_base_path = os.path.splitext(args.secondary_input_file)[0]
        intermediate_secondary_binary = f"{secondary_base_path}.bin"
        intermediate_secondary_compressed_binary_path = f"{secondary_base_path}.compressed.bin"
        run_script(f"{HEX2BIN_PATH} {args.secondary_input_file} {intermediate_secondary_binary}")
        hexfile_content_offset = get_hexfile_content_offset(args, args.secondary_input_file)
        compress_firmware(args, intermediate_secondary_binary,
                          intermediate_secondary_compressed_binary_path, hexfile_content_offset, args.flash_secondary_start_offset)

    return intermediate_primary_compressed_binary_path, intermediate_secondary_compressed_binary_path


def bundle_ota_image(args: GenerateOtaImageArguments, intermediate_primary_compressed_binary_path: str, intermediate_secondary_compressed_binary_path: Optional[str]):
    """Bundle the final OTA image from the input binaries"""
    run_script(f"{BUNDLEOTAIMAGE_PATH}" +
               (f" --chip_config_header {args.chip_config_header} " if args.chip_config_header else '') +
               (f" --factory_data_config {args.factory_data_config}" if args.factory_data_config else '') +
               (f" --se_image_file {args.se_image_file}" if args.se_image_file else '') +
               (f" --bl_image_file {args.bl_image_file}" if args.bl_image_file else '') +
               f" --chip_root {args.chip_root}" +
               f" --input_file {intermediate_primary_compressed_binary_path}" +
               (f" --secondary_input_file {intermediate_secondary_compressed_binary_path}" if intermediate_secondary_compressed_binary_path else '') +
               f" --out_file {args.out_file}" +
               (f" --version {args.version}" if args.version else '') +
               (f" --min_version {args.min_version}" if args.min_version else '') +
               (f" --max_version {args.max_version}" if args.max_version else '') +
               (f" --version_str {args.version_str}" if args.version_str else '') +
               (f" {'--test_update_image' if args.test_update_image else ''}") +
               (f" --vendor_id {args.vendor_id}" if args.vendor_id else '') +
               (f" --product_id {args.product_id}" if args.product_id else '') +
               (f" --developer-key {args.developer_key}" if args.developer_key else '') +
               (f" --pem_file_path {args.pem_file_path}" if args.pem_file_path else '') +
               (f" --pem_password {args.pem_password}" if args.pem_password else ''))


def create_ota_package(args: GenerateOtaImageArguments, intermediate_primary_compressed_binary_path: str, intermediate_secondary_compressed_binary_path: Optional[str]):
    """Package together hex files for SE, bootloader and applications and sign with Qorvo OTA package header"""
    input_base_path = os.path.splitext(intermediate_primary_compressed_binary_path)[0]
    ota_package_path = f"{input_base_path}.ota.package"
    pack_images(app_input_file=intermediate_primary_compressed_binary_path, se_input_file=args.se_image_file, bl_input_file=args.bl_image_file,
                header_version=args.header_version, key=args.pem_file_path, password=args.pem_password, developer_key=args.developer_key,
                output=ota_package_path, secondary_input_file=intermediate_secondary_compressed_binary_path)
    return ota_package_path


def main():
    """ Main """

    logging.basicConfig(level=logging.INFO)

    args = parse_command_line_arguments()

    if args.pem_file_path is not None and args.pem_password is None:
        # Newlines were added to accentuate the password prompt in the middle of
        # make output
        prompt = "\n\n" + f"PEM password for {os.path.abspath(args.pem_file_path)}:" + "\n\n"
        args.pem_password = getpass.getpass(prompt=prompt)

    # Qorvo specific OTA preparation
    intermediate_primary_compressed_binary_path, intermediate_secondary_compressed_binary_path = compress_ota_payload(
        args)

    # Print the size of the intermediate compressed binaries
    primary_size = os.path.getsize(intermediate_primary_compressed_binary_path)
    logging.info("Size of intermediate primary compressed binary: %d bytes", primary_size)

    if intermediate_secondary_compressed_binary_path:
        secondary_size = os.path.getsize(intermediate_secondary_compressed_binary_path)
        logging.info("Size of intermediate secondary compressed binary: %d bytes", secondary_size)
    bundle_ota_image(args, intermediate_primary_compressed_binary_path, intermediate_secondary_compressed_binary_path)


if __name__ == "__main__":
    main()
