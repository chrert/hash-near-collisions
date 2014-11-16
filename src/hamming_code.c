#include "hamming_code.h"

#include <stdio.h>

const uint64_t PARITY_MASKS[] =
{
  0xaaaaaaaaaaaaaaaaUL,
  0xccccccccccccccccUL,
  0xf0f0f0f0f0f0f0f0UL,
  0xff00ff00ff00ff00UL,
  0xffff0000ffff0000UL,
  0xffffffff00000000UL
};

inline bool parity(uint64_t v)
{
  v ^= v >> 1;
  v ^= v >> 2;
  v = (v & 0x1111111111111111UL) * 0x1111111111111111UL;
  return (v >> 60) & 1;
}

bool hamming_correct_inplace(uint64_t *codeword, size_t parity_bits)
{
  size_t e = 0;
  for (size_t parity_i = 0; parity_i < parity_bits; ++parity_i)
  {
    e += (1 << parity_i) * parity(*codeword & PARITY_MASKS[parity_i]);
  }

  if (e != 0)
  {
    *codeword ^= (1 << e);
    return true;
  }

  return false;
}
