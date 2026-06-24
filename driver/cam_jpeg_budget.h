#ifndef CAM_JPEG_BUDGET_H
#define CAM_JPEG_BUDGET_H

#include <stdint.h>
#include <stddef.h>

/*
 * JPEG receive-buffer budget for the PSRAM-DMA zero-copy capture path.
 *
 * This header is intentionally dependency-free (only <stdint.h>/<stddef.h>) so
 * the exact budget arithmetic compiled into the firmware can also be unit-tested
 * on the host build (tests/host/test_cam_jpeg_budget.c). It is included by
 * driver/cam_hal.c where cam_config() computes cam_obj->recv_size for JPEG mode.
 *
 * Sensor PIDs mirror the enum in driver/include/sensor.h; duplicated locally
 * under a distinct prefix so this header stays free of IDF/sensor includes.
 */
#define CAM_JPEG_BUDGET_OV3660_PID 0x3660u
#define CAM_JPEG_BUDGET_OV5640_PID 0x5640u

/*
 * OV3660/OV5640 are large-format (3MP/5MP) sensors whose hardware JPEG at high
 * quality routinely exceeds the w*h/5 budget tuned for OV2640 — at VGA that cap
 * is 60 KB, which trips cam_hal's DMA-overflow guard (cam_hal.c:314) and drops
 * every frame. Give the large-format class a w*h/2 budget (150 KB at VGA); keep
 * w*h/5 for OV2640 and any unknown sensor. See
 * docs/handoff/2026-06-24-esp32-ov3660-capture-rca-handoff.md.
 */
static inline size_t cam_jpeg_recv_size(uint16_t width, uint16_t height, uint16_t sensor_pid)
{
    if (sensor_pid == CAM_JPEG_BUDGET_OV3660_PID || sensor_pid == CAM_JPEG_BUDGET_OV5640_PID) {
        return (size_t)width * (size_t)height / 2u;
    }
    return (size_t)width * (size_t)height / 5u;
}

#endif /* CAM_JPEG_BUDGET_H */
