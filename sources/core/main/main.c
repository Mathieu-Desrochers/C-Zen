#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../core/tables/shipping_schedule_row.h"
#include "../../core/tables/shipping_schedules_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  shipping_schedule_row_t *shipping_schedule_row = NULL;

  sqlite3 *sql_connection;
  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  int select_result = shipping_schedules_table_select_by_shipping_schedule_id(sql_connection, 3, &shipping_schedule_row);
  check(select_result == 0, "select_result: %d",
    select_result);

  if (shipping_schedule_row != NULL)
  {
    printf("%d\n", shipping_schedule_row->shipping_schedule_id);
    printf("%d\n", shipping_schedule_row->day_of_week);
    printf("%d\n", (int)shipping_schedule_row->time);
    printf("%d\n", shipping_schedule_row->is_refrigerated);
    printf("%f\n", shipping_schedule_row->maximum_weight);

    shipping_schedule_row_free(shipping_schedule_row);
  }

  sql_close_connection(sql_connection);

  return 0;

error:

  if (shipping_schedule_row != NULL) { shipping_schedule_row_free(shipping_schedule_row); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }

  return -1;
}
