/* LOCAL ADDITION — not in the generated package; re-apply after every re-copy.
 *
 * Pipelined one-shot CAN FD transmit over OneWili.
 *
 * Every generated call is synchronous: write the command, wait for its
 * response frame. Over the FwGUI link that costs a full MAIN main-loop pass
 * per frame (~1 ms), so one-shot transmit tops out near 1000 frames/s no
 * matter how fast the link is. MAIN's display bridge executes commands in
 * arrival order and answers each one, so a caller may keep several commands
 * unanswered and match responses to commands by order. This helper does that
 * for ``i\c\w`` (write_canfd) with two limits: commands in flight and wire
 * bytes in flight (MAIN drains its 2 KB display RX ring once per loop pass;
 * exceeding it silently loses bytes). */
#ifndef ONEWILI_FAST_H
#define ONEWILI_FAST_H
#include "onewili.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Raw hooks (LOCAL ADDITION at the end of onewili.c). */
int       ow_raw_send(ow_device* dev, const char* cmd);
ow_status ow_raw_next_response(ow_device* dev, char* resp, size_t cap, int* ok, uint32_t timeout_ms);
/* ow_poll_text_line keeps response frames it completes for the pipeline;
 * these manage that stash (cleared by ow_fast_tx_reset). */
void      ow_raw_stash_clear(void);
unsigned  ow_raw_stash_lost_count(void);

#define OW_FAST_MAX_IN_FLIGHT 32u
#define OW_FAST_DEFAULT_MAX_BYTES 1400u

typedef void (*ow_fast_result_fn)(void* user, uint32_t token, int ok);

typedef struct ow_fast_tx {
    ow_device* dev;
    uint32_t max_in_flight;    /* commands allowed unanswered; 1 = synchronous */
    uint32_t max_bytes;        /* framed wire bytes allowed unanswered */
    uint32_t in_flight, bytes_in_flight;
    uint32_t sent, ok, failed, proto_errors, io_errors;
    uint32_t q_bytes[OW_FAST_MAX_IN_FLIGHT];
    uint32_t q_token[OW_FAST_MAX_IN_FLIGHT];
    uint32_t q_r, q_w;
    ow_fast_result_fn on_result;
    void* user;
} ow_fast_tx;

void ow_fast_tx_init(ow_fast_tx* t, ow_device* dev, uint32_t max_in_flight,
                     ow_fast_result_fn on_result, void* user);

/* Queue one write_canfd. Returns 1 when sent, 0 when the pipeline is full
 * (reap, then retry), or -(ow_status) on error. ``token`` is handed back to
 * on_result with the command's outcome (ok = the firmware accepted the
 * frame; 0 = it refused it, typically a full transmit FIFO). */
int ow_fast_canfd_write(ow_fast_tx* t, int32_t channel, uint32_t arb_id, int can_fd,
                        int xtd_id, const uint8_t* data, size_t len, uint32_t token);

/* Collect responses: with timeout_ms == 0 only what has already arrived,
 * otherwise wait up to timeout_ms for the first one and then take the rest.
 * Returns the number of responses consumed, or -(ow_status) on an IO error. */
int ow_fast_tx_reap(ow_fast_tx* t, uint32_t timeout_ms);

/* Forget every unanswered command (after a stall: MAIN lost them, their
 * responses will never come). Counters are kept. */
void ow_fast_tx_reset(ow_fast_tx* t);

/* Wait until nothing is in flight. OW_OK or OW_ERR_TIMEOUT. */
ow_status ow_fast_tx_drain(ow_fast_tx* t, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif
#endif /* ONEWILI_FAST_H */
