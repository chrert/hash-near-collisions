#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "configuration.h"
#include "bitutils.h"
#include "md5/md5.h"
#include "brents.h"
#include "distinguished_points.h"
#include "hamming_code.h"

void hamming_truncated_md5(uint8_t const* input, size_t input_len,
                           uint8_t output[TRUNCATED_SIZE]);

/**
 * @brief Computes a truncated md5 hash.
 * @param input the input buffer
 * @param input_len the length of the input buffer
 * @param output A buffer for the truncated hash ()
 */
void truncated_md5(uint8_t const* input, size_t input_len,
                   uint8_t output[TRUNCATED_SIZE]);

/**
 * @brief A string for a hex representation. Used in conjunction with
 * sprint_bytes_hex()
 */
typedef char hex_str[3 * TRUNCATED_SIZE];

/**
 * @brief Create a hex representation of a byte array
 * @param bytes
 * @param str
 */
void sprint_bytes_hex(uint8_t const bytes[TRUNCATED_SIZE], hex_str str);

/**
 * @brief The callback of brent's algorithm. Print progress.
 * @param power
 */
void brents_power_updated(unsigned int power);

/**
 * @brief dp_found_dp
 * @param trail
 * @param possibleCollision
 */
void dp_found_dp(dp_trail_t const* trail, bool possibleCollision) {
  printf("Point found!\n");
  hex_str str_dp, str_y0;
  sprint_bytes_hex(trail->y0, str_y0);
  sprint_bytes_hex(trail->dp, str_dp);
  printf("%s -> %s\n", str_y0, str_dp);

  if (possibleCollision) {
    printf("Possible collision detected!\n");
  }
}

void generate_random_bytes(size_t size, uint8_t bytes[size]) {
  for (int i = 0; i < TRUNCATED_SIZE; ++i) {
    bytes[i] = rand();
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: collider method\n");
    return -1;
  }

  struct timespec start_time, end_time;
  timespec_get(&start_time, TIME_UTC);

  srand((unsigned int)time(NULL));

  uint8_t m1[TRUNCATED_SIZE];
  uint8_t m2[TRUNCATED_SIZE];

  if (strcmp(argv[1], "b") == 0) {
    // generate random y0
    uint8_t y0[TRUNCATED_SIZE];
    generate_random_bytes(TRUNCATED_SIZE, y0);

    hex_str y0_str;
    sprint_bytes_hex(y0, y0_str);
    printf("Random y0: %s\n", y0_str);

    printf("Starting cycle finding...\n");
    uint64_t lambda, mu;

    brents_cycle_find_collision(TRUNCATED_SIZE, y0, hamming_truncated_md5,
                                &lambda, &mu, m1, m2, brents_power_updated);

    printf("lambda: %" PRIu64 "\n", lambda);
    printf("mu:     %" PRIu64 "\n", mu);

  } else if (strcmp(argv[1], "dp") == 0) {
    dp_find_collision_parallel(TRUNCATED_SIZE, hamming_truncated_md5,
                               generate_random_bytes, 4, 74, m1, m2, NULL);
  } else {
    printf("Unknown method!\n");
    return -1;
  }

  hex_str m1_str, m2_str;
  sprint_bytes_hex(m1, m1_str);
  sprint_bytes_hex(m2, m2_str);
  printf("m1: %s\n", m1_str);
  printf("m2: %s\n", m2_str);

  uint8_t h1[TRUNCATED_SIZE], h2[TRUNCATED_SIZE];
  truncated_md5(m1, TRUNCATED_SIZE, h1);
  truncated_md5(m2, TRUNCATED_SIZE, h2);
  printf("Hamming distance: %u\n",
         hamming_distance_bytes(h1, h2, TRUNCATED_SIZE));

  timespec_get(&end_time, TIME_UTC);
  printf("Computation took %ld seconds...\n",
         end_time.tv_sec - start_time.tv_sec);

  return 0;
}

void brents_power_updated(unsigned int power) {
  printf("Checked cycle length 2^%u\n", power);
  fflush(stdout);
}

void hamming_truncated_md5(uint8_t const* input, size_t input_len,
                           uint8_t output[TRUNCATED_SIZE]) {
  truncated_md5(input, input_len, output);

  for (size_t i = 0; i < 2; ++i) {
    hamming_correct_inplace16(output, i * 2);
  }

  for (size_t i = 0; i < 8; ++i) {
    hamming_correct_inplace8(output, 4 + i);
  }
}

/**
 * Size of a md5 hash in bytes
 */
#define MD5_SIZE 128 / 8

void truncated_md5(uint8_t const* input, size_t input_len,
                   uint8_t output[TRUNCATED_SIZE]) {
  md5_byte_t md5_out[MD5_SIZE];
  md5_state_t state;
  md5_init(&state);
  md5_append(&state, input, input_len);
  md5_finish(&state, md5_out);
  memcpy(output, md5_out, TRUNCATED_SIZE);
}

void sprint_bytes_hex(uint8_t const bytes[TRUNCATED_SIZE], hex_str str) {
  for (int i = 0; i < TRUNCATED_SIZE; ++i) {
    str += sprintf(str, "%02X", (unsigned int)bytes[i]);
    if (i != (TRUNCATED_SIZE - 1)) {
      str += sprintf(str, " ");
    }
  }
}
