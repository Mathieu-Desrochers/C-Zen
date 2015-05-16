#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/order_row.h"
#include "../../core/tables/orders_table.h"

// reads an order row
int orders_table_read(sqlite3_stmt *sql_statement, order_row_t **order_row)
{
  order_row_t *order_row_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(order_row != NULL, "order_row: NULL");

  order_row_return = order_row_malloc(
    sqlite3_column_int(sql_statement, 0),
    sqlite3_column_int(sql_statement, 1),
    sqlite3_column_int(sql_statement, 2),
    sql_read_date_time(sql_statement, 3),
    sql_read_date(sql_statement, 4));

  check(order_row_return != NULL, "order_row_return: NULL");

  *order_row = order_row_return;

  return 0;

error:

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
      "\"customer-id\", "
      "\"total\", "
      "\"placed-on-date-time\", "
      "\"shipped-on-date\") "
    "VALUES (?1, ?2, ?3, ?4);",
    &sql_insert_statement);

  check(sql_prepare_insert_statement_result == 0, "sql_prepare_insert_statement_result: %d",
    sql_prepare_insert_statement_result);

  int sql_bind_customer_id_result = sql_bind_int(sql_insert_statement, 1, order_row->customer_id);
  check(sql_bind_customer_id_result == 0, "sql_bind_customer_id_result: %d",
    sql_bind_customer_id_result);

  int sql_bind_total_result = sql_bind_int(sql_insert_statement, 2, order_row->total);
  check(sql_bind_total_result == 0, "sql_bind_total_result: %d",
    sql_bind_total_result);

  int sql_bind_placed_on_date_time_result = sql_bind_date_time(sql_insert_statement, 3, order_row->placed_on_date_time);
  check(sql_bind_placed_on_date_time_result == 0, "sql_bind_placed_on_date_time_result: %d",
    sql_bind_placed_on_date_time_result);

  int sql_bind_shipped_on_date_result = sql_bind_date(sql_insert_statement, 4, order_row->shipped_on_date);
  check(sql_bind_shipped_on_date_result == 0, "sql_bind_shipped_on_date_result: %d",
    sql_bind_shipped_on_date_result);

  int sql_step_execute_result = sql_step_execute(sql_insert_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  int sql_prepare_select_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT last_insert_rowid();",
    &sql_select_statement);

  check(sql_prepare_select_statement_result == 0, "sql_prepare_select_statement_result: %d",
    sql_prepare_select_statement_result);

  int row_available = 0;
  int sql_step_select_result = sql_step_select(sql_select_statement, &row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  check(row_available == 1, "row_available: %d", row_available);

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
      "\"customer-id\", "
      "\"total\", "
      "\"placed-on-date-time\", "
      "\"shipped-on-date\" "
    "FROM \"orders\" "
    "WHERE \"order-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 1, order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  if (row_available == 1)
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
  order_row_t **allocated_order_rows = NULL;
  int used_order_rows_count = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_rows != NULL, "order_rows: NULL");
  check(count != NULL, "count: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"order-id\", "
      "\"customer-id\", "
      "\"total\", "
      "\"placed-on-date-time\", "
      "\"shipped-on-date\" "
    "FROM \"orders\";",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int allocated_order_rows_count = 10;
  allocated_order_rows = realloc(allocated_order_rows, sizeof(order_row_t *) * allocated_order_rows_count);
  check_mem(allocated_order_rows);

  int row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (row_available == 1)
  {
    order_row_t **order_row = &(allocated_order_rows[used_order_rows_count]);

    int orders_table_read_result = orders_table_read(sql_statement, order_row);
    check(orders_table_read_result == 0, "orders_table_read_result: %d",
      orders_table_read_result);

    used_order_rows_count++;

    if (used_order_rows_count == allocated_order_rows_count)
    {
      allocated_order_rows_count *= 2;
      allocated_order_rows = realloc(allocated_order_rows, sizeof(order_row_t *) * allocated_order_rows_count);
      check_mem(allocated_order_rows);
    }

    sql_step_select_result = sql_step_select(sql_statement, &row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
  }

  allocated_order_rows = realloc(allocated_order_rows, sizeof(order_row_t *) * used_order_rows_count);
  check(allocated_order_rows != NULL || used_order_rows_count == 0,
    "allocated_order_rows: NULL");

  sql_finalize_statement(sql_statement);

  *order_rows = allocated_order_rows;
  *count = used_order_rows_count;

  return 0;

error:

  if (allocated_order_rows != NULL) { order_rows_free(allocated_order_rows, used_order_rows_count); }
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
      "\"customer-id\" = ?1, "
      "\"total\" = ?2, "
      "\"placed-on-date-time\" = ?3, "
      "\"shipped-on-date\" = ?4 "
    "WHERE \"order-id\" = ?5;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_customer_id_result = sql_bind_int(sql_statement, 1, order_row->customer_id);
  check(sql_bind_customer_id_result == 0, "sql_bind_customer_id_result: %d",
    sql_bind_customer_id_result);

  int sql_bind_total_result = sql_bind_int(sql_statement, 2, order_row->total);
  check(sql_bind_total_result == 0, "sql_bind_total_result: %d",
    sql_bind_total_result);

  int sql_bind_placed_on_date_time_result = sql_bind_date_time(sql_statement, 3, order_row->placed_on_date_time);
  check(sql_bind_placed_on_date_time_result == 0, "sql_bind_placed_on_date_time_result: %d",
    sql_bind_placed_on_date_time_result);

  int sql_bind_shipped_on_date_result = sql_bind_date(sql_statement, 4, order_row->shipped_on_date);
  check(sql_bind_shipped_on_date_result == 0, "sql_bind_shipped_on_date_result: %d",
    sql_bind_shipped_on_date_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 5, order_row->order_id);
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