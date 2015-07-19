#include <sqlite3.h>
#include <time.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"
#include "../../core/services/new_order_service.h"
#include "../../core/tables/order_row.h"
#include "../../core/tables/orders_table.h"
#include "../../core/tables/order_item_row.h"
#include "../../core/tables/order_items_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/time/time.h"

// executes the new order service
int new_order_service(
  sqlite3 *sql_connection,
  new_order_request_t *new_order_request,
  new_order_response_t **new_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count)
{
  new_order_response_t *new_order_response_return = NULL;

  validation_error_t **validation_errors_return = NULL;
  int allocated_errors_count = 0;
  int used_errors_count = 0;

  order_row_t *order_row = NULL;
  order_item_row_t *inserted_order_item_row = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(new_order_request != NULL, "new_order_request: NULL");
  check(new_order_response != NULL, "new_order_response: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(validation_errors_count != NULL, "validation_errors_count: NULL");

  int new_order_request_validate_result = new_order_request_validate(
    new_order_request,
    &validation_errors_return,
    &allocated_errors_count,
    &used_errors_count);

  check(new_order_request_validate_result == 0, "new_order_request_validate_result: %d",
    new_order_request_validate_result);

  if (validation_errors_return != NULL)
  {
    *validation_errors = validation_errors_return;
    *validation_errors_count = used_errors_count;

    return 0;
  }

  time_t placed_on_date_time;
  int utc_now_result = utc_now(&placed_on_date_time);
  check(utc_now_result == 0, "utc_now_result: %d",
    utc_now_result);

  order_row = order_row_malloc(
    NULL,
    new_order_request->customer_name,
    &placed_on_date_time,
    new_order_request->total);

  check(order_row != NULL, "order_row: NULL");

  int orders_table_insert_result = orders_table_insert(sql_connection, order_row);
  check(orders_table_insert_result == 0, "orders_table_insert_result: %d",
    orders_table_insert_result);

  for (int i = 0; i < new_order_request->order_items_count; i++)
  {
    inserted_order_item_row = order_item_row_malloc(
      NULL,
      order_row->order_id,
      new_order_request->order_items[i]->name,
      new_order_request->order_items[i]->quantity,
      NULL,
      NULL,
      NULL);

    check(inserted_order_item_row != NULL, "inserted_order_item_row: NULL");

    int order_items_table_insert_result = order_items_table_insert(sql_connection, inserted_order_item_row);
    check(order_items_table_insert_result == 0, "order_items_table_insert_result: %d",
      order_items_table_insert_result);

    order_item_row_free(inserted_order_item_row);
    inserted_order_item_row = NULL;
  }

  new_order_response_return = new_order_response_malloc(order_row->order_id);
  check(new_order_response != NULL, "new_order_response: NULL");

  order_row_free(order_row);

  *new_order_response = new_order_response_return;

  return 0;

error:

  if (new_order_response_return != NULL) { new_order_response_free(new_order_response_return); }
  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, used_errors_count); }
  if (order_row != NULL) { order_row_free(order_row); }
  if (inserted_order_item_row != NULL) { order_item_row_free(inserted_order_item_row); }

  return -1;
}
