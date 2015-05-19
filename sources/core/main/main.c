#include <stdio.h>
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

  customer_row_t **customer_rows = NULL;
  int customer_rows_count = 0;

  customers_table_select_all(sql_connection, &customer_rows, &customer_rows_count);
  check(customer_rows != NULL || customer_rows_count == 0, "customer_rows: NULL");

  for (int i = 0; i < customer_rows_count; i++)
  {
    printf("%d\n", customer_rows[i]->customer_id);
  }

  customer_rows_free(customer_rows, customer_rows_count);

  sql_close_connection(sql_connection);

  return 0;

error:

  if (customer_rows != NULL) { customer_rows_free(customer_rows, customer_rows_count); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }

  return -1;
}
