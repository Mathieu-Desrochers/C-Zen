#ifndef SQL_H
#define SQL_H

#include <sqlite3.h>

// opens a database connection
sqlite3 *sql_open_connection(char *filename);

// prepares a sql statement
sqlite3_stmt *sql_prepare_statement(sqlite3 *sql_connection, char *sql);

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int value);

// steps a sql statement that selects rows
int sql_select_step(sqlite3_stmt *sql_statement, int *sql_select_step_status);

// executes a sql statement that selects one row
int sql_select_one(
  sqlite3 *sql_connection,
  char *sql,
  void **parameters,
  int (*sql_bind)(sqlite3_stmt *sql_statement, void **parameters),
  int (*sql_read)(sqlite3_stmt *sql_statement, void **row),
  void (*sql_free_row)(void *row),
  void **row);

// executes a sql statement that selects many rows
int sql_select_many(
  sqlite3 *sql_connection,
  char *sql,
  void **parameters,
  int (*sql_bind)(sqlite3_stmt *sql_statement, void **parameters),
  int (*sql_read)(sqlite3_stmt *sql_statement, void **row),
  void (*sql_free_rows)(void **rows, int count),
  void ***rows,
  int *count);

// finalizes a sql statement
void sql_finalize_statement(sqlite3_stmt *sql_statement);

// closes a database connection
void sql_close_connection(sqlite3 *sql_connection);

#endif
