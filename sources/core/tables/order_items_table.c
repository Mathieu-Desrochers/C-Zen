#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/order_item_row.h"
#include "../../core/tables/order_items_table.h"

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

  int quantity;
  int sql_read_quantity_result = sql_read_int(sql_statement, 3, &quantity);
  check(sql_read_quantity_result == 0, "sql_read_quantity_result: %d",
    sql_read_quantity_result);

  order_item_row_return = order_item_row_malloc(
    order_item_id,
    order_id,
    name,
    quantity);

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
      "\"quantity\") "
    "VALUES (?1, ?2, ?3);",
    &sql_insert_statement);

  check(sql_prepare_insert_statement_result == 0, "sql_prepare_insert_statement_result: %d",
    sql_prepare_insert_statement_result);

  int sql_bind_order_id_result = sql_bind_int(sql_insert_statement, 1, order_item_row->order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int sql_bind_name_result = sql_bind_string(sql_insert_statement, 2, order_item_row->name);
  check(sql_bind_name_result == 0, "sql_bind_name_result: %d",
    sql_bind_name_result);

  int sql_bind_quantity_result = sql_bind_int(sql_insert_statement, 3, order_item_row->quantity);
  check(sql_bind_quantity_result == 0, "sql_bind_quantity_result: %d",
    sql_bind_quantity_result);

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

// selects an order item row by order_item id
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
      "\"quantity\" "
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
  order_item_row_t **order_item_rows_return = NULL;
  int count_return = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(order_item_rows != NULL, "order_item_rows: NULL");
  check(count != NULL, "count: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"order-item-id\", "
      "\"order-id\", "
      "\"name\", "
      "\"quantity\" "
    "FROM \"order-items\";",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int allocated_order_item_rows_count = 4;
  order_item_rows_return = order_item_rows_realloc(order_item_rows_return, allocated_order_item_rows_count);
  check(order_item_rows_return != NULL, "order_item_rows_return: NULL");

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (is_row_available == 1)
  {
    order_item_row_t **order_item_row = &(order_item_rows_return[count_return]);

    int order_items_table_read_result = order_items_table_read(sql_statement, order_item_row);
    check(order_items_table_read_result == 0, "order_items_table_read_result: %d",
      order_items_table_read_result);

    count_return++;

    if (count_return == allocated_order_item_rows_count)
    {
      allocated_order_item_rows_count *= 2;
      order_item_rows_return = order_item_rows_realloc(order_item_rows_return, allocated_order_item_rows_count);
      check(order_item_rows_return != NULL, "order_item_rows_return: NULL");
    }

    sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
  }

  order_item_rows_return = order_item_rows_realloc(order_item_rows_return, count_return);
  check(order_item_rows_return != NULL || count_return == 0, "order_item_rows_return: NULL");

  sql_finalize_statement(sql_statement);

  *order_item_rows = order_item_rows_return;
  *count = count_return;

  return 0;

error:

  if (order_item_rows_return != NULL) { order_item_rows_free(order_item_rows_return, count_return); }
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
      "\"quantity\" = ?3 "
    "WHERE \"order-item-id\" = ?4;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_order_id_result = sql_bind_int(sql_statement, 1, order_item_row->order_id);
  check(sql_bind_order_id_result == 0, "sql_bind_order_id_result: %d",
    sql_bind_order_id_result);

  int sql_bind_name_result = sql_bind_string(sql_statement, 2, order_item_row->name);
  check(sql_bind_name_result == 0, "sql_bind_name_result: %d",
    sql_bind_name_result);

  int sql_bind_quantity_result = sql_bind_int(sql_statement, 3, order_item_row->quantity);
  check(sql_bind_quantity_result == 0, "sql_bind_quantity_result: %d",
    sql_bind_quantity_result);

  int sql_bind_order_item_id_result = sql_bind_int(sql_statement, 4, order_item_row->order_item_id);
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
