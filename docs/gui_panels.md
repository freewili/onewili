# GUI Panels

`dev.gui.panels` - wire path `g\c` - generated from `fwMenuGUIPanels`.

## add_panel

Add Panel. Reinitializes the custom panel for controls.

Wire command: `g\c\a`

| Arg | Wire type |
|---|---|
| use_tile | bool |
| tile_id | decS32 |
| color | color |
| show_menu | bool |

Returns: none (Ok/Err only)

```python
dev.gui.panels.add_panel(use_tile: bool, tile_id: int, color: int | str, show_menu: bool) -> Result
```
```c
ow_status ow_gui_panels_add_panel(ow_device* dev, bool use_tile, int32_t tile_id, const char* color, bool show_menu);
```
```rust
dev.gui().panels().add_panel(use_tile: bool, tile_id: i32, color: &str, show_menu: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.panels.add_panel(use_tile, tile_id, color, show_menu)   # check dev.ok
```

## add_panel_picklist

Add Panel Picklist. Shows a panel that allows user to pick from a list.

Wire command: `g\c\b`

| Arg | Wire type |
|---|---|
| use_tile | bool |
| tile_id | decS32 |
| icon_id | decS32 |
| log_index | decS32 |
| back_color | color |
| fore_color | color |
| caption | string |

Returns: none (Ok/Err only)

```python
dev.gui.panels.add_panel_picklist(use_tile: bool, tile_id: int, icon_id: int, log_index: int, back_color: int | str, fore_color: int | str, caption: str) -> Result
```
```c
ow_status ow_gui_panels_add_panel_picklist(ow_device* dev, bool use_tile, int32_t tile_id, int32_t icon_id, int32_t log_index, const char* back_color, const char* fore_color, const char* caption);
```
```rust
dev.gui().panels().add_panel_picklist(use_tile: bool, tile_id: i32, icon_id: i32, log_index: i32, back_color: &str, fore_color: &str, caption: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.panels.add_panel_picklist(use_tile, tile_id, icon_id, log_index, back_color, fore_color, caption)   # check dev.ok
```

## show_panel

Show Panel. Brings the panel with the given index to the front on the DISPLAY.

Wire command: `g\c\c`

| Arg | Wire type |
|---|---|
| index | decS32 |

Returns: none (Ok/Err only)

```python
dev.gui.panels.show_panel(index: int) -> Result
```
```c
ow_status ow_gui_panels_show_panel(ow_device* dev, int32_t index);
```
```rust
dev.gui().panels().show_panel(index: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.panels.show_panel(index)   # check dev.ok
```

## set_menu_text

Set Menu Text. Sets a custom panel menu button label (up to 15 bytes).

Create the custom panel with ShowMenu enabled first. Button order is gray=0, yellow=1, green=2, blue=3, red=4. Labels do not assign actions; poll Read Buttons to handle presses.

Wire command: `g\c\f`

| Arg | Wire type |
|---|---|
| button | decS32 |
| text | string |

Returns: none (Ok/Err only)

```python
dev.gui.panels.set_menu_text(button: int, text: str) -> Result
```
```c
ow_status ow_gui_panels_set_menu_text(ow_device* dev, int32_t button, const char* text);
```
```rust
dev.gui().panels().set_menu_text(button: i32, text: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.panels.set_menu_text(button, text)   # check dev.ok
```

## read_buttons

Read Buttons. Returns and clears the panel and keypad button press bitmask.

Bits 0 through 4 represent gray, yellow, green, blue, and red. Keypad bits are Up=5, Down=6, Left=7, Right=8, Center=9, OK=10, X/Cancel=11, Home=12, Page=13. These are bit positions, not GUI event IDs (keypad event IDs 33 through 41 map to bits 5 through 13). Presses are latched until read, so short taps between polls are retained. Repeated presses of one button coalesce. Release and long-press events are ignored. Creating a custom panel clears the latch. This latch is shared by all API clients; use one polling consumer. Existing GUI event handling continues normally.

Wire command: `g\c\e`

Returns: pressed (hexU32)

```python
dev.gui.panels.read_buttons() -> Result
```
```c
ow_status ow_gui_panels_read_buttons(ow_device* dev, uint32_t* pressed);
```
```rust
dev.gui().panels().read_buttons() -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.gui.panels.read_buttons()   # returns value; check dev.ok
```
