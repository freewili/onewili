"""App Signals menu - generated from fwMenuAppSignals. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class AppSignals(MenuBase):
    r"""App Signals (``s\i``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "appSignal": {"binary": False, "payload": [("name", "string"), ("value", "float")], "description": "Streamed app-signal value."},
    }

    def app_signal_add(self, name: str) -> Result:
        r"""Add.

        Wire: ``s\i\a``

        Adds an app signal.

        Signal name

        Args:
            name: name (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(name)], [])

    def app_signal_remove(self, name: str) -> Result:
        r"""Remove.

        Wire: ``s\i\x``

        Removes an app signal.

        Signal name

        Args:
            name: name (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [encoding.enc_str(name)], [])

    def app_signal_rename(self, name: str, new_name: str) -> Result:
        r"""Rename.

        Wire: ``s\i\r``

        Renames an app signal.

        Old and new names

        Args:
            name: name (string).
            new_name: new_name (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_str(name), encoding.enc_str(new_name)], [])

    def app_signal_set(self, name: str, value: float) -> Result:
        r"""Set Value.

        Wire: ``s\i\s``

        Sets an app signal value.

        Signal name and value

        Args:
            name: name (string).
            value: value (float).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_str(name), encoding.enc_float(value)], [])

    def app_signal_get(self, name: str) -> Result:
        r"""Get Value.

        Wire: ``s\i\g``

        Gets an app signal value.

        Signal name

        Args:
            name: name (string).

        Returns:
            Result: Ok(name: str, value: float) or Err(message).
        """
        return self._call("g", [encoding.enc_str(name)], ["str", "float"])

    def app_signal_wave(self, name: str, wave: int) -> Result:
        r"""Apply Wave.

        Wire: ``s\i\w``

        Applies wave mode 0-8 (0 off; sine, triangle, square and saw at 0.5/2 Hz).

        Signal name and wave mode

        Args:
            name: name (string).
            wave: wave (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_str(name), encoding.enc_int(wave)], [])

    def app_signal_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream.

        Wire: ``s\i\t``

        Streams every defined app signal; 0 disables streaming.

        Stream interval in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(stream_rate_ms)], [])
