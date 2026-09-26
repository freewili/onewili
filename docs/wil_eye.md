# WILEye Functions

`dev.io.wil_eye` - wire path `i\f` - generated from `fwMenuWILEye`.

## take_picture

Take a Picture. Take a picture from WILEye and save its SD card or FREE-WILi's Files system by file name.

Wire command: `i\f\t`

| Arg | Wire type |
|---|---|
| destination | decS32 |
| filename | string |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.take_picture(destination: int, filename: str) -> Result
```
```c
ow_status ow_io_wil_eye_take_picture(ow_device* dev, int32_t destination, const char* filename);
```
```rust
dev.io().wil_eye().take_picture(destination: i32, filename: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.take_picture(destination, filename)   # check dev.ok
```

## start_recording_video

Start Recording Video. Start recording video from WILEye and save it to SD card by file name

Wire command: `i\f\v`

| Arg | Wire type |
|---|---|
| filename | string |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.start_recording_video(filename: str) -> Result
```
```c
ow_status ow_io_wil_eye_start_recording_video(ow_device* dev, const char* filename);
```
```rust
dev.io().wil_eye().start_recording_video(filename: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.start_recording_video(filename)   # check dev.ok
```

## stop_recording_video

Stop Recording Video. Stop recording video from WILEye

Wire command: `i\f\s`

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.stop_recording_video() -> Result
```
```c
ow_status ow_io_wil_eye_stop_recording_video(ow_device* dev);
```
```rust
dev.io().wil_eye().stop_recording_video() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.stop_recording_video()   # check dev.ok
```

## toggle_ai_detection_stream

Stream AI Detection Events. Stream AI Detection Events from WILEye

Wire command: `i\f\a`

| Arg | Wire type |
|---|---|
| ai_stream_mode | dec |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.toggle_ai_detection_stream(ai_stream_mode: int) -> Result
```
```c
ow_status ow_io_wil_eye_toggle_ai_detection_stream(ow_device* dev, int32_t ai_stream_mode);
```
```rust
dev.io().wil_eye().toggle_ai_detection_stream(ai_stream_mode: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.toggle_ai_detection_stream(ai_stream_mode)   # check dev.ok
```

## set_zoom_level

Set Zoom. Set the zoom level of WILEye

Wire command: `i\f\m`

| Arg | Wire type |
|---|---|
| zoom | dec |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.set_zoom_level(zoom: int) -> Result
```
```c
ow_status ow_io_wil_eye_set_zoom_level(ow_device* dev, int32_t zoom);
```
```rust
dev.io().wil_eye().set_zoom_level(zoom: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.set_zoom_level(zoom)   # check dev.ok
```

## set_contrast

Set Contrast. Set the contrast level of WILEye

Wire command: `i\f\c`

| Arg | Wire type |
|---|---|
| contrast | dec |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.set_contrast(contrast: int) -> Result
```
```c
ow_status ow_io_wil_eye_set_contrast(ow_device* dev, int32_t contrast);
```
```rust
dev.io().wil_eye().set_contrast(contrast: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.set_contrast(contrast)   # check dev.ok
```

## set_saturation

Set Saturation. Set the saturation level of WILEye

Wire command: `i\f\i`

| Arg | Wire type |
|---|---|
| saturation | dec |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.set_saturation(saturation: int) -> Result
```
```c
ow_status ow_io_wil_eye_set_saturation(ow_device* dev, int32_t saturation);
```
```rust
dev.io().wil_eye().set_saturation(saturation: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.set_saturation(saturation)   # check dev.ok
```

## set_brightness

Set Brightness. Set the brightness level of WILEye

Wire command: `i\f\b`

| Arg | Wire type |
|---|---|
| brightness | dec |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.set_brightness(brightness: int) -> Result
```
```c
ow_status ow_io_wil_eye_set_brightness(ow_device* dev, int32_t brightness);
```
```rust
dev.io().wil_eye().set_brightness(brightness: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.set_brightness(brightness)   # check dev.ok
```

## set_hue

Set Hue. Set the hue level of WILEye

Wire command: `i\f\u`

| Arg | Wire type |
|---|---|
| hue | dec |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.set_hue(hue: int) -> Result
```
```c
ow_status ow_io_wil_eye_set_hue(ow_device* dev, int32_t hue);
```
```rust
dev.io().wil_eye().set_hue(hue: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.set_hue(hue)   # check dev.ok
```

## set_resolution

Set Resolution. Set the resolution state of WILEye

Wire command: `i\f\y`

| Arg | Wire type |
|---|---|
| resolutionstate | dec |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.set_resolution(resolutionstate: int) -> Result
```
```c
ow_status ow_io_wil_eye_set_resolution(ow_device* dev, int32_t resolutionstate);
```
```rust
dev.io().wil_eye().set_resolution(resolutionstate: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.set_resolution(resolutionstate)   # check dev.ok
```

## set_flash_state

Enable Disable Flash. Set the flash state of WILEye

Wire command: `i\f\l`

| Arg | Wire type |
|---|---|
| flash | bool |

Returns: none (Ok/Err only)

```python
dev.io.wil_eye.set_flash_state(flash: bool) -> Result
```
```c
ow_status ow_io_wil_eye_set_flash_state(ow_device* dev, bool flash);
```
```rust
dev.io().wil_eye().set_flash_state(flash: bool) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.wil_eye.set_flash_state(flash)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `WILEye` (text)

DEPRECATED: never emitted; kept for wire compatibility (see WILEyeAI)

| Payload field | Wire type |
|---|---|
| data_bytes | hexbytes |

### `WILEyeImgStart` (text)

WILEye image transfer started ('Image Stream Start: N bytes')

| Payload field | Wire type |
|---|---|
| data | string |

### `WILEyeImgChunk` (text)

WILEye image chunk received ('N bytes')

| Payload field | Wire type |
|---|---|
| data | string |

### `WILEyeImgEnd` (text)

WILEye image transfer complete ('saved as: <file>')

| Payload field | Wire type |
|---|---|
| data | string |

### `WILEyeImgAbort` (text)

WILEye image transfer aborted (timeout)

| Payload field | Wire type |
|---|---|
| data | string |

### `WILEyeSDcard` (text)

WILEye SD card switched to USB mode

| Payload field | Wire type |
|---|---|
| data | string |

### `WILEyeAI` (text)

WILEye AI detection event (mode + bounding box text)

| Payload field | Wire type |
|---|---|
| data | string |

### `WILEyeUnknown` (text)

WILEye unknown message received

| Payload field | Wire type |
|---|---|
| data | string |
