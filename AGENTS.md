# Working on OneWili

The language packages and `docs/` are generated from FreeWili firmware menus.
Change the firmware menu sources or menutool generator, then regenerate and
sync the output. Do not patch generated bindings in this repository.

Root documentation, CI, and runtime regression tests are maintained here.
Preserve existing hand-maintained Python examples/tests during a sync.

Run `python -m pytest python/tests tests -q`, configure/build/CTest in `tests/`,
and `cargo test --manifest-path rust/Cargo.toml`. CI covers Windows, macOS,
and Linux. Hardware coverage and transport limitations belong in release notes.

CM0 mailbox commands and framed SD upload/download require MAIN protocol 1.2+
and the matching bridge. CM0 does not route FTDI binary events or USB directory
listing. Do not advertise those as mailbox capabilities. See `cm0/README.md`.

Applications for CM0 Linux can use the
[WiliCM0BSP](https://github.com/freewili/wilicm0bsp) and install into `/home/apps/`
on the Linux filesystem for the on-device Linux Apps launcher.
