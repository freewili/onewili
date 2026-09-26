# Apps functions

`dev.apps` - wire path `a` - generated from `fwMenuApps`.

## launch_app

Launch App. Switch the built-in display to the app with the given app ID

Wire command: `a\a`

| Arg | Wire type |
|---|---|
| app_id | decS32 |

Returns: none (Ok/Err only)

```python
dev.apps.launch_app(app_id: int) -> Result
```
```c
ow_status ow_apps_launch_app(ow_device* dev, int32_t app_id);
```
```rust
dev.apps().launch_app(app_id: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.apps.launch_app(app_id)   # check dev.ok
```

## run_app

Run App. Runs /apps/<filename> on the display processor. The destination is inferred by reading the image, not the name: a UF2 whose blocks target SRAM is staged in RAM and launched; one targeting the PSRAM window (0x11000000) is staged into PSRAM through the loader stub and launched; anything else is written to flash. RAM and PSRAM launches leave flash untouched. A flash load takes 30-60 seconds with the screen blank.

Wire command: `a\r`

| Arg | Wire type |
|---|---|
| filename | str |

Returns: none (Ok/Err only)

```python
dev.apps.run_app(filename: str) -> Result
```
```c
ow_status ow_apps_run_app(ow_device* dev, const char* filename);
```
```rust
dev.apps().run_app(filename: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.apps.run_app(filename)   # check dev.ok
```
