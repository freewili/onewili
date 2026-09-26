//! Decoded event types - generated. Do not edit.
//! Binary events arrive as WILI frames on the FTDI port
//! (`binary_transport`); text events as `[*<id> ...]` lines on the
//! main port. Both are delivered by `OneWili::poll_event`.

/// canRxReport - CAN RX frame report (binary API, MCP2518 memory-map layout). `apiFrame_canRxReport`, 84 bytes.
#[derive(Debug, Clone, PartialEq)]
pub struct CanRxReportEvent {
    pub time_stamp_ns: u64,   // ui64TimeStampNs @ 0
    pub gpio_bitfield: u32,   // uiGpioBitfield @ 8
    pub r0_canid: u32,   // uiR0_CANID @ 12
    pub r1_filter_header_bits: u32,   // uiR1_Filter_HeaderBits @ 16
    pub data_words: [u32; 16],   // uiDataWords @ 20
    pub error: bool,   // frame header error bit
}

/// gpioReport - Periodic GPIO bitfield report (binary API). `apiFrame_gpioReport`, 12 bytes.
#[derive(Debug, Clone, PartialEq)]
pub struct GpioReportEvent {
    pub time_stamp_ns: u64,   // ui64TimeStampNs @ 0
    pub gpio_bitfield: u32,   // uiGpioBitfield @ 8
    pub error: bool,   // frame header error bit
}

/// logicAnalyzerReport - Logic analyzer capture report (binary API; header then digital + analog samples). `apiFrame_LogicAnalyzerReport`, 44 bytes.
#[derive(Debug, Clone, PartialEq)]
pub struct LogicAnalyzerReportEvent {
    pub trigger_time_stamp_ns: u64,   // ui64TriggerTimeStampNs @ 0
    pub sample_rate_ns: u32,   // uiSampleRateNs @ 8
    pub gpio_start_pin: u8,   // uiGPIOStartPin @ 12
    pub bits_per_sample: u8,   // uiBitsPerSample @ 13
    pub trigger_type: u8,   // uiTriggerType @ 14
    pub dummy: u8,   // uiDummy @ 15
    pub trigger_location: u32,   // uiTriggerLocation @ 16
    pub buffer_head: u32,   // uiBufferHead @ 20
    pub analog_channel_mask: u8,   // uiAnalogChannelMask @ 24
    pub analog_resolution: u8,   // uiAnalogResolution @ 25
    pub analog_channel_count: u8,   // uiAnalogChannelCount @ 26
    pub analog_dummy: u8,   // uiAnalogDummy @ 27
    pub analog_sample_rate_ns: u32,   // uiAnalogSampleRateNs @ 28
    pub analog_sample_count: u32,   // uiAnalogSampleCount @ 32
    pub analog_buffer_head: u32,   // uiAnalogBufferHead @ 36
    pub analog_trigger_location: u32,   // uiAnalogTriggerLocation @ 40
    pub error: bool,   // frame header error bit
    pub sample_data: Vec<u8>, // digital then analog bytes, ring order
}

/// Any event the device can deliver via `OneWili::poll_event`.
#[derive(Debug, Clone, PartialEq)]
pub enum Event {
    CanRxReport(CanRxReportEvent),
    GpioReport(GpioReportEvent),
    LogicAnalyzerReport(LogicAnalyzerReportEvent),
    /// Text event `[*<id> <args>]` from the main serial port.
    Text { id: String, args: String },
    /// Binary frame with no generated decoder (counted in unknown_frames).
    Unknown { header_type: u16, len: usize },
}

/// (header_type, expected payload size, event id) per linked binary event.
pub const EXPECTED_SIZES: &[(u16, usize, &str)] = &[
    (1, 84, "canRxReport"),
    (0, 12, "gpioReport"),
    (2, 44, "logicAnalyzerReport"),
];

/// Decode one binary frame payload. `None` = known type, wrong size.
pub fn decode(header_type: u16, payload: &[u8], error: bool) -> Option<Event> {
    match header_type {
        1 => {
            if payload.len() != 84 {
                return None;
            }
            let mut data_words = [0u32; 16];
            for k in 0..16usize {
                let o = 20 + 4 * k;
                data_words[k] = u32::from_le_bytes(payload[o..o + 4].try_into().unwrap());
            }
            Some(Event::CanRxReport(CanRxReportEvent {
                time_stamp_ns: u64::from_le_bytes(payload[0..8].try_into().unwrap()),
                gpio_bitfield: u32::from_le_bytes(payload[8..12].try_into().unwrap()),
                r0_canid: u32::from_le_bytes(payload[12..16].try_into().unwrap()),
                r1_filter_header_bits: u32::from_le_bytes(payload[16..20].try_into().unwrap()),
                data_words,
                error,
            }))
        }
        0 => {
            if payload.len() != 12 {
                return None;
            }
            Some(Event::GpioReport(GpioReportEvent {
                time_stamp_ns: u64::from_le_bytes(payload[0..8].try_into().unwrap()),
                gpio_bitfield: u32::from_le_bytes(payload[8..12].try_into().unwrap()),
                error,
            }))
        }
        2 => {
            if payload.len() < 44 {
                return None;
            }
            if (payload.len() - 44) % 4 != 0 { return None; }
            Some(Event::LogicAnalyzerReport(LogicAnalyzerReportEvent {
                trigger_time_stamp_ns: u64::from_le_bytes(payload[0..8].try_into().unwrap()),
                sample_rate_ns: u32::from_le_bytes(payload[8..12].try_into().unwrap()),
                gpio_start_pin: payload[12],
                bits_per_sample: payload[13],
                trigger_type: payload[14],
                dummy: payload[15],
                trigger_location: u32::from_le_bytes(payload[16..20].try_into().unwrap()),
                buffer_head: u32::from_le_bytes(payload[20..24].try_into().unwrap()),
                analog_channel_mask: payload[24],
                analog_resolution: payload[25],
                analog_channel_count: payload[26],
                analog_dummy: payload[27],
                analog_sample_rate_ns: u32::from_le_bytes(payload[28..32].try_into().unwrap()),
                analog_sample_count: u32::from_le_bytes(payload[32..36].try_into().unwrap()),
                analog_buffer_head: u32::from_le_bytes(payload[36..40].try_into().unwrap()),
                analog_trigger_location: u32::from_le_bytes(payload[40..44].try_into().unwrap()),
                sample_data: payload[44..].to_vec(),
                error,
            }))
        }
        _ => Some(Event::Unknown { header_type, len: payload.len() }),
    }
}

#[cfg(test)]
mod logic_analyzer_report_tests {
    use super::*;
    #[test]
    fn variable_samples_and_malformed_tail() {
        let mut payload = vec![0u8; 44];
        payload.extend_from_slice(&[0, 0xff, 0x57, 0x49]);
        match decode(2, &payload, true).unwrap() {
            Event::LogicAnalyzerReport(event) => {
                assert_eq!(event.sample_data, vec![0, 0xff, 0x57, 0x49]);
                assert!(event.error);
            },
            _ => panic!("wrong event"),
        }
        payload.push(1);
        assert!(decode(2, &payload, false).is_none());
    }
}
