#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/time/time.h"
#include "../../web_api/bindings/get_order_response_http.h"
#include "../../web_api/bindings/get_order_response_order_item_http.h"
#include "../../web_api/services/get_order_response.h"
#include "../../web_api/services/get_order_response_order_item.h"

// formats a get order response to json
int get_order_response_json_format(get_order_response_t *get_order_response, json_t **json, json_context_t *json_context)
{
  json_t *json_return = NULL;

  char *placed_on_date_time_string = NULL;
  json_t *order_item_json = NULL;
  json_t *order_items_json = NULL;

  check(get_order_response != NULL, "get_order_response: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  json_return = json_object_malloc();
  check(json_return != NULL, "json_return: NULL");

  int *order_id = get_order_response->order_id;
  int json_object_set_order_id_result = json_object_set_int(json_return, "order-id", order_id, json_context);
  check(json_object_set_order_id_result == 0, "json_object_set_order_id_result: %d",
    json_object_set_order_id_result);

  char *customer_name = get_order_response->customer_name;
  int json_object_set_customer_name_result = json_object_set_string(json_return, "customer-name", customer_name, json_context);
  check(json_object_set_customer_name_result == 0, "json_object_set_customer_name_result: %d",
    json_object_set_customer_name_result);

  placed_on_date_time_string = format_utc_date_time(*(get_order_response->placed_on_date_time));
  check(placed_on_date_time_string != NULL, "placed_on_date_time_string: NULL");

  int json_object_set_placed_on_date_time_result = json_object_set_string(
    json_return,
    "placed-on-date-time",
    placed_on_date_time_string,
    json_context);

  check(json_object_set_placed_on_date_time_result == 0, "json_object_set_placed_on_date_time_result: %d",
    json_object_set_placed_on_date_time_result);

  order_items_json = json_array_malloc();
  check(order_items_json != NULL, "order_items_json: NULL");

  for (int i = 0; i < get_order_response->order_items_count; i++)
  {
    int get_order_response_order_item_json_format_result = get_order_response_order_item_json_format(
      get_order_response->order_items[i],
      &order_item_json,
      json_context);

    check(get_order_response_order_item_json_format_result == 0, "get_order_response_order_item_json_format_result: %d",
      get_order_response_order_item_json_format_result);

    int json_array_add_order_item_result = json_array_add_object(order_items_json, order_item_json, json_context);
    check(json_array_add_order_item_result == 0, "json_array_add_order_item_result: %d",
      json_array_add_order_item_result);

    json_free(order_item_json);

    order_item_json = NULL;
  }

  int json_object_set_order_items_result = json_object_set_array(json_return, "order-items", order_items_json, json_context);
  check(json_object_set_order_items_result == 0, "json_object_set_order_items_result: %d",
    json_object_set_order_items_result);

  int *total = get_order_response->total;
  int json_object_set_total_result = json_object_set_int(json_return, "total", total, json_context);
  check(json_object_set_total_result == 0, "json_object_set_total_result: %d",
    json_object_set_total_result);

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
