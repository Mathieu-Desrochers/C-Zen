#include <stdlib.h>
#include <time.h>

#include "../../core/tables/order_row.h"
#include "../../core/tables/orders_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  order_row_t *order_row = NULL;

  sqlite3 *sql_connection;
  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  order_row = order_row_malloc(
    0,
    1,
    100,
    1431952982,
    1431952982);

  orders_table_insert(
    sql_connection,
    order_row);

  printf("%d\n", order_row->order_id);

  order_row_free(order_row);

  sql_close_connection(sql_connection);

  return 0;

error:

  if (order_row != NULL) { order_row_free(order_row); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }

  return -1;
}
