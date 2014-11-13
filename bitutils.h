#ifndef BITUTILS_H__
#define BITUTILS_H__

#include <stdint.h>
#include <stddef.h>

unsigned int hamming_weight_byte(uint8_t byte);
unsigned int hamming_weight_bytes(uint8_t *bytes, size_t len);
unsigned int hamming_distance_byte(uint8_t byte1, uint8_t byte2);
unsigned int hamming_distance_bytes(uint8_t *bytes1, uint8_t *bytes2, size_t len);

#endif