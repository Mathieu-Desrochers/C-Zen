#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define _GNU_SOURCE

#include <search.h>

#include "../../infrastructure/hash/hash_values.h"

typedef struct hsearch_data hsearch_data;

// represents a hash table
typedef struct hash_table_t
{
  hsearch_data *hsearch;
  char **keys;
  int keys_allocated_count;
  int keys_used_count;
  hash_values_t **hash_values;
  int hash_values_allocated_count;
  int hash_values_used_count;
} hash_table_t;

// allocates a hash table
int hash_table_malloc(hash_table_t **hash_table, int maximum_distinct_keys_count);

// adds a value for a given key
int hash_table_add_int_int(hash_table_t *hash_table, int key, int value);
int hash_table_add_int_pointer(hash_table_t *hash_table, int key, void *value);

// gets the values for a given key
int hash_table_get_int_int(hash_table_t *hash_table, int key, int **values, int *values_count);
int hash_table_get_int_pointer(hash_table_t *hash_table, int key, void ***values, int *values_count);

// frees a hash table
void hash_table_free(hash_table_t *hash_table);

#endif
