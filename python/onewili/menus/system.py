"""System Functions menu - generated from fwMenuSystem. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class System(MenuBase):
    r"""System Functions (``h\a``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "battery": {"binary": False, "payload": [("data", "string")], "description": "Battery charger status text"},
    }

    def enable_battery_stream(self, enable: int) -> Result:
        r"""Stream Battery Info.

        Wire: ``h\a\o``

        Enables or disables streaming of battery info to the host.

        Enter 1 to enable 0 to disable

        Args:
            enable: enable (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(enable)], [])

    def read_otp_info(self, offset: int, length: int) -> Result:
        r"""Read OTP Info.

        Wire: ``h\a\b``

        Reads bytes from the fused OTP identity blob (bl_otp_info v3). An unprovisioned device reads all zeros. Read in chunks of 256 bytes or less.

        Dec: Offset, Length (bytes within the 512-byte identity blob)

        Args:
            offset: offset (decS32).
            length: length (decS32).

        Returns:
            Result: Ok(otp_blob: bytes | bytearray) or Err(message).
        """
        return self._call("b", [encoding.enc_int(offset), encoding.enc_int(length)], ["bytes"])

    def boot_uf2(self, filename: str) -> Result:
        r"""Boot UF2.

        Wire: ``h\a\u``

        Reboots into the SBL bootloader, which chain-loads the named RAM-app UF2 from the SD card /update directory (card root as fallback). No response is sent on success — the device resets.

        Enter the UF2 filename (8.3, SD card /update directory)

        Args:
            filename: filename (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_str(filename)], [])

    def device_state(self) -> Result:
        r"""Device State.

        Wire: ``h\a\g``

        Report the device state for host sync: SD card host (none|main|usb), event host-streaming gate (0|1), active-stream mask (hex, bit index = event id). More space-separated fields may be appended later.

        Returns:
            Result: Ok(sd: str, hoststream: bool, activemask: str) or Err(message).
        """
        return self._call("g", [], ["str", "bool", "str"])

    def event_host_streaming(self, enable: int) -> Result:
        r"""Event Host Streaming.

        Wire: ``h\a\e``

        Enables or disables streaming of events to the host. When disabled, stream-class events are suppressed at the host output; protocol events still flow. Same gate as control bytes 0x05 (off) and 0x06 (on).

        Enter 1 to enable 0 to disable

        Args:
            enable: enable (decS32).

        Returns:
            Result: Ok(enabled: bool) or Err(message).
        """
        return self._call("e", [encoding.enc_int(enable)], ["bool"])
