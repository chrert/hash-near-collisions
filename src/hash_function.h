#ifndef HASH_FUNCTION_H__
#define HASH_FUNCTION_H__

#include <inttypes.h>
#include <stddef.h>

#define CFG_MD5_FULL 0
#define CFG_100BIT_EPS20_UNIFORM 1
#define CFG_88BIT_EPS8_UNIFORM 2
#define CFG_80BIT_FULL 3

#ifndef HASH_CONFIG
#error Please define HASH_CONFIG! Possible values: TODO
#endif

#if HASH_CONFIG == CFG_MD5_FULL
#define HASH_BYTES 128/8
#elif HASH_CONFIG == CFG_100BIT_EPS20_UNIFORM
#define HASH_BYTES 96 / 8
#elif HASH_CONFIG == CFG_88BIT_EPS8_UNIFORM
#define HASH_BYTES 64 / 8
#elif HASH_CONFIG == CFG_80BIT_FULL
#define HASH_CONFIG 80 / 8
#endif

#ifndef HASH_BYTES
#error HASH_BYTES is undefined!
#endif

typedef uint8_t hash_t[HASH_BYTES];

void hash_function(uint8_t const* input, size_t input_len, hash_t output);

/**
 * @brief A string for a hex representation. Used in conjunction with
 * sprint_bytes_hex()
 */
typedef char hash_str_t[3 * HASH_BYTES];

/**
 * @brief Create a hex representation of a hash output
 * @param bytes
 * @param str
 */
void sprint_hash_hex(uint8_t const bytes[HASH_BYTES], hash_str_t str);

#endif
