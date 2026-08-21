"""Type-B (ISO 14443-B) detection driver over the OneWili nfc.raw primitives.

Reads a Type-B card's PUPI using only the generated nfc.raw.* API — no firmware
protocol code. Demonstrates writing a protocol driver in host code.

The ST25R3916 register setup below is derived from ST's own RFAL reference
(rfal_rfst25r3916.c RFAL_MODE_POLL_NFCB + st25r3916_com.h) and mirrors the
structure of the on-device st25r3916.cpp::configureForNFCA, which the FreeWili's
working NFC-A reader uses with the chip's default analog RX settings (it writes no
analog-config registers). Type-B detection uses the same approach: set the digital
mode/bit-rate/framing and enable TX/RX, and rely on the default analog front-end.
If a card is marginal on hardware, the analog RX config (ST's per-antenna
rfalSetAnalogConfig table) is the tuning layer to add — but it is board-specific,
so it is deliberately not hard-coded here.
"""
from __future__ import annotations

REQB = bytes([0x05, 0x00, 0x00])   # APf=0x05, AFI=0x00, PARAM=0x00 (REQB / SENSB_REQ, N=1 slot)
CRC = 0x03                          # transceive flags: append CRC on TX, check CRC on RX

# ST25R3916 registers. Space A registers use their bare address; the two correlator
# registers and AWS_CONF2 live in Space B (raw wire byte = address | 0x80).
REG_OP_CONTROL = 0x02
REG_MODE = 0x03
REG_BIT_RATE = 0x04
REG_ISO14443B_1 = 0x06
REG_ISO14443B_2 = 0x07
REG_AUX = 0x0A
REG_RX_CONF1 = 0x0B
REG_RX_CONF2 = 0x0C
REG_RX_CONF3 = 0x0D
REG_RX_CONF4 = 0x0E
REG_TX_DRIVER = 0x28
REG_CORR_CONF1 = 0x8C   # Space B 0x0C
REG_CORR_CONF2 = 0x8D   # Space B 0x0D
REG_AWS_CONF2 = 0xAF    # Space B 0x2F

# MODE: om = ISO14443B (0b0010 << 3 = 0x10) OR tr_am = AM 10% ASK (bit 2 = 0x04) -> 0x14.
# Type B uses AM modulation on TX, unlike Type A's OOK; leaving bit 2 clear (0x10) fails.
MODE_ISO14443B_INITIATOR = 0x14
BIT_RATE_106 = 0x00                 # fc/128 TX + RX = 106 kbps (Type-B base rate)

# AUX.dis_corr (bit 2) = 0 selects the correlator receiver (needed for the B subcarrier).
AUX_DIS_CORR = 0x04
# OP_CONTROL enable bits: TX, RX, and the external field detector (en_fd_c = 01).
OP_CONTROL_TX_EN = 0x08
OP_CONTROL_RX_EN = 0x40
OP_CONTROL_EN_FD_C1 = 0x02
OP_CONTROL_EN_FD_C0 = 0x01
# TX_DRIVER AM modulation index (am_mod field, bits 7:4).
TX_DRIVER_AM_MOD_MASK = 0xF0
TX_DRIVER_AM_MOD = 0x40
# AWS_CONF2 (Space B) AM wave shaping: am_sym set, en_modsink clear, am_filt = 0x08.
AWS_CONF2_MASK = 0x3F
AWS_CONF2_VALUE = 0x28


def parse_atqb(atqb: bytes | None) -> bytes | None:
    """Extract the 4-byte PUPI from an ATQB response, or None if not a valid ATQB.

    ATQB layout: [0x50][PUPI:4][Application Data:4][Protocol Info:3].
    """
    if atqb is None or len(atqb) < 5 or atqb[0] != 0x50:
        return None
    return atqb[1:5]


class TypeBDriver:
    """Detects an ISO 14443-B card and returns its PUPI, over nfc.raw primitives."""

    def __init__(self, raw):
        self.raw = raw

    def _rmw(self, addr: int, mask: int, value: int) -> None:
        """Read-modify-write one register (RFAL ChangeRegisterBits equivalent)."""
        current = self.raw.reg_read(addr).unwrap()
        self.raw.reg_write(addr, (current & ~mask) | (value & mask))

    def _configure_type_b(self) -> None:
        """Configure the ST25R3916 for ISO 14443-B 106 kbps initiator operation.

        Values are ST's RFAL reference for NFC-B (rfal_rfst25r3916.c mode setup plus
        the ST25R3916B NFC-B analog-config table): mode/framing, the correlator RX
        demodulator for the B subcarrier, and AM TX shaping. Assumes the chip is
        already initialized and calibrated -- enable the built-in reader once before
        the first raw session so init()/regulator calibration has run.
        """
        r = self.raw
        r.reg_write(REG_MODE, MODE_ISO14443B_INITIATOR)   # ISO14443B + AM modulation
        r.reg_write(REG_BIT_RATE, BIT_RATE_106)
        self._rmw(REG_ISO14443B_1, 0xFC, 0x00)            # EGT/SOF/EOF framing
        self._rmw(REG_ISO14443B_2, 0xF0, 0x00)            # TR1 = 80/80 fs
        # Enable TX/RX + field detector; en_fd_c = 01 (bit 0 set, bit 1 clear).
        op = r.reg_read(REG_OP_CONTROL).unwrap()
        op |= OP_CONTROL_TX_EN | OP_CONTROL_RX_EN | OP_CONTROL_EN_FD_C0
        op &= ~OP_CONTROL_EN_FD_C1
        r.reg_write(REG_OP_CONTROL, op)
        # RX demodulator: correlator receiver + NFC-B 106 receive config.
        self._rmw(REG_AUX, AUX_DIS_CORR, 0x00)
        r.reg_write(REG_RX_CONF1, 0x04)
        r.reg_write(REG_RX_CONF2, 0xFD)
        r.reg_write(REG_RX_CONF3, 0x00)
        r.reg_write(REG_RX_CONF4, 0x00)
        r.reg_write(REG_CORR_CONF1, 0x97)                # Space B
        r.reg_write(REG_CORR_CONF2, 0x00)                # Space B
        # TX AM modulation shaping.
        self._rmw(REG_TX_DRIVER, TX_DRIVER_AM_MOD_MASK, TX_DRIVER_AM_MOD)
        self._rmw(REG_AWS_CONF2, AWS_CONF2_MASK, AWS_CONF2_VALUE)   # Space B

    def detect(self) -> bytes | None:
        """Return the PUPI of a present Type-B card, or None if none answered."""
        self.raw.begin()
        try:
            self._configure_type_b()
            self.raw.field(1)
            res = self.raw.transceive(CRC, 20, REQB)   # (flags, timeout_ms, tx)
            if res.is_err():
                return None
            status, atqb = res.unwrap()
            if not (status & 0x01):   # bit0 = RX OK; anything else = timeout/error/no card
                return None
            return parse_atqb(atqb)
        finally:
            self.raw.end()


if __name__ == "__main__":
    import onewili

    dev = onewili.connect()
    try:
        pupi = TypeBDriver(dev.wireless.nfc.raw).detect()
        print("PUPI:", pupi.hex().upper() if pupi else "no Type-B card")
    finally:
        dev.close()
