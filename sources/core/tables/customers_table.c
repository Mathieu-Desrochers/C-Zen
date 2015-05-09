#include <stddef.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

// reads a customer_row
int customers_table_read(sqlite3_stmt *sql_statement, void **customer_row)
{
  customer_row_t *allocated_customer_row = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  allocated_customer_row = customer_row_malloc(
    sqlite3_column_int(sql_statement, 0),
    (char*)sqlite3_column_text(sql_statement, 1),
    (char*)sqlite3_column_text(sql_statement, 2),
    (char*)sqlite3_column_text(sql_statement, 3),
    (char*)sqlite3_column_text(sql_statement, 4),
    (char*)sqlite3_column_text(sql_statement, 5),
    (char*)sqlite3_column_text(sql_statement, 6));

  check(allocated_customer_row != NULL, "allocated_customer_row: NULL");

  *customer_row = allocated_customer_row;

  return 0;

error:

  if (allocated_customer_row != NULL) { customer_row_free(allocated_customer_row); }
  if (allocated_customer_row != NULL) { allocated_customer_row = NULL; }

  return -1;
}

// frees a customer row
void customers_table_free_row(void *customer_rows)
{
  customer_row_free((customer_row_t *)customer_rows);
}

// frees an array of customer rows
void customers_table_free_rows(void **customer_rows, int count)
{
  customer_rows_free((customer_row_t **)customer_rows, count);
}

// binds the parameters when
// selecting a customer row by customer id
int customers_table_select_by_customer_id_bind(sqlite3_stmt *sql_statement, void **parameters)
{
  int customer_id = *((int *)parameters[0]);

  int sql_bind_result = sql_bind_int(sql_statement, 1, customer_id);
  check(sql_bind_result == 0, "sql_bind_result: %d",
    sql_bind_result);

  return 0;

error:

  return -1;
}

// selects a customer row by customer id
int customers_table_select_by_customer_id(sqlite3 *sql_connection, int customer_id, customer_row_t **customer_row)
{
  customer_row_t *selected_row = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  void *parameters[1] =
  {
    &customer_id
  };

  int sql_select_one_result = sql_select_one(
    sql_connection,
    "SELECT "
      "\"customer-id\", "
      "\"first-name\", "
      "\"last-name\", "
      "\"address\", "
      "\"city\", "
      "\"state\", "
      "\"zip\" "
    "FROM \"customers\" "
    "WHERE \"customer-id\" = ?1;",
    parameters,
    &customers_table_select_by_customer_id_bind,
    &customers_table_read,
    &customers_table_free_row,
    (void **)&selected_row);

  check(sql_select_one_result == 0, "sql_select_one_result: %d",
    sql_select_one_result);

  *customer_row = selected_row;

  return 0;

error:

  if (selected_row != NULL) { customer_row_free(selected_row); }

  return -1;
}

// binds the parameters when
// selecting all the customer rows
int customers_table_select_all_bind(sqlite3_stmt *sql_statement, void **parameters)
{
  return 0;
}

// selects all the customer rows
int customers_table_select_all(sqlite3 *sql_connection, customer_row_t ***customer_rows, int *count)
{
  customer_row_t **selected_rows = NULL;
  int selected_rows_count = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_rows != NULL, "customer_rows: NULL");
  check(count != NULL, "count: NULL");

  void *parameters[0] = {};

  int sql_select_many_result = sql_select_many(
    sql_connection,
    "SELECT "
      "\"customer-id\", "
      "\"first-name\", "
      "\"last-name\", "
      "\"address\", "
      "\"city\", "
      "\"state\", "
      "\"zip\" "
    "FROM \"customers\";",
    parameters,
    &customers_table_select_all_bind,
    &customers_table_read,
    &customers_table_free_rows,
    (void ***)&selected_rows,
    &selected_rows_count);

  check(sql_select_many_result == 0, "sql_select_many_result: %d",
    sql_select_many_result);

  *customer_rows = selected_rows;
  *count = selected_rows_count;

  return 0;

error:

  if (selected_rows != NULL) { customer_rows_free(selected_rows, selected_rows_count); }

  return -1;
}
