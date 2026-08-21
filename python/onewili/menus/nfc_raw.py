"""Raw Transceiver menu - generated from fwMenuNFCRaw. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class NFCRaw(MenuBase):
    r"""Raw Transceiver (``w\n\k``)."""

    def begin(self) -> Result:
        r"""Begin.

        Wire: ``w\n\k\b``

        Initialize the ST25R3916 and take ownership of the NFC front-end

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [], [])

    def end(self) -> Result:
        r"""End.

        Wire: ``w\n\k\e``

        Release the ST25R3916 back to the normal reader/writer state machine

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def field(self, on: int) -> Result:
        r"""Field.

        Wire: ``w\n\k\f``

        Turn the RF field on or off

        Enter 1 to turn the RF field on, 0 to turn it off

        Args:
            on: on (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(on)], [])

    def reg_write(self, addr: int, value: int) -> Result:
        r"""Write Register.

        Wire: ``w\n\k\w``

        Write a single ST25R3916 register

        Enter register address and value (hex)

        Args:
            addr: addr (hex).
            value: value (hex).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_hex(addr), encoding.enc_hex(value)], [])

    def reg_read(self, addr: int) -> Result:
        r"""Read Register.

        Wire: ``w\n\k\r``

        Read a single ST25R3916 register

        Enter register address (hex)

        Args:
            addr: addr (hex).

        Returns:
            Result: Ok(value: int) or Err(message).
        """
        return self._call("r", [encoding.enc_hex(addr)], ["hex"])

    def cmd(self, command: int) -> Result:
        r"""Send Command.

        Wire: ``w\n\k\c``

        Send a direct command to the ST25R3916

        Enter direct command code (hex)

        Args:
            command: command (hex).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_hex(command)], [])

    def transceive(self, flags: int, timeout_ms: int, tx: bytes | bytearray) -> Result:
        r"""Transceive.

        Wire: ``w\n\k\t``

        Transmit bytes and receive the response over the RF field

        Enter flags (hex), timeout in ms, then tx bytes (hex)

        Args:
            flags: flags (hex).
            timeout_ms: timeout_ms (dec).
            tx: tx (hexbytes).

        Returns:
            Result: Ok(status: int, rx: bytes | bytearray) or Err(message).
        """
        return self._call("t", [encoding.enc_hex(flags), encoding.enc_int(timeout_ms), encoding.enc_bytes(tx)], ["hex", "bytes"])
