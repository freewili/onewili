"""TC10 Wake/Sleep menu - generated from fwMenuT1STc10. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class T1STc10(MenuBase):
    r"""TC10 Wake/Sleep (``i\r\w``)."""

    def t1s_tc10_generate_wake(self) -> Result:
        r"""Generate Wake.

        Wire: ``i\r\w\g``

        Emits a TC10 wake-up from the running LAN865x: a 1 ms DME wake burst onto the MDI (Forward to MDI) and/or a 90 us pulse on the WAKE_OUT pin (Forward to WAKE_OUT), per the settings below. The engine polls the PHY until the request completes (see Wake Status gen/done/timeout). Fails when the PHY is not in run, neither forward target is enabled, or a previous wake is still busy

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [], [])

    def t1s_tc10_wake_status(self) -> Result:
        r"""Wake Status.

        Wire: ``i\r\w\s``

        Prints one line of key=value TC10 status: state (engine state, sleep while asleep) gen done busy timeout (wake generations requested/completed/in flight/expired) sleeps woke pulses (sleep entries, wake detections, local WAKE_IN pulses) src (last wake source: none/mdi/wakein/mdi+wakein) sts2 (raw PHY STS2) fwd wake (forward targets and wake sources as configured) inhdly (INH release delay code) sleepms (ms asleep, 0 when awake). Wire-parseable, append-only

        Returns:
            Result: Ok(status: str) or Err(message).
        """
        return self._call("s", [], ["str"])

    def t1s_tc10_enter_sleep(self) -> Result:
        r"""Enter Sleep.

        Wire: ``i\r\w\e``

        Puts the LAN865x into TC10 sleep with the configured wake sources (Wake on MDI / Wake on WAKE_IN) and forward targets. On Orca the PHY's INH output then cuts its own SPI/IRQ path, so the engine tears the link down after a short grace and parks in the sleep state until the chip wakes (MDI energy, a WAKE_IN pulse, Local Wake Pulse) or Cancel Sleep reinits it. Fails when the PHY is not in run or a sleep is already pending

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def t1s_tc10_local_wake_pulse(self) -> Result:
        r"""Local Wake Pulse.

        Wire: ``i\r\w\w``

        Drives a 200 us HIGH pulse on the PHY's WAKE_IN pin from the board's IO expander (which stays powered while the PHY sleeps). Only a sleeping PHY reacts (it wakes and the engine reinitializes it); harmless when awake. Fails if the expander is not configured or the I2C write fails

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [], [])

    def t1s_tc10_cancel_sleep(self) -> Result:
        r"""Cancel Sleep.

        Wire: ``i\r\w\c``

        Abandons a pending sleep or leaves the sleep state by requesting a full PHY reinit (RST pulse + fresh init). Reports 'not sleeping' when no sleep is in progress

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def forward_to_mdi(self) -> Result:
        r"""Forward to MDI.

        Wire: ``i\r\w\m``

        When on, Generate Wake (and a wake forwarded during sleep) puts a 1 ms wake burst onto the MDI so the far end of the T1S segment wakes. At least one of Forward to MDI / Forward to WAKE_OUT must be on for Generate Wake to do anything

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [], [])

    def forward_to_wakeout(self) -> Result:
        r"""Forward to WAKE_OUT.

        Wire: ``i\r\w\o``

        When on, Generate Wake (and a wake forwarded during sleep) emits a 90 us pulse on the PHY's WAKE_OUT pin (routed to the header on Orca; the host cannot observe it). At least one of Forward to MDI / Forward to WAKE_OUT must be on for Generate Wake to do anything

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [], [])

    def wake_on_mdi(self) -> Result:
        r"""Wake on MDI.

        Wire: ``i\r\w\a``

        When on, a sleeping PHY wakes on energy detected on the MDI (any activity, not only a TC10 wake burst). Applied at the next Enter Sleep

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def wake_on_wakein(self) -> Result:
        r"""Wake on WAKE_IN.

        Wire: ``i\r\w\n``

        When on, a sleeping PHY wakes on a HIGH pulse longer than 40 us on its WAKE_IN pin (Local Wake Pulse drives that pin from the IO expander). Applied at the next Enter Sleep

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [], [])

    def sleep_inhibit_delay(self, value: int) -> Result:
        r"""Sleep Inhibit Delay.

        Wire: ``i\r\w\y``

        Delay before the PHY releases its INH output after entering sleep: 0 = 0 ms, 1 = 50 ms, 2 = 100 ms, 3 = 200 ms. On Orca INH powers the PHY's SPI/IRQ path, so this is how long the link stays reachable after Enter Sleep. Applied at the next Enter Sleep

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [encoding.enc_int(value)], [])
