"""Linux Functions menu - generated from fwMenuLinux. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class Linux(MenuBase):
    r"""Linux Functions (``l``)."""

    def enable_linux_cpu(self) -> Result:
        r"""Enable Linux CPU.

        Wire: ``l\a``

        Not yet implemented; always reports failure

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def open_shell(self) -> Result:
        r"""Open Shell.

        Wire: ``l\b``

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [], [])

    def open_shell_session(self, session: int) -> Result:
        r"""Open Shell Session.

        Wire: ``l\c``

        Open a framed Linux shell without entering menu passthrough

        Choose a nonzero session token and reuse it for read, write and close. An active panel or legacy shell refuses this request. Read or write at least every 30 seconds to retain ownership. Uses the FPGA mailbox, including when CM0 USB is in host mode. Requires the fwcm0 bridge daemon.

        Session token (nonzero hex32)

        Args:
            session: session (hex32).

        Returns:
            Result: Ok(session: int) or Err(message).
        """
        return self._call("c", [encoding.enc_hex(session, 8)], ["hex"])

    def close_shell_session(self, session: int) -> Result:
        r"""Close Shell Session.

        Wire: ``l\e``

        Release the framed Linux shell and terminate its session

        Only the matching session token can close the shell. MAIN menu and OneWili sessions stay connected.

        Session token (hex32)

        Args:
            session: session (hex32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_hex(session, 8)], [])

    def write_shell_session(self, session: int, data: str) -> Result:
        r"""Write Shell Session.

        Wire: ``l\w``

        Queue up to 192 shell bytes and return the accepted byte count

        Encode bytes as compact hexadecimal, including CR, Ctrl-C and ANSI keys. Send only the unaccepted suffix after a short write. A failed or timed-out request must not be blindly replayed. Shell data never enters the MAIN command parser.

        Session token and compact hex data

        Args:
            session: session (hex32).
            data: data (string).

        Returns:
            Result: Ok(accepted: int) or Err(message).
        """
        return self._call("w", [encoding.enc_hex(session, 8), encoding.enc_str(data)], ["int"])

    def read_shell_session(self, session: int, maximum: int) -> Result:
        r"""Read Shell Session.

        Wire: ``l\r``

        Read bounded shell output as hexadecimal inside a normal menu response

        Returns count, compact hex data (a dash when empty), and running. Drain final output after running becomes false, then close. Empty reads renew the 30-second lease. Overflow or link reset fails explicitly; reopen before continuing.

        Session token and maximum bytes (1..192)

        Args:
            session: session (hex32).
            maximum: maximum (dec).

        Returns:
            Result: Ok(count: int, data: str, running: bool) or Err(message).
        """
        return self._call("r", [encoding.enc_hex(session, 8), encoding.enc_int(maximum)], ["int", "str", "bool"])

    def cm0_usb_mode(self, mode: str) -> Result:
        r"""CM0 USB Mode.

        Wire: ``l\u``

        Query or switch CM0 USB between PC gadget and USB-A Port 3 host; requires CM0 image support

        Use status to query the live mode and whether dynamic switching is supported. Use gadget for the PC connection or host for a USB device on Port 3 (CN25). Requires a CM0 image implementing USB-mode control; older images return a support/no-reply error and no switch is sent. Switching to host disconnects Gadget Serial. Use the MAIN connection, which remains available. Finish gadget transfers or host USB operations and unmount host storage before switching. The command changes the runtime mode only, not the boot default. Success confirms the applied mode and routing, not peripheral enumeration. Requests are not automatically replayed after a timeout; query status before retrying. Returns mode (gadget, host or passthrough) and switchable (0 or 1).

        Mode: status, gadget or host

        Args:
            mode: mode (string).

        Returns:
            Result: Ok(mode: str, switchable: bool) or Err(message).
        """
        return self._call("u", [encoding.enc_str(mode)], ["str", "bool"])
