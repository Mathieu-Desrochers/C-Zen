#ifndef HASH_VALUES_H
#define HASH_VALUES_H

// represents hashed values
typedef struct hash_values_t
{
  void *values;
  int allocated_count;
  int used_count;
} hash_values_t;

// allocates hashed values
int hash_values_malloc(hash_values_t **hash_values);

// frees hashed values
void hash_values_free(hash_values_t *hash_values);

#endif

