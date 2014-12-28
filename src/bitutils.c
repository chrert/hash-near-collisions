#include "bitutils.h"

inline unsigned int hamming_weight_byte(uint8_t x)
{
  return ((0x876543210 >>
           (((0x4332322132212110 >> ((x & 0xF) << 2)) & 0xF) << 2)) >>
          ((0x4332322132212110 >> (((x & 0xF0) >> 2)) & 0xF) << 2))
      & 0xf;
}

unsigned int hamming_weight_bytes(const uint8_t *bytes, size_t len)
{
  unsigned int hm = 0;
  for (int i = 0; i < len; ++i)
  {
    hm += hamming_weight_byte(bytes[i]);
  }
  return hm;
}

inline unsigned int hamming_distance_byte(uint8_t byte1, uint8_t byte2)
{
  return hamming_weight_byte(byte1 ^ byte2);
}

unsigned int hamming_distance_bytes(const uint8_t *bytes1, const uint8_t *bytes2, size_t len)
{
  unsigned hd = 0;
  for (int i = 0; i < len; ++i)
  {
    hd += hamming_weight_byte(bytes1[i] ^ bytes2[i]);
  }
  return hd;
}

unsigned int count_leading_zeros(uint32_t x)
{
#if __GNUC__
#if __SIZEOF_LONG__ == 32/8
  return __builtin_clzl(x);
#elif __SIZEOF_INT__ == 32/8
  return __builtin_clz(x);
#else
#error no bultin clz for 32-bit word
#endif
#else
  static uint8_t const clz_lkup[] = {
      32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
      28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U,
      27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
      27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
      24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U
  };
  uint32_t n;
  if (x >= (1U << 16)) {
      if (x >= (1U << 24)) {
          n = 24U;
      }
      else {
          n = 16U;
      }
  }
  else {
      if (x >= (1U << 8)) {
          n = 8U;
      }
      else {
          n = 0U;
      }
  }
  return (uint32_t)clz_lkup[x >> n] - n;
#endif
}

unsigned int count_trailing_zeros(uint32_t x)
{
#if __GNUC__
#if __SIZEOF_LONG__ == 32/8
  return __builtin_ctzl(x);
#elif __SIZEOF_INT__ == 32/8
  return __builtin_ctz(x);
#else
#error no bultin ctz for 32-bit word
#endif
#else
#error missing portable implementation for tlz
#endif
}

