#include <stdlib.h>
#include <time.h>

#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  sqlite3 *sql_connection;
  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  customer_row_t *customer_row;
  int select_result = customers_table_select_by_customer_id(sql_connection, 1, &customer_row);
  check(select_result == 0, "select_result: %d",
    select_result);

  printf("%d\n", customer_row->customer_id);
  printf("%s\n", customer_row->first_name);
  printf("%s\n", customer_row->last_name);
  printf("%s\n", customer_row->address);
  printf("%s\n", customer_row->city);
  printf("%s\n", customer_row->state);
  printf("%s\n", customer_row->zip);

  customer_row_free(customer_row);

  sql_close_connection(sql_connection);

  return 0;

error:

  if (customer_row != NULL) { customer_row_free(customer_row); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }

  return -1;
}
