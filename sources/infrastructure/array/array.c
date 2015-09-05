#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_table.h"

// adds an element to an array
int array_add_int(int **array, int *allocated_count, int *used_count, int value)
{
  int *reallocated_array = NULL;

  check(allocated_count != NULL, "allocated_count: NULL");
  check(used_count != NULL, "used_count: NULL");

  if (*allocated_count == *used_count)
  {
    int reallocated_count = (*allocated_count) == 0 ? 4 : ((*allocated_count) * 2);
    reallocated_array = realloc(*array, sizeof(int) * reallocated_count);
    check_mem(reallocated_array);
    *array = reallocated_array;
    *allocated_count = reallocated_count;
  }

  (*array)[*used_count] = value;
  (*used_count)++;

  return 0;

error:

  return -1;
}

// adds an element to an array
int array_add_pointer(void ***array, int *allocated_count, int *used_count, void *value)
{
  void **reallocated_array = NULL;

  check(allocated_count != NULL, "allocated_count: NULL");
  check(used_count != NULL, "used_count: NULL");

  if (*allocated_count == *used_count)
  {
    int reallocated_count = (*allocated_count) == 0 ? 4 : ((*allocated_count) * 2);
    reallocated_array = realloc(*array, sizeof(void *) * reallocated_count);
    check_mem(reallocated_array);
    *array = reallocated_array;
    *allocated_count = reallocated_count;
  }

  (*array)[*used_count] = value;
  (*used_count)++;

  return 0;

error:

  return -1;
}

// adds an element to an array
int array_add_string(char ***array, int *allocated_count, int *used_count, char *value)
{
  char **reallocated_array = NULL;

  check(allocated_count != NULL, "allocated_count: NULL");
  check(used_count != NULL, "used_count: NULL");

  if (*allocated_count == *used_count)
  {
    int reallocated_count = (*allocated_count) == 0 ? 4 : ((*allocated_count) * 2);
    reallocated_array = realloc(*array, sizeof(char *) * reallocated_count);
    check_mem(reallocated_array);
    *array = reallocated_array;
    *allocated_count = reallocated_count;
  }

  (*array)[*used_count] = value;
  (*used_count)++;

  return 0;

error:

  return -1;
}

// finds duplicates within an array
int array_find_duplicates_int(int **array, int array_count, int **duplicate_indexes, int *duplicate_indexes_count)
{
  int *duplicate_indexes_return = NULL;
  int duplicate_indexes_allocated_count = 0;
  int duplicate_indexes_used_count = 0;

  hash_table_t *hash_table = NULL;

  check(array != NULL, "array: NULL");
  check(duplicate_indexes != NULL, "duplicate_indexes: NULL");
  check(duplicate_indexes_count != NULL, "duplicate_indexes_count: NULL");

  hash_table = hash_table_malloc(array_count);
  check(hash_table != NULL, "hash_table: NULL");

  for (int i = 0; i < array_count; i++)
  {
    if (array[i] == NULL)
    {
      continue;
    }

    int hash_table_add_result = hash_table_add_int_int(hash_table, *(array[i]), i);
    check(hash_table_add_result == 0, "hash_table_add_result: %d",
      hash_table_add_result);
  }

  int *values = NULL;
  int values_count = 0;

  for (int i = 0; i < array_count; i++)
  {
    if (array[i] == NULL)
    {
      continue;
    }

    int hash_table_get_result = hash_table_get_int_int(hash_table, *(array[i]), &values, &values_count);
    check(hash_table_get_result == 0, "hash_table_get_result: %d",
      hash_table_get_result);

    if (values_count > 1)
    {
      int array_add_result = array_add_int(
        &duplicate_indexes_return,
        &duplicate_indexes_allocated_count,
        &duplicate_indexes_used_count,
        i);

      check(array_add_result == 0, "array_add_result: %d",
        array_add_result);
    }
  }

  hash_table_free(hash_table);

  *duplicate_indexes = duplicate_indexes_return;
  *duplicate_indexes_count = duplicate_indexes_used_count;

  return 0;

error:

  if (duplicate_indexes_return != NULL) { free(duplicate_indexes_return); }
  if (hash_table != NULL) { hash_table_free(hash_table); }

  return -1;
}

// finds unknown values within an array
int array_find_unknowns_int(int **array, int array_count, int** known_array, int known_array_count, int **unknown_indexes, int *unknown_indexes_count)
{
  int *unknown_indexes_return = NULL;
  int unknown_indexes_allocated_count = 0;
  int unknown_indexes_used_count = 0;

  hash_table_t *hash_table = NULL;

  check(array != NULL, "array: NULL");
  check(known_array != NULL, "known_array: NULL");
  check(unknown_indexes != NULL, "unknown_indexes: NULL");
  check(unknown_indexes_count != NULL, "unknown_indexes_count: NULL");

  hash_table = hash_table_malloc(known_array_count);
  check(hash_table != NULL, "hash_table: NULL");

  for (int i = 0; i < known_array_count; i++)
  {
    if (known_array[i] == NULL)
    {
      continue;
    }

    int hash_table_add_result = hash_table_add_int_int(hash_table, *(known_array[i]), i);
    check(hash_table_add_result == 0, "hash_table_add_result: %d",
      hash_table_add_result);
  }

  int *values = NULL;
  int values_count = 0;

  for (int i = 0; i < array_count; i++)
  {
    if (array[i] == NULL)
    {
      continue;
    }

    int hash_table_get_result = hash_table_get_int_int(hash_table, *(array[i]), &values, &values_count);
    check(hash_table_get_result == 0, "hash_table_get_result: %d",
      hash_table_get_result);

    if (values_count == 0)
    {
      int array_add_result = array_add_int(
        &unknown_indexes_return,
        &unknown_indexes_allocated_count,
        &unknown_indexes_used_count,
        i);

      check(array_add_result == 0, "array_add_result: %d",
        array_add_result);
    }
  }

  hash_table_free(hash_table);

  *unknown_indexes = unknown_indexes_return;
  *unknown_indexes_count = unknown_indexes_used_count;

  return 0;

error:

  if (unknown_indexes_return != NULL) { free(unknown_indexes_return); }
  if (hash_table != NULL) { hash_table_free(hash_table); }

  return -1;
}

// frees an array
void array_free_string(char **array, int count)
{
  if (array == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    free(array[i]);
  }

  free(array);
}
