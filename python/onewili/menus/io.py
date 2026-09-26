"""IO functions menu - generated from fwMenuIO. Do not edit."""
from __future__ import annotations

from result import Result

from ..menubase import MenuBase
from ..transport import Transport
from .gpio import GPIO
from .uart import UART
from .mdio import MDIO
from .sensors import Sensors
from .i2c import I2C
from .spi import SPI
from .canfd import CANFD
from .analog_in import AnalogIn
from .analog_out import AnalogOut
from .logic_player import LogicPlayer
from .logic_analyzer import LogicAnalyzer
from .wil_eye import WILEye
from .audio import Audio
from .serial_leds import SerialLEDs
from .nice_usb import NiceUsb
from .cdc_perf import CdcPerf
from .t1s import T1S
from .net import Net


class IO(MenuBase):
    r"""IO functions (``i``)."""

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.gpio = GPIO(transport, nav_path + "\\g")
        self.uart = UART(transport, nav_path + "\\u")
        self.mdio = MDIO(transport, nav_path + "\\m")
        self.sensors = Sensors(transport, nav_path + "\\s")
        self.i2c = I2C(transport, nav_path + "\\i")
        self.spi = SPI(transport, nav_path + "\\e")
        self.canfd = CANFD(transport, nav_path + "\\c")
        self.analog_in = AnalogIn(transport, nav_path + "\\j")
        self.analog_out = AnalogOut(transport, nav_path + "\\a")
        self.logic_player = LogicPlayer(transport, nav_path + "\\p")
        self.logic_analyzer = LogicAnalyzer(transport, nav_path + "\\b")
        self.wil_eye = WILEye(transport, nav_path + "\\f")
        self.audio = Audio(transport, nav_path + "\\k")
        self.serial_leds = SerialLEDs(transport, nav_path + "\\l")
        self.nice_usb = NiceUsb(transport, nav_path + "\\n")
        self.cdc_perf = CdcPerf(transport, nav_path + "\\y")
        self.t1s = T1S(transport, nav_path + "\\r")
        self.net = Net(transport, nav_path + "\\w")
