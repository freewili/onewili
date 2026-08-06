/* OneWili event types - generated from the firmware sources. Do not edit.
 * Binary events: decoded WILI frames (see onewili_binary.h / ow_binary_poll).
 * Text events: "[*<id> <args>]" lines from the main port (ow_poll_text_event). */
#ifndef ONEWILI_EVENTS_H
#define ONEWILI_EVENTS_H
#include "onewili.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef OW_EVENT_ID_MAX
#define OW_EVENT_ID_MAX 64
#endif

/* A text event line "[*<id> <args>]" split into id + raw args. */
typedef struct ow_text_event {
    char id[OW_EVENT_ID_MAX];
    char args[OW_RESP_MAX];
} ow_text_event;

/* canRxReport - CAN RX frame report (binary API, MCP2518 memory-map layout).  apiFrame_canRxReport, 84 bytes. */
typedef struct ow_evt_can_rx_report {
    uint64_t time_stamp_ns;   /* ui64TimeStampNs @ 0 */
    uint32_t gpio_bitfield;   /* uiGpioBitfield @ 8 */
    uint32_t r0_canid;   /* uiR0_CANID @ 12 */
    uint32_t r1_filter_header_bits;   /* uiR1_Filter_HeaderBits @ 16 */
    uint32_t data_words[16];   /* uiDataWords @ 20 */
    bool error;   /* frame header error bit */
} ow_evt_can_rx_report;

/* gpioReport - Periodic GPIO bitfield report (binary API).  apiFrame_gpioReport, 12 bytes. */
typedef struct ow_evt_gpio_report {
    uint64_t time_stamp_ns;   /* ui64TimeStampNs @ 0 */
    uint32_t gpio_bitfield;   /* uiGpioBitfield @ 8 */
    bool error;   /* frame header error bit */
} ow_evt_gpio_report;

/* logicAnalyzerReport - Logic analyzer capture report (binary API; header then digital + analog samples).  apiFrame_LogicAnalyzerReport, 44 bytes. */
typedef struct ow_evt_logic_analyzer_report {
    uint64_t trigger_time_stamp_ns;   /* ui64TriggerTimeStampNs @ 0 */
    uint32_t sample_rate_ns;   /* uiSampleRateNs @ 8 */
    uint8_t gpio_start_pin;   /* uiGPIOStartPin @ 12 */
    uint8_t bits_per_sample;   /* uiBitsPerSample @ 13 */
    uint8_t trigger_type;   /* uiTriggerType @ 14 */
    uint8_t dummy;   /* uiDummy @ 15 */
    uint32_t trigger_location;   /* uiTriggerLocation @ 16 */
    uint32_t buffer_head;   /* uiBufferHead @ 20 */
    uint8_t analog_channel_mask;   /* uiAnalogChannelMask @ 24 */
    uint8_t analog_resolution;   /* uiAnalogResolution @ 25 */
    uint8_t analog_channel_count;   /* uiAnalogChannelCount @ 26 */
    uint8_t analog_dummy;   /* uiAnalogDummy @ 27 */
    uint32_t analog_sample_rate_ns;   /* uiAnalogSampleRateNs @ 28 */
    uint32_t analog_sample_count;   /* uiAnalogSampleCount @ 32 */
    uint32_t analog_buffer_head;   /* uiAnalogBufferHead @ 36 */
    uint32_t analog_trigger_location;   /* uiAnalogTriggerLocation @ 40 */
    bool error;   /* frame header error bit */
} ow_evt_logic_analyzer_report;

typedef enum ow_event_kind {
    OW_EV_NONE = -2,
    OW_EV_TEXT = -1,
    OW_EV_AIN_IN = 0,   /* ainIn (stream, text) */
    OW_EV_ADC_IN = 1,   /* adcIn (stream, text) */
    OW_EV_FDIR = 2,   /* fdir (protocol, text) */
    OW_EV_FILEDL = 3,   /* filedl (protocol, text) */
    OW_EV_FPGADL = 4,   /* fpgadl (protocol, text) */
    OW_EV_GPIO_REPORT = 5,   /* gpioReport (stream, binary) */
    OW_EV_FILEPICKED = 6,   /* filepicked (protocol, text) */
    OW_EV_POWER = 7,   /* power (stream, text) */
    OW_EV_MOTION = 8,   /* motion (stream, text) */
    OW_EV_FIELD = 9,   /* field (stream, text) */
    OW_EV_ENV = 10,   /* env (stream, text) */
    OW_EV_ORIENTATION = 11,   /* orientation (stream, text) */
    OW_EV_UART1 = 12,   /* uart1 (stream, text) */
    OW_EV_RECORD = 13,   /* record (stream, text) */
    OW_EV_AUDIO = 14,   /* audio (stream, text) */
    OW_EV_BTSCAN = 15,   /* btscan (stream, text) */
    OW_EV_BUTTON = 16,   /* button (stream, text) */
    OW_EV_I2CMON = 17,   /* i2cmon (stream, text) */
    OW_EV_IRRX = 18,   /* irrx (stream, text) */
    OW_EV_LORA = 19,   /* lora (stream, text) */
    OW_EV_NFC = 20,   /* nfc (stream, text) */
    OW_EV_RADIO1 = 21,   /* radio1 (stream, text) */
    OW_EV_RADIO2 = 22,   /* radio2 (stream, text) */
    OW_EV_RADIOASYNC = 23,   /* radioasync (protocol, text) */
    OW_EV_BATTERY = 24,   /* battery (stream, text) */
    OW_EV_WIFISTA_INFO = 25,   /* wifistaInfo (stream, text) */
    OW_EV_WIFIAP_INFO = 26,   /* wifiapInfo (stream, text) */
    OW_EV_WIFISCAN = 27,   /* wifiscan (stream, text) */
    OW_EV_WIFIAPDEVCON = 28,   /* wifiapdevcon (stream, text) */
    OW_EV_WIFIAPDEVDC = 29,   /* wifiapdevdc (stream, text) */
    OW_EV_WSCLIENTCON = 30,   /* wsclientcon (protocol, text) */
    OW_EV_WSCLIENTDC = 31,   /* wsclientdc (protocol, text) */
    OW_EV_WIFISTATIONS = 32,   /* wifistations (stream, text) */
    OW_EV_TIME_SYNC = 33,   /* TimeSync (stream, text) */
    OW_EV_ZOOMIO = 34,   /* zoomio (stream, text) */
    OW_EV_CAN0 = 35,   /* can0 (stream, text) */
    OW_EV_CAN1 = 36,   /* can1 (stream, text) */
    OW_EV_CAN_TX0 = 37,   /* canTx0 (stream, text) */
    OW_EV_CAN_TX1 = 38,   /* canTx1 (stream, text) */
    OW_EV_CAN_RX_REPORT = 39,   /* canRxReport (stream, binary) */
    OW_EV_LOGIC_ANALYZER_REPORT = 40,   /* logicAnalyzerReport (stream, binary) */
    OW_EV_SCRIPT = 41,   /* script (protocol, text) */
    OW_EV_WIL_EYE = 42,   /* WILEye (protocol, text) */
    OW_EV_WIL_EYE_IMG_START = 43,   /* WILEyeImgStart (protocol, text) */
    OW_EV_WIL_EYE_IMG_CHUNK = 44,   /* WILEyeImgChunk (protocol, text) */
    OW_EV_WIL_EYE_IMG_END = 45,   /* WILEyeImgEnd (protocol, text) */
    OW_EV_WIL_EYE_IMG_ABORT = 46,   /* WILEyeImgAbort (protocol, text) */
    OW_EV_WIL_EYE_S_DCARD = 47,   /* WILEyeSDcard (protocol, text) */
    OW_EV_WIL_EYE_AI = 48,   /* WILEyeAI (protocol, text) */
    OW_EV_WIL_EYE_UNKNOWN = 49,   /* WILEyeUnknown (protocol, text) */
    OW_EV_FUZZ = 50,   /* fuzz (diagnostic, text) */
    OW_EV_PUSH_SUB_MENU = 51,   /* pushSubMenu (diagnostic, text) */
    OW_EV_APP_SIGNAL = 52,   /* appSignal (stream, text) */
    OW_EV_LOGGER = 53,   /* logger (protocol, text) */
} ow_event_kind;

typedef struct ow_event {
    ow_event_kind kind;
    union {
        ow_text_event text;
        ow_evt_can_rx_report can_rx_report;
        ow_evt_gpio_report gpio_report;
        ow_evt_logic_analyzer_report logic_analyzer_report;
    } u;
} ow_event;

/* Decode one canRxReport payload. OW_ERR_PROTOCOL on bad length. */
ow_status ow_decode_can_rx_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_evt_can_rx_report* out);
/* Decode one gpioReport payload. OW_ERR_PROTOCOL on bad length. */
ow_status ow_decode_gpio_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_evt_gpio_report* out);
/* Decode one logicAnalyzerReport payload. OW_ERR_PROTOCOL on bad length. */
ow_status ow_decode_logic_analyzer_report(const uint8_t* payload, uint32_t payload_len, bool error, ow_evt_logic_analyzer_report* out);

/* header_type -> decoder table, used by ow_binary_poll. */
typedef struct ow_event_decoder {
    uint16_t header_type;
    uint32_t payload_size;
    const char* name;
    ow_status (*decode)(const uint8_t* payload, uint32_t payload_len,
                        bool error, ow_event* out);
} ow_event_decoder;
extern const ow_event_decoder ow_event_decoders[];
extern const size_t ow_event_decoder_count;

/* Poll for a text event; fills out->u.text and sets kind = OW_EV_TEXT.
 * Returns 1 = filled, 0 = none pending, negative = -(ow_status). */
int ow_poll_text_event(ow_device* dev, ow_event* out);

#ifdef __cplusplus
}
#endif
#endif /* ONEWILI_EVENTS_H */
