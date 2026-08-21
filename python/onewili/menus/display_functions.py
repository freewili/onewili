"""Display Functions menu - generated from fwMenuDisplayFunctions. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class DisplayFunctions(MenuBase):
    r"""Display Functions (``h\v``)."""

    def list_display_apps(self) -> Result:
        r"""List Display Apps.

        Wire: ``h\v\l``

        Lists the firmware images available in the SD card /apps/ directory.

        List /apps on the SD card

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [], [])

    def restore_display_firmware(self) -> Result:
        r"""Restore Display Firmware.

        Wire: ``h\v\r``

        Reflashes /firmware/FW2Display.uf2 to restore the standard display GUI.

        Reflash the stock display firmware

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def display_bl_version(self) -> Result:
        r"""Display Bootloader Version.

        Wire: ``h\v\v``

        Enters the display bootloader, reads its version, and releases the link without transferring anything.

        Query the display CPU bootloader

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [], [])

    def reset_display_cpu(self) -> Result:
        r"""Reset Display CPU.

        Wire: ``h\v\x``

        Pulses the display processor reset so it cold-boots its flash image.

        Pulse the display reset line

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [], [])

    def power_cycle_display_cpu(self) -> Result:
        r"""Power Cycle Display.

        Wire: ``h\v\c``

        Cuts the display processor's power rail and restores it, giving a true power-on reset. Heavier than Reset Display CPU, which only pulses RUN. Bootloader entry uses RUN/BOOT on its own; use this when a warm reset is not enough.

        Drop and restore the display's 3V3_S9 rail

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def set_ram_app_arg(self, text: str) -> Result:
        r"""Set RAM App Argument.

        Wire: ``h\v\g``

        Arms up to 128 bytes for the NEXT Run RAM App, placed at a fixed address near the top of the display's RAM window. Blank clears it. An armed argument makes the launch noticeably slower: the fused bootloader cannot seek, so the loader must pad the wire up to that address.

        Text handed to the next RAM app (blank clears)

        Args:
            text: text (str).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_str(text)], [])

    def run_app_on_display(self, filename: str) -> Result:
        r"""Run App On Display.

        Wire: ``h\v\a``

        Asks the display processor to load and run /apps/<filename> itself: it reads the UF2 over the SD link, shows a progress bar on its own screen, and jumps to the image. Works for UF2s targeting the PSRAM window (0x11000000, up to ~4 MB) or the RAM window (0x20000000, up to 448 KB) -- the display copies the image to its run address at the moment of launch. Flash is untouched; Reset Display CPU restores the stock firmware. Progress and errors appear on the display, not here.

        Enter the UF2 filename in /apps

        Args:
            filename: filename (str).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(filename)], [])

    def run_psram_app(self, filename: str) -> Result:
        r"""Run PSRAM App.

        Wire: ``h\v\p``

        Runs /apps/<filename> on the display processor from PSRAM (0x11000000 window, up to 8 MB). Two-hop launch: a small SRAM stub is staged through the fused bootloader, then the stub receives the image into PSRAM and jumps to it. Flash is untouched; Reset Display CPU restores the stock firmware. The image must be a UF2 whose blocks target the PSRAM window.

        Enter the UF2 filename in /apps

        Args:
            filename: filename (str).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_str(filename)], [])

    def load_psram_data(self, filename: str, offset: int) -> Result:
        r"""Load PSRAM Data.

        Wire: ``h\v\s``

        Stages /apps/<filename> verbatim into the display's PSRAM at <offset> bytes from 0x11000000, and leaves the loader stub running instead of launching anything. For bulk assets that would otherwise have to travel inside the app's own UF2. The file is taken as raw bytes: no UF2 decode. Repeat for as many blobs as needed, then Run PSRAM App -- the stub stays resident between calls, so only the first pays the two-hop entry, and the launch overwrites only what the app image itself covers. Staged data does NOT survive a display reset.

        Filename in /apps and a hex PSRAM offset

        Args:
            filename: filename (str).
            offset: offset (hexU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_str(filename), encoding.enc_hex(offset, 8)], [])
