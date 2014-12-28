#ifndef DP_HASH_TABLE_H
#define DP_HASH_TABLE_H

#include "distinguished_points.h"
#include "uthash.h"

#include <stddef.h>
#include <pthread.h>

/**
 * @brief A single entry of the hash table
 */
typedef struct
{
  dp_trail_t dp_trail;
  UT_hash_handle hh;

} dp_hash_entry_t;

/**
 * @brief The hashtable
 */
typedef struct
{
  dp_hash_entry_t *entries;
  pthread_mutex_t mutex;

} dp_hash_table_t;


/**
 * @brief Initialize a hash table
 * @param table
 */
void dp_hash_table_init(dp_hash_table_t *table);

/**
 * @brief Find an entry in the hash table
 * @param table The hash table
 * @param dp The key of the hash table
 * @param dp_len The length of the key
 * @param dp_trail This pointer will be set to the pointer of the found entry (iff the key is in the table)
 * @return true if the key was found, otherwise false
 */
bool dp_hash_table_find(dp_hash_table_t *table, uint8_t const *dp, size_t dp_len, dp_trail_t **dp_trail);

/**
 * @brief dp_hash_table_add
 * @param table
 * @param y0
 * @param y0_len
 * @param dp
 * @param dp_len
 * @param l
 * @return
 */
dp_trail_t *dp_hash_table_add(dp_hash_table_t *table, uint8_t const *y0, size_t y0_len, uint8_t const *dp, size_t dp_len, uint64_t l);

/**
 * @brief dp_hash_table_remove
 * @param table
 * @param dp
 * @param dp_len
 */
void dp_hash_table_remove(dp_hash_table_t *table, uint8_t const *dp, size_t dp_len);

#endif // DP_HASH_TABLE_H
