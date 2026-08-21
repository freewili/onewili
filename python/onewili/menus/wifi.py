"""Wifi Functions menu - generated from fwMenuWifi. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .wifi_settings_2 import WifiSettings


class Wifi(MenuBase):
    r"""Wifi Functions (``w\w``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "wifistaInfo": {"binary": False, "payload": [("ip", "string"), ("gateway", "string"), ("mask", "string")], "description": "Station IP configuration (got IP)"},
        "wifiapInfo": {"binary": False, "payload": [("ip", "string"), ("gateway", "string"), ("mask", "string")], "description": "Access point IP configuration"},
        "wifiscan": {"binary": False, "payload": [("bssid", "string"), ("rssi", "decS32"), ("channel", "decU32"), ("band", "decU32"), ("authmode", "decU32"), ("ssid", "string")], "description": "Wifi scan record (SSID last so consumers can bounded-split)"},
        "wifiapdevcon": {"binary": False, "payload": [("ip", "string"), ("mac", "string")], "description": "Device connected to the access point"},
        "wifiapdevdc": {"binary": False, "payload": [("mac", "string")], "description": "Device disconnected from the access point"},
        "wsclientcon": {"binary": False, "payload": [("ip", "string")], "description": "Websocket client connected"},
        "wsclientdc": {"binary": False, "payload": [("ip", "string")], "description": "Websocket client disconnected"},
        "wifistations": {"binary": False, "payload": [("ip", "string"), ("mac", "string")], "description": "Connected access-point station record (one event per device)"},
        "httpget": {"binary": False, "payload": [("written", "decU32"), ("total", "decU32")], "description": "Download progress; total is 0 when the server sent no length"},
        "httpgetdone": {"binary": False, "payload": [("result", "decU32"), ("status", "decU32"), ("written", "decU32")], "description": "Download finished; result 0 is success, otherwise a bnose_http_result_t"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.settings = WifiSettings(transport, nav_path + "\\e")

    def toggle_events(self) -> Result:
        r"""Enable Wifi Events.

        Wire: ``w\w\r``

        Toggle Wifi Event Streaming

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def on_start_access_point(self, ssid: str, password: str, authmode: int, hidessid: bool) -> Result:
        r"""Start Access Point.

        Wire: ``w\w\a``

        Starts up Access Point with provided SSID and Password

        Enter SSID, Password, Auth Mode [0-3], Hide SSID flag separated by spaces

        Args:
            ssid: ssid (string).
            password: password (string).
            authmode: authmode (decS32).
            hidessid: hidessid (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(ssid), encoding.enc_str(password), encoding.enc_int(authmode), encoding.enc_bool(hidessid)], [])

    def on_discconect_from_station(self) -> Result:
        r"""Stop Access Point.

        Wire: ``w\w\t``

        Turns off Access Point

        Enter SSID and Password, seperetad by spaces

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def get_connected_devices(self) -> Result:
        r"""Get Stations connected to AP.

        Wire: ``w\w\g``

        Turns off Access Point

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])

    def on_connect_to_station(self, ssid: str, password: str) -> Result:
        r"""Connect to a Wifi Access Point.

        Wire: ``w\w\c``

        Connect to a WAP with provided SSID and Password

        String: SSID, Password

        Args:
            ssid: ssid (string).
            password: password (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_str(ssid), encoding.enc_str(password)], [])

    def on_discconect_from_station_2(self) -> Result:
        r"""Disconnect From Wifi Access Point.

        Wire: ``w\w\f``

        Disconnect from Wifi Stations

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [], [])

    def on_scan_for_access_points(self) -> Result:
        r"""Scan for Access Points.

        Wire: ``w\w\s``

        Scans for available WIFI networks

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def on_get_wif_info(self) -> Result:
        r"""Print out Wifi Info.

        Wire: ``w\w\p``

        Scans for available Wifi networks

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [], [])

    def on_http_get_to_sd(self, url: str, path: str) -> Result:
        r"""Download To SDCard.

        Wire: ``w\w\l``

        HTTP GET a URL and write it to a file on the SD card

        url path

        Args:
            url: url (string).
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_str(url), encoding.enc_str(path)], [])

    def on_http_get_abort(self) -> Result:
        r"""Cancel Download.

        Wire: ``w\w\x``

        Stops a download started with Download To SDCard

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [], [])
