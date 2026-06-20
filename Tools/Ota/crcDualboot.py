#!/usr/bin/env python3
'''Add CRC to dual boot application for the bootloader to do integrety checking
'''

import argparse
import sys
import os
import io
import logging
from dataclasses import dataclass

moduleroot = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))

try:
    toolsroot = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
    sys.path.append(os.path.join(toolsroot, "Tools", "MemoryOverview"))
    from memoryusage import parseMemory
except ImportError:
    try:
        projroot = os.path.abspath(os.path.join(moduleroot, "..", ".."))
        sys.path.append(os.path.join(projroot, "Env"))
        from vless.gppy_vless.inf.getEnvVersion import getEnvVersion        # noqa

        versionXml = os.path.join(moduleroot, "gpVersion.xml")
        envpath = os.path.abspath(os.path.join(projroot, "Env", getEnvVersion(versionXml)))
        sys.path.append(envpath)

        from gppy.tools.memory.memoryusage import parseMemory
    except ImportError:
        print("Using outside of Env ?")

try:
    from crcFirmware import *
except ImportError:
    sys.path.append(os.path.join(moduleroot, 'apps', 'crc', 'src'))
    from crcFirmware import *

DESCRIPTION = """\
Adds crc to hex file required to verify application integrity when using dual bootloader
"""


@dataclass
class GenerateDualBootCrcArguments:
    """helper to enforce type checking on argparse output"""
    map_file: str
    compiler: str
    hex: str
    output: str
    crc_start_addr: str
    start_addr_area: str
    license_offset: str
    set_bootloader_loaded: bool


SCRIPT_PATH = os.path.dirname(__file__)
CRCFIRMWARE_PATH = f"{SCRIPT_PATH}/crcFirmware.py"
if not os.path.isfile(os.path.join(SCRIPT_PATH, "crypto_utils.py")):
    CRCFIRMWARE_PATH = os.getenv("QORVO_CRCFIRMWARE_PATH", CRCFIRMWARE_PATH)


def parse_command_line_arguments() -> GenerateDualBootCrcArguments:
    """Parse command-line arguments"""
    parser = argparse.ArgumentParser(description=DESCRIPTION)

    parser.add_argument("--hex",
                        help="path to hex file to be crced")
    parser.add_argument("--output",
                        help="output file to be written")
    parser.add_argument("--map_file",
                        dest="map_file",
                        default="",
                        help="Point to the map file of an application to analyze")
    parser.add_argument("--compiler",
                        dest="compiler",
                        default="",
                        help="compiler used for parsing the map file")
    parser.add_argument("--crc_start_addr",
                        help="optional: define start address for CRC calculation")
    parser.add_argument("--start_addr_area",
                        help="start address of app area")
    parser.add_argument("--license_offset",
                        help="offset relative to start_addr_area where the user license begins")
    parser.add_argument("--set_bootloader_loaded",
                        help="set USER_LICENSE_PROGRAM_LOADED_MAGIC_WORD",
                        action='store_true')

    logging.info(f"{sys.argv[0]} -> provided args : {sys.argv[1:]}")
    args = parser.parse_args()

    return GenerateDualBootCrcArguments(**vars(args))


def main():
    """ Main """

    logging.basicConfig(level=logging.INFO)

    args = parse_command_line_arguments()

    start_addr_area = int(args.start_addr_area, 16)

    info = parseMemory(args.map_file, args.compiler)
    appSize = info.segments["Flash"].sections['text'].size
    textAddress = info.segments["Flash"].sections['text'].address
    crcSize = appSize + textAddress - start_addr_area

    intel_hex_file = create_intel_hex_file_object(args.hex)

    if args.license_offset is not None:
        license_offset = int(args.license_offset, 16)
        license_address = start_addr_area + license_offset
    else:
        license_address = start_addr_area

    if args.set_bootloader_loaded:
        if 'license_address' not in locals():
            license_address = start_addr_area
        logging.info("populating USER_LICENSE_PROGRAM_LOADED_MAGIC_WORD at %#x",
                     license_address + USER_LICENSE_LOAD_COMPLETED_OFFSET)
        ihex_set_uint32(intel_hex_file,
                        license_address + USER_LICENSE_LOAD_COMPLETED_OFFSET,
                        USER_LICENSE_PROGRAM_LOADED_MAGIC_WORD)

    crc_start_addr = int(args.crc_start_addr, 16)
    add_dualboot_crc(intel_hex_file, crc_start_addr, hex(crcSize), license_address)

    if args.output:
        intel_hex_file.writePath = args.output
    # Overwrite the hex file with the modified (CRC'd) intel hex object
    logging.info("Writing Hex file: %s", intel_hex_file.writePath)
    intel_hex_file.tofile(intel_hex_file.writePath, format='hex')


if __name__ == "__main__":
    main()
