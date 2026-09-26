# MIFARE Classic

`dev.wireless.nfc.mifare_classic` - wire path `w\n\m` - generated from `fwMenuNFCMifareClassic`.

## read_with_keys

Read with Keys. Authenticate and read sectors using known keys

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\m\r`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.mifare_classic.read_with_keys() -> Result
```
```c
ow_status ow_wireless_nfc_mifare_classic_read_with_keys(ow_device* dev);
```
```rust
dev.wireless().nfc().mifare_classic().read_with_keys() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.mifare_classic.read_with_keys()   # check dev.ok
```

## dictionary_attack

Dictionary Attack. Try keys from dictionary file to recover unknown keys

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

The dictionary is a plain-text list of candidate 6-byte keys, one per line as 12 hex chars ('#' starts a comment); it is Flipper-Zero compatible.

Where to get one: the Flipper Zero 'mf_classic_dict.nfc' (bundled with Flipper firmware and in community key packs), or any MIFARE Classic key list.

Where to save it: put it on the SD card at /nfc/assets/mf_classic_dict.nfc (the default shown in brackets), or pass a full path as the argument. Each key is tried against every sector; keys that authenticate are cached so Dump Card can then read the card.

Wire command: `w\n\m\a`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.mifare_classic.dictionary_attack(path: str) -> Result
```
```c
ow_status ow_wireless_nfc_mifare_classic_dictionary_attack(ow_device* dev, const char* path);
```
```rust
dev.wireless().nfc().mifare_classic().dictionary_attack(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.mifare_classic.dictionary_attack(path)   # check dev.ok
```

## dump_card

Dump Card. Read all sectors with known keys and display contents

Requires power zone 13 (NFC/RFID). See [Errors](errors.md).

Wire command: `w\n\m\u`

Returns: none (Ok/Err only)

```python
dev.wireless.nfc.mifare_classic.dump_card() -> Result
```
```c
ow_status ow_wireless_nfc_mifare_classic_dump_card(ow_device* dev);
```
```rust
dev.wireless().nfc().mifare_classic().dump_card() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.nfc.mifare_classic.dump_card()   # check dev.ok
```
