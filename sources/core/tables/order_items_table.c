#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>

#include "../../core/tables/order_item_row.h"
#include "../../core/tables/order_items_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

// reads an order item row
int order_items_table_read(sqlite3_stmt *sql_statement, order_item_row_t **order_item_row)
{
  char *name = NULL;

  order_item_row_t *order_item_row_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int order_item_id;
  int sql_read_order_item_id_result = sql_read_int(sql_statement, 0, &order_item_id);
  check(sql_read_order_item_id_result == 0, "sql_read_order_item_id_result: %d",
    sql_read_order_item_id_result);

  int order_id;
  int sql_read_order_id_result = sql_read_int(sql_statement, 1, &order_id);
  check(sql_read_order_id_result == 0, "sql_read_order_id_result: %d",
    sql_read_order_id_result);

  int sql_read_name_result = sql_read_string(sql_statement, 2, &name);
  check(sql_read_name_result == 0, "sql_read_name_result: %d",
    sql_read_name_result);

  double quantity;
  int sql_read_quantity_result = sql_read_double(sql_statement, 3, &quantity);
  check(sql_read_quantity_result == 0, "sql_read_quantity_result: %d",
    sql_read_quantity_result);

  time_t shipping_date;
  int sql_read_shipping_date_result = sql_read_date(sql_statement, 4, &shipping_date);
  check(sql_read_shipping_date_result == 0, "sql_read_shipping_date_result: %d",
    sql_read_shipping_date_result);

  time_t shipping_time_before;
  int sql_read_shipping_time_before_result = sql_read_time(sql_statement, 5, &shipping_time_before);
  check(sql_read_shipping_time_before_result == 0, "sql_read_shipping_time_before_result: %d",
    sql_read_shipping_time_before_result);

  time_t shipping_time_after;
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

  free(name);

  *order_item_row = order_item_row_return;

  return 0;

error:

  if (name != NULL) { free(name); }

  if (order_item_row_return != NULL) { order_item_row_free(order_item_row_return); }

  return -1;
}

// inserts an order item row
int order_items_table_insert(sqlite3 *sql_connection, order_item_row_t *order_item_row)
{
  sqlite3_stmt *sql_insert_statement = NULL;
  sqlite3_stmt *sql_select_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_prepare_insert_statement_result = sql_prepare_statement(
    sql_connection,
    "INSERT INTO \"order-items\" ("
      "\"order-id\", "
      "\"name\", "
      "\"quantity\", "
      "\"shipping-date\", "
      "\"shipping-time-before\", "
      "\"shipping-time-after\") "
    "VALUES (?1, ?2, ?3, ?4, ?5, ?6);",
    &sql_insert_statement);

  check(sql_prepare_insert_statement_result == 0, "sql_prepare_insert_statement_result: %d",
    sql_prepare_insert_statement_result);

  int sql_bind_order_id_result = sql_bind_int(sql_insert_statement, 1, order_item_row->order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int sql_bind_name_result = sql_bind_string(sql_insert_statement, 2, order_item_row->name);
  check(sql_bind_name_result == 0, "sql_bind_name_result: %d",
    sql_bind_name_result);

  int sql_bind_quantity_result = sql_bind_double(sql_insert_statement, 3, order_item_row->quantity);
  check(sql_bind_quantity_result == 0, "sql_bind_quantity_result: %d",
    sql_bind_quantity_result);

  int sql_bind_shipping_date_result = sql_bind_date(sql_insert_statement, 4, order_item_row->shipping_date);
  check(sql_bind_shipping_date_result == 0, "sql_bind_shipping_date_result: %d",
    sql_bind_shipping_date_result);

  int sql_bind_shipping_time_before_result = sql_bind_time(sql_insert_statement, 5, order_item_row->shipping_time_before);
  check(sql_bind_shipping_time_before_result == 0, "sql_bind_shipping_time_before_result: %d",
    sql_bind_shipping_time_before_result);

  int sql_bind_shipping_time_after_result = sql_bind_time(sql_insert_statement, 6, order_item_row->shipping_time_after);
  check(sql_bind_shipping_time_after_result == 0, "sql_bind_shipping_time_after_result: %d",
    sql_bind_shipping_time_after_result);

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

  order_item_row->order_item_id = sqlite3_column_int(sql_select_statement, 0);

  sql_finalize_statement(sql_insert_statement);
  sql_finalize_statement(sql_select_statement);

  return 0;

error:

  if (sql_insert_statement != NULL) { sql_finalize_statement(sql_insert_statement); }
  if (sql_select_statement != NULL) { sql_finalize_statement(sql_select_statement); }

  return -1;
}

// selects an order item row by order item id
int order_items_table_select_by_order_item_id(sqlite3 *sql_connection, int order_item_id, order_item_row_t **order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;
  order_item_row_t *order_item_row_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
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

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_order_item_id_result = sql_bind_int(sql_statement, 1, order_item_id);
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

  sql_finalize_statement(sql_statement);

  *order_item_row = order_item_row_return;

  return 0;

error:

  if (order_item_row_return != NULL) { order_item_row_free(order_item_row_return); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// selects all the order item rows
int order_items_table_select_all(sqlite3 *sql_connection, order_item_row_t ***order_item_rows, int *count)
{
  sqlite3_stmt *sql_statement = NULL;
  order_item_row_t **allocated_order_item_rows = NULL;
  order_item_row_t **reallocated_order_item_rows = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_rows != NULL, "order_item_rows: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
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

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int read_order_item_rows_count = 0;

  int allocated_order_item_rows_count = 4;
  allocated_order_item_rows = malloc(sizeof(order_item_row_t *) * allocated_order_item_rows_count);
  check_mem(allocated_order_item_rows);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (is_row_available == 1)
  {
    order_item_row_t **order_item_row = &(allocated_order_item_rows[read_order_item_rows_count]);

    int order_items_table_read_result = order_items_table_read(sql_statement, order_item_row);
    check(order_items_table_read_result == 0, "order_items_table_read_result: %d",
      order_items_table_read_result);

    read_order_item_rows_count++;

    if (read_order_item_rows_count == allocated_order_item_rows_count)
    {
      allocated_order_item_rows_count *= 2;
      reallocated_order_item_rows = realloc(allocated_order_item_rows, sizeof(order_item_row_t *) * allocated_order_item_rows_count);
      check_mem(reallocated_order_item_rows);
      allocated_order_item_rows = reallocated_order_item_rows;
    }

    sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
  }

  reallocated_order_item_rows = realloc(allocated_order_item_rows, sizeof(order_item_row_t *) * read_order_item_rows_count);
  check_mem(reallocated_order_item_rows);
  allocated_order_item_rows = reallocated_order_item_rows;

  sql_finalize_statement(sql_statement);

  *order_item_rows = allocated_order_item_rows;
  *count = read_order_item_rows_count;

  return 0;

error:

  if (allocated_order_item_rows != NULL) { order_item_rows_free(allocated_order_item_rows, read_order_item_rows_count); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// updates an order item row
int order_items_table_update(sqlite3 *sql_connection, order_item_row_t *order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
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

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

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

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// deletes an order item row
int order_items_table_delete(sqlite3 *sql_connection, order_item_row_t *order_item_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_row != NULL, "order_item_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "DELETE FROM \"order-items\" "
    "WHERE \"order-item-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_order_item_id_result = sql_bind_int(sql_statement, 1, order_item_row->order_item_id);
  check(sql_bind_order_item_id_result == 0, "sql_bind_order_item_id_result: %d",
    sql_bind_order_item_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}
