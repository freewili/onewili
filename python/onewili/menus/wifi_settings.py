"""Wifi Settings menu - generated from fwMenuWifiSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class WifiSettings(MenuBase):
    r"""Wifi Settings (``h\s\w``)."""

    def enable_station_mode(self) -> Result:
        r"""Enable Station Mode.

        Wire: ``h\s\w\s``

        Connect the device to an existing Wi-Fi network in station mode, or disconnect from it

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def s_sid_for_station_mode(self, value: str) -> Result:
        r"""SSID for Station Mode.

        Wire: ``h\s\w\e``

        Set the name (SSID) of the Wi-Fi network to join in station mode

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_str(value)], [])

    def password_for_station_mode(self, value: str) -> Result:
        r"""Password for Station Mode.

        Wire: ``h\s\w\p``

        Set the password used to join the Wi-Fi network in station mode

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_str(value)], [])

    def enable_ap_mode(self) -> Result:
        r"""Enable AP Mode.

        Wire: ``h\s\w\a``

        Turn the device's own Wi-Fi access point on or off

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def a_p_auth(self, value: int) -> Result:
        r"""AP Auth.

        Wire: ``h\s\w\u``

        Choose the Wi-Fi security type used by the device's own access point

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(value)], [])

    def a_p_hide_ssid(self) -> Result:
        r"""AP hide SSID.

        Wire: ``h\s\w\i``

        Hide the access point's network name (SSID) so it isn't broadcast to nearby devices

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def s_sid_for_ap(self, value: str) -> Result:
        r"""SSID for AP.

        Wire: ``h\s\w\g``

        Set the network name (SSID) broadcast by the device's own access point

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_str(value)], [])

    def password_for_ap(self, value: str) -> Result:
        r"""Password for AP.

        Wire: ``h\s\w\x``

        Set the password required to join the device's own access point

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [encoding.enc_str(value)], [])
