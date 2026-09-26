/* Build against regenerated c/src/{onewili,onewili_events,binary_framing,binary_transport}.c. */
#include "onewili_binary.h"
#ifdef NDEBUG
#undef NDEBUG /* assertions perform the test calls in Release configurations too */
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct input { const uint8_t* data; size_t size, pos, fragment; } input;
static int read_bytes(void* ctx, uint8_t* data, size_t cap, uint32_t timeout) {
    input* in = (input*)ctx;
    size_t n = in->size - in->pos;
    assert(timeout == 0);
    if (n > cap) n = cap;
    if (n > in->fragment) n = in->fragment;
    memcpy(data, in->data + in->pos, n);
    in->pos += n;
    return (int)n;
}
static void le32(uint8_t* p, uint32_t value) {
    unsigned i;
    for (i = 0; i < 4; ++i) p[i] = (uint8_t)(value >> (8*i));
}
static void header(uint8_t* wire, uint16_t type, uint16_t repeat, uint32_t length) {
    memcpy(wire, "WILI", 4);
    wire[4] = (uint8_t)repeat; wire[5] = (uint8_t)(repeat >> 8);
    wire[6] = (uint8_t)type; wire[7] = (uint8_t)(type >> 8);
    le32(wire+8, length);
}
int main(void) {
    uint8_t* wire = (uint8_t*)calloc(1, OW_BIN_CAPTURE_CAPACITY + 12);
    uint8_t* storage = (uint8_t*)malloc(OW_BIN_CAPTURE_CAPACITY);
    ow_binary_device dev;
    ow_bin_frame raw;
    ow_event event;
    input in = {wire, OW_BIN_CAPTURE_CAPACITY + 12, 0, 113};
    ow_transport t = {&in, NULL, read_bytes};
    int status = 0;
    size_t polls = 0;
    assert(wire && storage);
    header(wire, 2, 7, OW_BIN_CAPTURE_CAPACITY | 0x80000000u);
    le32(wire+12+8, 1000);
    wire[12+12] = 25; wire[12+13] = 1;
    memset(wire+12+44, 0xAA, OW_BIN_CAPTURE_CAPACITY - 44);
    assert(ow_binary_open_buffer(&dev, &t, storage, OW_BIN_CAPTURE_CAPACITY) == OW_OK);
    while ((status = ow_binary_poll_raw(&dev, &raw)) == 0 && ++polls < 1000) {}
    assert(status == 1 && polls > 1);
    assert(raw.header_type == 2 && raw.repeat_count == 7 && raw.error);
    assert(raw.payload_len == OW_BIN_CAPTURE_CAPACITY);
    assert(memcmp(raw.payload, wire+12, raw.payload_len) == 0);
    assert(ow_decode_logic_analyzer_report(raw.payload, raw.payload_len, raw.error,
                                           &event.u.logic_analyzer_report) == OW_OK);
    assert(event.u.logic_analyzer_report.sample_rate_ns == 1000);
    assert(event.u.logic_analyzer_report.sample_bytes == OW_BIN_CAPTURE_CAPACITY - 44);
    assert(event.u.logic_analyzer_report.sample_data[1048575] == 0xAA);
    in.pos = 0;
    assert(ow_binary_open_buffer(&dev, &t, storage, OW_BIN_CAPTURE_CAPACITY) == OW_OK);
    polls = 0;
    while ((status = ow_binary_poll(&dev, &event)) == 0 && ++polls < 1000) {}
    assert(status == 1 && event.kind == OW_EV_LOGIC_ANALYZER_REPORT);
    assert(dev.size_mismatches == 0);
    /* Unknown frames, embedded markers, zero length, and a following CAN FD frame. */
    header(wire, 65535, 1234, 7 | 0x80000000u);
    memcpy(wire+12, "\0WILI\377\1", 7);
    header(wire+19, 999, 0, 0);
    header(wire+31, 1, 0, 84);
    memset(wire+43, 0, 84);
    le32(wire+43+12, 0x1FFFFFFF);
    memset(wire+43+20, 0x5A, 64);
    in.size = 127; in.pos = 0; in.fragment = 1;
    assert(ow_binary_open(&dev, &t) == OW_OK);
    assert(ow_binary_poll_raw(&dev, &raw) == 1);
    assert(raw.header_type == 65535 && raw.repeat_count == 1234 && raw.error);
    assert(raw.payload_len == 7 && !memcmp(raw.payload, "\0WILI\377\1", 7));
    assert(ow_binary_poll_raw(&dev, &raw) == 1 && raw.payload_len == 0);
    assert(ow_binary_poll(&dev, &event) == 1 && event.kind == OW_EV_CAN_RX_REPORT);
    assert(event.u.can_rx_report.r0_canid == 0x1FFFFFFF);
    assert(event.u.can_rx_report.data_words[15] == 0x5A5A5A5A);
    assert(ow_binary_poll(&dev, &event) == 0);
    ow_binary_close(&dev);
    assert(ow_binary_poll_raw(&dev, &raw) == -(int)OW_ERR_ARG);
    free(storage); free(wire);
    puts("C binary streaming: maximum capture, fragmented raw/typed frames, CAN FD and close passed");
    return 0;
}
