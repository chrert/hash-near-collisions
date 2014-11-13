#include "collision.h"

#include <string.h>
#include <math.h>

void brents_cycle_find(size_t len, uint8_t const y0[len],
                       brents_function h, brents_callback power_step_callback,
                       uint64_t *lambda, uint64_t *mu,
                       uint8_t m1[len], uint8_t m2[len])
{
  uint8_t tortoise[len];
  memcpy(tortoise, y0, len);

  uint8_t hare[len];
  h(y0, len, hare);

  uint64_t power = 1;
  *lambda = 1;

  // find the length of the cycle (lambda)
  while (memcmp(hare, tortoise, len) != 0)
  {
    if (power == *lambda)
    {
      if (power_step_callback)
      {
        power_step_callback((unsigned int) log2(power));
      }

      memcpy(tortoise, hare, len);
      power *= 2;
      *lambda = 0;
    }

    h(hare, len, hare);
    ++(*lambda);
  }

  // the hare has lambda steps lead
  memcpy(hare, y0, len);
  for (int i = 0; i < *lambda; ++i) {
    h(hare, len, hare);
  }

  // now both walk at the same speed, so they should meet at the collision point!
  *mu = 0;
  memcpy(tortoise, y0, len);
  while (memcmp(tortoise, hare, len) != 0)
  {
    memcpy(m1, hare, len);
    memcpy(m2, tortoise, len);
    h(hare, len, hare);
    h(tortoise, len, tortoise);
    ++(*mu);
  }

}
