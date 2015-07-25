#include <stdlib.h>

#include "../http/new_order_request_json.h"
#include "../core/services/new_order_request_order_item.h"
#include "../infrastructure/dbg/dbg.h"
#include "../infrastructure/json/json.h"

// parses a new order request from json
int new_order_request_json_parse(json_t *json, new_order_request_t **new_order_request)
{
  new_order_request_t *new_order_request_return = NULL;

  char *customer_name = NULL;
  int *total = NULL;

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

  free(customer_name);
  free(total);

  *new_order_request = new_order_request_return;

  return 0;

error:

  if (customer_name != NULL) { free(customer_name); }
  if (total != NULL) { free(total); }

  if (new_order_request_return != NULL) { new_order_request_free(new_order_request_return); }

  return -1;
}
