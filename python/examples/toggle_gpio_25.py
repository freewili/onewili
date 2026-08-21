"""Toggle GPIO 25 on a FreeWili."""
import onewili

dev = onewili.connect()
try:
    res = dev.io.gpio.set_io_toggle(25)
    print(res)
finally:
    dev.close()
