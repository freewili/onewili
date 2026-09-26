# OneWili API

Generated Python bindings for the FreeWili serial menu system.
Produced by the menutool API Generator - do not edit by hand;
regenerate from the firmware sources instead.

## Install

```
pip install -e .
```

## Quickstart

```python
import onewili

dev = onewili.connect()   # discovers the device with pyfwfinder
# every firmware menu is an attribute, e.g. dev.io - see docs/index.md

res = dev.some_menu.some_command(...)
match res:
    case onewili.Ok(value):
        print(value)
    case onewili.Err(message):
        print("failed:", message)

dev.close()
```

## Binary streaming

`connect(binary=True)` opens both the Main command port and FTDI binary port.
Commands remain available while a background reader receives binary frames.
Enable host streaming with `dev.hardware.system.event_host_streaming(1)`, then
start the producer (for example `dev.io.canfd.enable_canfd_stream(...)` or
`dev.io.logic_analyzer.start()`). FPGA power must be enabled for acquisition.

```python
import onewili
from onewili.binary_events import LogicAnalyzerReportEvent, CanRxReportEvent
from onewili.binary_framing import RawFrame

dev = onewili.connect(binary=True)
try:
    event = dev.binary_events.get(timeout=5)
    if isinstance(event, LogicAnalyzerReportEvent):
        values = list(event.digital_samples())  # chronological, relative to start pin
        pin_values = list(event.samples(event.gpio_start_pin))
        adc_values = list(event.analog_samples())  # chronological, interleaved
    elif isinstance(event, CanRxReportEvent):
        print(event.r0_canid, event.r1_filter_header_bits, event.data_words)
    elif isinstance(event, RawFrame):
        print(event.header_type, event.repeat_count, event.error, event.payload)
finally:
    dev.close()
```

For every frame unchanged, including future/unknown message types, use
`connect(binary=True, raw=True)`. `RawFrame.payload` is immutable bytes, including
embedded NULs. Known malformed payloads also arrive as raw frames in decoded mode;
they increment `dev.binary_stream.size_mismatches`. CAN reports retain all sixteen
data words (64 bytes) and controller header flags, including the DLC.

The default queue holds 256 events. Set `queue_size=` when opening; slow consumers
drop the oldest event and increment `dev.binary_stream.dropped_events`. Check
`last_error` for a disconnected reader. `close_binary()` leaves text commands
open; `open_binary()` can restart a failed reader. Reopening discards stale queued
events. `connect()` remembers a discovered FTDI port for a later `open_binary()`;
with an explicit `OneWili` constructor, pass `binary_port=` yourself.

Logic-analyzer `sample_data` preserves the original digital/analog buffers.
`digital_words` is in wire/ring order; `digital_samples()` rotates using
`buffer_head`, with LSB-first samples and `32 // bits_per_sample` samples per word.
An enabled analog buffer occupies the last 2048 bytes. The digital limit is
262144 words; the parser accepts the full 1 MiB digital buffer plus analog data
and the 44-byte report header. Helpers retain all samples, including any stale
FIFO prefix produced by firmware.

Run a complete PWM generation/capture/measurement check on an idle GPIO25:

```
python examples/capture_pwm.py --continuous --samples 262144 --captures 5
```

The example primes one capture, verifies frequency/duty across complete cycles,
saves sample bytes, and checks text commands during streaming. It documents the
FIFO-prefix exclusion used only for measurement. Cleanup stops capture, drives
the pin LOW, restores the host-streaming gate, and releases both ports.

## Menus

- `dev.io` - IO functions (hotkey `i`)
- `dev.gui` - GUI Functions (hotkey `g`)
- `dev.hardware` - Hardware Functions (hotkey `h`)
- `dev.wireless` - Wireless (hotkey `w`)
- `dev.scripting` - Scripting Functions (hotkey `s`)
- `dev.apps` - Apps functions (hotkey `a`)
- `dev.linux` - Linux Functions (hotkey `l`)
- `dev.logger` - Logger (hotkey `r`)

Full reference (shared across languages): [../docs/index.md](../docs/index.md)
