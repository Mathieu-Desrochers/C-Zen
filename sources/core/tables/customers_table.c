#include <stddef.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

// reads a customer_row
int customers_table_read(sqlite3_stmt *sql_statement, customer_row_t **customer_row)
{
  check(sql_statement != NULL, "customers_table_read | sql_statement == NULL");
  check(customer_row != NULL, "customers_table_read | customer_row == NULL");

  *customer_row = customer_row_malloc(
    sqlite3_column_int(sql_statement, 0),
    (char*)sqlite3_column_text(sql_statement, 1),
    (char*)sqlite3_column_text(sql_statement, 2),
    (char*)sqlite3_column_text(sql_statement, 3),
    (char*)sqlite3_column_text(sql_statement, 4),
    (char*)sqlite3_column_text(sql_statement, 5),
    (char*)sqlite3_column_text(sql_statement, 6));

  check(*customer_row != NULL, "customers_table_read | *customer_row == NULL");

  return 0;

error:

  return -1;
}

// selects a customer_row by customer id
int customers_table_select_by_customer_id(sqlite3 *sql_connection, int customer_id, customer_row_t **customer_row)
{
  check(sql_connection != NULL, "customers_table_select_by_customer_id | sql_connection == NULL");
  check(customer_row != NULL, "customers_table_select_by_customer_id | customer_row == NULL");

  sqlite3_stmt *sql_statement = sql_prepare_statement(sql_connection,
    "SELECT * "
    "FROM customers "
    "WHERE \"customer-id\" = ?1;");

  check(sql_statement != NULL, "customers_table_select_by_customer_id | sql_statement == NULL");

  int sql_bind_result = sql_bind_int(sql_statement, 1, customer_id);
  check(sql_bind_result == 0, "customers_table_select_by_customer_id | sql_bind_result: %d", sql_bind_result);

  int sql_step_result = sql_step(sql_statement);

  if (sql_step_result == SQLITE_DONE)
  {
    *customer_row = NULL;
  }
  else if (sql_step_result == SQLITE_ROW)
  {
    int read_result = customers_table_read(sql_statement, customer_row);
    check(read_result == 0, "customers_table_select_by_customer_id | read_result: %d", read_result);
  }
  else
  {
    sentinel("customers_table_select_by_customer_id | sql_step_result: %d", sql_step_result);
  }

  sql_finalize_statement(sql_statement);

  return 0;

error:

  sql_finalize_statement(sql_statement);

  return -1;
}
