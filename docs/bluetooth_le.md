# BT Functions

`dev.wireless.bluetooth_le` - wire path `w\b` - generated from `fwMenuBluetoothLE`.

## on_start_bt_advertising

Start BT Advertising. Sets the Host Name for the Bluetooth LE

Requires power zone 5 (ESP32). See [Errors](errors.md).

#### Start BT Advertising

Starts BT LE advertising on the FreeWili's ESP32-C5 wireless co-processor using the supplied **advertising (host) name**. Once advertising, the FreeWili is discoverable by BLE central devices (phones, laptops, scanners, etc.).

##### Arguments

- `hostname` *(string, required)* — The BLE advertising name to broadcast.
  - Must be **1–32 characters**.
  - Sent as the device's advertised local name.

##### Returns

- `success` *(basic)* — `true` if advertising was started, `false` otherwise.

##### Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Validates the hostname (non-empty, within length limits).
3. Stores the name in persistent menu settings (`szBTAdv`) and enables BLE (`m_bEnableBLE = true`).
4. Pushes the updated configuration to the wireless module via `UpdateBottleNoseSettings()`.

##### Failure Cases

- ESP32-C5 is not connected.
- `hostname` is empty.
- `hostname` exceeds the maximum allowed length.

##### Related

- `Stop BT Advertising` — stops the BLE advertisement.
- `Enable Terminal API Mode` — exposes the FreeWili Terminal API over BLE.
- `Scan for BT Devices` — scans for nearby BLE peripherals.

##### Example

```
a
MyFreeWili
```

Advertises the device as **MyFreeWili**.

Wire command: `w\b\a`

| Arg | Wire type |
|---|---|
| hostname | string |

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.on_start_bt_advertising(hostname: str) -> Result
```
```c
ow_status ow_wireless_bluetooth_le_on_start_bt_advertising(ow_device* dev, const char* hostname);
```
```rust
dev.wireless().bluetooth_le().on_start_bt_advertising(hostname: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.on_start_bt_advertising(hostname)   # check dev.ok
```

## on_stop_bt_advertising

Stop BT Advertising. Stops BT Advertising

Requires power zone 5 (ESP32). See [Errors](errors.md).

#### Stop BT Advertising

Stops BLE advertising on the FreeWili's ESP32-C5 wireless co-processor. After this command, the FreeWili is no longer discoverable by BLE central devices (phones, laptops, scanners, etc.).

##### Arguments

*None.*

##### Returns

- `success` *(basic)* — `true` if advertising was stopped, `false` otherwise.

##### Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Disables BLE in persistent menu settings (`m_bEnableBLE = false`).
3. Pushes the updated configuration to the wireless module via `UpdateBottleNoseSettings()`.

##### Failure Cases

- ESP32-C5 is not connected.

##### Related

- `Start BT Advertising` — begins BLE advertising with a chosen host name.
- `Enable Terminal API Mode` — exposes the FreeWili Terminal API over BLE.
- `Scan for BT Devices` — scans for nearby BLE peripherals.

##### Example

```
t
```

Stops BLE advertising on the device.

Wire command: `w\b\t`

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.on_stop_bt_advertising() -> Result
```
```c
ow_status ow_wireless_bluetooth_le_on_stop_bt_advertising(ow_device* dev);
```
```rust
dev.wireless().bluetooth_le().on_stop_bt_advertising() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.on_stop_bt_advertising()   # check dev.ok
```

## on_scan_bt_devices

Scan for BT Devices. Scans for BT devices for a given duration

Requires power zone 5 (ESP32). See [Errors](errors.md).

#### Scan for BT Devices

Performs a Bluetooth LE scan on the FreeWili's ESP32-C5 wireless co-processor for the specified duration. Discovered peripherals are reported asynchronously as `btscan` events containing the advertised name, MAC address, and RSSI.

##### Arguments

- `durationms` *(decU32, required)* — Scan duration in **milliseconds**. Must be a positive integer.

##### Returns

- `success` *(basic)* — `true` if the scan was successfully started, `false` otherwise.

##### Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Parses `durationms` from the input. On parse failure, returns `false`.
3. Starts a BLE scan via `startBluetoothLEScan(true, durationms)`.
4. For each device discovered, an event is emitted under the `btscan` channel with the format:

   ```
   <deviceName> <AA:BB:CC:DD:EE:FF> <rssi>
   ```

   - `deviceName` — advertised local name (up to 32 characters; may be empty)
   - MAC address — 6 bytes, colon-separated, lowercase hex
   - `rssi` — signed received signal strength in dBm

##### Failure Cases

- ESP32-C5 is not connected.
- `durationms` is missing or not a valid unsigned integer.

##### Related

- `Start BT Advertising` — begins BLE advertising with a chosen host name.
- `Stop BT Advertising` — stops the BLE advertisement.
- `Enable Terminal API Mode` — exposes the FreeWili Terminal API over BLE.

##### Example

```
s
5000
```

Scans for nearby BLE devices for 5 seconds, streaming each discovery as a `btscan` event.

Wire command: `w\b\s`

| Arg | Wire type |
|---|---|
| durationms | decU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.on_scan_bt_devices(durationms: int) -> Result
```
```c
ow_status ow_wireless_bluetooth_le_on_scan_bt_devices(ow_device* dev, int32_t durationms);
```
```rust
dev.wireless().bluetooth_le().on_scan_bt_devices(durationms: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.on_scan_bt_devices(durationms)   # check dev.ok
```

## on_enable_terminal

Toggle Enable Terminal API Mode. Enables BLE to FreeWili Terminal API Mode

Requires power zone 5 (ESP32). See [Errors](errors.md).

#### Toggle Enable Terminal API Mode

Toggles the FreeWili **Terminal API** exposure over BT LE on the ESP32-C5 wireless co-processor. When enabled, BLE central devices (phones, laptops, scripts, agents) can connect to the FreeWili and drive the same Terminal/CLI API that is normally available over USB. When disabled, the BLE GATT service for the Terminal API is torn down.

This is a **toggle**: each invocation flips the current state.

##### Arguments

*None.*

##### Returns

- `success` *(basic)* — `true` if the toggle was applied and pushed to the wireless module, `false` otherwise.

##### Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Inverts the persistent setting `m_bEnableBLETerm` (on ↔ off).
3. Pushes the updated configuration to the wireless module via `UpdateBottleNoseSettings()`.
4. Returns `true`.

##### Notes

- BLE advertising must be running (see `Start BT Advertising`) for central devices to discover and connect to the Terminal API.
- Because this is a toggle, run it twice to return to the original state.
- The new state is persisted in menu settings and survives subsequent setting updates.

##### Failure Cases

- ESP32-C5 is not connected.

##### Related

- `Start BT Advertising` — begins BLE advertising with a chosen host name.
- `Stop BT Advertising` — stops the BLE advertisement.
- `Scan for BT Devices` — scans for nearby BLE peripherals.

##### Example

```
e
```

Toggles Terminal API over BLE. Run once to enable, again to disable.

Wire command: `w\b\e`

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.on_enable_terminal() -> Result
```
```c
ow_status ow_wireless_bluetooth_le_on_enable_terminal(ow_device* dev);
```
```rust
dev.wireless().bluetooth_le().on_enable_terminal() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.on_enable_terminal()   # check dev.ok
```

## on_set_attribute

Set Attribute. Sets a user attribute slot a connected phone can read and subscribe to

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\b\v`

| Arg | Wire type |
|---|---|
| slot | decU32 |
| value | string |

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.on_set_attribute(slot: int, value: str) -> Result
```
```c
ow_status ow_wireless_bluetooth_le_on_set_attribute(ow_device* dev, int32_t slot, const char* value);
```
```rust
dev.wireless().bluetooth_le().on_set_attribute(slot: i32, value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.on_set_attribute(slot, value)   # check dev.ok
```

## on_get_attribute

Get Attribute. Reads a user attribute slot back from the radio

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\b\u`

| Arg | Wire type |
|---|---|
| slot | decU32 |

Returns: none (Ok/Err only)

```python
dev.wireless.bluetooth_le.on_get_attribute(slot: int) -> Result
```
```c
ow_status ow_wireless_bluetooth_le_on_get_attribute(ow_device* dev, int32_t slot);
```
```rust
dev.wireless().bluetooth_le().on_get_attribute(slot: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.bluetooth_le.on_get_attribute(slot)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `btscan` (text)

BLE scan result (device name, MAC address, RSSI)

| Payload field | Wire type |
|---|---|
| name | string |
| mac | string |
| rssi | decS32 |

### `attrvalue` (text)

User attribute slot value read back from the radio

| Payload field | Wire type |
|---|---|
| slot | decU32 |
| value | string |

### `attrwritten` (text)

A connected peer wrote a user attribute slot

| Payload field | Wire type |
|---|---|
| slot | decU32 |
| value | string |

## Sub-menus

- [Bluetooth Settings](ble_settings_2.md) - `dev.wireless.bluetooth_le.ble_settings`
