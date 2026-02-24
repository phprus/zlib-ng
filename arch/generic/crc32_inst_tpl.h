/* crc32_inst_tpl.h -- Private shared inline ARMv8 and LoongArch64 CRC32 functions
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zbuild.h"

#ifndef Z_TARGET_CRC
#  define Z_TARGET_CRC
#endif

Z_FORCEINLINE static Z_TARGET_CRC uint32_t CRC32_ALIGN(uint32_t crc, uint8_t *dst, const uint8_t *buf,
                                                       uintptr_t align_diff, const int COPY) {
    if (align_diff & 1) {
        if (COPY)
            *dst++ = *buf;
        crc = CRC32B(crc, *buf++);
    }

    if (align_diff & 2) {
        if (COPY) {
            memcpy(dst, buf, 2);
            dst += 2;
        }
        crc = CRC32H(crc, *((uint16_t*)buf));
        buf += 2;
    }

    if (align_diff & 4) {
        if (COPY) {
            memcpy(dst, buf, 4);
            dst += 4;
        }
        crc = CRC32W(crc, *((uint32_t*)buf));
        buf += 4;
    }

    if (align_diff & 8) {
        if (COPY) {
            memcpy(dst, buf, 8);
            dst += 8;
        }
        crc = CRC32D(crc, *((uint64_t*)buf));
        buf += 8;
    }

    return crc;
}

Z_FORCEINLINE static Z_TARGET_CRC uint32_t CRC32_TAIL(uint32_t crc, uint8_t *dst, const uint8_t *buf, size_t len,
                                                      const int COPY) {
    while (len >= 8) {
        if (COPY) {
            memcpy(dst, buf, 8);
            dst += 8;
        }
        crc = CRC32D(crc, *((uint64_t*)buf));
        buf += 8;
        len -= 8;
    }

    if (len & 4) {
        if (COPY) {
            memcpy(dst, buf, 4);
            dst += 4;
        }
        crc = CRC32W(crc, *((uint32_t*)buf));
        buf += 4;
    }

    if (len & 2) {
        if (COPY) {
            memcpy(dst, buf, 2);
            dst += 2;
        }
        crc = CRC32H(crc, *((uint16_t*)buf));
        buf += 2;
    }

    if (len & 1) {
        if (COPY)
            *dst = *buf;
        crc = CRC32B(crc, *buf);
    }

    return ~crc;
}
