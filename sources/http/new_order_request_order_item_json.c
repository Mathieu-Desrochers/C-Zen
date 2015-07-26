#include <stdlib.h>

#include "../http/new_order_request_order_item_json.h"
#include "../core/services/new_order_request_order_item.h"
#include "../infrastructure/dbg/dbg.h"
#include "../infrastructure/json/json.h"

// parses a new order request order item from json
int new_order_request_order_item_json_parse(json_t *json, new_order_request_order_item_t **new_order_request_order_item)
{
  new_order_request_order_item_t *new_order_request_order_item_return = NULL;

  char *name = NULL;
  double *quantity = NULL;

  check(json != NULL, "json: NULL");
  check(new_order_request_order_item != NULL, "new_order_request_order_item: NULL");

  int json_object_get_name_result = json_object_get_string(json, "name", &name);
  check(json_object_get_name_result == 0, "json_object_get_name_result: %d",
    json_object_get_name_result);

  int json_object_get_quantity_result = json_object_get_double(json, "quantity", &quantity);
  check(json_object_get_quantity_result == 0, "json_object_get_quantity_result: %d",
    json_object_get_quantity_result);

  new_order_request_order_item_return = new_order_request_order_item_malloc(
    name,
    quantity);

  check(new_order_request_order_item_return != NULL, "new_order_request_order_item_return: NULL");

  free(name);
  free(quantity);

  *new_order_request_order_item = new_order_request_order_item_return;

  return 0;

error:

  if (new_order_request_order_item_return != NULL) { new_order_request_order_item_free(new_order_request_order_item_return); }
  if (name != NULL) { free(name); }
  if (quantity != NULL) { free(quantity); }

  return -1;
}
