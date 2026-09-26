"""ZoomIO Functions menu - generated from fwMenuZoomIO. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class ZoomIO(MenuBase):
    r"""ZoomIO Functions (``s\b``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "zoomio": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "ZoomIO received packet (hex bytes)"},
    }

    def enable_rx_stream(self, enable: int) -> Result:
        r"""Stream ZoomIO Data.

        Wire: ``s\b\o``

        Enables or disables streaming of ZoomIO receive data to the host.

        # Stream ZoomIO Data

Enable or disable forwarding of bytes received on the ZoomIO RX FIFO to the host.

## Argument

- `enable` (`decS32`) — `1` to start streaming, `0` to stop.

## Behavior

While streaming is enabled, the menu's `processEvents` loop drains up to 64 bytes per pass from the ZoomIO RX FIFO and emits them to the host as:

- A console event named `zoomio` with the payload as space‑separated hex bytes.
- A `CAN_API_RX_ZOOM_IO` response (binary), when CAN API streaming is also active.

Packet framing is preserved: the first byte of each emitted record is the packet length, followed by the packet bytes. A new packet boundary is detected from the FIFO's first‑byte‑of‑packet flag.

## Examples

```
o 1   # start streaming RX data to host
o 0   # stop streaming
```

## Returns

- `success=basic` — `true` if the argument parsed successfully, `false` on invalid input.

## Notes

- Streaming only affects RX delivery to the host; it does not change ZoomIO transmit behavior or the schedule table.
- Pair with `w` (Write to FIFO), `u` (Update Schedule Table), and `p` (Setup Schedule Table) to drive traffic while observing responses.

        Enter 1 to enable 0 to disable

        Args:
            enable: enable (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(enable)], [])

    def send_data(self, delay: int, data: bytes | bytearray) -> Result:
        r"""Write to FIFO.

        Wire: ``s\b\w``

        Sends a single ZoomIO message after the given delay (us).

        Enter delay us and Hex Data Byte(s) Separated By Spaces

        Args:
            delay: delay (decS32).
            data: data (byteArray).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_int(delay), encoding.enc_bytes(data)], [])

    def update_table_data(self, table_index: int, delay: int, data: bytes | bytearray) -> Result:
        r"""Update Schedule Table.

        Wire: ``s\b\u``

        Updates a schedule-table transmit message.

        Enter Table Index, delay us and hex data byte(s) Separated By Spaces

        Args:
            table_index: table_index (decS32).
            delay: delay (decS32).
            data: data (byteArray).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(table_index), encoding.enc_int(delay), encoding.enc_bytes(data)], [])

    def enable_schedule_table(self, number_of_entries: int) -> Result:
        r"""Setup Schedule Table.

        Wire: ``s\b\p``

        Sets up the schedule table size (0 to disable).

        Enter number of table items, 0 to disable

        Args:
            number_of_entries: number_of_entries (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(number_of_entries)], [])

    def compile_test(self) -> Result:
        r"""Compile test.

        Wire: ``s\b\c``

        Compiles built-in ZoomIO milestone program and launches it on core1 as RISC-V

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def run_zio(self, path: str) -> Result:
        r"""Run ZoomIO.

        Wire: ``s\b\r``

        Compile and run a ZoomIO program on the RISC-V core1

        Enter the .zio script path

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_str(path)], [])

    def stop_zio(self) -> Result:
        r"""Stop ZoomIO.

        Wire: ``s\b\s``

        Reset core1 to stop the running program

        Stop the running ZoomIO program

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def exec_probe(self) -> Result:
        r"""Probe Exec Window.

        Wire: ``s\b\x``

        Stages a known pattern in ZoomIO's SCRATCH_X exec window, runs the full core1 launch sequence, and reads it back

        Probe the SCRATCH_X execution window

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [], [])
