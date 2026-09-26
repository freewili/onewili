"""PLCA Settings menu - generated from fwMenuPLCA. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class PLCA(MenuBase):
    r"""PLCA Settings (``i\r\p``)."""

    def p_lca_enabled(self) -> Result:
        r"""PLCAEnabled.

        Wire: ``i\r\p\a``

        When on, the PHY runs PLCA (collision-free round-robin transmit opportunities; the node with Local ID 0 coordinates the cycle). When off, the PHY falls back to CSMA/CD. Applied live to a running PHY

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def local_id(self, value: int) -> Result:
        r"""Local ID.

        Wire: ``i\r\p\l``

        This node's PLCA ID (0..254). ID 0 is the cycle coordinator -- exactly one node on the segment must be 0. Applied live to a running PHY

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_int(value)], [])

    def node_count(self, value: int) -> Result:
        r"""Node Count.

        Wire: ``i\r\p\n``

        Number of transmit opportunities in each PLCA cycle (1..255); only meaningful on the coordinator (Local ID 0). Applied live to a running PHY

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_int(value)], [])

    def t_o_timer(self, value: int) -> Result:
        r"""TO Timer.

        Wire: ``i\r\p\t``

        PLCA transmit-opportunity timer in bit times (1..255, silicon default 32). Written directly to the PHY's PLCA_TOTMR register when it differs from 32. Applied live to a running PHY

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(value)], [])

    def burst_max(self, value: int) -> Result:
        r"""Burst Max.

        Wire: ``i\r\p\m``

        Maximum extra packets this node may send in one transmit opportunity (0..255, 0 = burst off). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def burst_timer(self, value: int) -> Result:
        r"""Burst Timer.

        Wire: ``i\r\p\b``

        Idle time in bit times the PHY waits between burst packets before giving up the transmit opportunity (1..255). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])
