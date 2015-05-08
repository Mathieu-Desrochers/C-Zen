#include <stdio.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

int main()
{
  sqlite3 *sql_connection = sql_open_connection("/var/c-zen/c-zen.db");
  check(sql_connection != NULL, "sql_connection: NULL");

  customer_row_t **customer_rows = NULL;
  int customer_rows_count = 0;

  int select_result = customers_table_select_all(sql_connection, &customer_rows, &customer_rows_count);
  check(select_result == 0, "select_result: %d",
    select_result);

  printf("%d\n", customer_rows_count);
  customer_rows_free(customer_rows, customer_rows_count);

  sql_close_connection(sql_connection);

  return 0;

error:

  sql_close_connection(sql_connection);

  return -1;
}
