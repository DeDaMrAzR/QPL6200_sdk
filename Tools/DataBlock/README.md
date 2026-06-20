# Data Block Tools

## Tools used for ZigBee Persistent Storage Block (PSB) generation

Persistent Storage Block contains the runtime non-volatile information and default values.

> **Note:** The following tools are used internally by the PersistentStorageBlock makefiles in `Libraries/Qorvo/PersistentStorageBlock`, no need to call them directly.

| Tool                | Description                                                  |
|---------------------|--------------------------------------------------------------|
| `psbuild`           | Used to generate a hex file from a provided PSB xml file.    |
| `psconvert-qorvo`   | Used to convert `.bin` to `.txt` file format.                |
| `double_entries.py` | Used to duplicate some entries in PSB `.txt` files.          |
| `gpNvmImageTool.py` | Used to generate PSB NVM binary.                             |

### Example usage

Refer to PersistentStorageBlock makefiles in `Libraries/Qorvo/PersistentStorageBlock`.

## Tools used for ZigBee Factory Block (FB) generation

Factory Block stores the ZigBee network configuration for the end node.

> **Note:** The following tools are used internally by the FactoryBlock makefiles in `Libraries/Qorvo/FactoryBlock`, no need to call them directly.

| Script                | Description                                              |
|-----------------------|----------------------------------------------------------|
| `fbbuild`             | Used to generate a hex file from a provided FB xml file. |
| `install_code.py`     | Used to add the install code to the FB hex file.         |

### Example usage

Refer to FactoryBlock makefiles in `Libraries/Qorvo/FactoryBlock`.
