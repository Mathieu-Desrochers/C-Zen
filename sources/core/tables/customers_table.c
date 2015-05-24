#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"
#include "../../core/tables/customer_row.h"
#include "../../core/tables/customers_table.h"

// reads a customer row
int customers_table_read(sqlite3_stmt *sql_statement, customer_row_t **customer_row)
{
  char *first_name = NULL;
  char *last_name = NULL;
  char *address = NULL;
  char *city = NULL;
  char *state = NULL;
  char *zip = NULL;

  customer_row_t *customer_row_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  int customer_id;
  int sql_read_customer_id_result = sql_read_int(sql_statement, 0, &customer_id);
  check(sql_read_customer_id_result == 0, "sql_read_customer_id_result: %d",
    sql_read_customer_id_result);

  int sql_read_first_name_result = sql_read_string(sql_statement, 1, &first_name);
  check(sql_read_first_name_result == 0, "sql_read_first_name_result: %d",
    sql_read_first_name_result);

  int sql_read_last_name_result = sql_read_string(sql_statement, 2, &last_name);
  check(sql_read_last_name_result == 0, "sql_read_last_name_result: %d",
    sql_read_last_name_result);

  int sql_read_address_result = sql_read_string(sql_statement, 3, &address);
  check(sql_read_address_result == 0, "sql_read_address_result: %d",
    sql_read_address_result);

  int sql_read_city_result = sql_read_string(sql_statement, 4, &city);
  check(sql_read_city_result == 0, "sql_read_city_result: %d",
    sql_read_city_result);

  int sql_read_state_result = sql_read_string(sql_statement, 5, &state);
  check(sql_read_state_result == 0, "sql_read_state_result: %d",
    sql_read_state_result);

  int sql_read_zip_result = sql_read_string(sql_statement, 6, &zip);
  check(sql_read_zip_result == 0, "sql_read_zip_result: %d",
    sql_read_zip_result);

  customer_row_return = customer_row_malloc(
    customer_id,
    first_name,
    last_name,
    address,
    city,
    state,
    zip);

  check(customer_row_return != NULL, "customer_row_return: NULL");

  free(first_name);
  free(last_name);
  free(address);
  free(city);
  free(state);
  free(zip);

  *customer_row = customer_row_return;

  return 0;

error:

  if (first_name != NULL) { free(first_name); }
  if (last_name != NULL) { free(last_name); }
  if (address != NULL) { free(address); }
  if (city != NULL) { free(city); }
  if (state != NULL) { free(state); }
  if (zip != NULL) { free(zip); }

  if (customer_row_return != NULL) { customer_row_free(customer_row_return); }

  return -1;
}

// inserts a customer row
int customers_table_insert(sqlite3 *sql_connection, customer_row_t *customer_row)
{
  sqlite3_stmt *sql_insert_statement = NULL;
  sqlite3_stmt *sql_select_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  int sql_prepare_insert_statement_result = sql_prepare_statement(
    sql_connection,
    "INSERT INTO \"customers\" ("
      "\"first-name\", "
      "\"last-name\", "
      "\"address\", "
      "\"city\", "
      "\"state\", "
      "\"zip\") "
    "VALUES (?1, ?2, ?3, ?4, ?5, ?6);",
    &sql_insert_statement);

  check(sql_prepare_insert_statement_result == 0, "sql_prepare_insert_statement_result: %d",
    sql_prepare_insert_statement_result);

  int sql_bind_first_name_result = sql_bind_string(sql_insert_statement, 1, customer_row->first_name);
  check(sql_bind_first_name_result == 0, "sql_bind_first_name_result: %d",
    sql_bind_first_name_result);

  int sql_bind_last_name_result = sql_bind_string(sql_insert_statement, 2, customer_row->last_name);
  check(sql_bind_last_name_result == 0, "sql_bind_last_name_result: %d",
    sql_bind_last_name_result);

  int sql_bind_address_result = sql_bind_string(sql_insert_statement, 3, customer_row->address);
  check(sql_bind_address_result == 0, "sql_bind_address_result: %d",
    sql_bind_address_result);

  int sql_bind_city_result = sql_bind_string(sql_insert_statement, 4, customer_row->city);
  check(sql_bind_city_result == 0, "sql_bind_city_result: %d",
    sql_bind_city_result);

  int sql_bind_state_result = sql_bind_string(sql_insert_statement, 5, customer_row->state);
  check(sql_bind_state_result == 0, "sql_bind_state_result: %d",
    sql_bind_state_result);

  int sql_bind_zip_result = sql_bind_string(sql_insert_statement, 6, customer_row->zip);
  check(sql_bind_zip_result == 0, "sql_bind_zip_result: %d",
    sql_bind_zip_result);

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

  customer_row->customer_id = sqlite3_column_int(sql_select_statement, 0);

  sql_finalize_statement(sql_insert_statement);
  sql_finalize_statement(sql_select_statement);

  return 0;

error:

  if (sql_insert_statement != NULL) { sql_finalize_statement(sql_insert_statement); }
  if (sql_select_statement != NULL) { sql_finalize_statement(sql_select_statement); }

  return -1;
}

// selects a customer row by customer id
int customers_table_select_by_customer_id(sqlite3 *sql_connection, int customer_id, customer_row_t **customer_row)
{
  sqlite3_stmt *sql_statement = NULL;
  customer_row_t *customer_row_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
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
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_customer_id_result = sql_bind_int(sql_statement, 1, customer_id);
  check(sql_bind_customer_id_result == 0, "sql_bind_customer_id_result: %d",
    sql_bind_customer_id_result);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  if (is_row_available == 1)
  {
    int customers_table_read_result = customers_table_read(sql_statement, &customer_row_return);
    check(customers_table_read_result == 0, "customers_table_read_result: %d",
      customers_table_read_result);
  }

  sql_finalize_statement(sql_statement);

  *customer_row = customer_row_return;

  return 0;

error:

  if (customer_row_return != NULL) { customer_row_free(customer_row_return); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// selects all the customer rows
int customers_table_select_all(sqlite3 *sql_connection, customer_row_t ***customer_rows)
{
  sqlite3_stmt *sql_statement = NULL;
  customer_row_t **allocated_customer_rows = NULL;
  customer_row_t **reallocated_customer_rows = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_rows != NULL, "customer_rows: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
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
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int read_customer_rows_count = 0;

  int allocated_customer_rows_count = 4;
  allocated_customer_rows = (customer_row_t **)array_malloc(allocated_customer_rows_count);
  check_mem(allocated_customer_rows);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (is_row_available == 1)
  {
    customer_row_t **customer_row = &(allocated_customer_rows[read_customer_rows_count]);

    int customers_table_read_result = customers_table_read(sql_statement, customer_row);
    check(customers_table_read_result == 0, "customers_table_read_result: %d",
      customers_table_read_result);

    read_customer_rows_count++;

    if (read_customer_rows_count == allocated_customer_rows_count)
    {
      allocated_customer_rows_count *= 2;
      reallocated_customer_rows = (customer_row_t **)array_realloc(allocated_customer_rows, allocated_customer_rows_count);
      check_mem(reallocated_customer_rows);
      allocated_customer_rows = reallocated_customer_rows;
    }

    sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
  }

  reallocated_customer_rows = (customer_row_t **)array_realloc(allocated_customer_rows, read_customer_rows_count);
  check_mem(reallocated_customer_rows);
  allocated_customer_rows = reallocated_customer_rows;

  sql_finalize_statement(sql_statement);

  *customer_rows = allocated_customer_rows;

  return 0;

error:

  if (allocated_customer_rows != NULL) { array_free(allocated_customer_rows, customer_row_free); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// updates a customer row
int customers_table_update(sqlite3 *sql_connection, customer_row_t *customer_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "UPDATE \"customers\" SET "
      "\"first-name\" = ?1, "
      "\"last-name\" = ?2, "
      "\"address\" = ?3, "
      "\"city\" = ?4, "
      "\"state\" = ?5, "
      "\"zip\" = ?6 "
    "WHERE \"customer-id\" = ?7;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_first_name_result = sql_bind_string(sql_statement, 1, customer_row->first_name);
  check(sql_bind_first_name_result == 0, "sql_bind_first_name_result: %d",
    sql_bind_first_name_result);

  int sql_bind_last_name_result = sql_bind_string(sql_statement, 2, customer_row->last_name);
  check(sql_bind_last_name_result == 0, "sql_bind_last_name_result: %d",
    sql_bind_last_name_result);

  int sql_bind_address_result = sql_bind_string(sql_statement, 3, customer_row->address);
  check(sql_bind_address_result == 0, "sql_bind_address_result: %d",
    sql_bind_address_result);

  int sql_bind_city_result = sql_bind_string(sql_statement, 4, customer_row->city);
  check(sql_bind_city_result == 0, "sql_bind_city_result: %d",
    sql_bind_city_result);

  int sql_bind_state_result = sql_bind_string(sql_statement, 5, customer_row->state);
  check(sql_bind_state_result == 0, "sql_bind_state_result: %d",
    sql_bind_state_result);

  int sql_bind_zip_result = sql_bind_string(sql_statement, 6, customer_row->zip);
  check(sql_bind_zip_result == 0, "sql_bind_zip_result: %d",
    sql_bind_zip_result);

  int sql_bind_customer_id_result = sql_bind_int(sql_statement, 7, customer_row->customer_id);
  check(sql_bind_customer_id_result == 0, "sql_bind_customer_id_result: %d",
    sql_bind_customer_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// deletes a customer row
int customers_table_delete(sqlite3 *sql_connection, customer_row_t *customer_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(customer_row != NULL, "customer_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "DELETE FROM \"customers\" "
    "WHERE \"customer-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_customer_id_result = sql_bind_int(sql_statement, 1, customer_row->customer_id);
  check(sql_bind_customer_id_result == 0, "sql_bind_customer_id_result: %d",
    sql_bind_customer_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}
