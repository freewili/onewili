#include "onewili_binary.h"
#include <string.h>

ow_status ow_binary_open(ow_binary_device* bdev, const ow_transport* transport) {
    if (!bdev || !transport || !transport->read) return OW_ERR_ARG;
    memset(bdev, 0, sizeof *bdev);
    bdev->t = *transport;
    ow_bin_parser_init(&bdev->parser);
    return OW_OK;
}

void ow_binary_close(ow_binary_device* bdev) {
    if (bdev) memset(bdev, 0, sizeof *bdev); /* caller owns port and storage */
}

ow_status ow_binary_open_buffer(ow_binary_device* bdev, const ow_transport* transport,
                               uint8_t* buffer, uint32_t capacity) {
    ow_status status;
    if (!buffer || !capacity) return OW_ERR_ARG;
    status = ow_binary_open(bdev, transport);
    if (status == OW_OK) ow_bin_parser_init_buffer(&bdev->parser, buffer, capacity);
    return status;
}

int ow_binary_poll_raw(ow_binary_device* bdev, ow_bin_frame* out) {
    unsigned reads = 0;
    if (!bdev || !out || !bdev->t.read) return -(int)OW_ERR_ARG;
    for (;;) {
        while (bdev->rx_pos < bdev->rx_len) {
            int ready = 0;
            bdev->rx_pos += (uint32_t)ow_bin_parser_feed(
                &bdev->parser, bdev->rx + bdev->rx_pos,
                bdev->rx_len - bdev->rx_pos, out, &ready);
            if (ready) return 1;
        }
        bdev->rx_pos = bdev->rx_len = 0;
        if (++reads > 128) return 0; /* bounded work even under continuous traffic */
        {
            int n = bdev->t.read(bdev->t.ctx, bdev->rx, sizeof bdev->rx, 0);
            if (n == 0) return 0;
            if (n < 0 || (size_t)n > sizeof bdev->rx) return -(int)OW_ERR_IO;
            bdev->rx_len = (uint32_t)n;
        }
    }
}

int ow_binary_poll(ow_binary_device* bdev, ow_event* out) {
    unsigned frames;
    if (!bdev || !out) return -(int)OW_ERR_ARG;
    for (frames = 0; frames < 128; ++frames) {
        ow_bin_frame f;
        size_t i;
        int status = ow_binary_poll_raw(bdev, &f);
        if (status != 1) return status;
        for (i = 0; i < ow_event_decoder_count; ++i) {
            const ow_event_decoder* d = &ow_event_decoders[i];
            if (d->header_type != f.header_type) continue;
            if (d->decode(f.payload, f.payload_len, f.error, out) != OW_OK) {
                ++bdev->size_mismatches;
                break;
            }
            return 1;
        }
        if (i == ow_event_decoder_count) ++bdev->unknown_frames;
    }
    return 0;
}
