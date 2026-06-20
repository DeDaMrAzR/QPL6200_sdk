Commissioning a Qorvo Matter™ end device in the Google ecosystem
================================================================

In this guide, step by step instructions are given to commission a
Matter device onto the Matter network and control it making use of the
Google Nest Hub 2nd generation.

.. contents:: Features demonstrated in this guide are:
   :depth: 2
   :local:

Step 1: Validating hardware and software dependencies
-----------------------------------------------------

This section elaborates on hardware and software that can be used to
validate commissioning and controlling of the Qorvo Matter devices. For
more details visit `the what you need section of
Google <https://support.google.com/googlenest/answer/13127223?hl=en>`__.

- Google Nest Hub 2nd generation:

  - This device functions as a Thread Border Router for the google ecosystem. It allows the matter network to communicate to devices over Thread.
  - Required software version : 48.9.447810048 (or higher)
  - The Google Nest Hub 2nd generation is available on the `Google store <https://store.google.com/be/product/nest_hub_2nd_gen>`__.

- Android Tablet or Smartphone:

  - Android O (8.1, API level 27) or newer.
  - Google Play services version 22.48.14 or later
  - Bluetooth Low Energy (BLE) 4.2 or higher
  - Latest version of the `Google Home app <https://play.google.com/store/apps/details?id=com.google.android.apps.chromecast.app&hl=en_US>`__
  - To verify your mobile device has all the required Matter modules downloaded, you can follow `this <https://developers.home.google.com/matter/verify-services>`__ guide.

Step 2: Registering the Matter device as test device in the used Google account
-------------------------------------------------------------------------------

To comply to the security aspect of matter, a commissionee can only get
commissioned by a commissioner if it is registered as a trustworthy
consumer device in a `Distributed Compliance Ledger
(DCL) <https://csa-iot.org/certification/distributed-compliance-ledger/>`__
or under certain constraints as a test device for development purposes.

In below instructions, we will add the Qorvo Matter Device as a test
device in the `Google Home Developer
Console <https://console.home.google.com/projects>`__.

   **WARNING:** Google `does not support the onoff Light switch device
   type <https://developers.home.google.com/matter/supported-devices#onoff_light_switches>`__.
   Because the Google Home ecosystem does not expose the Matter Binding
   cluster to its controllers, end users cannot configure an On/Off
   Light Switch to control other Matter devices using the GHA. The
   switch application in this repository is supporting the Matter
   Binding cluster and as a consequence will not be able to control any
   light device in the google ecosystem.

Preparing a test device in the Google ecosystem
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   In this release by default all example applications will have test
   vendor ID set to *0xFFF1* which is mandatory for testing with Google.
   Also following product identifiers are used for the different types
   of applications:

   -  Matter light: 0x8005

Create a project in the `Google Home Developer
Console <https://console.home.google.com/projects>`__ and link all
devices to this account.

- Create a new Google account and make sure your tablet and Google Nest Hub 2nd generation are using this account.

  - Tablet: account switching can be done in settings > accounts and backup > Manage accounts.
  - Google Nest Hub: `Factory reset <https://support.google.com/googlenest/answer/7073477?hl=en#zippy=%2Cgoogle-nest-hub-nd-gen>`__ and set it up from scratch once the account switching is done on the tablet.

- Go to the `Google Home Developer Console <https://console.home.google.com/projects>`__ and login with the created Google account.
- Create a project “New Project” and give it a name. For example, “qorvo-matter-test”
- On the Home page you should be able to click “+ Add Matter Integration”
- click *Next: Develop*, which displays the Matter checklist page.
- click *Next: Setup*, which displays the Matter *Setup* page:
   Enter your Product name. For example: “Qorvo Matter Light”
- Click *Select device type* and select the device type from the dropdown menu. “Light”
- VendorID: Test VID and use *0xFFF1*
- ProductID: *0x8005* for light
- click *Save & continue*, which displays the Matter branding page.
- Leave the default
- click and click *Save*, which displays the Matter Develop page.
- click *Next:Test*, this accepts your integration and displays the Matter/Test page.
- Now you get a summary of your integrations and status should be “Ready”.
- Your matter device is now *ready* to be tested.

Step 3: Commissioning a Qorvo Matter light app, using QR code
-------------------------------------------------------------

1. Power on Qorvo IoT Development Kit for QPG6200 by plugging in the
   USB-cable to your PC.
2. Initiate the Bluetooth Low Energy (BLE) advertisements by `factory
   resetting <../../../Applications/Matter/Light/README.md#factory-reset>`__
   the Qorvo Matter light device.
3. Open the QR code related to the commissionee, for the light
   application in debug mode the link is printed over uart at boot of
   the application (See `Logging
   output <../../../Applications/Matter/Light/README.md#logging-output>`__
   to get the serial output). You can also use below QR code if you are
   commissioning the preprogrammed Matter Light application that comes
   with Qorvo IoT Development Kit for QPG6200:

.. image:: Images/qrcode.png
   :alt: QR code
   :align: center
   :width: 200px

4. Open the Google Home app on the Android tablet and click “+” in the
   upper left corner, “Set up device” > “New device” > “Select the
   correct Home”.
5. It will start looking for devices. Proceed with commissioning
   Matter-enabled device and scan the QR-code of the light device.
6. Now commissioning will start (takes 2 minutes or so).
7. After a while you need to add the Location of the device and Give the
   device a name.
8. Now it is part of the Google ecosystem, and you should be able to
   control it with Google (click on the tile).

Step 4: Controlling the Qorvo Matter light device
-------------------------------------------------

To control the devices there are 3 interfaces made available by Google:

- Google assistant
- Google Home App
- Smart Displays

Details on these can be found via Googles' `Matter control
guide <https://developers.home.google.com/matter/integration/control>`__

Future reads
-------------

-  `Get started with Matter in Google Home
   Ecosystem <https://developers.home.google.com/matter/get-started>`__
