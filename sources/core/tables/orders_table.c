#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>

#include "../../core/tables/order_row.h"
#include "../../core/tables/orders_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

// reads an order row
int orders_table_read(sqlite3_stmt *sql_statement, order_row_t **order_row)
{
  order_row_t *order_row_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(order_row != NULL, "order_row: NULL");

  int order_id;
  int sql_read_order_id_result = sql_read_int(sql_statement, 0, &order_id);
  check(sql_read_order_id_result == 0, "sql_read_order_id_result: %d",
    sql_read_order_id_result);

  char *customer_name;
  int sql_read_customer_name_result = sql_read_string(sql_statement, 1, &customer_name);
  check(sql_read_customer_name_result == 0, "sql_read_customer_name_result: %d",
    sql_read_customer_name_result);

  time_t placed_on_date_time;
  int sql_read_placed_on_date_time_result = sql_read_date_time(sql_statement, 2, &placed_on_date_time);
  check(sql_read_placed_on_date_time_result == 0, "sql_read_placed_on_date_time_result: %d",
    sql_read_placed_on_date_time_result);

  int total;
  int sql_read_total_result = sql_read_int(sql_statement, 3, &total);
  check(sql_read_total_result == 0, "sql_read_total_result: %d",
    sql_read_total_result);

  order_row_return = order_row_malloc(
    order_id,
    customer_name,
    placed_on_date_time,
    total);

  check(order_row_return != NULL, "order_row_return: NULL");

  free(customer_name);

  *order_row = order_row_return;

  return 0;

error:

  if (customer_name != NULL) { free(customer_name); }

  if (order_row_return != NULL) { order_row_free(order_row_return); }

  return -1;
}

// inserts an order row
int orders_table_insert(sqlite3 *sql_connection, order_row_t *order_row)
{
  sqlite3_stmt *sql_insert_statement = NULL;
  sqlite3_stmt *sql_select_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_row != NULL, "order_row: NULL");

  int sql_prepare_insert_statement_result = sql_prepare_statement(
    sql_connection,
    "INSERT INTO \"orders\" ("
      "\"customer-name\", "
      "\"placed-on-date-time\", "
      "\"total\") "
    "VALUES (?1, ?2, ?3);",
    &sql_insert_statement);

  check(sql_prepare_insert_statement_result == 0, "sql_prepare_insert_statement_result: %d",
    sql_prepare_insert_statement_result);

  int sql_bind_customer_name_result = sql_bind_string(sql_insert_statement, 1, order_row->customer_name);
  check(sql_bind_customer_name_result == 0, "sql_bind_customer_name_result: %d",
    sql_bind_customer_name_result);

  int sql_bind_placed_on_date_time_result = sql_bind_date_time(sql_insert_statement, 2, order_row->placed_on_date_time);
  check(sql_bind_placed_on_date_time_result == 0, "sql_bind_placed_on_date_time_result: %d",
    sql_bind_placed_on_date_time_result);

  int sql_bind_total_result = sql_bind_int(sql_insert_statement, 3, order_row->total);
  check(sql_bind_total_result == 0, "sql_bind_total_result: %d",
    sql_bind_total_result);

  int sql_step_execute_result = sql_step_execute(sql_insert_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  int sql_prepare_select_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT last_insert_rowid();",
    &sql_select_statement);

  check(sql_prepare_select_statement_result == 0, "sql_prepare_select_statement_result: %d",
    sql_prepare_select_statement_result);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_select_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  check(is_row_available == 1, "is_row_available: %d", is_row_available);

  order_row->order_id = sqlite3_column_int(sql_select_statement, 0);

  sql_finalize_statement(sql_insert_statement);
  sql_finalize_statement(sql_select_statement);

  return 0;

error:

  if (sql_insert_statement != NULL) { sql_finalize_statement(sql_insert_statement); }
  if (sql_select_statement != NULL) { sql_finalize_statement(sql_select_statement); }

  return -1;
}

// selects an order row by order id
int orders_table_select_by_order_id(sqlite3 *sql_connection, int order_id, order_row_t **order_row)
{
  sqlite3_stmt *sql_statement = NULL;
  order_row_t *order_row_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_row != NULL, "order_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"order-id\", "
      "\"customer-name\", "
      "\"placed-on-date-time\", "
      "\"total\" "
    "FROM \"orders\" "
    "WHERE \"order-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 1, order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  if (is_row_available == 1)
  {
    int orders_table_read_result = orders_table_read(sql_statement, &order_row_return);
    check(orders_table_read_result == 0, "orders_table_read_result: %d",
      orders_table_read_result);
  }

  sql_finalize_statement(sql_statement);

  *order_row = order_row_return;

  return 0;

error:

  if (order_row_return != NULL) { order_row_free(order_row_return); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// selects all the order rows
int orders_table_select_all(sqlite3 *sql_connection, order_row_t ***order_rows, int *count)
{
  sqlite3_stmt *sql_statement = NULL;
  order_row_t **order_rows_return = NULL;
  order_row_t *order_row = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_rows != NULL, "order_rows: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"order-id\", "
      "\"customer-name\", "
      "\"placed-on-date-time\", "
      "\"total\" "
    "FROM \"orders\";",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int allocated_order_rows_count = 0;
  int read_order_rows_count = 0;

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (is_row_available == 1)
  {
    int orders_table_read_result = orders_table_read(sql_statement, &order_row);
    check(orders_table_read_result == 0, "orders_table_read_result: %d",
      orders_table_read_result);

    int order_rows_add_result = order_rows_add(
      &order_rows_return, &allocated_order_rows_count, &read_order_rows_count,
      order_row);

    check(order_rows_add_result == 0, "order_rows_add_result: %d",
      order_rows_add_result);

    sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
  }

  sql_finalize_statement(sql_statement);

  *order_rows = order_rows_return;
  *count = read_order_rows_count;

  return 0;

error:

  if (order_rows_return != NULL) { order_rows_free(order_rows_return, read_order_rows_count); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// updates an order row
int orders_table_update(sqlite3 *sql_connection, order_row_t *order_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_row != NULL, "order_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "UPDATE \"orders\" SET "
      "\"customer-name\" = ?1, "
      "\"placed-on-date-time\" = ?2, "
      "\"total\" = ?3 "
    "WHERE \"order-id\" = ?4;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_customer_name_result = sql_bind_string(sql_statement, 1, order_row->customer_name);
  check(sql_bind_customer_name_result == 0, "sql_bind_customer_name_result: %d",
    sql_bind_customer_name_result);

  int sql_bind_placed_on_date_time_result = sql_bind_date_time(sql_statement, 2, order_row->placed_on_date_time);
  check(sql_bind_placed_on_date_time_result == 0, "sql_bind_placed_on_date_time_result: %d",
    sql_bind_placed_on_date_time_result);

  int sql_bind_total_result = sql_bind_int(sql_statement, 3, order_row->total);
  check(sql_bind_total_result == 0, "sql_bind_total_result: %d",
    sql_bind_total_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 4, order_row->order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// deletes an order row
int orders_table_delete(sqlite3 *sql_connection, order_row_t *order_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_row != NULL, "order_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "DELETE FROM \"orders\" "
    "WHERE \"order-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 1, order_row->order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}
