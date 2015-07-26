#include <stdlib.h>

#include "../http/new_order_request_json.h"
#include "../http/new_order_request_order_item_json.h"
#include "../core/services/new_order_request.h"
#include "../core/services/new_order_request_order_item.h"
#include "../infrastructure/array/array.h"
#include "../infrastructure/dbg/dbg.h"
#include "../infrastructure/json/json.h"

// parses a new order request from json
int new_order_request_json_parse(json_t *json, new_order_request_t **new_order_request)
{
  new_order_request_t *new_order_request_return = NULL;

  char *customer_name = NULL;
  json_t *order_items_json = NULL;
  json_t *order_item_json = NULL;
  int order_items_json_count = 0;
  int *total = NULL;

  new_order_request_order_item_t *new_order_request_order_item = NULL;
  int allocated_new_order_request_order_items_count = 0;

  check(json != NULL, "json: NULL");
  check(new_order_request != NULL, "new_order_request: NULL");

  int json_object_get_customer_name_result = json_object_get_string(json, "customer-name", &customer_name);
  check(json_object_get_customer_name_result == 0, "json_object_get_customer_name_result: %d",
    json_object_get_customer_name_result);

  int json_object_get_total_result = json_object_get_int(json, "total", &total);
  check(json_object_get_total_result == 0, "json_object_get_total_result: %d",
    json_object_get_total_result);

  new_order_request_return = new_order_request_malloc(
    customer_name,
    total);

  check(new_order_request_return != NULL, "new_order_request_return: NULL");

  int json_object_get_order_items_result = json_object_get_array(json, "order-items", &order_items_json, &order_items_json_count);
  check(json_object_get_order_items_result == 0, "json_object_get_order_items_result: %d",
    json_object_get_order_items_result);

  if (order_items_json != NULL)
  {
    for (int i = 0; i < order_items_json_count; i++)
    {
      int json_array_get_order_item_result = json_array_get_object(order_items_json, i, &order_item_json);
      check(json_array_get_order_item_result == 0, "json_array_get_order_item_result: %d",
        json_array_get_order_item_result);

      if (order_item_json != NULL)
      {
        int new_order_request_order_item_json_parse_result = new_order_request_order_item_json_parse(order_item_json, &new_order_request_order_item);
        check(new_order_request_order_item_json_parse_result == 0, "new_order_request_order_item_json_parse_result: %d",
          new_order_request_order_item_json_parse_result);
      }

      int array_add_order_item_order_item_request_result = array_add_pointer(
        (void ***)&(new_order_request_return->order_items),
        &allocated_new_order_request_order_items_count,
        &(new_order_request_return->order_items_count),
        new_order_request_order_item);

      check(array_add_order_item_order_item_request_result == 0, "array_add_order_item_order_item_request_result: %d",
        array_add_order_item_order_item_request_result);

      new_order_request_order_item = NULL;
    }
  }

  free(customer_name);
  free(total);

  *new_order_request = new_order_request_return;

  return 0;

error:

  if (new_order_request_return != NULL) { new_order_request_free(new_order_request_return); }
  if (customer_name != NULL) { free(customer_name); }
  if (total != NULL) { free(total); }
  if (new_order_request_order_item != NULL) { new_order_request_order_item_free(new_order_request_order_item); }

  return -1;
}
