#include <stdio.h>
#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"

int main()
{
  int *values = calloc(10000, sizeof(int));

  int *duplicate_indexes = NULL;
  int duplicate_indexes_count = 0;

  int array_find_duplicates_result = array_find_duplicates_int(
    values,
    10000,
    &duplicate_indexes,
    &duplicate_indexes_count);

  check(array_find_duplicates_result == 0, "array_find_duplicates_result: %d",
    array_find_duplicates_result);

  printf("%d\n", duplicate_indexes_count);

  free(duplicate_indexes);
  free(values);

  return 0;

error:

  if (duplicate_indexes != NULL) { free(duplicate_indexes); }
  if (values != NULL) { free(values); }

  return -1;
}
