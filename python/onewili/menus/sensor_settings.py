"""Sensor Settings menu - generated from fwMenuSensorSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class SensorSettings(MenuBase):
    r"""Sensor Settings (``h\s\v``)."""

    def accel_range(self, value: int) -> Result:
        r"""Accel Range.

        Wire: ``h\s\v\a``

        Accelerometer full-scale range index

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(value)], [])

    def gyro_range(self, value: int) -> Result:
        r"""Gyro Range.

        Wire: ``h\s\v\g``

        Gyroscope full-scale range index

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_int(value)], [])

    def move_threshold(self, value: int) -> Result:
        r"""Move Threshold.

        Wire: ``h\s\v\m``

        The amount accel must change to signal movement

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def t_cal_scale(self, value: float) -> Result:
        r"""TCal Scale.

        Wire: ``h\s\v\s``

        Temperature calibration, the m of mX+b

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_float(value)], [])

    def t_cal_offset(self, value: float) -> Result:
        r"""TCal Offset.

        Wire: ``h\s\v\o``

        Temperature calibration, the b of mX+b

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_float(value)], [])

    def stream_defaults(self, value: int) -> Result:
        r"""Stream Defaults.

        Wire: ``h\s\v\b``

        Bitmask of sensor streams enabled at boot: 1 accel-legacy, 2 temp, 4 motion, 8 field, 16 env, 32 orientation

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])
