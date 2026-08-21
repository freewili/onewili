"""Radio menu - generated from fwMenuRadio. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class Radio(MenuBase):
    r"""Radio (``w\r``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "radio1": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "Radio 1 received data (hex bytes)"},
        "radio2": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "Radio 2 received data (hex bytes)"},
        "radioasync": {"binary": False, "payload": [("data", "string")], "description": "Async sub-file transmit/capture status (free-form text)"},
    }

    def select_circuit(self, band: int) -> Result:
        r"""Select Circuit.

        Wire: ``w\r\s``

        Claims the sub-GHz front end for this client and holds it until Release.
The CC1101 and the WIO-E5 LoRa module are mutually exclusive in hardware:
one net drives both the antenna switches and the CC1101's chip-select
demux, so claiming the circuit disconnects the LoRa bridge until released.
band: 0 keep whatever is selected (defaults high), 1 low, 2 mid, 3 high.
A hold taken here never expires, which is what makes a multi-step script
safe; a command issued without one takes an implicit hold that DOES expire
after 20 s idle. Refused while the on-screen SubGHz app holds the circuit.

        Enter band (0 keep, 1 low, 2 mid, 3 high)

        Args:
            band: band (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(band)], [])

    def release_circuit(self) -> Result:
        r"""Release Circuit.

        Wire: ``w\r\e``

        Ends this client's circuit hold and hands the antenna back to LoRa.
Fails when no explicit hold is in force, so 'released' and 'was not mine'
stay distinguishable. Needs no hold of its own, so a session left behind by
a script that died is always recoverable from any client.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def read_state(self) -> Result:
        r"""State.

        Wire: ``w\r\t``

        Reads the mux and radio state back, in this order:
owner 0=LoRa 1=CC1101; holder 0=none 1=panel 2=command 3=session;
band 0=none 1=low 2=mid 3=high; wantV1/wantV2 the expander bits asked for;
haveValid/haveV1/haveV2 the same two bits read back out of the expander's
input port, so they are evidence about the pads rather than an echo;
loraPaused 1 = the LoRa PIO UART is stopped; freqHz the frequency the chip is
currently tuned to, which follows a .sub transmit as well as an explicit tune;
active 1 = something is running that outlived its request;
status 0=idle 2=ok 3=fail for the last operation; version the last
comm-check register read (0x14 is a live CC1101, FF means no comm-check has
run yet, and 00 means one ran and the part answered nothing).

        Returns:
            Result: Ok(owner: int, holder: int, band: int, want_v1: int, want_v2: int, have_valid: int, have_v1: int, have_v2: int, lora_paused: int, freq_hz: int, active: int, status: int, version: int) or Err(message).
        """
        return self._call("t", [], ["int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "int", "hex"])

    def select_band(self, band: int) -> Result:
        r"""Band.

        Wire: ``w\r\b``

        Forces the matched antenna path now: 1 low, 2 mid, 3 high.
The next tune re-derives the path from the frequency, so this is an
override for measurement rather than a persistent setting. Read it back
with State (band, and haveV1/haveV2 for the pads).

        Enter band (1 low, 2 mid, 3 high)

        Args:
            band: band (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(band)], [])

    def comm_check(self) -> Result:
        r"""Comm Check.

        Wire: ``w\r\c``

        Reads the CC1101 version register and returns it.
0x14 is a healthy part; anything else means the SPI answered with the wrong
value, and no answer at all fails the command. Not a link check despite the
name -- it is an identity probe on the chip itself.

        Returns:
            Result: Ok(version: int) or Err(message).
        """
        return self._call("c", [], ["hex"])

    def set_frequency(self, freq_hz: int) -> Result:
        r"""Frequency.

        Wire: ``w\r\f``

        Tunes the CC1101 and selects the matched antenna path for that band.
Returns the path the frequency selected (0 none, 1 low, 2 mid, 3 high), not
the frequency. Matched bands are 300-348, 387-464 and 779-928 MHz; outside
them the tune fails rather than transmitting into a mismatch.

        Enter frequency in Hz

        Args:
            freq_hz: freq_hz (decU32).

        Returns:
            Result: Ok(band: int) or Err(message).
        """
        return self._call("f", [encoding.enc_int(freq_hz)], ["int"])

    def read_rssi(self) -> Result:
        r"""RSSI.

        Wire: ``w\r\i``

        Samples received signal strength once, in dBm.
A one-shot reading, not a subscription: the radio re-applies the current
frequency, switches to receive and reads once. Poll it for a trace, or use
Monitor to have the radio keep the receiver open between reads.

        Returns:
            Result: Ok(rssi: int) or Err(message).
        """
        return self._call("i", [], ["int"])

    def carrier(self, on: int) -> Result:
        r"""Carrier.

        Wire: ``w\r\o``

        Keys or unkeys an unmodulated carrier at the current frequency.
Keying re-applies the frequency as a side effect. A keyed carrier counts as
the radio being active, so an implicit circuit hold will not expire
underneath it while you read it on an SDR.

        Enter 0 (off) or 1 (on)

        Args:
            on: on (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(on)], [])

    def rx_enable(self, on: int) -> Result:
        r"""RX Enable.

        Wire: ``w\r\r``

        Puts the CC1101 into continuous receive, or back to idle.
This only opens the receiver; nothing is streamed back by it. For packet
traffic use Packet RX, for signal strength use RSSI or Monitor, and for raw
pulse timing use Capture Start.

        Enter 0 (idle) or 1 (receive)

        Args:
            on: on (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(on)], [])

    def idle(self) -> Result:
        r"""Idle.

        Wire: ``w\r\w``

        Returns the CC1101 to idle from receive, transmit or carrier.
Does not hand the circuit back to LoRa; Release does that.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [], [])

    def packet_send(self, freq_hz: int, data: bytes | bytearray) -> Result:
        r"""Packet Send.

        Wire: ``w\r\x``

        Transmits one GFSK packet through the CC1101 packet engine.
The modem settings match a Flipper Zero in 'subghz chat' (GFSK 9.99 kb/s,
sync 0x464C, CRC on), so a Flipper in that mode receives it. Payload is
capped at 60 bytes by the chip's FIFO and a longer one is refused rather
than sent short. A chat peer expects a trailing 0x0A.

        Enter freqHz then payload bytes (hex, space-separated)

        Args:
            freq_hz: freq_hz (decU32).
            data: data (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [encoding.enc_int(freq_hz), encoding.enc_bytes(data)], [])

    def packet_rx(self, on: int, freq_hz: int) -> Result:
        r"""Packet RX.

        Wire: ``w\r\y``

        Opens or closes the GFSK packet receiver at the given frequency.
While it is open each received packet is reported as a 'radio' event (RSSI,
length and payload bytes) without being polled for, and the last one is
also readable with Packet Read. freqHz is ignored when turning it off.

        Enter 0 (off) or 1 (on), then freqHz

        Args:
            on: on (dec).
            freq_hz: freq_hz (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [encoding.enc_int(on), encoding.enc_int(freq_hz)], [])

    def packet_read(self) -> Result:
        r"""Packet Read.

        Wire: ``w\r\k``

        Reads the last received GFSK packet: RSSI in dBm, a sequence counter that
bumps once per packet, then the payload bytes. Compare the sequence against
the previous read to tell a new packet from a repeat. The payload is
reported up to 60 bytes, so a longer packet is truncated here but its real
length is still reported by the radio.

        Returns:
            Result: Ok(rssi: int, seq: int, data: bytes | bytearray) or Err(message).
        """
        return self._call("k", [], ["int", "int", "bytes"])

    def capture_start(self, freq_hz: int, preset: int) -> Result:
        r"""Capture Start.

        Wire: ``w\r\g``

        Arms a raw pulse-duration capture at the given frequency.
Squelch-triggered: it waits for signal strength to open, records edge
durations until it closes or the buffer fills, then holds the result for
Replay. Set the trigger level with Squelch. A frequency outside the matched
bands (300-348, 387-464, 779-928 MHz) is rejected here rather than captured
as noise. Watch progress in State.

        Enter freqHz then preset (0 OOK270, 1 OOK650, 2 FSK238, 3 FSK476, 4 custom)

        Args:
            freq_hz: freq_hz (decU32).
            preset: preset (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_int(freq_hz), encoding.enc_int(preset)], [])

    def capture_stop(self) -> Result:
        r"""Capture Stop.

        Wire: ``w\r\j``

        Ends a capture and returns how many pulse durations it recorded.
Zero means the squelch never opened, which usually means the level is set
below the receiver's noise floor or there was nothing to hear.

        Returns:
            Result: Ok(durations: int) or Err(message).
        """
        return self._call("j", [], ["int"])

    def replay(self) -> Result:
        r"""Replay.

        Wire: ``w\r\p``

        Re-transmits the last capture out the transmit path.
Fails when nothing has been captured. Acks as soon as the burst is armed
because the transmit itself runs on the display's PIO and DMA; watch the
active flag in State to see it finish. Flip the pulse phase first with
Replay Invert if the recovered signal is inverted.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [], [])

    def analyzer(self, on: int) -> Result:
        r"""Analyzer.

        Wire: ``w\r\a``

        Starts or stops the background frequency-analyzer sweep.
One frequency is measured per service tick so nothing else stalls, and the
bins plus the running peak are readable with Spectrum. The sweep counts as
the radio being active, so an implicit circuit hold will not expire while it
runs.

        Enter 0 (stop) or 1 (start)

        Args:
            on: on (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(on)], [])

    def spectrum(self) -> Result:
        r"""Spectrum.

        Wire: ``w\r\n``

        Reads the analyzer's results: the peak frequency in Hz, its level in dBm,
then one signed byte of dBm per sweep bin, lowest frequency first. Values
are two's-complement, so 0xC0 is -64 dBm. Empty until the sweep has been
started with Analyzer.

        Returns:
            Result: Ok(peak_freq_hz: int, peak_rssi: int, bins: bytes | bytearray) or Err(message).
        """
        return self._call("n", [], ["int", "int", "bytes"])

    def squelch(self, dbm: int) -> Result:
        r"""Squelch.

        Wire: ``w\r\u``

        Sets the level a capture must see before it starts recording, and below
which it stops. Default -65. Set it between the signal you want (a nearby
remote reads around -10) and the receiver's noise floor, which rides higher
than -75 on repeat captures because the gain control has wound up.

        Enter the capture trigger level in dBm (-120 to 0)

        Args:
            dbm: dbm (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(dbm)], [])

    def replay_invert(self, on: int) -> Result:
        r"""Replay Invert.

        Wire: ``w\r\v``

        Flips the captured low/high phase before re-keying it on Replay.
Use it when a replay does not reproduce the original and the capture looks
phase-inverted. Default off.

        Enter 0 (as captured) or 1 (inverted)

        Args:
            on: on (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(on)], [])

    def transmit_sub_file(self, path: str) -> Result:
        r"""Transmit Sub.

        Wire: ``w\r\m``

        Transmits a Flipper .sub file from the card.
A bare name is resolved under the radio directory and .sub is appended when
the name has no extension. This processor parses the file and streams the
decoded durations to the display, so the ack means the burst was armed, not
that it finished; a 'radioasync' event reports completion. The display holds
4096 durations and a longer file is refused rather than sent truncated.

        Enter the sub file name

        Args:
            path: path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_str(path)], [])

    def monitor(self, on: int) -> Result:
        r"""Monitor.

        Wire: ``w\r\l``

        Keeps the receiver open and samples signal strength continuously, so
repeated RSSI reads reflect a live channel instead of re-entering receive
each time. Read the value with RSSI or in State. Counts as the radio being
active, so an implicit circuit hold will not expire while it runs.

        Enter 0 (off) or 1 (on)

        Args:
            on: on (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_int(on)], [])
