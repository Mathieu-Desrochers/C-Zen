`#'ifndef NAME_SINGLE_UPPER()_ROW_H
`#'define NAME_SINGLE_UPPER()_ROW_H

// represents a NAME_SINGLE_LOWER() row
typedef struct NAME_SINGLE_LOWER()_row_t
{
  int NAME_SINGLE_LOWER()_id;
  char *name;
} NAME_SINGLE_LOWER()_row_t;

// allocates a NAME_SINGLE_LOWER() row
NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row_malloc(
  int NAME_SINGLE_LOWER()_id,
  char *name);

// frees a NAME_SINGLE_LOWER() row
void NAME_SINGLE_LOWER()_row_free(NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row);

// frees an array of NAME_SINGLE_LOWER() rows
void NAME_SINGLE_LOWER()_rows_free(NAME_SINGLE_LOWER()_row_t **NAME_SINGLE_LOWER()_rows, int count);

#endif
