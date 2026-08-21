"""Sound Settings menu - generated from fwMenuSoundSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class SoundSettings(MenuBase):
    r"""Sound Settings (``h\s\n``)."""

    def quiet_threshold(self, value: int) -> Result:
        r"""Quiet Threshold.

        Wire: ``h\s\n\f``

        The mic level that counts as an active sound

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def speaker_volume(self, value: int) -> Result:
        r"""Speaker Volume.

        Wire: ``h\s\n\v``

        The multiplier applied to sound playback

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(value)], [])

    def recording_volume(self, value: int) -> Result:
        r"""Recording Volume.

        Wire: ``h\s\n\c``

        The multiplier applied to mic recording

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def record_len_sec(self, value: int) -> Result:
        r"""Record Len Sec.

        Wire: ``h\s\n\r``

        The default length of a recording in seconds

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(value)], [])

    def system_sounds(self, value: int) -> Result:
        r"""System Sounds.

        Wire: ``h\s\n\p``

        Sounds for system events (also gates all audio playback)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(value)], [])
