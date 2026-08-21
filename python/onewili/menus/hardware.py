"""Hardware Functions menu - generated from fwMenuHardware. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport
from .settings_home import SettingsHome
from .system import System
from .file_system import FileSystem
from .power_management import PowerManagement
from .display_functions import DisplayFunctions


class Hardware(MenuBase):
    r"""Hardware Functions (``h``)."""

    def __init__(self, transport: Transport, nav_path: str) -> None:
        super().__init__(transport, nav_path)
        self.settings_home = SettingsHome(transport, nav_path + "\\s")
        self.system = System(transport, nav_path + "\\a")
        self.file_system = FileSystem(transport, nav_path + "\\x")
        self.power_management = PowerManagement(transport, nav_path + "\\p")
        self.display_functions = DisplayFunctions(transport, nav_path + "\\v")

    def get_time(self) -> Result:
        r"""Get Time.

        Wire: ``h\t``

        Read the current date and time from the board RTC (weekday 0=Sun..6=Sat)

        Reads the real-time clock kept by the board-manager PIC. The reply is one line of decimal tokens: year month day weekday hour min sec, with weekday 0=Sun..6=Sat and a 24-hour clock. Fails when the display link is down or the PIC has not seeded the clock yet.

        Returns:
            Result: Ok(year: int, month: int, day: int, weekday: int, hour: int, min: int, sec: int) or Err(message).
        """
        return self._call("t", [], ["int", "int", "int", "int", "int", "int", "int"])

    def set_time(self, year: int, month: int, day: int, hour: int, min: int, sec: int) -> Result:
        r"""Set Time.

        Wire: ``h\c``

        Set the board RTC date and time; the weekday is computed from the date

        Sets the real-time clock kept by the board-manager PIC. Arguments are year (2000-2099), month, day, hour (24-hour), minute, second; the weekday is derived from the date. See Also: t (Get Time).

        YYYY MM DD hh mm ss (24-hour)

        Args:
            year: year (dec).
            month: month (dec).
            day: day (dec).
            hour: hour (dec).
            min: min (dec).
            sec: sec (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(year), encoding.enc_int(month), encoding.enc_int(day), encoding.enc_int(hour), encoding.enc_int(min), encoding.enc_int(sec)], [])
