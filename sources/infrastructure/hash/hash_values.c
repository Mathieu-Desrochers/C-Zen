#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_values.h"

// allocates hashed values
hash_values_t *hash_values_malloc()
{
  hash_values_t *hash_values = NULL;

  hash_values = malloc(sizeof(hash_values_t));
  check_mem(hash_values);

  hash_values->values = NULL;
  hash_values->allocated_count = 0;
  hash_values->used_count = 0;

  return hash_values;

error:

  if (hash_values != NULL) { hash_values_free(hash_values); }

  return NULL;
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
