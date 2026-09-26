"""Ethernet Test menu - generated from fwMenuEthTest. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class EthTest(MenuBase):
    r"""Ethernet Test (``i\r\t``)."""

    def eth_test_start_periodic(self) -> Result:
        r"""Start Periodic.

        Wire: ``i\r\t\p``

        Starts the test-frame generator sending one frame every Period us (see setting u). Frames use the current Frame Size/Type/CRC settings and the destination MAC from command m. Refused while Loopback is on or on a build without the NCM stack

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [], [])

    def eth_test_start_flood(self) -> Result:
        r"""Start Flood.

        Wire: ``i\r\t\f``

        Starts the test-frame generator sending as fast as the USB link accepts (natural NTB backpressure paces it; submit failures are counted, not lost sequence numbers). Refused while Loopback is on

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [], [])

    def eth_test_start_line_rate(self) -> Result:
        r"""Start Line Rate.

        Wire: ``i\r\t\r``

        Starts the test-frame generator at Line Rate % (setting e) of a 10 Mbit/s reference wire, using a token bucket that charges each frame its size plus 24 bytes of preamble/FCS/gap overhead. Refused while Loopback is on

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def eth_test_start_burst(self) -> Result:
        r"""Start Burst.

        Wire: ``i\r\t\b``

        Starts the test-frame generator releasing Burst Count frames (setting n) every second, the first burst immediately. Refused while Loopback is on

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [], [])

    def eth_test_send_count(self, count: int) -> Result:
        r"""Send N Frames.

        Wire: ``i\r\t\o``

        Sends exactly Count test frames as fast as the link accepts, then stops by itself (Count 1 = one transmit). Counters keep running so the result can be read with Show Stats afterwards. Refused while Loopback is on

        Decimal: Frame Count (1..1000000)

        Args:
            count: count (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(count)], [])

    def eth_test_stop(self) -> Result:
        r"""Stop.

        Wire: ``i\r\t\x``

        Stops the test-frame generator. Counters are kept (use Clear Stats to zero them); the responder and loopback settings are unaffected

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [], [])

    def eth_test_show_stats(self) -> Result:
        r"""Show Stats.

        Wire: ``i\r\t\s``

        Prints one line of key=value counters: mode link TXf TXb TXfail TXfps TXkbps RXf RXb RXfps RXkbps gap lost crc under over other echoq echos echod. The fps/kbps values are 1 Hz rates; RXf counts received FWET test frames, other counts everything else (host OS chatter)

        Returns:
            Result: Ok(stats: str) or Err(message).
        """
        return self._call("s", [], ["str"])

    def eth_test_clear_stats(self) -> Result:
        r"""Clear Stats.

        Wire: ``i\r\t\c``

        Zeros every TX/RX/echo counter and restarts sequence-gap tracking. The generator, responder and link state are unaffected

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [], [])

    def eth_test_set_dest_mac(self, dest_mac: bytes | bytearray) -> Result:
        r"""Set Dest MAC.

        Wire: ``i\r\t\m``

        Sets the destination MAC for generated test frames (default FF FF FF FF FF FF broadcast). Takes effect at the next generator start. Not persisted across reboot

        Hex: 6 Destination MAC Bytes Separated by Spaces

        Args:
            dest_mac: dest_mac (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_bytes(dest_mac)], [])

    def eth_test_link_status(self) -> Result:
        r"""Link Status.

        Wire: ``i\r\t\k``

        Reports whether the USB network adapter is up (host selected the NCM data interface) plus the host-side MAC, device-side MAC and the device's static IP 10.55.0.2

        Returns:
            Result: Ok(info: str) or Err(message).
        """
        return self._call("k", [], ["str"])

    def frame_size(self, value: int) -> Result:
        r"""Frame Size.

        Wire: ``i\r\t\i``

        Total Ethernet frame size in bytes for generated test frames (headers included, FCS excluded). The udp frame type needs at least 66 bytes for its headers and is raised to that silently

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_int(value)], [])

    def period_us(self, value: int) -> Result:
        r"""Period us.

        Wire: ``i\r\t\u``

        Microseconds between frames in Periodic mode (10000 = 100 frames per second)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(value)], [])

    def burst_count(self, value: int) -> Result:
        r"""Burst Count.

        Wire: ``i\r\t\n``

        Frames released in each one-second burst in Burst mode

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_int(value)], [])

    def line_rate_percent(self, value: int) -> Result:
        r"""Line Rate Percent.

        Wire: ``i\r\t\e``

        Percentage of the 10 Mbit/s reference wire rate for Line Rate mode

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(value)], [])

    def payload_crc(self) -> Result:
        r"""Payload CRC.

        Wire: ``i\r\t\v``

        When on, each generated frame carries a CRC32 over its sequence/timestamp/fill so the host can prove payload integrity; costs a CRC pass per frame at high rates

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [], [])

    def responder(self) -> Result:
        r"""Responder.

        Wire: ``i\r\t\a``

        When on, the device answers as 10.55.0.2: ARP requests, ICMP echo (ping) and UDP echo on port 5556. Turn off to measure pure generator/counter behavior. (Served by lwIP when compiled in -- FW2MAIN_LWIP builds answer through the Network (TCP/IP) menu's stack and this toggle only drives the legacy mini-responder on non-lwIP builds)

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [], [])

    def loopback(self) -> Result:
        r"""Loopback.

        Wire: ``i\r\t\l``

        When on, EVERY received frame is echoed back with its MAC addresses swapped and the generator/responder are disabled (mutually exclusive). Always off after a reboot

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [], [])

    def frame_type(self, value: int) -> Result:
        r"""Frame Type.

        Wire: ``i\r\t\t``

        Carrier for generated test frames: raw = ethertype 0x88B5 (needs npcap/scapy on the host), udp = IPv4 broadcast 10.55.0.255 port 5555 (a plain host socket receives it)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(value)], [])
