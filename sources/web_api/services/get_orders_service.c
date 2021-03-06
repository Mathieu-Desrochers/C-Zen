#include <sqlite3.h>
#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../web_api/services/get_orders_request.h"
#include "../../web_api/services/get_orders_response.h"
#include "../../web_api/services/get_orders_service.h"
#include "../../web_api/tables/order_row.h"
#include "../../web_api/tables/orders_table.h"

// executes the get orders service
int get_orders_service(
  sqlite3 *sql_connection,
  get_orders_request_t *get_orders_request,
  get_orders_response_t **get_orders_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count)
{
  get_orders_response_t *get_orders_response_return = NULL;

  validation_error_t **validation_errors_return = NULL;
  int validation_errors_allocated_count = 0;
  int validation_errors_used_count = 0;

  order_row_t **order_rows = NULL;
  int order_rows_count = 0;

  get_orders_response_order_t *get_orders_response_order = NULL;

  check_not_null(sql_connection);
  check_not_null(get_orders_request);
  check_not_null(get_orders_response);
  check_not_null(validation_errors);
  check_not_null(validation_errors_count);

  check_result(
    get_orders_request_validate(
      get_orders_request,
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

  check_result(
    orders_table_select_all(
      sql_connection,
      &order_rows,
      &order_rows_count),
    0);

  get_orders_response_return = get_orders_response_malloc();
  check(get_orders_response_return != NULL, "get_orders_response_return: NULL");

  if (order_rows != NULL)
  {
    check_result(
      order_rows_sort_by_order_id(
        order_rows,
        order_rows_count),
      0);
  }

  get_orders_response_return->orders = malloc(sizeof(get_orders_response_order_t) * order_rows_count);
  check_mem(get_orders_response_return->orders);

  for (int i = 0; i < order_rows_count; i++)
  {
    get_orders_response_order = get_orders_response_order_malloc(
      order_rows[i]->order_id,
      order_rows[i]->customer_name);

    check_not_null(get_orders_response_order);

    get_orders_response_return->orders[i] = get_orders_response_order;
    get_orders_response_return->orders_count++;

    get_orders_response_order = NULL;
  }

  order_rows_free(order_rows, order_rows_count);

  *get_orders_response = get_orders_response_return;

  return 0;

error:

  if (get_orders_response_return != NULL) { get_orders_response_free(get_orders_response_return); }
  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, validation_errors_used_count); }
  if (order_rows != NULL) { order_rows_free(order_rows, order_rows_count); }
  if (get_orders_response_order != NULL) { get_orders_response_order_free(get_orders_response_order); }

  return -1;
}
