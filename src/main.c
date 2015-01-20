#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "hash_function.h"
#include "bitutils.h"
#include "brents.h"
#include "distinguished_points.h"


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
void dp_found_dp(dp_trail_t const* trail, size_t dp_length, bool possibleCollision) {
  printf("Point found!\n");

  hash_str_t str_y0;
  sprint_hash_hex(trail->y0, str_y0);

  hash_str_t str_dp;
  hash_t dp = {0};
  size_t dp_length_diff = HASH_BYTES - dp_length;
  memcpy(dp + dp_length_diff, trail->dp, dp_length);
  sprint_hash_hex(dp, str_dp);

  printf("%s -> %s (%" PRIu64 ")\n", str_y0, str_dp, trail->l);

  if (possibleCollision) {
    printf("Possible collision detected!\n");
  }
}

void generate_random_bytes(size_t size, uint8_t bytes[size]) {
  for (int i = 0; i < size; ++i) {
    bytes[i] = rand();
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s METHOD\n", argv[0]);
    return -1;
  }

  printf("Hash size is: %d\n", HASH_BYTES);

  struct timespec start_time, end_time;
  timespec_get(&start_time, TIME_UTC);

  srand((unsigned int)time(NULL));

  hash_t m1;
  hash_t m2;

  if (strcmp(argv[1], "b") == 0) {
    // generate random y0
    uint8_t y0[HASH_BYTES];
    generate_random_bytes(HASH_BYTES, y0);

    hash_str_t y0_str;
    sprint_hash_hex(y0, y0_str);
    printf("Random y0: %s\n", y0_str);

    printf("Starting cycle finding...\n");
    uint64_t lambda, mu;

    brents_cycle_find_collision(HASH_BYTES, y0, hash_function,
                                &lambda, &mu, m1, m2, brents_power_updated);

    printf("lambda: %" PRIu64 "\n", lambda);
    printf("mu:     %" PRIu64 "\n", mu);

  } else if (strcmp(argv[1], "dp") == 0) {
    if (argc < 4) {
      printf("Usage: %s dp NUM_THREADS NUM_LEADING_ZEROS\n", argv[0]);
      return -1;
    }

    unsigned int num_threads = strtol(argv[2], NULL, 10);
    if (num_threads == 0) {
      printf("Usage: %s NUM_THREADS NUM_LEADING_ZEROS\n", argv[0]);
      return -1;
    }

    unsigned int num_leading_zeros = strtol(argv[3], NULL, 10);
    printf("Using %u leading zeros as dp-property!\n", num_leading_zeros);

    dp_find_collision_parallel(HASH_BYTES, hash_function,
                               generate_random_bytes, num_threads, num_leading_zeros, m1,
                               m2, NULL);
  } else if (strcmp(argv[1], "test") == 0) {
    // test iteration speed
    if (argc < 3) {
      printf("Expecting iteration parameter!\n");
      return -1;
    }

    unsigned long num_iterations = strtoul(argv[2], NULL, 10);

    if (num_iterations == 0) {
      printf("Invalid number of iterations: %s\n", argv[2]);
      return -1;
    }

    printf("Running %lu hash iterations...\n", num_iterations);

    uint8_t y0[HASH_BYTES];
    generate_random_bytes(HASH_BYTES, y0);
    for (unsigned long i = 0; i < num_iterations; ++i) {
      hash_function(y0, HASH_BYTES, y0);
    }
  } else {
    printf("Unknown method!\n");
    return -1;
  }

  hash_t h1, h2;
  truncated_md5(m1, HASH_BYTES, h1);
  truncated_md5(m2, HASH_BYTES, h2);

  hash_str_t m1_str, m2_str, h1_str, h2_str;
  sprint_hash_hex(m1, m1_str);
  sprint_hash_hex(m2, m2_str);
  sprint_hash_hex(h1, h1_str);
  sprint_hash_hex(h2, h2_str);
  printf("m1: %s -> %s\n", m1_str, h1_str);
  printf("m2: %s -> %s\n", m2_str, h2_str);

  printf("Hamming distance: %u\n",
         hamming_distance_bytes(h1, h2, HASH_BYTES));

  timespec_get(&end_time, TIME_UTC);
  printf("Computation took %ld seconds...\n",
         end_time.tv_sec - start_time.tv_sec);

  return 0;
}

void brents_power_updated(unsigned int power) {
  printf("Checked cycle length 2^%u\n", power);
  fflush(stdout);
}

