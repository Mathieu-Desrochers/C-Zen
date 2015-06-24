#ifndef HASH_H
#define HASH_H

#define _GNU_SOURCE

#include <search.h>

typedef struct hsearch_data hsearch_data;

// represents a hash table
typedef struct hash_table_t
{
  hsearch_data *hsearch;
  char **keys;
  int allocated_keys_count;
  int used_keys_count;
} hash_table_t;

// allocates a hash table
int hash_malloc(hash_table_t **hash_table, int maximum_keys_count);

// sets the value for a given key
int hash_set(hash_table_t *hash_table, int key, void *value);

// gets the value for a given key
int hash_get(hash_table_t *hash_table, int key, void **value);

// frees a hash table
void hash_free(hash_table_t *hash_table);

#endif
