#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/time/time.h"
#include "../../web_api/bindings/get_order_response_http.h"
#include "../../web_api/bindings/get_order_response_order_item_http.h"
#include "../../web_api/services/get_order_response.h"
#include "../../web_api/services/get_order_response_order_item.h"

// formats a get order response
int get_order_response_http_format(get_order_response_t *get_order_response, json_t **json, json_context_t *json_context)
{
  json_t *json_return = NULL;

  char *placed_on_date_time_string = NULL;
  json_t *order_item_json = NULL;
  json_t *order_items_json = NULL;

  check_not_null(get_order_response);
  check_not_null(json);
  check_not_null(json_context);

  json_return = json_object_malloc();
  check_not_null(json_return);

  int *order_id = get_order_response->order_id;
  check_result(json_object_set_int(json_return, "order-id", order_id, json_context), 0);

  char *customer_name = get_order_response->customer_name;
  check_result(json_object_set_string(json_return, "customer-name", customer_name, json_context), 0);

  time_t *placed_on_date_time = get_order_response->placed_on_date_time;
  placed_on_date_time_string = format_utc_date_time(*placed_on_date_time);
  check_not_null(placed_on_date_time_string);
  check_result(json_object_set_string(json_return, "placed-on-date-time", placed_on_date_time_string, json_context), 0);

  order_items_json = json_array_malloc();
  check_not_null(order_items_json);

  for (int i = 0; i < get_order_response->order_items_count; i++)
  {
    check_result(
      get_order_response_order_item_http_format(
        get_order_response->order_items[i],
        &order_item_json,
        json_context),
      0);

    check_result(
      json_array_add_object(
        order_items_json,
        order_item_json,
        json_context),
      0);

    order_item_json = NULL;
  }

  check_result(json_object_set_array(json_return, "order-items", order_items_json, json_context), 0);

  int *total = get_order_response->total;
  check_result(json_object_set_int(json_return, "total", total, json_context), 0);

  free(placed_on_date_time_string);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (placed_on_date_time_string != NULL) { free(placed_on_date_time_string); }
  if (order_item_json != NULL) { json_free(order_item_json); }
  if (order_items_json != NULL) { json_free(order_items_json); }

  return -1;
}
