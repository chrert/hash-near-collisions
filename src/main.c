#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

#include "configuration.h"
#include "md5/md5.h"
#include "collision.h"

/**
 * @brief Computes a truncated md5 hash.
 * @param input the input buffer
 * @param input_len the length of the input buffer
 * @param output A buffer for the truncated hash ()
 */
void truncated_md5(uint8_t const *input, size_t input_len, uint8_t output[TRUNCATED_SIZE]);

/**
 * @brief The callback of brent's algorithm. Print progress.
 * @param power
 */
void brents_power_updated(unsigned int power);

/**
 * @brief A string for a hex representation. Use in conjunction with sprint_bytes_hex()
 */
typedef char hex_str[3 * TRUNCATED_SIZE];

/**
 * @brief Create a hex representation of a byte array
 * @param bytes
 * @param str
 */
void sprint_bytes_hex(uint8_t const bytes[TRUNCATED_SIZE], hex_str str);

int main(int argc, char *argv[])
{
  struct timespec start_time, end_time;
  timespec_get(&start_time, TIME_UTC);

  // initialize rand
  srand((unsigned int) time(NULL));

  // generate random y0
  uint8_t y0[TRUNCATED_SIZE];
  for (int i = 0; i < TRUNCATED_SIZE; ++i)
  {
    y0[i] = rand();
  }

  hex_str y0_str;
  sprint_bytes_hex(y0, y0_str);
  printf("Random y0: %s\n", y0_str);

  printf("Starting cycle finding...\n");
  uint64_t lambda, mu;
  uint8_t m1[TRUNCATED_SIZE];
  uint8_t m2[TRUNCATED_SIZE];
  brents_cycle_find(TRUNCATED_SIZE, y0, truncated_md5, brents_power_updated, &lambda, &mu, m1, m2);

  printf("lambda: %" PRIu64 "\n", lambda);
  printf("mu:     %" PRIu64 "\n", mu);

  hex_str m1_str, m2_str;
  sprint_bytes_hex(m1, m1_str);
  sprint_bytes_hex(m2, m2_str);
  printf("m1: %s\n", m1_str);
  printf("m2: %s\n", m2_str);

  uint8_t h1[TRUNCATED_SIZE], h2[TRUNCATED_SIZE];
  truncated_md5(m1, TRUNCATED_SIZE, h1);
  truncated_md5(m2, TRUNCATED_SIZE, h2);

  if (memcmp(h1, h2, TRUNCATED_SIZE) == 0)
  {
    hex_str h_str;
    sprint_bytes_hex(h1, h_str);
    printf("Success!\n");
    printf("Hash: %s\n", h_str);
  }
  else
  {
    printf("Fail!\n");
  }

  timespec_get(&end_time, TIME_UTC);
  printf("Computation took %ld seconds...\n", end_time.tv_sec - start_time.tv_sec);

  return 0;
}

/**
 * Size of a md5 hash in bytes
 */
#define MD5_SIZE 128/8

void truncated_md5(uint8_t const *input, size_t input_len, uint8_t output[TRUNCATED_SIZE])
{
  md5_byte_t md5_out[MD5_SIZE];
  md5_state_t state;
  md5_init(&state);
  md5_append(&state, input, input_len);
  md5_finish(&state, md5_out);
  memcpy(output, md5_out, TRUNCATED_SIZE);
}

void sprint_bytes_hex(uint8_t const bytes[TRUNCATED_SIZE], hex_str str)
{
  for (int i = 0; i < TRUNCATED_SIZE; ++i)
  {
    str += sprintf(str, "%02X", (unsigned int) bytes[i]);
    if (i != (sizeof(bytes) - 1))
    {
      str += sprintf(str, " ");
    }
  }
}

void brents_power_updated(unsigned int power)
{
  printf("Checked cycle length 2^%u\n", power);
  fflush(stdout);
}

