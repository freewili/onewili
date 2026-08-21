"""Audio Functions menu - generated from fwMenuAudio. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class Audio(MenuBase):
    r"""Audio Functions (``i\k``)."""

    #: Spontaneous event frames this menu emits (match Transport.events
    #: frames by id). payload lists (name, wire_type) pairs.
    EVENTS = {
        "record": {"binary": False, "payload": [("progress", "decU32")], "description": "Sound recording progress (permille of the clip length)"},
        "audio": {"binary": False, "payload": [("s0", "decS32"), ("s1", "decS32"), ("s2", "decS32"), ("s3", "decS32"), ("s4", "decS32"), ("s5", "decS32"), ("s6", "decS32"), ("s7", "decS32")], "description": "PDM microphone sample batch (8 signed samples per event)"},
    }

    def play_audio_file(self, file_path: str) -> Result:
        r"""Play Audio File.

        Wire: ``i\k\f``

        Plays a .wav file from the sounds directory.

        ## Play Audio File

Plays a `.wav` file from the device's `sounds/` directory through the built‑in speaker (or headphone jack, if connected).

### Argument
- `filePath` *(string)* — name or path of the file to play.
  - If no directory is given, `sounds/` is assumed.
  - If no extension is given, `.wav` is appended.
  - The file must already exist on the device's filesystem.

### Returns
- `success` *(basic)* — `true` if playback started, `false` if the file was not found or could not be opened.

### Examples
```
chime
chime.wav
\sounds\chime.wav
```

### Notes
- Only PCM `.wav` files are supported.
- Playback is asynchronous — the call returns as soon as playback begins.
- Use **Stream Audio** (`s`) to mirror audio to the host, or **Record Audio** (`r`) to capture from the microphone.

        Enter audio file name

        Args:
            file_path: file_path (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("f", [encoding.enc_str(file_path)], [])

    def record_audio_file(self, file_name: str) -> Result:
        r"""Record Audio.

        Wire: ``i\k\r``

        Records audio to a file (blank name = auto-named).

        Enter File Name or blank for auto file name.

        Args:
            file_name: file_name (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [encoding.enc_str(file_name)], [])

    def play_audio_asset(self, asset_name: str) -> Result:
        r"""Play Audio Asset.

        Wire: ``i\k\a``

        Plays a built-in audio asset by index or name.

        Enter asset index or name

        Args:
            asset_name: asset_name (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("a", [encoding.enc_str(asset_name)], [])

    def enable_audio_stream(self, enable: int) -> Result:
        r"""Stream Audio.

        Wire: ``i\k\s``

        Enables or disables audio streaming to the host.

        Enter 1 to enable 0 to disable

        Args:
            enable: enable (decS32).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [encoding.enc_int(enable)], [])

    def numbers_to_speech(self, number: float) -> Result:
        r"""Numbers to Speech.

        Wire: ``i\k\n``

        Speaks the given number aloud.

        Enter number

        Args:
            number: number (float).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("n", [encoding.enc_float(number)], [])

    def tone(self, frequency: float, duration_ms: float, amplitude: float) -> Result:
        r"""Play Tone.

        Wire: ``i\k\t``

        Plays a tone of the given frequency, duration, and amplitude.

        Enter frequency Duration and Amplitude

        Args:
            frequency: frequency (float).
            duration_ms: duration_ms (float).
            amplitude: amplitude (float).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [encoding.enc_float(frequency), encoding.enc_float(duration_ms), encoding.enc_float(amplitude)], [])

    def speak(self, text: str) -> Result:
        r"""Text to Speech.

        Wire: ``i\k\v``

        Speaks the given text aloud (text to speech).

        ## Text to Speech

Speaks the given text aloud through the built‑in speaker (or headphone jack, if connected) using the device's onboard text‑to‑speech engine.

### Argument
- `text` *(string)* — the text to be spoken. The entire remainder of the command line is treated as the text, so spaces and punctuation are preserved.
  - Maximum length: 128 characters.

### Returns
- `success` *(basic)* — `true` if the text was accepted and queued for playback, `false` if the input could not be parsed.

### Examples
```
hello world
The temperature is now seventy two degrees.
Warning: low battery
```

### Notes
- Playback is asynchronous — the call returns as soon as speech begins.
- To speak a numeric value, use **Numbers to Speech** (`n`) instead for proper digit/decimal pronunciation.
- Use **Play Tone** (`t`), **Play Audio File** (`f`), or **Play Audio Asset** (`a`) for non‑speech audio output.

        Enter text to speak

        Args:
            text: text (string).

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_str(text)], [])
