#include "hamming_code.h"

#include <stdio.h>

const uint64_t PARITY_MASKS[] = { 0xaaaaaaaaaaaaaaaaUL, 0xccccccccccccccccUL,
                                  0xf0f0f0f0f0f0f0f0UL, 0xff00ff00ff00ff00UL,
                                  0xffff0000ffff0000UL, 0xffffffff00000000UL };

bool parity64(uint64_t v) {
#if __GNUC__ > 0
#if __SIZEOF_LONG_LONG__ == 64 / 8
  return __builtin_parityll(v);
#elif __SIZEOF_LONG__ == 64 / 8
  return __builtin_parityl(v);
#else
  return __builting_parity(v);
#endif
#else
  v ^= v >> 1;
  v ^= v >> 2;
  v = (v & 0x1111111111111111UL) * 0x1111111111111111UL;
  return (v >> 60) & 1;
#endif
}

bool parity32(uint32_t v) {
#if __GNUC__ > 0
#if __SIZEOF_LONG__ == 32 / 8
  return __builtin_parityl(v);
#elif __SIZEOF_INT__ == 32 / 8
  return __builtin_parity(v);
#else
#error no builtin parity for 32-bit word
#endif
#else
  v ^= v >> 1;
  v ^= v >> 2;
  v = (v & 0x11111111U) * 0x11111111U;
  return (v >> 28) & 1;
#endif
}

bool parity16(uint16_t v) {
#if __GNUC__ > 0
  return __builtin_parity(v);
#else
#error no builtin parity for 16-bit word
#endif
}

bool parity8(uint8_t v) {
#if __GNUC__ > 0
  return __builtin_parity(v);
#else
#error no builtin parity for 8-bit word
#endif
}

#ifndef GENERATE_HAMMING_CORRECT_INPLACE

#define HAMMING_GLUE_HELPER(x, y) x##y
#define HAMMING_GLUE(x, y) HAMMING_GLUE_HELPER(x, y)

#define GENERATE_HAMMING_CORRECT_INPLACE(BITS, PARITY_BITS)                    \
  bool HAMMING_GLUE(hamming_correct_inplace, BITS)(uint8_t * codeword,         \
                                                   size_t offset) {            \
    HAMMING_GLUE(HAMMING_GLUE(uint, BITS), _t) *codeword_ptr =                 \
        (HAMMING_GLUE(HAMMING_GLUE(uint, BITS), _t) *)(codeword + offset);     \
    size_t e = 0;                                                              \
    for (size_t parity_i = 0; parity_i < PARITY_BITS; ++parity_i) {            \
      e += (1 << parity_i) *                                                   \
           HAMMING_GLUE(parity, BITS)(*codeword_ptr & PARITY_MASKS[parity_i]); \
    }                                                                          \
    if (e != 0) {                                                              \
      *codeword_ptr ^= (1 << e);                                               \
      return true;                                                             \
    }                                                                          \
    return false;                                                              \
  }
#endif

GENERATE_HAMMING_CORRECT_INPLACE(64, 6)
GENERATE_HAMMING_CORRECT_INPLACE(32, 5)
GENERATE_HAMMING_CORRECT_INPLACE(16, 4)
GENERATE_HAMMING_CORRECT_INPLACE(8, 3)
