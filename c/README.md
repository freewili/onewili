# OneWili C API

Generated C11 bindings for the FreeWili serial menu system.
Produced by the menutool API Generator - do not edit by hand.

The library is transport-agnostic: you provide `ow_transport` read/write
callbacks (embedded-friendly, zero dependencies). `examples/serial_pc.c`
implements them for Windows (Win32) and POSIX (termios) at 1,000,000 baud.

Events: `ow_binary_poll` (binary/FTDI WILI frames, see
`include/onewili_events.h` for the typed structs) and
`ow_poll_text_event` (text `[*id ...]` frames). Poll-based, no threads.

For arbitrary message types use `ow_binary_poll_raw`, which returns the
header type, repeat count, error flag and all payload bytes. Raw and typed
polls consume the same stream. Returned payload/sample pointers remain valid
until the next poll. Copy them to retain a capture.

`ow_binary_open` uses a 4096-byte inline buffer. For logic-analyzer captures,
allocate `OW_BIN_CAPTURE_CAPACITY` bytes and call `ow_binary_open_buffer`.
The caller owns that buffer through close; the API performs no allocation.
Typed logic-analyzer events include `sample_data`/`sample_bytes`: little-endian
digital words in ring order, then a 2048-byte analog buffer when enabled.
Digital head/trigger indices are words; analog indices are samples. CAN FD
events retain both controller headers and all sixteen data words.

Peer streams (`include/onewili_stream.h`): `ow_stream_write`, `ow_stream_poll`
and `ow_stream_drops` pass best-effort datagrams of 1-128 bytes between
OneWili clients (the display CPU, the ESP32, the CM0 and this host), routed
by the main CPU. Here they ride three menu commands, with poll results
fetched in batches; a push-capable transport binds a faster path with
`ow_stream_bind`. The same calls mean the same thing on every target.

## Build

```
cmake -S . -B build && cmake --build build
```

## Usage

```c
serial_pc* sp = serial_pc_open("COM5");   /* find it with list_devices */
ow_transport t = serial_pc_transport(sp);
ow_device dev;
ow_open(&dev, &t);
ow_status r = ow_io_gpio_set_io_toggle(&dev, 25);
ow_close(&dev);
serial_pc_close(sp);
```

Full reference: [../docs/index.md](../docs/index.md)
