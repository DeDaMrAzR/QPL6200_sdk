#!/usr/bin/env python3

import sys
from intelhex import IntelHex

BOOTLOADER_MAILBOX_BOOT_ADDRESS_OFFSET = (4)


def gen_bl_metadata(argv):
    output_hex = argv[0]

    app_address = int(argv[1])
    upgrade_image_user_license_addr = int(argv[2])

    intel_hex = IntelHex()
    intel_hex.frombytes(app_address.to_bytes(4, 'little'),
                        offset=upgrade_image_user_license_addr + BOOTLOADER_MAILBOX_BOOT_ADDRESS_OFFSET)
    intel_hex.tofile(output_hex, format='hex')


if __name__ == "__main__":
    gen_bl_metadata(sys.argv[1:])
