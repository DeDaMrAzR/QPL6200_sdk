# Hex files manipulation tools

We need to manipulate Intel HEX files on most stages of development and production.
They are necessary to prepare an application image or an OTA update.
To make this easier, we provide two tools for common Intel HEX file operations.
They are based on the open source [IntelHex](https://pypi.org/project/intelhex/) Python library.

- Hexfill - fills a specified region in an Intel HEX file with a byte value - default (0), or given.
- Hexmerge - merges multiple Intel HEX files into a single file.
- Hex2bin - converts an Intel Hex file into a binary file.


## Hexfill

Hexfill writes bytes to a region of an Intel HEX file.
On input, it takes a hex file, region address parameters, and an output filename.
By default, it fills this region with zeros.
Optionally, you can provide an integer byte fill value instead.


### Usage example

```shell
Usage: hexfill.py [OPTIONS] IN_FILE OUT_FILE

Options:
  -v, --value INTEGER            Byte value used to fill the address range.
  -s, --start INTEGER            Start address (inclusive).
  -e, --endex INTEGER            End address (exclusive).
  --ignore_start_execution_addr  Ignore start execution address.
  --help                         Show this message and exit.
```

### Example

```shell
python hexfill.py input.hex output.hex --value 0xAA --start 0x10013000 --endex 0x0x10020000
```

## Hexmerge

Hexmerge merges multiple Intel HEX files into a single Intel HEX file.
If the HEX files overlap, it returns an error by default.
Optionally, it can resolve overlap by keeping data from the preceding or succeeding file.
This overlap resolution applies to all files passed to Hexmerge in the argument.
Hexmerge accepts a single Intel HEX file on input, and passes it to the output.
The binary contents are unchanged.
Filename changes according to passed argument.

### Usage example

```shell
Usage: hexmerge.py [OPTIONS] OUTPUT_HEX [INPUT_HEX]...

Options:
  --overlap [error|keep_first|keep_last] Response to overlapping data in input.
  --ignore_start_execution_addr          Ignore start execution address.
  --help                                 Show this message and exit.
```

### Example

```shell
python hexmerge.py output.hex input_1.hex input_2.hex input_3.hex
```

## Hex2bin

Hex2bin converts an Intel Hex file into a binary file.
Optionally, it can convert files in opposite way, from a binary file to an Intel Hex file.
This tool can also change loading address for an Intel Hex output file, add additional bytes padding at the beginning of the file and converts only the given addresses range.

### Usage example

```shell
Usage: hex2bin.py [OPTIONS] INPUT_FILE OUTPUT_FILE

Options:
  --input_format [ihex|binary|hexstring]
                                  Input format
  --output_format [ihex|binary|hexstring]
                                  Input format
  --change-addresses, --change_addresses INTEGER
                                  Change loading address (only makes sense for
                                  ihex output)
  -p, --padding-byte INTEGER      use a padding byte
  -r, --addr-range TEXT           select a range, specify start:end addresses
  --help                          Show this message and exit.
```

### Example

```shell
python hex2bin.py input.hex output.bin
```
