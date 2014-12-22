#ifndef DP_HASH_TABLE_H
#define DP_HASH_TABLE_H

#include "collision.h"
#include "uthash.h"

#include <stddef.h>
#include <pthread.h>

typedef struct
{
  dp_trail_t dp_trail;
  UT_hash_handle hh;

} dp_hash_entry_t;

typedef struct
{
  dp_hash_entry_t *entries;
  pthread_mutex_t mutex;

} dp_hash_table_t;


void dp_hash_table_init(dp_hash_table_t *table);

void dp_hash_table_find(dp_hash_table_t *table, uint8_t const *dp, size_t dp_len, dp_trail_t **dp_trail);

void dp_hash_table_add(dp_hash_table_t *table, uint8_t const *y0, size_t y0_len, uint8_t const *dp, size_t dp_len, uint64_t l);

void dp_hash_table_remove(dp_hash_table_t *table, uint8_t const *dp, size_t dp_len);

#endif // DP_HASH_TABLE_H
