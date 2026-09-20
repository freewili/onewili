//! Ethernet Test menu - generated from fwMenuEthTest. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct EthTest<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> EthTest<'a> {
    /// Start Periodic. Starts the test-frame generator sending one frame every Period us (see setting u). Frames use the current Frame Size/Type/CRC settings and the destination MAC from command m. Refused while Loopback is on or on a build without the NCM stack. Wire: `i\t\p`
    pub fn eth_test_start_periodic(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(554 /* CMD_IO_ETH_TEST_ETH_TEST_START_PERIODIC */, &a)?;
        Ok(())
    }

    /// Start Flood. Starts the test-frame generator sending as fast as the USB link accepts (natural NTB backpressure paces it; submit failures are counted, not lost sequence numbers). Refused while Loopback is on. Wire: `i\t\f`
    pub fn eth_test_start_flood(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(547 /* CMD_IO_ETH_TEST_ETH_TEST_START_FLOOD */, &a)?;
        Ok(())
    }

    /// Start Line Rate. Starts the test-frame generator at Line Rate % (setting e) of a 10 Mbit/s reference wire, using a token bucket that charges each frame its size plus 24 bytes of preamble/FCS/gap overhead. Refused while Loopback is on. Wire: `i\t\r`
    pub fn eth_test_start_line_rate(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(555 /* CMD_IO_ETH_TEST_ETH_TEST_START_LINE_RATE */, &a)?;
        Ok(())
    }

    /// Start Burst. Starts the test-frame generator releasing Burst Count frames (setting n) every second, the first burst immediately. Refused while Loopback is on. Wire: `i\t\b`
    pub fn eth_test_start_burst(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(544 /* CMD_IO_ETH_TEST_ETH_TEST_START_BURST */, &a)?;
        Ok(())
    }

    /// Send N Frames. Sends exactly Count test frames as fast as the link accepts, then stops by itself (Count 1 = one transmit). Counters keep running so the result can be read with Show Stats afterwards. Refused while Loopback is on. Wire: `i\t\o`
    pub fn eth_test_send_count(&mut self, count: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(count);
        let _r = crate::transport::call(553 /* CMD_IO_ETH_TEST_ETH_TEST_SEND_COUNT */, &a)?;
        Ok(())
    }

    /// Stop. Stops the test-frame generator. Counters are kept (use Clear Stats to zero them); the responder and loopback settings are unaffected. Wire: `i\t\x`
    pub fn eth_test_stop(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(560 /* CMD_IO_ETH_TEST_ETH_TEST_STOP */, &a)?;
        Ok(())
    }

    /// Show Stats. Prints one line of key=value counters: mode link TXf TXb TXfail TXfps TXkbps RXf RXb RXfps RXkbps gap lost crc under over other echoq echos echod. The fps/kbps values are 1 Hz rates; RXf counts received FWET test frames, other counts everything else (host OS chatter). Wire: `i\t\s`
    pub fn eth_test_show_stats(&mut self) -> Result<String, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(556 /* CMD_IO_ETH_TEST_ETH_TEST_SHOW_STATS */, &a)?;
        Ok(_r.string())
    }

    /// Clear Stats. Zeros every TX/RX/echo counter and restarts sequence-gap tracking. The generator, responder and link state are unaffected. Wire: `i\t\c`
    pub fn eth_test_clear_stats(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(545 /* CMD_IO_ETH_TEST_ETH_TEST_CLEAR_STATS */, &a)?;
        Ok(())
    }

    /// Set Dest MAC. Sets the destination MAC for generated test frames (default FF FF FF FF FF FF broadcast). Takes effect at the next generator start. Not persisted across reboot. Wire: `i\t\m`
    pub fn eth_test_set_dest_mac(&mut self, dest_mac: &[u8]) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.bytes(dest_mac);
        let _r = crate::transport::call(551 /* CMD_IO_ETH_TEST_ETH_TEST_SET_DEST_MAC */, &a)?;
        Ok(())
    }

    /// Link Status. Reports whether the USB network adapter is up (host selected the NCM data interface) plus the host-side MAC, device-side MAC and the device's static IP 10.55.0.2. Wire: `i\t\k`
    pub fn eth_test_link_status(&mut self) -> Result<String, OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(549 /* CMD_IO_ETH_TEST_ETH_TEST_LINK_STATUS */, &a)?;
        Ok(_r.string())
    }

    /// Frame Size. Total Ethernet frame size in bytes for generated test frames (headers included, FCS excluded). The udp frame type needs at least 66 bytes for its headers and is raised to that silently. Wire: `i\t\i`
    pub fn frame_size(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(548 /* CMD_IO_ETH_TEST_FRAME_SIZE */, &a)?;
        Ok(())
    }

    /// Period us. Microseconds between frames in Periodic mode (10000 = 100 frames per second). Wire: `i\t\u`
    pub fn period_us(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(558 /* CMD_IO_ETH_TEST_PERIOD_US */, &a)?;
        Ok(())
    }

    /// Burst Count. Frames released in each one-second burst in Burst mode. Wire: `i\t\n`
    pub fn burst_count(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(552 /* CMD_IO_ETH_TEST_BURST_COUNT */, &a)?;
        Ok(())
    }

    /// Line Rate Percent. Percentage of the 10 Mbit/s reference wire rate for Line Rate mode. Wire: `i\t\e`
    pub fn line_rate_percent(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(546 /* CMD_IO_ETH_TEST_LINE_RATE_PERCENT */, &a)?;
        Ok(())
    }

    /// Payload CRC. When on, each generated frame carries a CRC32 over its sequence/timestamp/fill so the host can prove payload integrity; costs a CRC pass per frame at high rates. Wire: `i\t\v`
    pub fn payload_crc(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(559 /* CMD_IO_ETH_TEST_PAYLOAD_CRC */, &a)?;
        Ok(())
    }

    /// Responder. When on, the device answers as 10.55.0.2: ARP requests, ICMP echo (ping) and UDP echo on port 5556. Turn off to measure pure generator/counter behavior. Wire: `i\t\a`
    pub fn responder(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(543 /* CMD_IO_ETH_TEST_RESPONDER */, &a)?;
        Ok(())
    }

    /// Loopback. When on, EVERY received frame is echoed back with its MAC addresses swapped and the generator/responder are disabled (mutually exclusive). Always off after a reboot. Wire: `i\t\l`
    pub fn loopback(&mut self) -> Result<(), OwError> {
        let a = crate::transport::Args::new();
        let _r = crate::transport::call(550 /* CMD_IO_ETH_TEST_LOOPBACK */, &a)?;
        Ok(())
    }

    /// Frame Type. Carrier for generated test frames: raw = ethertype 0x88B5 (needs npcap/scapy on the host), udp = IPv4 broadcast 10.55.0.255 port 5555 (a plain host socket receives it). Wire: `i\t\t`
    pub fn frame_type(&mut self, value: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(value);
        let _r = crate::transport::call(557 /* CMD_IO_ETH_TEST_FRAME_TYPE */, &a)?;
        Ok(())
    }
}
