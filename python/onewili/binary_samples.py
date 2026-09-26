"""Sample access for the firmware's logic-analyzer ring-buffer report."""
import struct


class LogicAnalyzerSamples:
    """Digital values are relative to gpio_start_pin, packed LSB first.

    The analog tail always occupies 2048 bytes when enabled. Digital head and
    trigger are word indices; analog head and trigger are sample indices.
    Returned samples are chronological. No FIFO prefix is silently discarded.
    """

    def validate(self):
        if len(self.sample_data) % 4:
            raise ValueError("logic-analyzer samples must contain whole words")
        if not 1 <= self.bits_per_sample <= 32 or not self.sample_rate_ns:
            raise ValueError("invalid digital sample format")
        analog_bytes = 2048 if self.analog_channel_mask else 0
        digital_bytes = len(self.sample_data) - analog_bytes
        if digital_bytes <= 0 or digital_bytes > (1 << 20):
            raise ValueError("invalid digital buffer size")
        words = digital_bytes // 4
        if self.buffer_head >= words or self.trigger_location >= words:
            raise ValueError("invalid digital ring indices")
        if self.analog_channel_mask:
            if (self.analog_channel_mask & ~15 or self.analog_resolution not in (8, 16)
                    or self.analog_channel_count != self.analog_channel_mask.bit_count()
                    or not self.analog_sample_rate_ns):
                raise ValueError("invalid analog sample format")
            capacity = 2048 * 8 // self.analog_resolution
            if (not 0 < self.analog_sample_count <= capacity
                    or self.analog_buffer_head >= self.analog_sample_count
                    or self.analog_trigger_location >= self.analog_sample_count):
                raise ValueError("invalid analog ring indices")

    @property
    def digital_data(self):
        return self.sample_data[:-2048] if self.analog_channel_mask else self.sample_data

    @property
    def analog_data(self):
        return self.sample_data[-2048:] if self.analog_channel_mask else b""

    @property
    def digital_words(self):
        """Words in wire/ring order, before chronological rotation."""
        return tuple(v[0] for v in struct.iter_unpack("<I", self.digital_data))

    def digital_samples(self):
        """Iterate chronological values; bit zero is gpio_start_pin."""
        words = self.digital_words
        mask = (1 << self.bits_per_sample) - 1
        for i in range(len(words)):
            word = words[(self.buffer_head + i) % len(words)]
            for shift in range(0, (32 // self.bits_per_sample) * self.bits_per_sample,
                               self.bits_per_sample):
                yield (word >> shift) & mask

    def samples(self, pin):
        """Iterate 0/1 samples for an absolute GPIO pin."""
        bit = pin - self.gpio_start_pin
        if not 0 <= bit < self.bits_per_sample:
            raise ValueError("pin is outside the captured range")
        return ((value >> bit) & 1 for value in self.digital_samples())

    def analog_samples(self):
        """Iterate chronological interleaved ADC values (ascending enabled pins)."""
        if not self.analog_channel_mask:
            return
        fmt = "<B" if self.analog_resolution == 8 else "<H"
        values = tuple(v[0] for v in struct.iter_unpack(fmt, self.analog_data))
        for i in range(self.analog_sample_count):
            yield values[(self.analog_buffer_head + i) % self.analog_sample_count]
