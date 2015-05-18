#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../core/tables/order_item_row.h"
#include "../../core/tables/order_items_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  order_item_row_t *order_item_row = NULL;

  sqlite3 *sql_connection;
  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  order_item_row = order_item_row_malloc(
    0,
    1,
    "Wishing wand",
    2);

  check(order_item_row != NULL, "order_item_row: NULL");

  int insert_result = order_items_table_insert(sql_connection, order_item_row);
  check(insert_result == 0, "insert_result: %d",
    insert_result);

  order_item_row_free(order_item_row);

  sql_close_connection(sql_connection);

  return 0;

error:

  if (order_item_row != NULL) { order_item_row_free(order_item_row); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }

  return -1;
}
