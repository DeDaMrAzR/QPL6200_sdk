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
Bundle input hex files into Matter ota image
"""


@dataclass
class BundleOtaImageArguments:
    """helper to enforce type checking on argparse output"""
    chip_config_header: str
    factory_data_config: str
    se_image_file: str
    bl_image_file: str
    chip_root: str
    input_file: str
    out_file: str
    version: int
    min_version: int
    max_version: int
    version_str: str
    test_update_image: bool
    vendor_id: str
    product_id: str
    header_version: int
    secondary_input_file: str
    pem_file_path: str
    pem_password: str
    developer_key: str
    get_hash: bool
    inject_signature: str
    inject_signature_format: str


def parse_command_line_arguments() -> BundleOtaImageArguments:
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
    parser.add_argument('-vs', '--version_str', help='Software version (string)', default=None)
    parser.add_argument('-tui', '--test_update_image', action='store_true',
                        help='Extract incremented versions for OTA update test', default=False)
    parser.add_argument('-vid', '--vendor_id', help='Vendor ID (string)', default=None)
    parser.add_argument('-pid', '--product_id', help='Product ID (string)', default=None)
    parser.add_argument('--header_version', type=any_base_int, help='Qorvo Image Header version (numeric)', default=1)
    parser.add_argument("--secondary_input_file",
                        help="Path to input file for secondary application to format to Matter OTA fileformat")
    parser.add_argument('--pem_file_path', help='PEM file path (string)', default=None)
    parser.add_argument('--pem_password', help='PEM file password (string)', default=None)
    parser.add_argument('--developer-key', help='Path to the developer private key', default=None)
    parser.add_argument("--get_hash",
                        help="Get a SHA256 hash (in binary format) of the OTA header to sign externally",
                        action='store_true')
    parser.add_argument("--inject_signature", type=str, help="Inject a signature into the OTA header")
    parser.add_argument("--inject_signature_format", type=str, choices=["raw", "asn1"], default="asn1",
                        help="The format of the signature to inject (defaults to ASN.1)")

    requiredArgGroup = parser.add_argument_group('required arguments')
    requiredArgGroup.add_argument("--out_file",
                                  help="Path to output file (.ota file)", required=True)
    requiredArgGroup.add_argument("--input_file",
                                  help="Path to input file to format to Matter OTA fileformat", required=True)
    requiredArgGroup.add_argument("--chip_root",
                                  help="Path to root Matter directory", required=True)

    logging.info(f"{sys.argv[0]} -> provided args : {sys.argv[1:]}")
    args = parser.parse_args()

    if args.developer_key and args.inject_signature:
        raise ValueError("Cannot use both --developer_key and --inject-signature options simultaneously.")

    return BundleOtaImageArguments(**vars(args))


def run_script(command: str):
    """ run a python script using the current interpreter """
    assert command != ""
    logging.info(f"{command}")
    subprocess.check_output(f"{sys.executable} {command}", shell=True)


def extract_vid_and_pid(factory_data_config: str) -> Tuple[str, str]:
    """ Determine the product-id and vendor-id from the factory data config
    """
    product_id = None
    vendor_id = None

    if not os.path.isfile(factory_data_config):
        logging.warning(f"Unable to find file factory_data_config {factory_data_config} to readout product info", )
        return (product_id, vendor_id)

    with open(factory_data_config, 'r', encoding='utf-8') as fd_fact_data:
        for line in fd_fact_data.readlines():
            if "--vendor-id" in line:
                vendor_id = line.split("=")[1].strip("\n")
            if "--product-id" in line:
                product_id = line.split("=")[1].strip("\n")

    return (vendor_id, product_id)


def extract_versions(chip_config_header: str, test_update_image: bool) -> Tuple[str, str, str, str]:
    """ Determine version number/version string from a CHIP project's headers
    """
    version_number = None
    version_str = None

    if not os.path.isfile(chip_config_header):
        logging.warning(f"Unable to find file chip_config_header: {chip_config_header} to readout version info")
        return (version_number, version_str)

    with open(chip_config_header, 'r', encoding='utf-8') as config_file:
        lines = config_file.read()

        # Define the pattern based on the value of use_first_string
        version_pattern = r"#define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION\s+(0x[0-9a-fA-F]+)"
        version_string_pattern = r"#define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING\s+\"([^\"]+)"

        # Use re.search to find the matching patterns
        result = []
        for regex in (version_pattern, version_string_pattern):
            matches = re.findall(regex, lines)
            if len(matches) == 2:
                # Extract the version based on the matching group - first one (0) for normal image, second one (1) for OTA image
                result.append(matches[1 if test_update_image else 0])
            else:
                logging.error("CHIPProjectConfig.h file not properly structured (wrong version defines) - "
                              "need two versions, one for normal, one for OTA upgrade image")
                return ()

        # Extract min and max applicable versions (if defined)
        min_version_pattern = r"#define CHIP_DEVICE_CONFIG_MIN_APPLICABLE_SOFTWARE_VERSION\s+(0x[0-9a-fA-F]+)"
        max_version_pattern = r"#define CHIP_DEVICE_CONFIG_MAX_APPLICABLE_SOFTWARE_VERSION\s+(0x[0-9a-fA-F]+)"
        min_version_matches = re.findall(min_version_pattern, lines)
        max_version_matches = re.findall(max_version_pattern, lines)

        if len(min_version_matches) == 1:
            result.append(min_version_matches[0])
        else:
            result.append(None)
        if len(max_version_matches) == 1:
            result.append(max_version_matches[0])
        else:
            result.append(None)

    return tuple(result)


def determine_example_project_config_header(args: BundleOtaImageArguments):
    """ Determine the CHIPProjectConfig.h path of a matter-sourcetree based example application."""
    if 'lighting' in args.input_file:
        project_name = 'lighting-app'
    elif 'lock' in args.input_file:
        project_name = 'lock-app'
    elif 'persistent' in args.input_file:
        project_name = 'persistent-storage'
    elif 'shell' in args.input_file:
        project_name = 'shell'
    elif 'light-switch' in args.input_file:
        project_name = 'light-switch-app'
    elif 'thermostat' in args.input_file:
        project_name = 'thermostat'
    elif 'contact-sensor' in args.input_file:
        project_name = 'contact-sensor-app'
    elif 'occupancy-sensor' in args.input_file:
        project_name = 'occupancy-sensor-app'
    else:
        raise ValueError(f"Unable to deduce which example project {args.input_file} belongs to!")

    return f"{args.chip_root}/examples/{project_name}/qpg/include/CHIPProjectConfig.h"


def determine_product_values(args: BundleOtaImageArguments) -> Tuple[str, str]:
    """ Decide which Vendor ID, Product ID the user wants to use
    """
    used_factory_data_config = args.factory_data_config

    (vendor_id, product_id) = extract_vid_and_pid(used_factory_data_config)

    if args.vendor_id:
        logging.warning(f"Vendor ID from {used_factory_data_config} overruled by argument of {__file__}")
        vendor_id = args.vendor_id

    if args.product_id:
        logging.warning(f"Product ID from {used_factory_data_config} overruled by argument of {__file__}")
        product_id = args.product_id

    if vendor_id is None:
        raise ValueError(f"No Product ID provided as argument to {__file__} or found in {used_factory_data_config}")

    if product_id is None:
        raise ValueError(f"No Vendor ID provided as argument to {__file__} or found in {used_factory_data_config}")

    logging.info(f"Found Vendor ID:{vendor_id} Product ID:{product_id}")

    return (vendor_id, product_id)


def determine_version_values(args: BundleOtaImageArguments) -> Tuple[str, str]:
    """ Decide which Version number and string the user wants to use
    """
    used_chip_config_header = args.chip_config_header or determine_example_project_config_header(args)

    (version, version_str, min_version, max_version) = extract_versions(used_chip_config_header,
                                                                        args.test_update_image)

    if args.version:
        logging.warning(f"Version from {used_chip_config_header} overruled by argument of {__file__}")
        version = args.version

    if args.version_str:
        logging.warning(f"Version string from {used_chip_config_header} overruled by argument of {__file__}")
        version_str = args.version_str

    if args.min_version:
        if min_version:
            logging.warning(
                f"Minimum applicable software version from {used_chip_config_header} overruled by argument of {__file__}")
        min_version = args.min_version

    if args.max_version:
        if max_version:
            logging.warning(
                f"Maximum applicable software version from {used_chip_config_header} overruled by argument of {__file__}")
        max_version = args.max_version

    if version is None:
        raise ValueError(f"No SW version provided as argument to {__file__} or found in {used_chip_config_header}")

    if version_str is None:
        raise ValueError(
            f"No SW version string provided as argument to {__file__} or found in {used_chip_config_header}")

    logging.info(f"Found Version:{version} Version str:{version_str}")

    return (version, version_str, min_version, max_version)


def create_ota_package(args: BundleOtaImageArguments):
    """Package together hex files for SE, bootloader and applications and sign with Qorvo OTA package header"""
    input_base_path = os.path.splitext(args.input_file)[0]
    ota_package_path = f"{input_base_path}.ota.package"
    pack_images(app_input_file=args.input_file, se_input_file=args.se_image_file, bl_input_file=args.bl_image_file, header_version=args.header_version,
                key=args.pem_file_path, password=args.pem_password, developer_key=args.developer_key, get_hash=args.get_hash, inject_signature=args.inject_signature,
                inject_signature_format=args.inject_signature_format, output=args.out_file if args.get_hash else ota_package_path, secondary_input_file=args.secondary_input_file)
    return ota_package_path


def main():
    """ Main """

    logging.basicConfig(level=logging.INFO)

    args = parse_command_line_arguments()

    # Pack application + bootloader + se in OTA package
    ota_package_path = create_ota_package(args)

    # If only the hash is requested, we can stop here
    if args.get_hash:
        logging.info(f"Written hash to OTA header hash to: {args.out_file}")
        return

    # Parse Product values from application factory data config or CLI args
    (vendor_id, product_id) = determine_product_values(args)

    # Parse version values from application ProjectConfig header or CLI args
    (version, version_str, min_version, max_version) = determine_version_values(args)

    # Matter header wrapping
    tool_args = f"create -v {vendor_id} -p {product_id} -vn {version} -vs {version_str}"
    if min_version:
        tool_args += f" -mi {min_version}"
    if max_version:
        tool_args += f" -ma {max_version}"
    tool_args += " -da sha256 "

    run_script(f"{args.chip_root}/src/app/ota_image_tool.py {tool_args} {ota_package_path} {args.out_file}")


if __name__ == "__main__":
    main()
