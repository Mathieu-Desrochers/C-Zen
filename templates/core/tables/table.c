`#'include <stddef.h>
`#'include <stdlib.h>
`#'include <sqlite3.h>

`#'include "../../infrastructure/dbg/dbg.h"
`#'include "../../infrastructure/sql/sql.h"
`#'include "../../core/tables/NAME_SINGLE_LOWER()_row.h"
`#'include "../../core/tables/NAME_PLURAL_LOWER()_table.h"

// reads a NAME_SINGLE_LOWER() row
int NAME_PLURAL_LOWER()_table_read(sqlite3_stmt *sql_statement, NAME_SINGLE_LOWER()_row_t **NAME_SINGLE_LOWER()_row)
{
  char *name = NULL;

  NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(NAME_SINGLE_LOWER()_row != NULL, "NAME_SINGLE_LOWER()_row: NULL");

  int NAME_SINGLE_LOWER()_id;
  int sql_read_`'NAME_SINGLE_LOWER()_id_result = sql_read_int(sql_statement, 0, &NAME_SINGLE_LOWER()_id);
  check(sql_read_`'NAME_SINGLE_LOWER()_id_result == 0, "sql_read_`'NAME_SINGLE_LOWER()_id_result: %d",
    sql_read_`'NAME_SINGLE_LOWER()_id_result);

  int sql_read_name_result = sql_read_string(sql_statement, 1, &name);
  check(sql_read_name_result == 0, "sql_read_name_result: %d",
    sql_read_name_result);

  NAME_SINGLE_LOWER()_row_return = NAME_SINGLE_LOWER()_row_malloc(
    NAME_SINGLE_LOWER()_id,
    name);

  check(NAME_SINGLE_LOWER()_row_return != NULL, "NAME_SINGLE_LOWER()_row_return: NULL");

  free(name);

  *NAME_SINGLE_LOWER()_row = NAME_SINGLE_LOWER()_row_return;

  return 0;

error:

  if (name != NULL) { free(name); }

  if (NAME_SINGLE_LOWER()_row_return != NULL) { NAME_SINGLE_LOWER()_row_free(NAME_SINGLE_LOWER()_row_return); }

  return -1;
}

// inserts a NAME_SINGLE_LOWER() row
int NAME_PLURAL_LOWER()_table_insert(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row)
{
  sqlite3_stmt *sql_insert_statement = NULL;
  sqlite3_stmt *sql_select_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(NAME_SINGLE_LOWER()_row != NULL, "NAME_SINGLE_LOWER()_row: NULL");

  int sql_prepare_insert_statement_result = sql_prepare_statement(
    sql_connection,
    "INSERT INTO \"NAME_PLURAL_LOWER()\" ("
      "\"name\") "
    "VALUES (?1);",
    &sql_insert_statement);

  check(sql_prepare_insert_statement_result == 0, "sql_prepare_insert_statement_result: %d",
    sql_prepare_insert_statement_result);

  int sql_bind_name_result = sql_bind_string(sql_insert_statement, 1, NAME_SINGLE_LOWER()_row->name);
  check(sql_bind_name_result == 0, "sql_bind_name_result: %d",
    sql_bind_name_result);

  int sql_step_execute_result = sql_step_execute(sql_insert_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  int sql_prepare_select_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT last_insert_rowid();",
    &sql_select_statement);

  check(sql_prepare_select_statement_result == 0, "sql_prepare_select_statement_result: %d",
    sql_prepare_select_statement_result);

  int row_available = 0;
  int sql_step_select_result = sql_step_select(sql_select_statement, &row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  check(row_available == 1, "row_available: %d", row_available);

  NAME_SINGLE_LOWER()_row->NAME_SINGLE_LOWER()_id = sqlite3_column_int(sql_select_statement, 0);

  sql_finalize_statement(sql_insert_statement);
  sql_finalize_statement(sql_select_statement);

  return 0;

error:

  if (sql_insert_statement != NULL) { sql_finalize_statement(sql_insert_statement); }
  if (sql_select_statement != NULL) { sql_finalize_statement(sql_select_statement); }

  return -1;
}

// selects a NAME_SINGLE_LOWER() row by NAME_SINGLE_LOWER() id
int NAME_PLURAL_LOWER()_table_select_by_`'NAME_SINGLE_LOWER()_id(sqlite3 *sql_connection, int NAME_SINGLE_LOWER()_id, NAME_SINGLE_LOWER()_row_t **NAME_SINGLE_LOWER()_row)
{
  sqlite3_stmt *sql_statement = NULL;
  NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(NAME_SINGLE_LOWER()_row != NULL, "NAME_SINGLE_LOWER()_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"NAME_SINGLE_LOWER()-id\", "
      "\"name\" "
    "FROM \"NAME_PLURAL_LOWER()\" "
    "WHERE \"NAME_SINGLE_LOWER()-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_`'NAME_SINGLE_LOWER()_id_result = sql_bind_int(sql_statement, 1, NAME_SINGLE_LOWER()_id);
  check(sql_bind_`'NAME_SINGLE_LOWER()_id_result == 0, "sql_bind_`'NAME_SINGLE_LOWER()_id_result: %d",
    sql_bind_`'NAME_SINGLE_LOWER()_id_result);

  int row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  if (row_available == 1)
  {
    int NAME_PLURAL_LOWER()_table_read_result = NAME_PLURAL_LOWER()_table_read(sql_statement, &NAME_SINGLE_LOWER()_row_return);
    check(NAME_PLURAL_LOWER()_table_read_result == 0, "NAME_PLURAL_LOWER()_table_read_result: %d",
      NAME_PLURAL_LOWER()_table_read_result);
  }

  sql_finalize_statement(sql_statement);

  *NAME_SINGLE_LOWER()_row = NAME_SINGLE_LOWER()_row_return;

  return 0;

error:

  if (NAME_SINGLE_LOWER()_row_return != NULL) { NAME_SINGLE_LOWER()_row_free(NAME_SINGLE_LOWER()_row_return); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// selects all the NAME_SINGLE_LOWER() rows
int NAME_PLURAL_LOWER()_table_select_all(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t ***NAME_SINGLE_LOWER()_rows, int *count)
{
  sqlite3_stmt *sql_statement = NULL;
  NAME_SINGLE_LOWER()_row_t **allocated_`'NAME_SINGLE_LOWER()_rows = NULL;
  int used_`'NAME_SINGLE_LOWER()_rows_count = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(NAME_SINGLE_LOWER()_rows != NULL, "NAME_SINGLE_LOWER()_rows: NULL");
  check(count != NULL, "count: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"NAME_SINGLE_LOWER()-id\", "
      "\"name\" "
    "FROM \"NAME_PLURAL_LOWER()\";",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int allocated_`'NAME_SINGLE_LOWER()_rows_count = 10;
  allocated_`'NAME_SINGLE_LOWER()_rows = realloc(allocated_`'NAME_SINGLE_LOWER()_rows, sizeof(NAME_SINGLE_LOWER()_row_t *) * allocated_`'NAME_SINGLE_LOWER()_rows_count);
  check_mem(allocated_`'NAME_SINGLE_LOWER()_rows);

  int row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (row_available == 1)
  {
    NAME_SINGLE_LOWER()_row_t **NAME_SINGLE_LOWER()_row = &(allocated_`'NAME_SINGLE_LOWER()_rows[used_`'NAME_SINGLE_LOWER()_rows_count]);

    int NAME_PLURAL_LOWER()_table_read_result = NAME_PLURAL_LOWER()_table_read(sql_statement, NAME_SINGLE_LOWER()_row);
    check(NAME_PLURAL_LOWER()_table_read_result == 0, "NAME_PLURAL_LOWER()_table_read_result: %d",
      NAME_PLURAL_LOWER()_table_read_result);

    used_`'NAME_SINGLE_LOWER()_rows_count++;

    if (used_`'NAME_SINGLE_LOWER()_rows_count == allocated_`'NAME_SINGLE_LOWER()_rows_count)
    {
      allocated_`'NAME_SINGLE_LOWER()_rows_count *= 2;
      allocated_`'NAME_SINGLE_LOWER()_rows = realloc(allocated_`'NAME_SINGLE_LOWER()_rows, sizeof(NAME_SINGLE_LOWER()_row_t *) * allocated_`'NAME_SINGLE_LOWER()_rows_count);
      check_mem(allocated_`'NAME_SINGLE_LOWER()_rows);
    }

    sql_step_select_result = sql_step_select(sql_statement, &row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
  }

  allocated_`'NAME_SINGLE_LOWER()_rows = realloc(allocated_`'NAME_SINGLE_LOWER()_rows, sizeof(NAME_SINGLE_LOWER()_row_t *) * used_`'NAME_SINGLE_LOWER()_rows_count);
  check(allocated_`'NAME_SINGLE_LOWER()_rows != NULL || used_`'NAME_SINGLE_LOWER()_rows_count == 0,
    "allocated_`'NAME_SINGLE_LOWER()_rows: NULL");

  sql_finalize_statement(sql_statement);

  *NAME_SINGLE_LOWER()_rows = allocated_`'NAME_SINGLE_LOWER()_rows;
  *count = used_`'NAME_SINGLE_LOWER()_rows_count;

  return 0;

error:

  if (allocated_`'NAME_SINGLE_LOWER()_rows != NULL) { NAME_SINGLE_LOWER()_rows_free(allocated_`'NAME_SINGLE_LOWER()_rows, used_`'NAME_SINGLE_LOWER()_rows_count); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// updates a NAME_SINGLE_LOWER() row
int NAME_PLURAL_LOWER()_table_update(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(NAME_SINGLE_LOWER()_row != NULL, "NAME_SINGLE_LOWER()_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "UPDATE \"NAME_PLURAL_LOWER()\" SET "
      "\"name\" = ?1 "
    "WHERE \"NAME_SINGLE_LOWER()-id\" = ?2;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_name_result = sql_bind_string(sql_statement, 1, NAME_SINGLE_LOWER()_row->name);
  check(sql_bind_name_result == 0, "sql_bind_name_result: %d",
    sql_bind_name_result);

  int sql_bind_`'NAME_SINGLE_LOWER()_id_result = sql_bind_int(sql_statement, 2, NAME_SINGLE_LOWER()_row->NAME_SINGLE_LOWER()_id);
  check(sql_bind_`'NAME_SINGLE_LOWER()_id_result == 0, "sql_bind_`'NAME_SINGLE_LOWER()_id_result: %d",
    sql_bind_`'NAME_SINGLE_LOWER()_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// deletes a NAME_SINGLE_LOWER() row
int NAME_PLURAL_LOWER()_table_delete(sqlite3 *sql_connection, NAME_SINGLE_LOWER()_row_t *NAME_SINGLE_LOWER()_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(NAME_SINGLE_LOWER()_row != NULL, "NAME_SINGLE_LOWER()_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "DELETE FROM \"NAME_PLURAL_LOWER()\" "
    "WHERE \"NAME_SINGLE_LOWER()-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_`'NAME_SINGLE_LOWER()_id_result = sql_bind_int(sql_statement, 1, NAME_SINGLE_LOWER()_row->NAME_SINGLE_LOWER()_id);
  check(sql_bind_`'NAME_SINGLE_LOWER()_id_result == 0, "sql_bind_`'NAME_SINGLE_LOWER()_id_result: %d",
    sql_bind_`'NAME_SINGLE_LOWER()_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}
