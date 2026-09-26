# Extra Actions

`dev.wireless.nfc.extra` - wire path `w\n\x` - generated from `fwMenuNFCExtra`.

## halt_card

Halt Card. Send HLTA command to put card in HALT state

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\x\a`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.extra.halt_card() -> Result
```
```c
ow_status ow_wireless_nfc_extra_halt_card(ow_device* dev);
```
```rust
dev.wireless().nfc().extra().halt_card() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.extra.halt_card()   # check dev.ok
```
