#include <stddef.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

// reads a customer_row
int customers_table_read(sqlite3_stmt *sql_statement, customer_row_t **customer_row)
{
  check(sql_statement != NULL, "sql_statement: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  *customer_row = customer_row_malloc(
    sqlite3_column_int(sql_statement, 0),
    (char*)sqlite3_column_text(sql_statement, 1),
    (char*)sqlite3_column_text(sql_statement, 2),
    (char*)sqlite3_column_text(sql_statement, 3),
    (char*)sqlite3_column_text(sql_statement, 4),
    (char*)sqlite3_column_text(sql_statement, 5),
    (char*)sqlite3_column_text(sql_statement, 6));

  check(*customer_row != NULL, "*customer_row: NULL");

  return 0;

error:

  customer_row_free(*customer_row);

  return -1;
}

// selects a customer_row by customer id
int customers_table_select_by_customer_id(sqlite3 *sql_connection, int customer_id, customer_row_t **customer_row)
{
  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  sqlite3_stmt *sql_statement = sql_prepare_statement(sql_connection,
    "SELECT "
    "\"customer-id\", "
    "\"first-name\", "
    "\"last-name\", "
    "\"address\", "
    "\"city\", "
    "\"state\", "
    "\"zip\" "
    "FROM \"customers\" "
    "WHERE \"customer-id\" = ?1;");

  check(sql_statement != NULL, "sql_statement: NULL");

  int sql_bind_result_1 = sql_bind_int(sql_statement, 1, customer_id);
  check(sql_bind_result_1 == 0, "sql_bind_result_1: %d",
    sql_bind_result_1);

  int sql_select_step_status = 0;
  int sql_select_step_result = sql_select_step(sql_statement, &sql_select_step_status);
  check(sql_select_step_result == 0, "sql_select_step_result: %d",
    sql_select_step_result);

  if (sql_select_step_status == SQLITE_ROW)
  {
    int customers_table_rea_result = customers_table_read(sql_statement, customer_row);
    check(customers_table_rea_result == 0, "customers_table_rea_result: %d",
      customers_table_rea_result);
  }
  else if (sql_select_step_status == SQLITE_DONE)
  {
    *customer_row = NULL;
  }

  sql_finalize_statement(sql_statement);

  return 0;

error:

  sql_finalize_statement(sql_statement);

  return -1;
}
