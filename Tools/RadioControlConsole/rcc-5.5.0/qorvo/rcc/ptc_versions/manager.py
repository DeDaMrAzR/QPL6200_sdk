"""This module provides general functions and helpers."""

from typing import Optional, Union

from qorvo.rcc.ptc_versions.v1 import ptcdriver as PtcV1
from qorvo.rcc.ptc_versions.v1 import rccdriver as RccV1
from qorvo.rcc.ptc_versions.v2 import ptcdriver as PtcV2
from qorvo.rcc.ptc_versions.v2 import rccdriver as RccV2
from qorvo.rcc.ptc_versions.v3 import ptcdriver as PtcV3
from qorvo.rcc.ptc_versions.v3 import rccdriver as RccV3
from qorvo.rcc.rcc_types.deviceinfo import API_VERSION_DEFAULT, API_VERSION_UNKNOWN
from qorvo.rcc.rcc_types.generic import AppGeneric
from qorvo.rcc.transport.peakconnect import peakConnector

KnownPtcDrivers = Union[PtcV1.PTC, PtcV2.PTC, PtcV3.PTC]
KnownRccDrivers = Union[RccV1.PTCDriver, RccV2.PTCDriver, RccV3.PTCDriver]

version_map = {
    1: (PtcV1.PTC, RccV1.PTCDriver),
    2: (PtcV2.PTC, RccV2.PTCDriver),
    3: (PtcV3.PTC, RccV3.PTCDriver),
}


def connect_to_ptc_driver(
    connector: peakConnector,
    app: Optional[AppGeneric] = None,
    force_version: Optional[int] = None,
) -> KnownPtcDrivers:
    """Connect to DUT and return a known PTC object.

    :param connector: The transport connection object.
    :param app: Optional application to show errors.
    :raises ValueError: If the API is not supported.
    :return: The DUT object.
    """
    dut = version_map[API_VERSION_DEFAULT][0](connector)
    dut_api_version = dut.get_device_api_version()

    if dut_api_version == API_VERSION_UNKNOWN:
        dut_api_version = API_VERSION_DEFAULT
        if app is not None:
            app.show_user_warning(
                f"The DUT did not report API version, using the default version {API_VERSION_DEFAULT}"
            )

    if force_version is not None and force_version != dut_api_version:
        dut_api_version = force_version
        if app is not None:
            app.show_user_warning(
                f"Forcing to use API version {force_version}, but DUT reported version {dut_api_version}. "
                "Some commands may not work properly."
            )

    new_version_class_tuple = version_map.get(dut_api_version, None)
    if new_version_class_tuple is None:
        raise ValueError(f"Unable to handle API version {dut_api_version}")

    if dut_api_version != API_VERSION_DEFAULT:
        dut.disconnect()
        dut = new_version_class_tuple[0](connector)

    dut.own_connector()
    return dut


def connect_to_rcc_driver(
    connector: peakConnector,
    app: AppGeneric,
    force_version: Optional[int] = None,
) -> KnownRccDrivers:
    """Connect to DUT and return a known RCC object.

    :param connector: The transport connection object.
    :param app: Optional application to show errors.
    :raises ValueError: If the API is not supported.
    :return: The DUT object.
    """
    dut = version_map[API_VERSION_DEFAULT][1](app, connector)
    dut_api_version = dut.get_device_info().api_version

    if dut_api_version == API_VERSION_UNKNOWN:
        app.show_user_warning(f"The DUT did not report API version, using the default version {API_VERSION_DEFAULT}")
        dut_api_version = API_VERSION_DEFAULT

    if force_version is not None and force_version != dut_api_version:
        dut_api_version = force_version
        app.show_user_warning(
            f"Forcing to use API version {force_version}, but DUT reported version {dut_api_version}. "
            "Some commands may not work properly."
        )

    new_version_class_tuple = version_map.get(dut_api_version, None)
    if new_version_class_tuple is None:
        raise ValueError(f"Unable to handle API version {dut_api_version}")

    if dut_api_version != API_VERSION_DEFAULT:
        dut.stop()
        dut = new_version_class_tuple[1](app, connector)

    dut.promote_to_connection_owner()
    return dut
