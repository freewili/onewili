# OneWili API

Generated Python bindings for the FreeWili serial menu system.
Produced by the menutool API Generator - do not edit by hand;
regenerate from the firmware sources instead.

## Install

```
pip install -e .
```

## Quickstart

```python
import onewili

dev = onewili.connect()   # discovers the device with pyfwfinder
# every firmware menu is an attribute, e.g. dev.io - see docs/index.md

res = dev.some_menu.some_command(...)
match res:
    case onewili.Ok(value):
        print(value)
    case onewili.Err(message):
        print("failed:", message)

dev.close()
```

## Menus

- `dev.io` - IO functions (hotkey `i`)
- `dev.gui` - GUI Functions (hotkey `g`)
- `dev.hardware` - Hardware Functions (hotkey `h`)
- `dev.wireless` - Wireless (hotkey `w`)
- `dev.scripting` - Scripting Functions (hotkey `s`)
- `dev.apps` - Apps functions (hotkey `a`)
- `dev.linux` - Linux Functions (hotkey `l`)
- `dev.logger` - Logger (hotkey `r`)

Full reference (shared across languages): [../docs/index.md](../docs/index.md)
