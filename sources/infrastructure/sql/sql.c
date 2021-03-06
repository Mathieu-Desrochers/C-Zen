#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/sql/sql.h"
#include "../../infrastructure/time/time.h"

// opens a database connection
int sql_connection_open(char *filename, sqlite3 **sql_connection)
{
  sqlite3 *sql_connection_return = NULL;

  int exit_code = 0;

  sqlite3_stmt *sql_statement_foreign_keys = NULL;
  sqlite3_stmt *sql_statement_synchronous = NULL;

  check_not_null(filename);
  check_not_null(sql_connection);

  check_result(sqlite3_open_v2(filename, &sql_connection_return, SQLITE_OPEN_READWRITE, NULL), SQLITE_OK);

  check_result(
    sqlite3_prepare_v2(sql_connection_return, "PRAGMA foreign_keys = ON;", -1, &sql_statement_foreign_keys, NULL),
    SQLITE_OK);

  check_result(
    sqlite3_prepare_v2(sql_connection_return, "PRAGMA synchronous = OFF;", -1, &sql_statement_synchronous, NULL),
    SQLITE_OK);

  check_result(sqlite3_step(sql_statement_foreign_keys), SQLITE_DONE);
  check_result(sqlite3_step(sql_statement_synchronous), SQLITE_DONE);

  *sql_connection = sql_connection_return;

  goto cleanup;

error:

  if (sql_statement_foreign_keys != NULL) { sql_statement_finalize(sql_statement_foreign_keys); }
  if (sql_statement_synchronous != NULL) { sql_statement_finalize(sql_statement_synchronous); }
  if (sql_connection_return != NULL) { sql_connection_close(sql_connection_return); }

  sql_statement_foreign_keys = NULL;
  sql_statement_synchronous = NULL;

  exit_code = -1;

cleanup:

  if (sql_statement_foreign_keys != NULL) { sql_statement_finalize(sql_statement_foreign_keys); }
  if (sql_statement_synchronous != NULL) { sql_statement_finalize(sql_statement_synchronous); }

  return exit_code;
}

// begins a sql transaction
int sql_transaction_begin(sqlite3 *sql_connection)
{
  int exit_code = 0;

  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);

  check_result(sqlite3_prepare_v2(sql_connection, "BEGIN TRANSACTION;", -1, &sql_statement, NULL), SQLITE_OK);
  check_result(sqlite3_step(sql_statement), SQLITE_DONE);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return exit_code;
}

// prepares a sql statement
int sql_statement_prepare(sqlite3 *sql_connection, char *sql, sqlite3_stmt **sql_statement)
{
  sqlite3_stmt *sql_statement_return = NULL;

  int exit_code = 0;

  check_not_null(sql_connection);
  check_not_null(sql);
  check_not_null(sql_statement);

  check_result(sqlite3_prepare_v2(sql_connection, sql, -1, &sql_statement_return, NULL), SQLITE_OK);

  *sql_statement = sql_statement_return;

  goto cleanup;

error:

  if (sql_statement_return != NULL) { sql_statement_finalize(sql_statement_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// reads a sql column
int sql_read_int(sqlite3_stmt *sql_statement, int position, int **value)
{
  int *value_return = NULL;

  int exit_code = 0;

  check_not_null(sql_statement);
  check_not_null(value);

  int columns_count = sqlite3_column_count(sql_statement);
  check_int_between(position, 0, columns_count - 1);

  int column_type = sqlite3_column_type(sql_statement, position);
  check_int_or(column_type, SQLITE_INTEGER, SQLITE_NULL);

  if (column_type == SQLITE_INTEGER)
  {
    int column_int = sqlite3_column_int(sql_statement, position);
    check_result(malloc_memcpy_int(&value_return, &column_int), 0);
  }

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// reads a sql column
int sql_read_double(sqlite3_stmt *sql_statement, int position, double **value)
{
  double *value_return = NULL;

  int exit_code = 0;

  check_not_null(sql_statement);
  check_not_null(value);

  int columns_count = sqlite3_column_count(sql_statement);
  check_int_between(position, 0, columns_count - 1);

  int column_type = sqlite3_column_type(sql_statement, position);
  check_int_or(column_type, SQLITE_FLOAT, SQLITE_NULL);

  if (column_type == SQLITE_FLOAT)
  {
    double column_double = sqlite3_column_double(sql_statement, position);
    check_result(malloc_memcpy_double(&value_return, &column_double), 0);
  }

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// reads a sql column
int sql_read_string(sqlite3_stmt *sql_statement, int position, char **value)
{
  char *value_return = NULL;

  int exit_code = 0;

  check_not_null(sql_statement);
  check_not_null(value);

  int columns_count = sqlite3_column_count(sql_statement);
  check_int_between(position, 0, columns_count - 1);

  int column_type = sqlite3_column_type(sql_statement, position);
  check_int_or(column_type, SQLITE_TEXT, SQLITE_NULL);

  if (column_type == SQLITE_TEXT)
  {
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);
    check_result(malloc_memcpy_string(&value_return, column_text), 0);
  }

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// reads a sql column
int sql_read_date(sqlite3_stmt *sql_statement, int position, time_t **value)
{
  time_t *value_return = NULL;

  int exit_code = 0;

  check_not_null(sql_statement);
  check_not_null(value);

  int columns_count = sqlite3_column_count(sql_statement);
  check_int_between(position, 0, columns_count - 1);

  int column_type = sqlite3_column_type(sql_statement, position);
  check_int_or(column_type, SQLITE_TEXT, SQLITE_NULL);

  if (column_type == SQLITE_TEXT)
  {
    time_t parsed_value;
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);
    check_result(parse_utc_date(column_text, &parsed_value), 0);
    check_result(malloc_memcpy_time(&value_return, &parsed_value), 0);
  }

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// reads a sql column
int sql_read_date_time(sqlite3_stmt *sql_statement, int position, time_t **value)
{
  time_t *value_return = NULL;

  int exit_code = 0;

  check_not_null(sql_statement);
  check_not_null(value);

  int columns_count = sqlite3_column_count(sql_statement);
  check_int_between(position, 0, columns_count - 1);

  int column_type = sqlite3_column_type(sql_statement, position);
  check_int_or(column_type, SQLITE_TEXT, SQLITE_NULL);

  if (column_type == SQLITE_TEXT)
  {
    time_t parsed_value;
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);
    check_result(parse_utc_date_time(column_text, &parsed_value), 0);
    check_result(malloc_memcpy_time(&value_return, &parsed_value), 0);
  }

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// reads a sql column
int sql_read_time(sqlite3_stmt *sql_statement, int position, time_t **value)
{
  time_t *value_return = NULL;

  int exit_code = 0;

  check_not_null(sql_statement);
  check_not_null(value);

  int columns_count = sqlite3_column_count(sql_statement);
  check_int_between(position, 0, columns_count - 1);

  int column_type = sqlite3_column_type(sql_statement, position);
  check_int_or(column_type, SQLITE_TEXT, SQLITE_NULL);

  if (column_type == SQLITE_TEXT)
  {
    time_t parsed_value;
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);
    check_result(parse_utc_time(column_text, &parsed_value), 0);
    check_result(malloc_memcpy_time(&value_return, &parsed_value), 0);
  }

  *value = value_return;

  goto cleanup;

error:

  if (value_return != NULL) { free(value_return); }

  exit_code = -1;

cleanup:

  return exit_code;
}

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int *value)
{
  int exit_code = 0;

  check_not_null(sql_statement);

  if (value == NULL)
  {
    check_result(sqlite3_bind_null(sql_statement, position), SQLITE_OK);
  }
  else
  {
    check_result(sqlite3_bind_int(sql_statement, position, *value), SQLITE_OK);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// binds a sql statement parameter
int sql_bind_double(sqlite3_stmt *sql_statement, int position, double *value)
{
  int exit_code = 0;

  check_not_null(sql_statement);

  if (value == NULL)
  {
    check_result(sqlite3_bind_null(sql_statement, position), SQLITE_OK);
  }
  else
  {
    check_result(sqlite3_bind_double(sql_statement, position, *value), SQLITE_OK);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// binds a sql statement parameter
int sql_bind_date(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  int exit_code = 0;

  char *formatted_time = NULL;

  check_not_null(sql_statement);

  if (value == NULL)
  {
    check_result(sqlite3_bind_null(sql_statement, position), SQLITE_OK);
  }
  else
  {
    formatted_time = format_utc_date(*value);
    check_result(sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT), SQLITE_OK);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (formatted_time != NULL) { free(formatted_time); }

  return exit_code;
}

// binds a sql statement parameter
int sql_bind_date_time(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  int exit_code = 0;

  char *formatted_time = NULL;

  check_not_null(sql_statement);

  if (value == NULL)
  {
    check_result(sqlite3_bind_null(sql_statement, position), SQLITE_OK);
  }
  else
  {
    formatted_time = format_utc_date_time(*value);
    check_result(sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT), SQLITE_OK);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (formatted_time != NULL) { free(formatted_time); }

  return exit_code;
}

// binds a sql statement parameter
int sql_bind_time(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  int exit_code = 0;

  char *formatted_time = NULL;

  check_not_null(sql_statement);

  if (value == NULL)
  {
    check_result(sqlite3_bind_null(sql_statement, position), SQLITE_OK);
  }
  else
  {
    formatted_time = format_utc_time(*value);
    check_result(sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT), SQLITE_OK);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (formatted_time != NULL) { free(formatted_time); }

  return exit_code;
}

// binds a sql statement parameter
int sql_bind_string(sqlite3_stmt *sql_statement, int position, char *value)
{
  int exit_code = 0;

  check_not_null(sql_statement);

  if (value == NULL)
  {
    check_result(sqlite3_bind_null(sql_statement, position), SQLITE_OK);
  }
  else
  {
    check_result(sqlite3_bind_text(sql_statement, position, value, -1, SQLITE_TRANSIENT), SQLITE_OK);
  }

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// steps a sql statement that selects rows
int sql_step_select(sqlite3_stmt *sql_statement, int *is_row_available)
{
  int exit_code = 0;

  check_not_null(sql_statement);
  check_not_null(is_row_available);

  int sqlite3_step_result = sqlite3_step(sql_statement);
  check_int_or(sqlite3_step_result, SQLITE_ROW, SQLITE_DONE);

  *is_row_available = sqlite3_step_result == SQLITE_ROW ? 1 : 0;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// steps a sql statement that executes a command
int sql_step_execute(sqlite3_stmt *sql_statement)
{
  int exit_code = 0;

  check_not_null(sql_statement);

  check_result(sqlite3_step(sql_statement), SQLITE_DONE);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// finalizes a sql statement
void sql_statement_finalize(sqlite3_stmt *sql_statement)
{
  if (sql_statement == NULL)
  {
    return;
  }

  check_result(sqlite3_finalize(sql_statement), SQLITE_OK);

error:

  return;
}

// selects the row id generated by the last insert statement
int sql_last_generated_id(sqlite3 *sql_connection, int **last_insert_row_id)
{
  int *last_insert_row_id_return = NULL;

  int exit_code = 0;

  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);
  check_not_null(last_insert_row_id);

  check_result(sql_statement_prepare(sql_connection, "SELECT last_insert_rowid();", &sql_statement), 0);

  int is_row_available = 0;
  check_result(sql_step_select(sql_statement, &is_row_available), 0);
  check_int(is_row_available, 1);

  check_result(sql_read_int(sql_statement, 0, &last_insert_row_id_return), 0);

  *last_insert_row_id = last_insert_row_id_return;

  goto cleanup;

error:

  if (last_insert_row_id_return != NULL) { free(last_insert_row_id_return); }

  exit_code = -1;

cleanup:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return exit_code;
}

// ends a sql transaction
int sql_transaction_commit(sqlite3 *sql_connection)
{
  int exit_code = 0;

  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);

  check_result(sqlite3_prepare_v2(sql_connection, "COMMIT TRANSACTION;", -1, &sql_statement, NULL), SQLITE_OK);
  check_result(sqlite3_step(sql_statement), SQLITE_DONE);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return exit_code;
}

// ends a sql transaction
int sql_transaction_rollback(sqlite3 *sql_connection)
{
  int exit_code = 0;

  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);

  check_result(sqlite3_prepare_v2(sql_connection, "ROLLBACK TRANSACTION;", -1, &sql_statement, NULL), SQLITE_OK);
  check_result(sqlite3_step(sql_statement), SQLITE_DONE);

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return exit_code;
}

// closes a database connection
void sql_connection_close(sqlite3 *sql_connection)
{
  if (sql_connection == NULL)
  {
    return;
  }

  check_result(sqlite3_close(sql_connection), SQLITE_OK);

error:

  return;
}
