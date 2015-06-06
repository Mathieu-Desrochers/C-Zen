`#'include <stddef.h>
`#'include <stdlib.h>
`#'include <string.h>

`#'include "../../infrastructure/dbg/dbg.h"
`#'include "../../infrastructure/string/string.h"
`#'include "../../core/tables/NAME_SINGLE_LOWER()_row.h"

// allocates a NAME_SINGLE_LOWER() row
NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row_malloc(
  int NAME_SINGLE_LOWER()_id,
  char *name)
{
  NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row = malloc(sizeof(NAME_SINGLE_LOWER()_row_t));
  check_mem(NAME_SINGLE_LOWER()_row);

  NAME_SINGLE_LOWER()_row->NAME_SINGLE_LOWER()_id = NAME_SINGLE_LOWER()_id;

  NAME_SINGLE_LOWER()_row->name = strdup(name);
  check_mem(NAME_SINGLE_LOWER()_row->name);

  return NAME_SINGLE_LOWER()_row;

error:

  if (NAME_SINGLE_LOWER()_row != NULL) { NAME_SINGLE_LOWER()_row_free(NAME_SINGLE_LOWER()_row); }

  return NULL;
}

// adds a NAME_SINGLE_LOWER() row to an array
int NAME_SINGLE_LOWER()_rows_add(NAME_SINGLE_LOWER()_row_t ***NAME_SINGLE_LOWER()_rows, int *count, int *used, NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row)
{
  NAME_SINGLE_LOWER()_row_t **reallocated_`'NAME_SINGLE_LOWER()_rows = NULL;

  if (*count == *used)
  {
    int new_count = (*count) == 0 ? 4 : ((*count) * 2);
    reallocated_`'NAME_SINGLE_LOWER()_rows = realloc(*NAME_SINGLE_LOWER()_rows, sizeof(NAME_SINGLE_LOWER()_row_t *) * new_count);
    check_mem(reallocated_`'NAME_SINGLE_LOWER()_rows);
    *NAME_SINGLE_LOWER()_rows = reallocated_`'NAME_SINGLE_LOWER()_rows;
    *count = new_count;
  }

  (*NAME_SINGLE_LOWER()_rows)[*used] = NAME_SINGLE_LOWER()_row;
  (*used)++;

  return 0;

error:

  return -1;
}

// frees a NAME_SINGLE_LOWER() row
void NAME_SINGLE_LOWER()_row_free(NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row)
{
  if (NAME_SINGLE_LOWER()_row == NULL)
  {
    return;
  }

  if (NAME_SINGLE_LOWER()_row->name != NULL) { free(NAME_SINGLE_LOWER()_row->name); }

  free(NAME_SINGLE_LOWER()_row);
}

// frees an array of NAME_SINGLE_LOWER() rows
void NAME_SINGLE_LOWER()_rows_free(NAME_SINGLE_LOWER()_row_t **NAME_SINGLE_LOWER()_rows, int count)
{
  if (NAME_SINGLE_LOWER()_rows == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    NAME_SINGLE_LOWER()_row_free(NAME_SINGLE_LOWER()_rows[i]);
  }

  free(NAME_SINGLE_LOWER()_rows);
}
