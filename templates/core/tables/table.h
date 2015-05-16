`#'ifndef NAME_PLURAL_UPPER()_TABLE_H
`#'define NAME_PLURAL_UPPER()_TABLE_H

`#'include <sqlite3.h>

`#'include "../../core/tables/NAME_SINGLE_LOWER()_row.h"

// inserts a NAME_SINGLE_LOWER() row
int NAME_PLURAL_LOWER()_table_insert(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row);

// selects a NAME_SINGLE_LOWER() row by NAME_SINGLE_LOWER() id
int NAME_PLURAL_LOWER()_table_select_by_`'NAME_SINGLE_LOWER()_id(sqlite3 *sql_connection, int NAME_SINGLE_LOWER()_id, NAME_SINGLE_LOWER()_row_t **NAME_SINGLE_LOWER()_row);

// selects all the NAME_SINGLE_LOWER() rows
int NAME_PLURAL_LOWER()_table_select_all(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t ***NAME_SINGLE_LOWER()_row, int *count);

// updates a NAME_SINGLE_LOWER() row
int NAME_PLURAL_LOWER()_table_update(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row);

// deletes a NAME_SINGLE_LOWER() row
int NAME_PLURAL_LOWER()_table_delete(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row);

`#'endif
