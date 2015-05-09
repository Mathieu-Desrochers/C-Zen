#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

// opens a database connection
sqlite3 *sql_open_connection(char *filename)
{
  sqlite3 *sql_connection = NULL;

  check(filename != NULL, "filename: NULL");

  int sqlite3_open_result = sqlite3_open_v2(filename, &sql_connection, SQLITE_OPEN_READWRITE, NULL);
  check(sqlite3_open_result == SQLITE_OK, "sqlite3_open_result: %d | filename: %s",
    sqlite3_open_result, filename);

  return sql_connection;

error:

  if (sql_connection != NULL) { sql_close_connection(sql_connection); }

  return NULL;
}

// prepares a sql statement
sqlite3_stmt *sql_prepare_statement(sqlite3 *sql_connection, char *sql)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(sql != NULL, "sql: NULL");

  int sqlite3_prepare_result = sqlite3_prepare_v2(sql_connection, sql, -1, &sql_statement, NULL);
  check(sqlite3_prepare_result == SQLITE_OK, "sqlite3_prepare_result: %d | sql: %s",
    sqlite3_prepare_result, sql);

  return sql_statement;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return NULL;
}

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int value)
{
  check(sql_statement != NULL, "sql_statement: NULL)");

  int sqlite3_bind_int_result = sqlite3_bind_int(sql_statement, position, value);
  check(sqlite3_bind_int_result == SQLITE_OK, "sqlite3_bind_int_result: %d | position: %d | value: %d",
    sqlite3_bind_int_result, position, value);

  return 0;

error:

  return -1;
}

// steps a sql statement that selects rows
int sql_select_step(sqlite3_stmt *sql_statement, int *sql_select_step_status)
{
  check(sql_statement != NULL, "sql_statement: NULL");

  int sqlite3_step_result = sqlite3_step(sql_statement);
  check(sqlite3_step_result == SQLITE_ROW || sqlite3_step_result == SQLITE_DONE, "sqlite3_step_result: %d",
    sqlite3_step_result);

  *sql_select_step_status = sqlite3_step_result;

  return 0;

error:

  return -1;
}

// executes a sql statement that selects one row
int sql_select_one(
  sqlite3 *sql_connection,
  char *sql,
  void **parameters,
  int (*sql_bind)(sqlite3_stmt *sql_statement, void **parameters),
  int (*sql_read)(sqlite3_stmt *sql_statement, void **row),
  void (*sql_free_row)(void *row),
  void **row)
{
  sqlite3_stmt *sql_statement = NULL;

  void *sql_read_row = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(sql != NULL, "sql: NULL");
  check(parameters != NULL, "parameters: NULL");
  check(sql_bind != NULL, "sql_bind: NULL");
  check(sql_read != NULL, "sql_read: NULL");
  check(row != NULL, "row: NULL");

  sql_statement = sql_prepare_statement(sql_connection, sql);
  check(sql_statement != NULL, "sql_statement: NULL");

  int sql_bind_result = sql_bind(sql_statement, parameters);
  check(sql_bind_result == 0, "sql_bind_result: %d",
    sql_bind_result);

  int sql_select_step_status = 0;
  int sql_select_step_result = sql_select_step(sql_statement, &sql_select_step_status);
  check(sql_select_step_result == 0, "sql_select_step_result: %d",
    sql_select_step_result);

  if (sql_select_step_status == SQLITE_ROW)
  {
    int sql_read_result = sql_read(sql_statement, &sql_read_row);
    check(sql_read_result == 0, "sql_read_result: %d",
      sql_read_result);
  }

  sql_finalize_statement(sql_statement);

  *row = sql_read_row;

  return 0;

error:

  if (sql_read_row != NULL) { sql_free_row(sql_read_row); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// executes a sql statement that selects many rows
int sql_select_many(
  sqlite3 *sql_connection,
  char *sql,
  void **parameters,
  int (*sql_bind)(sqlite3_stmt *sql_statement, void **parameters),
  int (*sql_read)(sqlite3_stmt *sql_statement, void **row),
  void (*sql_free_rows)(void **rows, int count),
  void ***rows,
  int *count)
{
  sqlite3_stmt *sql_statement = NULL;

  void **allocated_rows = NULL;
  int allocated_rows_count = 0;

  int rows_count = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(sql != NULL, "sql: NULL");
  check(parameters != NULL, "parameters: NULL");
  check(sql_bind != NULL, "sql_bind: NULL");
  check(sql_read != NULL, "sql_read: NULL");
  check(rows != NULL, "rows: NULL");
  check(count != NULL, "count: NULL");

  sql_statement = sql_prepare_statement(sql_connection, sql);
  check(sql_statement != NULL, "sql_statement: NULL");

  int sql_bind_result = sql_bind(sql_statement, parameters);
  check(sql_bind_result == 0, "sql_bind_result: %d",
    sql_bind_result);

  int sql_select_step_status = 0;
  int sql_select_step_result = sql_select_step(sql_statement, &sql_select_step_status);
  check(sql_select_step_result == 0, "sql_select_step_result: %d",
    sql_select_step_result);

  allocated_rows_count = 10;
  allocated_rows = realloc(allocated_rows, sizeof(void *) * allocated_rows_count);
  check_mem(allocated_rows);

  while (sql_select_step_status == SQLITE_ROW)
  {
    void **sql_read_row = &(allocated_rows[rows_count]);

    int sql_read_result = sql_read(sql_statement, sql_read_row);
    check(sql_read_result == 0, "sql_read_result: %d",
      sql_read_result);

    rows_count++;

    if (rows_count == allocated_rows_count)
    {
      allocated_rows_count *= 2;
      allocated_rows = realloc(allocated_rows, sizeof(void *) * allocated_rows_count);
      check_mem(allocated_rows);
    }

    sql_select_step_result = sql_select_step(sql_statement, &sql_select_step_status);
    check(sql_select_step_result == 0, "sql_select_step_result: %d",
      sql_select_step_result);
  }

  allocated_rows = realloc(allocated_rows, sizeof(void *) * rows_count);
  check(rows_count == 0 || allocated_rows != NULL,
    "allocated_rows: NULL");

  sql_finalize_statement(sql_statement);

  *rows = allocated_rows;
  *count = rows_count;

  return 0;

error:

  if (allocated_rows != NULL) { allocated_rows = realloc(allocated_rows, sizeof(void *) * rows_count); }
  if (allocated_rows != NULL) { sql_free_rows(allocated_rows, rows_count); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

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
