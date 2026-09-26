# Websocket Server

`dev.hardware.settings_home.websocket_settings` - wire path `h\s\k` - generated from `fwMenuWebsocketSettings`.

## start_ws_server

Start WS Server. Turn the websocket server on or off

Requires power zone 5 (ESP32). See [Errors](errors.md).

Wire command: `h\s\k\r`

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.websocket_settings.start_ws_server() -> Result
```
```c
ow_status ow_hardware_settings_home_websocket_settings_start_ws_server(ow_device* dev);
```
```rust
dev.hardware().settings_home().websocket_settings().start_ws_server() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.websocket_settings.start_ws_server()   # check dev.ok
```

## w_s_server_port

WS Server Port. Set the TCP port the websocket server listens on

Wire command: `h\s\k\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.websocket_settings.w_s_server_port(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_websocket_settings_w_s_server_port(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().websocket_settings().w_s_server_port(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.websocket_settings.w_s_server_port(value)   # check dev.ok
```

## auth_mode

Auth Mode. Choose whether the websocket server allows open access or requires a username and password

Wire command: `h\s\k\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.websocket_settings.auth_mode(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_websocket_settings_auth_mode(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().websocket_settings().auth_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.websocket_settings.auth_mode(value)   # check dev.ok
```

## auth_username

Auth Username. Set the username required to connect to the websocket server when basic authentication is enabled

Wire command: `h\s\k\u`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.websocket_settings.auth_username(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_websocket_settings_auth_username(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().websocket_settings().auth_username(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.websocket_settings.auth_username(value)   # check dev.ok
```

## auth_password

Auth Password. Set the password required to connect to the websocket server when basic authentication is enabled

Wire command: `h\s\k\e`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.websocket_settings.auth_password(value: str) -> Result
```
```c
ow_status ow_hardware_settings_home_websocket_settings_auth_password(ow_device* dev, const char* value);
```
```rust
dev.hardware().settings_home().websocket_settings().auth_password(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.websocket_settings.auth_password(value)   # check dev.ok
```
