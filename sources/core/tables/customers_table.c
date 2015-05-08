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

  return 1;
}

// selects all the customer rows
int customers_table_select_all(sqlite3 *sql_connection, customer_row_t ***customer_rows, int *count)
{
  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_rows != NULL, "customer_row:s NULL");

  sqlite3_stmt *sql_statement = sql_prepare_statement(sql_connection,
    "SELECT "
    "\"customer-id\", "
    "\"first-name\", "
    "\"last-name\", "
    "\"address\", "
    "\"city\", "
    "\"state\", "
    "\"zip\" "
    "FROM \"customers\";");

  check(sql_statement != NULL, "sql_statement: NULL");

  int allocated_customer_rows_count = 10;
  customer_row_t **allocated_customer_rows = customer_rows_realloc(NULL, allocated_customer_rows_count);
  check_mem(allocated_customer_rows);

  int read_customer_rows_count = 0;

  int sql_step_read_status = 0;
  int sql_step_read_result = sql_step_read(sql_statement, &sql_step_read_status);
  check(sql_step_read_result == 0, "sql_step_read_result: %d",
    sql_step_read_result);

  while (sql_step_read_status == SQLITE_ROW)
  {
    int customers_table_read_result = customers_table_read(sql_statement, &(allocated_customer_rows[read_customer_rows_count]));
    check(customers_table_read_result == 0, "customers_table_read_result: %d",
      customers_table_read_result);

    read_customer_rows_count++;

    if (read_customer_rows_count == allocated_customer_rows_count)
    {
      allocated_customer_rows_count *= 2;
      allocated_customer_rows = customer_rows_realloc(allocated_customer_rows, allocated_customer_rows_count);
      check_mem(allocated_customer_rows);
    }

    sql_step_read_result = sql_step_read(sql_statement, &sql_step_read_status);
    check(sql_step_read_result == 0, "sql_step_read_result: %d",
      sql_step_read_result);
  }

  allocated_customer_rows = customer_rows_realloc(allocated_customer_rows, read_customer_rows_count);
  check(allocated_customer_rows != NULL || read_customer_rows_count == 0, "allocated_customer_rows: NULL");

  *customer_rows = allocated_customer_rows;
  *count = read_customer_rows_count;

  sql_finalize_statement(sql_statement);

  return 0;

error:

  sql_finalize_statement(sql_statement);

  return 1;
}

// selects a customer row by customer id
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

  int sql_bind_result = sql_bind_int(sql_statement, 1, customer_id);
  check(sql_bind_result == 0, "sql_bind_result: %d",
    sql_bind_result);

  int sql_step_read_status = 0;
  int sql_step_read_result = sql_step_read(sql_statement, &sql_step_read_status);
  check(sql_step_read_result == 0, "sql_step_read_result: %d",
    sql_step_read_result);

  customer_row_t *read_customer_row = NULL;

  if (sql_step_read_status == SQLITE_ROW)
  {
    int customers_table_read_result = customers_table_read(sql_statement, &read_customer_row);
    check(customers_table_read_result == 0, "customers_table_read_result: %d",
      customers_table_read_result);
  }

  *customer_row = read_customer_row;

  sql_finalize_statement(sql_statement);

  return 0;

error:

  sql_finalize_statement(sql_statement);

  return 1;
}
