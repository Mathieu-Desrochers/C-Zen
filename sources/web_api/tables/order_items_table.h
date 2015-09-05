#ifndef ORDER_ITEMS_TABLE_H
#define ORDER_ITEMS_TABLE_H

#include <sqlite3.h>

#include "../../web_api/tables/order_item_row.h"

// inserts an order item row
int order_items_table_insert(sqlite3 *sql_connection, order_item_row_t *order_item_row);

// selects an order item row by order item id
int order_items_table_select_by_order_item_id(sqlite3 *sql_connection, int order_item_id, order_item_row_t **order_item_row);

// selects order item rows by order id
int order_items_table_select_by_order_id(sqlite3 *sql_connection, int order_id, order_item_row_t ***order_item_rows, int *count);

// selects all the order item rows
int order_items_table_select_all(sqlite3 *sql_connection, order_item_row_t ***order_item_rows, int *count);

// updates an order item row
int order_items_table_update(sqlite3 *sql_connection, order_item_row_t *order_item_row);

// deletes an order item row
int order_items_table_delete(sqlite3 *sql_connection, order_item_row_t *order_item_row);

#endif
