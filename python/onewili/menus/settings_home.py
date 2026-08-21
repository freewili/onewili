"""Device Settings menu - generated from fwMenuSettingsHome. Do not edit."""
from __future__ import annotations

from result import Result

from ..menubase import MenuBase
from ..transport import Transport
from .uart_settings_2 import UARTSettings
from .i2c_settings_2 import I2CSettings
from .sensor_settings import SensorSettings
from .spi_settings_2 import SPISettings
from .io_direction_settings_2 import IODirectionSettings
from .fpga_clock_settings import FPGAClockSettings
from .radio_settings import RadioSettings
from .radio_settings import RadioSettings
from .radio_fa_settings import RadioFASettings
from .rtc_settings import RTCSettings
from .wifi_settings import WifiSettings
from .ble_settings import BLESettings
from .orca_settings import OrcaSettings
from .websocket_settings import WebsocketSettings
from .neptune_settings import NeptuneSettings
from .general_settings import GeneralSettings
from .analog_in_settings import AnalogInSettings
from .sound_settings import SoundSettings
from .power_settings import PowerSettings
from .light_show_settings import LightShowSettings
from .interface_settings import InterfaceSettings


class SettingsHome(MenuBase):
    r"""Device Settings (``h\s``)."""

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.uart_settings = UARTSettings(transport, nav_path + "\\u")
        self.i2c_settings = I2CSettings(transport, nav_path + "\\i")
        self.sensor_settings = SensorSettings(transport, nav_path + "\\v")
        self.spi_settings = SPISettings(transport, nav_path + "\\s")
        self.io_direction_settings = IODirectionSettings(transport, nav_path + "\\o")
        self.fpga_clock_settings = FPGAClockSettings(transport, nav_path + "\\f")
        self.radio_settings = RadioSettings(transport, nav_path + "\\r")
        self.radio_settings_2 = RadioSettings(transport, nav_path + "\\t")
        self.radio_fa_settings = RadioFASettings(transport, nav_path + "\\a")
        self.rtc_settings = RTCSettings(transport, nav_path + "\\c")
        self.wifi_settings = WifiSettings(transport, nav_path + "\\w")
        self.ble_settings = BLESettings(transport, nav_path + "\\b")
        self.orca_settings = OrcaSettings(transport, nav_path + "\\g")
        self.websocket_settings = WebsocketSettings(transport, nav_path + "\\k")
        self.neptune_settings = NeptuneSettings(transport, nav_path + "\\p")
        self.general_settings = GeneralSettings(transport, nav_path + "\\e")
        self.analog_in_settings = AnalogInSettings(transport, nav_path + "\\j")
        self.sound_settings = SoundSettings(transport, nav_path + "\\n")
        self.power_settings = PowerSettings(transport, nav_path + "\\m")
        self.light_show_settings = LightShowSettings(transport, nav_path + "\\l")
        self.interface_settings = InterfaceSettings(transport, nav_path + "\\x")

    def software_reset(self) -> Result:
        r"""Software Reset.

        Wire: ``h\s\1``

        Performs a software reset of the device.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("1", [], [])

    def software_reset_to_bootloader(self) -> Result:
        r"""Reset To Bootloader.

        Wire: ``h\s\2``

        Resets the device into the USB bootloader.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("2", [], [])

    def all_settings_to_defaults(self) -> Result:
        r"""All Settings To Defaults.

        Wire: ``h\s\3``

        Restores all settings to their default values.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("3", [], [])
