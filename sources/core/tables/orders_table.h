#ifndef ORDERS_TABLE_H
#define ORDERS_TABLE_H

#include <sqlite3.h>

#include "../../core/tables/order_row.h"

// inserts an order row
int orders_table_insert(sqlite3 *sql_connection, order_row_t *order_row);

// selects an order row by order id
int orders_table_select_by_order_id(sqlite3 *sql_connection, int order_id, order_row_t **order_row);

// selects all the order rows
int orders_table_select_all(sqlite3 *sql_connection, order_row_t ***order_rows, int *count);

// updates an order row
int orders_table_update(sqlite3 *sql_connection, order_row_t *order_row);

// deletes an order row
int orders_table_delete(sqlite3 *sql_connection, order_row_t *order_row);

#endif
