"""OneWili device object - generated. Do not edit."""
from __future__ import annotations

from .transport import Transport
from .menus.io import IO
from .menus.gui import GUI
from .menus.hardware import Hardware
from .menus.wireless import Wireless
from .menus.scripting import Scripting
from .menus.apps import Apps
from .menus.linux import Linux
from .menus.logger import Logger


class OneWili:
    """A FreeWili device: every firmware menu is an attribute."""

    def __init__(self, port_name: str | None = None, binary_port: str | None = None, transport=None) -> None:
        self._transport = transport if transport is not None else Transport(port_name)
        self._binary_port = binary_port
        self._binary = None
        self._files = None
        self.io = IO(self._transport, "i")
        self.gui = GUI(self._transport, "g")
        self.hardware = Hardware(self._transport, "h")
        self.wireless = Wireless(self._transport, "w")
        self.scripting = Scripting(self._transport, "s")
        self.apps = Apps(self._transport, "a")
        self.linux = Linux(self._transport, "l")
        self.logger = Logger(self._transport, "r")

    def open(self) -> "OneWili":
        self._transport.open()
        return self

    def open_binary(self) -> "OneWili":
        if self._binary is not None:
            return self
        if self._binary_port is None:
            raise RuntimeError(
                "no binary port known - pass binary_port=... or use connect(binary=True)")
        from . import binary_events
        from .binary_transport import BinaryTransport
        self._binary = BinaryTransport(self._binary_port, binary_events.DECODERS)
        self._binary.open()
        return self

    @property
    def binary_events(self):
        """Queue of decoded binary events (open_binary() first)."""
        if self._binary is None:
            raise RuntimeError(
                "binary port not open - call open_binary() or connect(binary=True)")
        return self._binary.events

    @property
    def files(self):
        """File transfer and directory listing (lazily constructed)."""
        if self._files is None:
            from .files import Files
            self._files = Files(self)
        return self._files

    def close(self) -> None:
        if self._binary is not None:
            self._binary.close()
            self._binary = None
        self._transport.close()

    def __enter__(self) -> "OneWili":
        return self.open()

    def __exit__(self, *exc) -> bool:
        self.close()
        return False
