# -*- coding: utf-8 -*-
"""Attributes Generic API."""

import abc
from enum import Enum
from typing import Any, Callable, Generic, Optional, Type, TypeVar, Union

from ..transport.gpAPI_2 import gpAPIConfirmIndication

T = TypeVar("T")
Tenum = TypeVar("Tenum", bound=Enum)


class PropertyAttributeDriver(abc.ABC):
    """Interface on setting and querying attribute values via PTC/CTC attribute model."""

    @abc.abstractmethod
    def set_attribute(
        self,
        attrib: int,
        value: int,
        is_bytedata: bool = False,
    ) -> Union[gpAPIConfirmIndication, None]:
        """Set an attribute by ID.

        :param attrib: Attribute ID.
        :param value: The value to set.
        :param is_bytedata: True if the supplied `int` value is an array., defaults to False
        :return: The API's response object.
        """

    @abc.abstractmethod
    def get_attribute(self, attrib: int) -> int:
        """Get an attribute value.

        :param attrib: The parameter ID.
        :return: The read value.
        """


class PropertyAttributeBase(abc.ABC, Generic[T]):
    """Generic class that represents an attribute.

    :param Generic: The Python type that this attribute holds.
    """

    def __init__(self, proto: Callable[[Any], None]):
        """Create an object.

        :param proto: The property.
        """
        self.__doc__ = proto.__doc__

    @abc.abstractmethod
    def __get__(self, __instance: PropertyAttributeDriver, __owner: Optional[type] = None) -> T:
        """Get the attribute's value.

        :param __instance: The driver's instance.
        :param __owner: Any, defaults to None
        :return: The read value.
        """

    @abc.abstractmethod
    def __set__(self, __instance: PropertyAttributeDriver, __value: T) -> None:
        """Set the attribute's value.

        :param __instance: The driver's instance.
        :param __value: The value to set.
        """


def property_attribute_int(attr_id: int) -> Type[PropertyAttributeBase[int]]:
    """Property decorator for an integer attribute.

    :param attr_id: The parameter ID in the API.
    :return: A property-like class.
    """

    class Attr(PropertyAttributeBase[int]):
        def __get__(self, __instance: PropertyAttributeDriver, __owner: Optional[type] = None) -> int:
            """Get the attribute's value.

            :param __instance: The driver's instance.
            :param __owner: Any, defaults to None
            :return: The read value.
            """
            return __instance.get_attribute(attr_id)

        def __set__(self, __instance: PropertyAttributeDriver, __value: int) -> None:
            """Set the attribute's value.

            :param __instance: The driver's instance.
            :param __value: The value to set.
            """
            __instance.set_attribute(attr_id, __value)

    return Attr


def property_attribute_enum(attr_id: int, enum_type: Type[Tenum]) -> Type[PropertyAttributeBase[Tenum]]:
    """Property decorator for an enumration attribute.

    :param attr_id: The parameter ID in the API.
    :param enum_type: The Python enumeration type to use.
    :return: A property-like class.
    """

    class Attr(PropertyAttributeBase[Tenum]):
        def __get__(self, __instance: PropertyAttributeDriver, __owner: Optional[type] = None) -> Tenum:
            """Get the attribute's value.

            :param __instance: The driver's instance.
            :param __owner: Any, defaults to None
            :return: The read value.
            """
            return enum_type(__instance.get_attribute(attr_id))

        def __set__(self, __instance: PropertyAttributeDriver, __value: Tenum) -> None:
            """Set the attribute's value.

            :param __instance: The driver's instance.
            :param __value: The value to set.
            """
            __instance.set_attribute(attr_id, __value.value)

    return Attr


def property_attribute_bool(attr_id: int) -> Type[PropertyAttributeBase[bool]]:
    """Property decorator for a boolean attribute.

    :param attr_id: The parameter ID in the API.
    :return: A property-like class.
    """

    class Attr(PropertyAttributeBase[bool]):
        def __get__(self, __instance: PropertyAttributeDriver, __owner: Optional[type] = None) -> bool:
            """Get the attribute's value.

            :param __instance: The driver's instance.
            :param __owner: Any, defaults to None
            :return: The read value.
            """
            return __instance.get_attribute(attr_id) != 0

        def __set__(self, __instance: PropertyAttributeDriver, __value: bool) -> None:
            """Set the attribute's value.

            :param __instance: The driver's instance.
            :param __value: The value to set.
            """
            __instance.set_attribute(attr_id, int(__value))

    return Attr


def property_attribute_bytes(attr_id: int, cached: bool = False) -> Type[PropertyAttributeBase[bytes]]:
    """Property decorator for a bytearray (bytes) attribute.

    :param attr_id: The parameter ID in the API.
    :param cached: Flag enabling client-side (we) to cache the last sent value.
    :return: A property-like class.
    """

    class Attr(PropertyAttributeBase[bytes]):
        def __get__(self, __instance: PropertyAttributeDriver, __owner: Optional[type] = None) -> bytes:
            """Get the attribute's value.

            :param __instance: The driver's instance.
            :param __owner: Any, defaults to None
            :return: The read value.
            """
            if cached:
                return getattr(__instance, f"__prop_attr_bytes_{attr_id}", bytes())
            raise NotImplementedError

        def __set__(self, __instance: PropertyAttributeDriver, __value: bytes) -> None:
            """Set the attribute's value.

            :param __instance: The driver's instance.
            :param __value: The value to set.
            """
            __instance.set_attribute(attr_id, int.from_bytes(__value, "little"), is_bytedata=True)
            setattr(__instance, f"__prop_attr_bytes_{attr_id}", __value)

    return Attr
