#ifndef DISTINGUISHED_POINTS_H
#define DISTINGUISHED_POINTS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief A hash function
 */
typedef void (*dp_hash_function_t)(uint8_t const *input, size_t input_len, uint8_t *output);

/**
 * @brief A random byte generator
 */
typedef void (*dp_random_byte_generator_t)(size_t len, uint8_t r[len]);

/**
 * @brief represents a stored trail which leads to a distinguished point
 */
typedef struct
{
  /**
   * @brief y0 starting point
   */
  uint8_t *y0;

  /**
   * @brief dp distinguished point
   */
  uint8_t *dp;

  /**
   * @brief l length of the trail ( i.e.: dp = h_l(y0) )
   */
  uint64_t l;

} dp_trail_t;

/**
 * @brief Callback function for dp_find_collision_parallel()
 */
typedef void (*dp_callback_t)(dp_trail_t const *found_trail, bool possibleCollision);

/**
 * @brief Performs a parallel collision search based on distinguished points
 * @param len The lenght of the hashes
 * @param h The hash function to be tested
 * @param rnd A function that generates random bytes
 * @param num_threads Number of parallel threads to use
 * @param num_leading_zeros The property of the distinguished points
 * @param m1 A buffer for the first colliding message, s.t. h(m1) = h(m2)
 * @param m2 A buffer for the second colliding message, s.t. h(m2) = h(m1)
 * @param point_found_callback This callback function will be called if
 * a distinguished point was found
 */
void dp_find_collision_parallel(size_t hash_len, dp_hash_function_t h, dp_random_byte_generator_t rnd,
                                unsigned int num_threads, unsigned int num_leading_zeros,
                                uint8_t m1[hash_len], uint8_t m2[hash_len],
                                dp_callback_t point_found_callback);

/**
 * @brief create_dp_trail
 * @param y0
 * @param y0_len
 * @param dp
 * @param dp_len
 * @param l
 * @return
 */
dp_trail_t * create_dp_trail(uint8_t const *y0, size_t y0_len, uint8_t const *dp, size_t dp_len, uint64_t l);

/**
 * @brief destroy_dp_trail
 * @param trail
 */
void destroy_dp_trail(dp_trail_t *trail);

#endif // DISTINGUISHED_POINTS_H

