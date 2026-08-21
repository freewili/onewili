"""RTC Settings menu - generated from fwMenuRTCSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class RTCSettings(MenuBase):
    r"""RTC Settings (``h\s\c``)."""

    def year(self, value: int) -> Result:
        r"""Year.

        Wire: ``h\s\c\y``

        Set the year on the real-time clock

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [encoding.enc_int(value)], [])

    def month(self, value: int) -> Result:
        r"""Month.

        Wire: ``h\s\c\n``

        Set the month on the real-time clock

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_int(value)], [])

    def day(self, value: int) -> Result:
        r"""Day.

        Wire: ``h\s\c\e``

        Set the day of the month on the real-time clock

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(value)], [])

    def day_of_week(self, value: int) -> Result:
        r"""Day Of Week.

        Wire: ``h\s\c\w``

        Set the day of the week on the real-time clock

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_int(value)], [])

    def hours(self, value: int) -> Result:
        r"""Hours.

        Wire: ``h\s\c\o``

        Set the hour on the real-time clock (24-hour format)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(value)], [])

    def minutes(self, value: int) -> Result:
        r"""Minutes.

        Wire: ``h\s\c\m``

        Set the minutes on the real-time clock

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def seconds(self, value: int) -> Result:
        r"""Seconds.

        Wire: ``h\s\c\s``

        Set the seconds on the real-time clock

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(value)], [])

    def trim(self, value: int) -> Result:
        r"""Trim.

        Wire: ``h\s\c\t``

        Add or subtract n*2 clock cycles every minute

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(value)], [])
