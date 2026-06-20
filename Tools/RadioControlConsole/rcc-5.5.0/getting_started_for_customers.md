# Getting started with Radio Control Console Python package

## Description

This package is a Python implementation of the Radio Control Console package v4.0.0.
The Radio Control Console (rcc) module allows the user to issue commands to, and retrieve information from, the Qorvo chip running test firmware from a PC console.
The rcc Python package is delivered  as a binary file with the name `rcc-<version>.tar.gz` eg: `rcc-0.2.0.tar.gz`

## Prerequisites
The project requires at least [Python 3.10](https://www.python.org/downloads/release/python-3100/).

## How to install

Installation through binary files is done with the following command:

```
python3 -m pip install <binay_file_name>
```

Example installation for version 0.2.0
```
python3 -m pip install rcc-0.2.0.tar.gz
```

## How to use

With RCC installed, it should be available as a Python module with an entrypoint, accessible through:
```
python3 -m rcc
```

It should be also available using the following command:
```
rcc
```

Please use the help command for information on how to use the rcc module:
```
python3 -m rcc --help
```
or
```
rcc --help
```

### Example usage

Description: Connect to a raspberry pi with IP `10.154.18.14` that exposes PTC on port `9192` and run the `info` command.

```
$ rcc -e 10.154.18.14 -p 9192
== Connecting 10.154.18.14 - Peak ==
host 10.154.18.14 port 9192
host 10.154.18.14 port 9192
802.15.4 MAC Address: 18:FC:26:00:00:56:31:99
BLE Address: 18:FC:26:56:31:99
APP version: 1.11.0.0 - Commit: 18de63e
PTC Version: 1.11.0.0
API Version: 1
Target: PTC_QPG7015M_RPi - QPG7015
Chip Version: 0DFF
Welcome to RCC! Type ? to list commands.
rcc> info
802.15.4 MAC Address: 18:FC:26:00:00:56:31:99
BLE Address: 18:FC:26:56:31:99
APP Version: 1.11.0.0 - Commit: 18de63e
API Version: 1
Target: PTC_QPG7015M_RPi - QPG7015
Chip Version: 0DFF
rcc>
```

## More resources

Please refer to `GP_P864_UM_16380_PTC_Overview.pdf` document for description of the most common PTC commands.
