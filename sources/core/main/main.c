#include <stdio.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

int main()
{
  sqlite3 *sql_connection = NULL;
  customer_row_t **customer_rows = NULL;
  int customer_rows_count = 0;

  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

//  int customers_table_select_by_customer_id_result = customers_table_select_by_customer_id(
//    sql_connection, 3, &customer_row);
//
//  check(customers_table_select_by_customer_id_result == 0, "customers_table_select_by_customer_id_result: %d",
//    customers_table_select_by_customer_id_result);
//
//  if (customer_row != NULL)
//  {
//    puts(customer_row->first_name);
//    customer_row_free(customer_row);
//  }

  int customers_table_select_all_result = customers_table_select_all(sql_connection, &customer_rows, &customer_rows_count);
  check(customers_table_select_all_result == 0, "customers_table_select_all_result: %d",
    customers_table_select_all_result);

  printf("%d\n", customer_rows_count);
  customer_rows_free(customer_rows, customer_rows_count);

  sql_close_connection(sql_connection);

  return 0;

error:

  if (customer_rows != NULL) { customer_rows_free(customer_rows, customer_rows_count); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }

  return -1;
}
