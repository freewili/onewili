"""Apps functions menu - generated from fwMenuApps. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class Apps(MenuBase):
    r"""Apps functions (``a``)."""

    def launch_app(self, app_id: int) -> Result:
        r"""Launch App.

        Wire: ``a\a``

        Switch the built-in display to the app with the given app ID

        Enter app id

        Args:
            app_id: app_id (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(app_id)], [])

    def run_app(self, filename: str) -> Result:
        r"""Run App.

        Wire: ``a\r``

        Runs /apps/<filename> on the display processor. The destination is inferred by reading the image, not the name: a UF2 whose blocks target SRAM is staged in RAM and launched; one targeting the PSRAM window (0x11000000) is staged into PSRAM through the loader stub and launched; anything else is written to flash. RAM and PSRAM launches leave flash untouched. A flash load takes 30-60 seconds with the screen blank.

        Enter the UF2 filename in /apps

        Args:
            filename: filename (str).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_str(filename)], [])
