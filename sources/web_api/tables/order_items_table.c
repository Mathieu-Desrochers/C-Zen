#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../web_api/tables/order_item_row.h"
#include "../../web_api/tables/order_items_table.h"

// reads an order item row
int order_items_table_read(sqlite3_stmt *sql_statement, order_item_row_t **order_item_row)
{
  int *order_item_id = NULL;
  int *order_id = NULL;
  char *name = NULL;
  double *quantity = NULL;

  order_item_row_t *order_item_row_return = NULL;

  check_not_null(sql_statement);
  check_not_null(order_item_row);

  check_result(sql_read_int(sql_statement, 0, &order_item_id), 0);
  check_result(sql_read_int(sql_statement, 1, &order_id), 0);
  check_result(sql_read_string(sql_statement, 2, &name), 0);
  check_result(sql_read_double(sql_statement, 3, &quantity), 0);

  order_item_row_return = order_item_row_malloc(
    order_item_id,
    order_id,
    name,
    quantity);

  check_not_null(order_item_row_return);

  free(order_item_id);
  free(order_id);
  free(name);
  free(quantity);

  *order_item_row = order_item_row_return;

  return 0;

error:

  if (order_item_id != NULL) { free(order_item_id); }
  if (order_id != NULL) { free(order_id); }
  if (name != NULL) { free(name); }
  if (quantity != NULL) { free(quantity); }

  if (order_item_row_return != NULL) { order_item_row_free(order_item_row_return); }

  return -1;
}

// inserts an order item row
int order_items_table_insert(sqlite3 *sql_connection, order_item_row_t *order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;
  int *order_item_id_return = NULL;

  check_not_null(sql_connection);
  check_not_null(order_item_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "INSERT INTO \"order-items\" ("
        "\"order-id\", "
        "\"name\", "
        "\"quantity\") "
      "VALUES (?1, ?2, ?3);",
      &sql_statement),
    0);

  check_result(sql_bind_int(sql_statement, 1, order_item_row->order_id), 0);
  check_result(sql_bind_string(sql_statement, 2, order_item_row->name), 0);
  check_result(sql_bind_double(sql_statement, 3, order_item_row->quantity), 0);

  check_result(sql_step_execute(sql_statement), 0);

  check_result(sql_last_generated_id(sql_connection, &order_item_id_return), 0);

  free(order_item_row->order_item_id);
  order_item_row->order_item_id = order_item_id_return;

  sql_statement_finalize(sql_statement);

  return 0;

error:

  if (order_item_id_return != NULL) { free(order_item_id_return); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// selects an order item row by order item id
int order_items_table_select_by_order_item_id(sqlite3 *sql_connection, int order_item_id, order_item_row_t **order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;
  order_item_row_t *order_item_row_return = NULL;

  check_not_null(sql_connection);
  check_not_null(order_item_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "SELECT "
        "\"order-item-id\", "
        "\"order-id\", "
        "\"name\", "
        "\"quantity\" "
      "FROM \"order-items\" "
      "WHERE \"order-item-id\" = ?1;",
      &sql_statement),
    0);

  check_result(sql_bind_int(sql_statement, 1, &order_item_id), 0);

  int is_row_available = 0;
  check_result(sql_step_select(sql_statement, &is_row_available), 0);

  if (is_row_available == 1)
  {
    check_result(order_items_table_read(sql_statement, &order_item_row_return), 0);
  }

  sql_statement_finalize(sql_statement);

  *order_item_row = order_item_row_return;

  return 0;

error:

  if (order_item_row_return != NULL) { order_item_row_free(order_item_row_return); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// selects order item rows by order id
int order_items_table_select_by_order_id(sqlite3 *sql_connection, int order_id, order_item_row_t ***order_item_rows, int *count)
{
  sqlite3_stmt *sql_statement = NULL;
  order_item_row_t **order_item_rows_return = NULL;
  order_item_row_t *order_item_row = NULL;

  check_not_null(sql_connection);
  check_not_null(order_item_rows);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "SELECT "
        "\"order-item-id\", "
        "\"order-id\", "
        "\"name\", "
        "\"quantity\" "
      "FROM \"order-items\" "
      "WHERE \"order-id\" = ?1;",
      &sql_statement),
    0);

  check_result(sql_bind_int(sql_statement, 1, &order_id), 0);

  int order_item_rows_allocated_count = 0;
  int order_item_rows_used_count = 0;

  int is_row_available = 0;
  check_result(sql_step_select(sql_statement, &is_row_available), 0);

  while (is_row_available == 1)
  {
    check_result(order_items_table_read(sql_statement, &order_item_row), 0);

    check_result(
      array_add_pointer(
        (void ***)&order_item_rows_return, &order_item_rows_allocated_count, &order_item_rows_used_count,
        (void *)order_item_row),
      0);

    order_item_row = NULL;

    check_result(sql_step_select(sql_statement, &is_row_available), 0);
  }

  sql_statement_finalize(sql_statement);

  *order_item_rows = order_item_rows_return;
  *count = order_item_rows_used_count;

  return 0;

error:

  if (order_item_rows_return != NULL) { order_item_rows_free(order_item_rows_return, order_item_rows_used_count); }
  if (order_item_row != NULL) { order_item_row_free(order_item_row); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// selects all the order item rows
int order_items_table_select_all(sqlite3 *sql_connection, order_item_row_t ***order_item_rows, int *count)
{
  sqlite3_stmt *sql_statement = NULL;
  order_item_row_t **order_item_rows_return = NULL;
  order_item_row_t *order_item_row = NULL;

  check_not_null(sql_connection);
  check_not_null(order_item_rows);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "SELECT "
        "\"order-item-id\", "
        "\"order-id\", "
        "\"name\", "
        "\"quantity\" "
      "FROM \"order-items\";",
      &sql_statement),
    0);

  int order_item_rows_allocated_count = 0;
  int order_item_rows_used_count = 0;

  int is_row_available = 0;
  check_result(sql_step_select(sql_statement, &is_row_available), 0);

  while (is_row_available == 1)
  {
    check_result(order_items_table_read(sql_statement, &order_item_row), 0);

    check_result(
      array_add_pointer(
        (void ***)&order_item_rows_return, &order_item_rows_allocated_count, &order_item_rows_used_count,
        (void *)order_item_row),
      0);

    order_item_row = NULL;

    check_result(sql_step_select(sql_statement, &is_row_available), 0);
  }

  sql_statement_finalize(sql_statement);

  *order_item_rows = order_item_rows_return;
  *count = order_item_rows_used_count;

  return 0;

error:

  if (order_item_rows_return != NULL) { order_item_rows_free(order_item_rows_return, order_item_rows_used_count); }
  if (order_item_row != NULL) { order_item_row_free(order_item_row); }
  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// updates an order item row
int order_items_table_update(sqlite3 *sql_connection, order_item_row_t *order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);
  check_not_null(order_item_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "UPDATE \"order-items\" SET "
        "\"order-id\" = ?1, "
        "\"name\" = ?2, "
        "\"quantity\" = ?3 "
      "WHERE \"order-item-id\" = ?4;",
      &sql_statement),
    0);

  check_result(sql_bind_int(sql_statement, 1, order_item_row->order_id), 0);
  check_result(sql_bind_string(sql_statement, 2, order_item_row->name), 0);
  check_result(sql_bind_double(sql_statement, 3, order_item_row->quantity), 0);
  check_result(sql_bind_int(sql_statement, 4, order_item_row->order_item_id), 0);

  check_result(sql_step_execute(sql_statement), 0);

  sql_statement_finalize(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}

// deletes an order item row
int order_items_table_delete(sqlite3 *sql_connection, order_item_row_t *order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check_not_null(sql_connection);
  check_not_null(order_item_row);

  check_result(
    sql_statement_prepare(
      sql_connection,
      "DELETE FROM \"order-items\" "
      "WHERE \"order-item-id\" = ?1;",
      &sql_statement),
    0);

  check_result(sql_bind_int(sql_statement, 1, order_item_row->order_item_id), 0);

  check_result(sql_step_execute(sql_statement), 0);

  sql_statement_finalize(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}
