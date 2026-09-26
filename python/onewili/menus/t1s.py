"""Ethernet 10BaseT1S menu - generated from fwMenuT1S. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .eth_test import EthTest
from .plca import PLCA
from .t1s_tc10 import T1STc10


class T1S(MenuBase):
    r"""Ethernet 10BaseT1S (``i\r``)."""

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.eth_test = EthTest(transport, nav_path + "\\t")
        self.plca = PLCA(transport, nav_path + "\\p")
        self.tc10 = T1STc10(transport, nav_path + "\\w")

    def t1s_status(self) -> Result:
        r"""Status.

        Wire: ``i\r\s``

        Prints one line of key=value T1S engine status: state link plca plcaen id cnt to chipRev t1sTx t1sTxDrop t1sRx t1sRxDrop spiAbort errs evts faults lastErr lastEvt ... term tc10 wkgen wksrc. plca=1 means the PLCA cycle is locked; chipRev is 0 until the PHY initialized; tc10 is 0 awake / 1 sleep pending / 2 sleeping, wkgen counts TC10 wake generations requested, wksrc is the last wake source (bit1 MDI, bit0 WAKE_IN). Wire-parseable, append-only

        Returns:
            Result: Ok(status: str) or Err(message).
        """
        return self._call("s", [], ["str"])

    def t1s_link_status(self) -> Result:
        r"""Link Status.

        Wire: ``i\r\k``

        Reports the T1S link (up while the PHY is initialized and running), the engine state name and whether the NCM<->T1S bridge is on

        Returns:
            Result: Ok(info: str) or Err(message).
        """
        return self._call("k", [], ["str"])

    def t1s_reinit_phy(self) -> Result:
        r"""Reinit PHY.

        Wire: ``i\r\i``

        Requests a full PHY reinit: RST pulse plus fresh TC6 init with the current PLCA settings (this is how Burst Max/Burst Timer changes take effect). Also enables the T1S engine and clears the FAULT retry budget; bring-up itself still waits for the IO-header rail (zone 6)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def t1s_clear_counters(self) -> Result:
        r"""Clear Counters.

        Wire: ``i\r\c``

        Zeros the T1S TX/RX/drop/error counters (chip revision is kept). The engine state, link and bridge are unaffected

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def t1s_register_read(self, mms: int, address: int) -> Result:
        r"""Register Read.

        Wire: ``i\r\g``

        Reads one 32-bit register from the LAN865x over the TC6 SPI protocol: MMS is the memory map selector (0..15), Address the 16-bit register address within it. Requires the PHY to be initialized and running

        Decimal MMS, Hex Register Address (e.g. 4 CA03)

        Args:
            mms: mms (dec).
            address: address (hex16).

        Returns:
            Result: Ok(value: int) or Err(message).
        """
        return self._call("g", [encoding.enc_int(mms), encoding.enc_hex(address, 4)], ["hex"])

    def bridge(self) -> Result:
        r"""Bridge.

        Wire: ``i\r\b``

        When on, host NCM frames forward to the T1S wire and T1S frames forward to the host (the local classifier/responder/loopback step aside) and the host adapter's link mirrors the T1S link. Always off after a reboot

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [], [])
