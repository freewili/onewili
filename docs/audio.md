# Audio Functions

`dev.io.audio` - wire path `i\k` - generated from `fwMenuAudio`.

## play_audio_file

Play Audio File. Plays a .wav file from the sounds directory.

Requires power zone 3 (Audio). See [Errors](errors.md).

##### Play Audio File

Plays a `.wav` file from the device's `sounds/` directory through the built‑in speaker (or headphone jack, if connected).

###### Argument
- `filePath` *(string)* — name or path of the file to play.
  - If no directory is given, `sounds/` is assumed.
  - If no extension is given, `.wav` is appended.
  - The file must already exist on the device's filesystem.

###### Returns
- `success` *(basic)* — `true` if playback started, `false` if the file was not found or could not be opened.

###### Examples
```
chime
chime.wav
\sounds\chime.wav
```

###### Notes
- Only PCM `.wav` files are supported.
- Playback is asynchronous — the call returns as soon as playback begins.
- Use **Stream Audio** (`s`) to mirror audio to the host, or **Record Audio** (`r`) to capture from the microphone.

Wire command: `i\k\f`

| Arg | Wire type |
|---|---|
| file_path | string |

Returns: none (Ok/Err only)

```python
dev.io.audio.play_audio_file(file_path: str) -> Result
```
```c
ow_status ow_io_audio_play_audio_file(ow_device* dev, const char* file_path);
```
```rust
dev.io().audio().play_audio_file(file_path: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.audio.play_audio_file(file_path)   # check dev.ok
```

## record_audio_file

Record Audio. Records audio to a file (blank name = auto-named).

Requires power zone 3 (Audio). See [Errors](errors.md).

Wire command: `i\k\r`

| Arg | Wire type |
|---|---|
| file_name | string |

Returns: none (Ok/Err only)

```python
dev.io.audio.record_audio_file(file_name: str) -> Result
```
```c
ow_status ow_io_audio_record_audio_file(ow_device* dev, const char* file_name);
```
```rust
dev.io().audio().record_audio_file(file_name: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.audio.record_audio_file(file_name)   # check dev.ok
```

## play_audio_asset

Play Audio Asset. Plays a built-in audio asset by index or name.

Requires power zone 3 (Audio). See [Errors](errors.md).

Wire command: `i\k\a`

| Arg | Wire type |
|---|---|
| asset_name | string |

Returns: none (Ok/Err only)

```python
dev.io.audio.play_audio_asset(asset_name: str) -> Result
```
```c
ow_status ow_io_audio_play_audio_asset(ow_device* dev, const char* asset_name);
```
```rust
dev.io().audio().play_audio_asset(asset_name: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.audio.play_audio_asset(asset_name)   # check dev.ok
```

## enable_audio_stream

Stream Audio. Enables or disables audio streaming to the host.

Requires power zone 3 (Audio). See [Errors](errors.md).

Wire command: `i\k\s`

| Arg | Wire type |
|---|---|
| enable | decS32 |

Returns: none (Ok/Err only)

```python
dev.io.audio.enable_audio_stream(enable: int) -> Result
```
```c
ow_status ow_io_audio_enable_audio_stream(ow_device* dev, int32_t enable);
```
```rust
dev.io().audio().enable_audio_stream(enable: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.audio.enable_audio_stream(enable)   # check dev.ok
```

## numbers_to_speech

Numbers to Speech. Speaks the given number aloud.

Requires power zone 3 (Audio). See [Errors](errors.md).

Wire command: `i\k\n`

| Arg | Wire type |
|---|---|
| number | float |

Returns: none (Ok/Err only)

```python
dev.io.audio.numbers_to_speech(number: float) -> Result
```
```c
ow_status ow_io_audio_numbers_to_speech(ow_device* dev, double number);
```
```rust
dev.io().audio().numbers_to_speech(number: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.audio.numbers_to_speech(number)   # check dev.ok
```

## tone

Play Tone. Plays a tone of the given frequency, duration, and amplitude.

Requires power zone 3 (Audio). See [Errors](errors.md).

Wire command: `i\k\t`

| Arg | Wire type |
|---|---|
| frequency | float |
| duration_ms | float |
| amplitude | float |

Returns: none (Ok/Err only)

```python
dev.io.audio.tone(frequency: float, duration_ms: float, amplitude: float) -> Result
```
```c
ow_status ow_io_audio_tone(ow_device* dev, double frequency, double duration_ms, double amplitude);
```
```rust
dev.io().audio().tone(frequency: f64, duration_ms: f64, amplitude: f64) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.audio.tone(frequency, duration_ms, amplitude)   # check dev.ok
```

## speak

Text to Speech. Speaks the given text aloud (text to speech).

Requires power zone 3 (Audio). See [Errors](errors.md).

##### Text to Speech

Speaks the given text aloud through the built‑in speaker (or headphone jack, if connected) using the device's onboard text‑to‑speech engine.

###### Argument
- `text` *(string)* — the text to be spoken. The entire remainder of the command line is treated as the text, so spaces and punctuation are preserved.
  - Maximum length: 128 characters.

###### Returns
- `success` *(basic)* — `true` if the text was accepted and queued for playback, `false` if the input could not be parsed.

###### Examples
```
hello world
The temperature is now seventy two degrees.
Warning: low battery
```

###### Notes
- Playback is asynchronous — the call returns as soon as speech begins.
- To speak a numeric value, use **Numbers to Speech** (`n`) instead for proper digit/decimal pronunciation.
- Use **Play Tone** (`t`), **Play Audio File** (`f`), or **Play Audio Asset** (`a`) for non‑speech audio output.

Wire command: `i\k\v`

| Arg | Wire type |
|---|---|
| text | string |

Returns: none (Ok/Err only)

```python
dev.io.audio.speak(text: str) -> Result
```
```c
ow_status ow_io_audio_speak(ow_device* dev, const char* text);
```
```rust
dev.io().audio().speak(text: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.audio.speak(text)   # check dev.ok
```

## Events

Spontaneous frames this menu emits (not command responses). Text events arrive as `[*<id> ...]` frames (Python: `Transport.events`; C: `ow_poll_text_event`; Rust: `poll_event` -> `Event::Text`); binary events use the binary API below. The on-device rthon and WASM bindings do not receive event streams; menus that expose a polled receive command (for example `receive_canfd`) are the on-device path.

### `record` (text)

Sound recording progress (permille of the clip length)

| Payload field | Wire type |
|---|---|
| progress | decU32 |

### `audio` (text)

PDM microphone sample batch (8 signed samples per event)

| Payload field | Wire type |
|---|---|
| s0 | decS32 |
| s1 | decS32 |
| s2 | decS32 |
| s3 | decS32 |
| s4 | decS32 |
| s5 | decS32 |
| s6 | decS32 |
| s7 | decS32 |
