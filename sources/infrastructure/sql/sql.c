#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

// opens a database connection
sqlite3 *sql_open_connection(char *filename)
{
  check(filename != NULL, "filename == NULL");

  sqlite3 *sql_connection = NULL;

  int sqlite3_open_result = sqlite3_open_v2(filename, &sql_connection, SQLITE_OPEN_READWRITE, NULL);
  check(sqlite3_open_result == SQLITE_OK, "filename: '%s' | error: '%s'",
    filename, sqlite3_errmsg(sql_connection));

  return sql_connection;

error:

  sql_close_connection(sql_connection);

  return NULL;
}

// prepares a sql statement
sqlite3_stmt *sql_prepare_statement(sqlite3 *sql_connection, char *sql)
{
  check(sql_connection != NULL, "sql_connection == NULL");
  check(sql != NULL, "sql == NULL");

  sqlite3_stmt *sql_statement = NULL;

  int sqlite3_prepare_result = sqlite3_prepare_v2(sql_connection, sql, -1, &sql_statement, NULL);
  check(sqlite3_prepare_result == SQLITE_OK, "sql: '%s' | error: '%s'",
    sql, sqlite3_errmsg(sql_connection));

  return sql_statement;

error:

  return NULL;
}

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int value)
{
  check(sql_statement != NULL, "sql_statement == NULL");

  int sqlite3_bind_int_result = sqlite3_bind_int(sql_statement, position, value);
  check(sqlite3_bind_int_result == SQLITE_OK, "position: %d | value: %d | error: '%s'",
    position, value, sqlite3_errstr(sqlite3_bind_int_result));

  return 0;

error:

  return -1;
}

// steps a sql statement
int sql_step(sqlite3_stmt *sql_statement)
{
  check(sql_statement != NULL, "sql_statement == NULL");

  int sqlite3_step_result = sqlite3_step(sql_statement);
  check(sqlite3_step_result == SQLITE_DONE || sqlite3_step_result == SQLITE_ROW, "error: '%s'",
    sqlite3_errstr(sqlite3_step_result));

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
  check(sqlite3_finalize_result == SQLITE_OK, "sqlite3_finalize_result: '%s'",
    sqlite3_errstr(sqlite3_finalize_result));

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
  check(sqlite3_close_result == SQLITE_OK, "sqlite3_close_result: '%s'",
    sqlite3_errstr(sqlite3_close_result));

error:

  return;
}
