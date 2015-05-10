#ifndef SQL_H
#define SQL_H

#include <sqlite3.h>

// opens a database connection
int sql_open_connection(char *filename, sqlite3 **sql_connection);

// prepares a sql statement
int sql_prepare_statement(sqlite3 *sql_connection, char *sql, sqlite3_stmt **sql_statement);

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int value);

// steps a sql statement that selects rows
int sql_step_select(sqlite3_stmt *sql_statement, int *sql_select_step_status);

// finalizes a sql statement
void sql_finalize_statement(sqlite3_stmt *sql_statement);

// closes a database connection
void sql_close_connection(sqlite3 *sql_connection);

#endif
