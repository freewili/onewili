"""List connected FreeWili devices via pyfwfinder."""
import pyfwfinder

for device in pyfwfinder.find_all():
    print(f"{device.name} (serial: {device.serial})")
    for usb in device.usb_devices:
        print(f"  {usb.kind}: {usb.name}")
