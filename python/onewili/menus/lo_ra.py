"""LoRa menu - generated from fwMenuLoRa. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class LoRa(MenuBase):
    r"""LoRa (``w\l``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "lora": {"binary": False, "payload": [("data", "string")], "description": "LoRa RX / status / event line (free-form text)"},
    }

    def configure(self, freq_hz: int, sf: int, bw_enc: int, cr: int, power: int, preamble: int, sync: int) -> Result:
        r"""Configure.

        Wire: ``w\l\c``

        LoRa modem params:
freqHz: carrier Hz, US 902-928M (def 906875000)
sf: spreading factor 6-12 (def 11)
bwEnc: 0=125 1=250 2=500 kHz (def 1)
cr: coding rate 5-8 = 4/5..4/8 (def 5)
power: TX dBm, -9..22 (def 22)
preamble: symbols (def 8)
sync: 1-byte LoRa sync word, hex (def 12; 2b = Meshtastic)
preamble and sync may be omitted; they then take those defaults
only power is range-checked

        https://meshtastic.org/docs/configuration/radio/lora/

        Enter freqHz sf bwEnc cr power [preamble] [sync hex] (bw 0/1/2=125/250/500k; cr 5-8; pwr -9..22)

        Args:
            freq_hz: freq_hz (decS32).
            sf: sf (dec).
            bw_enc: bw_enc (dec).
            cr: cr (dec).
            power: power (decS32).
            preamble: preamble (dec).
            sync: sync (hex8).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(freq_hz), encoding.enc_int(sf), encoding.enc_int(bw_enc), encoding.enc_int(cr), encoding.enc_int(power), encoding.enc_int(preamble), encoding.enc_hex(sync, 2)], [])

    def send_payload(self, data: bytes | bytearray) -> Result:
        r"""Send.

        Wire: ``w\l\s``

        Transmits a LoRa packet

        Enter payload bytes (hex, space-separated)

        Args:
            data: data (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_bytes(data)], [])

    def rx_enable(self, mode: int) -> Result:
        r"""RX Enable.

        Wire: ``w\l\r``

        RX control:
0 = standby (radio idle, no RX, low power)
1 = receive (RX armed; packets print as 'lora' events)
default 1 (RX on); not persisted across reboot

        Enter 0 (standby) or 1 (receive)

        Args:
            mode: mode (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_int(mode)], [])

    def status(self) -> Result:
        r"""Status.

        Wire: ``w\l\t``

        WIO-E5 bridge status (a 'lora' STATUS event):
state: IDLE/RX/TX/SLEEP
chip: raw SX126x status byte
rssi: live channel, dBm
rxPkts/polls/rxBytes/cmds: counters
rxBytes+cmds rising = DISPLAY<->WIO link alive

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def raw_frame(self, cmd: int, payload: bytes | bytearray) -> Result:
        r"""Raw Frame.

        Wire: ``w\l\f``

        Sends a raw framed command to the bridge (advanced)

        Enter cmd (hex8), then payload bytes (hex)

        Args:
            cmd: cmd (hex8).
            payload: payload (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_hex(cmd, 2), encoding.enc_bytes(payload)], [])
