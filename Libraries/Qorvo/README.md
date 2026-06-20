# Libraries

This folder contains makefiles for `Qorvo libraries`.

Qorvo libraries are:
* FactoryBlock: Factory Block stores the ZigBee network configuration for the end node.
* PersistentStorageBlock: Persistent Storage Block contains the ZigBee runtime non-volatile information and its default values.
* FactoryData: This is a factory block that gets generated and gets merged to the full Matter application.
* Matter-Qorvo glue library: Matter-Qorvo glue code library.
* OpenThread-Qorvo glue library: OpenThread-Qorvo glue code library.
* mbedtls alternative library: Library that implements the mbedtls API on top of the Qorvo crypto hardware block.

Please read the [build flow with make](../../README.md#working-with-examples) guide for more details.

- [Programming Prebuilt Binaries](../../README.md#programming-applications)
