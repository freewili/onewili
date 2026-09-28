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

        Report the device state for host sync: SD card host (none|main|usb), event host-streaming gate (0|1), active-stream mask (hex, bit index = event id), clk_sys in Hz. More space-separated fields may be appended later.

        Returns:
            Result: Ok(sd: str, hoststream: bool, activemask: str, clksyshz: int) or Err(message).
        """
        return self._call("g", [], ["str", "bool", "str", "int"])

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

    def stream_write(self, dst: int, data: bytes | bytearray) -> Result:
        r"""Stream Write.

        Wire: ``h\a\w``

        Sends one peer-stream datagram (1-128 bytes) to another OneWili client through MAIN. Best effort: a datagram the destination cannot take now is dropped and counted, never queued behind.

        # Stream Write

Sends one peer-stream datagram to another OneWili client (DISPLAY, ESP32, CM0 or the PC host). MAIN routes it and stamps the sender as the client that issued this command.

- `dst` is the destination peer: 0 main (reserved, always dropped), 1 display, 2 esp32, 3 cm0, 4 host.
- `data` is 1-128 bytes. Longer datagrams are rejected, never split.
- `delivered` is 1 when the datagram left MAIN for the destination (or was queued for a polling client), 0 when it was dropped: the destination is not using streams right now, its link had no room, or its queue is full.

This is the text route of `ow_stream_write`; the DISPLAY and ESP32 have faster push links for the same datagrams. See also `p` (Stream Poll) and `c` (Stream Status).

        Enter destination peer (0 main, 1 display, 2 esp32, 3 cm0, 4 host) followed by 1-128 data bytes (hex, space separated)

        Args:
            dst: dst (decS32).
            data: data (hexbytes).

        Returns:
            Result: Ok(delivered: bool) or Err(message).
        """
        return self._call("w", [encoding.enc_int(dst), encoding.enc_bytes(data)], ["bool"])

    def stream_poll(self, max: int) -> Result:
        r"""Stream Poll.

        Wire: ``h\a\p``

        Pops peer-stream datagrams queued for the calling client: frames popped, frames still queued, frames dropped for this client so far, then the datagrams packed as [src][len][bytes] records.

        # Stream Poll

Pops the datagrams MAIN has queued for the calling client, oldest first, as many whole ones as fit in `max` bytes.

- `frames` is how many were popped (0 when none are waiting).
- `queued` is how many are still waiting.
- `dropped` is how many datagrams addressed to this client were dropped so far (queue full), free-running.
- `data` packs the popped datagrams as records: source peer (1 byte), length (1 byte), then that many bytes.

Only clients without a push link (the PC host and the CM0) have a queue; for the DISPLAY and ESP32 the datagrams are pushed on their own links and this returns none. This is the text route of `ow_stream_poll`. See also `w` (Stream Write) and `c` (Stream Status).

        Enter the most bytes of packed records to return (at least 130)

        Args:
            max: max (decS32).

        Returns:
            Result: Ok(frames: int, queued: int, dropped: int, data: bytes | bytearray) or Err(message).
        """
        return self._call("p", [encoding.enc_int(max)], ["int", "int", "int", "bytes"])

    def stream_status(self) -> Result:
        r"""Stream Status.

        Wire: ``h\a\c``

        Peer-stream state for the calling client: the datagram MTU, datagrams waiting in its queue, datagrams addressed to it that MAIN dropped, and datagrams it sent that MAIN dropped.

        # Stream Status

Reports the peer-stream counters MAIN keeps for the client that issues it.

- `mtu` is the largest datagram in bytes, the same on every link.
- `queued` is how many datagrams wait in this client's queue (always 0 for push-link clients).
- `droppedto` counts datagrams addressed to this client that MAIN dropped; `droppedfrom` counts datagrams this client sent that MAIN dropped. Both are free-running since MAIN booted.

`ow_stream_drops` on a text client is their sum. See also `w` (Stream Write) and `p` (Stream Poll).

        Returns:
            Result: Ok(mtu: int, queued: int, droppedto: int, droppedfrom: int) or Err(message).
        """
        return self._call("c", [], ["int", "int", "int", "int"])
