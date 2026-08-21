"""MIFARE Classic menu - generated from fwMenuNFCMifareClassic. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class NFCMifareClassic(MenuBase):
    r"""MIFARE Classic (``w\n\m``)."""

    def read_with_keys(self) -> Result:
        r"""Read with Keys.

        Wire: ``w\n\m\r``

        Authenticate and read sectors using known keys

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def dictionary_attack(self, path: str) -> Result:
        r"""Dictionary Attack.

        Wire: ``w\n\m\a``

        Try keys from dictionary file to recover unknown keys

        The dictionary is a plain-text list of candidate 6-byte keys, one per line as 12 hex chars ('#' starts a comment); it is Flipper-Zero compatible.

Where to get one: the Flipper Zero 'mf_classic_dict.nfc' (bundled with Flipper firmware and in community key packs), or any MIFARE Classic key list.

Where to save it: put it on the SD card at /nfc/assets/mf_classic_dict.nfc (the default shown in brackets), or pass a full path as the argument. Each key is tried against every sector; keys that authenticate are cached so Dump Card can then read the card.

        Enter dictionary path (or blank for default)

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(path)], [])

    def dump_card(self) -> Result:
        r"""Dump Card.

        Wire: ``w\n\m\u``

        Read all sectors with known keys and display contents

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [], [])
