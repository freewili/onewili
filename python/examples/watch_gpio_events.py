"""Watch decoded binary events from the FTDI port (Intrepid FW2)."""
import sys
import time

import onewili

seconds = float(sys.argv[1]) if len(sys.argv) > 1 else 5.0
dev = onewili.connect(binary=True)
try:
    dev.io.gpio.stream_io(10)   # 10 ms report rate
    deadline = time.monotonic() + seconds
    count = 0
    while time.monotonic() < deadline:
        try:
            evt = dev.binary_events.get(timeout=0.5)
        except Exception:
            continue
        count += 1
        print(evt)
    print(f"{count} events in {seconds} s (unknown frames: {dev._binary.unknown_frames}, size mismatches: {dev._binary.size_mismatches})")
    dev.io.gpio.stream_io(0)   # stop the stream
finally:
    dev.close()
