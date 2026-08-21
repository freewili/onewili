/* OneWili event decoders - generated. Do not edit. */
#include "onewili_events.h"

static uint16_t ow__rd_u16le(const uint8_t* p) {
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
}
static uint32_t ow__rd_u32le(const uint8_t* p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
static uint64_t ow__rd_u64le(const uint8_t* p) {
    return (uint64_t)ow__rd_u32le(p) | ((uint64_t)ow__rd_u32le(p + 4) << 32);
}
/* Not every generated decoder uses every width. */
static void ow__rd_touch(void) {
    (void)ow__rd_u16le; (void)ow__rd_u32le; (void)ow__rd_u64le; (void)ow__rd_touch;
}

ow_status ow_decode_can_rx_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_evt_can_rx_report* out)
{
    if (!payload || !out || payload_len != 84u) return OW_ERR_PROTOCOL;
    out->time_stamp_ns = ow__rd_u64le(payload + 0);
    out->gpio_bitfield = ow__rd_u32le(payload + 8);
    out->r0_canid = ow__rd_u32le(payload + 12);
    out->r1_filter_header_bits = ow__rd_u32le(payload + 16);
    { int k; for (k = 0; k < 16; ++k)
          out->data_words[k] = ow__rd_u32le(payload + 20 + 4 * k); }
    out->error = error;
    return OW_OK;
}

static ow_status ow__ev_can_rx_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_event* out)
{
    out->kind = OW_EV_CAN_RX_REPORT;
    return ow_decode_can_rx_report(payload, payload_len, error, &out->u.can_rx_report);
}

ow_status ow_decode_gpio_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_evt_gpio_report* out)
{
    if (!payload || !out || payload_len != 12u) return OW_ERR_PROTOCOL;
    out->time_stamp_ns = ow__rd_u64le(payload + 0);
    out->gpio_bitfield = ow__rd_u32le(payload + 8);
    out->error = error;
    return OW_OK;
}

static ow_status ow__ev_gpio_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_event* out)
{
    out->kind = OW_EV_GPIO_REPORT;
    return ow_decode_gpio_report(payload, payload_len, error, &out->u.gpio_report);
}

ow_status ow_decode_logic_analyzer_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_evt_logic_analyzer_report* out)
{
    if (!payload || !out || payload_len != 44u) return OW_ERR_PROTOCOL;
    out->trigger_time_stamp_ns = ow__rd_u64le(payload + 0);
    out->sample_rate_ns = ow__rd_u32le(payload + 8);
    out->gpio_start_pin = payload[12];
    out->bits_per_sample = payload[13];
    out->trigger_type = payload[14];
    out->dummy = payload[15];
    out->trigger_location = ow__rd_u32le(payload + 16);
    out->buffer_head = ow__rd_u32le(payload + 20);
    out->analog_channel_mask = payload[24];
    out->analog_resolution = payload[25];
    out->analog_channel_count = payload[26];
    out->analog_dummy = payload[27];
    out->analog_sample_rate_ns = ow__rd_u32le(payload + 28);
    out->analog_sample_count = ow__rd_u32le(payload + 32);
    out->analog_buffer_head = ow__rd_u32le(payload + 36);
    out->analog_trigger_location = ow__rd_u32le(payload + 40);
    out->error = error;
    return OW_OK;
}

static ow_status ow__ev_logic_analyzer_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_event* out)
{
    out->kind = OW_EV_LOGIC_ANALYZER_REPORT;
    return ow_decode_logic_analyzer_report(payload, payload_len, error, &out->u.logic_analyzer_report);
}

const ow_event_decoder ow_event_decoders[] = {
    { 1, 84, "canRxReport", ow__ev_can_rx_report },
    { 0, 12, "gpioReport", ow__ev_gpio_report },
    { 2, 44, "logicAnalyzerReport", ow__ev_logic_analyzer_report },
};
const size_t ow_event_decoder_count =
    sizeof ow_event_decoders / sizeof ow_event_decoders[0];

int ow_poll_text_event(ow_device* dev, ow_event* out) {
    int r;
    if (!dev || !out) return -(int)OW_ERR_ARG;
    r = ow_poll_text_line(dev, out->u.text.id, sizeof out->u.text.id,
                          out->u.text.args, sizeof out->u.text.args);
    if (r == 1) out->kind = OW_EV_TEXT;
    return r;
}
