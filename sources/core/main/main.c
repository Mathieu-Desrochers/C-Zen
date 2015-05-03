#include <stdio.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

int main()
{
  sqlite3 *sql_connection = sql_open_connection("/var/c-zen/c-zen.db");
  check(sql_connection != NULL, "main | sql_connection == NULL");

  customer_row_t *customer_row = NULL;
  int select_result = customers_table_select_by_customer_id(sql_connection, 1, &customer_row);
  check(select_result == 0, "main | select_result: %d", select_result);

  if (customer_row != NULL)
  {
    printf("%s\n", customer_row->first_name);
    customer_row_free(customer_row);
  }

  sql_close_connection(sql_connection);

  return 0;

error:

  sql_close_connection(sql_connection);

  return -1;
}
