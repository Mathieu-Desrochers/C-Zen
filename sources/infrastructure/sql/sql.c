#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

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
