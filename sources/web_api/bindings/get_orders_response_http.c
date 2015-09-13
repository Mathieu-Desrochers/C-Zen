#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/time/time.h"
#include "../../web_api/bindings/get_orders_response_http.h"
#include "../../web_api/bindings/get_orders_response_order_http.h"
#include "../../web_api/services/get_orders_response.h"
#include "../../web_api/services/get_orders_response_order.h"

// formats a get orders response
int get_orders_response_http_format(get_orders_response_t *get_orders_response, json_t **json, json_context_t *json_context)
{
  json_t *json_return = NULL;

  json_t *order_json = NULL;

  check(get_orders_response != NULL, "get_orders_response: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  json_return = json_array_malloc();
  check(json_return != NULL, "json_return: NULL");

  for (int i = 0; i < get_orders_response->orders_count; i++)
  {
    int get_orders_response_order_http_format_result = get_orders_response_order_http_format(
      get_orders_response->orders[i],
      &order_json,
      json_context);

    check(get_orders_response_order_http_format_result == 0, "get_orders_response_order_http_format_result: %d",
      get_orders_response_order_http_format_result);

    int json_array_add_order_result = json_array_add_object(json_return, order_json, json_context);
    check(json_array_add_order_result == 0, "json_array_add_order_result: %d",
      json_array_add_order_result);

    json_free(order_json);

    order_json = NULL;
  }

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (order_json != NULL) { json_free(order_json); }

  return -1;
}
