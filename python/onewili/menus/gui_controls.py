"""GUI Controls menu - generated from fwMenuGUIControls. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class GUIControls(MenuBase):
    r"""GUI Controls (``g\b``)."""

    def add_led(self, index: int, x: int, y: int, color: int, size: int, inital_value: bool) -> Result:
        r"""Add LED.

        Wire: ``g\b\a``

        Add a LED control to the panel.

        Enter index, x, y, color, size, inital value (0/1)

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            color: color (decS32).
            size: size (decS32).
            inital_value: inital_value (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(color), encoding.enc_int(size), encoding.enc_bool(inital_value)], [])

    def add_log_list(self, index: int, log: int, x: int, y: int, width: int, height: int, font_type: int, font_size: int, back_color: int | str, fore_color: int | str, list_mode: bool) -> Result:
        r"""Add LogList.

        Wire: ``g\b\b``

        Adds a Log control or a list control to the panel.

        Enter index, log, x, y, width, height, font type, font size, back color, fore color, list mode (0/1)

        Args:
            index: index (decS32).
            log: log (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            font_type: font_type (decS32).
            font_size: font_size (decS32).
            back_color: back_color (color).
            fore_color: fore_color (color).
            list_mode: list_mode (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(index), encoding.enc_int(log), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_int(font_type), encoding.enc_int(font_size), encoding.enc_color(back_color), encoding.enc_color(fore_color), encoding.enc_bool(list_mode)], [])

    def add_plot(self, index: int, plot_data_index_bit_field: int, x: int, y: int, width: int, height: int, min_y: int, max_y: int, back_color: int | str) -> Result:
        r"""Add Plot.

        Wire: ``g\b\c``

        Adds a plot to the panel.

        Enter index, plot data index bit field, x, y, width, height, min y, max y, back color

        Args:
            index: index (decS32).
            plot_data_index_bit_field: plot_data_index_bit_field (decU32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            min_y: min_y (decS32).
            max_y: max_y (decS32).
            back_color: back_color (color).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(index), encoding.enc_int(plot_data_index_bit_field), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_int(min_y), encoding.enc_int(max_y), encoding.enc_color(back_color)], [])

    def add_number(self, index: int, x: int, y: int, width: int, font_type: int, font_size: int, fore_color: int | str, back_color: int | str, is_float: bool, float_digit_count: int, is_hex_format: bool, is_unsigned: bool) -> Result:
        r"""Add Number.

        Wire: ``g\b\l``

        add a numeric control to a panel

        Enter index, x, y, width, font type, font size, fore color, back color, is float (0/1), float digit count, is hex format (0/1), is unsigned (0/1)

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            font_type: font_type (decS32).
            font_size: font_size (decS32).
            fore_color: fore_color (color).
            back_color: back_color (color).
            is_float: is_float (bool).
            float_digit_count: float_digit_count (decS32).
            is_hex_format: is_hex_format (bool).
            is_unsigned: is_unsigned (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(font_type), encoding.enc_int(font_size), encoding.enc_color(fore_color), encoding.enc_color(back_color), encoding.enc_bool(is_float), encoding.enc_int(float_digit_count), encoding.enc_bool(is_hex_format), encoding.enc_bool(is_unsigned)], [])

    def add_text(self, index: int, x: int, y: int, font_type: int, font_size: int, fore_color: int | str, back_color: int | str, text: str) -> Result:
        r"""Add Text.

        Wire: ``g\b\e``

        Add static text to the panel

        Enter index, x, y, font type, font size, fore color, back color, text

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            font_type: font_type (decS32).
            font_size: font_size (decS32).
            fore_color: fore_color (color).
            back_color: back_color (color).
            text: text (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(font_type), encoding.enc_int(font_size), encoding.enc_color(fore_color), encoding.enc_color(back_color), encoding.enc_str(text)], [])

    def add_bargraph(self, index: int, x: int, y: int, width: int, height: int, min: int, max: int, bar_color: int | str) -> Result:
        r"""Add Bargraph.

        Wire: ``g\b\f``

        Add a bar graph to a panel.

        Enter index, x, y, width, height, min, max, bar color

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            min: min (decS32).
            max: max (decS32).
            bar_color: bar_color (color).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_int(min), encoding.enc_int(max), encoding.enc_color(bar_color)], [])

    def add_meter(self, index: int, x: int, y: int, width: int, height: int, min: int, max: int, needle_color: int | str) -> Result:
        r"""Add Meter.

        Wire: ``g\b\g``

        Add a Meter control to a panel

        Enter index, x, y, width, height, min, max, needle color

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            min: min (decS32).
            max: max (decS32).
            needle_color: needle_color (color).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_int(min), encoding.enc_int(max), encoding.enc_color(needle_color)], [])

    def add_button(self, index: int, x: int, y: int, width: int, height: int, fore_color: int | str, back_color: int | str, text: str) -> Result:
        r"""Add Button.

        Wire: ``g\b\i``

        Add a button control to a panel

        Enter index, x, y, width, height, fore color, back color, text

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            fore_color: fore_color (color).
            back_color: back_color (color).
            text: text (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_color(fore_color), encoding.enc_color(back_color), encoding.enc_str(text)], [])

    def add_picture(self, index: int, x: int, y: int, picture_id: int) -> Result:
        r"""Add Picture.

        Wire: ``g\b\j``

        Shows a ROM picture on the panel.

        Enter index, x, y, picture id

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            picture_id: picture_id (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(picture_id)], [])

    def add_picture_from_file(self, index: int, x: int, y: int, picture_path: str) -> Result:
        r"""Add Picture From File.

        Wire: ``g\b\k``

        Loads a picture from the file system

        Enter index, x, y, picture path

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            picture_path: picture_path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_str(picture_path)], [])

    def add_waterfall(self, index: int, plot_data_index: int, bin_count: int, x: int, y: int, width: int, height: int, back_color: int | str) -> Result:
        r"""Add Waterfall.

        Wire: ``g\b\m``

        Adds an FFT waterfall (spectrogram) control to the panel. Rows commit when the control value changes.

        Enter index, plot data index, bin count, x, y, width, height, back color

        Args:
            index: index (decS32).
            plot_data_index: plot_data_index (decS32).
            bin_count: bin_count (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            back_color: back_color (color).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(index), encoding.enc_int(plot_data_index), encoding.enc_int(bin_count), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_color(back_color)], [])

    def add_wili8(self, index: int, x: int, y: int, width: int, height: int, scale: int, back_color: int | str, animation: int, script_path: str) -> Result:
        r"""Add Wili8.

        Wire: ``g\b\n``

        Adds a clipped, integer-scaled Wili8 canvas control. Animation 0 is Wave; 255 stores ScriptPath for future custom execution.

        Enter index, x, y, width, height, scale, back color, animation (0 Wave; 255 custom), script path (use "" for empty wave path)

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            scale: scale (decS32).
            back_color: back_color (color).
            animation: animation (decU32).
            script_path: script_path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_int(scale), encoding.enc_color(back_color), encoding.enc_int(animation), encoding.enc_str(script_path)], [])

    def add_file_list(self, index: int, x: int, y: int, width: int, height: int, mode: int, back_color: int | str, start_path: str, filter: str) -> Result:
        r"""Add File List.

        Wire: ``g\b\o``

        Adds a device-fed SD/flash file browser control. Activating a file (or OK in pick dir mode) raises a filepicked event with the full path.

        Enter index, x, y, width, height, mode (0 browse; 1 pick file; 2 pick dir), back color, start path, filter (ext;ext, "" for all)

        Args:
            index: index (decS32).
            x: x (decS32).
            y: y (decS32).
            width: width (decS32).
            height: height (decS32).
            mode: mode (decU32).
            back_color: back_color (color).
            start_path: start_path (string).
            filter: filter (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [encoding.enc_int(index), encoding.enc_int(x), encoding.enc_int(y), encoding.enc_int(width), encoding.enc_int(height), encoding.enc_int(mode), encoding.enc_color(back_color), encoding.enc_str(start_path), encoding.enc_str(filter)], [])
