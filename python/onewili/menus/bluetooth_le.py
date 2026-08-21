"""BT Functions menu - generated from fwMenuBluetoothLE. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .ble_settings_2 import BLESettings


class BluetoothLE(MenuBase):
    r"""BT Functions (``w\b``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "btscan": {"binary": False, "payload": [("name", "string"), ("mac", "string"), ("rssi", "decS32")], "description": "BLE scan result (device name, MAC address, RSSI)"},
        "attrvalue": {"binary": False, "payload": [("slot", "decU32"), ("value", "string")], "description": "User attribute slot value read back from the radio"},
        "attrwritten": {"binary": False, "payload": [("slot", "decU32"), ("value", "string")], "description": "A connected peer wrote a user attribute slot"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.ble_settings = BLESettings(transport, nav_path + "\\b")

    def on_start_bt_advertising(self, hostname: str) -> Result:
        r"""Start BT Advertising.

        Wire: ``w\b\a``

        Sets the Host Name for the Bluetooth LE

        # Start BT Advertising

Starts BT LE advertising on the FreeWili's ESP32-C5 wireless co-processor using the supplied **advertising (host) name**. Once advertising, the FreeWili is discoverable by BLE central devices (phones, laptops, scanners, etc.).

## Arguments

- `hostname` *(string, required)* — The BLE advertising name to broadcast.
  - Must be **1–32 characters**.
  - Sent as the device's advertised local name.

## Returns

- `success` *(basic)* — `true` if advertising was started, `false` otherwise.

## Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Validates the hostname (non-empty, within length limits).
3. Stores the name in persistent menu settings (`szBTAdv`) and enables BLE (`m_bEnableBLE = true`).
4. Pushes the updated configuration to the wireless module via `UpdateBottleNoseSettings()`.

## Failure Cases

- ESP32-C5 is not connected.
- `hostname` is empty.
- `hostname` exceeds the maximum allowed length.

## Related

- `Stop BT Advertising` — stops the BLE advertisement.
- `Enable Terminal API Mode` — exposes the FreeWili Terminal API over BLE.
- `Scan for BT Devices` — scans for nearby BLE peripherals.

## Example

```
a
MyFreeWili
```

Advertises the device as **MyFreeWili**.

        String: Advertising Name

        Args:
            hostname: hostname (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(hostname)], [])

    def on_stop_bt_advertising(self) -> Result:
        r"""Stop BT Advertising.

        Wire: ``w\b\t``

        Stops BT Advertising

        # Stop BT Advertising

Stops BLE advertising on the FreeWili's ESP32-C5 wireless co-processor. After this command, the FreeWili is no longer discoverable by BLE central devices (phones, laptops, scanners, etc.).

## Arguments

*None.*

## Returns

- `success` *(basic)* — `true` if advertising was stopped, `false` otherwise.

## Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Disables BLE in persistent menu settings (`m_bEnableBLE = false`).
3. Pushes the updated configuration to the wireless module via `UpdateBottleNoseSettings()`.

## Failure Cases

- ESP32-C5 is not connected.

## Related

- `Start BT Advertising` — begins BLE advertising with a chosen host name.
- `Enable Terminal API Mode` — exposes the FreeWili Terminal API over BLE.
- `Scan for BT Devices` — scans for nearby BLE peripherals.

## Example

```
t
```

Stops BLE advertising on the device.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def on_scan_bt_devices(self, durationms: int) -> Result:
        r"""Scan for BT Devices.

        Wire: ``w\b\s``

        Scans for BT devices for a given duration

        # Scan for BT Devices

Performs a Bluetooth LE scan on the FreeWili's ESP32-C5 wireless co-processor for the specified duration. Discovered peripherals are reported asynchronously as `btscan` events containing the advertised name, MAC address, and RSSI.

## Arguments

- `durationms` *(decU32, required)* — Scan duration in **milliseconds**. Must be a positive integer.

## Returns

- `success` *(basic)* — `true` if the scan was successfully started, `false` otherwise.

## Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Parses `durationms` from the input. On parse failure, returns `false`.
3. Starts a BLE scan via `startBluetoothLEScan(true, durationms)`.
4. For each device discovered, an event is emitted under the `btscan` channel with the format:

   ```
   <deviceName> <AA:BB:CC:DD:EE:FF> <rssi>
   ```

   - `deviceName` — advertised local name (up to 32 characters; may be empty)
   - MAC address — 6 bytes, colon-separated, lowercase hex
   - `rssi` — signed received signal strength in dBm

## Failure Cases

- ESP32-C5 is not connected.
- `durationms` is missing or not a valid unsigned integer.

## Related

- `Start BT Advertising` — begins BLE advertising with a chosen host name.
- `Stop BT Advertising` — stops the BLE advertisement.
- `Enable Terminal API Mode` — exposes the FreeWili Terminal API over BLE.

## Example

```
s
5000
```

Scans for nearby BLE devices for 5 seconds, streaming each discovery as a `btscan` event.

        Enter scan duration in milliseconds:

        Args:
            durationms: durationms (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(durationms)], [])

    def on_enable_terminal(self) -> Result:
        r"""Toggle Enable Terminal API Mode.

        Wire: ``w\b\e``

        Enables BLE to FreeWili Terminal API Mode

        # Toggle Enable Terminal API Mode

Toggles the FreeWili **Terminal API** exposure over BT LE on the ESP32-C5 wireless co-processor. When enabled, BLE central devices (phones, laptops, scripts, agents) can connect to the FreeWili and drive the same Terminal/CLI API that is normally available over USB. When disabled, the BLE GATT service for the Terminal API is torn down.

This is a **toggle**: each invocation flips the current state.

## Arguments

*None.*

## Returns

- `success` *(basic)* — `true` if the toggle was applied and pushed to the wireless module, `false` otherwise.

## Behavior

1. Verifies the ESP32-C5 is connected. If not, prints `ESP32 Not Connected` and returns `false`.
2. Inverts the persistent setting `m_bEnableBLETerm` (on ↔ off).
3. Pushes the updated configuration to the wireless module via `UpdateBottleNoseSettings()`.
4. Returns `true`.

## Notes

- BLE advertising must be running (see `Start BT Advertising`) for central devices to discover and connect to the Terminal API.
- Because this is a toggle, run it twice to return to the original state.
- The new state is persisted in menu settings and survives subsequent setting updates.

## Failure Cases

- ESP32-C5 is not connected.

## Related

- `Start BT Advertising` — begins BLE advertising with a chosen host name.
- `Stop BT Advertising` — stops the BLE advertisement.
- `Scan for BT Devices` — scans for nearby BLE peripherals.

## Example

```
e
```

Toggles Terminal API over BLE. Run once to enable, again to disable.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def on_set_attribute(self, slot: int, value: str) -> Result:
        r"""Set Attribute.

        Wire: ``w\b\v``

        Sets a user attribute slot a connected phone can read and subscribe to

        slot value

        Args:
            slot: slot (decU32).
            value: value (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(slot), encoding.enc_str(value)], [])

    def on_get_attribute(self, slot: int) -> Result:
        r"""Get Attribute.

        Wire: ``w\b\u``

        Reads a user attribute slot back from the radio

        slot

        Args:
            slot: slot (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(slot)], [])
