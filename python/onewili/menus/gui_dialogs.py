"""Dialogs menu - generated from fwMenuGUIDialogs. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class GUIDialogs(MenuBase):
    r"""Dialogs (``g\f``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "filepicked": {"binary": False, "payload": [("panel", "decS32"), ("control", "decS32"), ("picked", "bool"), ("path", "string")], "description": "File list / file picker result"},
    }

    def message_box(self, auto_close_half_sec: int, show_ok: bool, show_ok_cancel: bool, show_none: bool, picture_index: int, message: str) -> Result:
        r"""Message Box.

        Wire: ``g\f\a``

        Shows a message box with optional buttons and auto close timer. 

        Enter auto close half sec (0=disabled), show ok (0/1), show ok cancel (0/1), show none (0/1), picture index, message

        Args:
            auto_close_half_sec: auto_close_half_sec (decS32).
            show_ok: show_ok (bool).
            show_ok_cancel: show_ok_cancel (bool).
            show_none: show_none (bool).
            picture_index: picture_index (decS32).
            message: message (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(auto_close_half_sec), encoding.enc_bool(show_ok), encoding.enc_bool(show_ok_cancel), encoding.enc_bool(show_none), encoding.enc_int(picture_index), encoding.enc_str(message)], [])

    def set_dialog_description(self, description: str) -> Result:
        r"""Set Dialog Description.

        Wire: ``g\f\b``

        Sets the description of the dialog.

        Enter description

        Args:
            description: description (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_str(description)], [])

    def progress_bar(self, picture_index: int, ok_to_close: bool, auto_close_at100: bool, auto_close_half_sec: int, title: str) -> Result:
        r"""Progress Bar.

        Wire: ``g\f\c``

        shows a dialog with a progress bar

        Enter picture index, ok to close (0/1), auto close at100 (0/1), auto close half sec, title

        Args:
            picture_index: picture_index (decS32).
            ok_to_close: ok_to_close (bool).
            auto_close_at100: auto_close_at100 (bool).
            auto_close_half_sec: auto_close_half_sec (decS32).
            title: title (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(picture_index), encoding.enc_bool(ok_to_close), encoding.enc_bool(auto_close_at100), encoding.enc_int(auto_close_half_sec), encoding.enc_str(title)], [])

    def number_edit(self, min: int, max: int, initial: int, use_min_max: bool, is_unsigned: bool, hex_fomat: bool, message: str) -> Result:
        r"""Number Edit.

        Wire: ``g\f\k``

        Shows a dialog box to edit numbers

        Enter min, max, initial, use min max (0/1), is unsigned (0/1), hex fomat (0/1), message

        Args:
            min: min (decS32).
            max: max (decS32).
            initial: initial (decS32).
            use_min_max: use_min_max (bool).
            is_unsigned: is_unsigned (bool).
            hex_fomat: hex_fomat (bool).
            message: message (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(min), encoding.enc_int(max), encoding.enc_int(initial), encoding.enc_bool(use_min_max), encoding.enc_bool(is_unsigned), encoding.enc_bool(hex_fomat), encoding.enc_str(message)], [])

    def number_edit_float(self, min: float, max: float, initial: float, use_min_max: bool, digit_count: int, message: str) -> Result:
        r"""Number Edit Float.

        Wire: ``g\f\e``

        Shows a dialog to enter a float number

        Enter min, max, initial, use min max (0/1), digit count, message

        Args:
            min: min (float).
            max: max (float).
            initial: initial (float).
            use_min_max: use_min_max (bool).
            digit_count: digit_count (decS32).
            message: message (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_float(min), encoding.enc_float(max), encoding.enc_float(initial), encoding.enc_bool(use_min_max), encoding.enc_int(digit_count), encoding.enc_str(message)], [])

    def text_edit(self, message: str, inital_value: str) -> Result:
        r"""Text Edit.

        Wire: ``g\f\f``

        Shows a dialog to edit a text value.

        Enter message (in quotes), inital value

        Args:
            message: message (string).
            inital_value: inital_value (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_str(message), encoding.enc_str(inital_value)], [])

    def pick_list(self, log_index: int, message: str) -> Result:
        r"""Pick List.

        Wire: ``g\f\g``

        Shows a list of items to pick from. The list of items is loaded into a log.

        Enter log index, message

        Args:
            log_index: log_index (decS32).
            message: message (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_int(log_index), encoding.enc_str(message)], [])

    def show_text_editor(self, editor_type: int, message: str, inital_value: str) -> Result:
        r"""Show Text Editor.

        Wire: ``g\f\i``

        Shows a full screen text editor.

        Enter editor type, message (in quotes), inital value

        Args:
            editor_type: editor_type (decS32).
            message: message (string).
            inital_value: inital_value (string).

        Returns:
            Result: Ok(basic: bool) or Err(message).
        """
        return self._call("i", [encoding.enc_int(editor_type), encoding.enc_str(message), encoding.enc_str(inital_value)], ["bool"])

    def set_progess_dialog_value(self, value0_to100: int) -> Result:
        r"""Set Progess Dialog Value.

        Wire: ``g\f\j``

        Sets the value of progress on the dialog

        Enter value0to100 (0-100)

        Args:
            value0_to100: value0_to100 (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_int(value0_to100)], [])

    def file_picker(self, mode: int, start_path: str, filter: str) -> Result:
        r"""File Picker.

        Wire: ``g\f\l``

        Shows a full screen file browser dialog over the current view. The chosen path (or cancel) returns as a filepicked event.

        Enter mode (0 browse; 1 pick file; 2 pick dir), start path, filter (ext;ext, "" for all)

        Args:
            mode: mode (decU32).
            start_path: start_path (string).
            filter: filter (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_int(mode), encoding.enc_str(start_path), encoding.enc_str(filter)], [])
