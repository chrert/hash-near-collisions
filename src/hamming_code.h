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
bool hamming_correct_inplace(uint64_t *codeword, size_t parity_bits);

#endif
