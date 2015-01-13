#ifndef HAMMING_CODE_H__
#define HAMMING_CODE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


/**
* @brief Perform correction on a hamming code word
* @param codeword The codeword to be corrected of length code_length
* @return true <=> an error was corrected
*/
bool hamming_correct_inplace64(uint8_t *codeword, size_t offset);
bool hamming_correct_inplace32(uint8_t *codeword, size_t offset);
bool hamming_correct_inplace16(uint8_t *codeword, size_t offset);
bool hamming_correct_inplace8(uint8_t *codeword, size_t offset);

#endif
