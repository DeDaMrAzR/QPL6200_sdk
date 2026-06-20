Matter™ Device Attestation (DS)
===============================

Matter device attestation verifies that the commissioned device is
Matter protocol certified. Matter device attestation happens during
commissioning. It is also used to securely obtain certain information
about the device (e.g., vendor identifier, product identifier, etc.).
Matter device attestation is mandatory for all Matter devices. It makes
sure no counterfeit devices can join the Matter network.

This guide explains functionality and usage of the Matter device
attestation. It helps to integrate it in the development cycle. For
production, it offers options for provisioning device attestation data
in the factory - ensuring the data is secure and unique for each device.

Terminology
-----------

-  **Vendor Identifier (VID)**: A unique identifier of the vendor that
   is producing the device. This identifier is assigned by the
   Connectivity Standards Alliance (CSA).
-  **Product Identifier (PID)**: A unique identifier to identify the
   product. This identifier is assigned by the vendor.
-  **Public Key Infrastructure (PKI)**: A set of entities that are
   linked as a chain of trust in which the authenticity of one entity is
   validated by another entity by using the associated private key. At
   the top of the certificate chain is always a certificate authority
   (CA). The PKI in the Matter stack is defined by a set of certificates
   together with its corresponding public/private key pair.
-  **Certificate Authority (CA)**: Entity that stores, signs and issues
   certificates.
-  **Distributed Compliance Ledger (DCL)**: Database that is maintained
   by the CSA. It maintains a trusted root CA store and stores
   information about the certification status of different vendors and
   products.
-  **Product Attestation Authority (PAA)**: The root CA. PAAs are part
   of the DCL. PAA can be shared between different vendors or a vendor
   can set up his own PAA to serve its own products only.
-  **Product Attestation Intermediate (PAI)**: Intermediate certificate
   for a specific vendor that is issued and signed by a PAA. Vendor
   identifier is present in the PAI. For this certificate also a product
   identifier can be present but is optional. This means that a vendor
   can create one PAI for all its products, or create one PAI per
   product.
-  **Device Attestation Certificates (DAC)**: Unique certificate per
   device that gets issued and signed by the PAI. Both vendor identifier
   and product identifier are mandatory for this certificate. Vendor
   identifier needs to match the vendor identifier of the PAI.
-  **Certification Declaration (CD)**: Contains information that is
   needed for the device attestation process (VID, PID, certificate
   identifier, certification type). This is a set of information that is
   issued by the CSA, that proves the device's compliance with the
   Matter protocol. This is created by the CSA when the certification
   process finishes.
-  **Distinguished Encoding Rules (DER)**: Binary encoding for X.509
   certificates and private keys.
-  **Privacy Enhanced Mail (PEM)**: Common format for X.509 certificates
   and cryptographic keys.

Device Attestation PKI
----------------------

Every commissionable Matter node needs to have a unique DAC and
corresponding private key. The DAC is a DER-encoded X.509v3-compliant
certificate as defined in `RFC
5280 <https://www.rfc-editor.org/rfc/rfc5280>`__ and shall be issued by
a PAI that chains directly to an approved PAA.

The DAC contains a signature which is validated with the PAI public key.
At the root of the chain of trust, the PAA public key validates
signatures of the PAI. The hierarchy of the Matter device attestation
PKI can be found in below picture.

.. figure:: Images/pki_hierarchy.png
   :alt: Matter device attestation PKI
   :align: center

   Matter device attestation PKI

Device Attestation data
-----------------------

The DAC is signed by the PAI certificate. The PAI is based on a vendor
identifier which is typically held by the device manufacturer. The PAI
certificate is signed by the PAA certificate which acts as the root CA.
The list of trusted PAA certificates is stored in the DCL.

During manufacturing, the following elements need to be stored in the
persistent storage space of the device for completing the device
attestation process (see also picture below):

-  PAI certificate
-  DAC certificate
-  DAC private key
-  CD

All this information will be fetched and used during the commissioning
process by the Matter controller. The Matter controller will verify them
together with the vendor and product identifier. This process makes sure
the commissioned Matter device is a trusted device.

.. figure:: Images/persistent_storage_dac.png
   :alt: Matter protocol certificate storage
   :width: 500px
   :align: center

   Matter protocol certificate storage

Device Attestation procedure
----------------------------

During the device attestation procedure, the commissioner (Matter
controller) is responsible for attesting the commissionee (Matter
device). The picture below illustrates the device attestation steps that
are taken during commissioning.

**Step 1**: The commissioner generates a random 32-byte attestation
nonce.

**Step 2**: The commissioner sends the attestation nonce to the
commissionee and requests attestation information. using the command
*Attestation Request*.

**Step 3**: The commissionee generates the attestation information and
signs the information with the DAC private key. Data that is part of the
attestation information includes:

-  Certification Declaration
-  Attestation Nonce
-  Timestamp
-  Firmware information

**Step 4**: The commissionee sends the signed attestation information to
the commissioner using the command *Attestation Response*.

**Step 5**: The commissioner reads the certificate chain and requests
the DAC and PAI certificates using the command *Certificate Chain
Request*. The commissionee sends back the PAI and DAC certificates using
the command *Certificate Chain Response*.

**Step 6**: The commissioner validates the attestation information.
Validation steps:

-  Validate the DAC certificate chain. Ensure the PAA is validated for
   presence in the commissioner's trusted root store.
-  VID on the DAC matches the VID on the PAI.
-  VID and PID matches the attribute values found in the Basic
   Information Cluster.
-  Attestation signature is valid.
-  Attestation nonce matches the attestation nonce generated by the
   commissioner.
-  Validate Certificate Declaration signature. Certificate must be
   signed with one of the CSA's CD signing keys.
-  Firmware information matches the information present in the DCL.

.. figure:: Images/attestation_steps.png
   :alt: Overview of the Matter device attestation steps
   :width: 500px
   :align: center

   Overview of the Matter device attestation steps

Using device attestation during development
-------------------------------------------

Qorvo Matter SDK provides all the tools needed for Matter device
attestation. The SDK contains reference applications - all signed with
Qorvo development certificates and keys to get you started immediately.
See: `Default configuration <#default-configuration>`__. To generate
your own PAA, PAI, DACs and CDs, see `Generation of certificate
data <#generation-of-certificate-data>`__. To program the generated PAI,
DAC, DAC keys and CD on your device, and to go through the device
attestation procedure during commissioning, see `Generation of the
factory block <#generation-of-the-factory-block>`__.

Default configuration
~~~~~~~~~~~~~~~~~~~~~

The device attestation credentials provider is selected in the
initialization start-up procedure:

.. code:: cpp

       SetDeviceAttestationCredentialsProvider(&mFactoryDataProvider);

The default vendor identifier for Matter reference applications is set
to 0xFFF1. The build process generates a factory block, and links it
automatically after building. The factory block stores information
needed for device attestation. Factory data provider implementation
reads the credentials off the factory block.

Factory block generation for the Matter reference applications uses
these elements:

-  DAC: test\_<app>_dac_cert_0.der
-  PAI: test\_<app>_pai_cert_0.der
-  CD: test\_<app>_cd.bin

The credentials are located in ``Tools/QorvoPlatformTools/Matter/Credentials``. The
credentials that start with the prefix ‘qorvo' are generated with Qorvo
vendor ID (0x10D0).

For more details please refer to the `FactoryData
Documentation <https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/matter/factory-data.html>`__.

You can create credentials with your own vendor ID. See: `Generation of
certificate data <#generation-of-certificate-data>`__.

The ``_debug`` and ``_release`` builds create a .hex file to program
with JLink. The factory block binary is linked in the firmware image.
The (development) DAC private key is also embedded into the factory
block:

-  DAC private key: test\_<app>_dac_key_0.der

The development private key files are located in
``Tools/QorvoPlatformTools/Matter/Credentials``.

   WARNING: DO NOT STORE the DAC private key in a factory block when in
   production environment. This is UNSAFE. For proper private key
   handling, refer to `Security
   Toolbox <https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/matter/factory-data.html>`__

Generation of certificate data
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To generate your own certificates and use them with the device
attestation feature, use the CHIP Certificate Tool (chip-cert) provided
in the `Matter <https://github.com/project-chip/connectedhomeip>`__
repository. `CHIP Certificate Tool
Documentation <https://project-chip.github.io/connectedhomeip-doc/src/tools/chip-cert/README.html>`__
guides you how to do that.

Generation of the factory block
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See: `FactoryData
Documentation <https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/matter/factory-data.html>`__. This document
contains information on usage of the factory block.

Generating and provisioning device certificates in a production environment
---------------------------------------------------------------------------

Requirements for releasing your Matter product:

-  CSA certifies and approves **authenticated firmware**,
-  CSA issues the **Certification Declaration (CD)**,
-  both are **programmed** onto the device.

Each device needs to be **provisioned** with:

-  a **Product Attestation Intermediate (PAI) certificate**
-  a unique **Device Attestation Certificate (DAC)**.

Finally, the **DAC private key** is **provisioned securely** in the
Qorvo Silicon.

These steps are illustrated in the picture below. It shows the Device
Attestation PKI and signing chain. The root CA is the PAA. Two options
are supported by Qorvo for PAA selection.

.. figure:: Images/matter_certificates_dac_flow_qorvo.png
   :alt: Overview of the Matter device attestation steps
   :width: 800px
   :align: center

   Overview of the Matter device attestation steps

**Option A:** Use your own PKI. If you set up your own PKI and comply
with the `Matter PKI Certificate
Policy <https://github.com/CHIP-Specifications/connectedhomeip-spec/blob/master/policies/matter_certificate_policy.adoc>`__,
you can obtain your own PAA entry in the DCL (Vendor ID scoped or
Non-Vendor ID scoped). This can be used to generate your PAI and unique
DACs.

**Option B:** Select a PKI provider. Contract a PKI provider who is a
member of the CSA to generate PAI and DAC for your product. This way you
don't need to set up your own Certification Authority chain. PKI
providers have a non-Vendor ID scoped PAA making it possible for them to
create certificates for multiple vendors. Their PAA will be used to
create a manufacturer specific PAI that signs unique DACs for each
device.

The certificates need to be used as input for generating your production
factory block. More information on the steps to take to get all the
needed data programmed on your production device can be found in
`Creating production build for the
factory <../../User_Journey/Production/creating_production_build.md#application-configurations-for-production-builds>`__
and `Qorvo Production Tool
Documentation <https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/production_tool/overview.html>`__.
