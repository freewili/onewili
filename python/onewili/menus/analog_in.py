"""Analog In Functions menu - generated from fwMenuAnalogIn. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class AnalogIn(MenuBase):
    r"""Analog In Functions (``i\j``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "ainIn": {"binary": False, "payload": [("v0", "float"), ("v1", "float"), ("v2", "float"), ("v3", "float")], "description": "Internal ADC voltages (connector channels 0-3)"},
        "adcIn": {"binary": False, "payload": [("v0", "float"), ("v1", "float"), ("v2", "float"), ("v3", "float")], "description": "TLA2024 voltages (connector channels 0-3)"},
    }

    def enable_analog_in_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream Analog In.

        Wire: ``i\j\s``

        Streams analog input values to the host at the given rate.

        Enter Sample Time in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(stream_rate_ms)], [])

    def read_analog_in2024(self) -> Result:
        r"""Read TLA2024.

        Wire: ``i\j\r``

        Reads the latest TLA2024 voltages for all 4 channels.

        Returns:
            Result: Ok(v0: float, v1: float, v2: float, v3: float) or Err(message).
        """
        return self._call("r", [], ["float", "float", "float", "float"])

    def config_analog_in2024(self, channel: int, mux: int, range: int) -> Result:
        r"""Config TLA2024 Channel.

        Wire: ``i\j\c``

        Configures a TLA2024 channel: mux 0-7 = A0-A1,A0-A3,A1-A3,A2-A3,A0-GND,A1-GND,A2-GND,A3-GND; range 0-5 = 6.144V,4.096V,2.048V,1.024V,0.512V,0.256V.

        Enter channel (0-3), mux (0-7), range (0-5)

        Args:
            channel: channel (decS32).
            mux: mux (decS32).
            range: range (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(channel), encoding.enc_int(mux), encoding.enc_int(range)], [])

    def set_data_rate2024(self, rate: int) -> Result:
        r"""TLA2024 Data Rate.

        Wire: ``i\j\f``

        Sets the TLA2024 data rate: 0-6 = 128,250,490,920,1600,2400,3300 SPS.

        Enter data rate (0-6)

        Args:
            rate: rate (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(rate)], [])

    def enable_analog_in2024_stream(self, stream_rate_ms: int) -> Result:
        r"""Stream TLA2024.

        Wire: ``i\j\t``

        Streams TLA2024 voltages to the host at the given rate (0 stops).

        Enter Sample Time in milliseconds

        Args:
            stream_rate_ms: stream_rate_ms (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(stream_rate_ms)], [])
