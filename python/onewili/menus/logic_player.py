"""Logic Player Functions menu - generated from fwMenuLogicPlayer. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class LogicPlayer(MenuBase):
    r"""Logic Player Functions (``i\p``)."""

    def setup_player(self, sample_rate_ns: int, sample_count: int, pin_start: int, pin_stop: int, start_mode: int, trigger_pin: int, loop: bool) -> Result:
        r"""configure.

        Wire: ``i\p\c``

        Configures digital playback

        Enter SampleRateNs SampleCount PinStart PinStop StartMode(0=now,1=rising,2=falling) TriggerPin Loop(0/1)

        Args:
            sample_rate_ns: sample_rate_ns (decU32).
            sample_count: sample_count (decS32).
            pin_start: pin_start (decS32).
            pin_stop: pin_stop (decS32).
            start_mode: start_mode (decS32).
            trigger_pin: trigger_pin (decS32).
            loop: loop (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(sample_rate_ns), encoding.enc_int(sample_count), encoding.enc_int(pin_start), encoding.enc_int(pin_stop), encoding.enc_int(start_mode), encoding.enc_int(trigger_pin), encoding.enc_bool(loop)], [])

    def setup_analog(self, mask: int, analog_rate_ns: int, analog_resolution: int) -> Result:
        r"""configure analog.

        Wire: ``i\p\a``

        Configures DAC playback

        Enter AnalogMask(bit0=Aout0 Bit3=Aout3) AnalogRateNs AnalogRes(8/16)

        Args:
            mask: mask (decU32).
            analog_rate_ns: analog_rate_ns (decS32).
            analog_resolution: analog_resolution (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(mask), encoding.enc_int(analog_rate_ns), encoding.enc_int(analog_resolution)], [])

    def load_file(self, file_path: str) -> Result:
        r"""load.

        Wire: ``i\p\l``

        Loads a raw buffer from the filesystem

        Enter file path to load into the play buffer

        Args:
            file_path: file_path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_str(file_path)], [])

    def start(self) -> Result:
        r"""start.

        Wire: ``i\p\s``

        Starts playback

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def stop(self) -> Result:
        r"""stop.

        Wire: ``i\p\e``

        Stops playback

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])
