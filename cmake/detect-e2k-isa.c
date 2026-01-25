/* detect-e2k-isa.c -- Detect and print to stderr e2k architecture version.
 * Copyright (C) 2026 Vladislav Shchapov
 * Licensed under the Zlib license, see LICENSE.md for details
 */

#include <stdio.h>

#define STRINGIZE(X) DO_STRINGIZE(X)
#define DO_STRINGIZE(X) #X

int main(void) {
    fprintf(stderr, "archversion " STRINGIZE(__iset__) "\n");
    return 0;
}
