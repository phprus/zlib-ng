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

/* Count number of codes for each code length. */
static inline void count_lengths(uint16_t *lens, int codes, uint16_t *count) {
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

    vst1q_u16(&count[0], vaddl_u8(vget_low_u8(s1), vget_low_u8(s2)));
    vst1q_u16(&count[8], vaddl_u8(vget_high_u8(s1), vget_high_u8(s2)));
}


#define INFLATE_TABLE     inflate_table_neon

#include "inftrees_tpl.h"

#endif
