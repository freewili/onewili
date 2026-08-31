cmake_minimum_required(VERSION 3.24)
project(onewili_lv C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# The OneWili C package, compiled straight into the DLL. Nothing from it is
# exported: onewili_lv.def lists only the owlv_* entry points, so LabVIEW sees
# a flat, session-handle ABI and never a raw ow_device*.
set(ONEWILI_C ${CMAKE_CURRENT_SOURCE_DIR}/../c)

add_library(onewili_lv SHARED
    src/onewili_lv.c
    src/onewili_lv_api.c
    ${ONEWILI_C}/src/onewili.c
    ${ONEWILI_C}/src/onewili_events.c
    ${ONEWILI_C}/src/binary_framing.c
    ${ONEWILI_C}/src/binary_transport.c
    ${ONEWILI_C}/src/onewili_files.c
    ${ONEWILI_C}/examples/serial_pc.c)

target_include_directories(onewili_lv PRIVATE
    include
    src
    ${ONEWILI_C}/include
    ${ONEWILI_C}/examples)

set_target_properties(onewili_lv PROPERTIES
    OUTPUT_NAME onewili_lv
    PREFIX ""                       # libonewili_lv.dll -> onewili_lv.dll on MinGW
    C_VISIBILITY_PRESET hidden)

# The scripted loopback transport the Python test suite drives. OFF for any
# build you ship: it adds owlv_test_* entry points and, on Windows, drops the
# curated export list in favour of exporting everything.
option(ONEWILI_LV_LOOPBACK "Add the loopback transport used by the test suite" OFF)
if(ONEWILI_LV_LOOPBACK)
    target_sources(onewili_lv PRIVATE src/onewili_lv_mock.c)
    set_target_properties(onewili_lv PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(WIN32)
    # Explicit export list. Keeps the DLL's export table to exactly the
    # LabVIEW ABI, and keeps the names undecorated in the 32-bit build.
    if(NOT ONEWILI_LV_LOOPBACK)
        target_sources(onewili_lv PRIVATE onewili_lv.def)
    endif()
    target_link_libraries(onewili_lv PRIVATE setupapi advapi32)
    target_compile_definitions(onewili_lv PRIVATE
        _CRT_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN)
    if(MSVC)
        target_compile_options(onewili_lv PRIVATE /W3)
    endif()
else()
    find_package(Threads REQUIRED)
    target_link_libraries(onewili_lv PRIVATE Threads::Threads)
    target_compile_options(onewili_lv PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# A pure-C exercise of the LabVIEW ABI. Build and run it before touching
# LabVIEW: if this fails, the problem is not LabVIEW.
option(ONEWILI_LV_SELFTEST "Build the C self-test for the LabVIEW ABI" ON)
if(ONEWILI_LV_SELFTEST)
    add_executable(owlv_selftest examples/selftest.c)
    target_include_directories(owlv_selftest PRIVATE include)
    target_link_libraries(owlv_selftest PRIVATE onewili_lv)
endif()

install(TARGETS onewili_lv RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(FILES include/onewili_lv.h include/onewili_lv_api.h DESTINATION include)
