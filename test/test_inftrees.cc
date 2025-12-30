/* test_inftrees.cc -- test zlib's inflate routines with full code coverage
 * Copyright (C) 2011, 2016 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#  include "zbuild.h"
#  include "arch_functions.h"
#  include "test_cpu_features.h"
}

#include <gtest/gtest.h>

/* cover remaining lines in inftrees.c */
static inline void inflate_table_check(inflate_table_func inflate_table) {
    int ret;
    unsigned bits;
    uint16_t lens[16], work[16];
    code *next, table[ENOUGH_DISTS];

    /* we need to call inflate_table() directly in order to manifest not-
       enough errors, since zlib ensures that enough is always enough */
    for (bits = 0; bits < 15; bits++)
        lens[bits] = (uint16_t)(bits + 1);
    lens[15] = 15;
    next = table;
    bits = 15;
    ret = inflate_table(DISTS, lens, 16, &next, &bits, work);
    EXPECT_EQ(ret, 1);

    next = table;
    bits = 1;
    ret = inflate_table(DISTS, lens, 16, &next, &bits, work);
    EXPECT_EQ(ret, 1);
}

#define TEST_INFLATE_TABLE(name, func, support_flag) \
    TEST(inflate_table, name) { \
        if (!(support_flag)) { \
            GTEST_SKIP(); \
            return; \
        } \
        inflate_table_check(func); \
    }

TEST_INFLATE_TABLE(c, inflate_table_c, 1)

#ifdef DISABLE_RUNTIME_CPU_DETECTION
TEST_INFLATE_TABLE(native, native_inflate_table, 1)
#else

#ifdef ARM_NEON
TEST_INFLATE_TABLE(neon, inflate_table_neon, test_cpu_features.arm.has_neon)
#endif

#ifdef X86_SSS2
TEST_INFLATE_TABLE(sse2, inflate_table_sse2, test_cpu_features.x86.has_sse2)
#endif
#ifdef X86_AVX2
TEST_INFLATE_TABLE(avx2, inflate_table_avx2, test_cpu_features.x86.has_avx2)
#endif

#endif
