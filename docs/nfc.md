# NFC Functions

`dev.wireless.nfc` - wire path `w\n` - generated from `fwMenuNFC`.

## enable_reader

Enable Reader. Enable/disable NFC reader with auto tag streaming

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\r`

| Arg | Wire type |
|---|---|
| enable | dec |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.enable_reader(enable: int) -> Result
```
```c
ow_status ow_wireless_nfc_enable_reader(ow_device* dev, int32_t enable);
```
```rust
dev.wireless().nfc().enable_reader(enable: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.enable_reader(enable)   # check dev.ok
```

## print_card_info

Print Card Info. Display detailed info about detected card

Wire command: `w\n\c`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.print_card_info() -> Result
```
```c
ow_status ow_wireless_nfc_print_card_info(ow_device* dev);
```
```rust
dev.wireless().nfc().print_card_info() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.print_card_info()   # check dev.ok
```

## get_status

Get Status (debug). Display NFC hardware state and debug info

Wire command: `w\n\g`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.get_status() -> Result
```
```c
ow_status ow_wireless_nfc_get_status(ow_device* dev);
```
```rust
dev.wireless().nfc().get_status() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.get_status()   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `nfc` (text)

NFC card status text (card detected / removed)

| Payload field | Wire type |
|---|---|
| data | string |

## Sub-menus

- [Saved Cards](nfc_saved_cards.md) - `dev.wireless.nfc.saved_cards`
- [MIFARE Classic](nfc_mifare_classic.md) - `dev.wireless.nfc.mifare_classic`
- [Raw Transceiver](nfc_raw.md) - `dev.wireless.nfc.raw`
- [Extra Actions](nfc_extra.md) - `dev.wireless.nfc.extra`
