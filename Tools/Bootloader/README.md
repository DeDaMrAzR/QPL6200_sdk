# Bootloader Metadata Generator

**NOTE: bootloader metadata is no longer existed in bootloader OTA sub image as of version 2.0.2.**

The `gen_bl_metadata.py` tool is used to generate data used by the bootloader to select the bootable application and the OTA address. The output file is in `*.hex` format.

Example use:

```shell

gen_bl_metadata.py <out_file> <boot_addr> <metadata_addr>

```

Where:

- `<out_file>` - name of the output hex file containing the bootloader metadata

- `<boot_addr>` - address of the application to boot

- `<metadata_addr>` - address of the bootloader metadata section
