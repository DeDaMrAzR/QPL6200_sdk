Secure Boot
============

Description
-----------
Secure Boot is a four-stage process:

1. On power up, the Secure Element executes the ROM Bootloader (ROMBL).
2. The ROM Bootloader then verifies the Secure Element Firmware and transfers control to the Secure Element Firmware.
3. The Secure Element Firmware verifies the Application Bootloader and releases the Application Processor (APPUC) from reset, which starts the Application Bootloader.
4. The Application Bootloader verifies the Application and starts application if verification is done successfully. On failure, the start-up process ends in application Bootloader, i.e. cannot launch the application.

.. figure:: Images/SecureBootOverview.png
   :alt: Secure Boot overview
   :width: 400px
   :align: center

   Secure Boot overview


Once the Secure Boot is enabled, the cryptographic signature verification will be in place to authenticate the genuineness of the firmware. The authentication process requires the certificate, e.g. developer certificate, and firmware signature. The certificate contains the public key corresponding to the private key that signed the firmware. Both certificate and signature are added as a block of authentication metadata to secure element firmware and application bootloader.

Therefore, the application firmware are required to be signed. More information will be provided in below section.


Signing firmware
----------------
To execute applications in Secure Boot mode, sign them with the correct credentials using `Qorvo Platform Tools <../QorvoPlatformTools/README.md#qorvo-platform-tools>`__.

The tool supports signing binary files with direct or indirect access to the signing private key.
Please refer to below links for more information of signing:

- `Direct Signing <../../Documents/User_Journey/Security/Security_User_Journey.md#direct-signing>`__

- `Indirect Signing <../../Documents/User_Journey/Security/Security_User_Journey.md#indirect-signing>`__


Files Description
-----------------
This SecureBoot folder contains files which will be used in different signing processes:

1. crypto_utils.py                   - a python file used to sign and verify the payload, it will write signatures to the HEX files.
2. developer_certificate_signed.cert - a signed developer certificate to be used with developer private key for signing
3. developer_key_private.der         - a developer private key to be used with the signed developer certificate for signing
4. private_key.pem                   - an encoded private key in string format
5. public_key.pem                    - an encoded public key in string format
