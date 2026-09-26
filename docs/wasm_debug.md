# WASM Debug

`dev.scripting.wasm_debug` - wire path `s\w` - generated from `fwMenuWasmDebug`.

## debug_start

WASM Debug Start. Loads a .wilwasm for debugging.

Wire command: `s\w\c`

| Arg | Wire type |
|---|---|
| path | string |

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_start(path: str) -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_start(ow_device* dev, const char* path);
```
```rust
dev.scripting().wasm_debug().debug_start(path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_start(path)   # check dev.ok
```

## debug_breakpoints

WASM Debug Breakpoints. Replaces the byte-PC breakpoint set.

Wire command: `s\w\j`

| Arg | Wire type |
|---|---|
| pcs | string |

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_breakpoints(pcs: str) -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_breakpoints(ow_device* dev, const char* pcs);
```
```rust
dev.scripting().wasm_debug().debug_breakpoints(pcs: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_breakpoints(pcs)   # check dev.ok
```

## debug_step

WASM Debug Step. Steps one opcode, or until the PC leaves [lo,hi).

Wire command: `s\w\e`

| Arg | Wire type |
|---|---|
| range | string |

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_step(range: str) -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_step(ow_device* dev, const char* range);
```
```rust
dev.scripting().wasm_debug().debug_step(range: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_step(range)   # check dev.ok
```

## debug_continue

WASM Debug Continue. Resumes until the next breakpoint.

Wire command: `s\w\f`

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_continue() -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_continue(ow_device* dev);
```
```rust
dev.scripting().wasm_debug().debug_continue() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_continue()   # check dev.ok
```

## debug_pause

WASM Debug Pause. Pauses at the next opcode.

Wire command: `s\w\g`

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_pause() -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_pause(ow_device* dev);
```
```rust
dev.scripting().wasm_debug().debug_pause() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_pause()   # check dev.ok
```

## debug_stop

WASM Debug Stop. Stops the active wasm debug session.

Wire command: `s\w\t`

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_stop() -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_stop(ow_device* dev);
```
```rust
dev.scripting().wasm_debug().debug_stop() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_stop()   # check dev.ok
```

## debug_locals

WASM Debug Locals. Dumps stack frames and raw frame-0 locals.

Wire command: `s\w\i`

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_locals() -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_locals(ow_device* dev);
```
```rust
dev.scripting().wasm_debug().debug_locals() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_locals()   # check dev.ok
```

## debug_mem_read

WASM Debug Memory Read. Reads up to 64 bytes of wasm linear memory (hex).

Wire command: `s\w\r`

| Arg | Wire type |
|---|---|
| addr | string |

Returns: none (Ok/Err only)

```python
dev.scripting.wasm_debug.debug_mem_read(addr: str) -> Result
```
```c
ow_status ow_scripting_wasm_debug_debug_mem_read(ow_device* dev, const char* addr);
```
```rust
dev.scripting().wasm_debug().debug_mem_read(addr: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wasm_debug.debug_mem_read(addr)   # check dev.ok
```
