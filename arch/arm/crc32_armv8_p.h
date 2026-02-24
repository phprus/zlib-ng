/* crc32_armv8_p.h -- Private shared inline ARMv8 CRC32 functions
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifndef CRC32_ARMV8_P_H
#define CRC32_ARMV8_P_H

#include "zbuild.h"
#include "acle_intrins.h"

#define CRC32B(crc, c) __crc32b((crc), (c))
#define CRC32H(crc, h) __crc32h((crc), (h))
#define CRC32w(crc, w) __crc32h((crc), (w))
#define CRC32D(crc, d) __crc32h((crc), (d))

#define CRC32_ALIGN crc32_armv8_align
#define CRC32_TAIL  crc32_armv8_tail

#include "arch/generic/crc32_inst_tpl.h"

#endif /* CRC32_ARMV8_P_H */
