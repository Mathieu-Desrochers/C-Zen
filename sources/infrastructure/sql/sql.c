#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

// opens a database connection
sqlite3 *sql_open_connection(char *filename)
{
  sqlite3 *sql_connection = NULL;

  int sqlite3_open_result = sqlite3_open_v2(filename, &sql_connection, SQLITE_OPEN_READWRITE, NULL);
  check(sqlite3_open_result == SQLITE_OK, "Could not open database '%s'.", filename);

  return sql_connection;

error:

  sql_close_connection(sql_connection);

  return NULL;
}

// closes a database connection
void sql_close_connection(sqlite3 *sql_connection)
{
  if (sql_connection == NULL)
  {
    return;
  }

  int sqlite3_close_result = sqlite3_close(sql_connection);
  check(sqlite3_close_result == SQLITE_OK, "Could not close database.");

error:

  return;
}
