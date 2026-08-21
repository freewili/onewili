"""GUI Control Properties menu - generated from fwMenuGUIControlProperties. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class GUIControlProperties(MenuBase):
    r"""GUI Control Properties (``g\e``)."""

    def set_control_value_text(self, index: int, text: str) -> Result:
        r"""Set Control Value Text.

        Wire: ``g\e\a``

        sets the text value of a control

        Enter index, text

        Args:
            index: index (decS32).
            text: text (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(index), encoding.enc_str(text)], [])

    def set_control_value_int(self, index: int, value: int) -> Result:
        r"""Set Control Value Int.

        Wire: ``g\e\b``

        Set the text value of a control

        Enter index, value

        Args:
            index: index (decS32).
            value: value (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(index), encoding.enc_int(value)], [])

    def set_control_value_float(self, index: int, value: float) -> Result:
        r"""Set Control Value Float.

        Wire: ``g\e\c``

        Set the float value of the control.

        Enter index, value

        Args:
            index: index (decS32).
            value: value (float).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(index), encoding.enc_float(value)], [])

    def set_list_item_text(self, log_index: int, list_item: int, color: int, text: str) -> Result:
        r"""Set List Item Text.

        Wire: ``g\e\k``

        Sets the text and color of a specific list item

        Enter log index, list item, color, text

        Args:
            log_index: log_index (decS32).
            list_item: list_item (decS32).
            color: color (decS32).
            text: text (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("k", [encoding.enc_int(log_index), encoding.enc_int(list_item), encoding.enc_int(color), encoding.enc_str(text)], [])

    def set_control_value_min_max_int(self, index: int, enable: bool, min: int, max: int) -> Result:
        r"""Set Control Value Min Max Int.

        Wire: ``g\e\e``

        Sets whether a min and max is applied to a controls value

        Enter index, enable (0/1), min, max

        Args:
            index: index (decS32).
            enable: enable (bool).
            min: min (decS32).
            max: max (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("e", [encoding.enc_int(index), encoding.enc_bool(enable), encoding.enc_int(min), encoding.enc_int(max)], [])

    def set_control_value_min_max_float(self, index: int, enable: bool, min: float, max: float) -> Result:
        r"""Set Control Value Min Max Float.

        Wire: ``g\e\l``

        Sets whether a min and max is applied to a controls value

        Enter index, enable (0/1), min, max

        Args:
            index: index (decS32).
            enable: enable (bool).
            min: min (float).
            max: max (float).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_int(index), encoding.enc_bool(enable), encoding.enc_float(min), encoding.enc_float(max)], [])

    def set_plot_data(self, plot_data_index: int, settings: int, value: int) -> Result:
        r"""Set Plot Data.

        Wire: ``g\e\f``

        This adds data to a plot

        Enter plot data index, settings, value

        Args:
            plot_data_index: plot_data_index (decS32).
            settings: settings (decS32).
            value: value (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_int(plot_data_index), encoding.enc_int(settings), encoding.enc_int(value)], [])

    def set_list_item_selected(self, log_index: int, list_index: int) -> Result:
        r"""Set List Item Selected.

        Wire: ``g\e\g``

        This sets which item in a list is selected.

        Enter log index, list index

        Args:
            log_index: log_index (decS32).
            list_index: list_index (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("g", [encoding.enc_int(log_index), encoding.enc_int(list_index)], [])

    def set_list_item_top_index(self, log_item: int, list_index: int) -> Result:
        r"""Set List Item Top Index.

        Wire: ``g\e\i``

        This sets the first viewable item in the list. 

        Enter log item, list index

        Args:
            log_item: log_item (decS32).
            list_index: list_index (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_int(log_item), encoding.enc_int(list_index)], [])

    def set_control_property(self, index: int, property: int, value: int) -> Result:
        r"""Set Control Property.

        Wire: ``g\e\j``

        Sets a property based on a property type index

        Enter index, property, value

        Args:
            index: index (decS32).
            property: property (decS32).
            value: value (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("j", [encoding.enc_int(index), encoding.enc_int(property), encoding.enc_int(value)], [])
