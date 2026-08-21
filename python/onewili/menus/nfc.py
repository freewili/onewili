"""NFC Functions menu - generated from fwMenuNFC. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .nfc_saved_cards import NFCSavedCards
from .nfc_mifare_classic import NFCMifareClassic
from .nfc_raw import NFCRaw
from .nfc_extra import NFCExtra


class NFC(MenuBase):
    r"""NFC Functions (``w\n``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "nfc": {"binary": False, "payload": [("data", "string")], "description": "NFC card status text (card detected / removed)"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.saved_cards = NFCSavedCards(transport, nav_path + "\\s")
        self.mifare_classic = NFCMifareClassic(transport, nav_path + "\\m")
        self.raw = NFCRaw(transport, nav_path + "\\k")
        self.extra = NFCExtra(transport, nav_path + "\\x")

    def enable_reader(self, enable: int) -> Result:
        r"""Enable Reader.

        Wire: ``w\n\r``

        Enable/disable NFC reader with auto tag streaming

        Enter 1 to enable, 0 to disable

        Args:
            enable: enable (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(enable)], [])

    def print_card_info(self) -> Result:
        r"""Print Card Info.

        Wire: ``w\n\c``

        Display detailed info about detected card

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def get_status(self) -> Result:
        r"""Get Status (debug).

        Wire: ``w\n\g``

        Display NFC hardware state and debug info

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])
