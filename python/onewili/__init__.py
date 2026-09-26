"""OneWili API - generated Python bindings for the FreeWili serial menu system."""
from __future__ import annotations

from result import Err, Ok, Result

from .device import OneWili

__version__ = "0.1.0"
__all__ = ["OneWili", "Ok", "Err", "Result", "connect", "find_devices"]


def find_devices():
    """All connected FreeWili devices (via pyfwfinder)."""
    import pyfwfinder

    return pyfwfinder.find_all()


def connect(serial: "str | None" = None, binary: bool = False, *, raw: bool = False,
            queue_size: int = 256) -> OneWili:
    """Find a FreeWili with pyfwfinder and open its Main-processor serial port.

    binary=True also opens the FTDI binary-event port (dev.binary_events).
    """
    devices = find_devices()
    if serial is not None:
        devices = [d for d in devices if getattr(d, "serial", None) == serial]
    if not devices:
        raise RuntimeError(
            "no FreeWili device found" + (f" with serial {serial!r}" if serial else "")
        )
    if len(devices) > 1:
        raise RuntimeError(f"{len(devices)} FreeWili devices found; pass serial=...")
    device = devices[0]
    try:
        binary_port = _binary_port(device)
    except RuntimeError:
        if binary:
            raise
        binary_port = None
    dev = OneWili(_main_port(device), binary_port=binary_port).open()
    if binary:
        try:
            dev.open_binary(raw=raw, queue_size=queue_size)
        except Exception:
            dev.close()   # don't leak the open text port when the FTDI open fails
            raise
    return dev


def _main_port(device) -> str:
    ports = [u for u in device.usb_devices
             if "ftdi" not in str(getattr(u, "kind", "")).lower()
             and ("serial" in str(getattr(u, "kind", "")).lower()
                  or "fw2 v" in str(getattr(u, "name", "")).lower())]
    if not ports:
        raise RuntimeError(f"{getattr(device, 'name', device)!r}: no serial USB device found")
    main = [p for p in ports if "main" in str(getattr(p, "name", "")).lower()]
    return _port_path((main or ports)[0])


def _binary_port(device) -> str:
    candidates = list(getattr(device, "usb_devices", []))
    # kind is authoritative (pyfwfinder reports USBDeviceType.FTDI for the binary
    # interface); the name is only a fallback. Never match on the FW2 substring:
    # the MAIN serial port's name (FW2 v01) contains it too (verified on hardware).
    for u in candidates:
        if "ftdi" in str(getattr(u, "kind", "")).lower():
            return _port_path(u)
    for u in candidates:
        if "ftdi" in str(getattr(u, "name", "")).lower():
            return _port_path(u)
    listing = ", ".join(
        f"({getattr(u, 'kind', '?')}, {getattr(u, 'name', '?')})" for u in candidates
    ) or "none"
    raise RuntimeError(
        "could not identify the FTDI binary port among usb_devices: " + listing
        + " - adjust the match in _binary_port()"
    )


def _port_path(usb) -> str:
    for attr in ("port", "path", "port_name", "location"):
        value = getattr(usb, attr, None)
        if value:
            return str(value)
    raise RuntimeError("could not determine the serial port path from pyfwfinder")
