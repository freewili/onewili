"""IR Functions menu - generated from fwMenuIR. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class IR(MenuBase):
    r"""IR Functions (``w\i``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "irrx": {"binary": False, "payload": [("code", "hexU32")], "description": "Received IR code"},
    }

    def enable_ir_stream(self, enable: int) -> Result:
        r"""Stream IR.

        Wire: ``w\i\o``

        Enables or disables streaming of received IR codes to the host.

        Enter 1 to enable 0 to disable

        Args:
            enable: enable (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(enable)], [])

    def send_ir_data(self, ir_code: int) -> Result:
        r"""Send IR.

        Wire: ``w\i\a``

        Transmits a 4-byte IR code.

        Enter 4 byte IR code in hex

        Args:
            ir_code: ir_code (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(ir_code)], [])

    def ir_self_test(self) -> Result:
        r"""IR Self Test.

        Wire: ``w\i\t``

        Transmits one frame per supported protocol and checks that the on-board receiver decodes each one back. Takes a few seconds and emits infrared.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def ir_list_dir(self, path: str) -> Result:
        r"""List IR Dir.

        Wire: ``w\i\l``

        Lists the directories and .ir files on the SD card, directories first. Empty path lists \ir\.

        Directory to list, empty for \ir\

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_str(path)], [])

    def ir_list_buttons(self, path: str) -> Result:
        r"""List IR Buttons.

        Wire: ``w\i\b``

        Lists the buttons in one Flipper .ir file with the index each one is sent by. Malformed entries are counted as skipped, not listed.

        Path of a .ir file

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_str(path)], [])

    def ir_send_button(self, index: int, path: str) -> Result:
        r"""Send IR Button.

        Wire: ``w\i\s``

        Transmits one button from a .ir file, repeated by the IR Repeat setting. Emits infrared.

        Button index then the .ir file path

        Args:
            index: index (decU32).
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(index), encoding.enc_str(path)], [])

    def ir_save_capture(self, name: str) -> Result:
        r"""Save IR Capture.

        Wire: ``w\i\c``

        Appends the last received signal to \ir\learned.ir under this name, decoded when the protocol was recognised and as raw timings when it was not.

        Name for the captured signal

        Args:
            name: name (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_str(name)], [])

    def ir_status(self) -> Result:
        r"""IR Status.

        Wire: ``w\i\i``

        Reports the IR engine's carrier, repeat count, capture overruns and whether the \ir\ tree exists on the card.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def i_r_carrier(self, value: int) -> Result:
        r"""IR Carrier.

        Wire: ``w\i\f``

        Default transmit carrier frequency. Only these four are legal; a .ir raw entry with its own frequency line overrides this for that entry.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def i_r_repeat(self, value: int) -> Result:
        r"""IR Repeat.

        Wire: ``w\i\r``

        How many times Send IR Button transmits each frame, 1 to 5, with a 40 ms gap between repeats.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(value)], [])
