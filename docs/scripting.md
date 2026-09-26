# Scripting Functions

`dev.scripting` - wire path `s` - generated from `fwMenuScripting`.

## launch_script

Launch Script. Not yet implemented; always reports failure

Wire command: `s\a`

Returns: none (Ok/Err only)

```python
dev.scripting.launch_script() -> Result
```
```c
ow_status ow_scripting_launch_script(ow_device* dev);
```
```rust
dev.scripting().launch_script() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.launch_script()   # check dev.ok
```

## power_cycle_debugger

Power Cycle Debugger. Powers debugger zone 16 off for 500 ms, then powers it back on.

Wire command: `s\c`

Returns: none (Ok/Err only)

```python
dev.scripting.power_cycle_debugger() -> Result
```
```c
ow_status ow_scripting_power_cycle_debugger(ow_device* dev);
```
```rust
dev.scripting().power_cycle_debugger() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.power_cycle_debugger()   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `script` (text)

Script engine output / status line (wasm and rThon runners)

| Payload field | Wire type |
|---|---|
| data | string |

## Sub-menus

- [App Signals](app_signals.md) - `dev.scripting.app_signals`
- [Wili Files](wili_files.md) - `dev.scripting.wili_files`
- [ZoomIO Functions](zoom_io.md) - `dev.scripting.zoom_io`
- [WASM Debug](wasm_debug.md) - `dev.scripting.wasm_debug`
- [rThon Debug](rthon_debug.md) - `dev.scripting.rthon_debug`
