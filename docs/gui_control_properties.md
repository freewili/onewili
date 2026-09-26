# GUI Control Properties

`dev.gui.control_properties` - wire path `g\e` - generated from `fwMenuGUIControlProperties`.

## set_control_value_text

Set Control Value Text. sets the text value of a control

Wire command: `g\e\a`

| Arg | Wire type |
|---|---|
| index | decS32 |
| text | string |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_control_value_text(index: int, text: str) -> Result
```
```c
ow_status ow_gui_control_properties_set_control_value_text(ow_device* dev, int32_t index, const char* text);
```
```rust
dev.gui().control_properties().set_control_value_text(index: i32, text: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_control_value_text(index, text)   # check dev.ok
```

## set_control_value_int

Set Control Value Int. Set the text value of a control

Wire command: `g\e\b`

| Arg | Wire type |
|---|---|
| index | decS32 |
| value | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_control_value_int(index: int, value: int) -> Result
```
```c
ow_status ow_gui_control_properties_set_control_value_int(ow_device* dev, int32_t index, int32_t value);
```
```rust
dev.gui().control_properties().set_control_value_int(index: i32, value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_control_value_int(index, value)   # check dev.ok
```

## set_control_value_float

Set Control Value Float. Set the float value of the control.

Wire command: `g\e\c`

| Arg | Wire type |
|---|---|
| index | decS32 |
| value | float |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_control_value_float(index: int, value: float) -> Result
```
```c
ow_status ow_gui_control_properties_set_control_value_float(ow_device* dev, int32_t index, double value);
```
```rust
dev.gui().control_properties().set_control_value_float(index: i32, value: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_control_value_float(index, value)   # check dev.ok
```

## set_list_item_text

Set List Item Text. Sets the text and color of a specific list item

Wire command: `g\e\k`

| Arg | Wire type |
|---|---|
| log_index | decS32 |
| list_item | decS32 |
| color | decS32 |
| text | string |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_list_item_text(log_index: int, list_item: int, color: int, text: str) -> Result
```
```c
ow_status ow_gui_control_properties_set_list_item_text(ow_device* dev, int32_t log_index, int32_t list_item, int32_t color, const char* text);
```
```rust
dev.gui().control_properties().set_list_item_text(log_index: i32, list_item: i32, color: i32, text: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_list_item_text(log_index, list_item, color, text)   # check dev.ok
```

## set_control_value_min_max_int

Set Control Value Min Max Int. Sets whether a min and max is applied to a controls value

Wire command: `g\e\e`

| Arg | Wire type |
|---|---|
| index | decS32 |
| enable | bool |
| min | decS32 |
| max | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_control_value_min_max_int(index: int, enable: bool, min: int, max: int) -> Result
```
```c
ow_status ow_gui_control_properties_set_control_value_min_max_int(ow_device* dev, int32_t index, bool enable, int32_t min, int32_t max);
```
```rust
dev.gui().control_properties().set_control_value_min_max_int(index: i32, enable: bool, min: i32, max: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_control_value_min_max_int(index, enable, min, max)   # check dev.ok
```

## set_control_value_min_max_float

Set Control Value Min Max Float. Sets whether a min and max is applied to a controls value

Wire command: `g\e\l`

| Arg | Wire type |
|---|---|
| index | decS32 |
| enable | bool |
| min | float |
| max | float |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_control_value_min_max_float(index: int, enable: bool, min: float, max: float) -> Result
```
```c
ow_status ow_gui_control_properties_set_control_value_min_max_float(ow_device* dev, int32_t index, bool enable, double min, double max);
```
```rust
dev.gui().control_properties().set_control_value_min_max_float(index: i32, enable: bool, min: f64, max: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_control_value_min_max_float(index, enable, min, max)   # check dev.ok
```

## set_plot_data

Set Plot Data. This adds data to a plot

Wire command: `g\e\f`

| Arg | Wire type |
|---|---|
| plot_data_index | decS32 |
| settings | decS32 |
| value | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_plot_data(plot_data_index: int, settings: int, value: int) -> Result
```
```c
ow_status ow_gui_control_properties_set_plot_data(ow_device* dev, int32_t plot_data_index, int32_t settings, int32_t value);
```
```rust
dev.gui().control_properties().set_plot_data(plot_data_index: i32, settings: i32, value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_plot_data(plot_data_index, settings, value)   # check dev.ok
```

## set_list_item_selected

Set List Item Selected. This sets which item in a list is selected.

Wire command: `g\e\g`

| Arg | Wire type |
|---|---|
| log_index | decS32 |
| list_index | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_list_item_selected(log_index: int, list_index: int) -> Result
```
```c
ow_status ow_gui_control_properties_set_list_item_selected(ow_device* dev, int32_t log_index, int32_t list_index);
```
```rust
dev.gui().control_properties().set_list_item_selected(log_index: i32, list_index: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_list_item_selected(log_index, list_index)   # check dev.ok
```

## set_list_item_top_index

Set List Item Top Index. This sets the first viewable item in the list. 

Wire command: `g\e\i`

| Arg | Wire type |
|---|---|
| log_item | decS32 |
| list_index | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_list_item_top_index(log_item: int, list_index: int) -> Result
```
```c
ow_status ow_gui_control_properties_set_list_item_top_index(ow_device* dev, int32_t log_item, int32_t list_index);
```
```rust
dev.gui().control_properties().set_list_item_top_index(log_item: i32, list_index: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_list_item_top_index(log_item, list_index)   # check dev.ok
```

## set_control_property

Set Control Property. Sets a property based on a property type index

Wire command: `g\e\j`

| Arg | Wire type |
|---|---|
| index | decS32 |
| property | decS32 |
| value | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.control_properties.set_control_property(index: int, property: int, value: int) -> Result
```
```c
ow_status ow_gui_control_properties_set_control_property(ow_device* dev, int32_t index, int32_t property, int32_t value);
```
```rust
dev.gui().control_properties().set_control_property(index: i32, property: i32, value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.control_properties.set_control_property(index, property, value)   # check dev.ok
```
