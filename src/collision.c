#include "collision.h"

#include <string.h>
#include <math.h>
#include <pthread.h>
#include "dp_hash_table.h"

//----------------------------------------------------------------
// BRENTS
//----------------------------------------------------------------

void brents_cycle_find(size_t len, uint8_t const y0[len],
                       hash_function_t h,
                       uint64_t *lambda, uint64_t *mu,
                       uint8_t m1[len], uint8_t m2[len],
                       brents_callback_t power_step_callback)
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

//----------------------------------------------------------------
// DISTINGUISHED POINTS
//----------------------------------------------------------------


typedef struct
{
  const size_t hash_len;
  const unsigned int num_leading_zeros;
  const hash_function_t  h;
  const random_byte_generator_t rnd;
  const dp_callback_t point_found_callback;

  dp_hash_table_t dp_hash_table;

} TRAIL_THREAD_DATA;

void * trail_thread(void *arg)
{
  TRAIL_THREAD_DATA const *data = (TRAIL_THREAD_DATA const *) arg;

  uint8_t y0[data->hash_len];
  data->rnd(data->hash_len, y0);

  uint8_t y[data->hash_len];
  memcpy(y, y0, data->hash_len);
  bool dp_found = false;

  do
  {
    data->h(y, data->hash_len, y);

    // TODO: check if dp was found

  } while (! dp_found);

  // TODO: check if dp was already found (by a different y0)
  // and compute collision if possible (no pseudo collision)

  return NULL;
}

void dp_find_collision_parallel(size_t hash_len, hash_function_t h, random_byte_generator_t rnd,
                                unsigned int num_threads, unsigned int num_leading_zeros,
                                uint8_t m1[hash_len], uint8_t m2[hash_len],
                                dp_callback_t point_found_callback)
{ 
  TRAIL_THREAD_DATA trail_thread_data =
  {
    .hash_len = hash_len,
    .num_leading_zeros = num_leading_zeros,
    .point_found_callback = point_found_callback,
    .rnd = rnd,
    .h = h
  };

  dp_hash_table_init(&(trail_thread_data.dp_hash_table));

  pthread_t trail_threads[num_threads];
  for (size_t i = 0; i < num_threads; ++i)
  {
    pthread_create(&(trail_threads[i]), NULL, trail_thread, (void*) &trail_thread_data);
  }

  for (size_t i = 0; i < num_threads; ++i)
  {
    pthread_join(trail_threads[i], NULL);
  }
}
