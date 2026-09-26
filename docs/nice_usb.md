# niceusb

`dev.io.nice_usb` - wire path `i\n` - generated from `fwMenuNiceUsb`.

## nice_usb_start

Start Gadget. Attaches the USB HID gadget on the second USB port, so the host sees a keyboard, mouse and gamepad. Returns as soon as the display accepts the request; the host takes a moment longer to enumerate. Refused while SubGHz is in use.

Brings up the USB HID gadget on the FreeWili's second USB connector. The host PC then sees a composite keyboard/mouse/gamepad device that this firmware can type on.

Starting the gadget costs three things for as long as it stays up:

1. SubGHz is locked out. The gadget and the CC1101 radio share PIO block 2 and cannot both have it, so Start is refused outright while the SubGHz app is open or the radio is mid-operation. Close it and try again.
2. The display's screen becomes the gadget status screen and the menu tree is unreachable on the glass. That is deliberate, not a fault: the framebuffer memory is where the running USB code lives while the gadget is attached. CANCEL on the device stops the gadget and gives the screen back.
3. LoRa keeps working throughout; only the sub-GHz CC1101 path is affected.

Success here means the request was ACCEPTED, not that the host enumerated the device. Run Status to see which of those actually happened - it distinguishes 'attached, no host' (a cable or host-side problem) from 'enumerated' (working).

Starting is also safe from this console while the device is showing any screen at all, because it does not go through the menu tree.

Wire command: `i\n\s`

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.nice_usb_start() -> Result
```
```c
ow_status ow_io_nice_usb_nice_usb_start(ow_device* dev);
```
```rust
dev.io().nice_usb().nice_usb_start() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.nice_usb_start()   # check dev.ok
```

## nice_usb_stop

Stop Gadget. Detaches the USB HID gadget, gives the display screen back and releases the SubGHz lockout. A stop asked for while a script is running takes effect when that script finishes.

Takes the USB HID gadget down. The host sees a clean detach, the display returns to the normal menus, and SubGHz becomes available again without a reboot.

The stop is LATCHED rather than immediate. If a typing script is running when you ask, the gadget stays attached until that script has finished - stopping mid-transfer would leave the host with a half-sent report and, worse, modifier keys held down. Status reports 'stopping' during that window.

Success here means the stop was accepted. Run Status afterwards to confirm it reached 'stopped'; only then has the SubGHz lockout actually cleared.

The same thing can be done on the device itself: while the gadget is attached the display shows the gadget screen, and CANCEL there stops it. That is the only button the device honours in that state.

Wire command: `i\n\t`

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.nice_usb_stop() -> Result
```
```c
ow_status ow_io_nice_usb_nice_usb_stop(ow_device* dev);
```
```rust
dev.io().nice_usb().nice_usb_stop() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.nice_usb_stop()   # check dev.ok
```

## nice_usb_status

niceusb Status. Reports the gadget state as the display holds it, whether a script is running, whether SubGHz is locked out, and the last script error.

Asks the display for the gadget's live state and prints it here. The report is taken as a single snapshot, so the state and the enumeration flag it prints always belong to the same instant.

The state word is one of:

  stopped                  - nothing running, SubGHz is free
  starting (claiming PIO2) - the request was accepted; the USB stack is not up yet
  starting (stack up, D+ low) - the stack is up and the pull-up is about to be raised
  attached, no host        - the device is presenting itself but nothing enumerated it. Suspect the cable, the port, or the host.
  enumerated               - the host has accepted the device. This is the working state.
  stopping                 - teardown in flight; SubGHz is still locked out

'attached, no host' and 'enumerated' are never collapsed into one word, because they have completely different causes and completely different fixes.

The second line reports the last script error as a line number and message, or 'none'. It is not cleared by stopping the gadget, so it still names the failure after the fact.

Wire command: `i\n\i`

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.nice_usb_status() -> Result
```
```c
ow_status ow_io_nice_usb_nice_usb_status(ow_device* dev);
```
```rust
dev.io().nice_usb().nice_usb_status() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.nice_usb_status()   # check dev.ok
```

## nice_usb_run_test_script

Run Test Script. Types a two-line proof script on the host, three seconds apart. The gadget must already be attached. Types into whatever window has focus on the host.

Runs the compiled-in proof script on the host PC. It types 'FreeWili niceusb OK', presses Enter, waits three seconds, types 'still alive' and presses Enter again.

THIS TYPES INTO WHATEVER WINDOW HAS FOCUS on the host. Put the cursor somewhere harmless - a text editor or an empty document - before running it. The text is deliberately inert: plain words only, no shell metacharacters and no GUI/CTRL/ALT combinations.

Each part of the script proves something separate. The multi-character typing proves the IN endpoint is being re-armed between reports; the shifted characters prove modifiers work; and the three-second delay proves the watchdog beat inside the USB pump holds while a script blocks the display's second core.

Refused if the gadget is not attached (start it first) or if a script is already running. The command returns as soon as the script is queued - the typing happens on the display over the next few seconds.

Wire command: `i\n\r`

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.nice_usb_run_test_script() -> Result
```
```c
ow_status ow_io_nice_usb_nice_usb_run_test_script(ow_device* dev);
```
```rust
dev.io().nice_usb().nice_usb_run_test_script() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.nice_usb_run_test_script()   # check dev.ok
```

## niceusb_script

niceusb Script. SD path of the wusb script Start Gadget loads, or empty for the gadget's built-in personality.

Wire command: `i\n\c`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.niceusb_script(value: str) -> Result
```
```c
ow_status ow_io_nice_usb_niceusb_script(ow_device* dev, const char* value);
```
```rust
dev.io().nice_usb().niceusb_script(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.niceusb_script(value)   # check dev.ok
```

## niceusb_force_vidpid

niceusb Force VID/PID. Enable to make Start Gadget present the VID/PID below instead of the script's own or the built-in personality's.

Wire command: `i\n\o`

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.niceusb_force_vidpid() -> Result
```
```c
ow_status ow_io_nice_usb_niceusb_force_vidpid(ow_device* dev);
```
```rust
dev.io().nice_usb().niceusb_force_vidpid() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.niceusb_force_vidpid()   # check dev.ok
```

## niceusb_vid

niceusb VID. Forced USB vendor ID, used only while niceusb Force VID/PID is on.

Wire command: `i\n\v`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.niceusb_vid(value: int) -> Result
```
```c
ow_status ow_io_nice_usb_niceusb_vid(ow_device* dev, int32_t value);
```
```rust
dev.io().nice_usb().niceusb_vid(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.niceusb_vid(value)   # check dev.ok
```

## niceusb_pid

niceusb PID. Forced USB product ID, used only while niceusb Force VID/PID is on.

Wire command: `i\n\p`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.nice_usb.niceusb_pid(value: int) -> Result
```
```c
ow_status ow_io_nice_usb_niceusb_pid(ow_device* dev, int32_t value);
```
```rust
dev.io().nice_usb().niceusb_pid(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.nice_usb.niceusb_pid(value)   # check dev.ok
```
