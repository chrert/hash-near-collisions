#include "collision.h"

#include <string.h>
#include <math.h>
#include <pthread.h>
#include <caca_conio.h>
#include "dp_hash_table.h"
#include "bitutils.h"

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
  TRAIL_THREAD_DATA *data = (TRAIL_THREAD_DATA *) arg;

  uint8_t y0[data->hash_len];
  search:
  data->rnd(data->hash_len, y0);

  uint8_t y[data->hash_len];
  memcpy(y, y0, data->hash_len);
  bool dp_found = false;
  uint64_t l = 0;

  do
  {
    data->h(y, data->hash_len, y);
    ++l;

    // TODO: stop if too long

    dp_found = true;
    for (unsigned int i = 0; dp_found && i < data->num_leading_zeros; ++i) {
      unsigned int byte_i = i / 8;
      unsigned int bit_i = i % 8;
      if (get_bit(y[byte_i], 7-bit_i)) {
        dp_found = false;
      }
    }

  } while (! dp_found);

  dp_trail_t *trail;
  if (! dp_hash_table_find(&(data->dp_hash_table), y, data->hash_len, &trail)) {
    trail = dp_hash_table_add(&(data->dp_hash_table), y0, data->hash_len, y, data->hash_len, l);
    if (data->point_found_callback) {
      data->point_found_callback(trail, false);
    }
  } else {
    if (data->point_found_callback) {
      dp_trail_t *trailTmp = (dp_trail_t *) malloc(sizeof(dp_trail_t));
      trailTmp->y0 = (uint8_t *) malloc(data->hash_len);
      memcpy(trailTmp->y0, y0, data->hash_len);
      trailTmp->dp = (uint8_t *) malloc(data->hash_len);
      memcpy(trailTmp->dp, y, data->hash_len);
      trailTmp->l = l;
      data->point_found_callback(trailTmp, true);
      free(trailTmp);
    }

    uint8_t y0_longer[data->hash_len], y0_smaller[data->hash_len];
    uint64_t length_diff;
    if (trail->l > l) {
      memcpy(y0_longer, trail->y0, data->hash_len);
      memcpy(y0_smaller, y0, data->hash_len);
      length_diff = trail->l - l;
    } else {
      memcpy(y0_longer, y0, data->hash_len);
      memcpy(y0_smaller, trail->y0, data->hash_len);
      length_diff = l - trail->l;
    }

    for (uint64_t i = 0; i < length_diff; ++i) {
      data->h(y0_longer, data->hash_len, y0_longer);
    }

    if (memcmp(y0_longer, y0_smaller, data->hash_len) == 0) {
      // robin hood
    } else {
      while (memcmp(y0_longer, y0_smaller, data->hash_len) != 0) {
        data->h(y0_longer, data->hash_len, y0_longer);
        data->h(y0_smaller, data->hash_len, y0_smaller);
      }
    }
  }

  goto search;

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
