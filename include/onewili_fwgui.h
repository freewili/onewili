/* FwGUI display-link transport for the OneWili C API (WiliBSP / RP2350B).
 * Talks to the FreeWili 2 main CPU over UART0 (Rx=GPIO0, Tx=GPIO1,
 * RTS=GPIO2, CTS=GPIO3) at 8,000,000 baud with hardware flow control.
 * Single link per board: the state behind these transports is static. */
#ifndef ONEWILI_FWGUI_H
#define ONEWILI_FWGUI_H
#include "onewili.h"

/* Inits UART0 + pins, installs the command transport, and calls ow_open
 * (which performs the 0x02 reset handshake). Call once at startup.
 * Receive is interrupt driven (UART0_IRQ, exclusive handler): bytes land in
 * a 32 KB ring whatever the app is doing, so MAIN is never held on CTS by a
 * slow display loop. */
ow_status ow_open_fwgui(ow_device* dev);

/* The binary-event stream (FWGUI_API_ONEWILL_BINARY frames) as a transport
 * for ow_binary_open/ow_binary_poll. Valid after ow_open_fwgui. */
ow_transport ow_fwgui_binary_transport(void);

/* Whole frames discarded on receive, from all three sources combined:
 *   - a text (0x5D, 8 KB) or binary (0x5E, 32 KB) stream buffer was full --
 *     the app is not draining ow_poll_* / ow_binary_poll often enough;
 *   - the SDFS (0x5F) response ring was full -- the server sent more frames
 *     than an in-flight SD call consumed, so polling more often does NOT help
 *     (the app was already blocked inside that call);
 *   - an SDFS frame was longer than SDFS_MAX_FRAME.
 * Free-running counter; never reset except by ow_open_fwgui. */
uint32_t ow_fwgui_dropped_frames(void);

/* The SDFS request/response stream as an sdfslib transport. ow_open_fwgui
 * already binds this to the ow_sd_* API (onewili_sd.h); use it directly only
 * to drive sdfslib yourself. Valid after ow_open_fwgui. */
#include "sdfs_transport.h"
sdfs_transport_t ow_fwgui_sdfs_transport(void);

/* LOCAL ADDITION — not in the generated package; re-apply after every re-copy.
 * Sends FWGUI_EVENT_POWER_ZONES (event 48) with the live rail mask so MAIN
 * can (re)init anything gated on a zone, e.g. its CAN controller. Idempotent
 * fire-and-forget (no response is read) — call after open, on mask change,
 * and periodically, since MAIN can reboot independently of DISPLAY. */
void ow_fwgui_send_power_zones(uint32_t zone_mask);

/* LOCAL ADDITION — not in the generated package; re-apply after every re-copy.
 * Link health counters, all free-running since ow_open_fwgui. */
typedef struct ow_fwgui_stats {
    uint32_t ring_overrun_bytes;   /* IRQ found the 32 KB ring full: the app stopped pumping for >40 ms at full rate */
    uint32_t ring_max_fill;        /* high-water mark of the ring (bytes) */
    uint32_t hw_overruns;          /* UART RX FIFO overruns: hardware flow control did not hold MAIN */
    uint32_t frames_text;          /* 0x5D frames accepted */
    uint32_t frames_binary;        /* 0x5E frames accepted */
    uint32_t frames_sdfs;          /* 0x5F frames accepted */
    uint32_t frames_other;         /* GUI traffic discarded */
    uint32_t checksum_errors;      /* frames failing the additive checksum */
    uint32_t dropped_frames;       /* see ow_fwgui_dropped_frames */
    uint32_t text_max_fill;        /* high-water marks of the stream FIFOs (bytes) */
    uint32_t binary_max_fill;
    uint32_t tx_bytes;             /* bytes written to the link (framed) */
} ow_fwgui_stats;
void ow_fwgui_get_stats(ow_fwgui_stats* out);

/* LOCAL ADDITION. Move ring bytes through the frame parser into the stream
 * FIFOs now. Every read already does this; an app that is busy elsewhere for
 * a while can call it to keep the ring shallow. */
void ow_fwgui_pump(void);
uint32_t ow_fwgui_ring_fill(void);

#endif /* ONEWILI_FWGUI_H */
