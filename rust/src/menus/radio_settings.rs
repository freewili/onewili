//! radio1 menu - generated from fwMenuRadioSettings. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct RadioSettings<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> RadioSettings<'a> {
    /// FrequencyMhz. basic frequency calculated automatically (default = 433.92). The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Wire: `h\s\r\f`
    pub fn frequency_mhz(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\f");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Modulation. set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK. Wire: `h\s\r\m`
    pub fn modulation(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Devation. Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.. Wire: `h\s\r\a`
    pub fn devation(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\a");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Channel. Channelnumber from 0 to 255. Default is channel 0. Wire: `h\s\r\b`
    pub fn channel(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\b");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// ChannelSpacing. channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.. Wire: `h\s\r\c`
    pub fn channel_spacing(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\c");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// RxBandwidth. Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.. Wire: `h\s\r\y`
    pub fn rx_bandwidth(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\y");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// DataRate. Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud. Wire: `h\s\r\e`
    pub fn data_rate(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\e");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PowerAmp. TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max. Wire: `h\s\r\g`
    pub fn power_amp(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\g");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SyncMode. Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync- carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.. Wire: `h\s\r\1`
    pub fn sync_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\1");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// SyncWord. sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low). Wire: `h\s\r\i`
    pub fn sync_word(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\i");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// AddrCheck. Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.. Wire: `h\s\r\j`
    pub fn addr_check(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\j");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Address. Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).. Wire: `h\s\r\k`
    pub fn address(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\k");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// WhiteData. Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.. Wire: `h\s\r\l`
    pub fn white_data(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\r\\l");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PacketFormat. Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.. Wire: `h\s\r\n`
    pub fn packet_format(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\n");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// LengthConfig. 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved. Wire: `h\s\r\o`
    pub fn length_config(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\o");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PacketLength. Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.. Wire: `h\s\r\p`
    pub fn packet_length(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\p");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CRCEnabled. 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.. Wire: `h\s\r\x`
    pub fn c_rc_enabled(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\r\\x");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// CRCAutoFlush. Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.. Wire: `h\s\r\0`
    pub fn c_rc_auto_flush(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\r\\0");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// DCBlockingFilter. Disable digital DC blocking filter before demodulator. Only for data rates <= 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).. Wire: `h\s\r\r`
    pub fn d_c_blocking_filter(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\r\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Manchester. Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.. Wire: `h\s\r\s`
    pub fn manchester(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\r\\s");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// ForwordErrorCorrection. Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.. Wire: `h\s\r\t`
    pub fn forword_error_correction(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\r\\t");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PreambleBytes. Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24. Wire: `h\s\r\u`
    pub fn preamble_bytes(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\u");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// PQT. Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4-PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.. Wire: `h\s\r\v`
    pub fn p_qt(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("h\\s\\r\\v");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// AppendStatus. When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.. Wire: `h\s\r\w`
    pub fn append_status(&mut self) -> Result<(), OwError> {
        let cmd = String::from("h\\s\\r\\w");
        self.t.call(&cmd)?;
        Ok(())
    }
}
