# OneWili Rust API

Generated Rust bindings for the FreeWili serial menu system.
Produced by the menutool API Generator - do not edit by hand.

## Usage

```rust
let mut dev = onewili::OneWili::connect()?;   // finds the device by USB VID
dev.io().gpio().set_io_toggle(25)?;
```

Run the examples:

```
cargo run --example list_devices
cargo run --example toggle_gpio_25
```

Events (text + binary/FTDI): open with `OneWili::connect_binary()?`
and drain with `dev.poll_event()?` - see `src/events.rs` for the
typed structs. Poll-based, no threads. The crate's WILI parser has
its own unit tests: `cargo test`.

Use `dev.poll_binary_frame()?` for every message unchanged, including unknown
types. The owned `RawFrame` contains header_type, repeat_count, error and
payload bytes. Raw and typed polls consume the same stream. Typed logic
events include owned `sample_data`: digital words in little-endian ring
order, followed by 2048 analog bytes when the channel mask is nonzero.
The parser supports the full 1 MiB digital capture plus analog/header bytes.

Full reference: [../docs/index.md](../docs/index.md)
