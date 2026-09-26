# Saved Cards

`dev.wireless.nfc.saved_cards` - wire path `w\n\s` - generated from `fwMenuNFCSavedCards`.

## list_saved_cards

List Saved Cards. List all .nfc files in the saved cards directory

Requires power zone 7 (SD Card). See [Errors](errors.md).

Wire command: `w\n\s\l`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.saved_cards.list_saved_cards() -> Result
```
```c
ow_status ow_wireless_nfc_saved_cards_list_saved_cards(ow_device* dev);
```
```rust
dev.wireless().nfc().saved_cards().list_saved_cards() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.saved_cards.list_saved_cards()   # check dev.ok
```

## load_card

Load Card. Load card data from .nfc file

Requires power zone 7 (SD Card). See [Errors](errors.md).

Wire command: `w\n\s\o`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.saved_cards.load_card(path: str) -> Result
```
```c
ow_status ow_wireless_nfc_saved_cards_load_card(ow_device* dev, const char* path);
```
```rust
dev.wireless().nfc().saved_cards().load_card(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.saved_cards.load_card(path)   # check dev.ok
```

## save_current_card

Save Current Card. Save currently detected card to .nfc file

Requires power zone 7 (SD Card). See [Errors](errors.md).

Wire command: `w\n\s\s`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.saved_cards.save_current_card(path: str) -> Result
```
```c
ow_status ow_wireless_nfc_saved_cards_save_current_card(ow_device* dev, const char* path);
```
```rust
dev.wireless().nfc().saved_cards().save_current_card(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.saved_cards.save_current_card(path)   # check dev.ok
```

## emulate_card

Play (Emulate) Card. Transmit (emulate) a saved card's NFC-A UID/ATQA/SAK

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\s\e`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.saved_cards.emulate_card(path: str) -> Result
```
```c
ow_status ow_wireless_nfc_saved_cards_emulate_card(ow_device* dev, const char* path);
```
```rust
dev.wireless().nfc().saved_cards().emulate_card(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.saved_cards.emulate_card(path)   # check dev.ok
```

## stop_emulation

Stop Emulation. Stop NFC card emulation

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\s\t`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.saved_cards.stop_emulation() -> Result
```
```c
ow_status ow_wireless_nfc_saved_cards_stop_emulation(ow_device* dev);
```
```rust
dev.wireless().nfc().saved_cards().stop_emulation() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.saved_cards.stop_emulation()   # check dev.ok
```
