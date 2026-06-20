# -*- coding: utf-8 -*-
"""RCC data classes."""

from dataclasses import dataclass

API_VERSION_UNKNOWN = -1
API_VERSION_DEFAULT = 1


@dataclass
class DeviceInfo:
    """Device info as queried via PTCApi commands."""

    mac_address: str
    ble_address: str
    ptc_version: str
    app_version: str
    changelist: str
    chip_version: str
    product_id: str
    product_name: str
    api_version: int
