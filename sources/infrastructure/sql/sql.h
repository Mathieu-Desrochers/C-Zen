#ifndef SQL_H
#define SQL_H

#include <sqlite3.h>
#include <time.h>

// opens a database connection
int sql_open_connection(char *filename, sqlite3 **sql_connection);

// prepares a sql statement
int sql_prepare_statement(sqlite3 *sql_connection, char *sql, sqlite3_stmt **sql_statement);

// reads a sql column
int sql_read_int(sqlite3_stmt *sql_statement, int position, int *value);
int sql_read_string(sqlite3_stmt *sql_statement, int position, char **value);
int sql_read_date(sqlite3_stmt *sql_statement, int position, time_t *value);
int sql_read_date_time(sqlite3_stmt *sql_statement, int position, time_t *value);
int sql_read_time(sqlite3_stmt *sql_statement, int position, time_t *value);

// binds a sql statement parameter
int sql_bind_int(sqlite3_stmt *sql_statement, int position, int value);
int sql_bind_string(sqlite3_stmt *sql_statement, int position, char *value);
int sql_bind_date_time(sqlite3_stmt *sql_statement, int position, int value);
int sql_bind_date(sqlite3_stmt *sql_statement, int position, int value);

// steps a sql statement that selects rows
int sql_step_select(sqlite3_stmt *sql_statement, int *row_available);

// steps a sql statement that executes a command
int sql_step_execute(sqlite3_stmt *sql_statement);

// finalizes a sql statement
void sql_finalize_statement(sqlite3_stmt *sql_statement);

// closes a database connection
void sql_close_connection(sqlite3 *sql_connection);

#endif
