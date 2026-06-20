#############
API Reference
#############


Below you'll find a list of PDFs describing API references. 


********************
Bluetooth Low Energy
********************


Attribute Protocol (ATT) Module Documentation
============================================

`ATT API <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/att-api.pdf?ref_type=heads>`_

Attribute Protocol (ATT) block for Bluetooth Low Energy (BLE). It explains the ATT protocol, which enables communication between attribute protocol clients (ATTC) and servers (ATTS). The document describes how ATTC and ATTS support multiple simultaneous connections and route messages using the ATT layer. Enhanced ATT (EATT) bearers are also supported.

**Key points:**

- ATT API, ATT Server API, and ATT Client API usage
- The structure and management of attribute databases on the server, including services, characteristics, and attributes
- How to implement and interact with attribute protocol servers and clients, including data structure organization and API function calls

**Refer to this PDF when you need:**

- Guidance on implementing or using BLE ATT client/server features.
- Reference for ATT/EATT API functions and data structures
- Understanding of attribute database organization and service/characteristic relationships in BLE development


GAP Device Manager (DM) Module Documentation
============================================

`DM API <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/dm-api.pdf?ref_type=heads>`_

GAP Device Manager (DM) subsystem. The DM manages essential Bluetooth device operations, organized by procedure and device role (controller/controlled).

**Key points:**

- Advertising and device visibility (enable/disable, parameters, connectability, discoverability)
- Scanning and device discovery (start/stop, parameters, advertising reports, name discovery)
- Connection management (create/accept/remove connections, update parameters, read RSSI)
- Local device management (initialization, reset, local parameters, vendor-specific commands, GAP attribute management)
- Security management (bonding, security storage, authentication, encryption, authorization, random address)
- Privacy management (resolving list, random address, privacy mode)
- PHY control
- The DM implements these procedures to support the GAP profile and controls all GAP-related behavior, including advertising, scanning, connections, and privacy.

**Refer to this PDF when you need:**

- Details on implementing or managing Bluetooth GAP device operations
- Reference for Device Manager API functions and supported procedures
- Guidance on security, privacy, and connection management for BLE devices


Host Controller Interface (HCI) API Manual
==========================================

`HCI API <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/hci-api.pdf?ref_type=heads>`_

Host Controller Interface (HCI) block for Bluetooth Low Energy (BLE) stacks. The HCI layer defines the commands, events, and data packets exchanged between the BLE Core-Host Layer and the BLE Core-Controller Layer. HCI provides a thin, configurable interface for both single-chip and traditional wired (UART, USB) HCI systems. It includes a routing layer for system configuration and supports multiple transport options like UART and USB.

**Key points:**

- **Initialization & Reset:** Provides functions for HCI initialization, registration, and reset. The reset sequence is configurable and triggers a series of HCI commands, with a completion event sent via callback.
- **Command & Optimization Interfaces:** Offers direct mapping to Bluetooth HCI commands and an optimized interface for fast access to common controller data (e.g., device address, buffer size). Not all command functions need to be implemented on every platform.
- **Event & Data Handling:** Defines event structures and callback mechanisms for handling HCI events and ACL (Asynchronous Connection-Less) data. The ACL interface manages data transmission, reception, and flow control.
- **Usage:** The API enables BLE stack components to send commands, receive events, and exchange ACL data with the controller. It supports both message-passing and direct function call models, depending on system configuration.

**Refer to this PDF when you need:**

- To implement or debug BLE HCI communication in Qorvo-based systems
- Details on HCI initialization, command, event, and data interfaces
- Reference for supported HCI commands, optimization functions, and event handling patterns.


L2C API Manual
==============

`L2C API <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/l2c-api.pdf?ref_type=heads>`_

L2C block, which implements the Bluetooth Low Energy (LE) L2CAP protocol. L2C is a simplified version of classic Bluetooth L2CAP. It handles building and parsing L2CAP packets, flow control, and connection parameter updates.

**Key points:**

- L2C interfaces with HCI to send/receive ACL packets, and with ATT/SMP for protocol data exchange.
- In the transmit path, L2C builds L2CAP packets and manages flow control.
- In the receive path, L2C routes packets from HCI to the correct protocol layer (SMP or ATT).
- The block supports connection parameter updates and may implement buffer admission control for multiple links.
- The architecture is modular, with dedicated modules for API, main event handling, master/slave roles, and Connection Oriented Channels.

**Refer to this PDF when you need:**

- To implement or debug L2CAP communication in BLE systems
- Details on packet flow, protocol interfacing, and connection updates in the Qorvo BLE stack
- Reference for L2C API functions and block architecture.


SMP API Manual
==============

`SMP API <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/smp-api.pdf?ref_type=heads>`_

Security Manager Protocol (SMP) block for Bluetooth Low Energy (BLE). SMP handles device pairing, authentication, and encryption. 

SMP includes a cryptographic toolbox using AES (with asynchronous, abstracted interface) and supports data signing. The protocol routes received messages to the appropriate initiator or responder layer. It supports both Secure Connections and Legacy pairing modes, for both central (initiator) and peripheral (responder) roles.

**Refer to this PDF when you need:**

- To implement or debug BLE security (pairing, authentication, encryption)
- Details on initiator/responder roles and supported security modes


**********************
Zigbee / IEEE 802.15.4
**********************


gpMacDispatcher Reference Manual
================================

`gpMacDispatcher Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpMacDispatcher_API_Manual.pdf?ref_type=heads>`_

API for the gpMacDispatcher component, which manages the IEEE 802.15.4 MAC layer for Qorvo devices. It covers initialization, data transfer, management, and attribute access functions.

**Key points:**

- Initialization routines for the MAC core and network stack registration
- Data primitives for sending, polling, and purging data packets, with callback support for data confirmation and indication
- Management primitives for scanning, association, orphan response, and network joining
- Attribute access for all MAC PIB (PAN Information Base) attributes, such as channel, PAN ID, addresses, transmit power, security settings, and more
- Data structures and callback types for handling MAC events and status notifications

**Refer to this PDF when you need:**

- To build directly on the raw IEEE 802.15.4 MAC stack.
- Via ConcurrentConnect multi-channel capability, you run your stack concurrently with Zigbee and Thread.
- Configure low level IEEE 802.15.4 MAC (CCA, retries, addresses, etc.)
- Debug low level communication through callback structures and result codes


gpZgpStub Reference Manual
==========================

`gpZgpStub Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpZgpStub_API_Manual.pdf?ref_type=heads>`_

API for the gpZgpStub component. Provides control and management of Zigbee Green Power (ZGP) stub functionality. It details the data structures used for ZGP operations, including callback registration, device identification, and security parameters.

**Key points:**

- Defines callback structures for handling data confirmation, data indication, security failures, and purge confirmations.
- Includes structures for device identification using IEEE 64-bit MAC addresses and 32-bit source IDs.
- Documents the organization of ID information, endpoint usage, and security parameters.

**Refer to this PDF when you need:**

- To implement or integrate Zigbee Green Power stub functionality in Qorvo-based applications
- Details on ZGP data structures, callbacks, and security handling
- Reference for developing or troubleshooting ZGP-related features.


**************************
Radio, Hardware and System
**************************


gpRadio Reference Manual
========================

`gpRadio Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpRadio_API_Manual.pdf?ref_type=heads>`_

API for the gpRadio component. Controls radio functionality on Qorvo devices. It covers initialization, configuration, and status retrieval for radio operations.

**Key points:**

- Provides functions to initialize the radio module.
- Supports configuration of receive (Rx) modes, including options for multi-standard, multi-channel, and high sensitivity operation.
- Allows selection and querying of the Rx antenna, with options for fixed or automatic antenna selection.
- Includes configuration of FIR filters for radio transmission.
- Defines status and error codes for API calls.

**Refer to this PDF when you need:**

- To initialize or configure the radio hardware on Qorvo platforms
- Details on antenna selection, Rx mode options, or FIR filter settings
- Reference for API usage, return values, and error handling in radio control.


gpPeripheral Reference Manual
=============================

`gpPeripheral Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpPeripheral_API_Manual.pdf?ref_type=heads>`_

gpPeripheral module. Provides low-level drivers for a wide range of hardware peripherals on Qorvo devices. It covers APIs for various drivers, including ADCs, DMA, GPIO, I2S, infrared, keypad, LED, comparators, timers, UART, watchdog, and more.

**Key points:**

- Each driver section details configuration structures, API functions, and usage.
- The manual includes both general-purpose and specialized peripheral drivers (e.g., high-resolution ADC, dual circular buffer DMA, pulse modulation, temperature sensor).
- Data structure references and file documentation are provided for integration and development.

**Refer to this PDF when you need:**

- To implement, configure, or troubleshoot low-level hardware drivers on Qorvo platforms
- Details on specific peripheral APIs, configuration options, and usage patterns
- Reference for integrating or extending hardware support in embedded applications


*************************
Cryptography and Security
*************************


eSecure Library
===============

`eSecure Library <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/1.0.0/Documents/libesecure.pdf?ref_type=tags>`_

eSecure library, which provides a C API for cryptographic operations using a Secure Element. The library supports hashing (SHA-1, SHA-224, SHA-256, SHA-384, SHA-512), message authentication (HMAC, GMAC, CMAC), symmetric encryption and decryption (AES in multiple modes), authenticated encryption (AES-GCM, AES-CCM), elliptic curve digital signatures, key exchange (ECDH, EC J-PAKE), random/prime number generation, and key derivation (PBKDF2, HKDF, HKDF2).

**Key points:**

- All API functions use the `esec_` prefix and return status codes as uint32_t values.
- Both synchronous and asynchronous APIs are available. Applications must choose one mode and use the appropriate header file (`esecure.h` for synchronous, `esecure_async.h` for asynchronous).
- Asynchronous operations are non-blocking, use `_go`-suffixed functions, and require polling for completion.
- The library manages key material, supporting both local and Secure Element–protected keys.
- Predefined macros and data structures are provided for various cryptographic curves and key types.

**Refer to this PDF when you need:**

- To implement secure cryptographic operations or manage keys with the Secure Element
- Details on supported algorithms, API usage, and error/status handling
- Reference for integrating synchronous or asynchronous cryptography into your application.


gpEncryption Reference Manual
=============================

`gpEncryption Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpEncryption_API_Manual.pdf?ref_type=heads>`_

API for the gpEncryption component, which provides AES and CCM encryption and decryption for Qorvo devices.

**Key points:**

- AES encryption (fixed 16-byte blocks, key sizes: 128, 192, or 256 bits depending on chip version)
- CCM encryption/decryption (key size: 128 bits, all IEEE 802.15.4 CCM* security levels)
- The API returns results in the same data buffer. Functions include synchronous AES and CCM encryption/decryption, and component initialization. The manual details required parameters, option flags (such as key selection and hardening), result codes, and the data structures for encryption options.

**Refer to this PDF when you need:**

- To implement or use AES/CCM encryption or decryption on Qorvo chips
- Details on API calls, option flags, result codes, and supported key lengths
- Reference for security level settings and buffer/data structure requirements


**********************************
Memory, Scheduling, OS Abstraction
**********************************


gpNvm Reference Manual
======================

`gpNvm Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpNvm_ElemIf_API_Manual.pdf?ref_type=heads>`_

The gpNvm component provides the primary API for managing Non-Volatile Memory (NVM) storage in Qorvo devices. This includes storing, retrieving, and managing persistent data with features such as automatic wear leveling, defragmentation, and redundancy configuration to ensure data reliability and endurance. All NVM storage and retrieval operations are accessed through the element-based interface (gpNvm_ElemIf), which is the only supported method for interacting with NVM in current Qorvo SDKs.

**Key points:**

- The element interface (gpNvm_ElemIf) is the required API for all NVM operations. It allows explicit registration and identification of NVM attributes using unique IDs, providing flexibility and compatibility when adding, removing, or resizing attributes across firmware versions.
- The underlying gpNvm storage system automatically manages wear leveling and defragmentation, helping to maximize NVM lifetime and maintain data integrity.
- Redundancy and consistency checking are supported, allowing applications to configure and verify the reliability of stored data.
- The API provides functions for configuring, registering, and managing NVM elements, as well as for checking attribute consistency and handling compatibility across firmware upgrades or downgrades.
- The element interface enables safe migration and reuse of NVM content, with mechanisms to ensure that inherited data remains valid and up-to-date even as firmware evolves.

**Refer to this PDF when you need:**

- To store, retrieve, or manage persistent data in Qorvo devices using NVM
- To configure or understand wear leveling, defragmentation, or redundancy options in NVM storage
- Details on registering and accessing NVM elements with unique IDs, and ensuring compatibility across firmware versions
- Guidance on maintaining NVM data integrity and reliability in your application


gpPoolMem Reference Manual
==========================

`gpPoolMem Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpPoolMem_API_Manual.pdf?ref_type=heads>`_

gpPoolMem component. Manages fixed-size memory pools in Qorvo devices. It supports up to three different chunk sizes for memory allocation. Each memory chunk includes guard sections for boundary checking, a data section for user data, and an "InUse" flag to indicate allocation status.

The API is focused primarily on efficient memory allocation and deallocation.

There are two implementation flavors:
- **Static:** Pools are pre-allocated by the user at compile-time, offering deterministic memory usage and no runtime heap allocation.
- **Dynamic:** Pools can be allocated at runtime, providing more flexibility if supported by the platform.

**Key API functions:**
- `gpPoolMem_Init()`: Initializes all memory chunks, resetting guards and allocation flags.
- `gpPoolMem_Reset()`: Similar to Init, used for warm restarts.
- `gpPoolMem_Malloc()`: Allocates a chunk of memory of requested size.
- `gpPoolMem_Free()`: Frees an allocated chunk, with checks for pointer validity and guard integrity.
- `gpPoolMem_Dump()`: Dumps pool content and checks consistency.

**Refer to this PDF when you need:**
- To implement or debug memory pool management in Qorvo SDKs
- Details on static or dynamic pool setup, chunk structure, safety checks, and allocation/freeing routines
- Reference for API usage, error/assertion behaviors, and best practices for embedded memory management


gpSched Reference Manual
========================

`gpSched Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpSched_API_Manual.pdf?ref_type=heads>`_

gpSched component. Lightweight cooperative scheduler used in the Qorvo stack. It acts as a minimal OS, running function callbacks at scheduled times. The main loop executes polled actions (like radio interrupt handling), checks for scheduled events, runs callbacks when their time arrives, and manages system sleep based on event timing.

**Key points:**

- Supports callbacks with or without arguments.
- Designed for top-half/bottom-half interrupt handling to keep interrupt routines short and responsive.
- Suitable for size-constrained devices; can be compiled to exclude argument support.
- Provides APIs for scheduling, unscheduling, and querying events.

**Refer to this PDF when you need:**

- To implement or debug event scheduling in Qorvo-based embedded systems
- Guidance on cooperative scheduling, event timing, and interrupt handling patterns
- Reference for gpSched API usage and design principles.


*****************************
Qorvo Protocols and Utilities
*****************************

gpCom Reference Manual
======================

`gpCom Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpCom_API_Manual.pdf?ref_type=heads>`_

gpCom component, a communication protocol. It details the message format used for communication: each message consists of a six-byte header, a variable-length payload, and a two-byte CRC footer. The maximum packet size is 4103 bytes. The header includes fields for synchronization (SYN), payload length, frame control, and module ID. The payload is sent least-significant byte first, and the CRC ensures data integrity.

The document explains how to synchronize with the message stream, parse message fields, and validate messages using a 16-bit CRC with polynomial 0xA001. It also covers handling unknown module IDs and maintaining synchronization in the data stream.

**Refer to this PDF when you need:**

- Details on the gpCom message structure and field definitions
- Debug the communication between the production test component (PTC) embedded application and the Radio Control Console (RCC) PC application


gpUpgrade Reference Manual
==========================

`gpUpgrade Reference Manual API Description <https://gitlab.com/qorvo_sdk/public/devkits/qpg6200-iot-sdk/-/blob/main/Documents/API%20Manuals/gpUpgrade_API_Manual.pdf?ref_type=heads>`_

gpUpgrade component. Enables firmware updates for running applications on Qorvo devices. It provides APIs for both the application and the User Mode Boot Loader to manage, validate, and activate upgrade images.

**Key points:**

- The application uses gpUpgrade to store downloaded images, mark them as pending, and trigger upgrades.
- The boot loader uses gpUpgrade to validate, activate, and manage images, supporting both Shared User Mode Boot Table and License Based Boot approaches.
- The manual details architecture, implementation, application and boot loader interfaces, image format requirements (including CRC, signing, and compression), and upgrade process flow.
- Data structures and API functions for upgrade management, image integrity checks, and error handling are provided.

**Refer to this PDF when you need:**

- To implement or troubleshoot firmware upgrade and bootloader processes on Qorvo platforms
- Details on upgrade image handling, integrity checks, secure boot, and flash organization options
- Reference for API usage, supported boot modes, and image format requirements.
