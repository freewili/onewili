/* ow_stream_wire.h -- the peer-stream wire contract, shared verbatim by MAIN
 * and every OneWili client transport.
 *
 * Firmware-owned and dependency-free (C99, no includes beyond <stdint.h>):
 * MAIN's router includes it from here, and menutool embeds it into every
 * generated C package as include/ow_stream_wire.h, so the peer ids, the MTU
 * and the control-frame layout can never drift between the two ends.
 *
 * A peer stream is a best-effort datagram channel between OneWili clients
 * (DISPLAY, ESP32, CM0, the PC host), routed by MAIN. Every link carries the
 * same stream payload P:
 *
 *   P[0]  dst   ow_peer id 0..4 for a datagram, or OW_STREAM_CTL_* (0xF0..)
 *               for a hop-local control frame that MAIN never forwards
 *   P[1]  src   the sender's own id; MAIN OVERWRITES it with the id of the
 *               link the datagram arrived on before forwarding (no spoofing)
 *   P[2..]      datagram: 1..OW_STREAM_MTU bytes (zero-length is invalid:
 *               a poll result of 0 means "nothing waiting")
 *
 * Links: FwGUI command 0xF1 (MAIN->DISPLAY) and event 0xF1 (DISPLAY->MAIN);
 * Bottlenose id 103 both ways on the ESP32 link; text commands h\a\w, h\a\p
 * and h\a\c for clients with no push channel (PC host, CM0). One datagram is
 * always exactly one link frame -- never split, never merged.
 *
 * Push links (DISPLAY, ESP32) are flow-controlled by credits so a client can
 * never overrun MAIN's receive ring (which laps silently):
 *  - The client sends HELLO when it starts using streams, then at least
 *    every OW_STREAM_KEEPALIVE_MS while it keeps using them. MAIN treats the
 *    link as open while stream frames keep arriving and closes it after
 *    OW_STREAM_EXPIRE_MS of silence (or when the client CPU restarts).
 *    Datagrams for a closed link are dropped and counted.
 *  - MAIN answers every HELLO, and follows every consumed data frame, with a
 *    CREDIT carrying free-running totals since MAIN booted.
 *  - The client keeps (bytes sent - consumed_total) <= OW_STREAM_WINDOW,
 *    counting OW_STREAM_WIRE_BYTES(n) per datagram -- the payload plus the
 *    link framing around it, so the window bounds real bytes in MAIN's ring
 *    whatever the datagram size -- and refuses (drops and counts) a write
 *    that would exceed it. Until the first CREDIT arrives the
 *    link is unconfirmed and writes are refused.
 */
#ifndef OW_STREAM_WIRE_H
#define OW_STREAM_WIRE_H

#include <stdint.h>

/* Peer ids (P[0] / P[1]). */
#define OW_STREAM_PEER_MAIN     0u
#define OW_STREAM_PEER_DISPLAY  1u
#define OW_STREAM_PEER_ESP32    2u
#define OW_STREAM_PEER_CM0      3u
#define OW_STREAM_PEER_HOST     4u
#define OW_STREAM_PEER_COUNT    5u

/* Largest datagram, in data bytes, on every link. 128 is what the text path
 * can carry with generated hexbytes arguments: "h\a\w 4 " plus 3 chars per
 * byte must fit MAIN's 511-character command line with room to spare, so a
 * truncated line can never pass for a shorter valid datagram. */
#define OW_STREAM_MTU           128u

/* Stream payload header: dst + src. */
#define OW_STREAM_HDR           2u

/* Hop-local control frames (P[0] >= OW_STREAM_CTL_FIRST). */
#define OW_STREAM_CTL_FIRST     0xF0u
#define OW_STREAM_CTL_HELLO     0xF0u  /* client->MAIN, data = { version } */
#define OW_STREAM_CTL_CREDIT    0xF1u  /* MAIN->client, data = ow_stream_credit */
#define OW_STREAM_VERSION       1u

/* CREDIT data, little-endian u32s, all free-running since MAIN booted:
 *   [0..3]  consumed_total    OW_STREAM_WIRE_BYTES() of every data frame MAIN
 *                             has taken off this link (routed, dropped or gated)
 *   [4..7]  dropped_from      datagrams FROM this client that MAIN dropped
 *   [8..11] dropped_to        datagrams addressed TO this client that MAIN
 *                             dropped (link closed, full, or not enabled)   */
#define OW_STREAM_CREDIT_LEN    12u

/* Link framing around one stream payload: the larger of the FwGUI event frame
 * (sync 2 + len 2 + event 1 + cksum 2 = 7) and the Bottlenose frame
 * (sync 2 + len 2 + id 2 + cksum 2 = 8). */
#define OW_STREAM_LINK_OVERHEAD 8u

/* Flow-control accounting unit for one datagram of n data bytes: what it
 * occupies in MAIN's receive ring. Counting the payload alone let a burst of
 * tiny datagrams (256 x 1 byte = 768 units, 2560 wire bytes) overrun the ring. */
#define OW_STREAM_WIRE_BYTES(n) ((uint32_t)(n) + OW_STREAM_HDR + OW_STREAM_LINK_OVERHEAD)

/* Credit window in OW_STREAM_WIRE_BYTES units, i.e. ring bytes in flight:
 * about 5 max-size or 69 one-byte datagrams, leaving the rest of MAIN's 2 KB
 * (display) / 4 KB (ESP32, shared with SDFS) receive ring for commands and SD
 * traffic. */
#define OW_STREAM_WINDOW        768u

/* Timings (ms). */
#define OW_STREAM_HELLO_RETRY_MS  250u   /* while unconfirmed */
#define OW_STREAM_KEEPALIVE_MS    1000u  /* HELLO at least this often while in use */
#define OW_STREAM_EXPIRE_MS       3000u  /* MAIN closes a silent link */

static inline void ow_stream_put_u32(uint8_t* p, uint32_t v)
{
    p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24);
}

static inline uint32_t ow_stream_get_u32(const uint8_t* p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

#endif /* OW_STREAM_WIRE_H */
