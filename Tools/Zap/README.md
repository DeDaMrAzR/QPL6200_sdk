# ZCL Advanced Platform (ZAP) Tool

## Abbreviations

- **ZCL**: ZigBee Cluster Library

## Purpose

The `Tools/Zap/generate_zap_files.py` tool generates ZAP files and corresponding source files used for development of
custom Matter&trade; applications. This tool defines the clusters used in the Matter application and forms the basis for
Matter application development.


## Implementation overview

This tool is implemented as a Python wrapper around the Matter ZAP tool. More details about this Matter tool can be found at [Official CSA ZAP Python Library](https://github.com/project-chip/zap/tree/master).

## Usage


```
python3 generate_zap_files.py --help
usage: generate_zap_files.py [-h] --input INPUT [--nogui]

1. Starting the ZAP GUI 2. Regenerate the .zap file 3. Generate corresponding source/header files

optional arguments:
      -h, --help       show this help message and exit
      --input INPUT    path to input .zap file
      --nogui          Add this option if it is not needed to do configuration in the gui
```

## Example

Set up the environment for the ZAP tool:

```
source Scripts/activate.sh
```

Bring up the ZAP tool for the Matter Light application:

```
python3 ./Tools/Zap/generate_zap_files.py --input Applications/Matter/Light/light.zap
```
