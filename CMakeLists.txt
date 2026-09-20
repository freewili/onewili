# OneWili C API for WiliBSP (FreeWili 2 display CPU, RP2350B).
# COPY — source of truth: freewili-firmware/freewilimain/testprojects/onewili/wilibsp (menutool-generated; re-copy on regeneration, do not edit here).
# Local additions that survive a re-copy are tagged "LOCAL ADDITION" — grep for
# it before and after regenerating: today that is ow_fwgui_send_power_zones().
# Drop this directory into your wilibsp tree and add_subdirectory() it, or
# point an out-of-tree app at it. Requires the pico-sdk (already initialized
# by the wilibsp top-level CMakeLists).
cmake_minimum_required(VERSION 3.20)

add_library(onewili_fwgui STATIC
    src/onewili.c
    src/onewili_events.c
    src/binary_framing.c
    src/binary_transport.c
    src/onewili_fwgui.c
    src/onewili_fast.c
    src/onewili_sd.c
    src/sdfs_client.c
    src/sdfs_codec.c
)
target_include_directories(onewili_fwgui PUBLIC include)
target_link_libraries(onewili_fwgui PUBLIC pico_stdlib hardware_uart hardware_gpio hardware_irq)

# SDFS wire tunables. These MUST match the values fw2main is built with
# (freewilimain/sdfslib/CMakeLists.txt) -- a mismatch does not fail to build,
# it silently truncates paths and desyncs chunking against the server.
# onewili_sd.c carries an #error guard so a divergence is caught at build time.
target_compile_definitions(onewili_fwgui PUBLIC
    SDFS_MAX_PAYLOAD=96
    SDFS_MAX_PATH=128
)
set_property(TARGET onewili_fwgui PROPERTY C_STANDARD 11)
