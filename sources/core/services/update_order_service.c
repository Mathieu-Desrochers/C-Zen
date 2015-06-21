#include <sqlite3.h>
#include <time.h>

#include "../../core/services/update_order_request.h"
#include "../../core/services/update_order_response.h"
#include "../../core/services/update_order_service.h"
#include "../../core/tables/order_row.h"
#include "../../core/tables/orders_table.h"
#include "../../core/tables/order_item_row.h"
#include "../../core/tables/order_items_table.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/time/time.h"

// executes the update order service
int update_order_service(
  sqlite3 *sql_connection,
  update_order_request_t *update_order_request,
  update_order_response_t **update_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count)
{
  update_order_response_t *update_order_response_return = NULL;
  validation_error_t **validation_errors_return = NULL;
  int validation_errors_count_return = 0;

  order_row_t *order_row = NULL;
  order_item_row_t **order_item_rows = NULL;
  int order_item_rows_count = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(update_order_request != NULL, "update_order_request: NULL");
  check(update_order_response != NULL, "update_order_response: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(validation_errors_count != NULL, "validation_errors_count: NULL");

  int update_order_request_validate_result = update_order_request_validate(
    update_order_request,
    &validation_errors_return,
    &validation_errors_count_return);

  check(update_order_request_validate_result == 0, "update_order_request_validate_result: %d",
    update_order_request_validate_result);

  if (validation_errors_return != NULL)
  {
    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_count_return;
    return 0;
  }

  int orders_table_select_by_order_id_result = orders_table_select_by_order_id(
    sql_connection,
    *(update_order_request->order_id),
    &order_row);

  check(orders_table_select_by_order_id_result == 0, "orders_table_select_by_order_id_result: %d",
    orders_table_select_by_order_id_result);

  if (order_row == NULL)
  {
    int validation_errors_single_result = validation_errors_single(
      &validation_errors_return, &validation_errors_count_return,
      UPDATE_ORDER_SERVICE_INVALID_ORDER_ID);

    check(validation_errors_single_result == 0, "validation_errors_single_result: %d",
      validation_errors_single_result);

    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_count_return;
    return 0;
  }

  int order_items_table_select_by_order_id_result = order_items_table_select_by_order_id(
    sql_connection,
    *(update_order_request->order_id),
    &order_item_rows,
    &order_item_rows_count);

  check(order_items_table_select_by_order_id_result == 0, "order_items_table_select_by_order_id_result: %d",
    order_items_table_select_by_order_id_result);

  order_row_free(order_row);
  order_item_rows_free(order_item_rows, order_item_rows_count);

  *update_order_response = update_order_response_return;

  return 0;

error:

  if (order_item_rows != NULL) { order_item_rows_free(order_item_rows, order_item_rows_count); }
  if (order_row != NULL) { order_row_free(order_row); }
  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, validation_errors_count_return); }

  return -1;
}
