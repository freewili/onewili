# rThon Debug

`dev.scripting.rthon_debug` - wire path `s\r` - generated from `fwMenuRthonDebug`.

## debug_start

Debug Start. Loads and compiles a script for debugging.

Wire command: `s\r\c`

| Arg | Wire type |
|---|---|
| path | str |

Returns: none (Ok/Err only)

```python
dev.scripting.rthon_debug.debug_start(path: str) -> Result
```
```c
ow_status ow_scripting_rthon_debug_debug_start(ow_device* dev, const char* path);
```
```rust
dev.scripting().rthon_debug().debug_start(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.rthon_debug.debug_start(path)   # check dev.ok
```

## debug_breakpoints

Debug Breakpoints. Replaces the breakpoint set for the active debug session.

Wire command: `s\r\j`

| Arg | Wire type |
|---|---|
| lines | str |

Returns: none (Ok/Err only)

```python
dev.scripting.rthon_debug.debug_breakpoints(lines: str) -> Result
```
```c
ow_status ow_scripting_rthon_debug_debug_breakpoints(ow_device* dev, const char* lines);
```
```rust
dev.scripting().rthon_debug().debug_breakpoints(lines: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.rthon_debug.debug_breakpoints(lines)   # check dev.ok
```

## debug_step

Debug Step. Single-steps the active debug session.

Wire command: `s\r\e`

Returns: none (Ok/Err only)

```python
dev.scripting.rthon_debug.debug_step() -> Result
```
```c
ow_status ow_scripting_rthon_debug_debug_step(ow_device* dev);
```
```rust
dev.scripting().rthon_debug().debug_step() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.rthon_debug.debug_step()   # check dev.ok
```

## debug_continue

Debug Continue. Resumes the active debug session until the next breakpoint.

Wire command: `s\r\f`

Returns: none (Ok/Err only)

```python
dev.scripting.rthon_debug.debug_continue() -> Result
```
```c
ow_status ow_scripting_rthon_debug_debug_continue(ow_device* dev);
```
```rust
dev.scripting().rthon_debug().debug_continue() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.rthon_debug.debug_continue()   # check dev.ok
```

## debug_pause

Debug Pause. Pauses the active debug session at the next statement.

Wire command: `s\r\g`

Returns: none (Ok/Err only)

```python
dev.scripting.rthon_debug.debug_pause() -> Result
```
```c
ow_status ow_scripting_rthon_debug_debug_pause(ow_device* dev);
```
```rust
dev.scripting().rthon_debug().debug_pause() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.rthon_debug.debug_pause()   # check dev.ok
```

## debug_stop

Debug Stop. Stops the active debug session.

Wire command: `s\r\t`

Returns: none (Ok/Err only)

```python
dev.scripting.rthon_debug.debug_stop() -> Result
```
```c
ow_status ow_scripting_rthon_debug_debug_stop(ow_device* dev);
```
```rust
dev.scripting().rthon_debug().debug_stop() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.rthon_debug.debug_stop()   # check dev.ok
```

## debug_locals

Debug Locals. Dumps the local variables of the active debug session.

Wire command: `s\r\i`

Returns: none (Ok/Err only)

```python
dev.scripting.rthon_debug.debug_locals() -> Result
```
```c
ow_status ow_scripting_rthon_debug_debug_locals(ow_device* dev);
```
```rust
dev.scripting().rthon_debug().debug_locals() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.rthon_debug.debug_locals()   # check dev.ok
```
