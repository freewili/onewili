"""SPI Functions menu - generated from fwMenuSPI. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .spi_settings import SPISettings


class SPI(MenuBase):
    r"""SPI Functions (``i\e``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "spislv": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "SPI slave received bytes from the master"},
    }

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.settings = SPISettings(transport, nav_path + "\\s")

    def s_pi_write(self, data_bytes: bytes | bytearray) -> Result:
        r"""Write and Read.

        Wire: ``i\e\w``

        Writes data to SPI and returns response data

        ## Write and Read (SPI)

Performs a full-duplex SPI transaction: writes the supplied bytes on MOSI while simultaneously capturing the bytes returned on MISO, then prints the received bytes.

### Usage
Enter one or more data bytes as hexadecimal values separated by spaces.

```
w 9F
w 03 00 00 00
w AB CD EF
```

### Behavior
- **Chip Select** is asserted automatically (driven low before the transfer, high after).
- The transfer length equals the number of input bytes; MISO is captured for every clocked byte.
- Uses the currently configured SPI **baud rate**, **mode**, and **CS pin** from the SPI settings.

### Arguments
- `dataBytes` — one or more hex bytes to clock out (e.g. `9F`, `03 00 00 00`).

### Returns
- `success` — `true` if the transfer completed, `false` on invalid input or bus error.
- Response bytes are printed as space-separated hex.

### Example
Reading a SPI flash JEDEC ID:
```
w 9F 00 00 00
```
Response bytes 2–4 contain the manufacturer / device ID.

        Enter Data Byte(s) Separated By Spaces

        Args:
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(spi_response: bytes | bytearray) or Err(message).
        """
        return self._call("w", [encoding.enc_bytes(data_bytes)], ["bytes"])

    def s_pi_slave_enable(self) -> Result:
        r"""SPI Slave Enable.

        Wire: ``i\e\e``

        Toggles SPI slave mode using the configured response data

        ## SPI Slave Enable

Toggles this device into SPI slave mode on the breakout bus (MISO 12 / CS 13 / SCLK 14 / MOSI 15).

### Behavior
- Enabling reconfigures the breakout directions (CS and SCLK become inputs) and arms the response data set with **Set SPI Slave Response Data** (`l`).
- Bytes clocked in by the master are streamed as `spislv` events.
- The response (up to 8 bytes, the hardware FIFO depth) is re-armed after every chip-select release.
- While slave mode is active, **Write and Read** reports Failed.
- Disabling restores the default breakout directions and master mode per the SPI settings.

### CPHA requirement for multi-byte transfers
The master here holds Chip Select low for the whole duration of a **Write and Read** (`w`), not just one byte. The PL022 SPI peripheral behind this slave mode only supports that when **CPHA is 1**: at the default CPHA=0, the hardware requires Chip Select to pulse between every single byte, so only the first byte of a multi-byte transfer is valid and every byte after it reads back as idle (0xFF) on both sides. Set CPHA (SPI Settings `a`) to 1 on BOTH the master and the slave board before exchanging more than one byte; a single-byte transfer works at either CPHA setting.

### Direction apply while slave is active
An FPGA/direction operation while slave mode is active may steal the SPI pins; re-enable slave mode if that happens.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [], [])

    def s_pi_slave_set_data(self, data_bytes: bytes | bytearray) -> Result:
        r"""Set SPI Slave Response Data.

        Wire: ``i\e\l``

        Sets the bytes the SPI slave clocks out (max 8, FIFO depth)

        ## Set SPI Slave Response Data

Stores the bytes the SPI slave will clock out on MISO when a master transfers. Limited to 8 bytes -- the PL022 transmit FIFO depth.

### Usage
```
l A1 B2 C3 D4
```

### Behavior
- Takes effect immediately if slave mode is active (the FIFO is flushed and re-armed).
- A master clocking more bytes than the response length reads undefined data for the excess bytes; its extra written bytes are still captured and streamed.

        Enter Up To 8 Data Byte(s) Separated By Spaces

        Args:
            data_bytes: data_bytes (hexbytes).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_bytes(data_bytes)], [])
