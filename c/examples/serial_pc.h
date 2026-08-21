/* Example PC serial transport for the OneWili C API (Win32 + POSIX). */
#ifndef ONEWILI_SERIAL_PC_H
#define ONEWILI_SERIAL_PC_H
#include "onewili.h"

typedef struct serial_pc serial_pc;

/* port_name: "COM5" on Windows, "/dev/ttyACM0" on Linux, "/dev/cu.usbmodem..."
 * on macOS. Opens at 1,000,000 baud 8N1. Returns NULL on failure. */
serial_pc* serial_pc_open(const char* port_name);
void serial_pc_close(serial_pc* sp);

/* The ow_transport callbacks backed by this port. */
ow_transport serial_pc_transport(serial_pc* sp);

#endif
