/* crc32_la.c - LoongArch version of crc32
 * Copyright (C) 2025 Vladislav Shchapov <vladislav@shchapov.ru>
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifdef LOONGARCH_CRC

#include "zbuild.h"

#include <larchintrin.h>

Z_FORCEINLINE static uint32_t crc32_loongarch64_align(uint32_t crc, uint8_t *dst, const uint8_t *buf,
                                                      uintptr_t align_diff, const int COPY) {
    if (align_diff & 1) {
        if (COPY)
            *dst++ = *buf;
        crc = (uint32_t)__crc_w_b_w((char)(*buf++), (int)crc);
    }

    if (align_diff & 2) {
        if (COPY) {
            memcpy(dst, buf, 2);
            dst += 2;
        }
        crc = (uint32_t)__crc_w_h_w((short)*((uint16_t*)buf), (int)crc);
        buf += 2;
    }

    if (align_diff & 4) {
        if (COPY) {
            memcpy(dst, buf, 4);
            dst += 4;
        }
        crc = (uint32_t)__crc_w_w_w((int)*((uint32_t*)buf), (int)crc);
        buf += 4;
    }

    if (align_diff & 8) {
        if (COPY) {
            memcpy(dst, buf, 8);
            dst += 8;
        }
        crc = (uint32_t)__crc_w_d_w((long int)*((uint64_t*)buf), (int)crc);
        buf += 8;
    }

    return crc;
}

Z_FORCEINLINE static uint32_t crc32_loongarch64_tail(uint32_t crc, uint8_t *dst, const uint8_t *buf, size_t len,
                                                     const int COPY) {
    while (len >= 8) {
        if (COPY) {
            memcpy(dst, buf, 8);
            dst += 8;
        }
        crc = (uint32_t)__crc_w_d_w((long int)*((uint64_t*)buf), (int)crc);
        buf += 8;
        len -= 8;
    }

    if (len & 4) {
        if (COPY) {
            memcpy(dst, buf, 4);
            dst += 4;
        }
        crc = (uint32_t)__crc_w_w_w((int)*((uint32_t*)buf), (int)crc);
        buf += 4;
    }

    if (len & 2) {
        if (COPY) {
            memcpy(dst, buf, 2);
            dst += 2;
        }
        crc = (uint32_t)__crc_w_h_w((short)*((uint16_t*)buf), (int)crc);
        buf += 2;
    }

    if (len & 1) {
        if (COPY)
            *dst = *buf;
        crc = (uint32_t)__crc_w_b_w((char)(*buf), (int)crc);
    }

    return ~crc;
}

Z_INTERNAL uint32_t crc32_loongarch64(uint32_t crc, const uint8_t *buf, size_t len) {
    uint32_t c = ~crc;

    if (UNLIKELY(len == 1)) {
        c = (uint32_t)__crc_w_b_w((char)(*buf), (int)c);
        return ~c;
    }

    /* Align to 8-byte boundary for tail processing */
    uintptr_t align_diff = MIN(ALIGN_DIFF(buf, 8), len);
    if (align_diff) {
        c = crc32_loongarch64_align(c, NULL, buf, align_diff, 0);

        buf += align_diff;
        len -= align_diff;
    }

    return crc32_loongarch64_tail(c, NULL, buf, len, 0);
}

Z_INTERNAL uint32_t crc32_copy_loongarch64(uint32_t crc, uint8_t *dst, const uint8_t *src, size_t len) {
    uint32_t c = ~crc;

    if (UNLIKELY(len == 1)) {
        *dst = *src;
        c = (uint32_t)__crc_w_b_w((char)(*src), (int)c);
        return ~c;
    }

    /* Align to 8-byte boundary for tail processing */
    uintptr_t align_diff = MIN(ALIGN_DIFF(src, 8), len);
    if (align_diff) {
        c = crc32_loongarch64_align(c, dst, src, align_diff, 1);

        dst += align_diff;
        src += align_diff;
        len -= align_diff;
    }

    return crc32_loongarch64_tail(c, dst, src, len, 1);
}

#endif
