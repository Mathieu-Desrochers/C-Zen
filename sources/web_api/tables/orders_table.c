#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../web_api/tables/order_row.h"
#include "../../web_api/tables/orders_table.h"

// reads an order row
int orders_table_read(sqlite3_stmt *sql_statement, order_row_t **order_row)
{
  int *order_id = NULL;
  char *customer_name = NULL;
  time_t *placed_on_date_time = NULL;
  int *total = NULL;

  order_row_t *order_row_return = NULL;

  check_not_null(sql_statement);
  check_not_null(order_row);

  check_result(sql_read_int(sql_statement, 0, &order_id), 0);
  check_result(sql_read_string(sql_statement, 1, &customer_name), 0);
  check_result(sql_read_date_time(sql_statement, 2, &placed_on_date_time), 0);
  check_result(sql_read_int(sql_statement, 3, &total), 0);

  order_row_return = order_row_malloc(
    order_id,
    customer_name,
    placed_on_date_time,
    total);

  check_not_null(order_row_return);

  free(order_id);
  free(customer_name);
  free(placed_on_date_time);
  free(total);

  *order_row = order_row_return;

  return 0;

error:

  if (order_id != NULL) { free(order_id); }
  if (customer_name != NULL) { free(customer_name); }
  if (placed_on_date_time != NULL) { free(placed_on_date_time); }
  if (total != NULL) { free(total); }

  if (order_row_return != NULL) { order_row_free(order_row_return); }

  return -1;
}

// inserts an order row
int orders_table_insert(sqlite3 *sql_connection, order_row_t *order_row)
{
  sqlite3_stmt *sql_statement = NULL;
  int *order_id_return = NULL;

  check_not_null(sql_connection);
  check_not_null(order_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "INSERT INTO \"orders\" ("
        "\"customer-name\", "
        "\"placed-on-date-time\", "
        "\"total\") "
      "VALUES (?1, ?2, ?3);",
      &sql_statement),
    0);

  check_result(sql_bind_string(sql_statement, 1, order_row->customer_name), 0);
  check_result(sql_bind_date_time(sql_statement, 2, order_row->placed_on_date_time), 0);
  check_result(sql_bind_int(sql_statement, 3, order_row->total), 0);

  check_result(sql_step_execute(sql_statement), 0);

  check_result(sql_last_generated_id(sql_connection, &order_id_return), 0);

  free(order_row->order_id);
  order_row->order_id = order_id_return;

  sql_statement_finalize(sql_statement);

  return 0;

error:

  if (order_id_return != NULL) { free(order_id_return); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// selects an order row by order id
int orders_table_select_by_order_id(sqlite3 *sql_connection, int order_id, order_row_t **order_row)
{
  sqlite3_stmt *sql_statement = NULL;
  order_row_t *order_row_return = NULL;

  check_not_null(sql_connection);
  check_not_null(order_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "SELECT "
        "\"order-id\", "
        "\"customer-name\", "
        "\"placed-on-date-time\", "
        "\"total\" "
      "FROM \"orders\" "
      "WHERE \"order-id\" = ?1;",
      &sql_statement),
    0);

  check_result(sql_bind_int(sql_statement, 1, &order_id), 0);

  int is_row_available = 0;
  check_result(sql_step_select(sql_statement, &is_row_available), 0);

  if (is_row_available == 1)
  {
    check_result(orders_table_read(sql_statement, &order_row_return), 0);
  }

  sql_statement_finalize(sql_statement);

  *order_row = order_row_return;

  return 0;

error:

  if (order_row_return != NULL) { order_row_free(order_row_return); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// selects all the order rows
int orders_table_select_all(sqlite3 *sql_connection, order_row_t ***order_rows, int *count)
{
  sqlite3_stmt *sql_statement = NULL;
  order_row_t **order_rows_return = NULL;
  order_row_t *order_row = NULL;

  check_not_null(sql_connection);
  check_not_null(order_rows);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "SELECT "
        "\"order-id\", "
        "\"customer-name\", "
        "\"placed-on-date-time\", "
        "\"total\" "
      "FROM \"orders\";",
      &sql_statement),
    0);

  int order_rows_allocated_count = 0;
  int order_rows_used_count = 0;

  int is_row_available = 0;
  check_result(sql_step_select(sql_statement, &is_row_available), 0);

  while (is_row_available == 1)
  {
    check_result(orders_table_read(sql_statement, &order_row), 0);

    check_result(
      array_add_pointer(
        (void ***)&order_rows_return, &order_rows_allocated_count, &order_rows_used_count,
        (void *)order_row),
      0);

    order_row = NULL;

    check_result(sql_step_select(sql_statement, &is_row_available), 0);
  }

  sql_statement_finalize(sql_statement);

  *order_rows = order_rows_return;
  *count = order_rows_used_count;

  return 0;

error:

  if (order_rows_return != NULL) { order_rows_free(order_rows_return, order_rows_used_count); }
  if (order_row != NULL) { order_row_free(order_row); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// updates an order row
int orders_table_update(sqlite3 *sql_connection, order_row_t *order_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);
  check_not_null(order_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "UPDATE \"orders\" SET "
        "\"customer-name\" = ?1, "
        "\"placed-on-date-time\" = ?2, "
        "\"total\" = ?3 "
      "WHERE \"order-id\" = ?4;",
      &sql_statement),
    0);

  check_result(sql_bind_string(sql_statement, 1, order_row->customer_name), 0);
  check_result(sql_bind_date_time(sql_statement, 2, order_row->placed_on_date_time), 0);
  check_result(sql_bind_int(sql_statement, 3, order_row->total), 0);
  check_result(sql_bind_int(sql_statement, 4, order_row->order_id), 0);

  check_result(sql_step_execute(sql_statement), 0);

  sql_statement_finalize(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// deletes an order row
int orders_table_delete(sqlite3 *sql_connection, order_row_t *order_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);
  check_not_null(order_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "DELETE FROM \"orders\" "
      "WHERE \"order-id\" = ?1;",
      &sql_statement),
    0);

  check_result(sql_bind_int(sql_statement, 1, order_row->order_id), 0);

  check_result(sql_step_execute(sql_statement), 0);

  sql_statement_finalize(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}
