#define _BSD_SOURCE
#define _XOPEN_SOURCE

#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../infrastructure/string/string.h"
#include "../../infrastructure/time/time.h"

// opens a database connection
int sql_open_connection(char *filename, sqlite3 **sql_connection)
{
  sqlite3 *sql_connection_return = NULL;

  check(filename != NULL, "filename: NULL");

  int sqlite3_open_result = sqlite3_open_v2(filename, &sql_connection_return, SQLITE_OPEN_READWRITE, NULL);
  check(sqlite3_open_result == SQLITE_OK, "sqlite3_open_result: %d | filename: %s",
    sqlite3_open_result, filename);

  *sql_connection = sql_connection_return;

  return 0;

error:

  if (sql_connection_return != NULL) { sql_close_connection(sql_connection_return); }

  return -1;
}

// prepares a sql statement
int sql_prepare_statement(sqlite3 *sql_connection, char *sql, sqlite3_stmt **sql_statement)
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

  if (sql_statement_return != NULL) { sql_finalize_statement(sql_statement_return); }

  return -1;
}

// reads a sql column
int sql_read_int(sqlite3_stmt *sql_statement, int position, int *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_INTEGER, "column_type: %d",
    column_type);

  *value = sqlite3_column_int(sql_statement, position);

  return 0;

error:

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
  check(column_type == SQLITE_TEXT, "column_type: %d",
    column_type);

  char *column_text = (char *)sqlite3_column_text(sql_statement, position);
  string_duplicate(value_return, column_text);

  *value = value_return;

  return 0;

error:

  if (value_return != NULL) { free(value_return); }

  return -1;
}

// reads a sql column
int sql_read_date(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_TEXT, "column_type: %d",
    column_type);

  char *column_text = (char *)sqlite3_column_text(sql_statement, position);

  time_t time_result;

  int parse_utc_date_result = parse_utc_date(column_text, &time_result);
  check(parse_utc_date_result == 0, "parse_utc_date_result: %d",
    parse_utc_date_result);

  *value = time_result;

  return 0;

error:

  return -1;
}

// reads a sql column
int sql_read_date_time(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_TEXT, "column_type: %d",
    column_type);

  char *column_text = (char *)sqlite3_column_text(sql_statement, position);

  time_t time_result;

  int parse_utc_date_time_result = parse_utc_date_time(column_text, &time_result);
  check(parse_utc_date_time_result == 0, "parse_utc_date_time_result: %d",
    parse_utc_date_time_result);

  *value = time_result;

  return 0;

error:

  return -1;
}

// reads a sql column
int sql_read_time(sqlite3_stmt *sql_statement, int position, time_t *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");
  check(value != NULL, "value: NULL");

  int columns_count = sqlite3_column_count(sql_statement);
  check(position >= 0 && position < columns_count, "position: %d",
    position);

  int column_type = sqlite3_column_type(sql_statement, position);
  check(column_type == SQLITE_TEXT, "column_type: %d",
    column_type);

  char *column_text = (char *)sqlite3_column_text(sql_statement, position);

  time_t time_result;

  int parse_utc_time_result = parse_utc_time(column_text, &time_result);
  check(parse_utc_time_result == 0, "parse_utc_time_result: %d",
    parse_utc_time_result);

  *value = time_result;

  return 0;

error:

  return -1;
}

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int value)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  int sqlite3_bind_int_result = sqlite3_bind_int(sql_statement, position, value);
  check(sqlite3_bind_int_result == SQLITE_OK, "sqlite3_bind_int_result: %d | position: %d | value: %d",
    sqlite3_bind_int_result, position, value);

  return 0;

error:

  return -1;
}

// binds a sql statement parameter
int sql_bind_date(sqlite3_stmt *sql_statement, int position, time_t value)
{
  char *formatted_time = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");

  formatted_time = format_utc_date(value);

  int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT);
  check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
    sqlite3_bind_text_result, position, formatted_time);

  free(formatted_time);

  return 0;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return -1;
}

// binds a sql statement parameter
int sql_bind_date_time(sqlite3_stmt *sql_statement, int position, time_t value)
{
  char *formatted_time = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");

  formatted_time = format_utc_date_time(value);

  int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT);
  check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
    sqlite3_bind_text_result, position, formatted_time);

  free(formatted_time);

  return 0;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return -1;
}

// binds a sql statement parameter
int sql_bind_time(sqlite3_stmt *sql_statement, int position, time_t value)
{
  char *formatted_time = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");

  formatted_time = format_utc_time(value);

  int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, formatted_time, -1, SQLITE_TRANSIENT);
  check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
    sqlite3_bind_text_result, position, formatted_time);

  free(formatted_time);

  return 0;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return -1;
}

// binds a sql statement parameter
int sql_bind_string(sqlite3_stmt *sql_statement, int position, char *value)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  int sqlite3_bind_text_result = sqlite3_bind_text(sql_statement, position, value, -1, SQLITE_TRANSIENT);
  check(sqlite3_bind_text_result == SQLITE_OK, "sqlite3_bind_text_result: %d | position: %d | value: %s",
    sqlite3_bind_text_result, position, value);

  return 0;

error:

  return -1;
}

// steps a sql statement that selects rows
int sql_step_select(sqlite3_stmt *sql_statement, int *row_available)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  int sqlite3_step_result = sqlite3_step(sql_statement);
  check(sqlite3_step_result == SQLITE_ROW || sqlite3_step_result == SQLITE_DONE, "sqlite3_step_result: %d",
    sqlite3_step_result);

  *row_available = sqlite3_step_result == SQLITE_ROW ? 1 : 0;

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
void sql_finalize_statement(sqlite3_stmt *sql_statement)
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

// closes a database connection
void sql_close_connection(sqlite3 *sql_connection)
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
