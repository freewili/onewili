/* OneWili LabVIEW ABI -- core session, discovery, events and file transfer.
 *
 * This header is the one you point LabVIEW's Import Shared Library Wizard at
 * (Tools >> Import >> Shared Library (.dll)). Everything in it is deliberately
 * restricted to what a Call Library Function Node can express:
 *
 *   - plain C base types only (int / unsigned int / unsigned char / double /
 *     char* / const char*). No stdint.h, no size_t, no bool, no enums, no
 *     structs, no function pointers, no opaque pointers.
 *   - sessions are `int` handles into a table inside the DLL, never raw
 *     pointers, so the 32-bit and 64-bit builds have an identical ABI.
 *   - every function returns an int status (OWLV_OK == 0); results come back
 *     through pointer-to-value or array-data-pointer arguments.
 *   - calling convention is cdecl ("C" in the Call Library Function Node).
 *
 * Strings out: pass a byte array (or string) large enough for `cap` bytes.
 * The DLL writes a NUL-terminated string and returns OWLV_ERR_BUFFER if the
 * value did not fit. 256 bytes is enough for every string this API returns
 * except command responses; use 4096 for those.
 *
 * Scalar and string outputs may be left unwired -- the DLL accepts a null
 * pointer or a zero capacity and skips them. Byte-array outputs (the
 * `unsigned char* x, int x_cap, int* x_len` triple) may NOT: the decoder in
 * the C package returns OWLV_ERR_ARG when the buffer or the length pointer
 * is missing, so always wire both.
 *
 * On any failure -- OWLV_ERR_SESSION included -- output arguments are left
 * exactly as the caller passed them. Never read an output without first
 * checking the status.
 *
 * The 540 device commands live in onewili_lv_api.h, included at the bottom.
 */
#ifndef ONEWILI_LV_H
#define ONEWILI_LV_H

/* ---- status codes ------------------------------------------------------ */
/* 0..6 mirror ow_status in the C package. 100+ are shim-only. */
#define OWLV_OK               0
#define OWLV_ERR_ARG          1
#define OWLV_ERR_IO           2
#define OWLV_ERR_TIMEOUT      3
#define OWLV_ERR_FAILED       4
#define OWLV_ERR_PROTOCOL     5
#define OWLV_ERR_BUFFER       6
#define OWLV_ERR_SESSION    100   /* bad or closed session handle */
#define OWLV_ERR_NO_DEVICE  101   /* auto-discovery found nothing */
#define OWLV_ERR_OPEN       102   /* the serial port would not open */
#define OWLV_ERR_LIMIT      103   /* too many open sessions */
#define OWLV_ERR_STATE      104   /* wrong order, e.g. poll before open */
#define OWLV_ERR_UNSUPPORTED 105  /* not available on this platform */

/* Suggested LabVIEW error code = OWLV_LV_ERROR_BASE + status. 5000..9999 is
 * the user-defined range, so these never collide with NI codes. */
#define OWLV_LV_ERROR_BASE 5300

/* ---- port kinds, as reported by owlv_port_kind ------------------------- */
#define OWLV_PORT_UNKNOWN       0
#define OWLV_PORT_FW_MAIN       1   /* main CPU text/command port */
#define OWLV_PORT_FW_DISPLAY    2   /* display CPU port */
#define OWLV_PORT_FW_BINARY     3   /* FTDI binary/WILI event port */
#define OWLV_PORT_FW_DEBUG      4   /* CMSIS-DAP debug probe CDC */
#define OWLV_PORT_FW_ESP32      5   /* ESP32 JTAG/serial */

/* ---- event kinds, as reported by owlv_binary_poll ---------------------- */
/* Values match ow_event_kind in the C package. Only the three binary kinds
 * have typed accessors; OWLV_EV_TEXT means "read it with owlv_last_text_event". */
#define OWLV_EV_NONE                    (-2)
#define OWLV_EV_TEXT                    (-1)
#define OWLV_EV_GPIO_REPORT               5
#define OWLV_EV_CAN_RX_REPORT            39
#define OWLV_EV_LOGIC_ANALYZER_REPORT    40

/* ======================================================================== */
/* Library information                                                      */
/* ======================================================================== */

/* Wrapper version. Independent of the OneWili command-set version. */
int owlv_version(int* major, int* minor, int* patch);

/* 4 for the 32-bit DLL, 8 for the 64-bit one. Call this first from LabVIEW:
 * if it fails to load or returns the wrong value you have a bitness mismatch,
 * which is the single most common Call Library Function Node failure. */
int owlv_pointer_size(void);

/* Human-readable text for any OWLV_* status code. Always succeeds. */
int owlv_status_message(int status, char* buf, int cap);

/* The last failure message recorded for a session, or for the library itself
 * when session is 0 (open/discovery failures land there). Empty if none. */
int owlv_last_error(int session, char* buf, int cap);

/* ======================================================================== */
/* Serial port discovery                                                    */
/* ======================================================================== */

/* Rescan the system's serial ports and return how many were found. The
 * results are cached until the next refresh; index them 0..count-1 with the
 * owlv_port_* calls below. Windows uses SetupAPI (friendly names and USB
 * VID/PID are available, so port kinds are identified); POSIX globs
 * /dev/ttyACM*, /dev/ttyUSB* and /dev/cu.usbmodem* and reports
 * OWLV_PORT_UNKNOWN for every one. */
int owlv_refresh_ports(int* count);

/* "COM7" on Windows, "/dev/ttyACM0" on Linux, "/dev/cu.usbmodem..." on macOS. */
int owlv_port_name(int index, char* buf, int cap);

/* OS friendly name, e.g. "USB Serial Device (COM7)". May be empty. */
int owlv_port_description(int index, char* buf, int cap);

/* One of the OWLV_PORT_* constants. */
int owlv_port_kind(int index, int* kind);

/* USB identifiers, 0 when unknown or not a USB port. */
int owlv_port_usb_ids(int index, int* vid, int* pid);

/* Convenience: refresh, then return the first port of the given kind.
 * OWLV_ERR_NO_DEVICE when there is none. */
int owlv_find_port(int kind, char* buf, int cap);

/* ======================================================================== */
/* Sessions                                                                 */
/* ======================================================================== */

/* Open the main command port by name and start a session. The returned
 * handle is >= 1; 0 is never a valid session. Sends the 0x02 reset-to-root
 * byte, exactly as ow_open does. Port is opened 8N1 at 1,000,000 baud. */
int owlv_open(const char* port, int* session);

/* Find the FreeWili main port and open it. port_used receives the port that
 * was chosen (pass cap 0 / a NULL array if you do not care). */
int owlv_open_auto(int* session, char* port_used, int cap);

/* Close one session, or every session this DLL has open. Closing an already
 * closed session is not an error. Call owlv_close_all from an "application
 * exit" case; LabVIEW does not unload the DLL between runs, so a session
 * leaked by an aborted VI stays open until then. */
int owlv_close(int session);
int owlv_close_all(void);

/* valid receives 1 when the handle names an open session, 0 otherwise. */
int owlv_session_valid(int session, int* valid);

/* The port name this session was opened on. */
int owlv_session_port(int session, char* buf, int cap);

/* ======================================================================== */
/* Raw command escape hatch                                                 */
/* ======================================================================== */

/* Send one already-formed wire line (e.g. "i\\g\\t 25") and return the
 * response frame's payload. This exists so a LabVIEW program can reach a
 * firmware command that is newer than this wrapper; prefer the generated
 * owlv_* commands, which encode arguments for you. Returns OWLV_ERR_FAILED
 * when the device answers with a not-ok frame. */
int owlv_send_raw(int session, const char* command, char* response, int cap);

/* ======================================================================== */
/* Text events                                                              */
/* ======================================================================== */

/* Non-blocking. got receives 1 when an event was dequeued, 0 when none was
 * pending -- neither is an error. Poll this in its own LabVIEW loop.
 *
 * A device event line is "[*<name> <hexTimestampNs> <seq> <payload> <ok>]" --
 * the same frame shape a command response uses. id receives the name
 * ("uart1", "power", ...) and args the payload alone, with the frame's own
 * fields handed back separately: the 64-bit timestamp as two U32 halves
 * (recombine as hi * 2^32 + lo) and the sequence number. */
int owlv_poll_text_event(int session, int* got,
                         char* id, int id_cap, char* args, int args_cap,
                         unsigned int* time_stamp_ns_lo,
                         unsigned int* time_stamp_ns_hi,
                         int* sequence);

/* Text events arriving while the queue is full are dropped and counted.
 * A non-zero, growing count means your poll loop is not keeping up. */
int owlv_dropped_text_events(int session, int* dropped);

/* ======================================================================== */
/* Binary (FTDI/WILI) event port                                            */
/* ======================================================================== */

/* Attach the binary event port to an existing session. This is a second,
 * separate serial port on the same board -- find it with
 * owlv_find_port(OWLV_PORT_FW_BINARY, ...) or let owlv_binary_open_auto do it. */
int owlv_binary_open(int session, const char* port);
int owlv_binary_open_auto(int session, char* port_used, int cap);
int owlv_binary_close(int session);

/* Non-blocking pump. kind receives OWLV_EV_NONE when no complete event was
 * available, otherwise one of the OWLV_EV_* values; the decoded event is
 * latched in the session, read it with the matching owlv_last_* call below.
 * Unknown and size-mismatched frames are skipped and counted. */
int owlv_binary_poll(int session, int* kind);

/* Counts of frames the parser could not use since the port was opened. */
int owlv_binary_stats(int session, int* unknown_frames, int* size_mismatches);

/* 64-bit device timestamps are split into two unsigned 32-bit halves because
 * that is the widest integer every LabVIEW version handles identically in a
 * Call Library Function Node. Recombine as hi * 2^32 + lo. */
int owlv_last_gpio_report(int session,
                          unsigned int* time_stamp_ns_lo,
                          unsigned int* time_stamp_ns_hi,
                          unsigned int* gpio_bitfield,
                          int* error);

/* data_words must be an array of at least 16 unsigned int (pass its length in
 * data_words_cap); it receives the MCP2518 payload words verbatim. */
int owlv_last_can_rx_report(int session,
                            unsigned int* time_stamp_ns_lo,
                            unsigned int* time_stamp_ns_hi,
                            unsigned int* gpio_bitfield,
                            unsigned int* canid,
                            unsigned int* filter_header_bits,
                            unsigned int* data_words, int data_words_cap,
                            int* error);

int owlv_last_logic_analyzer_report(int session,
                                    unsigned int* trigger_time_stamp_ns_lo,
                                    unsigned int* trigger_time_stamp_ns_hi,
                                    unsigned int* sample_rate_ns,
                                    int* gpio_start_pin,
                                    int* bits_per_sample,
                                    int* trigger_type,
                                    unsigned int* trigger_location,
                                    unsigned int* buffer_head,
                                    int* analog_channel_mask,
                                    int* analog_resolution,
                                    int* analog_channel_count,
                                    unsigned int* analog_sample_rate_ns,
                                    unsigned int* analog_sample_count,
                                    unsigned int* analog_buffer_head,
                                    unsigned int* analog_trigger_location,
                                    int* error);

/* The text event latched by the last owlv_binary_poll that returned
 * OWLV_EV_TEXT. */
int owlv_last_text_event(int session, char* id, int id_cap,
                         char* args, int args_cap,
                         unsigned int* time_stamp_ns_lo,
                         unsigned int* time_stamp_ns_hi,
                         int* sequence);

/* ======================================================================== */
/* File transfer                                                            */
/* ======================================================================== */

/* These block for the duration of the transfer. To watch progress from a
 * parallel LabVIEW loop, configure the transfer VI's Call Library Function
 * Node to "Run in any thread" and poll owlv_file_progress from the other
 * loop; the counters are written without taking the session lock. */
int owlv_file_put(int session, const char* host_path, const char* device_path);
int owlv_file_get(int session, const char* device_path, const char* host_path);

int owlv_file_put_mem(int session, const char* device_path,
                      const unsigned char* data, int len);
int owlv_file_get_mem(int session, const char* device_path,
                      unsigned char* buf, int cap, int* len);

/* done/total are byte counts for the transfer currently running on this
 * session; total is 0 when the size is not yet known. */
int owlv_file_progress(int session, unsigned int* done, unsigned int* total);

/* List a directory. dir_path may be "" or "/" for the current directory.
 * count receives the number of entries, which you then read with
 * owlv_file_entry(session, 0..count-1, ...). The listing is latched in the
 * session until the next owlv_file_list on it. */
int owlv_file_list(int session, const char* dir_path, int* count);
int owlv_file_entry(int session, int index, char* name, int name_cap,
                    int* is_dir, unsigned int* size);

/* Connect the SD card to the USB card reader (to_pc != 0) or to the main CPU
 * (to_pc == 0). */
int owlv_sd_host_select(int session, int to_pc);

/* ======================================================================== */
/* The generated device commands                                            */
/* ======================================================================== */

#include "onewili_lv_api.h"

#endif /* ONEWILI_LV_H */
