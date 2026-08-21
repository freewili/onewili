"""WILEye Functions menu - generated from fwMenuWILEye. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class WILEye(MenuBase):
    r"""WILEye Functions (``i\f``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "WILEye": {"binary": False, "payload": [("data_bytes", "hexbytes")], "description": "DEPRECATED: never emitted; kept for wire compatibility (see WILEyeAI)"},
        "WILEyeImgStart": {"binary": False, "payload": [("data", "string")], "description": "WILEye image transfer started ('Image Stream Start: N bytes')"},
        "WILEyeImgChunk": {"binary": False, "payload": [("data", "string")], "description": "WILEye image chunk received ('N bytes')"},
        "WILEyeImgEnd": {"binary": False, "payload": [("data", "string")], "description": "WILEye image transfer complete ('saved as: <file>')"},
        "WILEyeImgAbort": {"binary": False, "payload": [("data", "string")], "description": "WILEye image transfer aborted (timeout)"},
        "WILEyeSDcard": {"binary": False, "payload": [("data", "string")], "description": "WILEye SD card switched to USB mode"},
        "WILEyeAI": {"binary": False, "payload": [("data", "string")], "description": "WILEye AI detection event (mode + bounding box text)"},
        "WILEyeUnknown": {"binary": False, "payload": [("data", "string")], "description": "WILEye unknown message received"},
    }

    def take_picture(self, destination: int, filename: str) -> Result:
        r"""Take a Picture.

        Wire: ``i\f\t``

        Take a picture from WILEye and save its SD card or FREE-WILi's Files system by file name.

        Enter Destination as a Dec (0=SDCard, 1=FreeWili) and String for file name, separated by space

        Args:
            destination: destination (decS32).
            filename: filename (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_int(destination), encoding.enc_str(filename)], [])

    def start_recording_video(self, filename: str) -> Result:
        r"""Start Recording Video.

        Wire: ``i\f\v``

        Start recording video from WILEye and save it to SD card by file name

        Enter file name for video (Will be saved on WILEye's SD card)

        Args:
            filename: filename (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_str(filename)], [])

    def stop_recording_video(self) -> Result:
        r"""Stop Recording Video.

        Wire: ``i\f\s``

        Stop recording video from WILEye

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def toggle_ai_detection_stream(self, ai_stream_mode: int) -> Result:
        r"""Stream AI Detection Events.

        Wire: ``i\f\a``

        Stream AI Detection Events from WILEye

        Enter AI Stream Mode (0=Off, 1=Pedestrian, 2=Face)

        Args:
            ai_stream_mode: ai_stream_mode (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_int(ai_stream_mode)], [])

    def set_zoom_level(self, zoom: int) -> Result:
        r"""Set Zoom.

        Wire: ``i\f\m``

        Set the zoom level of WILEye

        Enter Zoom Level 1-4

        Args:
            zoom: zoom (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("m", [encoding.enc_int(zoom)], [])

    def set_contrast(self, contrast: int) -> Result:
        r"""Set Contrast.

        Wire: ``i\f\c``

        Set the contrast level of WILEye

        Enter Contrast percentages 0-100

        Args:
            contrast: contrast (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_int(contrast)], [])

    def set_saturation(self, saturation: int) -> Result:
        r"""Set Saturation.

        Wire: ``i\f\i``

        Set the saturation level of WILEye

        Enter Saturation percentages 0-100

        Args:
            saturation: saturation (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [encoding.enc_int(saturation)], [])

    def set_brightness(self, brightness: int) -> Result:
        r"""Set Brightness.

        Wire: ``i\f\b``

        Set the brightness level of WILEye

        Enter Brightness percentages 0-100

        Args:
            brightness: brightness (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("b", [encoding.enc_int(brightness)], [])

    def set_hue(self, hue: int) -> Result:
        r"""Set Hue.

        Wire: ``i\f\u``

        Set the hue level of WILEye

        Enter Hue percentages 0-100

        Args:
            hue: hue (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("u", [encoding.enc_int(hue)], [])

    def set_resolution(self, resolutionstate: int) -> Result:
        r"""Set Resolution.

        Wire: ``i\f\y``

        Set the resolution state of WILEye

        Enter Resolution Selection (0=640x480, 1=1280x720, 2=1920x1080)

        Args:
            resolutionstate: resolutionstate (dec).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("y", [encoding.enc_int(resolutionstate)], [])

    def set_flash_state(self, flash: bool) -> Result:
        r"""Enable Disable Flash.

        Wire: ``i\f\l``

        Set the flash state of WILEye

        Enter Flash State (0=Off, 1=On)

        Args:
            flash: flash (bool).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("l", [encoding.enc_bool(flash)], [])
