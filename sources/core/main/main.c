#include <stdio.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash.h"

int main()
{
  hash_table_t *hash_table = NULL;

  int hash_malloc_result = hash_malloc(&hash_table, 2);
  check(hash_malloc_result == 0, "hash_malloc_result: %d",
    hash_malloc_result);

  int hash_set_result;

  hash_set_result = hash_set(hash_table, 1, (void *)1);
  check(hash_set_result == 0, "hash_set_result: %d",
    hash_set_result);

  hash_set_result = hash_set(hash_table, 1, (void *)4);
  check(hash_set_result == 0, "hash_set_result: %d",
    hash_set_result);

  hash_set_result = hash_set(hash_table, 2, (void *)2);
  check(hash_set_result == 0, "hash_set_result: %d",
    hash_set_result);

  int value;

  int hash_get_result = hash_get(hash_table, 1, (void **)&value);
  check(hash_get_result == 0, "hash_get_result: %d",
    hash_get_result);

  printf("%d\n", value);

  hash_free(hash_table);

  return 0;

error:

  if (hash_table != NULL) { hash_free(hash_table); }

  return -1;
}
