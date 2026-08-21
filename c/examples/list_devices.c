/* List FreeWili devices via the freewili-finder C API (cfwfinder).
 * Built only when ONEWILI_EXAMPLES=ON (CMake fetches freewili-finder).
 * Every out-parameter size here is an IN/OUT uint32_t*: you set it to the
 * buffer size and the call writes back the length it used. See
 * https://github.com/freewili/freewili-finder examples/c_api_basic_usage.c */
#include <cfwfinder.h>
#include <stdio.h>

int main(void) {
    char error_message[256] = {0};
    uint32_t error_message_size = sizeof error_message;
    fw_freewili_device_t* devices[16] = {0};
    uint32_t count = 16;

    fw_error_t err = fw_device_find_all(devices, &count,
                                        error_message, &error_message_size);
    if (err != fw_error_success) {
        printf("fw_device_find_all failed (%d): %s\n", (int)err, error_message);
        return 1;
    }
    printf("found %u device(s)\n", count);

    for (uint32_t i = 0; i < count; ++i) {
        char name[128] = {0}, serial[128] = {0};
        uint32_t name_size = sizeof name, serial_size = sizeof serial;
        if (!fw_device_is_valid(devices[i])) continue;
        if (fw_device_get_str(devices[i], fw_stringtype_name, name, &name_size)
                != fw_error_success) continue;
        if (fw_device_get_str(devices[i], fw_stringtype_serial, serial, &serial_size)
                != fw_error_success) continue;
        printf("%s (serial: %s)\n", name, serial);
    }
    fw_device_free(devices, count);
    return 0;
}
