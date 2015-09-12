#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../web_api/bindings/get_order_response_order_item_http.h"
#include "../../web_api/services/get_order_response_order_item.h"

// formats a get order response order item to json
int get_order_response_order_item_json_format(
  get_order_response_order_item_t *get_order_response_order_item,
  json_t **json,
  json_context_t *json_context)
{
  json_t *json_return = NULL;

  check(get_order_response_order_item != NULL, "get_order_response_order_item: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  json_return = json_object_malloc();
  check(json_return != NULL, "json_return: NULL");

  int *order_item_id = get_order_response_order_item->order_item_id;
  int json_object_set_order_item_id_result = json_object_set_int(json_return, "order-item-id", order_item_id, json_context);
  check(json_object_set_order_item_id_result == 0, "json_object_set_order_item_id_result: %d",
    json_object_set_order_item_id_result);

  char *name = get_order_response_order_item->name;
  int json_object_set_name_result = json_object_set_string(json_return, "name", name, json_context);
  check(json_object_set_name_result == 0, "json_object_set_name_result: %d",
    json_object_set_name_result);

  double *quantity = get_order_response_order_item->quantity;
  int json_object_set_quantity_result = json_object_set_double(json_return, "quantity", quantity, json_context);
  check(json_object_set_quantity_result == 0, "json_object_set_quantity_result: %d",
    json_object_set_quantity_result);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}
