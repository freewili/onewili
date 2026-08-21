"""Off-hardware tests for the Type-B detection driver (mocked transport)."""
from result import Ok, Err

from typeb_detect import parse_atqb, TypeBDriver


def test_parse_atqb_extracts_pupi():
    atqb = bytes([0x50, 0x12, 0x34, 0x56, 0x78] + [0] * 7)
    assert parse_atqb(atqb) == bytes([0x12, 0x34, 0x56, 0x78])


def test_parse_atqb_rejects_bad_prefix():
    assert parse_atqb(bytes([0x00, 0x12, 0x34, 0x56, 0x78])) is None
    assert parse_atqb(bytes([0x50, 0x12])) is None
    assert parse_atqb(None) is None


class _MockRaw:
    """Models the register file so read-modify-write configuration is exercised."""

    def __init__(self, transceive_result):
        self._tr = transceive_result
        self.writes = []            # ordered (addr, value) log for assertions
        # Post-init register state: OP_CONTROL has the oscillator/regulator EN bit
        # (0x80) set by init(); framing registers at their reset default of 0x00.
        self.regs = {0x02: 0x80, 0x06: 0x00, 0x07: 0x00}

    def begin(self): return Ok(None)
    def end(self): return Ok(None)
    def field(self, on): return Ok(None)

    def reg_read(self, addr):
        return Ok(self.regs.get(addr, 0x00))

    def reg_write(self, addr, value):
        self.regs[addr] = value
        self.writes.append((addr, value))
        return Ok(None)

    def transceive(self, flags, timeout_ms, tx):
        return self._tr


def test_detect_returns_pupi_with_mock_transport():
    atqb = bytes([0x50, 0x12, 0x34, 0x56, 0x78] + [0] * 7)
    raw = _MockRaw(Ok((0x01, atqb)))   # status ok + ATQB
    assert TypeBDriver(raw).detect() == bytes([0x12, 0x34, 0x56, 0x78])


def test_detect_returns_none_on_card_timeout():
    raw = _MockRaw(Ok((0x02, bytes())))   # bit0 clear = no RX / timeout
    assert TypeBDriver(raw).detect() is None


def test_detect_returns_none_on_link_error():
    raw = _MockRaw(Err("link timeout"))
    assert TypeBDriver(raw).detect() is None


def test_configure_writes_st_reference_type_b_registers():
    """The register writes must match ST's RFAL RFAL_MODE_POLL_NFCB sequence."""
    atqb = bytes([0x50, 0x12, 0x34, 0x56, 0x78] + [0] * 7)
    raw = _MockRaw(Ok((0x01, atqb)))
    TypeBDriver(raw).detect()
    final = dict(raw.writes)
    assert final[0x03] == 0x14                       # MODE = ISO14443B + AM modulation
    assert final[0x04] == 0x00                       # BIT_RATE = 106 kbps
    assert final[0x06] == 0x00                       # ISO14443B_1 framing (bits 7:2 cleared)
    assert final[0x07] == 0x00                       # ISO14443B_2 framing (bits 7:4 cleared)
    # OP_CONTROL: EN (0x80) preserved, plus TX (0x08) | RX (0x40) | en_fd_c=01 (0x01).
    assert final[0x02] == 0x80 | 0x08 | 0x40 | 0x01
    # RX demodulator (NFC-B 106) + TX AM shaping.
    assert final[0x0B] == 0x04                       # RX_CONF1
    assert final[0x0C] == 0xFD                       # RX_CONF2
    assert final[0x8C] == 0x97                       # CORR_CONF1 (Space B)
    assert final[0x28] == 0x40                       # TX_DRIVER am_mod (from default 0x00)
