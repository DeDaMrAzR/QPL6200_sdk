# Tools

This folder contains all the needed tools to make development and testing easy with Qorvo&reg; IoT Dev Kit for QPG6200. Tools that are required for testing:

* [Bootloader](Bootloader/README.md):
* [DataBlock](DataBlock/README.md): A tool to generate the Factory Block and the Persistent Storage Block for Zigbee applications.
* [FactoryData](FactoryData/README.md): A tool to generate a factory block that can be linked in at build time. This factory block
holds the certificates needed for device attestation and also information as discriminator, passcode, etc.
* [Hex](Hex/README.md): Tool to merge hex files together.
* [MatterControllers](MatterControllers/README.md)
* [MatterOtaProviders](MatterOtaProviders/README.md)
* [MemoryOverview](MemoryOverview/README.md): Tool that can be used to calculate the Flash and RAM usage after builds
* [Ota](Ota/README.md): OTA Firmware Upgrade Image/File Creation Tooling.
* [RadioControlConsole](RadioControlConsole/README.md): Tool needed to do RF testing of QPG6200 products that run the PTC firmware.
* [QorvoPlatformTools](QorvoPlatformTools/README.md): Individual tools for use during development, referred to as the "security toolbox", and a pair of server and client applications as reference for use in production lines. It is also used to sign your firmware for secure boot.
* [SecureBoot](SecureBoot/README.md): Example Developer key pair, used for signing.
* [Zap](Zap/README.md): Tool used to define the clusters for a custom Matter application and generation of the corresponding source
