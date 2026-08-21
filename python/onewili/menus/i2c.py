"""I2C Functions menu - generated from fwMenuI2C. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .i2c_settings import I2CSettings


class I2C(MenuBase):
    r"""I2C Functions (``i\i``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "i2cmon": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "I2C monitor captured bytes"},
        "i2cslv": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "I2C slave received master write (register, data bytes)"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.settings = I2CSettings(transport, nav_path + "\\s")

    def i2c_write(self, address: int, register: int, data_bytes: bytes | bytearray) -> Result:
        r"""Write.

        Wire: ``i\i\w``

        Writes data to a specific I2C Address

        Hex: Address, Register, Data Byte(s) Separated By Spaces

        Args:
            address: address (hex8).
            register: register (hex8).
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("w", [encoding.enc_hex(address, 2), encoding.enc_hex(register, 2), encoding.enc_bytes(data_bytes)], [])

    def i2c_read(self) -> Result:
        r"""Read.

        Wire: ``i\i\r``

        Reads the number from the address

        Hex: Address, Hex Register, Dec Data Length to read Separated By Spaces

        Returns:
            Result: Ok(i2crepsone: bytes | bytearray) or Err(message).
        """
        return self._call("r", [], ["bytes"])

    def i2c_poll(self) -> Result:
        r"""Poll.

        Wire: ``i\i\p``

        Tests all addresses for I2C Response

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [], [])

    def i2c_slave_enable(self, address: int) -> Result:
        r"""I2C Slave Enable.

        Wire: ``i\i\e``

        Enables I2C slave mode at the given 7-bit address; 0 disables

        ## I2C Slave Enable

Turns this device into an I2C slave on the breakout bus (SDA 16 / SCL 17) at the given 7-bit address, backed by a 256-byte register file with auto-incrementing pointer semantics.

### Usage
```
e 17
e 0
```
`e 17` enables slave mode at address 0x17; `e 0` disables it and returns the bus to master mode using the current I2C settings.

### Behavior
- While slave mode is active the master items (Write, Read, Poll) report Failed.
- Master writes received by the slave are streamed as `i2cslv` events (`register` followed by the data bytes).
- Seed the register file with **Set I2C Slave Data** (`l`).

### Arguments
- `address` — 7-bit slave address in hex; `0` disables slave mode.

        Hex: Address (0 disables)

        Args:
            address: address (hex8).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_hex(address, 2)], [])

    def i2c_slave_set_data(self, data_bytes: bytes | bytearray) -> Result:
        r"""Set I2C Slave Data.

        Wire: ``i\i\l``

        Writes bytes into the I2C slave register file

        ## Set I2C Slave Data

Seeds the I2C slave register file: the first byte is the starting register index, the rest are data bytes stored from there.

### Usage
```
l 00 DE AD BE EF
```
Stores DE AD BE EF at registers 0x00-0x03.

### Behavior
- Works whether or not slave mode is currently enabled; the register file persists across enable/disable.
- Bounds-checked against the 256-byte register file.

### Arguments
- `dataBytes` — register index followed by one or more data bytes, all hex.

        Hex: Register, Data Byte(s) Separated By Spaces

        Args:
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_bytes(data_bytes)], [])
