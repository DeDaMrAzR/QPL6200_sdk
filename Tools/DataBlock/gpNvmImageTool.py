#!/usr/bin/env python

#
# Copyright (c) 2018, 2021 Qorvo Inc
#
# Tool for reading/writing binary NVM images.
#
#
# This software is owned by Qorvo Inc
# and protected under applicable copyright laws.
# It is delivered under the terms of the license
# and is intended and supplied for use solely and
# exclusively with products manufactured by
# Qorvo Inc.
#
#
# THIS SOFTWARE IS PROVIDED IN AN "AS IS"
# CONDITION. NO WARRANTIES, WHETHER EXPRESS,
# IMPLIED OR STATUTORY, INCLUDING, BUT NOT
# LIMITED TO, IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
# QORVO INC. SHALL NOT, IN ANY
# CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
# INCIDENTAL OR CONSEQUENTIAL DAMAGES,
# FOR ANY REASON WHATSOEVER.
#
#


from __future__ import print_function

import sys
import collections
import optparse
import struct

import six

SUPPORTED_FORMATS = ["subpagedflash_v2_qpg6200"]

MAX_VALID_UPDATEFREQ = 4

NVM_SECTOR_HEADER_VALIDFLAG_VALUE = 0xC3
NVM_SECTOR_HEADER_ERASEPENDING_VALUE = 0xEE
NVM_MASKED_SEQNUM_MAX = 0x7fff

SubPagedFlashFormatParams = collections.namedtuple("SubPagedFlashFormatParams", "pageSize pagesPerSector writeUnit")

FORMAT_PARAMS_SUBPAGEDFLASH_V2_QPG6200 = SubPagedFlashFormatParams(pageSize=512, pagesPerSector=2, writeUnit=16)

NvmItem = collections.namedtuple("NvmItem", "poolId updateFreq token data")


def _align_up(v, alignment):
    return v + (alignment - v) % alignment


def _write_uint16(v):
    d = bytearray(2)
    d[0] = v & 0xff
    d[1] = (v >> 8) & 0xff
    return d


def _read_uint16(d):
    assert len(d) == 2
    return d[0] + (d[1] << 8)


def _crc16(data):
    crc = 0
    for b in data:
        crc ^= b
        for i in range(8):
            crc = (crc >> 1) ^ (crc & 1) ^ ((crc & 1) << 13) ^ ((crc & 1) << 15)
    return crc


def _analyze_subpaged_flash_v2_pool(rawdata, fmtparams, poolId, redundantsectors):
    num_valid = 0
    num_corrupt = 0

    sector_size = fmtparams.pageSize * fmtparams.pagesPerSector
    sector_header_size = _align_up(4, fmtparams.writeUnit)
    record_header_size = 8

    physical_sectors = len(rawdata) // sector_size

    log_sector_map = {}

    all_items = []

    for physical_sector in range(physical_sectors):

        sector_offset = physical_sector * sector_size
        corrupt = False

        # Decode sector header
        valid_flag = rawdata[sector_offset]
        logical_sector = rawdata[sector_offset + 1]
        update_frequency = rawdata[sector_offset + 2]
        erase_flag = rawdata[sector_offset + 3]

        if valid_flag != NVM_SECTOR_HEADER_VALIDFLAG_VALUE:
            # invalid sector
            continue

        if erase_flag == NVM_SECTOR_HEADER_ERASEPENDING_VALUE:
            # sector pending erase
            continue

        # Verify that sector header is replicated on each page within the sector.
        for page in range(fmtparams.pagesPerSector):
            page_offset = sector_offset + page * fmtparams.pageSize
            if (valid_flag != rawdata[page_offset] != valid_flag or
                    logical_sector != rawdata[page_offset + 1] or
                    update_frequency != rawdata[page_offset + 2] or
                    erase_flag != rawdata[page_offset + 3]):
                print("WARNING: Corrupt phy sector %d in pool %d (mismatching page header)" % (physical_sector, poolId),
                      file=sys.stderr)
                corrupt = True
                break

        if corrupt:
            num_corrupt += 1
            continue

        sector_items = []

        # Read items from sector.
        for page in range(fmtparams.pagesPerSector):
            page_offset = sector_offset + page * fmtparams.pageSize
            pos = sector_header_size

            while pos + record_header_size <= fmtparams.pageSize:
                # Read record header
                crc = _read_uint16(rawdata[page_offset + pos:page_offset + pos + 2])
                token_length = rawdata[page_offset + pos + 2]
                update_frequency = rawdata[page_offset + pos + 4]
                data_length = rawdata[page_offset + pos + 5]
                sequence_number = _read_uint16(rawdata[page_offset + pos + 6:page_offset + pos + 8])

                if not token_length:
                    # past last record in page
                    break

                if pos + record_header_size + token_length + data_length > fmtparams.pageSize:
                    print("WARNING: Corrupt phy sector %d in pool %d (record crossing page boundary)" % (
                        physical_sector, poolId), file=sys.stderr)
                    corrupt = True
                    break

                token = rawdata[
                    page_offset + pos + record_header_size:page_offset + pos + record_header_size + token_length]
                data = rawdata[
                    page_offset + pos + record_header_size + token_length:page_offset + pos + record_header_size + token_length + data_length]

                if crc != _crc16(rawdata[
                                 page_offset + pos + 2:page_offset + pos + record_header_size + token_length + data_length]):
                    print("WARNING: Corrupt phy sector %d in pool %d (CRC mismatch)" % (physical_sector, poolId),
                          file=sys.stderr)
                    corrupt = True
                    break

                # Store item in sector item cache.
                sector_items.append((sequence_number, NvmItem(poolId=poolId, updateFreq=update_frequency,
                                                              token=str(token), data=str(data))))

                # Skip to next record.
                pos = _align_up(pos + record_header_size + token_length + data_length, fmtparams.writeUnit)

            if corrupt:
                break

        if corrupt:
            num_corrupt += 1
            continue

        # This sector is valid. Add to logical sector map.
        num_valid += 1
        if logical_sector not in log_sector_map:
            log_sector_map[logical_sector] = []
        log_sector_map[logical_sector].append(physical_sector)

        # Add sector items to the global list.
        all_items += sector_items

    # Check that each logical sector has at least 2 identical physical sectors.
    for logical_sector in list(log_sector_map.keys()):
        if len(log_sector_map[logical_sector]) < redundantsectors:
            print("WARNING: Logical sector %d has only %d physical sectors in pool %d" % (
                logical_sector, len(log_sector_map[logical_sector]), poolId), file=sys.stderr)
        if redundantsectors > 1:
            physical_sector_0 = log_sector_map[logical_sector][0]
            for physical_sector in log_sector_map[logical_sector][1:]:
                if rawdata[physical_sector_0 * sector_size:(physical_sector_0 + 1) * sector_size] != \
                        rawdata[physical_sector * sector_size:(physical_sector + 1) * sector_size]:
                    print("WARNING: Logical sector %d has mismatching physical sectors %d and %d in pool %d" % (
                        logical_sector, physical_sector_0, physical_sector, poolId), file=sys.stderr)

    # Figure out how to interpret sequence numbers (wrapping or non-wrapping).
    wrapping = True
    non_wrapping = True
    for (sequence_number, item) in six.iteritems(all_items):
        if sequence_number > 3 * NVM_MASKED_SEQNUM_MAX // 4:
            non_wrapping = False
        if NVM_MASKED_SEQNUM_MAX // 4 < sequence_number <= 3 * NVM_MASKED_SEQNUM_MAX // 4:
            wrapping = False
    if (not wrapping) and (not non_wrapping):

        print("ERROR: Sequence numbers in pool %d are ambiguous" % poolId, file=sys.stderr)
        sys.exit(-1)
    if non_wrapping:
        wrapping = False

    # Take the most recent occurrence of each item.
    item_cache = {}
    for (sequence_number, item) in six.iteritems(all_items):
        ext_sequence_number = sequence_number
        if wrapping and sequence_number <= NVM_MASKED_SEQNUM_MAX // 4:
            ext_sequence_number += NVM_MASKED_SEQNUM_MAX + 1
        if item.token not in item_cache:
            item_cache[item.token] = (ext_sequence_number, item)
        else:
            (seqCached, itemCached) = item_cache[item.token]
            if ext_sequence_number > seqCached:
                item_cache[item.token] = (ext_sequence_number, item)

    # Extract final list of items.
    items = [item for (sequence_number, item) in list(item_cache.values())]

    print("Pool %d: %d valid sectors, %d corrupt sectors, %d items" % (poolId, num_valid, num_corrupt, len(items)),
          file=sys.stderr)
    return items


def _analyze_subpaged_flash_v2(raw_data, format_parameters, num_sectors, redundant_sectors):
    sector_size = format_parameters.pageSize * format_parameters.pagesPerSector

    if len(raw_data) % sector_size != 0:
        print("ERROR: NVM data file size %d is not a multiple of the sector size %d" % (len(raw_data), sector_size),
              file=sys.stderr)
        sys.exit(-1)

    total_sectors = len(raw_data) // sector_size

    if num_sectors == [None]:
        num_sectors = [total_sectors]
    else:
        if total_sectors != sum(num_sectors):
            print("ERROR: NVM data file contains %d sectors but expecting %s sectors" % (
                total_sectors, '+'.join(map(str, num_sectors))), file=sys.stderr)
            sys.exit(-1)

    items = []
    offs = 0
    for poolId in range(len(num_sectors)):
        pool_start = offs
        pool_end = offs + num_sectors[poolId] * sector_size
        pool_items = _analyze_subpaged_flash_v2_pool(raw_data[pool_start:pool_end], format_parameters, poolId,
                                                     redundant_sectors)
        items += pool_items
        offs = pool_end

    return items


def _generate_subpaged_flash_v2_pool(items, format_parameters, num_sectors, redundant_sectors):
    sector_size = format_parameters.pageSize * format_parameters.pagesPerSector
    sector_header_size = _align_up(4, format_parameters.writeUnit)
    record_header_size = 8

    raw_data = bytearray(num_sectors * sector_size)

    logical_sectors = num_sectors // redundant_sectors

    next_logical_sector = 0
    next_sequence_number = 0

    # Group elements by update frequency.
    items_by_update_freq = {}
    for item in items:
        if item.updateFreq not in items_by_update_freq:
            items_by_update_freq[item.updateFreq] = []
        items_by_update_freq[item.updateFreq].append(item)

    # Process each update frequency.
    for update_freq in sorted(items_by_update_freq.keys()):

        item_position = 0

        # Write logical sectors until all items are stored.
        while item_position < len(items_by_update_freq[update_freq]):

            # Allocate a new logical sector.
            if next_logical_sector >= logical_sectors:
                print("ERROR: Not enough sectors to write items in pool %d" %
                      items_by_update_freq[update_freq][0].poolId, file=sys.stderr)
                sys.exit(-1)
            logical_sector = next_logical_sector
            next_logical_sector += 1

            # Allocate redundant physical sectors
            physical_sector = []
            for x in range(redundant_sectors):
                physical_sector.append(logical_sector + logical_sectors * x)

            # Write first physical sector.
            sector_offset = physical_sector[0] * sector_size

            # Write sector header to each page in this sector.
            for page in range(format_parameters.pagesPerSector):
                page_offset = sector_offset + page * format_parameters.pageSize
                raw_data[page_offset] = NVM_SECTOR_HEADER_VALIDFLAG_VALUE
                raw_data[page_offset + 1] = logical_sector
                raw_data[page_offset + 2] = update_freq
                raw_data[page_offset + 3] = 0  # erase flag

            # Write items to this sector.
            for page in range(format_parameters.pagesPerSector):
                page_offset = sector_offset + page * format_parameters.pageSize
                pos = sector_header_size

                while item_position < len(items_by_update_freq[update_freq]):

                    item = items_by_update_freq[update_freq][item_position]
                    token_length = len(item.token)
                    data_length = len(item.data)

                    # Sanity check on item size
                    if (token_length == 0) or (token_length > 255) or (data_length > 255) or (
                            token_length + data_length + record_header_size >= format_parameters.pageSize):
                        print("ERROR: Invalid item size token_length=%d data_length=%d" % (token_length, data_length),
                              file=sys.stderr)
                        sys.exit(-1)

                    if pos + record_header_size + token_length + data_length > format_parameters.pageSize:
                        # Item does not fit in this page anymore. Go to next page.
                        break

                    # Allocate sequence number.
                    sequence_number = next_sequence_number
                    next_sequence_number += 1

                    # Write record.
                    raw_data[page_offset + pos + 2] = token_length
                    raw_data[page_offset + pos + 4] = update_freq
                    raw_data[page_offset + pos + 5] = data_length
                    raw_data[page_offset + pos + 6:page_offset + pos + 8] = _write_uint16(sequence_number)
                    raw_data[page_offset + pos + record_header_size:page_offset + pos +
                             record_header_size + token_length] = bytearray(item.token)
                    raw_data[page_offset + pos + record_header_size + token_length:page_offset +
                             pos + record_header_size + token_length + data_length] = bytearray(item.data)

                    # Write CRC.
                    crc = _crc16(raw_data[page_offset + pos + 2:page_offset + pos +
                                          record_header_size + token_length + data_length])
                    raw_data[page_offset + pos:page_offset + pos + 2] = _write_uint16(crc)

                    # Skip to next record.
                    pos = _align_up(pos + record_header_size + token_length + data_length, format_parameters.writeUnit)

                    item_position += 1

            # Copy to redundant physical sectors.
            for x in range(redundant_sectors):
                if x != 0:
                    sector_offset_red = physical_sector[x] * sector_size
                    raw_data[sector_offset_red:sector_offset_red + sector_size] = raw_data[
                        sector_offset:sector_offset + sector_size]

    return raw_data


def _generate_subpaged_flash_v2(items, format_parameters, num_sectors, redundant_sectors):
    sector_size = format_parameters.pageSize * format_parameters.pagesPerSector

    raw_data = bytearray(0)

    for pool_id in range(len(num_sectors)):
        pool_items = [item for item in items if item.poolId == pool_id]
        pool_data = _generate_subpaged_flash_v2_pool(pool_items, format_parameters,
                                                     num_sectors[pool_id], redundant_sectors)
        raw_data += pool_data

    assert len(raw_data) == sum(num_sectors) * sector_size
    return raw_data


def read_items(in_file, num_pools):
    """Read NVM content items from text file.

    Expected format: <poolid> <updfreq> <token_length> (token_length * <byteval>) <data_length> (data_length * <byteval>)

    All numbers specified as decimal integers or as hexadecimal integers with "0x" prefix.
    Numbers separated by white space.

    <poolid> must be in range 0 .. GP_NVM_NBR_OF_POOLS
    <token_length> must be in range 1 .. 255.
    <data_length> must be in range 1 .. 65535.
    <updfreq> must be in range 0 .. 4.

    Empty lines and lines starting with "#" are ignored.
    """

    if in_file is None:
        inf = sys.stdin
    else:
        inf = open(in_file, "r")

    items = []
    line_number = 0

    for line in inf:

        line_number += 1

        if line.strip().startswith("#"):
            continue

        words = line.strip().split()
        if len(words) == 0:
            continue

        values = [int(w, 0) for w in words]
        p = 0

        pool_id = values[p]
        p += 1
        if pool_id < 0 or pool_id >= num_pools:
            print("ERROR: line %d, Invalid <pool_id> %s (num_pools %d)" % (line_number, pool_id, num_pools),
                  file=sys.stderr)
            sys.exit(-1)

        if p >= len(values):
            print("ERROR: line %d, Missing <updfreq>" % line_number, file=sys.stderr)
            sys.exit(-1)

        update_frequency = values[p]
        p += 1
        if update_frequency < 0 or update_frequency > MAX_VALID_UPDATEFREQ:
            print("ERROR: line %d, Invalid <updfreq> %d" % (line_number, update_frequency), file=sys.stderr)
            sys.exit(-1)

        if p >= len(values):
            print("ERROR: line %d, Missing <token_length>" % line_number, file=sys.stderr)
            sys.exit(-1)

        token_length = values[p]
        p += 1

        if p + token_length > len(values):
            print("ERROR: line %d, Not enough values on line to fill token length" % line_number, file=sys.stderr)
            sys.exit(-1)

        token = bytearray(token_length)
        for i in range(token_length):
            if values[p] < 0 or values[p] > 255:
                print("ERROR: line %d, Invalid value %d for token byte" % (line_number, values[p]), file=sys.stderr)
                sys.exit(-1)
            token[i] = values[p]
            p += 1

        if p >= len(values):
            print("ERROR: line %d, Missing <data_length>" % line_number, file=sys.stderr)
            sys.exit(-1)

        data_length = values[p]
        p += 1

        if p + data_length > len(values):
            print("ERROR: line %d, Not enough values on line to fill data length" % line_number, file=sys.stderr)
            sys.exit(-1)

        data = bytearray(data_length)
        for i in range(data_length):
            if values[p] < 0 or values[p] > 255:
                print("ERROR: line %d, Invalid value %d for data byte" % (line_number, values[p]), file=sys.stderr)
                sys.exit(-1)
            data[i] = values[p]
            p += 1

        if p < len(values):
            print("ERROR: line %d, Too many values on line" % line_number, file=sys.stderr)
            sys.exit(-1)

        item = NvmItem(poolId=pool_id, updateFreq=update_frequency, token=token, data=data)
        items.append(item)

    if in_file is not None:
        inf.close()

    return items


def _format_bytes(data):
    return " ".join(["0x%02x" % ord(d) for d in data])


def write_items(outfile, items, number_of_pools):
    """Write NVM content items to text file."""

    if outfile is None:
        out_file = sys.stdout
    else:
        out_file = open(outfile, "w")

    out_file.write("# NVM items\n")

    for poolId in range(number_of_pools):
        pool_items = [item for item in items if item.poolId == poolId]

        out_file.write("# Pool ID %d, %d items\n" % (poolId, len(pool_items)))
        out_file.write("# PoolId UpdateFreq TokenLen TokenBytes DataLen DataBytes\n")

        for item in pool_items:
            out_file.write("%s %s %s %s %s %s\n" % (
                item.poolId, item.updateFreq, len(item.token), _format_bytes(item.token),
                len(item.data), _format_bytes(item.data)
            ))

        out_file.write("\n")

    if outfile is not None:
        out_file.close()


def read_nvm(infile, format_string, num_sectors, redundant_sectors):
    """Read NVM binary image file and return a list of content items."""

    with open(infile, "rb") as f:
        raw_data = f.read()

    if format_string.lower() == "subpagedflash_v2_qpg6200":
        items = _analyze_subpaged_flash_v2(raw_data=raw_data, format_parameters=FORMAT_PARAMS_SUBPAGEDFLASH_V2_QPG6200,
                                           num_sectors=num_sectors, redundant_sectors=redundant_sectors)
    else:
        assert False

    return items


def write_nvm(outfile, items, format_string, num_sectors, redundant_sectors):
    """Generate binary NVM image from list of content items and write to file."""

    if format_string.lower() == "subpagedflash_v2_qpg6200":
        raw_data = _generate_subpaged_flash_v2(items=items, format_parameters=FORMAT_PARAMS_SUBPAGEDFLASH_V2_QPG6200,
                                               num_sectors=num_sectors, redundant_sectors=redundant_sectors)
    else:
        assert False

    with open(outfile, "wb") as f:
        f.write(raw_data)


def _nvm_synth(argv):
    usage = "%prog [options] {--generate|--analyze}"
    desc = "Tool for reading/writing binary NVM images."

    parser = optparse.OptionParser(usage=usage,
                                   description=desc)
    parser.add_option("--generate",
                      action="store_true",
                      help="Generate NVM binary image from list of items.")
    parser.add_option("--analyze",
                      action="store_true",
                      help="Analyze NVM binary image and output list of items.")
    parser.add_option("--nvmin",
                      action="store",
                      help="Input file with binary NVM image (for --analyze).")
    parser.add_option("--nvmout",
                      action="store",
                      help="Output file to store binary NVM image (for --generate).")
    parser.add_option("--format",
                      action="store",
                      help="Low-level NVM format. Use '--format help' for a list of supported format.")
    parser.add_option("--itemsin",
                      action="store",
                      help="Input file with content items (text format). Default is to read from stdin.")
    parser.add_option("--itemsout",
                      action="store",
                      help="Output file for content items (text format). Default is to write to stdout.")
    parser.add_option("--numpools",
                      action="store",
                      type=int,
                      default=1,
                      help="Set number of pools (default 1).")
    parser.add_option("--numsectors",
                      action="store",
                      help="Set number of physical flash sectors per pool (e.g. --num_sectors=8,8,12).")
    parser.add_option("--redundantsectors",
                      action="store",
                      type=int,
                      default=2,
                      help="Set number of redundant sectors for data in NVM (default 2)")

    (options, args) = parser.parse_args(argv)

    if args:
        parser.print_usage()
        print("", file=sys.stderr)
        print("ERROR: Unexpected command line argument", file=sys.stderr)
        sys.exit(1)

    if (not options.generate) and (not options.analyze):
        parser.print_help()
        print("", file=sys.stderr)
        print("ERROR: Specify either --generate or --analyze", file=sys.stderr)
        sys.exit(1)

    if options.generate and options.nvmout is None:
        parser.print_help()
        print("", file=sys.stderr)
        print("ERROR: Missing option --nvmout", file=sys.stderr)
        sys.exit(1)

    if options.analyze and options.nvmin is None:
        parser.print_help()
        print("", file=sys.stderr)
        print("ERROR: Missing option --nvmin", file=sys.stderr)
        sys.exit(1)

    if options.format is None:
        parser.print_help()
        print("", file=sys.stderr)
        print("ERROR: Missing option --format", file=sys.stderr)
        sys.exit(1)

    if options.format.lower() not in SUPPORTED_FORMATS:
        print("", file=sys.stderr)
        print("ERROR: Unsupported format '%s'" % options.format, file=sys.stderr)
        print("Supported formats:", ", ".join(SUPPORTED_FORMATS), file=sys.stderr)
        sys.exit(1)

    if (options.generate or (options.numpools != 1)) and (not options.numsectors):
        parser.print_help()
        print("", file=sys.stderr)
        print("ERROR: Missing option --numsectors", file=sys.stderr)

    if options.numsectors:
        num_sectors = list(map(int, options.numsectors.split(',')))
    else:
        num_sectors = [None]

    if len(num_sectors) != options.numpools:
        print("", file=sys.stderr)
        print("ERROR: --num_sectors specifies %d pools but --numpools specifies %d pools" % (
            len(num_sectors), options.numpools), file=sys.stderr)
        sys.exit(1)

    if options.generate:
        items = read_items(in_file=options.itemsin, num_pools=options.numpools)
        write_nvm(outfile=options.nvmout, items=items, format_string=options.format, num_sectors=num_sectors,
                  redundant_sectors=options.redundantsectors)

    if options.analyze:
        items = read_nvm(infile=options.nvmin, format_string=options.format, num_sectors=num_sectors)
        write_items(outfile=options.itemsout, items=items, number_of_pools=options.numpools)


if __name__ == "__main__":
    _nvm_synth(sys.argv[1:])
