#include "bitutils.h"

inline bool get_bit(uint8_t byte, size_t bit)
{
  return byte & (1 << bit);
}

inline bool get_bit_bytes(uint8_t const *bytes, size_t bit)
{
  return get_bit(bytes[bit / 8], bit % 8);
}

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
