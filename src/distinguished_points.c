#ifndef DISTINGUISHED_POINTS_C
#define DISTINGUISHED_POINTS_C

#include "distinguished_points.h"

#include <string.h>
#include <pthread.h>

#include "dp_hash_table.h"
#include "bitutils.h"

typedef struct
{
  size_t const hash_len;
  unsigned int const num_leading_zeros;
  dp_hash_function_t const  h;
  dp_random_byte_generator_t const rnd;
  dp_callback_t const point_found_callback;

  dp_hash_table_t dp_hash_table;

  pthread_mutex_t possible_collision_mutex;
  pthread_cond_t possible_collision_cv;
  dp_trail_t *collision_trail_1;
  dp_trail_t *collision_trail_2;

} TRAIL_THREAD_DATA;

void * trail_thread(void *arg)
{
  TRAIL_THREAD_DATA *data = (TRAIL_THREAD_DATA *) arg;

  uint8_t y0[data->hash_len];
  uint8_t y[data->hash_len];
  uint64_t l;
  bool dp_found;

  while (true)
  {
    pthread_testcancel();

    data->rnd(data->hash_len, y0);
    memcpy(y, y0, data->hash_len);
    dp_found = false;
    l = 0;

    do
    {
      data->h(y, data->hash_len, y);
      ++l;
      // TODO: stop if too long

      uint32_t *leading32 = (uint32_t *) y;
      dp_found = count_trailing_zeros(*leading32) >= data->num_leading_zeros;

    } while (! dp_found);

    dp_trail_t *trail;
    if (! dp_hash_table_find(&(data->dp_hash_table), y, data->hash_len, &trail))
    {
      trail = dp_hash_table_add(&(data->dp_hash_table), y0, data->hash_len, y, data->hash_len, l);
      if (data->point_found_callback)
      {
        data->point_found_callback(trail, false);
      }
    }
    else
    {
      dp_trail_t *trailTmp = create_dp_trail(y0, data->hash_len, y, data->hash_len, l);

      pthread_mutex_lock(&(data->possible_collision_mutex));
      data->collision_trail_1 = trailTmp;
      data->collision_trail_2 = trail;
      pthread_cond_signal(&(data->possible_collision_cv));
      pthread_mutex_unlock(&(data->possible_collision_mutex));

      if (data->point_found_callback)
      {
        data->point_found_callback(trailTmp, true);
      }
    }
  }

  return NULL;
}

void dp_find_collision_parallel(size_t hash_len, dp_hash_function_t h, dp_random_byte_generator_t rnd,
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

  pthread_mutex_init(&(trail_thread_data.possible_collision_mutex), NULL);
  pthread_cond_init(&(trail_thread_data.possible_collision_cv), NULL);
  pthread_mutex_lock(&(trail_thread_data.possible_collision_mutex));

  pthread_t trail_threads[num_threads];
  for (size_t i = 0; i < num_threads; ++i)
  {
    pthread_create(&(trail_threads[i]), NULL, trail_thread, (void*) &trail_thread_data);
  }

  bool collision_found = false;
  while (! collision_found)
  {
    pthread_cond_wait(&(trail_thread_data.possible_collision_cv), &(trail_thread_data.possible_collision_mutex));

    uint8_t y0_longer[hash_len], y0_smaller[hash_len];
    uint64_t length_diff;
    if (trail_thread_data.collision_trail_1->l > trail_thread_data.collision_trail_2->l) {
      memcpy(y0_longer, trail_thread_data.collision_trail_1->y0, hash_len);
      memcpy(y0_smaller, trail_thread_data.collision_trail_2->y0, hash_len);
      length_diff = trail_thread_data.collision_trail_1->l - trail_thread_data.collision_trail_2->l;
    } else {
      memcpy(y0_longer, trail_thread_data.collision_trail_2->y0, hash_len);
      memcpy(y0_smaller, trail_thread_data.collision_trail_1->y0, hash_len);
      length_diff = trail_thread_data.collision_trail_2->l - trail_thread_data.collision_trail_1->l;
    }

    for (uint64_t i = 0; i < length_diff; ++i) {
      h(y0_longer, hash_len, y0_longer);
    }

    if (memcmp(y0_longer, y0_smaller, hash_len) == 0) {
      // robin hood
    } else {
      collision_found = true;
      while (memcmp(y0_longer, y0_smaller, hash_len) != 0) {
        memcpy(m1, y0_longer, hash_len);
        memcpy(m2, y0_smaller, hash_len);
        h(y0_longer, hash_len, y0_longer);
        h(y0_smaller, hash_len, y0_smaller);
      }
    }
  }
  pthread_mutex_unlock(&(trail_thread_data.possible_collision_mutex));

  for (size_t i = 0; i < num_threads; ++i)
  {
    pthread_cancel(trail_threads[i]);
  }

  for (size_t i = 0; i < num_threads; ++i)
  {
    pthread_join(trail_threads[i], NULL);
  }

  return;
}

dp_trail_t * create_dp_trail(uint8_t const *y0, size_t y0_len, uint8_t const *dp, size_t dp_len, uint64_t l)
{
  dp_trail_t *trail = (dp_trail_t *) malloc(sizeof(dp_trail_t));
  trail->y0 = (uint8_t *) malloc(y0_len);
  memcpy(trail->y0, y0, y0_len);
  trail->dp = (uint8_t *) malloc(dp_len);
  memcpy(trail->dp, dp, dp_len);
  trail->l = l;
  return trail;
}

void destroy_dp_trail(dp_trail_t *trail)
{
  free(trail->y0);
  free(trail->dp);
  free(trail);
}

#endif // DISTINGUISHED_POINTS_C

