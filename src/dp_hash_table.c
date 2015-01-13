#include "dp_hash_table.h"

#include <stdlib.h>

void dp_hash_table_init(dp_hash_table_t *table) {
  table->entries = NULL;
  pthread_mutex_init(&(table->mutex), NULL);
}

bool dp_hash_table_find(dp_hash_table_t *table, uint8_t const *dp,
                        size_t dp_len, dp_trail_t **dp_trail) {
  dp_hash_entry_t *entry = NULL;
  pthread_mutex_lock(&(table->mutex));
  HASH_FIND(hh, table->entries, dp, dp_len, entry);
  pthread_mutex_unlock(&(table->mutex));
  if (entry) {
    *dp_trail = &(entry->dp_trail);
    return true;
  } else {
    return false;
  }
}

dp_trail_t *dp_hash_table_add(dp_hash_table_t *table, uint8_t const *y0,
                              size_t y0_len, uint8_t const *dp, size_t dp_len,
                              uint64_t l) {
  dp_hash_entry_t *entry = (dp_hash_entry_t *)malloc(sizeof(dp_hash_entry_t));

  entry->dp_trail.y0 = (uint8_t *)malloc(y0_len);
  memcpy(entry->dp_trail.y0, y0, y0_len);

  entry->dp_trail.dp = (uint8_t *)malloc(dp_len);
  memcpy(entry->dp_trail.dp, dp, dp_len);

  entry->dp_trail.l = l;

  pthread_mutex_lock(&(table->mutex));
  HASH_ADD_KEYPTR(hh, table->entries, entry->dp_trail.dp, dp_len, entry);
  pthread_mutex_unlock(&(table->mutex));

  return &(entry->dp_trail);
}

void dp_hash_table_remove(dp_hash_table_t *table, uint8_t const *dp,
                          size_t dp_len) {
  dp_hash_entry_t *entry;
  pthread_mutex_lock(&(table->mutex));
  HASH_FIND(hh, table->entries, dp, dp_len, entry);
  HASH_DEL(table->entries, entry);
  free(entry->dp_trail.y0);
  free(entry->dp_trail.dp);
  free(entry);
  pthread_mutex_unlock(&(table->mutex));
}
