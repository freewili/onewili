"""Connect to a FreeWili and walk the generated OneWili menu tree."""
import onewili
from onewili.menubase import MenuBase


def walk(obj, path: str = "dev") -> None:
    for name in sorted(vars(obj)):
        child = getattr(obj, name)
        if isinstance(child, MenuBase):
            print(f"{path}.{name}:")
            for m in sorted(type(child).__dict__):
                if not m.startswith("_"):
                    print(f"  {path}.{name}.{m}()")
            walk(child, f"{path}.{name}")


dev = onewili.connect()
try:
    walk(dev)
finally:
    dev.close()
