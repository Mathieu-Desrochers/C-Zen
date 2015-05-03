#ifndef CUSTOMERS_TABLE_H
#define CUSTOMERS_TABLE_H

#include <sqlite3.h>

#include "../../core/tables/customer_row.h"

// selects a customer_row by customer id
int customers_table_select_by_customer_id(sqlite3 *sql_connection, int customer_id, customer_row_t **customer_row);

#endif
