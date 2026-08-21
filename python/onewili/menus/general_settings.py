"""General Settings menu - generated from fwMenuGeneralSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class GeneralSettings(MenuBase):
    r"""General Settings (``h\s\e``)."""

    def startup_wasm_script(self, value: str) -> Result:
        r"""Startup Wasm Script.

        Wire: ``h\s\e\a``

        Path to wasm or RTHON script.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(value)], [])

    def startup_zoom_script(self, value: str) -> Result:
        r"""Startup Zoom Script.

        Wire: ``h\s\e\b``

        Path to zoom script.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_str(value)], [])

    def default_fpga_script(self, value: str) -> Result:
        r"""Default FPGA Script.

        Wire: ``h\s\e\c``

        Path to FPGA bit file

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_str(value)], [])

    def wasm_debug_level(self, value: int) -> Result:
        r"""Wasm debug level.

        Wire: ``h\s\e\f``

        Debug messaging from WiliWasm

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])
