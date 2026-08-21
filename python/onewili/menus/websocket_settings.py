"""Websocket Server menu - generated from fwMenuWebsocketSettings. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class WebsocketSettings(MenuBase):
    r"""Websocket Server (``h\s\k``)."""

    def start_ws_server(self) -> Result:
        r"""Start WS Server.

        Wire: ``h\s\k\r``

        Turn the websocket server on or off

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def w_s_server_port(self, value: int) -> Result:
        r"""WS Server Port.

        Wire: ``h\s\k\p``

        Set the TCP port the websocket server listens on

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(value)], [])

    def auth_mode(self, value: int) -> Result:
        r"""Auth Mode.

        Wire: ``h\s\k\m``

        Choose whether the websocket server allows open access or requires a username and password

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(value)], [])

    def auth_username(self, value: str) -> Result:
        r"""Auth Username.

        Wire: ``h\s\k\u``

        Set the username required to connect to the websocket server when basic authentication is enabled

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_str(value)], [])

    def auth_password(self, value: str) -> Result:
        r"""Auth Password.

        Wire: ``h\s\k\e``

        Set the password required to connect to the websocket server when basic authentication is enabled

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_str(value)], [])
