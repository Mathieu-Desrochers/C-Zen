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
  string_duplicate(NAME_SINGLE_LOWER()_row->name, name);

  return NAME_SINGLE_LOWER()_row;

error:

  if (NAME_SINGLE_LOWER()_row != NULL) { NAME_SINGLE_LOWER()_row_free(NAME_SINGLE_LOWER()_row); }

  return NULL;
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
