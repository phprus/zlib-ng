/* inftrees_neon.c -- NEN generate Huffman trees for efficient decoding
 * Copyright (C) 1995-2024 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifdef ARM_NEON
#include "neon_intrins.h"
#include "zbuild.h"
#include "zutil.h"
#include "inftrees.h"
#include "arch/generic/inftrees_one.h"


/* vaddvq_u16 and vceqzq_u16 is only available on aarch64 */
#if defined(HAVE_BUILTIN_CTZ) && defined(HAVE_BUILTIN_CLZ) && (          \
        defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC) \
    )
#  define FAST_COUNT_MIN_MAX
#endif

/* Count number of codes for each code length. */
static inline
#ifdef FAST_COUNT_MIN_MAX
unsigned
#else
void
#endif
count_lengths(uint16_t *lens, int codes, uint16_t *count) {
    int sym;

    uint8x16_t s1 = vdupq_n_u8(0);
    uint8x16_t s2 = vdupq_n_u8(0);

    if (codes & 1) {
        s1 = vld1q_u8(&inftrees_one[16 * lens[0]]);
    }
    for (sym = codes & 1; sym < codes; sym += 2) {
      s1 = vaddq_u8(s1, vld1q_u8(&inftrees_one[16 * lens[sym]]));
      s2 = vaddq_u8(s2, vld1q_u8(&inftrees_one[16 * lens[sym+1]]));
    }

    uint16x8_t sum_lo = vaddl_u8(vget_low_u8(s1), vget_low_u8(s2));
    uint16x8_t sum_hi = vaddl_u8(vget_high_u8(s1), vget_high_u8(s2));

    vst1q_u16(&count[0], sum_lo);
    vst1q_u16(&count[8], sum_hi);

#ifdef FAST_COUNT_MIN_MAX
    static const ALIGNED_(16) uint16_t bitmask[8] = { 0x3, 0xC, 0x30, 0xC0, 0x300, 0xC00, 0x3000, 0xC000 };
    uint16x8_t vbitmask = vld1q_u16(bitmask);
    uint16x8_t mask_lo = vandq_u16(vceqzq_u16(sum_lo), vbitmask);
    uint16x8_t mask_hi = vandq_u16(vceqzq_u16(sum_hi), vbitmask);
    return ~(
        ((unsigned)vaddvq_u16(mask_hi) << 16) | (unsigned)vaddvq_u16(mask_lo)
    );
#endif
}


#define INFLATE_TABLE     inflate_table_neon

#include "inftrees_tpl.h"

#endif
