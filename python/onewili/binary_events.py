"""Decoded binary (FTDI/WILI) event types - generated. Do not edit."""
from __future__ import annotations

import dataclasses
import struct


@dataclasses.dataclass(frozen=True)
class CanRxReportEvent:
    time_stamp_ns: int
    gpio_bitfield: int
    r0_canid: int
    r1_filter_header_bits: int
    data_words: tuple
    error: bool

_CAN_RX_REPORT_FMT = "<QIII16I"

def _decode_can_rx_report(payload: bytes, error: bool) -> CanRxReportEvent:
    vals = struct.unpack(_CAN_RX_REPORT_FMT, payload)
    args = []
    i = 0
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(tuple(vals[i:i + 16])); i += 16
    return CanRxReportEvent(*args, error)

@dataclasses.dataclass(frozen=True)
class GpioReportEvent:
    time_stamp_ns: int
    gpio_bitfield: int
    error: bool

_GPIO_REPORT_FMT = "<QI"

def _decode_gpio_report(payload: bytes, error: bool) -> GpioReportEvent:
    vals = struct.unpack(_GPIO_REPORT_FMT, payload)
    args = []
    i = 0
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    return GpioReportEvent(*args, error)

@dataclasses.dataclass(frozen=True)
class LogicAnalyzerReportEvent:
    trigger_time_stamp_ns: int
    sample_rate_ns: int
    gpio_start_pin: int
    bits_per_sample: int
    trigger_type: int
    dummy: int
    trigger_location: int
    buffer_head: int
    analog_channel_mask: int
    analog_resolution: int
    analog_channel_count: int
    analog_dummy: int
    analog_sample_rate_ns: int
    analog_sample_count: int
    analog_buffer_head: int
    analog_trigger_location: int
    error: bool

_LOGIC_ANALYZER_REPORT_FMT = "<QIBBBBIIBBBBIIII"

def _decode_logic_analyzer_report(payload: bytes, error: bool) -> LogicAnalyzerReportEvent:
    vals = struct.unpack(_LOGIC_ANALYZER_REPORT_FMT, payload)
    args = []
    i = 0
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    args.append(vals[i]); i += 1
    return LogicAnalyzerReportEvent(*args, error)

DECODERS = {
    1: ("canRxReport", 84, _decode_can_rx_report),
    0: ("gpioReport", 12, _decode_gpio_report),
    2: ("logicAnalyzerReport", 44, _decode_logic_analyzer_report),
}
