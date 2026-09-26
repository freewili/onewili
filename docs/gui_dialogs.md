# Dialogs

`dev.gui.dialogs` - wire path `g\f` - generated from `fwMenuGUIDialogs`.

## message_box

Message Box. Shows a message box with optional buttons and auto close timer. 

Wire command: `g\f\a`

| Arg | Wire type |
|---|---|
| auto_close_half_sec | decS32 |
| show_ok | bool |
| show_ok_cancel | bool |
| show_none | bool |
| picture_index | decS32 |
| message | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.message_box(auto_close_half_sec: int, show_ok: bool, show_ok_cancel: bool, show_none: bool, picture_index: int, message: str) -> Result
```
```c
ow_status ow_gui_dialogs_message_box(ow_device* dev, int32_t auto_close_half_sec, bool show_ok, bool show_ok_cancel, bool show_none, int32_t picture_index, const char* message);
```
```rust
dev.gui().dialogs().message_box(auto_close_half_sec: i32, show_ok: bool, show_ok_cancel: bool, show_none: bool, picture_index: i32, message: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.message_box(auto_close_half_sec, show_ok, show_ok_cancel, show_none, picture_index, message)   # check dev.ok
```

## set_dialog_description

Set Dialog Description. Sets the description of the dialog.

Wire command: `g\f\b`

| Arg | Wire type |
|---|---|
| description | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.set_dialog_description(description: str) -> Result
```
```c
ow_status ow_gui_dialogs_set_dialog_description(ow_device* dev, const char* description);
```
```rust
dev.gui().dialogs().set_dialog_description(description: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.set_dialog_description(description)   # check dev.ok
```

## progress_bar

Progress Bar. shows a dialog with a progress bar

Wire command: `g\f\c`

| Arg | Wire type |
|---|---|
| picture_index | decS32 |
| ok_to_close | bool |
| auto_close_at100 | bool |
| auto_close_half_sec | decS32 |
| title | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.progress_bar(picture_index: int, ok_to_close: bool, auto_close_at100: bool, auto_close_half_sec: int, title: str) -> Result
```
```c
ow_status ow_gui_dialogs_progress_bar(ow_device* dev, int32_t picture_index, bool ok_to_close, bool auto_close_at100, int32_t auto_close_half_sec, const char* title);
```
```rust
dev.gui().dialogs().progress_bar(picture_index: i32, ok_to_close: bool, auto_close_at100: bool, auto_close_half_sec: i32, title: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.progress_bar(picture_index, ok_to_close, auto_close_at100, auto_close_half_sec, title)   # check dev.ok
```

## number_edit

Number Edit. Shows a dialog box to edit numbers

Wire command: `g\f\k`

| Arg | Wire type |
|---|---|
| min | decS32 |
| max | decS32 |
| initial | decS32 |
| use_min_max | bool |
| is_unsigned | bool |
| hex_fomat | bool |
| message | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.number_edit(min: int, max: int, initial: int, use_min_max: bool, is_unsigned: bool, hex_fomat: bool, message: str) -> Result
```
```c
ow_status ow_gui_dialogs_number_edit(ow_device* dev, int32_t min, int32_t max, int32_t initial, bool use_min_max, bool is_unsigned, bool hex_fomat, const char* message);
```
```rust
dev.gui().dialogs().number_edit(min: i32, max: i32, initial: i32, use_min_max: bool, is_unsigned: bool, hex_fomat: bool, message: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.number_edit(min, max, initial, use_min_max, is_unsigned, hex_fomat, message)   # check dev.ok
```

## number_edit_float

Number Edit Float. Shows a dialog to enter a float number

Wire command: `g\f\e`

| Arg | Wire type |
|---|---|
| min | float |
| max | float |
| initial | float |
| use_min_max | bool |
| digit_count | decS32 |
| message | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.number_edit_float(min: float, max: float, initial: float, use_min_max: bool, digit_count: int, message: str) -> Result
```
```c
ow_status ow_gui_dialogs_number_edit_float(ow_device* dev, double min, double max, double initial, bool use_min_max, int32_t digit_count, const char* message);
```
```rust
dev.gui().dialogs().number_edit_float(min: f64, max: f64, initial: f64, use_min_max: bool, digit_count: i32, message: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.number_edit_float(min, max, initial, use_min_max, digit_count, message)   # check dev.ok
```

## text_edit

Text Edit. Shows a dialog to edit a text value.

Wire command: `g\f\f`

| Arg | Wire type |
|---|---|
| message | string |
| inital_value | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.text_edit(message: str, inital_value: str) -> Result
```
```c
ow_status ow_gui_dialogs_text_edit(ow_device* dev, const char* message, const char* inital_value);
```
```rust
dev.gui().dialogs().text_edit(message: &str, inital_value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.text_edit(message, inital_value)   # check dev.ok
```

## pick_list

Pick List. Shows a list of items to pick from. The list of items is loaded into a log.

Wire command: `g\f\g`

| Arg | Wire type |
|---|---|
| log_index | decS32 |
| message | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.pick_list(log_index: int, message: str) -> Result
```
```c
ow_status ow_gui_dialogs_pick_list(ow_device* dev, int32_t log_index, const char* message);
```
```rust
dev.gui().dialogs().pick_list(log_index: i32, message: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.pick_list(log_index, message)   # check dev.ok
```

## show_text_editor

Show Text Editor. Shows a full screen text editor.

Wire command: `g\f\i`

| Arg | Wire type |
|---|---|
| editor_type | decS32 |
| message | string |
| inital_value | string |

Returns: basic (bool)

```python
dev.gui.dialogs.show_text_editor(editor_type: int, message: str, inital_value: str) -> Result
```
```c
ow_status ow_gui_dialogs_show_text_editor(ow_device* dev, int32_t editor_type, const char* message, const char* inital_value, bool* basic);
```
```rust
dev.gui().dialogs().show_text_editor(editor_type: i32, message: &str, inital_value: &str) -> Result<bool, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.show_text_editor(editor_type, message, inital_value)   # returns value; check dev.ok
```

## set_progess_dialog_value

Set Progess Dialog Value. Sets the value of progress on the dialog

Wire command: `g\f\j`

| Arg | Wire type |
|---|---|
| value0_to100 | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.set_progess_dialog_value(value0_to100: int) -> Result
```
```c
ow_status ow_gui_dialogs_set_progess_dialog_value(ow_device* dev, int32_t value0_to100);
```
```rust
dev.gui().dialogs().set_progess_dialog_value(value0_to100: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.set_progess_dialog_value(value0_to100)   # check dev.ok
```

## file_picker

File Picker. Shows a full screen file browser dialog over the current view. The chosen path (or cancel) returns as a filepicked event.

Wire command: `g\f\l`

| Arg | Wire type |
|---|---|
| mode | decU32 |
| start_path | string |
| filter | string |

Returns: none (Ok/Err only)

```python
dev.gui.dialogs.file_picker(mode: int, start_path: str, filter: str) -> Result
```
```c
ow_status ow_gui_dialogs_file_picker(ow_device* dev, int32_t mode, const char* start_path, const char* filter);
```
```rust
dev.gui().dialogs().file_picker(mode: i32, start_path: &str, filter: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.dialogs.file_picker(mode, start_path, filter)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `filepicked` (text)

File list / file picker result

| Payload field | Wire type |
|---|---|
| panel | decS32 |
| control | decS32 |
| picked | bool |
| path | string |
