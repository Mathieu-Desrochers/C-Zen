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

  check(filename != NULL, "filename: NULL");

  int sqlite3_open_result = sqlite3_open_v2(filename, &sql_connection_return, SQLITE_OPEN_READWRITE, NULL);
  check(sqlite3_open_result == SQLITE_OK, "sqlite3_open_result: %d | filename: %s",
    sqlite3_open_result, filename);

  *sql_connection = sql_connection_return;

  return 0;

error:

  if (sql_connection_return != NULL) { sql_connection_close(sql_connection_return); }

  return -1;
}

// prepares a sql statement
int sql_statement_prepare(sqlite3 *sql_connection, char *sql, sqlite3_stmt **sql_statement)
{
  sqlite3_stmt *sql_statement_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(sql != NULL, "sql: NULL");

  int sqlite3_prepare_result = sqlite3_prepare_v2(sql_connection, sql, -1, &sql_statement_return, NULL);
  check(sqlite3_prepare_result == SQLITE_OK, "sqlite3_prepare_result: %d | sql: %s",
    sqlite3_prepare_result, sql);

  *sql_statement = sql_statement_return;

  return 0;

error:

  if (sql_statement_return != NULL) { sql_statement_finalize(sql_statement_return); }

  return -1;
}

// reads a sql column
int sql_read_int(sqlite3_stmt *sql_statement, int position, int **value)
{
  int *value_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_INTEGER || column_type == SQLITE_NULL, "column_type: %d",
    column_type);

  if (column_type == SQLITE_INTEGER)
  {
    int column_int = sqlite3_column_int(sql_statement, position);

    int malloc_memcpy_int_result = malloc_memcpy_int(&value_return, &column_int);
    check(malloc_memcpy_int_result == 0, "malloc_memcpy_int_result: %d",
      malloc_memcpy_int_result);
  }

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// reads a sql column
int sql_read_double(sqlite3_stmt *sql_statement, int position, double **value)
{
  double *value_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_FLOAT || column_type == SQLITE_NULL, "column_type: %d",
    column_type);

  if (column_type == SQLITE_FLOAT)
  {
    double column_double = sqlite3_column_double(sql_statement, position);

    int malloc_memcpy_double_result = malloc_memcpy_double(&value_return, &column_double);
    check(malloc_memcpy_double_result == 0, "malloc_memcpy_double_result: %d",
      malloc_memcpy_double_result);
  }

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// reads a sql column
int sql_read_string(sqlite3_stmt *sql_statement, int position, char **value)
{
  char *value_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_TEXT || column_type == SQLITE_NULL, "column_type: %d",
    column_type);

  if (column_type == SQLITE_TEXT)
  {
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);

    int malloc_memcpy_string_result = malloc_memcpy_string(&value_return, column_text);
    check(malloc_memcpy_string_result == 0, "malloc_memcpy_string_result: %d",
      malloc_memcpy_string_result);
  }

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// reads a sql column
int sql_read_date(sqlite3_stmt *sql_statement, int position, time_t **value)
{
  time_t *value_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_TEXT || column_type == SQLITE_NULL, "column_type: %d",
    column_type);

  if (column_type == SQLITE_TEXT)
  {
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);

    time_t parsed_value;
    int parse_utc_date_result = parse_utc_date(column_text, &parsed_value);
    check(parse_utc_date_result == 0, "parse_utc_date_result: %d",
      parse_utc_date_result);

    int malloc_memcpy_time_result = malloc_memcpy_time(&value_return, &parsed_value);
    check(malloc_memcpy_time_result == 0, "malloc_memcpy_time_result: %d",
      malloc_memcpy_time_result);
  }

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// reads a sql column
int sql_read_date_time(sqlite3_stmt *sql_statement, int position, time_t **value)
{
  time_t *value_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_TEXT || column_type == SQLITE_NULL, "column_type: %d",
    column_type);

  if (column_type == SQLITE_TEXT)
  {
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);

    time_t parsed_value;
    int parse_utc_date_time_result = parse_utc_date_time(column_text, &parsed_value);
    check(parse_utc_date_time_result == 0, "parse_utc_date_time_result: %d",
      parse_utc_date_time_result);

    int malloc_memcpy_time_result = malloc_memcpy_time(&value_return, &parsed_value);
    check(malloc_memcpy_time_result == 0, "malloc_memcpy_time_result: %d",
      malloc_memcpy_time_result);
  }

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// reads a sql column
int sql_read_time(sqlite3_stmt *sql_statement, int position, time_t **value)
{
  time_t *value_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_TEXT ||column_type == SQLITE_NULL, "column_type: %d",
    column_type);

  if (column_type == SQLITE_TEXT)
  {
    char *column_text = (char *)sqlite3_column_text(sql_statement, position);

    time_t parsed_value;
    int parse_utc_time_result = parse_utc_time(column_text, &parsed_value);
    check(parse_utc_time_result == 0, "parse_utc_time_result: %d",
      parse_utc_time_result);

    int malloc_memcpy_time_result = malloc_memcpy_time(&value_return, &parsed_value);
    check(malloc_memcpy_time_result == 0, "malloc_memcpy_time_result: %d",
      malloc_memcpy_time_result);
  }

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  if (value == NULL)
  {
    int sqlite3_bind_null_result = sqlite3_bind_null(sql_statement, position);
    check(sqlite3_bind_null_result == SQLITE_OK, "sqlite3_bind_null_result: %d | position: %d",
      sqlite3_bind_null_result, position);
  }
  else
  {
    int sqlite3_bind_int_result = sqlite3_bind_int(sql_statement, position, *value);
    check(sqlite3_bind_int_result == SQLITE_OK, "sqlite3_bind_int_result: %d | position: %d | value: %d",
      sqlite3_bind_int_result, position, *value);
  }

  return 0;

error:

  return -1;
}

// binds a sql statement parameter
int sql_bind_double(sqlite3_stmt *sql_statement, int position, double *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  if (value == NULL)
  {
    int sqlite3_bind_null_result = sqlite3_bind_null(sql_statement, position);
    check(sqlite3_bind_null_result == SQLITE_OK, "sqlite3_bind_null_result: %d | position: %d",
      sqlite3_bind_null_result, position);
  }
  else
  {
    int sqlite3_bind_double_result = sqlite3_bind_double(sql_statement, position, *value);
    check(sqlite3_bind_double_result == SQLITE_OK, "sqlite3_bind_double_result: %d | position: %d | value: %f",
      sqlite3_bind_double_result, position, *value);
  }

  return 0;

error:

  return -1;
}

// binds a sql statement parameter
int sql_bind_date(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  char *formatted_time = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");

  if (value == NULL)
  {
    int sqlite3_bind_null_result = sqlite3_bind_null(sql_statement, position);
    check(sqlite3_bind_null_result == SQLITE_OK, "sqlite3_bind_null_result: %d | position: %d",
      sqlite3_bind_null_result, position);
  }
  else
  {
    formatted_time = format_utc_date(*value);

    int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT);
    check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
      sqlite3_bind_text_result, position, formatted_time);

    free(formatted_time);
  }

  return 0;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return -1;
}

// binds a sql statement parameter
int sql_bind_date_time(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  char *formatted_time = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");

  if (value == NULL)
  {
    int sqlite3_bind_null_result = sqlite3_bind_null(sql_statement, position);
    check(sqlite3_bind_null_result == SQLITE_OK, "sqlite3_bind_null_result: %d | position: %d",
      sqlite3_bind_null_result, position);
  }
  else
  {
    formatted_time = format_utc_date_time(*value);

    int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT);
    check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
      sqlite3_bind_text_result, position, formatted_time);

    free(formatted_time);
  }

  return 0;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return -1;
}

// binds a sql statement parameter
int sql_bind_time(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  char *formatted_time = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");

  if (value == NULL)
  {
    int sqlite3_bind_null_result = sqlite3_bind_null(sql_statement, position);
    check(sqlite3_bind_null_result == SQLITE_OK, "sqlite3_bind_null_result: %d | position: %d",
      sqlite3_bind_null_result, position);
  }
  else
  {
    formatted_time = format_utc_time(*value);

    int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT);
    check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
      sqlite3_bind_text_result, position, formatted_time);

    free(formatted_time);
  }

  return 0;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return -1;
}

// binds a sql statement parameter
int sql_bind_string(sqlite3_stmt *sql_statement, int position, char *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  if (value == NULL)
  {
    int sqlite3_bind_null_result = sqlite3_bind_null(sql_statement, position);
    check(sqlite3_bind_null_result == SQLITE_OK, "sqlite3_bind_null_result: %d | position: %d",
      sqlite3_bind_null_result, position);
  }
  else
  {
    int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, value, -1, SQLITE_TRANSIENT);
    check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
      sqlite3_bind_text_result, position, value);
  }

  return 0;

error:

  return -1;
}

// steps a sql statement that selects rows
int sql_step_select(sqlite3_stmt *sql_statement, int *is_row_available)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  int sqlite3_step_result = sqlite3_step(sql_statement);
  check(sqlite3_step_result == SQLITE_ROW || sqlite3_step_result == SQLITE_DONE, "sqlite3_step_result: %d",
    sqlite3_step_result);

  *is_row_available = sqlite3_step_result == SQLITE_ROW ? 1 : 0;

  return 0;

error:

  return -1;
}

// steps a sql statement that executes a command
int sql_step_execute(sqlite3_stmt *sql_statement)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  int sqlite3_step_result = sqlite3_step(sql_statement);
  check(sqlite3_step_result == SQLITE_DONE, "sqlite3_step_result: %d",
    sqlite3_step_result);

  return 0;

error:

  return -1;
}

// finalizes a sql statement
void sql_statement_finalize(sqlite3_stmt *sql_statement)
{
  if (sql_statement == NULL)
  {
    return;
  }

  int sqlite3_finalize_result = sqlite3_finalize(sql_statement);
  check(sqlite3_finalize_result == SQLITE_OK, "sqlite3_finalize_result: %d",
    sqlite3_finalize_result);

error:

  return;
}

// selects the row id generated by the last insert statement
int sql_last_generated_id(sqlite3 *sql_connection, int **last_insert_row_id)
{
  sqlite3_stmt *sql_statement = NULL;
  int *last_insert_row_id_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(last_insert_row_id != NULL, "last_insert_row_id: NULL");

  int sql_statement_prepare_result = sql_statement_prepare(
    sql_connection,
    "SELECT last_insert_rowid();",
    &sql_statement);

  check(sql_statement_prepare_result == 0, "sql_statement_prepare_result: %d",
    sql_statement_prepare_result);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  check(is_row_available == 1, "is_row_available: %d", is_row_available);

  int sql_read_result = sql_read_int(sql_statement, 0, &last_insert_row_id_return);
  check(sql_read_result == 0, "sql_read_result: %d",
    sql_read_result);

  sql_statement_finalize(sql_statement);

  *last_insert_row_id = last_insert_row_id_return;

  return 0;

error:

  if (last_insert_row_id_return != NULL) { free(last_insert_row_id_return); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// closes a database connection
void sql_connection_close(sqlite3 *sql_connection)
{
  if (sql_connection == NULL)
  {
    return;
  }

  int sqlite3_close_result = sqlite3_close(sql_connection);
  check(sqlite3_close_result == SQLITE_OK, "sqlite3_close_result: %d",
    sqlite3_close_result);

error:

  return;
}
