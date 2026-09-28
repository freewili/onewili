cmake_minimum_required(VERSION 3.24)
project(onewili C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(onewili STATIC
    src/onewili.c
    src/onewili_events.c
    src/binary_framing.c
    src/binary_transport.c
    src/onewili_files.c
    src/onewili_stream.c)
target_include_directories(onewili PUBLIC include)

option(ONEWILI_EXAMPLES "Build the examples" ON)
if(ONEWILI_EXAMPLES)
    add_executable(toggle_gpio_25 examples/toggle_gpio_25.c examples/serial_pc.c)
    target_include_directories(toggle_gpio_25 PRIVATE examples)
    target_link_libraries(toggle_gpio_25 PRIVATE onewili)

    add_executable(toggle_gpio_25_loop examples/toggle_gpio_25_loop.c examples/serial_pc.c)
    target_include_directories(toggle_gpio_25_loop PRIVATE examples)
    target_link_libraries(toggle_gpio_25_loop PRIVATE onewili)

    add_executable(spi_write_read examples/spi_write_read.c examples/serial_pc.c)
    target_include_directories(spi_write_read PRIVATE examples)
    target_link_libraries(spi_write_read PRIVATE onewili)

    add_executable(watch_events examples/watch_events.c examples/serial_pc.c)
    target_include_directories(watch_events PRIVATE examples)
    target_link_libraries(watch_events PRIVATE onewili)

    # Device discovery example uses the freewili-finder C API. Its default
    # branch is master (there is no main), and its tests/examples default ON
    # -- turn them off so this build only pays for the C API.
    include(FetchContent)
    FetchContent_Declare(fwfinder
        GIT_REPOSITORY https://github.com/freewili/freewili-finder.git
        GIT_TAG        master)
    set(FW_BUILD_C_API ON CACHE BOOL "" FORCE)
    set(FW_FINDER_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(FW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(fwfinder)
    add_executable(list_devices examples/list_devices.c)
    # cfwfinder = "c" + its project name (c_api/CMakeLists.txt).
    target_link_libraries(list_devices PRIVATE cfwfinder)
endif()
