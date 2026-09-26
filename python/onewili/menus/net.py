"""Network (TCP/IP) menu - generated from fwMenuNet. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class Net(MenuBase):
    r"""Network (TCP/IP) (``i\w``)."""

    def net_status(self) -> Result:
        r"""Status.

        Wire: ``i\w\s``

        Prints one line of key=value TCP/IP stack status: lwip host up ncm_link ncm_mode ncm_ip ncm_mask ncm_gw ncm_mac ncm_dhcp t1s_link t1s_ip t1s_mask t1s_mac ncm_rx ncm_tx ncm_rxdrop ncm_txdrop t1s_rx t1s_tx t1s_rxdrop t1s_txdrop rx_nomem tcp_pcbs udp_pcbs tcp_echo udp_echo http mem_used mem_max pbuf_used pbuf_max bridge tcp_echo_bytes udp_sink echo_on http_on t1s_rxfilt. New keys are only ever APPENDED (host parsers key on names, never positions). lwip=0 means the stack is not compiled in; ncm_mode is static or dhcp, ncm_dhcp is off/init/discover/request/bound/renew/rebind/backoff/autoip/autoip-probe. Wire-parseable, append-only

        Returns:
            Result: Ok(status: str) or Err(message).
        """
        return self._call("s", [], ["str"])

    def net_link_status(self) -> Result:
        r"""Link Status.

        Wire: ``i\w\l``

        Reports the USB network adapter (NCM) link as lwIP sees it, the addressing mode, the current IP, the DHCP hostname and whether the NCM<->T1S bridge (which takes lwIP off both wires) is on

        Returns:
            Result: Ok(info: str) or Err(message).
        """
        return self._call("l", [], ["str"])

    def net_dhcp_renew(self) -> Result:
        r"""DHCP Renew.

        Wire: ``i\w\r``

        Asks the DHCP client on the USB network adapter to renew its lease now. Fails when NCM Mode is static or DHCP is not running

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def net_ping(self, ip: str, count: int) -> Result:
        r"""Ping.

        Wire: ``i\w\p``

        ICMP echo client: sends count (1..5, default 3) echo requests to the dotted-decimal IPv4 address one at a time with a 1 s timeout each and prints seq=N rtt_ms=X or seq=N timeout per request, then sent= recv= min/avg/max. Blocks the console for up to count seconds; the stack keeps being serviced meanwhile

        IPv4 address, then count 1..5 (e.g. 10.55.0.1 3)

        Args:
            ip: ip (string).
            count: count (dec).

        Returns:
            Result: Ok(result: str) or Err(message).
        """
        return self._call("p", [encoding.enc_str(ip), encoding.enc_int(count)], ["str"])

    def net_clear_counters(self) -> Result:
        r"""Clear Counters.

        Wire: ``i\w\c``

        Zeros the net rx/tx/drop counters, ring high-water marks, service counters and the lwIP memory max/error marks. Addresses, links and services are unaffected

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def net_apply(self) -> Result:
        r"""Apply.

        Wire: ``i\w\a``

        Re-pushes the settings below into the stack (every setting change already applies live; this is for scripts and after a rejected address). Fails when an address does not parse -- the previous configuration stays live and the settings are resynced to it

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def n_cm_mode(self, value: int) -> Result:
        r"""NCM Mode.

        Wire: ``i\w\m``

        Addressing mode of the USB network adapter: static uses NCM IP/Netmask/Gateway; dhcp runs the DHCP client (hostname freewili-xxxx) and falls back to an AutoIP 169.254.x.x address after 3 unanswered discovers. Applies live and persists

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def n_cmip(self, value: str) -> Result:
        r"""NCM IP.

        Wire: ``i\w\i``

        Static IPv4 address of the USB network adapter (dotted decimal, default 10.55.0.2 -- the host tests expect this). Ignored while NCM Mode is dhcp. Rejected (previous kept) if it does not parse

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_str(value)], [])

    def n_cm_netmask(self, value: str) -> Result:
        r"""NCM Netmask.

        Wire: ``i\w\k``

        Static netmask of the USB network adapter (dotted decimal, default 255.255.255.0). Ignored while NCM Mode is dhcp

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_str(value)], [])

    def n_cm_gateway(self, value: str) -> Result:
        r"""NCM Gateway.

        Wire: ``i\w\g``

        Static default gateway on the USB network adapter (dotted decimal, default 10.55.0.1 = the host). Ignored while NCM Mode is dhcp

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_str(value)], [])

    def t1sip(self, value: str) -> Result:
        r"""T1S IP.

        Wire: ``i\w\j``

        Static IPv4 address of the 10BASE-T1S port's own lwIP netif (dotted decimal, default 10.56.0.2; static only, no gateway, never the default route)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_str(value)], [])

    def t1s_netmask(self, value: str) -> Result:
        r"""T1S Netmask.

        Wire: ``i\w\u``

        Static netmask of the 10BASE-T1S port's own netif (dotted decimal, default 255.255.255.0)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_str(value)], [])

    def echo_servers(self) -> Result:
        r"""Echo Servers.

        Wire: ``i\w\e``

        When on (default), the device runs the UDP echo server on port 5556, the UDP 5555 sink (swallows stray FWET test datagrams) and the TCP echo server on port 7 on every netif. Turning it off aborts live echo connections

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def h_ttp_server(self) -> Result:
        r"""HTTP Server.

        Wire: ``i\w\t``

        When on (default), the device serves an HTML status page on port 80 (GET /) and the plain-text Status line (GET /status)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])
