"""Behavioral tests of the regenerated OneWili binary API; no hardware required.

Run: python -m unittest discover -s tests -p test_binary_stream.py
"""
from pathlib import Path
import queue
import struct
import sys
import unittest
from unittest.mock import patch
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "python"))
from onewili import OneWili, _main_port, connect
from onewili.binary_framing import Parser, RawFrame, MAX_PAYLOAD
from onewili.binary_events import DECODERS, LogicAnalyzerReportEvent, CanRxReportEvent
from onewili.binary_transport import BinaryTransport


def frame(kind, payload, repeat=0, error=False):
    return struct.pack("<4sHHI", b"WILI", repeat, kind,
                       len(payload) | (0x80000000 if error else 0)) + payload


def capture(words=(0x12345678, 0x87654321), bits=4, head=1, analog=False):
    header = struct.pack("<QIBBBBIIBBBBIIII", 123456789, 1000, 8, bits, 2, 0,
                         0, head, 3 if analog else 0, 16 if analog else 0,
                         2 if analog else 0, 0, 500 if analog else 0,
                         1024 if analog else 0, 1022 if analog else 0, 0)
    return header + struct.pack(f"<{len(words)}I", *words) + (
        struct.pack("<1024H", *range(1024)) if analog else b"")


class BinaryStreamTests(unittest.TestCase):
    def test_discovery_handles_composite_main_and_legacy_serial(self):
        main = SimpleNamespace(kind="Other", name="FW2 v08", port="COM10")
        binary = SimpleNamespace(kind="FTDI", name="FW2", port="COM11")
        device = SimpleNamespace(usb_devices=[binary, main])
        self.assertEqual(_main_port(device), "COM10")
        legacy = SimpleNamespace(kind="Serial", name="Main", path="/dev/ttyACM0")
        self.assertEqual(_main_port(SimpleNamespace(usb_devices=[legacy])), "/dev/ttyACM0")
        with patch("onewili.find_devices", return_value=[device]), patch.object(OneWili, "open", lambda self: self):
            dev = connect()
            self.assertEqual(dev._binary_port, "COM11")

    def test_unknown_frames_preserve_every_byte_and_metadata(self):
        payload = bytes(range(256)) + b"\x00WILI\xff"
        stream = BinaryTransport("unused", DECODERS)
        wire = frame(65535, payload, 27, True)
        for byte in wire:
            stream.feed(bytes([byte]))
        self.assertEqual(stream.events.get_nowait(), RawFrame(65535, 27, payload, True))
        self.assertEqual(stream.unknown_frames, 1)

    def test_canfd_and_capture_interleaved_at_every_split(self):
        can = struct.pack("<QIII16I", 123, 1, 0x1FFFFFFF, 0x300000, *range(16))
        wire = frame(1, can, error=True) + frame(2, capture()) + frame(404, b"")
        for split in range(len(wire) + 1):
            stream = BinaryTransport("unused", DECODERS)
            stream.feed(wire[:split])
            stream.feed(wire[split:])
            event = stream.events.get_nowait()
            self.assertIsInstance(event, CanRxReportEvent)
            self.assertEqual(event.data_words, tuple(range(16)))
            self.assertTrue(event.error)
            event = stream.events.get_nowait()
            self.assertIsInstance(event, LogicAnalyzerReportEvent)
            self.assertEqual(list(event.digital_samples()), list(range(1, 9)) + list(range(8, 0, -1)))
            self.assertEqual(list(event.samples(8)), [x & 1 for x in event.digital_samples()])
            self.assertIsInstance(stream.events.get_nowait(), RawFrame)

    def test_maximum_digital_plus_analog_capture(self):
        payload = capture((0xAAAAAAAA,) * 262144, bits=1, head=0, analog=True)
        self.assertEqual(len(payload), MAX_PAYLOAD)
        stream = BinaryTransport("unused", DECODERS)
        wire = frame(2, payload)
        for pos in range(0, len(wire), 509):
            stream.feed(wire[pos:pos+509])
        event = stream.events.get_nowait()
        self.assertEqual(len(event.digital_data), 1048576)
        self.assertEqual(list(event.analog_samples())[:4], [1022, 1023, 0, 1])
        self.assertEqual(stream.size_mismatches, 0)

    def test_invalid_known_messages_fall_back_to_raw_and_continue(self):
        stream = BinaryTransport("unused", DECODERS)
        malformed = [b"short", capture() + b"x", capture(bits=0), capture(head=10), capture(analog=True)[:-2048]]
        for payload in malformed:
            stream.feed(frame(2, payload))
            self.assertEqual(stream.events.get_nowait().payload, payload)
        stream.feed(frame(0, struct.pack("<QI", 777, 42)))
        self.assertEqual(stream.events.get_nowait().gpio_bitfield, 42)
        self.assertEqual(stream.size_mismatches, len(malformed))

    def test_raw_mode_includes_known_frames(self):
        stream = BinaryTransport("unused", DECODERS, raw=True)
        payload = capture()
        stream.feed(frame(2, payload, 99, True))
        self.assertEqual(stream.events.get_nowait(), RawFrame(2, 99, payload, True))

    def test_bounded_queue_reports_drops(self):
        stream = BinaryTransport("unused", DECODERS, queue_size=2)
        stream.feed(b"".join(frame(123, bytes([n])) for n in range(5)))
        self.assertEqual(stream.dropped_events, 3)
        self.assertEqual([stream.events.get_nowait().payload for _ in range(2)], [b"\x03", b"\x04"])
        with self.assertRaises(ValueError):
            BinaryTransport("unused", DECODERS, queue_size=0)

    def test_garbage_oversize_header_and_embedded_marker(self):
        parser = Parser()
        wire = b"junkW" + struct.pack("<4sHHI", b"WILI", 0, 0, MAX_PAYLOAD+1)
        wire += frame(500, b"WILI\x00\xff") + frame(501, b"")
        out = parser.feed(wire)
        self.assertEqual([(f.header_type, f.payload) for f in out], [(500, b"WILI\x00\xff"), (501, b"")])

    def test_failed_open_is_retryable_and_text_connection_survives(self):
        dev = OneWili("unused", binary_port="unused")
        with patch("serial.Serial", side_effect=OSError("busy")):
            with self.assertRaises(OSError):
                dev.open_binary()
        self.assertIsNone(dev._binary)
        dev.close_binary()

    def test_reader_failure_is_reported_and_close_releases_port(self):
        class BrokenSerial:
            def __init__(self, *args, **kwargs): self.closed = False
            def read(self, n): raise OSError("unplugged")
            def close(self): self.closed = True
        stream = BinaryTransport("unused", DECODERS)
        with patch("serial.Serial", BrokenSerial):
            stream.open()
            stream._reader.join(1)
            self.assertIn("unplugged", str(stream.last_error))
            port = stream._serial
            stream.close()
            self.assertTrue(port.closed)
            stream.open()
            stream.close()


if __name__ == "__main__":
    unittest.main()
