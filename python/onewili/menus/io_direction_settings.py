"""IO Directions menu - generated from fwMenuIODirectionSettings. Do not edit."""
from __future__ import annotations

from result import Result

from ..menubase import MenuBase
from ..transport import Transport


class IODirectionSettings(MenuBase):
    r"""IO Directions (``i\g\a``)."""

    def s_pi1_rx12(self) -> Result:
        r"""SPI1 Rx (12).

        Wire: ``i\g\a\a``

        IO direction for SPI1 Rx pin 12 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def g_pio2626(self) -> Result:
        r"""GPIO 26 (26).

        Wire: ``i\g\a\b``

        IO direction for GPIO 26 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [], [])

    def s_pi1cs13(self) -> Result:
        r"""SPI1 CS (13).

        Wire: ``i\g\a\c``

        IO direction for SPI1 CS pin 13 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def g_pio27(self) -> Result:
        r"""GPIO (27).

        Wire: ``i\g\a\l``

        IO direction for GPIO 27 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [], [])

    def u_art1_rx9(self) -> Result:
        r"""UART1 Rx (9).

        Wire: ``i\g\a\e``

        IO direction for UART1 Rx pin 9 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def u_art1cts10(self) -> Result:
        r"""UART1 CTS (10).

        Wire: ``i\g\a\f``

        IO direction for UART1 CTS pin 10 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [], [])

    def u_art1_tx8(self) -> Result:
        r"""UART1 Tx (8).

        Wire: ``i\g\a\g``

        IO direction for UART1 Tx pin 8 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])

    def u_art1rts11(self) -> Result:
        r"""UART1 RTS (11).

        Wire: ``i\g\a\m``

        IO direction for UART1 RTS pin 11 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [], [])

    def s_pi1_tx15(self) -> Result:
        r"""SPI1 Tx (15).

        Wire: ``i\g\a\i``

        IO direction for SPI1 Tx pin 15 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def s_pi1sclk14(self) -> Result:
        r"""SPI1 SCLK (14).

        Wire: ``i\g\a\j``

        IO direction for SPI1 SCLK pin 14 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [], [])

    def g_pio2525(self) -> Result:
        r"""GPIO25 (25).

        Wire: ``i\g\a\k``

        IO direction for GPIO 25 (out/in)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [], [])
