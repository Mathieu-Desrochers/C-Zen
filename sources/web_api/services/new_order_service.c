#include <sqlite3.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/time/time.h"
#include "../../web_api/services/new_order_request.h"
#include "../../web_api/services/new_order_response.h"
#include "../../web_api/services/new_order_service.h"
#include "../../web_api/tables/order_row.h"
#include "../../web_api/tables/orders_table.h"
#include "../../web_api/tables/order_item_row.h"
#include "../../web_api/tables/order_items_table.h"

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
  int validation_errors_allocated_count = 0;
  int validation_errors_used_count = 0;

  order_row_t *order_row = NULL;
  order_item_row_t *inserted_order_item_row = NULL;

  check_not_null(sql_connection);
  check_not_null(new_order_request);
  check_not_null(new_order_response);
  check_not_null(validation_errors);
  check_not_null(validation_errors_count);

  check_result(
    new_order_request_validate(
      new_order_request,
      &validation_errors_return,
      &validation_errors_allocated_count,
      &validation_errors_used_count),
    0);

  if (validation_errors_return != NULL)
  {
    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_used_count;

    return 0;
  }

  time_t placed_on_date_time;
  check_result(utc_now(&placed_on_date_time), 0);

  order_row = order_row_malloc(
    NULL,
    new_order_request->customer_name,
    &placed_on_date_time,
    new_order_request->total);

  check_not_null(order_row);

  check_result(orders_table_insert(sql_connection, order_row), 0);

  for (int i = 0; i < new_order_request->order_items_count; i++)
  {
    inserted_order_item_row = order_item_row_malloc(
      NULL,
      order_row->order_id,
      new_order_request->order_items[i]->name,
      new_order_request->order_items[i]->quantity);

    check_not_null(inserted_order_item_row);

    check_result(order_items_table_insert(sql_connection, inserted_order_item_row), 0);

    order_item_row_free(inserted_order_item_row);
    inserted_order_item_row = NULL;
  }

  new_order_response_return = new_order_response_malloc(order_row->order_id);
  check_not_null(new_order_response);

  order_row_free(order_row);

  *new_order_response = new_order_response_return;

  return 0;

error:

  if (new_order_response_return != NULL) { new_order_response_free(new_order_response_return); }
  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, validation_errors_used_count); }
  if (order_row != NULL) { order_row_free(order_row); }
  if (inserted_order_item_row != NULL) { order_item_row_free(inserted_order_item_row); }

  return -1;
}
