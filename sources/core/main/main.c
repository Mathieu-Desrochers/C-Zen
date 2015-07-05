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

  hash_set_result = hash_set_int_int(hash_table, 1, 1);
  check(hash_set_result == 0, "hash_set_int_int_result: %d",
    hash_set_result);

  hash_set_result = hash_set_int_int(hash_table, 1, 4);
  check(hash_set_result == 0, "hash_set_int_int_result: %d",
    hash_set_result);

  hash_set_result = hash_set_int_int(hash_table, 2, 2);
  check(hash_set_result == 0, "hash_set_int_int_result: %d",
    hash_set_result);

  int value = 0;

  int hash_get_result = hash_get_int_int(hash_table, 1, &value);
  check(hash_get_result == 0, "hash_get_int_int_result: %d",
    hash_get_result);

  printf("%d\n", value);

  hash_free(hash_table);

  return 0;

error:

  if (hash_table != NULL) { hash_free(hash_table); }

  return -1;
}
