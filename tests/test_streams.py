r"""Peer streams (onewili.streams) against a fake MAIN; no hardware required.

The fake speaks MAIN's text stream commands (h\a\w, h\a\p, h\a\c) through
the generated menus, so these tests cover the Python runtime and its
bindings together.

Run: python -m unittest discover -s tests -p test_streams.py
"""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "python"))
from onewili import OneWili, framing
from onewili.streams import Streams


class FakeMain:
    """A Transport stand-in that answers the stream commands like MAIN."""

    def __init__(self, poll_bytes=None):
        if poll_bytes is not None:
            self.stream_poll_bytes = poll_bytes
        self.sent = []
        self.queue = []            # (src, data) waiting for this client
        self.dropped_to = 0
        self.dropped_from = 0
        self.corrupt = None        # raw record bytes appended to the next poll
        self.fail = set()          # paths that answer with ok=0
        self._reply = None

    def flush_queues(self):
        pass

    def send(self, cmd):
        self.sent.append(cmd)
        path, _, rest = cmd.partition(" ")
        args = rest.split()
        if path in self.fail:
            self._reply = f"[{path} 0 0 refused 0]"
        elif path == "h\\a\\w":
            dst, data = int(args[0]), bytes(int(t, 16) for t in args[1:])
            if dst == Streams.HOST:
                self.queue.append((Streams.HOST, data))
                self._reply = f"[{path} 0 0 1 1]"
            else:
                self.dropped_from += 1
                self._reply = f"[{path} 0 0 0 1]"
        elif path == "h\\a\\p":
            room, packed, frames = int(args[0]), b"", 0
            while self.queue and len(packed) + 2 + len(self.queue[0][1]) <= room:
                src, data = self.queue.pop(0)
                packed += bytes([src, len(data)]) + data
                frames += 1
            if self.corrupt is not None:
                packed += self.corrupt
                frames = max(frames, 1)
                self.corrupt = None
            body = " ".join([str(frames), str(len(self.queue)), str(self.dropped_to)]
                            + [f"{b:02X}" for b in packed])
            self._reply = f"[{path} 0 0 {body} 1]"
        elif path == "h\\a\\c":
            self._reply = (f"[{path} 0 0 {Streams.MTU} {len(self.queue)} "
                           f"{self.dropped_to} {self.dropped_from} 1]")
        else:
            self._reply = f"[{path} 0 0 unknown 0]"

    def wait_frame(self, timeout):
        reply, self._reply = self._reply, None
        return framing.ResponseFrame.parse(reply) if reply else None

    def polls(self):
        return sum(1 for c in self.sent if c.startswith("h\\a\\p"))


def device(**kw):
    main = FakeMain(**kw)
    return OneWili(transport=main), main


class StreamsTests(unittest.TestCase):
    def test_write_rejects_bad_peer_and_length_without_a_command(self):
        dev, main = device()
        for dst in (-1, 5, True, "2", 2.0):
            with self.assertRaises(ValueError):
                dev.streams.write(dst, b"x")
        for data in (b"", bytes(Streams.MTU + 1)):
            with self.assertRaises(ValueError):
                dev.streams.write(Streams.ESP32, data)
        self.assertEqual(main.sent, [])

    def test_write_encodes_the_datagram(self):
        dev, main = device()
        dev.streams.write(Streams.ESP32, bytearray(b"\x00\xff\x10"))
        self.assertEqual(main.sent, ["h\\a\\w 2 00 FF 10"])

    def test_loopback_round_trip_batches_polls(self):
        dev, main = device()
        big = bytes(range(Streams.MTU))
        for data in (b"one", b"two", big):
            dev.streams.write(Streams.HOST, data)
        self.assertEqual(dev.streams.poll(), (Streams.HOST, b"one"))
        self.assertEqual(dev.streams.poll(), (Streams.HOST, b"two"))
        self.assertEqual(dev.streams.poll(), (Streams.HOST, big))
        self.assertEqual(main.polls(), 1)
        self.assertIsNone(dev.streams.poll())
        self.assertEqual(main.polls(), 2)
        self.assertEqual(main.sent[3], f"h\\a\\p {Streams.POLL_BYTES}")

    def test_batch_holds_whole_records_up_to_poll_bytes(self):
        dev, main = device()
        big = bytes(Streams.MTU)
        for _ in range(3):
            dev.streams.write(Streams.HOST, big)
        self.assertEqual(dev.streams.poll(), (Streams.HOST, big))
        self.assertEqual(dev.streams.poll(), (Streams.HOST, big))
        self.assertEqual((main.polls(), len(main.queue)), (1, 1))
        self.assertEqual(dev.streams.poll(), (Streams.HOST, big))
        self.assertEqual(main.polls(), 2)

    def test_transport_poll_bytes_is_honoured(self):
        dev, main = device(poll_bytes=1024)
        big = bytes(Streams.MTU)
        for _ in range(7):
            dev.streams.write(Streams.HOST, big)
        for _ in range(7):
            self.assertEqual(dev.streams.poll(), (Streams.HOST, big))
        self.assertEqual(main.polls(), 1)
        self.assertIn("h\\a\\p 1024", main.sent)

    def test_drops_sum_main_counters_and_local_losses(self):
        dev, main = device()
        self.assertEqual(dev.streams.drops(), 0)
        dev.streams.write(Streams.MAIN, b"x")   # MAIN drops it: not an error here
        self.assertEqual(dev.streams.drops(), 1)
        main.dropped_to = 2
        self.assertEqual(dev.streams.drops(), 3)

    def test_corrupt_batch_is_dropped_and_counted_once(self):
        dev, main = device()
        dev.streams.write(Streams.HOST, b"ok")
        dev.streams.write(Streams.HOST, b"lost")
        main.corrupt = bytes([Streams.HOST, 0x7F, 1])   # claims 127 bytes, has 1
        self.assertEqual(dev.streams.poll(), (Streams.HOST, b"ok"))
        self.assertEqual(dev.streams.poll(), (Streams.HOST, b"lost"))
        with self.assertRaisesRegex(RuntimeError, "bad record"):
            dev.streams.poll()
        self.assertEqual(dev.streams.drops(), 1)
        self.assertIsNone(dev.streams.poll())

        main.corrupt = bytes([Streams.HOST])             # a cut record header
        with self.assertRaisesRegex(RuntimeError, "cut record header"):
            dev.streams.poll()
        main.corrupt = bytes([9, 1, 0])                  # no such peer
        with self.assertRaisesRegex(RuntimeError, "bad record"):
            dev.streams.poll()
        self.assertEqual(dev.streams.drops(), 3)

    def test_failed_commands_raise(self):
        for path, call in (("h\\a\\w", lambda s: s.write(Streams.HOST, b"x")),
                           ("h\\a\\p", lambda s: s.poll()),
                           ("h\\a\\c", lambda s: s.drops())):
            dev, main = device()
            main.fail.add(path)
            with self.assertRaises(RuntimeError):
                call(dev.streams)

    def test_dev_streams_is_one_lazy_instance(self):
        dev, main = device()
        self.assertIs(dev.streams, dev.streams)
        self.assertIsInstance(dev.streams, Streams)
        self.assertEqual(main.sent, [])


if __name__ == "__main__":
    unittest.main()
