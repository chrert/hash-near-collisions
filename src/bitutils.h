#ifndef BITUTILS_H__
#define BITUTILS_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Computes the hamming weight of a byte
 * @param byte
 * @return
 */
extern unsigned int hamming_weight_byte(uint8_t byte);

/**
 * @brief Computes the hamming weight of a byte buffer
 * @param bytes
 * @param len
 * @return
 */
unsigned int hamming_weight_bytes(uint8_t const *bytes, size_t len);

/**
 * @brief Computes the hamming distance betweeen two bytes
 * @param byte1
 * @param byte2
 * @return
 */
extern unsigned int hamming_distance_byte(uint8_t byte1, uint8_t byte2);

/**
 * @brief Computes the hamming distance between two byte buffers
 * @param bytes1
 * @param bytes2
 * @param len
 * @return
 */
unsigned int hamming_distance_bytes(uint8_t const *bytes1, uint8_t const *bytes2, size_t len);

/**
 * @brief Counts the number of leading zeros of a 32 bit word
 * @param byte
 * @return
 */
unsigned int count_leading_zeros(uint32_t x);

/**
 * @brief Counts the number of training zeros of a 32 bit word
 * @param x
 * @return
 */
unsigned int count_trailing_zeros(uint32_t x);
#endif
