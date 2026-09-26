# Wifi Functions

`dev.wireless.wifi` - wire path `w\w` - generated from `fwMenuWifi`.

## toggle_events

Enable Wifi Events. Toggle Wifi Event Streaming

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\r`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.toggle_events() -> Result
```
```c
ow_status ow_wireless_wifi_toggle_events(ow_device* dev);
```
```rust
dev.wireless().wifi().toggle_events() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.toggle_events()   # check dev.ok
```

## on_start_access_point

Start Access Point. Starts up Access Point with provided SSID and Password

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\a`

| Arg | Wire type |
|---|---|
| ssid | string |
| password | string |
| authmode | decS32 |
| hidessid | bool |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_start_access_point(ssid: str, password: str, authmode: int, hidessid: bool) -> Result
```
```c
ow_status ow_wireless_wifi_on_start_access_point(ow_device* dev, const char* ssid, const char* password, int32_t authmode, bool hidessid);
```
```rust
dev.wireless().wifi().on_start_access_point(ssid: &str, password: &str, authmode: i32, hidessid: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_start_access_point(ssid, password, authmode, hidessid)   # check dev.ok
```

## on_discconect_from_station

Stop Access Point. Turns off Access Point

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\t`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_discconect_from_station() -> Result
```
```c
ow_status ow_wireless_wifi_on_discconect_from_station(ow_device* dev);
```
```rust
dev.wireless().wifi().on_discconect_from_station() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_discconect_from_station()   # check dev.ok
```

## get_connected_devices

Get Stations connected to AP. Turns off Access Point

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\g`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.get_connected_devices() -> Result
```
```c
ow_status ow_wireless_wifi_get_connected_devices(ow_device* dev);
```
```rust
dev.wireless().wifi().get_connected_devices() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.get_connected_devices()   # check dev.ok
```

## on_connect_to_station

Connect to a Wifi Access Point. Connect to a WAP with provided SSID and Password

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\c`

| Arg | Wire type |
|---|---|
| ssid | string |
| password | string |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_connect_to_station(ssid: str, password: str) -> Result
```
```c
ow_status ow_wireless_wifi_on_connect_to_station(ow_device* dev, const char* ssid, const char* password);
```
```rust
dev.wireless().wifi().on_connect_to_station(ssid: &str, password: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_connect_to_station(ssid, password)   # check dev.ok
```

## on_discconect_from_station_2

Disconnect From Wifi Access Point. Disconnect from Wifi Stations

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\f`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_discconect_from_station_2() -> Result
```
```c
ow_status ow_wireless_wifi_on_discconect_from_station_2(ow_device* dev);
```
```rust
dev.wireless().wifi().on_discconect_from_station_2() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_discconect_from_station_2()   # check dev.ok
```

## on_scan_for_access_points

Scan for Access Points. Scans for available WIFI networks

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\s`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_scan_for_access_points() -> Result
```
```c
ow_status ow_wireless_wifi_on_scan_for_access_points(ow_device* dev);
```
```rust
dev.wireless().wifi().on_scan_for_access_points() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_scan_for_access_points()   # check dev.ok
```

## on_get_wif_info

Print out Wifi Info. Scans for available Wifi networks

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\p`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_get_wif_info() -> Result
```
```c
ow_status ow_wireless_wifi_on_get_wif_info(ow_device* dev);
```
```rust
dev.wireless().wifi().on_get_wif_info() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_get_wif_info()   # check dev.ok
```

## on_http_get_to_sd

Download To SDCard. HTTP GET a URL and write it to a file on the SD card

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\l`

| Arg | Wire type |
|---|---|
| url | string |
| path | string |

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_http_get_to_sd(url: str, path: str) -> Result
```
```c
ow_status ow_wireless_wifi_on_http_get_to_sd(ow_device* dev, const char* url, const char* path);
```
```rust
dev.wireless().wifi().on_http_get_to_sd(url: &str, path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_http_get_to_sd(url, path)   # check dev.ok
```

## on_http_get_abort

Cancel Download. Stops a download started with Download To SDCard

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `w\w\x`

Returns: none (Ok/Err only)

```python
dev.wireless.wifi.on_http_get_abort() -> Result
```
```c
ow_status ow_wireless_wifi_on_http_get_abort(ow_device* dev);
```
```rust
dev.wireless().wifi().on_http_get_abort() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.wireless.wifi.on_http_get_abort()   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `wifistaInfo` (text)

Station IP configuration (got IP)

| Payload field | Wire type |
|---|---|
| ip | string |
| gateway | string |
| mask | string |

### `wifiapInfo` (text)

Access point IP configuration

| Payload field | Wire type |
|---|---|
| ip | string |
| gateway | string |
| mask | string |

### `wifiscan` (text)

Wifi scan record (SSID last so consumers can bounded-split)

| Payload field | Wire type |
|---|---|
| bssid | string |
| rssi | decS32 |
| channel | decU32 |
| band | decU32 |
| authmode | decU32 |
| ssid | string |

### `wifiapdevcon` (text)

Device connected to the access point

| Payload field | Wire type |
|---|---|
| ip | string |
| mac | string |

### `wifiapdevdc` (text)

Device disconnected from the access point

| Payload field | Wire type |
|---|---|
| mac | string |

### `wsclientcon` (text)

Websocket client connected

| Payload field | Wire type |
|---|---|
| ip | string |

### `wsclientdc` (text)

Websocket client disconnected

| Payload field | Wire type |
|---|---|
| ip | string |

### `wifistations` (text)

Connected access-point station record (one event per device)

| Payload field | Wire type |
|---|---|
| ip | string |
| mac | string |

### `httpget` (text)

Download progress; total is 0 when the server sent no length

| Payload field | Wire type |
|---|---|
| written | decU32 |
| total | decU32 |

### `httpgetdone` (text)

Download finished; result 0 is success, otherwise a bnose_http_result_t

| Payload field | Wire type |
|---|---|
| result | decU32 |
| status | decU32 |
| written | decU32 |

## Sub-menus

- [Wifi Settings](wifi_settings_2.md) - `dev.wireless.wifi.settings`
