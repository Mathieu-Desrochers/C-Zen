#ifndef SQL_H
#define SQL_H

#include <sqlite3.h>

// opens a database connection
sqlite3 *sql_open_connection(char *filename);

// closes a database connection
void sql_close_connection(sqlite3 *sql_connection);

#endif
