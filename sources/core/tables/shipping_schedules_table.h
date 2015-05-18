#ifndef SHIPPING_SCHEDULES_TABLE_H
#define SHIPPING_SCHEDULES_TABLE_H

#include <sqlite3.h>

#include "../../core/tables/shipping_schedule_row.h"

// inserts a shipping schedule row
int shipping_schedules_table_insert(sqlite3 *sql_connection, shipping_schedule_row_t *shipping_schedule_row);

// selects a shipping schedule row by shipping schedule id
int shipping_schedules_table_select_by_shipping_schedule_id(sqlite3 *sql_connection, int shipping_schedule_id, shipping_schedule_row_t **shipping_schedule_row);

// selects all the shipping schedule rows
int shipping_schedules_table_select_all(sqlite3 *sql_connection, shipping_schedule_row_t ***shipping_schedule_row, int *count);

// updates a shipping schedule row
int shipping_schedules_table_update(sqlite3 *sql_connection, shipping_schedule_row_t *shipping_schedule_row);

// deletes a shipping schedule row
int shipping_schedules_table_delete(sqlite3 *sql_connection, shipping_schedule_row_t *shipping_schedule_row);

#endif
