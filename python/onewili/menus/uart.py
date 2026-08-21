"""UART Functions menu - generated from fwMenuUART. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .uart_settings import UARTSettings


class UART(MenuBase):
    r"""UART Functions (``i\u``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "uart1": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "uart receive frame"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.settings = UARTSettings(transport, nav_path + "\\s")

    def u_art_write(self, data_bytes: bytes | bytearray) -> Result:
        r"""Write.

        Wire: ``i\u\w``

        Writes data to a specific I2C Address

        Enter Data Byte(s) Separated By Spaces

        Args:
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_bytes(data_bytes)], [])

    def toggle_stream(self) -> Result:
        r"""Enable UART Read Events.

        Wire: ``i\u\r``

        Reads the number from the address

        Returns:
            Result: Ok(data_bytes: bytes | bytearray) or Err(message).
        """
        return self._call("r", [], ["bytes"])

    def uart_enable_api_mode(self) -> Result:
        r"""Enable UART API mode.

        Wire: ``i\u\t``

        Tests all addresses for I2C Response

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])
