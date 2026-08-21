"""Wireless menu - generated from fwMenuWireless. Do not edit."""
from __future__ import annotations

from result import Result

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
