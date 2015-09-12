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
  time_t *shipping_date = NULL;
  time_t *shipping_time_before = NULL;
  time_t *shipping_time_after = NULL;

  order_item_row_t *order_item_row_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_read_order_item_id_result = sql_read_int(sql_statement, 0, &order_item_id);
  check(sql_read_order_item_id_result == 0, "sql_read_order_item_id_result: %d",
    sql_read_order_item_id_result);

  int sql_read_order_id_result = sql_read_int(sql_statement, 1, &order_id);
  check(sql_read_order_id_result == 0, "sql_read_order_id_result: %d",
    sql_read_order_id_result);

  int sql_read_name_result = sql_read_string(sql_statement, 2, &name);
  check(sql_read_name_result == 0, "sql_read_name_result: %d",
    sql_read_name_result);

  int sql_read_quantity_result = sql_read_double(sql_statement, 3, &quantity);
  check(sql_read_quantity_result == 0, "sql_read_quantity_result: %d",
    sql_read_quantity_result);

  int sql_read_shipping_date_result = sql_read_date(sql_statement, 4, &shipping_date);
  check(sql_read_shipping_date_result == 0, "sql_read_shipping_date_result: %d",
    sql_read_shipping_date_result);

  int sql_read_shipping_time_before_result = sql_read_time(sql_statement, 5, &shipping_time_before);
  check(sql_read_shipping_time_before_result == 0, "sql_read_shipping_time_before_result: %d",
    sql_read_shipping_time_before_result);

  int sql_read_shipping_time_after_result = sql_read_time(sql_statement, 6, &shipping_time_after);
  check(sql_read_shipping_time_after_result == 0, "sql_read_shipping_time_after_result: %d",
    sql_read_shipping_time_after_result);

  order_item_row_return = order_item_row_malloc(
    order_item_id,
    order_id,
    name,
    quantity,
    shipping_date,
    shipping_time_before,
    shipping_time_after);

  check(order_item_row_return != NULL, "order_item_row_return: NULL");

  free(order_item_id);
  free(order_id);
  free(name);
  free(quantity);
  free(shipping_date);
  free(shipping_time_before);
  free(shipping_time_after);

  *order_item_row = order_item_row_return;

  return 0;

error:

  if (order_item_id != NULL) { free(order_item_id); }
  if (order_id != NULL) { free(order_id); }
  if (name != NULL) { free(name); }
  if (quantity != NULL) { free(quantity); }
  if (shipping_date != NULL) { free(shipping_date); }
  if (shipping_time_before != NULL) { free(shipping_time_before); }
  if (shipping_time_after != NULL) { free(shipping_time_after); }

  if (order_item_row_return != NULL) { order_item_row_free(order_item_row_return); }

  return -1;
}

// inserts an order item row
int order_items_table_insert(sqlite3 *sql_connection, order_item_row_t *order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;
  int *order_item_id_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_statement_prepare_result = sql_statement_prepare(
    sql_connection,
    "INSERT INTO \"order-items\" ("
      "\"order-id\", "
      "\"name\", "
      "\"quantity\", "
      "\"shipping-date\", "
      "\"shipping-time-before\", "
      "\"shipping-time-after\") "
    "VALUES (?1, ?2, ?3, ?4, ?5, ?6);",
    &sql_statement);

  check(sql_statement_prepare_result == 0, "sql_statement_prepare_result: %d",
    sql_statement_prepare_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 1, order_item_row->order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int sql_bind_name_result = sql_bind_string(sql_statement, 2, order_item_row->name);
  check(sql_bind_name_result == 0, "sql_bind_name_result: %d",
    sql_bind_name_result);

  int sql_bind_quantity_result = sql_bind_double(sql_statement, 3, order_item_row->quantity);
  check(sql_bind_quantity_result == 0, "sql_bind_quantity_result: %d",
    sql_bind_quantity_result);

  int sql_bind_shipping_date_result = sql_bind_date(sql_statement, 4, order_item_row->shipping_date);
  check(sql_bind_shipping_date_result == 0, "sql_bind_shipping_date_result: %d",
    sql_bind_shipping_date_result);

  int sql_bind_shipping_time_before_result = sql_bind_time(sql_statement, 5, order_item_row->shipping_time_before);
  check(sql_bind_shipping_time_before_result == 0, "sql_bind_shipping_time_before_result: %d",
    sql_bind_shipping_time_before_result);

  int sql_bind_shipping_time_after_result = sql_bind_time(sql_statement, 6, order_item_row->shipping_time_after);
  check(sql_bind_shipping_time_after_result == 0, "sql_bind_shipping_time_after_result: %d",
    sql_bind_shipping_time_after_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  int sql_last_generated_id_result = sql_last_generated_id(sql_connection, &order_item_id_return);
  check(sql_last_generated_id_result == 0, "sql_last_generated_id_result: %d",
    sql_last_generated_id_result);

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

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_statement_prepare_result = sql_statement_prepare(
    sql_connection,
    "SELECT "
      "\"order-item-id\", "
      "\"order-id\", "
      "\"name\", "
      "\"quantity\", "
      "\"shipping-date\", "
      "\"shipping-time-before\", "
      "\"shipping-time-after\" "
    "FROM \"order-items\" "
    "WHERE \"order-item-id\" = ?1;",
    &sql_statement);

  check(sql_statement_prepare_result == 0, "sql_statement_prepare_result: %d",
    sql_statement_prepare_result);

  int sql_bind_order_item_id_result = sql_bind_int(sql_statement, 1, &order_item_id);
  check(sql_bind_order_item_id_result == 0, "sql_bind_order_item_id_result: %d",
    sql_bind_order_item_id_result);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  if (is_row_available == 1)
  {
    int order_items_table_read_result = order_items_table_read(sql_statement, &order_item_row_return);
    check(order_items_table_read_result == 0, "order_items_table_read_result: %d",
      order_items_table_read_result);
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

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_rows != NULL, "order_item_rows: NULL");

  int sql_statement_prepare_result = sql_statement_prepare(
    sql_connection,
    "SELECT "
      "\"order-item-id\", "
      "\"order-id\", "
      "\"name\", "
      "\"quantity\", "
      "\"shipping-date\", "
      "\"shipping-time-before\", "
      "\"shipping-time-after\" "
    "FROM \"order-items\" "
    "WHERE \"order-id\" = ?1;",
    &sql_statement);

  check(sql_statement_prepare_result == 0, "sql_statement_prepare_result: %d",
    sql_statement_prepare_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 1, &order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int order_item_rows_allocated_count = 0;
  int order_item_rows_used_count = 0;

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (is_row_available == 1)
  {
    int order_items_table_read_result = order_items_table_read(sql_statement, &order_item_row);
    check(order_items_table_read_result == 0, "order_items_table_read_result: %d",
      order_items_table_read_result);

    int array_add_result = array_add_pointer(
      (void ***)&order_item_rows_return, &order_item_rows_allocated_count, &order_item_rows_used_count,
      (void *)order_item_row);

    check(array_add_result == 0, "array_add_result: %d",
      array_add_result);

    order_item_row = NULL;

    sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
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

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_rows != NULL, "order_item_rows: NULL");

  int sql_statement_prepare_result = sql_statement_prepare(
    sql_connection,
    "SELECT "
      "\"order-item-id\", "
      "\"order-id\", "
      "\"name\", "
      "\"quantity\", "
      "\"shipping-date\", "
      "\"shipping-time-before\", "
      "\"shipping-time-after\" "
    "FROM \"order-items\";",
    &sql_statement);

  check(sql_statement_prepare_result == 0, "sql_statement_prepare_result: %d",
    sql_statement_prepare_result);

  int order_item_rows_allocated_count = 0;
  int order_item_rows_used_count = 0;

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (is_row_available == 1)
  {
    int order_items_table_read_result = order_items_table_read(sql_statement, &order_item_row);
    check(order_items_table_read_result == 0, "order_items_table_read_result: %d",
      order_items_table_read_result);

    int array_add_result = array_add_pointer(
      (void ***)&order_item_rows_return, &order_item_rows_allocated_count, &order_item_rows_used_count,
      (void *)order_item_row);

    check(array_add_result == 0, "array_add_result: %d",
      array_add_result);

    order_item_row = NULL;

    sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
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

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_statement_prepare_result = sql_statement_prepare(
    sql_connection,
    "UPDATE \"order-items\" SET "
      "\"order-id\" = ?1, "
      "\"name\" = ?2, "
      "\"quantity\" = ?3, "
      "\"shipping-date\" = ?4, "
      "\"shipping-time-before\" = ?5, "
      "\"shipping-time-after\" = ?6 "
    "WHERE \"order-item-id\" = ?7;",
    &sql_statement);

  check(sql_statement_prepare_result == 0, "sql_statement_prepare_result: %d",
    sql_statement_prepare_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 1, order_item_row->order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int sql_bind_name_result = sql_bind_string(sql_statement, 2, order_item_row->name);
  check(sql_bind_name_result == 0, "sql_bind_name_result: %d",
    sql_bind_name_result);

  int sql_bind_quantity_result = sql_bind_double(sql_statement, 3, order_item_row->quantity);
  check(sql_bind_quantity_result == 0, "sql_bind_quantity_result: %d",
    sql_bind_quantity_result);

  int sql_bind_shipping_date_result = sql_bind_date(sql_statement, 4, order_item_row->shipping_date);
  check(sql_bind_shipping_date_result == 0, "sql_bind_shipping_date_result: %d",
    sql_bind_shipping_date_result);

  int sql_bind_shipping_time_before_result = sql_bind_time(sql_statement, 5, order_item_row->shipping_time_before);
  check(sql_bind_shipping_time_before_result == 0, "sql_bind_shipping_time_before_result: %d",
    sql_bind_shipping_time_before_result);

  int sql_bind_shipping_time_after_result = sql_bind_time(sql_statement, 6, order_item_row->shipping_time_after);
  check(sql_bind_shipping_time_after_result == 0, "sql_bind_shipping_time_after_result: %d",
    sql_bind_shipping_time_after_result);

  int sql_bind_order_item_id_result = sql_bind_int(sql_statement, 7, order_item_row->order_item_id);
  check(sql_bind_order_item_id_result == 0, "sql_bind_order_item_id_result: %d",
    sql_bind_order_item_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

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

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_statement_prepare_result = sql_statement_prepare(
    sql_connection,
    "DELETE FROM \"order-items\" "
    "WHERE \"order-item-id\" = ?1;",
    &sql_statement);

  check(sql_statement_prepare_result == 0, "sql_statement_prepare_result: %d",
    sql_statement_prepare_result);

  int sql_bind_order_item_id_result = sql_bind_int(sql_statement, 1, order_item_row->order_item_id);
  check(sql_bind_order_item_id_result == 0, "sql_bind_order_item_id_result: %d",
    sql_bind_order_item_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_statement_finalize(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_statement_finalize(sql_statement); }

  return -1;
}