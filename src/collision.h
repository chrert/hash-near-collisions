#ifndef COLLISION_H__
#define COLLISION_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief A function to use in brents_cycle_find()
 */
typedef void (*brents_function)(uint8_t const *input, size_t input_len, uint8_t *output);

/**
 * @brief Callback function for brents_cycle_find()
 */
typedef void (*brents_callback)(unsigned int power);

/**
 * @brief Performs Brent's cycle finding algorithm and returns the cycle length lambda,
 * the tail length mu, as well as two colliding messages m1 and m2.
 * @param len The length of the buffers (y0, m1, m2)
 * @param y0 The starting value
 * @param h A function to iterate
 * @param power_step_callback A pointer to a callback function, that will be called on
 * every power step (2^1, 2^2, 2^3, ...)
 * @param lambda A pointer where the resulting lambda will be written to
 * @param mu A pointer where the resulting mu will be written to
 * @param m1 A buffer for the first colliding message, s.t. h(m1) = h(m2)
 * @param m2 A buffer for the second colliding message, s.t. h(m2) = h(m1)
 */
void brents_cycle_find(size_t len, uint8_t const y0[len],
                       brents_function h, brents_callback power_step_callback,
                       uint64_t *lambda, uint64_t *mu,
                       uint8_t m1[len], uint8_t m2[len]);

#endif
