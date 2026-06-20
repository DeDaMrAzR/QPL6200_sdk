Installation Scripts
====================

This folder contains scripts necessary for the automatic setup of the build
environment. These scripts ensure that all required dependencies for development
and testing are configured on the build machine.

Usage
-----

   **NOTE:** Installation scripts require ``sudo`` privileges.

The ``activate.sh`` is the primary script for setting up the build
environment, it should be used as described in the :ref:`Installation` section.

The ``bootstrap.sh`` script is the core implementation of ``activate.sh``. It
performs the following actions:

-  Installs the `Arm GNU Toolchain <https://developer.arm.com/downloads/-/arm-gnu-
   toolchain-downloads>`__ for GNU/Linux in the directory ``/opt/TOOL_ARMGCCEMB``.
-  Installs necessary Debian/Ubuntu packages using ``apt``.
-  Creates a Python 3.9 virtual environment using the ``venv`` module.
-  Installs the ``Qorvo Platform Tools`` package along with other required Python packages in the
   virtual environment.
-  Initializes and updates repository submodules.

The ``build_install_spake2p.sh`` script builds the `spake2p <https://project-chip.github.
io/connectedhomeip-doc/src/tools/spake2p/README.html>`__ tool from the Matter submodule
(``Components/ThirdParty/Matter/repo``) and installs it into ``/usr/bin``. It is executed
from the ``bootstrap.sh`` script.
