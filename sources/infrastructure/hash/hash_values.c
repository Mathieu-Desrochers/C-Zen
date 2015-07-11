#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_values.h"

// allocates hashed values
int hash_values_malloc(hash_values_t **hash_values)
{
  hash_values_t *hash_values_return = NULL;

  check(hash_values != NULL, "hash_values: NULL");

  hash_values_return = malloc(sizeof(hash_values_t));
  check_mem(hash_values_return);

  hash_values_return->values = NULL;
  hash_values_return->allocated_count = 0;
  hash_values_return->used_count = 0;

  *hash_values = hash_values_return;

  return 0;

error:

  if (hash_values_return != NULL) { hash_values_free(hash_values_return); }

  return -1;
}

// frees hashed values
void hash_values_free(hash_values_t *hash_values)
{
  if (hash_values == NULL)
  {
    return;
  }

  free(hash_values->values);
  free(hash_values);
}
