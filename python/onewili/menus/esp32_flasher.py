"""ESP32 Flasher Functions menu - generated from fwMenuESP32Flasher. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class ESP32Flasher(MenuBase):
    r"""ESP32 Flasher Functions (``w\a``)."""

    def enter_bootloader(self, upgrade_transmission_rate: int) -> Result:
        r"""Connect To Bootloader.

        Wire: ``w\a\b``

        Opens a ROM-loader session: resets the ESP32 into its bootloader and loads the flasher stub

        # Connect To Bootloader

Drives the ESP32's `BOOT` and `EN` pins to put the target into ROM bootloader (download) mode and establishes a serial-loader sync over the UART. Once synced, the session stays open for the other flash/memory/register operations in this menu.

## Session

- While a session is open the chip sits in its ROM loader: the FREE-WILi's Wi-Fi/BLE link to it is parked, and Flash From Folder reports `Busy`.
- `r` (Reset) closes the session and restarts the ESP32 application; `p 1` and a non-zero `t` entry point close it too.
- A session with no command for 60 s closes itself and restarts the application.
- Write, erase and memory commands need an open session and answer `Not connected` without one. The read-only queries (`i`, `k`, `m`, `j`, `c`) open a session for themselves when none is open and restart the application afterwards.
- Calling `b` again restarts the session, so a new baud rate takes effect.

## Argument

- `upgrade_transmission_rate` (`decU32`, baud)
  - Baud rate to switch to **after** a successful sync.
  - Initial sync always occurs at the default `115200` baud.
  - Pass `0` to keep the link at `115200`.
  - Typical values: `230400`, `460800`, `921600`.
  - Ignored on ESP8266 targets (not supported by ROM).

## Returns

- `success` — `true` if the bootloader handshake (and optional rate change) completed.

## Behavior

1. Toggle `BOOT`/`EN` to enter ROM download mode.
2. Sync with the ESP loader at `115200`.
3. If `upgrade_transmission_rate != 0`, request the target to switch baud and reconfigure the host UART to match.

## Typical Workflow

```text
b <baud>     # Connect To Bootloader
i            # Read Chip ID / security info
k            # Read flash size
f ...        # Start flash operations
o ...        # Write flash data
p 1          # Finish flash, reboot
```

## Troubleshooting

- **Timeout** — check wiring of `EN`, `BOOT`, `TX`, `RX`, `GND`.
- **Invalid target** — chip or revision not supported by the loader build.
- **Invalid response at high baud** — retry with `0` (stay at 115200) or shorter / better-quality wires.

        Enter baud rate to switch to once connected (0 keeps 115200)

        Args:
            upgrade_transmission_rate: upgrade_transmission_rate (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(upgrade_transmission_rate)], [])

    def enter_application(self) -> Result:
        r"""Reset.

        Wire: ``w\a\r``

        Closes any loader session and resets the ESP32 into its application

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def get_i_dand_security(self) -> Result:
        r"""Read Chip ID And Security Info.

        Wire: ``w\a\i``

        Reads the ESP32's chip ID, ECO version and security flags

        Returns:
            Result: Ok(esp_chip_id: int, version: int, sb_en: bool, sbar_en: bool, sdm_en: bool, sbrk_1: bool, sbrk_2: bool, sbrk_3: bool, jtag_sw_dis: bool, jtag_hw_dis: bool, usb_dis: bool, flash_enc_en: bool, dcache_dis: bool, icache_dis: bool) or Err(message).
        """
        return self._call("i", [], ["int", "int", "bool", "bool", "bool", "bool", "bool", "bool", "bool", "bool", "bool", "bool", "bool", "bool"])

    def read_flash_size(self) -> Result:
        r"""Read Flash Size.

        Wire: ``w\a\k``

        Detects the ESP32's flash size in bytes

        Returns:
            Result: Ok(flash_size_bytes: int) or Err(message).
        """
        return self._call("k", [], ["int"])

    def read_esp32mac(self) -> Result:
        r"""Read MAC.

        Wire: ``w\a\m``

        Reads the ESP32's factory MAC address

        Returns:
            Result: Ok(esp32_mac: str) or Err(message).
        """
        return self._call("m", [], ["str"])

    def erase_all_flash(self) -> Result:
        r"""Erase All Flash.

        Wire: ``w\a\e``

        Erases the ESP32's entire flash. Needs an open loader session

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def start_flash_operations(self, offset: int, size: int, block_size: int) -> Result:
        r"""Start Writing Flash Operations.

        Wire: ``w\a\f``

        Prepares ESP32 to write flash at offset and expected size. Block size can be up to 128 bytes; each Write Flash sends one block

        Enter offset (hex), followed by image size (int) and expected block size (int)

        Args:
            offset: offset (hexU32).
            size: size (decU32).
            block_size: block_size (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_hex(offset, 8), encoding.enc_int(size), encoding.enc_int(block_size)], [])

    def stop_flash_operation(self, reboot: bool) -> Result:
        r"""Finish Flash Writing Operations.

        Wire: ``w\a\p``

        Ends ESP32 flashing; reboot=1 also closes the session and starts the new image

        Reboot esp32 after stopping flash operations (1 - yes, 0 - no:)

        Args:
            reboot: reboot (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_bool(reboot)], [])

    def flash_write(self, flash_data: bytes | bytearray) -> Result:
        r"""Write Flash.

        Wire: ``w\a\o``

        Writes one block (up to the block size given to f) into flash

        Enter Byte separated by spaces, up to 128 bytes:

        Args:
            flash_data: flash_data (bytearray).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_bytes(flash_data)], [])

    def flash_read(self, offset: int, size: int) -> Result:
        r"""Read Flash.

        Wire: ``w\a\j``

        Reads up to 128 bytes of ESP32 flash at the given address

        Enter offset (hex) followed by size to read

        Args:
            offset: offset (hexU32).
            size: size (decU32).

        Returns:
            Result: Ok(data: bytes | bytearray) or Err(message).
        """
        return self._call("j", [encoding.enc_hex(offset, 8), encoding.enc_int(size)], ["bytes"])

    def start_write_memory_operations(self, offset: int, size: int, block_size: int) -> Result:
        r"""Start Memory Write Operations.

        Wire: ``w\a\y``

        Prepares a RAM load on the ESP32. Block size can be up to 128 bytes

        Enter RAM address (hex), followed by image size (int) and block size (int)

        Args:
            offset: offset (hexU32).
            size: size (decU32).
            block_size: block_size (decU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [encoding.enc_hex(offset, 8), encoding.enc_int(size), encoding.enc_int(block_size)], [])

    def memory_write(self, data: bytes | bytearray) -> Result:
        r"""Write Memory.

        Wire: ``w\a\0``

        Writes one block (up to the block size given to y) into ESP32 RAM

        Enter bytes separated by spaces, up to 128 bytes:

        Args:
            data: data (bytearray).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("0", [encoding.enc_bytes(data)], [])

    def stop_memory_operation(self, entry_address: int) -> Result:
        r"""Stop Memory Write Operations.

        Wire: ``w\a\t``

        Ends a RAM load; a non-zero entry point starts the loaded code and closes the session

        Enter entry offset (hex) that the esp32 will boot to in RAM:

        Args:
            entry_address: entry_address (hexU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_hex(entry_address, 8)], [])

    def register_write(self, offset: int, value: int) -> Result:
        r"""Write Register.

        Wire: ``w\a\g``

        Writes a 4 byte value onto a register in the esp32

        Enter Regiser Address (hex) followed by value (hex):

        Args:
            offset: offset (hexU32).
            value: value (hexU32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_hex(offset, 8), encoding.enc_hex(value, 8)], [])

    def register_read(self, offset: int) -> Result:
        r"""Read Register.

        Wire: ``w\a\c``

        Reads a 4 byte value from a register in the esp32

        Enter Regiser Address (hex) to read from:

        Args:
            offset: offset (hexU32).

        Returns:
            Result: Ok(memory_block: int) or Err(message).
        """
        return self._call("c", [encoding.enc_hex(offset, 8)], ["hex"])

    def flash_default(self) -> Result:
        r"""Flash Default App.

        Wire: ``w\a\n``

        Not available on FW2: there is no built-in image. Use Flash From Folder

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [], [])

    def flash_from_folder(self, folder: str) -> Result:
        r"""Flash From Folder.

        Wire: ``w\a\w``

        Flashes the ESP32 from an idf.py build folder on the SD card

        # Flash From Folder

Starts flashing the ESP32 from a folder on the SD card. The folder must contain a `flasher_args.json` manifest as produced by an `idf.py build` (copy the whole build output folder - the manifest plus the `.bin` files it references - onto the SD card).

## Argument

- `folder` (`string`) - SD card path of the build folder, e.g. `1:/bottlenose/`. A trailing `/` is optional.

## Returns

- `success` - `true` if the flash was STARTED. Flashing itself runs in the background and takes tens of seconds.

The command fails immediately (with a reason in the payload) when:

- `Busy` - a flash is already running
- `No flasher_args.json` - the folder has no manifest

## Behavior

1. The ESP32 is put into its ROM bootloader (BOOT/EN via the IO expander).
2. The loader syncs at 115200 baud, then upgrades to 460800.
3. Each partition listed in `flash_files` is written in turn.
4. The ESP32 is reset back into its application.

Progress is streamed to the console as `[*espflasher <message> <0|1>]` events and shown on the display as a progress dialog. Poll `Flash Status` (`s`) for machine-readable progress.

## Typical Workflow

```text
w            # wireless menu
a            # ESP32 Flasher Functions
w 1:/bottlenose/   # start flashing
s            # poll: flashing progress partition_index partition_count
```

## Troubleshooting

- **Timeout events** - ESP32 not entering the bootloader; check power and the BOOT/EN lines.
- **Manifest parse errors** - `flasher_args.json` larger than 4 KB or more than 6 partitions is rejected.

        Enter SD card folder containing flasher_args.json (e.g. 1:/bottlenose/)

        Args:
            folder: folder (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_str(folder)], [])

    def flash_status(self) -> Result:
        r"""Flash Status.

        Wire: ``w\a\s``

        Reports ESP32 flashing state and progress percentage

        Returns:
            Result: Ok(flashing: bool, progress: int, partition_index: int, partition_count: int) or Err(message).
        """
        return self._call("s", [], ["bool", "int", "int", "int"])
