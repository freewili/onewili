# Wifi Settings

`dev.wireless.wifi.settings` - wire path `w\w\e` - generated from `fwMenuWifiSettings`.

## enable_station_mode

Enable Station Mode. Connect the device to an existing Wi-Fi network in station mode, or disconnect from it

Wire command: `w\w\e\s`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.enable_station_mode() -> Result
```
```c
ow_status ow_wireless_wifi_settings_enable_station_mode(ow_device* dev);
```
```rust
dev.wireless().wifi().settings().enable_station_mode() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.enable_station_mode()   # check dev.ok
```

## s_sid_for_station_mode

SSID for Station Mode. Set the name (SSID) of the Wi-Fi network to join in station mode

Wire command: `w\w\e\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.s_sid_for_station_mode(value: str) -> Result
```
```c
ow_status ow_wireless_wifi_settings_s_sid_for_station_mode(ow_device* dev, const char* value);
```
```rust
dev.wireless().wifi().settings().s_sid_for_station_mode(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.s_sid_for_station_mode(value)   # check dev.ok
```

## password_for_station_mode

Password for Station Mode. Set the password used to join the Wi-Fi network in station mode

Wire command: `w\w\e\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.password_for_station_mode(value: str) -> Result
```
```c
ow_status ow_wireless_wifi_settings_password_for_station_mode(ow_device* dev, const char* value);
```
```rust
dev.wireless().wifi().settings().password_for_station_mode(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.password_for_station_mode(value)   # check dev.ok
```

## enable_ap_mode

Enable AP Mode. Turn the device's own Wi-Fi access point on or off

Wire command: `w\w\e\a`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.enable_ap_mode() -> Result
```
```c
ow_status ow_wireless_wifi_settings_enable_ap_mode(ow_device* dev);
```
```rust
dev.wireless().wifi().settings().enable_ap_mode() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.enable_ap_mode()   # check dev.ok
```

## a_p_auth

AP Auth. Choose the Wi-Fi security type used by the device's own access point

Wire command: `w\w\e\u`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.a_p_auth(value: int) -> Result
```
```c
ow_status ow_wireless_wifi_settings_a_p_auth(ow_device* dev, int32_t value);
```
```rust
dev.wireless().wifi().settings().a_p_auth(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.a_p_auth(value)   # check dev.ok
```

## a_p_hide_ssid

AP hide SSID. Hide the access point's network name (SSID) so it isn't broadcast to nearby devices

Wire command: `w\w\e\i`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.a_p_hide_ssid() -> Result
```
```c
ow_status ow_wireless_wifi_settings_a_p_hide_ssid(ow_device* dev);
```
```rust
dev.wireless().wifi().settings().a_p_hide_ssid() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.a_p_hide_ssid()   # check dev.ok
```

## s_sid_for_ap

SSID for AP. Set the network name (SSID) broadcast by the device's own access point

Wire command: `w\w\e\g`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.s_sid_for_ap(value: str) -> Result
```
```c
ow_status ow_wireless_wifi_settings_s_sid_for_ap(ow_device* dev, const char* value);
```
```rust
dev.wireless().wifi().settings().s_sid_for_ap(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.s_sid_for_ap(value)   # check dev.ok
```

## password_for_ap

Password for AP. Set the password required to join the device's own access point

Wire command: `w\w\e\x`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.settings.password_for_ap(value: str) -> Result
```
```c
ow_status ow_wireless_wifi_settings_password_for_ap(ow_device* dev, const char* value);
```
```rust
dev.wireless().wifi().settings().password_for_ap(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.settings.password_for_ap(value)   # check dev.ok
```
