#ifndef CUSTOMERS_TABLE_H
#define CUSTOMERS_TABLE_H

#include <sqlite3.h>

#include "../../core/tables/customer_row.h"

// inserts a customer row
int customers_table_insert(sqlite3 *sql_connection, customer_row_t *customer_row);

// selects a customer row by customer id
int customers_table_select_by_customer_id(sqlite3 *sql_connection, int customer_id, customer_row_t **customer_row);

// selects all the customer rows
int customers_table_select_all(sqlite3 *sql_connection, customer_row_t ***customer_row);

// updates a customer row
int customers_table_update(sqlite3 *sql_connection, customer_row_t *customer_row);

// deletes a customer row
int customers_table_delete(sqlite3 *sql_connection, customer_row_t *customer_row);

#endif
