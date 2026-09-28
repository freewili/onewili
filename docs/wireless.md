# Wireless

`dev.wireless` - wire path `w` - generated from `fwMenuWireless`.

## e_sp32_mode

ESP32 Mode. What the ESP32-C5 runs: Default Firmware (the stock WiFi/BLE app) or OneWili API (a BSP app on the ESP32 drives MAIN's OneWili API). OneWili API does not install an app - flash one first.

#### ESP32 Mode

What the ESP32-C5 runs, and therefore whether MAIN treats its link as a OneWili client.

- **Default Firmware** (0) - the stock Bottlenose application: WiFi, BLE and the BLE terminal. MAIN ignores OneWili traffic from the ESP32.
- **OneWili API** (1) - a BSP app on the ESP32 drives MAIN's OneWili API over the Bottlenose link: MAIN answers its commands, mirrors text and binary events to it, and routes its peer-stream datagrams. The ESP32's power zone stays on while this is selected.

Selecting OneWili API does not put an app on the ESP32 - flash one first. The setting is saved and survives a reboot.

Wire command: `w\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.e_sp32_mode(value: int) -> Result
```
```c
ow_status ow_wireless_e_sp32_mode(ow_device* dev, int32_t value);
```
```rust
dev.wireless().e_sp32_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.e_sp32_mode(value)   # check dev.ok
```

## Sub-menus

- [NFC Functions](nfc.md) - `dev.wireless.nfc`
- [ESP32 Flasher Functions](esp32_flasher.md) - `dev.wireless.esp32_flasher`
- [Wifi Functions](wifi.md) - `dev.wireless.wifi`
- [BT Functions](bluetooth_le.md) - `dev.wireless.bluetooth_le`
- [IR Functions](ir.md) - `dev.wireless.ir`
- [LoRa](lo_ra.md) - `dev.wireless.lo_ra`
- [Radio](radio.md) - `dev.wireless.radio`
- [RFID Functions](rfid.md) - `dev.wireless.rfid`
