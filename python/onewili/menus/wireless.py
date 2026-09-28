"""Wireless menu - generated from fwMenuWireless. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .nfc import NFC
from .esp32_flasher import ESP32Flasher
from .wifi import Wifi
from .bluetooth_le import BluetoothLE
from .ir import IR
from .lo_ra import LoRa
from .radio import Radio
from .rfid import RFID


class Wireless(MenuBase):
    r"""Wireless (``w``)."""

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.nfc = NFC(transport, nav_path + "\\n")
        self.esp32_flasher = ESP32Flasher(transport, nav_path + "\\a")
        self.wifi = Wifi(transport, nav_path + "\\w")
        self.bluetooth_le = BluetoothLE(transport, nav_path + "\\b")
        self.ir = IR(transport, nav_path + "\\i")
        self.lo_ra = LoRa(transport, nav_path + "\\l")
        self.radio = Radio(transport, nav_path + "\\r")
        self.rfid = RFID(transport, nav_path + "\\p")

    def e_sp32_mode(self, value: int) -> Result:
        r"""ESP32 Mode.

        Wire: ``w\e``

        What the ESP32-C5 runs: Default Firmware (the stock WiFi/BLE app) or OneWili API (a BSP app on the ESP32 drives MAIN's OneWili API). OneWili API does not install an app - flash one first.

        # ESP32 Mode

What the ESP32-C5 runs, and therefore whether MAIN treats its link as a OneWili client.

- **Default Firmware** (0) - the stock Bottlenose application: WiFi, BLE and the BLE terminal. MAIN ignores OneWili traffic from the ESP32.
- **OneWili API** (1) - a BSP app on the ESP32 drives MAIN's OneWili API over the Bottlenose link: MAIN answers its commands, mirrors text and binary events to it, and routes its peer-stream datagrams. The ESP32's power zone stays on while this is selected.

Selecting OneWili API does not put an app on the ESP32 - flash one first. The setting is saved and survives a reboot.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(value)], [])
