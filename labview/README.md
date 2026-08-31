# OneWili for LabVIEW

All 540 OneWili device commands, plus sessions, discovery, events and file
transfer, as a flat C DLL that LabVIEW's **Call Library Function Node** can
call directly.

There are no `.vi` files in this repository on purpose. LabVIEW's *Import
Shared Library Wizard* generates the whole VI library from the header and the
DLL in one pass, and a regenerated header immediately gives you regenerated
VIs. Checking in 540 binary VIs would mean re-exporting them by hand every
time the firmware menus change, which is exactly what the rest of OneWili is
built to avoid.

```
labview/
├── include/
│   ├── onewili_lv.h          <- point the wizard at this one
│   └── onewili_lv_api.h        (generated: the 540 commands)
├── src/
│   ├── onewili_lv.c            hand-written core
│   ├── onewili_lv_internal.h
│   └── onewili_lv_api.c        (generated: the forwarders)
├── examples/selftest.c         C exercise of the ABI, no LabVIEW needed
├── tests/                      pytest suite, no LabVIEW needed
├── tools/gen_lv_api.py         the generator
├── docs/commands.md            (generated: every command, wire path, doc)
├── onewili_lv.def              (generated: the export list)
├── CMakeLists.txt
└── build.ps1                   builds both bitnesses, runs the self-test
```

## 1. Build the DLL

Needs CMake and Visual Studio with the C++ workload (Build Tools is enough).

```powershell
cd labview
.\build.ps1
```

That produces both bitnesses and runs the C self-test against each:

```
bin\win64\onewili_lv.dll     for 64-bit LabVIEW
bin\win32\onewili_lv.dll     for 32-bit LabVIEW
```

The two files deliberately share a name. A Call Library Function Node that
refers to `onewili_lv.dll` **by name only** — no absolute path — loads
whichever copy is on the search path, so one VI library works in either
LabVIEW bitness once you drop in the matching DLL.

Other ways in:

```powershell
.\build.ps1 -Arch x64 -Config Debug
.\build.ps1 -Regenerate            # re-run the API generator first
.\build.ps1 -SelfTestPort COM7     # also run the device half of the self-test
```

On Linux/macOS, or with MinGW, plain CMake works too:

```bash
cmake -S labview -B build && cmake --build build
```

## 2. Testing -- almost none of it needs LabVIEW

LabVIEW is only required for the last layer. Everything below runs without it,
and the first three run without a FreeWili either.

```powershell
.\build.ps1 -Tests                       # layers 1-3
.\build.ps1 -Tests -SelfTestPort auto    # layers 1-4, against a real board
```

| Layer | Needs | Catches |
| --- | --- | --- |
| 1. Header lint | nothing | a type the wizard cannot express, an export/header mismatch, generator churn |
| 2. ABI suite | the shipped DLL | crashes on bad handles, buffer overruns, threading faults |
| 3. Loopback round trip | a test build | wrong argument encoding, wrong output marshalling |
| 4. Hardware | a FreeWili | wrong VID/PID, framing, timing, real events |
| 5. LabVIEW acceptance | LabVIEW | see §12 |

**Layer 1 -- the header is the contract.** `tests/test_header.py` parses the
header the wizard will read and asserts every parameter is one of eleven
spellings a Call Library Function Node can express. It also checks the `.def`
matches the headers exactly, that every command is documented, that
regenerating changes nothing, and that the generator *fails* on a shape it does
not recognise instead of guessing.

**Layer 2 -- the ABI, through ctypes.** `tests/test_abi.py` drives the shipped
DLL through `ctypes`, which loads a library by name, declares types by hand and
passes pre-allocated buffers -- the same model LabVIEW uses. The important one
is the sweep: it calls **all 540 commands with a closed session** and requires
every one to return `OWLV_ERR_SESSION`. That is what catches a function which
touches an output before validating the handle; in LabVIEW that is a hard crash
of the IDE, not an error cluster. It also hammers the DLL from eight threads,
the way parallel loops do.

**Layer 3 -- round trip against a scripted device.** Build with
`-DONEWILI_LV_LOOPBACK=ON` and the DLL gains a fake transport plus four
`owlv_test_*` entry points. A test sets the response it wants, calls a command,
and asserts on *both* halves of the round trip:

```python
set_response(lib, s, "DEADBEEF")
lib.owlv_io_gpio_read_all(s, byref(bits))   # bits == 0xDEADBEEF
last_command(lib, s)                        # == r"i\g\u"
```

That covers every argument shape the generator emits -- scalars, strings, byte
arrays in and out, bools, mixed outputs -- with no board attached. The 540
forwarders are identical in both builds; only the transport differs.

**Layer 4 -- a real FreeWili.** `pytest tests --port auto`, or `--port COM7`.
Read-only by default: it will not change a setting, write a file or switch a
power rail. The one test that needs a rail on is gated behind `--power-zones`
and restores it afterwards.

```powershell
.\bin\win64\owlv_selftest.exe          # the same checks from C, no Python
.\bin\win64\owlv_selftest.exe auto
```

Run the C self-test when you want to prove the DLL works with no Python in the
picture at all. `build.ps1` runs it after every build.

## 3. Generate the VIs

1. Copy the DLL for your LabVIEW bitness somewhere permanent, e.g.
   `C:\Program Files\OneWili\onewili_lv.dll`.
2. In LabVIEW: **Tools » Import » Shared Library (.dll)**.
3. *Create VIs for a shared library.*
4. **Shared Library**: the DLL you just copied.
   **Header File**: `labview/include/onewili_lv.h`.
5. **Include Paths**: add `labview/include` (so `onewili_lv_api.h` resolves).
   Leave *Preprocessor Definitions* empty — the header has no macros in any
   declaration, by design.
6. Pick the functions you want. All 540 is fine but slow to generate; if you
   only need GPIO and UART, filter on `owlv_io_` and cut it to a minute.
   Always include the core ones: `owlv_open*`, `owlv_close*`,
   `owlv_status_message`, `owlv_last_error`, `owlv_pointer_size`.
7. Choose a project directory and library name, then **Finish**.

The wizard asks how to treat each pointer parameter. The rules are uniform
across this API:

| Parameter shape in the header | Configure as |
| --- | --- |
| `int x`, `unsigned int x`, `double x`, `unsigned char x` | Numeric, value |
| `const char* x` | String, C String Pointer |
| `int* x`, `unsigned int* x`, `double* x` | Numeric, **Pointer to Value**, output |
| `char* x` with a following `int x_cap` | Array of U8, Array Data Pointer, output, minimum size `x_cap` |
| `const unsigned char* x` with `int x_len` | Array of U8, Array Data Pointer, input, size `x_len` |
| `unsigned char* x` with `int x_cap`, `int* x_len` | Array of U8, Array Data Pointer, output, minimum size `x_cap` |

Every function returns `int` — wire it to the status handling in step 5 below.

### Doing it by hand instead

For a handful of commands it is quicker to drop a Call Library Function Node
and configure it yourself:

- **Library name or path**: `onewili_lv.dll`
- **Thread**: *Run in any thread* (see §6)
- **Calling convention**: **C**
- **Function name**: e.g. `owlv_io_gpio_set_io_toggle`
- **Return type**: Numeric, Signed 32-bit Integer
- **Parameters**: `session` as I32 value, then the arguments per the table.

## 4. Talking to a device

```
owlv_open_auto  ->  session
    owlv_io_gpio_set_io_toggle(session, 25)
    owlv_io_gpio_read_all(session, &bitfield)
owlv_close(session)
```

`owlv_open_auto` finds the FreeWili main port by USB VID/PID and opens it at
1 Mbaud 8N1. To pick a port yourself:

```
owlv_refresh_ports(&count)
    for i in 0..count-1:
        owlv_port_name(i, name, 64)
        owlv_port_description(i, desc, 192)
        owlv_port_kind(i, &kind)          -- 1 == FreeWili main
owlv_open(name, &session)
```

Port *kinds* come from the USB identifiers, so they are only populated on
Windows. On Linux and macOS every port reports `OWLV_PORT_UNKNOWN` and
`owlv_open_auto` falls back to "open it if it is the only candidate".

`docs/commands.md` lists all 540 commands grouped by firmware menu, with the
wire path and the description straight from the firmware source. It is the
fastest way to find the function name for a menu entry you know.

## 5. Errors

Every function returns 0 on success. Turn that into a LabVIEW error cluster
once, in a subVI you reuse:

```
status != 0  ->  error = true
                 code   = 5300 + status          (OWLV_LV_ERROR_BASE)
                 source = owlv_status_message(status)  + owlv_last_error(session)
```

5300 sits in LabVIEW's 5000–9999 user-defined range, so it never collides
with an NI code.

`owlv_last_error(session, ...)` gives the message for the last failure on that
session; `owlv_last_error(0, ...)` covers failures that happened before a
session existed, such as `owlv_open` not finding the port.

| Code | Meaning |
| --- | --- |
| 0 | ok |
| 1 | bad argument |
| 2 | serial I/O error |
| 3 | timed out waiting for the device |
| 4 | the device rejected the command |
| 5 | malformed response frame |
| 6 | output buffer too small |
| 100 | invalid or closed session handle |
| 101 | no matching FreeWili port found |
| 102 | could not open the serial port |
| 103 | too many open sessions (32 max) |
| 104 | not valid in this state |
| 105 | not supported on this platform |

A `6` on a call with a string or array output means your buffer was too small,
not that the device failed. 256 bytes covers every string this API returns
except command responses; use 4096 for `owlv_send_raw`.

## 6. Threading, and the things LabVIEW does that will bite you

**Bitness.** A 32-bit DLL will not load into 64-bit LabVIEW and the error
message does not say so. `owlv_pointer_size` returns 4 or 8; call it once at
startup and check it against your LabVIEW.

**LabVIEW does not unload the DLL when a VI stops.** Sessions survive between
runs. If a VI is aborted between `owlv_open` and `owlv_close`, the port stays
open and the next run cannot open it. Put `owlv_close_all` in the error case
of your top-level VI, or call it once at startup before opening anything.

**Thread safety.** The DLL is thread-safe: a global lock guards the session
table and each session has its own lock, so calls on different sessions run in
parallel and calls on the same session serialize. Set every node to *Run in
any thread*; the wizard's default of *Run in UI thread* is safe but funnels
everything through one thread and will cap your throughput.

**Blocking.** Every command waits for the device's response frame, up to
5 seconds. Keep command loops off the UI thread and out of any loop that also
drives the front panel.

**Reentrancy.** If you call the same VI from parallel loops, mark it
*Preallocated clone reentrant execution*. Otherwise LabVIEW serializes on the
VI itself, not on the session, and two sessions cannot run concurrently.

## 7. Events

Both event paths are **polled**. Nothing in this DLL starts a thread or calls
back into LabVIEW, so a plain While Loop with a small wait is the whole
pattern.

Text events arrive on the main port, interleaved with command responses, and
are queued by the session so one that lands mid-command is not lost:

```
loop:
    owlv_poll_text_event(session, &got, id, 64, args, 4096,
                         &ts_lo, &ts_hi, &sequence)
    if got: handle id/args
    wait 10 ms
```

An event on the wire is a full frame —
`[*<name> <hexTimestampNs> <seq> <payload> <ok>]`, the same shape a command
response uses. The wrapper splits it: `id` is the name, `args` the payload
alone, and the frame's own fields come back as separate outputs. Recombine the
timestamp as `ts_hi * 2^32 + ts_lo`. Do not strip the trailing ok token
yourself — several event payloads end in a `0`/`1` field of their own, so that
is done for you.

The queue holds 8 events; older ones are dropped and counted. Poll
`owlv_dropped_text_events` occasionally — a growing count means the loop is
not keeping up.

Binary WILI events (GPIO reports, CAN RX, logic analyzer) arrive on a
*second* serial port, the FTDI one:

```
owlv_binary_open_auto(session, port, 64)
loop:
    owlv_binary_poll(session, &kind)
    if kind == 5:   owlv_last_gpio_report(...)
    if kind == 39:  owlv_last_can_rx_report(...)
    if kind == 40:  owlv_last_logic_analyzer_report(...)
owlv_binary_close(session)
```

`owlv_binary_poll` latches the decoded event in the session; the matching
`owlv_last_*` call reads it out and returns `104` if you ask for the wrong
kind. Device timestamps are 64-bit and come back as two U32 halves —
recombine as `hi * 2^32 + lo` — because the Import Wizard's handling of
`unsigned long long` varies by LabVIEW version.

## 8. Files

```
owlv_file_put(session, "C:\\data\\image.fwi", "/images/image.fwi")
owlv_file_get(session, "/logs/log_0001.csv", "C:\\data\\log.csv")
owlv_file_list(session, "/logs", &count)
    owlv_file_entry(session, i, name, 64, &is_dir, &size)
```

`owlv_file_put_mem` / `owlv_file_get_mem` do the same against a U8 array
instead of a host file.

Transfers block. To drive a progress bar, put the transfer node in one loop
(configured *Run in any thread*) and poll `owlv_file_progress` from another —
it deliberately does not take the session lock, so it stays readable while the
transfer holds it.

## 9. The escape hatch

`owlv_send_raw(session, "i\\g\\u", response, 4096)` sends one already-formed
wire line and returns the response payload. Use it to reach a firmware command
that is newer than this wrapper. It is deliberately simpler than the generated
path: it does not reassemble a response frame the firmware split across
physical lines, and it discards text events seen while waiting instead of
queueing them. Prefer the generated `owlv_*` commands for anything the
generator already covers.

## 10. Regenerating

`include/onewili_lv_api.h`, `src/onewili_lv_api.c`, `onewili_lv.def` and
`docs/commands.md` are generated from `c/include/onewili.h` — which is itself
generated from the firmware menu sources. **Do not edit them.** After a
OneWili sync:

```bash
python labview/tools/gen_lv_api.py
```

then rebuild and re-run the Import Shared Library Wizard. Command IDs are
stable and append-only, so regenerating adds VIs and never renames the ones
you have already wired.

The generator refuses to guess. If the firmware introduces a parameter shape
it does not recognise it stops with the function name and the offending type,
rather than emitting something that silently marshals wrong. Teach it the new
shape in `classify()` in `tools/gen_lv_api.py`.

`src/onewili_lv.c`, `src/onewili_lv_internal.h`, `include/onewili_lv.h`,
`examples/selftest.c` and this file are hand-written and survive regeneration.

## 11. Troubleshooting

**"Error 7 occurred at Call Library Function Node"** — LabVIEW cannot find
`onewili_lv.dll`. Put it next to the VI, in the same folder as the built
executable, or somewhere on `PATH`.

**Node loads but the VI errors immediately with 1097** — usually a bitness
mismatch, or a pointer parameter configured as *Pointer to Value* when it
should be *Array Data Pointer*. Check the table in §3.

**Every command returns 3 (timeout)** — you are on the wrong port. The FTDI
binary port and the debug-probe CDC also enumerate as COM ports and do not
answer text commands. Use `owlv_port_kind`, or `owlv_open_auto`.

**First command after a run works, later ones return 100** — a previous run
was aborted without closing. Call `owlv_close_all`.

**Strings come back with trailing garbage** — the buffer was passed as a U8
array and LabVIEW is showing all `cap` bytes. Trim at the first `0` before
converting to a string.

## 12. LabVIEW acceptance checklist

The only part that needs LabVIEW. Get everything above green first.

1. **It loads.** Drop a Call Library Function Node, point it at
   `onewili_lv.dll`, function `owlv_pointer_size`, return type I32, no
   parameters. Run it: `8` in 64-bit LabVIEW, `4` in 32-bit. Error 7 means
   LabVIEW cannot find the DLL; a load failure with the path correct means the
   wrong bitness.
2. **The wizard reads the header.** Run the import on
   `include/onewili_lv.h` and confirm it offers all 540 `owlv_*` functions. If
   any are missing, the header lint in layer 1 should have caught it -- check
   that first.
3. **A no-argument command.** Generate `owlv_open_auto`, `owlv_close` and
   `owlv_hardware_power_management_get_zones`. Wire open -> command -> close.
   Status 0 means the whole chain works.
4. **A scalar output.** `owlv_io_gpio_read_all`, `gpiostate` configured as
   Numeric / Pointer to Value / output. Compare against
   `owlv_selftest.exe auto`.
5. **A string output.** `owlv_hardware_system_device_state` with `sd` as an
   Array of U8 / Array Data Pointer / minimum size 64. Trim at the first `0`
   before converting. Readable text here means the buffer convention is right,
   which is the most common wizard misconfiguration.
6. **A byte array in and out.** `owlv_io_spi_s_pi_write`. Both the input
   length and the output capacity must be wired -- byte-array outputs are not
   optional (§5).
7. **An event loop.** `owlv_poll_text_event` in a While Loop with a 10 ms
   wait; `owlv_hardware_power_management_enable_power_stream(200)` to start the
   traffic, `(0)` to stop it. You should see `power` events with a payload, a
   timestamp and an advancing sequence number.
8. **Abort mid-run.** Start a VI that opens a session, hit abort, then run it
   again. The second run must connect. If it does not, your error case is
   missing `owlv_close_all`.
9. **Parallel loops.** Two loops, two sessions, both nodes set to *Run in any
   thread*, VIs marked preallocated-clone reentrant. Neither should stall on
   the other.

## What is not here

- **Callbacks.** Nothing calls back into LabVIEW; events and progress are
  polled. This is deliberate — user-event callbacks from a DLL thread are the
  single most common way to crash LabVIEW.
- **Clusters.** Every structured result is flattened into scalar outputs, so
  the ABI does not depend on LabVIEW's cluster packing rules.
- **NI Linux RT / cRIO.** The CMake build has a POSIX path and should
  cross-compile, but it has not been tried on an RT target; port discovery
  there returns names without USB identification.
- **A packaged `.vip`.** If you want one, build the VI library with the
  wizard, then wrap it with the VI Package Manager tooling.
