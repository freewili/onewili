# OneWili WASM API

Generated bindings for FreeWili **WASM guest programs** (C/C++ and Rust,
compiled to `.wasm` and run by the on-device interpreter). Produced by the
menutool API Generator - do not edit by hand.

The function signatures (`include/onewili.h`) are byte-identical to the PC
host C library: the same `ow_io_gpio_set_io_toggle(&dev, 25)` calls. But
`src/onewili.c` is a thin guest here: each call packs its arguments into a
little-endian blob and dispatches through the single `ow_call` host import
(declared in `include/onewili_wasm.h`, backed by `include/onewili_cmdids.h`'s
`CMD_*` constants) - the firmware assembles and runs the wire command
natively, so there is no string assembly or frame parsing on the guest side.

## Retained non-menu imports

`include/onewili_wasm.h` also declares the imports that are not device menu
commands and have no OneWili equivalent: timing (`waitms`, `millis`),
`wilirand`, the `log*` debug family, and file I/O.

## Examples

`examples/blink.c` / `rust/examples/blink.rs` toggle a GPIO; `examples/can_rx.c` /
`rust/examples/can_rx.rs` poll the on-device CAN(FD) receive queue with
`receive_canfd` and echo each frame - the on-device pattern for anything a host
would receive as an event stream.

## Build

Compile with the bundled wiliclang against the wasm32 target, `-nostdlib`,
`-Wl,--no-entry --allow-undefined`, exporting `_start`. See the firmware's
`WiliWasm/test/fixtures/README.md` for the exact invocation.
