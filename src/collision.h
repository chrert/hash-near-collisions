#ifndef COLLISION_H__
#define COLLISION_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//----------------------------------------------------------------
// BRENTS
//----------------------------------------------------------------

/**
 * @brief A hash function
 */
typedef void (*hash_function_t)(uint8_t const *input, size_t input_len, uint8_t *output);

/**
 * @brief Callback function for brents_cycle_find()
 */
typedef void (*brents_callback_t)(unsigned int power);

/**
 * @brief Performs Brent's cycle finding algorithm and returns the cycle length lambda,
 * the tail length mu, as well as two colliding messages m1 and m2.
 * @param len The length of the buffers (y0, m1, m2)
 * @param y0 The starting value
 * @param h A function to iterate
 * @param lambda A pointer where the resulting lambda will be written to
 * @param mu A pointer where the resulting mu will be written to
 * @param m1 A buffer for the first colliding message, s.t. h(m1) = h(m2)
 * @param m2 A buffer for the second colliding message, s.t. h(m2) = h(m1)
 * @param power_step_callback A pointer to a callback function, that will be called on
 * every power step (2^1, 2^2, 2^3, ...)
 */
void brents_cycle_find_collision(size_t len, uint8_t const y0[len],
                       hash_function_t h,
                       uint64_t *lambda, uint64_t *mu,
                       uint8_t m1[len], uint8_t m2[len],
                       brents_callback_t power_step_callback);


//----------------------------------------------------------------
// DISTINGUISHED POINTS
//----------------------------------------------------------------


/**
 * @brief A random byte generator
 */
typedef void (*random_byte_generator_t)(size_t len, uint8_t r[len]);


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
typedef void (*dp_callback_t)(dp_trail_t *found_trail, bool possibleCollision);

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
void dp_find_collision_parallel(size_t hash_len, hash_function_t h, random_byte_generator_t rnd,
                                unsigned int num_threads, unsigned int num_leading_zeros,
                                uint8_t m1[hash_len], uint8_t m2[hash_len],
                                dp_callback_t point_found_callback);

#endif
