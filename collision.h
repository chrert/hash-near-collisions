#ifndef COLLISION_H__
#define COLLISION_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef void (*hash_function)(uint8_t const *input, size_t input_len, uint8_t *output);
typedef bool (*brents_callback)(unsigned int power);

/**
* Performs Brent's cycle finding algorithm and returns the cycle length lambda, the tail length mu, as well as two
* colliding messages m1 and m2.
*/
bool brents_cycle_find(size_t len, uint8_t const y0[len],
        hash_function h, brents_callback power_step_callback,
        uint64_t *lambda, uint64_t *mu,
        uint8_t m1[len], uint8_t m2[len]);

#endif