"""Saved Cards menu - generated from fwMenuNFCSavedCards. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class NFCSavedCards(MenuBase):
    r"""Saved Cards (``w\n\s``)."""

    def list_saved_cards(self) -> Result:
        r"""List Saved Cards.

        Wire: ``w\n\s\l``

        List all .nfc files in the saved cards directory

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [], [])

    def load_card(self, path: str) -> Result:
        r"""Load Card.

        Wire: ``w\n\s\o``

        Load card data from .nfc file

        Enter filename to load

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_str(path)], [])

    def save_current_card(self, path: str) -> Result:
        r"""Save Current Card.

        Wire: ``w\n\s\s``

        Save currently detected card to .nfc file

        Enter filename to save

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_str(path)], [])

    def emulate_card(self, path: str) -> Result:
        r"""Play (Emulate) Card.

        Wire: ``w\n\s\e``

        Transmit (emulate) a saved card's NFC-A UID/ATQA/SAK

        Enter filename to emulate

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_str(path)], [])

    def stop_emulation(self) -> Result:
        r"""Stop Emulation.

        Wire: ``w\n\s\t``

        Stop NFC card emulation

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])
