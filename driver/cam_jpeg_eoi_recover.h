#ifndef CAM_JPEG_EOI_RECOVER_H
#define CAM_JPEG_EOI_RECOVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline bool cam_jpeg_find_recoverable_eoi(const uint8_t *jpeg,
                                                 size_t reported_len,
                                                 size_t recv_size,
                                                 size_t max_extra_scan,
                                                 size_t *eoi_offset_out)
{
    if (eoi_offset_out != NULL) {
        *eoi_offset_out = 0;
    }
    if (jpeg == NULL || recv_size < 2) {
        return false;
    }

    size_t scan_len = recv_size;
    if (max_extra_scan > 0 && reported_len < recv_size) {
        size_t bounded_len = reported_len + max_extra_scan;
        if (bounded_len < reported_len) {
            bounded_len = recv_size;
        }
        if (bounded_len < scan_len) {
            scan_len = bounded_len;
        }
    }

    if (scan_len < 2) {
        return false;
    }

    for (size_t offset = 0; offset + 1 < scan_len; offset++) {
        if (jpeg[offset] == 0xFF && jpeg[offset + 1] == 0xD9) {
            if (eoi_offset_out != NULL) {
                *eoi_offset_out = offset;
            }
            return true;
        }
    }

    return false;
}

#endif /* CAM_JPEG_EOI_RECOVER_H */
