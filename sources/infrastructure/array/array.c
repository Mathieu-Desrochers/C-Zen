#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_table.h"

// adds an element to an array
int array_add_int(int **array, int *allocated_count, int *used_count, int value)
{
  int exit_code = 0;

  check_not_null(allocated_count);
  check_not_null(used_count);

  if (*allocated_count == *used_count)
  {
    int reallocated_count = (*allocated_count) == 0 ? 4 : ((*allocated_count) * 2);
    int *reallocated_array = realloc(*array, sizeof(int) * reallocated_count);
    check_mem(reallocated_array);

    *array = reallocated_array;
    *allocated_count = reallocated_count;
  }

  (*array)[*used_count] = value;
  (*used_count)++;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// adds an element to an array
int array_add_pointer(void ***array, int *allocated_count, int *used_count, void *value)
{
  int exit_code = 0;

  check_not_null(allocated_count);
  check_not_null(used_count);

  if (*allocated_count == *used_count)
  {
    int reallocated_count = (*allocated_count) == 0 ? 4 : ((*allocated_count) * 2);
    void **reallocated_array = realloc(*array, sizeof(void *) * reallocated_count);
    check_mem(reallocated_array);

    *array = reallocated_array;
    *allocated_count = reallocated_count;
  }

  (*array)[*used_count] = value;
  (*used_count)++;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// adds an element to an array
int array_add_string(char ***array, int *allocated_count, int *used_count, char *value)
{
  int exit_code = 0;

  check_not_null(allocated_count);
  check_not_null(used_count);

  if (*allocated_count == *used_count)
  {
    int reallocated_count = (*allocated_count) == 0 ? 4 : ((*allocated_count) * 2);
    char **reallocated_array = realloc(*array, sizeof(char *) * reallocated_count);
    check_mem(reallocated_array);

    *array = reallocated_array;
    *allocated_count = reallocated_count;
  }

  (*array)[*used_count] = value;
  (*used_count)++;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// finds duplicates within an array
int array_find_duplicates_int(int **array, int array_count, int **duplicate_indexes, int *duplicate_indexes_count)
{
  int *duplicate_indexes_return = NULL;

  int exit_code = 0;

  hash_table_t *hash_table = NULL;

  check_not_null(array);
  check_not_null(duplicate_indexes);
  check_not_null(duplicate_indexes_count);

  hash_table = hash_table_malloc(array_count);
  check_not_null(hash_table);

  for (int i = 0; i < array_count; i++)
  {
    if (array[i] == NULL)
    {
      continue;
    }

    check_result(hash_table_add_int_int(hash_table, *(array[i]), i), 0);
  }

  int duplicate_indexes_allocated_count = 0;
  int duplicate_indexes_used_count = 0;

  for (int i = 0; i < array_count; i++)
  {
    if (array[i] == NULL)
    {
      continue;
    }

    int *values = NULL;
    int values_count = 0;

    check_result(hash_table_get_int_int(hash_table, *(array[i]), &values, &values_count), 0);

    if (values_count > 1)
    {
      check_result(
        array_add_int(
          &duplicate_indexes_return,
          &duplicate_indexes_allocated_count,
          &duplicate_indexes_used_count,
          i),
        0);
    }
  }

  *duplicate_indexes = duplicate_indexes_return;
  *duplicate_indexes_count = duplicate_indexes_used_count;

  goto cleanup;

error:

  if (duplicate_indexes_return != NULL) { free(duplicate_indexes_return); }

  exit_code = -1;

cleanup:

  if (hash_table != NULL) { hash_table_free(hash_table); }

  return exit_code;
}

// finds unknown values within an array
int array_find_unknowns_int(int **array, int array_count, int **known_array, int known_array_count, int **unknown_indexes, int *unknown_indexes_count)
{
  int *unknown_indexes_return = NULL;

  int exit_code = 0;

  hash_table_t *hash_table = NULL;

  check_not_null(array);
  check_not_null(known_array);
  check_not_null(unknown_indexes);
  check_not_null(unknown_indexes_count);

  hash_table = hash_table_malloc(known_array_count);
  check_not_null(hash_table);

  for (int i = 0; i < known_array_count; i++)
  {
    if (known_array[i] == NULL)
    {
      continue;
    }

    check_result(hash_table_add_int_int(hash_table, *(known_array[i]), i), 0);
  }

  int unknown_indexes_allocated_count = 0;
  int unknown_indexes_used_count = 0;

  for (int i = 0; i < array_count; i++)
  {
    if (array[i] == NULL)
    {
      continue;
    }

    int *values = NULL;
    int values_count = 0;

    check_result(hash_table_get_int_int(hash_table, *(array[i]), &values, &values_count), 0);

    if (values_count == 0)
    {
      check_result(
        array_add_int(
          &unknown_indexes_return,
          &unknown_indexes_allocated_count,
          &unknown_indexes_used_count,
          i),
        0);
    }
  }

  *unknown_indexes = unknown_indexes_return;
  *unknown_indexes_count = unknown_indexes_used_count;

  goto cleanup;

error:

  if (unknown_indexes_return != NULL) { free(unknown_indexes_return); }

  exit_code = -1;

cleanup:

  if (hash_table != NULL) { hash_table_free(hash_table); }

  return exit_code;
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
