#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../web_api/services/get_order_request.h"
#include "../../web_api/services/get_order_response.h"
#include "../../web_api/services/get_order_service.h"
#include "../../web_api/tables/order_row.h"
#include "../../web_api/tables/orders_table.h"
#include "../../web_api/tables/order_item_row.h"
#include "../../web_api/tables/order_items_table.h"

// executes the get order service
int get_order_service(
  sqlite3 *sql_connection,
  get_order_request_t *get_order_request,
  get_order_response_t **get_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count)
{
  get_order_response_t *get_order_response_return = NULL;

  validation_error_t **validation_errors_return = NULL;
  int validation_errors_allocated_count = 0;
  int validation_errors_used_count = 0;

  order_row_t *order_row = NULL;
  order_item_row_t **order_item_rows = NULL;
  int order_item_rows_count = 0;

  get_order_response_order_item_t *get_order_response_order_item = NULL;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(get_order_request != NULL, "get_order_request: NULL");
  check(get_order_response != NULL, "get_order_response: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(validation_errors_count != NULL, "validation_errors_count: NULL");

  int get_order_request_validate_result = get_order_request_validate(
    get_order_request,
    &validation_errors_return,
    &validation_errors_allocated_count,
    &validation_errors_used_count);

  check(get_order_request_validate_result == 0, "get_order_request_validate_result: %d",
    get_order_request_validate_result);

  if (validation_errors_return != NULL)
  {
    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_used_count;

    return 0;
  }

  int orders_table_select_by_order_id_result = orders_table_select_by_order_id(
    sql_connection,
    *(get_order_request->order_id),
    &order_row);

  check(orders_table_select_by_order_id_result == 0, "orders_table_select_by_order_id_result: %d",
    orders_table_select_by_order_id_result);

  if (order_row == NULL)
  {
    int validation_errors_add_result = validation_errors_add_level_1(
      &validation_errors_return, &validation_errors_allocated_count, &validation_errors_used_count,
      GET_ORDER_REQUEST_ORDER_ID, -1,
      VALIDATION_RESULT_UNKNOWN);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);

    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_used_count;

    return 0;
  }

  int order_items_table_select_by_order_id_result = order_items_table_select_by_order_id(
    sql_connection,
    *(get_order_request->order_id),
    &order_item_rows,
    &order_item_rows_count);

  check(order_items_table_select_by_order_id_result == 0, "order_items_table_select_by_order_id_result: %d",
    order_items_table_select_by_order_id_result);

  get_order_response_return = get_order_response_malloc(
    order_row->order_id,
    order_row->customer_name,
    order_row->placed_on_date_time,
    order_row->total);

  check(get_order_response_return != NULL, "get_order_response_return: NULL");

  get_order_response_return->order_items = malloc(sizeof(get_order_response_order_item_t) * order_item_rows_count);
  check_mem(get_order_response_return->order_items);

  for (int i = 0; i < order_item_rows_count; i++)
  {
    get_order_response_order_item = get_order_response_order_item_malloc(
      order_item_rows[i]->order_item_id,
      order_item_rows[i]->name,
      order_item_rows[i]->quantity);

    check(get_order_response_order_item != NULL, "get_order_response_order_item: NULL");

    get_order_response_return->order_items[i] = get_order_response_order_item;
    get_order_response_return->order_items_count++;

    get_order_response_order_item = NULL;
  }

  order_row_free(order_row);
  order_item_rows_free(order_item_rows, order_item_rows_count);

  *get_order_response = get_order_response_return;

  return 0;

error:

  if (get_order_response_return != NULL) { get_order_response_free(get_order_response_return); }
  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, validation_errors_used_count); }
  if (order_row != NULL) { order_row_free(order_row); }
  if (order_item_rows != NULL) { order_item_rows_free(order_item_rows, order_item_rows_count); }
  if (get_order_response_order_item != NULL) { get_order_response_order_item_free(get_order_response_order_item); }

  return -1;
}
