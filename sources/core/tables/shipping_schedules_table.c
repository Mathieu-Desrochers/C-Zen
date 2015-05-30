#include <stddef.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "../../core/tables/shipping_schedule_row.h"
#include "../../core/tables/shipping_schedules_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

// reads a shipping schedule row
int shipping_schedules_table_read(sqlite3_stmt *sql_statement, shipping_schedule_row_t **shipping_schedule_row)
{
  shipping_schedule_row_t *shipping_schedule_row_return = NULL;

  check(sql_statement != NULL, "sql_statement: NULL");
  check(shipping_schedule_row != NULL, "shipping_schedule_row: NULL");

  int shipping_schedule_id;
  int sql_read_shipping_schedule_id_result = sql_read_int(sql_statement, 0, &shipping_schedule_id);
  check(sql_read_shipping_schedule_id_result == 0, "sql_read_shipping_schedule_id_result: %d",
    sql_read_shipping_schedule_id_result);

  int day_of_week;
  int sql_read_day_of_week_result = sql_read_int(sql_statement, 1, &day_of_week);
  check(sql_read_day_of_week_result == 0, "sql_read_day_of_week_result: %d",
    sql_read_day_of_week_result);

  time_t time;
  int sql_read_time_result = sql_read_time(sql_statement, 2, &time);
  check(sql_read_time_result == 0, "sql_read_time_result: %d",
    sql_read_time_result);

  int is_refrigerated;
  int sql_read_is_refrigerated_result = sql_read_int(sql_statement, 3, &is_refrigerated);
  check(sql_read_is_refrigerated_result == 0, "sql_read_is_refrigerated_result: %d",
    sql_read_is_refrigerated_result);

  double maximum_weight;
  int sql_read_maximum_weight_result = sql_read_double(sql_statement, 4, &maximum_weight);
  check(sql_read_maximum_weight_result == 0, "sql_read_maximum_weight_result: %d",
    sql_read_maximum_weight_result);

  shipping_schedule_row_return = shipping_schedule_row_malloc(
    shipping_schedule_id,
    day_of_week,
    time,
    is_refrigerated,
    maximum_weight);

  check(shipping_schedule_row_return != NULL, "shipping_schedule_row_return: NULL");

  *shipping_schedule_row = shipping_schedule_row_return;

  return 0;

error:

  if (shipping_schedule_row_return != NULL) { shipping_schedule_row_free(shipping_schedule_row_return); }

  return -1;
}

// inserts a shipping schedule row
int shipping_schedules_table_insert(sqlite3 *sql_connection, shipping_schedule_row_t *shipping_schedule_row)
{
  sqlite3_stmt *sql_insert_statement = NULL;
  sqlite3_stmt *sql_select_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(shipping_schedule_row != NULL, "shipping_schedule_row: NULL");

  int sql_prepare_insert_statement_result = sql_prepare_statement(
    sql_connection,
    "INSERT INTO \"shipping-schedules\" ("
      "\"day-of-week\", "
      "\"time\", "
      "\"is-refrigerated\", "
      "\"maximum-weight\") "
    "VALUES (?1, ?2, ?3, ?4);",
    &sql_insert_statement);

  check(sql_prepare_insert_statement_result == 0, "sql_prepare_insert_statement_result: %d",
    sql_prepare_insert_statement_result);

  int sql_bind_day_of_week_result = sql_bind_int(sql_insert_statement, 1, shipping_schedule_row->day_of_week);
  check(sql_bind_day_of_week_result == 0, "sql_bind_day_of_week_result: %d",
    sql_bind_day_of_week_result);

  int sql_bind_time_result = sql_bind_time(sql_insert_statement, 2, shipping_schedule_row->time);
  check(sql_bind_time_result == 0, "sql_bind_time_result: %d",
    sql_bind_time_result);

  int sql_bind_is_refrigerated_result = sql_bind_int(sql_insert_statement, 3, shipping_schedule_row->is_refrigerated);
  check(sql_bind_is_refrigerated_result == 0, "sql_bind_is_refrigerated_result: %d",
    sql_bind_is_refrigerated_result);

  int sql_bind_maximum_weight_result = sql_bind_double(sql_insert_statement, 4, shipping_schedule_row->maximum_weight);
  check(sql_bind_maximum_weight_result == 0, "sql_bind_maximum_weight_result: %d",
    sql_bind_maximum_weight_result);

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

  shipping_schedule_row->shipping_schedule_id = sqlite3_column_int(sql_select_statement, 0);

  sql_finalize_statement(sql_insert_statement);
  sql_finalize_statement(sql_select_statement);

  return 0;

error:

  if (sql_insert_statement != NULL) { sql_finalize_statement(sql_insert_statement); }
  if (sql_select_statement != NULL) { sql_finalize_statement(sql_select_statement); }

  return -1;
}

// selects a shipping schedule row by shipping schedule id
int shipping_schedules_table_select_by_shipping_schedule_id(sqlite3 *sql_connection, int shipping_schedule_id, shipping_schedule_row_t **shipping_schedule_row)
{
  sqlite3_stmt *sql_statement = NULL;
  shipping_schedule_row_t *shipping_schedule_row_return = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(shipping_schedule_row != NULL, "shipping_schedule_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"shipping-schedule-id\", "
      "\"day-of-week\", "
      "\"time\", "
      "\"is-refrigerated\", "
      "\"maximum-weight\" "
    "FROM \"shipping-schedules\" "
    "WHERE \"shipping-schedule-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_shipping_schedule_id_result = sql_bind_int(sql_statement, 1, shipping_schedule_id);
  check(sql_bind_shipping_schedule_id_result == 0, "sql_bind_shipping_schedule_id_result: %d",
    sql_bind_shipping_schedule_id_result);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  if (is_row_available == 1)
  {
    int shipping_schedules_table_read_result = shipping_schedules_table_read(sql_statement, &shipping_schedule_row_return);
    check(shipping_schedules_table_read_result == 0, "shipping_schedules_table_read_result: %d",
      shipping_schedules_table_read_result);
  }

  sql_finalize_statement(sql_statement);

  *shipping_schedule_row = shipping_schedule_row_return;

  return 0;

error:

  if (shipping_schedule_row_return != NULL) { shipping_schedule_row_free(shipping_schedule_row_return); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// selects all the shipping schedule rows
int shipping_schedules_table_select_all(sqlite3 *sql_connection, shipping_schedule_row_t ***shipping_schedule_rows, int *count)
{
  sqlite3_stmt *sql_statement = NULL;
  shipping_schedule_row_t **allocated_shipping_schedule_rows = NULL;
  shipping_schedule_row_t **reallocated_shipping_schedule_rows = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(shipping_schedule_rows != NULL, "shipping_schedule_rows: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "SELECT "
      "\"shipping-schedule-id\", "
      "\"day-of-week\", "
      "\"time\", "
      "\"is-refrigerated\", "
      "\"maximum-weight\" "
    "FROM \"shipping_schedules\";",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int read_shipping_schedule_rows_count = 0;

  int allocated_shipping_schedule_rows_count = 4;
  allocated_shipping_schedule_rows = malloc(sizeof(shipping_schedule_row_t *) * allocated_shipping_schedule_rows_count);
  check_mem(allocated_shipping_schedule_rows);

  int is_row_available = 0;
  int sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
  check(sql_step_select_result == 0, "sql_step_select_result: %d",
    sql_step_select_result);

  while (is_row_available == 1)
  {
    shipping_schedule_row_t **shipping_schedule_row = &(allocated_shipping_schedule_rows[read_shipping_schedule_rows_count]);

    int shipping_schedules_table_read_result = shipping_schedules_table_read(sql_statement, shipping_schedule_row);
    check(shipping_schedules_table_read_result == 0, "shipping_schedules_table_read_result: %d",
      shipping_schedules_table_read_result);

    read_shipping_schedule_rows_count++;

    if (read_shipping_schedule_rows_count == allocated_shipping_schedule_rows_count)
    {
      allocated_shipping_schedule_rows_count *= 2;
      reallocated_shipping_schedule_rows = realloc(allocated_shipping_schedule_rows, sizeof(shipping_schedule_row_t *) * allocated_shipping_schedule_rows_count);
      check_mem(reallocated_shipping_schedule_rows);
      allocated_shipping_schedule_rows = reallocated_shipping_schedule_rows;
    }

    sql_step_select_result = sql_step_select(sql_statement, &is_row_available);
    check(sql_step_select_result == 0, "sql_step_select_result: %d",
      sql_step_select_result);
  }

  reallocated_shipping_schedule_rows = realloc(allocated_shipping_schedule_rows, sizeof(shipping_schedule_row_t *) * read_shipping_schedule_rows_count);
  check_mem(reallocated_shipping_schedule_rows);
  allocated_shipping_schedule_rows = reallocated_shipping_schedule_rows;

  sql_finalize_statement(sql_statement);

  *shipping_schedule_rows = allocated_shipping_schedule_rows;
  *count = read_shipping_schedule_rows_count;

  return 0;

error:

  if (allocated_shipping_schedule_rows != NULL) { shipping_schedule_rows_free(allocated_shipping_schedule_rows, read_shipping_schedule_rows_count); }
  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// updates a shipping schedule row
int shipping_schedules_table_update(sqlite3 *sql_connection, shipping_schedule_row_t *shipping_schedule_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(shipping_schedule_row != NULL, "shipping_schedule_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "UPDATE \"shipping-schedules\" SET "
      "\"day-of-week\" = ?1, "
      "\"time\" = ?2, "
      "\"is-refrigerated\" = ?3, "
      "\"maximum-weight\" = ?4 "
    "WHERE \"shipping-schedule-id\" = ?5;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_day_of_week_result = sql_bind_int(sql_statement, 1, shipping_schedule_row->day_of_week);
  check(sql_bind_day_of_week_result == 0, "sql_bind_day_of_week_result: %d",
    sql_bind_day_of_week_result);

  int sql_bind_time_result = sql_bind_time(sql_statement, 2, shipping_schedule_row->time);
  check(sql_bind_time_result == 0, "sql_bind_time_result: %d",
    sql_bind_time_result);

  int sql_bind_is_refrigerated_result = sql_bind_int(sql_statement, 3, shipping_schedule_row->is_refrigerated);
  check(sql_bind_is_refrigerated_result == 0, "sql_bind_is_refrigerated_result: %d",
    sql_bind_is_refrigerated_result);

  int sql_bind_maximum_weight_result = sql_bind_double(sql_statement, 4, shipping_schedule_row->maximum_weight);
  check(sql_bind_maximum_weight_result == 0, "sql_bind_maximum_weight_result: %d",
    sql_bind_maximum_weight_result);

  int sql_bind_shipping_schedule_id_result = sql_bind_int(sql_statement, 5, shipping_schedule_row->shipping_schedule_id);
  check(sql_bind_shipping_schedule_id_result == 0, "sql_bind_shipping_schedule_id_result: %d",
    sql_bind_shipping_schedule_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}

// deletes a shipping schedule row
int shipping_schedules_table_delete(sqlite3 *sql_connection, shipping_schedule_row_t *shipping_schedule_row)
{
  sqlite3_stmt *sql_statement = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(shipping_schedule_row != NULL, "shipping_schedule_row: NULL");

  int sql_prepare_statement_result = sql_prepare_statement(
    sql_connection,
    "DELETE FROM \"shipping-schedules\" "
    "WHERE \"shipping-schedule-id\" = ?1;",
    &sql_statement);

  check(sql_prepare_statement_result == 0, "sql_prepare_statement_result: %d",
    sql_prepare_statement_result);

  int sql_bind_shipping_schedule_id_result = sql_bind_int(sql_statement, 1, shipping_schedule_row->shipping_schedule_id);
  check(sql_bind_shipping_schedule_id_result == 0, "sql_bind_shipping_schedule_id_result: %d",
    sql_bind_shipping_schedule_id_result);

  int sql_step_execute_result = sql_step_execute(sql_statement);
  check(sql_step_execute_result == 0, "sql_step_execute_result: %d",
    sql_step_execute_result);

  sql_finalize_statement(sql_statement);

  return 0;

error:

  if (sql_statement != NULL) { sql_finalize_statement(sql_statement); }

  return -1;
}
