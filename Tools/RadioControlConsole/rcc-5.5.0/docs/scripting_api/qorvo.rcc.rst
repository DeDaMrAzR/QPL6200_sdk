Scripting API
=============

The application is based on the following layers:

* [Top-Level RCC] User command line interface parser and driver
* [RccDriver x] CLI processor for API version `x`
* [PtcDriver x] Programming API for API version `x`
* [Transport] Message serialization and lower-layer communication


.. code-block::

   +---------------+
   | Top-level RCC |--*--*--*--*--*--*--*--*.                User Commands
   +---------------+                         \                 e.g. SETCW U ON
            |                                |
           Use                               |
            |                                |
      +---------------+                 +-------------+
      | Generic Types |<--Implement-----| RccDriver x |      String to Py. obj.
      +---------------+                 +-------------+        Translation
                                               |
                                           Has Member
                                             `api`
                                               |
                                        +-------------+
                                        | PtcDriver x |     Py. object to binary
                                        +-------------+      Message Translation
                                               |
                                               |         ------+
                                           +-------+           |
                                           | gpAPI |           |
                                           +-------+           |
                                               |               |  Transport
                                               |               |  Layer
                                        +-------------+        |
                                        | peakconnect |        |
                                        +-------------+        |
                                                         ------+



Subpackages
-----------

.. toctree::
   :maxdepth: 1

   qorvo.rcc.rcc
   qorvo.rcc.rcc_types
   qorvo.rcc.ctc_versions.v1
   qorvo.rcc.ptc_versions.v1
   qorvo.rcc.ptc_versions.v2
   qorvo.rcc.transport
