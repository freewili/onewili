# OneWili C API

Generated C11 bindings for the FreeWili serial menu system.
Produced by the menutool API Generator - do not edit by hand.

The library is transport-agnostic: you provide `ow_transport` read/write
callbacks (embedded-friendly, zero dependencies). `examples/serial_pc.c`
implements them for Windows (Win32) and POSIX (termios) at 1,000,000 baud.

Events: `ow_binary_poll` (binary/FTDI WILI frames, see
`include/onewili_events.h` for the typed structs) and
`ow_poll_text_event` (text `[*id ...]` frames). Poll-based, no threads.

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
