"""niceusb menu - generated from fwMenuNiceUsb. Do not edit."""
from __future__ import annotations

from result import Result

from .. import encoding
from ..menubase import MenuBase
from ..transport import Transport


class NiceUsb(MenuBase):
    r"""niceusb (``i\n``)."""

    def nice_usb_start(self) -> Result:
        r"""Start Gadget.

        Wire: ``i\n\s``

        Attaches the USB HID gadget on the second USB port, so the host sees a keyboard, mouse and gamepad. Returns as soon as the display accepts the request; the host takes a moment longer to enumerate. Refused while SubGHz is in use.

        Brings up the USB HID gadget on the FreeWili's second USB connector. The host PC then sees a composite keyboard/mouse/gamepad device that this firmware can type on.

Starting the gadget costs three things for as long as it stays up:

1. SubGHz is locked out. The gadget and the CC1101 radio share PIO block 2 and cannot both have it, so Start is refused outright while the SubGHz app is open or the radio is mid-operation. Close it and try again.
2. The display's screen becomes the gadget status screen and the menu tree is unreachable on the glass. That is deliberate, not a fault: the framebuffer memory is where the running USB code lives while the gadget is attached. CANCEL on the device stops the gadget and gives the screen back.
3. LoRa keeps working throughout; only the sub-GHz CC1101 path is affected.

Success here means the request was ACCEPTED, not that the host enumerated the device. Run Status to see which of those actually happened - it distinguishes 'attached, no host' (a cable or host-side problem) from 'enumerated' (working).

Starting is also safe from this console while the device is showing any screen at all, because it does not go through the menu tree.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("s", [], [])

    def nice_usb_stop(self) -> Result:
        r"""Stop Gadget.

        Wire: ``i\n\t``

        Detaches the USB HID gadget, gives the display screen back and releases the SubGHz lockout. A stop asked for while a script is running takes effect when that script finishes.

        Takes the USB HID gadget down. The host sees a clean detach, the display returns to the normal menus, and SubGHz becomes available again without a reboot.

The stop is LATCHED rather than immediate. If a typing script is running when you ask, the gadget stays attached until that script has finished - stopping mid-transfer would leave the host with a half-sent report and, worse, modifier keys held down. Status reports 'stopping' during that window.

Success here means the stop was accepted. Run Status afterwards to confirm it reached 'stopped'; only then has the SubGHz lockout actually cleared.

The same thing can be done on the device itself: while the gadget is attached the display shows the gadget screen, and CANCEL there stops it. That is the only button the device honours in that state.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("t", [], [])

    def nice_usb_status(self) -> Result:
        r"""niceusb Status.

        Wire: ``i\n\i``

        Reports the gadget state as the display holds it, whether a script is running, whether SubGHz is locked out, and the last script error.

        Asks the display for the gadget's live state and prints it here. The report is taken as a single snapshot, so the state and the enumeration flag it prints always belong to the same instant.

The state word is one of:

  stopped                  - nothing running, SubGHz is free
  starting (claiming PIO2) - the request was accepted; the USB stack is not up yet
  starting (stack up, D+ low) - the stack is up and the pull-up is about to be raised
  attached, no host        - the device is presenting itself but nothing enumerated it. Suspect the cable, the port, or the host.
  enumerated               - the host has accepted the device. This is the working state.
  stopping                 - teardown in flight; SubGHz is still locked out

'attached, no host' and 'enumerated' are never collapsed into one word, because they have completely different causes and completely different fixes.

The second line reports the last script error as a line number and message, or 'none'. It is not cleared by stopping the gadget, so it still names the failure after the fact.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("i", [], [])

    def nice_usb_run_test_script(self) -> Result:
        r"""Run Test Script.

        Wire: ``i\n\r``

        Types a two-line proof script on the host, three seconds apart. The gadget must already be attached. Types into whatever window has focus on the host.

        Runs the compiled-in proof script on the host PC. It types 'FreeWili niceusb OK', presses Enter, waits three seconds, types 'still alive' and presses Enter again.

THIS TYPES INTO WHATEVER WINDOW HAS FOCUS on the host. Put the cursor somewhere harmless - a text editor or an empty document - before running it. The text is deliberately inert: plain words only, no shell metacharacters and no GUI/CTRL/ALT combinations.

Each part of the script proves something separate. The multi-character typing proves the IN endpoint is being re-armed between reports; the shifted characters prove modifiers work; and the three-second delay proves the watchdog beat inside the USB pump holds while a script blocks the display's second core.

Refused if the gadget is not attached (start it first) or if a script is already running. The command returns as soon as the script is queued - the typing happens on the display over the next few seconds.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("r", [], [])

    def niceusb_script(self, value: str) -> Result:
        r"""niceusb Script.

        Wire: ``i\n\c``

        SD path of the wusb script Start Gadget loads, or empty for the gadget's built-in personality.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("c", [encoding.enc_str(value)], [])

    def niceusb_force_vidpid(self) -> Result:
        r"""niceusb Force VID/PID.

        Wire: ``i\n\o``

        Enable to make Start Gadget present the VID/PID below instead of the script's own or the built-in personality's.

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("o", [], [])

    def niceusb_vid(self, value: int) -> Result:
        r"""niceusb VID.

        Wire: ``i\n\v``

        Forced USB vendor ID, used only while niceusb Force VID/PID is on.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("v", [encoding.enc_int(value)], [])

    def niceusb_pid(self, value: int) -> Result:
        r"""niceusb PID.

        Wire: ``i\n\p``

        Forced USB product ID, used only while niceusb Force VID/PID is on.

        Args:
            value: value ().

        Returns:
            Result: Ok(None) or Err(message).
        """
        return self._call("p", [encoding.enc_int(value)], [])
