Matter™ protocol Over-The-Air (OTA) Device firmware upgrade
===========================================================

In this guide we will explain how you can trigger a download and
software upgrade of a Matter node in the Matter network. The Matter OTA
protocol will be used for this.

.. image:: Images/chip_ota_provider_app_setup.png
   :alt: chip-ota-provider-app setup
   :width: 500px

**Prerequisites:**

-  Qorvo's QPG7015M Gateway development kit is used as OpenThread Border
   Router.
   See :doc:`How to setup the OpenThread Border Router <./setup_qpg7015m_ot_borderrouter>`.
-  Host for the *chip-tool* such as Linux 64-bit PC running Ubuntu
   20.04+ or RPi4 running Ubuntu 20.04+.
-  Host for the *ota-provider-app* such as Linux 64-bit PC running
   Ubuntu 20.04+ or RPi4 running Ubuntu 20.04+.

*In the example below a RaspberryPi4 will be used as host device for
these tools and referred to as RPi.* *If an other platform is selected
for one or all of the tools/apps please use the respective folder
versions of the tools.*

To do a device upgrade over the Matter protocol, two nodes are needed in
the Matter network : Matter OTA provider and Matter OTA requestor node.
An OTA provider node is the node that can supply the available software
update to the OTA requestor node. In this example the QPG6200 Matter
combo switch device will be the Matter OTA requestor. This node will
query the Matter OTA provider node for a software update. The Matter OTA
provider for this test is your Linux 64-bit PC that runs the Matter OTA
provider tool from the Matter repository.

Step 1: OpenThread Border router is running and a Thread network is formed
--------------------------------------------------------------------------

See the guide :doc:`How to setup the OpenThread Border Router <./setup_qpg7015m_ot_borderrouter>`.

Step 2: Rebuild the application to obtain an update .ota file
-------------------------------------------------------------

A prerequisite for starting the Matter OTA protocol download process is
that the version of the application firmware needs to be updated. If the
software version matches the one that is running, the update image will
be ignored.. To change the version of the software, and as a consequence
also the version of the ota image, the following actions need to be
taken:

1. Update the application's software version in the CHIPProjectConfig.h
   of your application by adding ``#define OTA_TEST_IMAGE`` to
   qorvo_internals.h as shown below (for the ``switch`` application this
   is
   `Applications/Combo/Switch/gen/Combo_qpg6200/qorvo_internals.h`:

.. code:: diff

    #define QVCHIP_DIVERSITY_KVS_HASH_KEYS
    #define WSF_ASSERT_ENABLED                                        TRUE

   +#define OTA_TEST_IMAGE

This definition will update the application's software version in the
CHIPProjectConfig.h from ``1.3-0.2.1`` to ``1.3-0.2.2`` as shown below:

.. code:: c

   #ifndef CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION
   #ifndef OTA_TEST_IMAGE
   #define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION 0x01030201
   #else
   #define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION 0x01030202
   #endif
   #endif

   #ifndef CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING
   #ifndef OTA_TEST_IMAGE
   #define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING "1.3-0.2.1"
   #else
   #define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING "1.3-0.2.2"
   #endif
   #endif

1. The version is also part of the OTA header when the .ota file gets
   generated. The
   ``Tools/Ota/generate_ota_img.py``
   script that generates this file will parse this C header and consider
   this a new version. Add ``--test-update-image`` to the parameter of
   generate_ota_img.py in application_postbuild.sh as below (for the
   ``switch`` application this is
   ``Applications/Combo/Switch/Switch_Combo_qpg6200_thread_certified_postbuild.sh``)

.. code:: diff

   -"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/Ota/generate_ota_img.py --chip_config_header "${SCRIPT_DIR}"/../../../Applications/Matter/switch/include/CHIPProjectConfig.h --chip_root "${SCRIPT_DIR}"/../../../Components/ThirdParty/Matter/repo --compression lzma --factory_data_config "${SCRIPT_DIR}"/../../../Tools/QorvoPlatformTools/Matter/Credentials/test_switch_qpg6200.factory_data_config --flash_app_start_offset 0x1fc00 --flash_start 0x10013000 --in_file "${SCRIPT_DIR}"/../../../Work/Switch_Combo_qpg6200/Switch_Combo_qpg6200.hex --ota_offset 0xe0e00 --out_file "${SCRIPT_DIR}"/../../../Work/Switch_Combo_qpg6200/Switch_Combo_qpg6200.ota
   +"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/Ota/generate_ota_img.py --chip_config_header "${SCRIPT_DIR}"/../../../Applications/Matter/switch/include/CHIPProjectConfig.h --chip_root "${SCRIPT_DIR}"/../../../Components/ThirdParty/Matter/repo --compression lzma --factory_data_config "${SCRIPT_DIR}"/../../../Tools/QorvoPlatformTools/Matter/Credentials/test_switch_qpg6200.factory_data_config --flash_app_start_offset 0x1fc00 --flash_start 0x10013000 --in_file "${SCRIPT_DIR}"/../../../Work/Switch_Combo_qpg6200/Switch_Combo_qpg6200.hex --ota_offset 0xe0e00 --out_file "${SCRIPT_DIR}"/../../../Work/Switch_Combo_qpg6200/Switch_Combo_qpg6200.ota --test-update-image

Once these lines are modified restart the application build to get the
updated ota file.

::

   source Scripts/activate.sh
   cd Applications/Combo/Switch
   make -f Makefile.Switch_Combo_qpg6200

After successful compilation you can tell the OTA provider tool where it
can find the .ota file.

Step 3: Provide the OTA provider node with the needed .ota file
---------------------------------------------------------------

.. code:: console

   sudo ./chip-ota-provider-app.elf -f <path to SDK>/Work/Switch_Combo_qpg6200/Switch_Combo_qpg6200.ota

**Remarks: In order to apply the application version control, please refer
to** `Managing the Software Version Maximum/Minimum Application
Version <#managing-the-software-version-maximum-minimum-application-version>`__

Now make sure to keep this application keeps running. Open a new
terminal window to complete the next steps.

Step 4: Commission the OTA provider node in the Matter network
--------------------------------------------------------------

For this the Matter controller tool is needed. It can run on the same
machine as the chip-ota-provider-app. The OTA provider will be
commissioned as node id 1 in the network:

.. code:: console

   sudo ./chip-tool.elf pairing onnetwork 1 20202021

Step 5: Commission the QPG6200 Matter switch (OTA requestor) node in the Matter network
---------------------------------------------------------------------------------------

We will also use the matter controller tool to commission thi device.
First, get the active dataset from the running OpenThread border router.
Details how to retrieve this can be found in the guide
`How to Setup OpenThread Border Router <setup_qpg7015m_ot_borderrouter.md#step-6-get-active-dataset-of-the-running-openthread-border-router>`__:
With this information we can commission the Matter switch node with node
identifier 2 in the network:

.. code:: console

   sudo ./chip-tool.elf pairing ble-thread 2 hex:<otbr active dataset> 20202021 3840 --paa-trust-store-path /home/ubuntu/cert/paa

Step 6: Configure the default OTA provider on the OTA requestor (Matter switch)
-------------------------------------------------------------------------------

We will instruct the Matter controller to write configuration to the
default OTA provider in the Matter switch. We need to provide it the
fabricIndex on which the OTA provider lives, the node identifier we
assigned to it and the endpoint of the OTA software update provider
cluster. The last arguments in the command contain the node identifier
of the OTA requestor (e.g. the switch) and the endpoint where the OTA
software update requestor cluster lives.

.. code:: console

   sudo ./chip-tool.elf otasoftwareupdaterequestor write default-otaproviders '[{"fabricIndex": 1, "providerNodeID": 1, "endpoint": 0}]' 2 0

Step 7: Configuration of Access Control List (ACL) on the OTA provider node
---------------------------------------------------------------------------

We need to add access control configuration to the OTA provider node so
it accepts commands from nodes that are part of the fabric. This command
will grant Operation privileges to all the nodes that are part of the
fabric:

.. code:: console

   sudo ./chip-tool.elf accesscontrol write acl '[{"fabricIndex": 1, "privilege": 5, "authMode": 2, "subjects": [112233], "targets": null}, {"fabricIndex": 1, "privilege": 3, "authMode": 2, "subjects": null, "targets": null}]' 1 0

Step 8: Trigger download and device upgrade
-------------------------------------------

The final step in the flow is to announce that a software update is
available for the Matter node. This will trigger the download of the
full .ota image and once the image is taken in, a reboot of the device
will take place to allow the bootloader to upgrade to the new firmware.

.. code:: console

   sudo ./chip-tool.elf otasoftwareupdaterequestor announce-otaprovider 1 0 0 0 2 0

Arguments in this command are:

-  OTA Provider node identifier [1]
-  OTA Provider vendor identifier [0]
-  Announcement Reason [0 - SimpleAnnouncement]
-  OTA Requestor node endpoint [0]
-  OTA Requestor node identifier [2]
-  Endpoint to be ignored for group commands [0]

If all goes well, you should now have your application rebooted with the
updated version ``1.3-0.2.2`` on the ota requestor node:

.. code:: console

   [P][-] ============================
   [P][-] Current Software Version: 1.3-0.2.2

Matter re-certification of software updates
-------------------------------------------

Although the CSA has a Matter rapid recert program available, full
recertification may be advised in most cases (see CSA document
22-27427-01 “Matter Rapid Recert Program Version 1.0” on the CSA
webportal https://groups.csa-iot.org/). Rapid recert may be applicable
for a software update containing only eg. a bootloader or Secure Element
update.

Managing the Software Version, Maximum/Minimum Application Version
------------------------------------------------------------------

Starting the ota-provider-app with the –otaImageList command line option
which allows the user to specify the Software version, Maximum and
Minimum Application Version and other attributes to the corresponding
images served by the Provider via a JSON file.

Example provider configuration file:

.. code:: json

   {
     "deviceSoftwareVersionModel":
     [
         { "vendorId": 65521, "productId": 32773, "softwareVersion": 16977920, "softwareVersionString": "1.3-1.0.0", "cDVersionNumber": 18, "softwareVersionValid": true, "minApplicableSoftwareVersion": 16975369, "maxApplicableSoftwareVersion": 16977919, "otaURL": "app-example.ota" }
     ]
   }

Example of the command:

.. code:: console

   sudo ./chip-ota-provider-app --otaImageList  <path to JSON file>/ota_images.json
