"""Logger menu - generated from fwMenuLogger. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class Logger(MenuBase):
    r"""Logger (``r``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "logger": {"binary": False, "payload": [("info", "string")], "description": "Logger state change, prefixed with the instance number 0-3: <inst> armed, <inst> triggered, <inst> complete <csv> <rtix> <n> records, or <inst> error <reason>"},
    }

    def start(self) -> Result:
        r"""Start.

        Wire: ``r\s``

        Arms the logger with the current settings; Immediate trigger mode starts capturing at once. Emits logger events (armed/triggered/complete/error) as it runs.

        Allocates the PSRAM capture arena and arms the trigger state machine.
Immediate mode begins logging right away; Button/Expression modes wait for their trigger.
Files are written under /logs on the SD card as logNNNN.csv / logNNNN.rtix.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def stop(self) -> Result:
        r"""Stop.

        Wire: ``r\e``

        Stops the logger: an armed capture is discarded, a running capture drains its remaining events to the files and closes them.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def trigger(self) -> Result:
        r"""Trigger.

        Wire: ``r\t``

        Software trigger: fires an armed capture regardless of the configured trigger mode.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def status(self) -> Result:
        r"""Status.

        Wire: ``r\i``

        Prints the logger state, file format, trigger mode, output file names and event counters.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def file_format(self, value: int) -> Result:
        r"""File Format.

        Wire: ``r\f``

        Output file format for the next capture: CSV text, RTIX binary, or both

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def trigger_mode(self, value: int) -> Result:
        r"""Trigger Mode.

        Wire: ``r\m``

        How an armed capture is triggered: Immediate (on start), Button (a device button press), or Expression (a device expression becoming nonzero)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def trigger_button(self, value: int) -> Result:
        r"""Trigger Button.

        Wire: ``r\b``

        Device button that fires the trigger in Button mode

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])

    def trigger_expression(self, value: str) -> Result:
        r"""Trigger Expression.

        Wire: ``r\x``

        Expression evaluated every 50 ms in Expression mode; the trigger fires when it evaluates nonzero

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [encoding.enc_str(value)], [])

    def pre_trigger_ms(self, value: int) -> Result:
        r"""Pre Trigger Ms.

        Wire: ``r\p``

        Milliseconds of events kept from before the trigger (0-60000)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(value)], [])

    def post_trigger_ms(self, value: int) -> Result:
        r"""Post Trigger Ms.

        Wire: ``r\o``

        Milliseconds captured after the trigger before the files close (0 = until stop, max 600000)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(value)], [])

    def events(self, value: str) -> Result:
        r"""Events.

        Wire: ``r\v``

        Selects which events this instance captures: "all", "none", a comma-separated event-name list, or +name/-name to add/remove one event from the current selection

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_str(value)], [])

    def active_instance(self, value: int) -> Result:
        r"""Active Instance.

        Wire: ``r\n``

        Selects which of the four logger instances (0-3) the settings rows show and the start, stop and trigger commands act on; every instance keeps its own saved configuration

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_int(value)], [])

    def name(self, value: str) -> Result:
        r"""Name.

        Wire: ``r\a``

        Optional name for this instance; captures are written to /logs/<name>/<name>_NNNN.* instead of /logs/logI_NNNN.*

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(value)], [])
