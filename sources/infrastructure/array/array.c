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
int array_find_duplicates_int(int *array, int array_count, int **duplicate_indexes, int *duplicate_indexes_count)
{
//  int **duplicate_indexes_return = NULL;
//  int duplicate_indexes_allocated_count = 0;
//  int duplicate_indexes_used_count = 0;
//
//  check(array != NULL, "array: NULL");
//  check(duplicate_indexes != NULL, "duplicate_indexes: NULL");
//  check(duplicate_indexes_count != NULL, "duplicate_indexes_count: NULL");
//
//  hash_table_t *hash_table = NULL;
//
//  int hash_malloc_result = hash_malloc(&hash_table, count);
//  check(hash_malloc_result == 0, "hash_malloc_result: %d",
//    hash_malloc_result);
//
//  0 1 2 3 4
//  a b a c a
//
//
//
//  for (int i = 0; i < array_count; i++)
//  {
//    int hash_get_int_int_result = hash_get_int_int(hash_table, array[i], &last_seen_index);
//    check(hash_get_int_int_result == 0 || hash_get_int_int_result == 1, "hash_get_int_int_result: %d",
//      hash_get_int_int_result);
//
//    if (hash_get_int_int_result == 0)
//    {
//      int hash_set_result = hash_set_int_int(hash_table, array[i], i);
//      check(hash_set_result == 0, "hash_set_result: %d",
//        hash_set_result);
//    }
//    else
//    {
//      int array_add_result = array_add_int(
//        &duplicate_indexes_return,
//        &duplicate_indexes_allocated_count,
//        &duplicate_indexes_used_count,
//        i);
//
//      check(array_add_result == 0, "array_add_result: %d",
//        array_add_result);
//    }
//  }
//
//  return 0;
//
//error:
//
//  if (hash_table != NULL) { hash_free(hash_table); }
//
  return -1;
}
