"""GUI Panels menu - generated from fwMenuGUIPanels. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class GUIPanels(MenuBase):
    r"""GUI Panels (``g\c``)."""

    def add_panel(self, use_tile: bool, tile_id: int, color: int | str, show_menu: bool) -> Result:
        r"""Add Panel.

        Wire: ``g\c\a``

        Reinitializes the custom panel for controls.

        Enter use tile (0/1), tile id, color, show menu (0/1)

        Args:
            use_tile: use_tile (bool).
            tile_id: tile_id (decS32).
            color: color (color).
            show_menu: show_menu (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_bool(use_tile), encoding.enc_int(tile_id), encoding.enc_color(color), encoding.enc_bool(show_menu)], [])

    def add_panel_picklist(self, use_tile: bool, tile_id: int, icon_id: int, log_index: int, back_color: int | str, fore_color: int | str, caption: str) -> Result:
        r"""Add Panel Picklist.

        Wire: ``g\c\b``

        Shows a panel that allows user to pick from a list.

        Enter use tile (0/1), tile id, icon id, log index, back color, fore color, caption

        Args:
            use_tile: use_tile (bool).
            tile_id: tile_id (decS32).
            icon_id: icon_id (decS32).
            log_index: log_index (decS32).
            back_color: back_color (color).
            fore_color: fore_color (color).
            caption: caption (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_bool(use_tile), encoding.enc_int(tile_id), encoding.enc_int(icon_id), encoding.enc_int(log_index), encoding.enc_color(back_color), encoding.enc_color(fore_color), encoding.enc_str(caption)], [])

    def show_panel(self, index: int) -> Result:
        r"""Show Panel.

        Wire: ``g\c\c``

        Brings the panel with the given index to the front on the DISPLAY.

        Enter index

        Args:
            index: index (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(index)], [])
