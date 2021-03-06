#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../web_api/bindings/get_orders_response_order_http.h"
#include "../../web_api/services/get_orders_response.h"
#include "../../web_api/services/get_orders_response_order.h"

// formats a get orders response order
int get_orders_response_order_http_format(
  get_orders_response_order_t *get_orders_response_order,
  json_t **json,
  json_context_t *json_context)
{
  json_t *json_return = NULL;

  check_not_null(get_orders_response_order);
  check_not_null(json);
  check_not_null(json_context);

  json_return = json_object_malloc();
  check_not_null(json_return);

  int *order_id = get_orders_response_order->order_id;
  check_result(json_object_set_int(json_return, "order-id", order_id, json_context), 0);

  char *customer_name = get_orders_response_order->customer_name;
  check_result(json_object_set_string(json_return, "customer-name", customer_name, json_context), 0);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}

// formats an array of get orders response orders
int get_orders_response_orders_http_format(
  get_orders_response_order_t **get_orders_response_orders,
  int get_orders_response_orders_count,
  json_t **json,
  json_context_t *json_context)
{
  json_t *json_return = NULL;

  json_t *order_json = NULL;

  check_not_null(get_orders_response_orders);
  check_not_null(json);
  check_not_null(json_context);

  json_return = json_array_malloc();
  check_not_null(json_return);

  for (int i = 0; i < get_orders_response_orders_count; i++)
  {
    check_result(
      get_orders_response_order_http_format(
        get_orders_response_orders[i],
        &order_json,
        json_context),
      0);

    check_result(
      json_array_add_object(
        json_return,
        order_json,
        json_context),
      0);

    order_json = NULL;
  }

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (order_json != NULL) { json_free(order_json); }

  return -1;
}
