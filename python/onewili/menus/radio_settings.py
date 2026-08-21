"""radio1 menu - generated from fwMenuRadioSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class RadioSettings(MenuBase):
    r"""radio1 (``h\s\r``)."""

    def frequency_mhz(self, value: int) -> Result:
        r"""FrequencyMhz.

        Wire: ``h\s\r\f``

        basic frequency calculated automatically (default = 433.92). The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(value)], [])

    def modulation(self, value: int) -> Result:
        r"""Modulation.

        Wire: ``h\s\r\m``

        set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def devation(self, value: int) -> Result:
        r"""Devation.

        Wire: ``h\s\r\a``

        Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(value)], [])

    def channel(self, value: int) -> Result:
        r"""Channel.

        Wire: ``h\s\r\b``

        Channelnumber from 0 to 255. Default is channel 0

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(value)], [])

    def channel_spacing(self, value: int) -> Result:
        r"""ChannelSpacing.

        Wire: ``h\s\r\c``

        channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(value)], [])

    def rx_bandwidth(self, value: int) -> Result:
        r"""RxBandwidth.

        Wire: ``h\s\r\y``

        Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [encoding.enc_int(value)], [])

    def data_rate(self, value: int) -> Result:
        r"""DataRate.

        Wire: ``h\s\r\e``

        Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(value)], [])

    def power_amp(self, value: int) -> Result:
        r"""PowerAmp.

        Wire: ``h\s\r\g``

        TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_int(value)], [])

    def sync_mode(self, value: int) -> Result:
        r"""SyncMode.

        Wire: ``h\s\r\1``

        Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync- carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("1", [encoding.enc_int(value)], [])

    def sync_word(self, value: int) -> Result:
        r"""SyncWord.

        Wire: ``h\s\r\i``

        sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low)

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_int(value)], [])

    def addr_check(self, value: int) -> Result:
        r"""AddrCheck.

        Wire: ``h\s\r\j``

        Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_int(value)], [])

    def address(self, value: int) -> Result:
        r"""Address.

        Wire: ``h\s\r\k``

        Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(value)], [])

    def white_data(self) -> Result:
        r"""WhiteData.

        Wire: ``h\s\r\l``

        Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [], [])

    def packet_format(self, value: int) -> Result:
        r"""PacketFormat.

        Wire: ``h\s\r\n``

        Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_int(value)], [])

    def length_config(self, value: int) -> Result:
        r"""LengthConfig.

        Wire: ``h\s\r\o``

        0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(value)], [])

    def packet_length(self, value: int) -> Result:
        r"""PacketLength.

        Wire: ``h\s\r\p``

        Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(value)], [])

    def c_rc_enabled(self) -> Result:
        r"""CRCEnabled.

        Wire: ``h\s\r\x``

        1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("x", [], [])

    def c_rc_auto_flush(self) -> Result:
        r"""CRCAutoFlush.

        Wire: ``h\s\r\0``

        Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("0", [], [])

    def d_c_blocking_filter(self) -> Result:
        r"""DCBlockingFilter.

        Wire: ``h\s\r\r``

        Disable digital DC blocking filter before demodulator. Only for data rates <= 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def manchester(self) -> Result:
        r"""Manchester.

        Wire: ``h\s\r\s``

        Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def forword_error_correction(self) -> Result:
        r"""ForwordErrorCorrection.

        Wire: ``h\s\r\t``

        Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def preamble_bytes(self, value: int) -> Result:
        r"""PreambleBytes.

        Wire: ``h\s\r\u``

        Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(value)], [])

    def p_qt(self, value: int) -> Result:
        r"""PQT.

        Wire: ``h\s\r\v``

        Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4-PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(value)], [])

    def append_status(self) -> Result:
        r"""AppendStatus.

        Wire: ``h\s\r\w``

        When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [], [])
