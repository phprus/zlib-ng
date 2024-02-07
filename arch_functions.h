/* arch_functions.h -- Arch-specific function prototypes.
 * Copyright (C) 2017 Hans Kristian Rosbach
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#ifndef CPU_FUNCTIONS_H_
#define CPU_FUNCTIONS_H_

// Workaround for MSVC. By default MSVC not defined __SSE*__ macros.
// Fix it if AVX enabled.
#if defined(_MSC_VER) && !defined(__clang__) && defined(__AVX__)
#  define __SSE__ 1
#  define __SSE2__ 1
#  define __SSE3__ 1
#  define __SSSE3__ 1
#  define __SSE4_1__ 1
#  define __SSE4_2__ 1
#  define __PCLMUL__ 1
#endif

#include "zbuild.h"
#include "zutil.h"
#include "crc32.h"
#include "deflate.h"
#include "fallback_builtins.h"

#include "arch/generic/generic_functions.h"

#if defined(X86_FEATURES)
#  include "arch/x86/x86_functions.h"
#elif defined(ARM_FEATURES)
#  include "arch/arm/arm_functions.h"
#elif defined(PPC_FEATURES) || defined(POWER_FEATURES)
#  include "arch/power/power_functions.h"
#elif defined(S390_FEATURES)
#  include "arch/s390/s390_functions.h"
#elif defined(RISCV_FEATURES)
#  include "arch/riscv/riscv_functions.h"
#endif

#endif
