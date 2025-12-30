/* inftrees_sse2.c -- SSE2 generate Huffman trees for efficient decoding
 * Copyright (C) 1995-2024 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "x86_intrins.h"
#include "zbuild.h"
#include "zutil.h"
#include "inftrees.h"
#include "arch/generic/inftrees_one.h"
#include "fallback_builtins.h"
#include <stdint.h>


#if defined(HAVE_BUILTIN_CTZ) && defined(HAVE_BUILTIN_CLZ)
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

    __m128i s1 = _mm_setzero_si128();
    __m128i s2 = _mm_setzero_si128();

    if (codes & 1) {
        s1 = _mm_load_si128((const __m128i*)&inftrees_one[16 * lens[0]]);
    }
    for (sym = codes & 1; sym < codes; sym += 2) {
        s1 = _mm_add_epi8(s1, _mm_load_si128((const __m128i*)&inftrees_one[16 * lens[sym]]));
        s2 = _mm_add_epi8(s2, _mm_load_si128((const __m128i*)&inftrees_one[16 * lens[sym+1]]));
    }

    __m128i zero = _mm_setzero_si128();

    __m128i s1_lo = _mm_unpacklo_epi8(s1, zero);
    __m128i s2_lo = _mm_unpacklo_epi8(s2, zero);
    __m128i sum_lo = _mm_add_epi16(s1_lo, s2_lo);
    _mm_storeu_si128((__m128i*)&count[0], sum_lo);

    __m128i s1_hi = _mm_unpackhi_epi8(s1, zero);
    __m128i s2_hi = _mm_unpackhi_epi8(s2, zero);
    __m128i sum_hi = _mm_add_epi16(s1_hi, s2_hi);
    _mm_storeu_si128((__m128i*)&count[8], sum_hi);

#ifdef FAST_COUNT_MIN_MAX
    __m128i mask_lo = _mm_cmpeq_epi16(sum_lo, _mm_setzero_si128());
    __m128i mask_hi = _mm_cmpeq_epi16(sum_hi, _mm_setzero_si128());
    return ~(
        ((unsigned)_mm_movemask_epi8(mask_hi) << 16) | (unsigned)_mm_movemask_epi8(mask_lo)
    );
#endif
}


#define INFLATE_TABLE     inflate_table_sse2

#include "inftrees_tpl.h"
