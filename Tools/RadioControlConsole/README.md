# Getting Started with Radio Control Console

The Radio Control Console (RCC) application allows users to issue commands to and retrieve information from the Qorvo chip running PTC firmware via an RCC console.

The specifications for the supported test modes are beyond the scope of this manual. They are covered in the documentation for the test component software and documentation package.

## Prerequisites

The project requires at least [Python 3.10](https://www.python.org/downloads/release/python-3100/).

**Note:** The RCC application is not supported in the IoT_SDK Python virtual environment created by following the [Installation](../../sphinx/docs/QUICKSTART.rst#install-the-sdk) steps, as it uses Python 3.9. To use RCC, please install the RCC package in a separate Python environment.

## Installation

The RCC application is distributed as a Python package. To install the RCC package, run the following command:

```console
python3 -m pip install ./rcc-5.5.0
```

The SDK also includes a standalone executable file `PyRCC_5.5.0.exe` for Windows as a convenience. This executable can be run without installing the RCC Python package.

## Usage

With RCC installed, it should be available as a Python module with an entry point, accessible through:

```console
python3 -m rcc
```

It should also be available using the following command:

```console
rcc
```

Use the help command for information on how to use the RCC module:

```console
python3 -m rcc --help
```

or

```console
rcc --help
```

### Example Usage

Description: Connect to a QPG6200 DK that is running PTC firmware via a serial port `/dev/ttyACM0` at 115200 baud rate.

```console
$ rcc -c /dev/ttyACM0 -b 115200
802.15.4 MAC Address: 74:05:7C:00:00:24:66:E6
BLE Address: 74:05:7C:24:66:E6
APP version: 1.0.0.0 - Commit: b460d2c
PTC Version: 1.0.0.0
API Version: 3
Target: PTC_QPG6200_10DBM_UART8to9 - QPG6200L
Chip Version: 1201
Welcome to RCC! Type ? to list commands.
rcc> info
802.15.4 MAC Address: 74:05:7C:00:00:24:66:E6
BLE Address: 74:05:7C:24:66:E6
APP version: 1.0.0.0 - Commit: b460d2c
API Version: 3
Target: PTC_QPG6200_10DBM_UART8to9 - QPG6200L
Chip Version: 1201
rcc>
```

## Additional Resources

The following user manuals provide instructions on how to use the Radio Control Console application:

- [Development Quick Start Guide](rcc-5.5.0/README)
- [Reference Documentation](rcc-5.5.0/docs/index)
