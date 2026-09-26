# Wili Files

`dev.scripting.wili_files` - wire path `s\f` - generated from `fwMenuWiliFiles`.

## wili_load

Load. Loads a .wili project.

Wire command: `s\f\l`

| Arg | Wire type |
|---|---|
| filepath | string |

Returns: none (Ok/Err only)

```python
dev.scripting.wili_files.wili_load(filepath: str) -> Result
```
```c
ow_status ow_scripting_wili_files_wili_load(ow_device* dev, const char* filepath);
```
```rust
dev.scripting().wili_files().wili_load(filepath: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wili_files.wili_load(filepath)   # check dev.ok
```

## wili_save

Save Current. Saves the current Wili project to its source path.

Wire command: `s\f\s`

Returns: none (Ok/Err only)

```python
dev.scripting.wili_files.wili_save() -> Result
```
```c
ow_status ow_scripting_wili_files_wili_save(ow_device* dev);
```
```rust
dev.scripting().wili_files().wili_save() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wili_files.wili_save()   # check dev.ok
```

## wili_reset

Reset. Clears the live panels, Wili Blocks, and app signals.

Wire command: `s\f\r`

Returns: none (Ok/Err only)

```python
dev.scripting.wili_files.wili_reset() -> Result
```
```c
ow_status ow_scripting_wili_files_wili_reset(ow_device* dev);
```
```rust
dev.scripting().wili_files().wili_reset() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wili_files.wili_reset()   # check dev.ok
```

## wili_default

Make Default. Sets the Wili project loaded at boot.

Wire command: `s\f\m`

| Arg | Wire type |
|---|---|
| filepath | string |

Returns: none (Ok/Err only)

```python
dev.scripting.wili_files.wili_default(filepath: str) -> Result
```
```c
ow_status ow_scripting_wili_files_wili_default(ow_device* dev, const char* filepath);
```
```rust
dev.scripting().wili_files().wili_default(filepath: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wili_files.wili_default(filepath)   # check dev.ok
```

## wili_remove_default

Remove Default. Removes the configured boot Wili project.

Wire command: `s\f\x`

Returns: none (Ok/Err only)

```python
dev.scripting.wili_files.wili_remove_default() -> Result
```
```c
ow_status ow_scripting_wili_files_wili_remove_default(ow_device* dev);
```
```rust
dev.scripting().wili_files().wili_remove_default() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.scripting.wili_files.wili_remove_default()   # check dev.ok
```
