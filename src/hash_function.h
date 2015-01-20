#ifndef HASH_FUNCTION_H__
#define HASH_FUNCTION_H__

#include <inttypes.h>
#include <stddef.h>

#define CFG_MD5_FULL 1
#define CFG_96BIT_EPS16_UNIFORM 2
#define CFG_88BIT_EPS8_UNIFORM 3
#define CFG_80BIT_FULL 4
#define CFG_64BIT_EPS2 5
#define CFG_88BIT_EPS8_SECOND 6
#define CFG_96BIT_EPS16_SECOND 7
#define CFG_128BIT_EPS16 8

#ifndef HASH_CONFIG
#error Please define HASH_CONFIG! See hash_function.h
#endif

#if HASH_CONFIG == CFG_MD5_FULL
#define HASH_BYTES (128 / 8)
#elif HASH_CONFIG == CFG_96BIT_EPS16_UNIFORM
#define HASH_BYTES (96 / 8)
#elif HASH_CONFIG == CFG_88BIT_EPS8_UNIFORM
#define HASH_BYTES (88 / 8)
#elif HASH_CONFIG == CFG_80BIT_FULL
#define HASH_BYTES (80 / 8)
#elif HASH_CONFIG == CFG_88BIT_EPS8_SECOND
#define HASH_BYTES (88 / 8)
#elif HASH_CONFIG == CFG_96BIT_EPS16_SECOND
#define HASH_BYTES (96 / 8)
#elif HASH_CONFIG == CFG_64BIT_EPS2
#define HASH_BYTES (64/8)
#elif HASH_CONFIG == CFG_128BIT_EPS16
#define HASH_BYTES (128 / 8)
#endif

#ifndef HASH_BYTES
#error HASH_BYTES is undefined!
#endif

/**
 * @brief hash_t
 */
typedef uint8_t hash_t[HASH_BYTES];

/**
 * @brief truncated_md5
 * @param input
 * @param input_len
 * @param output
 */
extern void truncated_md5(uint8_t const* input, size_t input_len, hash_t output);


/**
 * @brief hash_function
 * @param input
 * @param input_len
 * @param output
 */
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
