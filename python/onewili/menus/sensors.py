"""Sensor Functions menu - generated from fwMenuSensors. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class Sensors(MenuBase):
    r"""Sensor Functions (``i\s``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "motion": {"binary": False, "payload": [("ax_mg", "decS32"), ("ay_mg", "decS32"), ("az_mg", "decS32"), ("gx_ddps", "decS32"), ("gy_ddps", "decS32"), ("gz_ddps", "decS32")], "description": "Accelerometer and gyroscope data"},
        "field": {"binary": False, "payload": [("mx_dut", "decS32"), ("my_dut", "decS32"), ("mz_dut", "decS32"), ("magnitude_dut", "decS32"), ("heading_cdeg", "decS32")], "description": "Magnetometer data"},
        "env": {"binary": False, "payload": [("temp_cc", "decS32"), ("rh_cpct", "decS32"), ("lux_clux", "decU32")], "description": "Temperature, humidity and ambient light"},
        "orientation": {"binary": False, "payload": [("roll_cdeg", "decS32"), ("pitch_cdeg", "decS32"), ("yaw_cdeg", "decS32"), ("heading_cdeg", "decS32"), ("flags", "decS32")], "description": "Fused roll, pitch, yaw and heading"},
    }

    def enable_motion_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream Motion.

        Wire: ``i\s\m``

        Streams accelerometer and gyroscope data to the host at the given rate. 0 stops the stream.

        Enter Sample Time in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(stream_rate_ms)], [])

    def enable_field_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream Field.

        Wire: ``i\s\f``

        Streams magnetometer data to the host at the given rate. 0 stops the stream.

        Enter Sample Time in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(stream_rate_ms)], [])

    def enable_env_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream Env.

        Wire: ``i\s\e``

        Streams temperature, humidity and ambient light to the host. This stream is change-driven: the rate is a heartbeat floor, so samples can arrive faster when readings move. 0 stops the stream.

        Enter Sample Time in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(stream_rate_ms)], [])

    def enable_orientation_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream Orientation.

        Wire: ``i\s\r``

        Streams fused roll, pitch, yaw and heading to the host at the given rate. 0 stops the stream.

        Enter Sample Time in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(stream_rate_ms)], [])

    def get_sensors(self) -> Result:
        r"""Get Sensors.

        Wire: ``i\s\g``

        Prints the most recent sample from each of the four sensor groups.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])
