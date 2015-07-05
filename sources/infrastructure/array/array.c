#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"

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
