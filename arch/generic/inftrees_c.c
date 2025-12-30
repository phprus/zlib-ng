/* inftrees_c.c -- generate Huffman trees for efficient decoding
 * Copyright (C) 1995-2024 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "zbuild.h"
#include "zutil.h"
#include "inftrees.h"

/* Count number of codes for each code length. */
static inline void count_lengths(uint16_t *lens, int codes, uint16_t *count) {
    int sym;
    int len;
    for (len = 0; len <= MAX_BITS; len++)
        count[len] = 0;
    for (sym = 0; sym < codes; sym++)
        count[lens[sym]]++;
}


#define INFLATE_TABLE     inflate_table_c

#include "inftrees_tpl.h"
