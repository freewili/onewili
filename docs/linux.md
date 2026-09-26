# Linux Functions

`dev.linux` - wire path `l` - generated from `fwMenuLinux`.

## enable_linux_cpu

Enable Linux CPU. Not yet implemented; always reports failure

Wire command: `l\a`

Returns: none (Ok/Err only)

```python
dev.linux.enable_linux_cpu() -> Result
```
```c
ow_status ow_linux_enable_linux_cpu(ow_device* dev);
```
```rust
dev.linux().enable_linux_cpu() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.linux.enable_linux_cpu()   # check dev.ok
```

## open_shell

Open Shell.

Requires power zones 6 and 17 (FPGA, CM0). See [Errors](errors.md).

Wire command: `l\b`

Returns: none (Ok/Err only)

```python
dev.linux.open_shell() -> Result
```
```c
ow_status ow_linux_open_shell(ow_device* dev);
```
```rust
dev.linux().open_shell() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.linux.open_shell()   # check dev.ok
```

## open_shell_session

Open Shell Session. Open a framed Linux shell without entering menu passthrough

Requires power zones 6 and 17 (FPGA, CM0). See [Errors](errors.md).

Choose a nonzero session token and reuse it for read, write and close. An active panel or legacy shell refuses this request. Read or write at least every 30 seconds to retain ownership. Uses the FPGA mailbox, including when CM0 USB is in host mode. Requires the fwcm0 bridge daemon.

Wire command: `l\c`

| Arg | Wire type |
|---|---|
| session | hex32 |

Returns: session (hex32)

```python
dev.linux.open_shell_session(session: int) -> Result
```
```c
ow_status ow_linux_open_shell_session(ow_device* dev, uint32_t session, uint32_t* session_out);
```
```rust
dev.linux().open_shell_session(session: u32) -> Result<u32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.linux.open_shell_session(session)   # returns value; check dev.ok
```

## close_shell_session

Close Shell Session. Release the framed Linux shell and terminate its session

Only the matching session token can close the shell. MAIN menu and OneWili sessions stay connected.

Wire command: `l\e`

| Arg | Wire type |
|---|---|
| session | hex32 |

Returns: none (Ok/Err only)

```python
dev.linux.close_shell_session(session: int) -> Result
```
```c
ow_status ow_linux_close_shell_session(ow_device* dev, uint32_t session);
```
```rust
dev.linux().close_shell_session(session: u32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.linux.close_shell_session(session)   # check dev.ok
```

## write_shell_session

Write Shell Session. Queue up to 192 shell bytes and return the accepted byte count

Requires power zones 6 and 17 (FPGA, CM0). See [Errors](errors.md).

Encode bytes as compact hexadecimal, including CR, Ctrl-C and ANSI keys. Send only the unaccepted suffix after a short write. A failed or timed-out request must not be blindly replayed. Shell data never enters the MAIN command parser.

Wire command: `l\w`

| Arg | Wire type |
|---|---|
| session | hex32 |
| data | string |

Returns: accepted (dec)

```python
dev.linux.write_shell_session(session: int, data: str) -> Result
```
```c
ow_status ow_linux_write_shell_session(ow_device* dev, uint32_t session, const char* data, int32_t* accepted);
```
```rust
dev.linux().write_shell_session(session: u32, data: &str) -> Result<i32, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.linux.write_shell_session(session, data)   # returns value; check dev.ok
```

## read_shell_session

Read Shell Session. Read bounded shell output as hexadecimal inside a normal menu response

Requires power zones 6 and 17 (FPGA, CM0). See [Errors](errors.md).

Returns count, compact hex data (a dash when empty), and running. Drain final output after running becomes false, then close. Empty reads renew the 30-second lease. Overflow or link reset fails explicitly; reopen before continuing.

Wire command: `l\r`

| Arg | Wire type |
|---|---|
| session | hex32 |
| maximum | dec |

Returns: count (dec), data (string), running (bool)

```python
dev.linux.read_shell_session(session: int, maximum: int) -> Result
```
```c
ow_status ow_linux_read_shell_session(ow_device* dev, uint32_t session, int32_t maximum, int32_t* count, char* data, size_t data_cap, bool* running);
```
```rust
dev.linux().read_shell_session(session: u32, maximum: i32) -> Result<(i32, String, bool), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.linux.read_shell_session(session, maximum)   # returns value; check dev.ok
```

## cm0_usb_mode

CM0 USB Mode. Query or switch CM0 USB between PC gadget and USB-A Port 3 host; requires CM0 image support

Requires power zones 6 and 17 (FPGA, CM0). See [Errors](errors.md).

Use status to query the live mode and whether dynamic switching is supported. Use gadget for the PC connection or host for a USB device on Port 3 (CN25). Requires a CM0 image implementing USB-mode control; older images return a support/no-reply error and no switch is sent. Switching to host disconnects Gadget Serial. Use the MAIN connection, which remains available. Finish gadget transfers or host USB operations and unmount host storage before switching. The command changes the runtime mode only, not the boot default. Success confirms the applied mode and routing, not peripheral enumeration. Requests are not automatically replayed after a timeout; query status before retrying. Returns mode (gadget, host or passthrough) and switchable (0 or 1).

Wire command: `l\u`

| Arg | Wire type |
|---|---|
| mode | string |

Returns: mode (string), switchable (bool)

```python
dev.linux.cm0_usb_mode(mode: str) -> Result
```
```c
ow_status ow_linux_cm0_usb_mode(ow_device* dev, const char* mode, char* mode_out, size_t mode_out_cap, bool* switchable);
```
```rust
dev.linux().cm0_usb_mode(mode: &str) -> Result<(String, bool), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.linux.cm0_usb_mode(mode)   # returns value; check dev.ok
```
