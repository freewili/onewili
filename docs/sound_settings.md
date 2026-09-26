# Sound Settings

`dev.hardware.settings_home.sound_settings` - wire path `h\s\n` - generated from `fwMenuSoundSettings`.

## quiet_threshold

Quiet Threshold. The mic level that counts as an active sound

Wire command: `h\s\n\f`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sound_settings.quiet_threshold(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sound_settings_quiet_threshold(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sound_settings().quiet_threshold(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sound_settings.quiet_threshold(value)   # check dev.ok
```

## speaker_volume

Speaker Volume. The multiplier applied to sound playback

Wire command: `h\s\n\v`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sound_settings.speaker_volume(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sound_settings_speaker_volume(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sound_settings().speaker_volume(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sound_settings.speaker_volume(value)   # check dev.ok
```

## recording_volume

Recording Volume. The multiplier applied to mic recording

Wire command: `h\s\n\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sound_settings.recording_volume(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sound_settings_recording_volume(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sound_settings().recording_volume(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sound_settings.recording_volume(value)   # check dev.ok
```

## record_len_sec

Record Len Sec. The default length of a recording in seconds

Wire command: `h\s\n\r`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sound_settings.record_len_sec(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sound_settings_record_len_sec(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sound_settings().record_len_sec(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sound_settings.record_len_sec(value)   # check dev.ok
```

## system_sounds

System Sounds. Sounds for system events (also gates all audio playback)

Wire command: `h\s\n\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.hardware.settings_home.sound_settings.system_sounds(value: int) -> Result
```
```c
ow_status ow_hardware_settings_home_sound_settings_system_sounds(ow_device* dev, int32_t value);
```
```rust
dev.hardware().settings_home().sound_settings().system_sounds(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.hardware.settings_home.sound_settings.system_sounds(value)   # check dev.ok
```
