#include <stdio.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_table.h"

int main()
{
  hash_table_t *hash_table = NULL;

  int *values = NULL;
  int values_count = 0;

  int hash_table_malloc_result = hash_table_malloc(&hash_table, 2);
  check(hash_table_malloc_result == 0, "hash_table_malloc_result: %d",
    hash_table_malloc_result);

  int hash_table_add_result;

  hash_table_add_result = hash_table_add_int_int(hash_table, 1, 1);
  check(hash_table_add_result == 0, "hash_table_add_int_int_result: %d",
    hash_table_add_result);

  hash_table_add_result = hash_table_add_int_int(hash_table, 1, 4);
  check(hash_table_add_result == 0, "hash_table_add_int_int_result: %d",
    hash_table_add_result);

  hash_table_add_result = hash_table_add_int_int(hash_table, 2, 2);
  check(hash_table_add_result == 0, "hash_table_add_int_int_result: %d",
    hash_table_add_result);

  int hash_table_get_result = hash_table_get_int_int(hash_table, 1, &values, &values_count);
  check(hash_table_get_result == 0, "hash_table_get_int_int_result: %d",
    hash_table_get_result);

  printf("%d\n---\n", values_count);

  for (int i = 0; i < values_count; i++)
  {
    printf("%d\n", values[i]);
  }

  hash_table_free(hash_table);

  return 0;

error:

  if (hash_table != NULL) { hash_table_free(hash_table); }

  return -1;
}
