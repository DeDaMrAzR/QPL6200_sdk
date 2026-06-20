#!/usr/bin/env python3

import sys
from intelhex import IntelHex


def installCode(argv):
    xml_file = argv[0]
    input_file = argv[1]
    output_file = argv[2]

    ih = IntelHex()
    ih.loadhex(input_file)

    with open(xml_file, "rb") as infile:
        lines = infile.readlines()
        infile.close()

    for line in lines:
        strline = str(line)
        if "install-code" in strline:
            installcode = strline.split('>')[1].split('<')[0]
            print("Install Code found: %s" % installcode)
            break

    ih.puts(ih.maxaddr() + 1, bytes.fromhex(installcode))

    # Remove the start address record (type 05) if exists
    ih.start_addr = None

    ih.write_hex_file(output_file)


if __name__ == "__main__":
    installCode(sys.argv[1:])
