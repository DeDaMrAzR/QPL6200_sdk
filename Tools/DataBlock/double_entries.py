#!/usr/bin/env python3

import sys


def doubleEntries(argv):
    inPsbFile = argv[0]
    outPsbFile = argv[1]

    f = open(inPsbFile, "r")
    input = f.readlines()
    f.close()

    f = open(outPsbFile, "w")
    for line in input:
        f.write(line)
        if "#" not in line:
            # Add duplicate in pool 2
            newline = line.replace("1 ", "2 ", 1)
            f.write(newline)
    f.close()


if __name__ == "__main__":
    doubleEntries(sys.argv[1:])
