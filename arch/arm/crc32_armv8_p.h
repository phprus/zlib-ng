/* crc32_armv8_p.h -- Private shared inline ARMv8 CRC32 functions
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifndef CRC32_ARMV8_P_H
#define CRC32_ARMV8_P_H

#include "zbuild.h"
#include "acle_intrins.h"

Z_FORCEINLINE static Z_TARGET_CRC uint32_t crc32_armv8_align(uint32_t crc, uint8_t *dst, const uint8_t *buf,
                                                             uintptr_t align_diff, const int COPY) {
    if (align_diff & 1) {
        if (COPY)
            *dst++ = *buf;
        crc = __crc32b(crc, *buf++);
    }

    if (align_diff & 2) {
        if (COPY) {
            memcpy(dst, buf, 2);
            dst += 2;
        }
        crc = __crc32h(crc, *((uint16_t*)buf));
        buf += 2;
    }

    if (align_diff & 4) {
        if (COPY) {
            memcpy(dst, buf, 4);
            dst += 4;
        }
        crc = __crc32w(crc, *((uint32_t*)buf));
        buf += 4;
    }

    if (align_diff & 8) {
        if (COPY) {
            memcpy(dst, buf, 8);
            dst += 8;
        }
        crc = __crc32d(crc, *((uint64_t*)buf));
        buf += 8;
    }

    return crc;
}

Z_FORCEINLINE static Z_TARGET_CRC uint32_t crc32_armv8_tail(uint32_t crc, uint8_t *dst, const uint8_t *buf, size_t len,
                                                            const int COPY) {
    while (len >= 8) {
        if (COPY) {
            memcpy(dst, buf, 8);
            dst += 8;
        }
        crc = __crc32d(crc, *((uint64_t*)buf));
        buf += 8;
        len -= 8;
    }

    if (len & 4) {
        if (COPY) {
            memcpy(dst, buf, 4);
            dst += 4;
        }
        crc = __crc32w(crc, *((uint32_t*)buf));
        buf += 4;
    }

    if (len & 2) {
        if (COPY) {
            memcpy(dst, buf, 2);
            dst += 2;
        }
        crc = __crc32h(crc, *((uint16_t*)buf));
        buf += 2;
    }

    if (len & 1) {
        if (COPY)
            *dst = *buf;
        crc = __crc32b(crc, *buf);
    }

    return ~crc;
}

#endif /* CRC32_ARMV8_P_H */
