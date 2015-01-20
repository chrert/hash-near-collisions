#include "hash_function.h"
#include "md5/md5.h"
#include "hamming_code.h"

#include <string.h>
#include <stdio.h>

/**
 * Size of a md5 hash in bytes
 */
#define MD5_SIZE 128 / 8

inline void truncated_md5(uint8_t const* input, size_t input_len, hash_t output) {
  md5_byte_t md5_out[MD5_SIZE];
  md5_state_t state;
  md5_init(&state);
  md5_append(&state, input, input_len);
  md5_finish(&state, md5_out);
  memcpy(output, md5_out, HASH_BYTES);
}

void hash_function(uint8_t const* input, size_t input_len, hash_t output) {
  truncated_md5(input, input_len, output);
#if HASH_CONFIG == CFG_MD5_FULL
  ;
#elif HASH_CONFIG == CFG_80BIT_FULL
  ;
#elif HASH_CONFIG == CFG_96BIT_EPS16_UNIFORM
  for (size_t i = 0; i < 6; ++i) {
    hamming_correct_inplace8(output, i);
  }
  hamming_correct_inplace16(output, 6);
  hamming_correct_inplace32(output, 8);
#elif HASH_CONFIG == CFG_96BIT_EPS16_SECOND
  for (size_t i = 0; i < 4; ++i) {
    hamming_correct_inplace8(output, i);
  }
  for (size_t i = 0; i < 4; ++i) {
    hamming_correct_inplace16(output, 4 + 2 * i);
  }
#elif HASH_CONFIG == CFG_88BIT_EPS8_UNIFORM
  hamming_correct_inplace32(output, 0);
  hamming_correct_inplace32(output, 4);
  hamming_correct_inplace16(output, 8);
  hamming_correct_inplace8(output, 10);
#elif HASH_CONFIG == CFG_88BIT_EPS8_SECOND
  hamming_correct_inplace64(output, 0);
  for (size_t i = 0; i < 3; ++i) {
    hamming_correct_inplace8(output, 8 + i);
  }
#elif HASH_CONFIG == CFG_64BIT_EPS2
  hamming_correct_inplace64(output, 0);
#elif HASH_CONFIG == CFG_128BIT_EPS16
  for (size_t i = 0; i < 8; ++i) {
    hamming_correct_inplace16(output, 2*i);
  }
#endif
}

void sprint_hash_hex(uint8_t const bytes[HASH_BYTES], hash_str_t str) {
  for (int i = 0; i < HASH_BYTES; ++i) {
    str += sprintf(str, "%02X", (unsigned int)bytes[i]);
    if (i != (HASH_BYTES - 1)) {
      str += sprintf(str, " ");
    }
  }
}
