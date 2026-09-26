"""Generate PWM and verify it with OneWili's binary logic-analyzer stream.

Use an otherwise idle GPIO (default 25). This measures the processor pad;
no jumper is needed. FPGA power must be enabled and other host applications
must release the Main/FTDI ports. Cleanup stops capture, drives the pin LOW,
restores the host event gate and closes both ports. Existing pin mux/capture
configuration is not restored. Works with the same API on Windows/macOS/Linux.
"""
import argparse
import json
from pathlib import Path
import queue
import time

import onewili
from onewili.binary_events import LogicAnalyzerReportEvent


def measure(event, pin):
    # The firmware can prepend up to eight old PIO FIFO words. Preserve them
    # in the saved capture; exclude them only from the timing measurement.
    samples = list(event.samples(pin))
    skip = 8 * (32 // event.bits_per_sample)
    rising = [i for i in range(skip + 1, len(samples)) if samples[i] and not samples[i-1]]
    if len(rising) < 4:
        raise AssertionError("capture does not contain at least three complete PWM cycles")
    periods = [b-a for a, b in zip(rising, rising[1:])]
    high = [sum(samples[a:b]) for a, b in zip(rising, rising[1:])]
    frequency = 1e9 / (sum(periods) / len(periods) * event.sample_rate_ns)
    duty = 100 * sum(high) / sum(periods)
    return dict(samples=len(samples), sample_period_ns=event.sample_rate_ns,
                excluded_fifo_samples=skip, complete_cycles=len(periods),
                frequency_hz=frequency, duty_percent=duty)


def receive_capture(dev, timeout):
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if dev.binary_stream.last_error:
            raise OSError("binary reader failed") from dev.binary_stream.last_error
        try:
            event = dev.binary_events.get(timeout=min(.1, max(0, deadline-time.monotonic())))
        except queue.Empty:
            continue
        if isinstance(event, LogicAnalyzerReportEvent):
            if event.error:
                raise RuntimeError("device marked capture as an error")
            return event
    raise TimeoutError("no logic-analyzer capture; check FPGA power and host streaming")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--serial")
    parser.add_argument("--port", help="explicit Main serial port; requires --binary-port")
    parser.add_argument("--binary-port", help="explicit FTDI serial port")
    parser.add_argument("--pin", type=int, default=25)
    parser.add_argument("--frequency", type=float, default=1000)
    parser.add_argument("--duty", type=float, default=25)
    parser.add_argument("--samples", type=int, default=32768)
    parser.add_argument("--sample-period-ns", type=int, default=1000)
    parser.add_argument("--captures", type=int, default=3)
    parser.add_argument("--continuous", action="store_true", help="auto-rearm between captures")
    parser.add_argument("--timeout", type=float, default=15)
    parser.add_argument("--output", type=Path, default=Path("pwm-capture.json"))
    args = parser.parse_args()
    if bool(args.port) != bool(args.binary_port):
        parser.error("--port and --binary-port must be supplied together")
    if (args.captures < 1 or args.timeout <= 0 or args.samples < 1024
            or args.frequency <= 0 or not 0 < args.duty < 100 or args.sample_period_ns <= 0):
        parser.error("positive capture settings and 0 < duty < 100 required")
    if args.port:
        dev = onewili.OneWili(args.port, binary_port=args.binary_port).open()
    else:
        dev = onewili.connect(serial=args.serial, binary=True)
    restore_stream = None
    configured = False
    measurements = []
    try:
        dev.open_binary()
        restore_stream = dev.hardware.system.device_state().unwrap()[1]
        dev.hardware.system.event_host_streaming(1).unwrap()
        la = dev.io.logic_analyzer
        configured = True
        la.stop().unwrap()
        la.setup_analog(0, 1000, 8).unwrap()
        la.setup_logic_analyzer(args.sample_period_ns, args.samples,
                               args.pin, args.pin, args.pin, 2, int(args.continuous)).unwrap()
        dev.io.gpio.set_pwm(args.pin, args.frequency, args.duty).unwrap()
        # Prime once, then verify repeated full-buffer acquisitions.
        for index in range(args.captures + 1):
            if index == 0 or not args.continuous:
                la.start().unwrap()
            event = receive_capture(dev, args.timeout)
            if not args.continuous:
                la.stop().unwrap()
            # A text command must still complete while binary reception is active.
            dev.hardware.system.device_state().unwrap()
            if index == 0:
                continue
            result = measure(event, args.pin)
            if abs(result["frequency_hz"] / args.frequency - 1) > .02:
                raise AssertionError(f"PWM frequency mismatch: {result}")
            if abs(result["duty_percent"] - args.duty) > 2:
                raise AssertionError(f"PWM duty mismatch: {result}")
            measurements.append(result)
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.with_suffix(f".{index}.samples.bin").write_bytes(event.sample_data)
        stream = dev.binary_stream
        if stream.dropped_events or stream.size_mismatches:
            raise AssertionError("binary queue overflow or malformed capture")
        report = dict(pin=args.pin, continuous=args.continuous, expected_hz=args.frequency, expected_duty=args.duty,
                      captures=measurements, dropped_events=stream.dropped_events,
                      size_mismatches=stream.size_mismatches)
        args.output.write_text(json.dumps(report, indent=2) + "\n")
        print(json.dumps(report, indent=2))
    finally:
        try:
            if configured:
                try:
                    dev.io.logic_analyzer.stop().unwrap()
                finally:
                    dev.io.gpio.set_io_low(args.pin).unwrap()
        finally:
            try:
                if restore_stream is not None:
                    dev.hardware.system.event_host_streaming(int(restore_stream)).unwrap()
            finally:
                dev.close()


if __name__ == "__main__":
    main()
