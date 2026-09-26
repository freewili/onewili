# GUI Controls

`dev.gui.controls` - wire path `g\b` - generated from `fwMenuGUIControls`.

## add_led

Add LED. Add a LED control to the panel.

Wire command: `g\b\a`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| color | decS32 |
| size | decS32 |
| inital_value | bool |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_led(index: int, x: int, y: int, color: int, size: int, inital_value: bool) -> Result
```
```c
ow_status ow_gui_controls_add_led(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t color, int32_t size, bool inital_value);
```
```rust
dev.gui().controls().add_led(index: i32, x: i32, y: i32, color: i32, size: i32, inital_value: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_led(index, x, y, color, size, inital_value)   # check dev.ok
```

## add_log_list

Add LogList. Adds a Log control or a list control to the panel.

Wire command: `g\b\b`

| Arg | Wire type |
|---|---|
| index | decS32 |
| log | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| font_type | decS32 |
| font_size | decS32 |
| back_color | color |
| fore_color | color |
| list_mode | bool |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_log_list(index: int, log: int, x: int, y: int, width: int, height: int, font_type: int, font_size: int, back_color: int | str, fore_color: int | str, list_mode: bool) -> Result
```
```c
ow_status ow_gui_controls_add_log_list(ow_device* dev, int32_t index, int32_t log, int32_t x, int32_t y, int32_t width, int32_t height, int32_t font_type, int32_t font_size, const char* back_color, const char* fore_color, bool list_mode);
```
```rust
dev.gui().controls().add_log_list(index: i32, log: i32, x: i32, y: i32, width: i32, height: i32, font_type: i32, font_size: i32, back_color: &str, fore_color: &str, list_mode: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_log_list(index, log, x, y, width, height, font_type, font_size, back_color, fore_color, list_mode)   # check dev.ok
```

## add_plot

Add Plot. Adds a plot to the panel.

Wire command: `g\b\c`

| Arg | Wire type |
|---|---|
| index | decS32 |
| plot_data_index_bit_field | decU32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| min_y | decS32 |
| max_y | decS32 |
| back_color | color |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_plot(index: int, plot_data_index_bit_field: int, x: int, y: int, width: int, height: int, min_y: int, max_y: int, back_color: int | str) -> Result
```
```c
ow_status ow_gui_controls_add_plot(ow_device* dev, int32_t index, int32_t plot_data_index_bit_field, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min_y, int32_t max_y, const char* back_color);
```
```rust
dev.gui().controls().add_plot(index: i32, plot_data_index_bit_field: i32, x: i32, y: i32, width: i32, height: i32, min_y: i32, max_y: i32, back_color: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_plot(index, plot_data_index_bit_field, x, y, width, height, min_y, max_y, back_color)   # check dev.ok
```

## add_number

Add Number. add a numeric control to a panel

Wire command: `g\b\l`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| font_type | decS32 |
| font_size | decS32 |
| fore_color | color |
| back_color | color |
| is_float | bool |
| float_digit_count | decS32 |
| is_hex_format | bool |
| is_unsigned | bool |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_number(index: int, x: int, y: int, width: int, font_type: int, font_size: int, fore_color: int | str, back_color: int | str, is_float: bool, float_digit_count: int, is_hex_format: bool, is_unsigned: bool) -> Result
```
```c
ow_status ow_gui_controls_add_number(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, bool is_float, int32_t float_digit_count, bool is_hex_format, bool is_unsigned);
```
```rust
dev.gui().controls().add_number(index: i32, x: i32, y: i32, width: i32, font_type: i32, font_size: i32, fore_color: &str, back_color: &str, is_float: bool, float_digit_count: i32, is_hex_format: bool, is_unsigned: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_number(index, x, y, width, font_type, font_size, fore_color, back_color, is_float, float_digit_count, is_hex_format, is_unsigned)   # check dev.ok
```

## add_text

Add Text. Add static text to the panel

Wire command: `g\b\e`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| font_type | decS32 |
| font_size | decS32 |
| fore_color | color |
| back_color | color |
| text | string |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_text(index: int, x: int, y: int, font_type: int, font_size: int, fore_color: int | str, back_color: int | str, text: str) -> Result
```
```c
ow_status ow_gui_controls_add_text(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, const char* text);
```
```rust
dev.gui().controls().add_text(index: i32, x: i32, y: i32, font_type: i32, font_size: i32, fore_color: &str, back_color: &str, text: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_text(index, x, y, font_type, font_size, fore_color, back_color, text)   # check dev.ok
```

## add_bargraph

Add Bargraph. Add a bar graph to a panel.

Wire command: `g\b\f`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| min | decS32 |
| max | decS32 |
| bar_color | color |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_bargraph(index: int, x: int, y: int, width: int, height: int, min: int, max: int, bar_color: int | str) -> Result
```
```c
ow_status ow_gui_controls_add_bargraph(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* bar_color);
```
```rust
dev.gui().controls().add_bargraph(index: i32, x: i32, y: i32, width: i32, height: i32, min: i32, max: i32, bar_color: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_bargraph(index, x, y, width, height, min, max, bar_color)   # check dev.ok
```

## add_meter

Add Meter. Add a Meter control to a panel

Wire command: `g\b\g`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| min | decS32 |
| max | decS32 |
| needle_color | color |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_meter(index: int, x: int, y: int, width: int, height: int, min: int, max: int, needle_color: int | str) -> Result
```
```c
ow_status ow_gui_controls_add_meter(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* needle_color);
```
```rust
dev.gui().controls().add_meter(index: i32, x: i32, y: i32, width: i32, height: i32, min: i32, max: i32, needle_color: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_meter(index, x, y, width, height, min, max, needle_color)   # check dev.ok
```

## add_button

Add Button. Add a button control to a panel

Wire command: `g\b\i`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| fore_color | color |
| back_color | color |
| text | string |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_button(index: int, x: int, y: int, width: int, height: int, fore_color: int | str, back_color: int | str, text: str) -> Result
```
```c
ow_status ow_gui_controls_add_button(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, const char* fore_color, const char* back_color, const char* text);
```
```rust
dev.gui().controls().add_button(index: i32, x: i32, y: i32, width: i32, height: i32, fore_color: &str, back_color: &str, text: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_button(index, x, y, width, height, fore_color, back_color, text)   # check dev.ok
```

## add_picture

Add Picture. Shows a ROM picture on the panel.

Wire command: `g\b\j`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| picture_id | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_picture(index: int, x: int, y: int, picture_id: int) -> Result
```
```c
ow_status ow_gui_controls_add_picture(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t picture_id);
```
```rust
dev.gui().controls().add_picture(index: i32, x: i32, y: i32, picture_id: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_picture(index, x, y, picture_id)   # check dev.ok
```

## add_picture_from_file

Add Picture From File. Loads a picture from the file system

Wire command: `g\b\k`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| picture_path | string |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_picture_from_file(index: int, x: int, y: int, picture_path: str) -> Result
```
```c
ow_status ow_gui_controls_add_picture_from_file(ow_device* dev, int32_t index, int32_t x, int32_t y, const char* picture_path);
```
```rust
dev.gui().controls().add_picture_from_file(index: i32, x: i32, y: i32, picture_path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_picture_from_file(index, x, y, picture_path)   # check dev.ok
```

## add_waterfall

Add Waterfall. Adds an FFT waterfall (spectrogram) control to the panel. Rows commit when the control value changes.

Wire command: `g\b\m`

| Arg | Wire type |
|---|---|
| index | decS32 |
| plot_data_index | decS32 |
| bin_count | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| back_color | color |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_waterfall(index: int, plot_data_index: int, bin_count: int, x: int, y: int, width: int, height: int, back_color: int | str) -> Result
```
```c
ow_status ow_gui_controls_add_waterfall(ow_device* dev, int32_t index, int32_t plot_data_index, int32_t bin_count, int32_t x, int32_t y, int32_t width, int32_t height, const char* back_color);
```
```rust
dev.gui().controls().add_waterfall(index: i32, plot_data_index: i32, bin_count: i32, x: i32, y: i32, width: i32, height: i32, back_color: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_waterfall(index, plot_data_index, bin_count, x, y, width, height, back_color)   # check dev.ok
```

## add_wili8

Add Wili8. Adds a clipped, integer-scaled Wili8 canvas control. Animation 0 is Wave; 255 stores ScriptPath for future custom execution.

Wire command: `g\b\n`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| scale | decS32 |
| back_color | color |
| animation | decU32 |
| script_path | string |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_wili8(index: int, x: int, y: int, width: int, height: int, scale: int, back_color: int | str, animation: int, script_path: str) -> Result
```
```c
ow_status ow_gui_controls_add_wili8(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t scale, const char* back_color, int32_t animation, const char* script_path);
```
```rust
dev.gui().controls().add_wili8(index: i32, x: i32, y: i32, width: i32, height: i32, scale: i32, back_color: &str, animation: i32, script_path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_wili8(index, x, y, width, height, scale, back_color, animation, script_path)   # check dev.ok
```

## add_file_list

Add File List. Adds a device-fed SD/flash file browser control. Activating a file (or OK in pick dir mode) raises a filepicked event with the full path.

Wire command: `g\b\o`

| Arg | Wire type |
|---|---|
| index | decS32 |
| x | decS32 |
| y | decS32 |
| width | decS32 |
| height | decS32 |
| mode | decU32 |
| back_color | color |
| start_path | string |
| filter | string |

Returns: none (Ok/Err only)

```python
dev.gui.controls.add_file_list(index: int, x: int, y: int, width: int, height: int, mode: int, back_color: int | str, start_path: str, filter: str) -> Result
```
```c
ow_status ow_gui_controls_add_file_list(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t mode, const char* back_color, const char* start_path, const char* filter);
```
```rust
dev.gui().controls().add_file_list(index: i32, x: i32, y: i32, width: i32, height: i32, mode: i32, back_color: &str, start_path: &str, filter: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.controls.add_file_list(index, x, y, width, height, mode, back_color, start_path, filter)   # check dev.ok
```
