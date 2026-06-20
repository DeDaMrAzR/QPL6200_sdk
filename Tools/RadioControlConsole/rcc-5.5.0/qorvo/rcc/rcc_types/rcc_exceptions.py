# -*- coding: utf-8 -*-
"""This module defines Exception types for RCC UI."""

from typing import Optional


class InvalidStateException(Exception):
    """This class defines the invalid state exception type."""

    def __init__(self, message: str, action: Optional[list[str]] = None):
        """Create an invalid state exception type.

        :param message: Exception message.
        :param action: Action to be taken by the user.
        """
        super().__init__(message)
        self.message = message
        self.action = action

    def __str__(self) -> str:
        return self.message
