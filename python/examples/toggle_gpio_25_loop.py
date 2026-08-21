"""Toggle GPIO 25 once a second (default 10 times; pass a count)."""
import sys
import time

import onewili

count = int(sys.argv[1]) if len(sys.argv) > 1 else 10
dev = onewili.connect()
try:
    for i in range(1, count + 1):
        res = dev.io.gpio.set_io_toggle(25)
        print(f"[{i}/{count}] toggle GPIO 25: {res}")
        if i < count:
            time.sleep(1.0)
finally:
    dev.close()
