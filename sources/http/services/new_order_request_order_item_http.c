#include <stdlib.h>

#include "../../core/services/new_order_request_order_item.h"
#include "../../http/services/new_order_request_order_item_http.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation_json/validation_json.h"

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

  if (new_order_request_order_item_return != NULL)
  {
    new_order_request_order_item_free(new_order_request_order_item_return);
  }

  if (name != NULL) { free(name); }
  if (quantity != NULL) { free(quantity); }

  return -1;
}

// formats a new order request order item error to json
int new_order_request_order_item_json_format_error(
  validation_error_t *validation_error,
  char *error_buffer)
{
  check(validation_error != NULL, "validation_error: NULL");
  check(error_buffer != NULL, "error_buffer: NULL");

  if (validation_error->validation_path->next->property == NEW_ORDER_REQUEST_ORDER_ITEM_NAME)
  {
    int sprintf_result = sprintf(error_buffer, "order-items-%d-name-%s",
      validation_error->validation_path->index,
      validation_errors_json[validation_error->error_code]);

    check(sprintf_result > 0, "sprintf_result: %d",
      sprintf_result);
  }

  if (validation_error->validation_path->next->property == NEW_ORDER_REQUEST_ORDER_ITEM_QUANTITY)
  {
    int sprintf_result = sprintf(error_buffer, "order-items-%d-quantity-%s",
      validation_error->validation_path->index,
      validation_errors_json[validation_error->error_code]);

    check(sprintf_result > 0, "sprintf_result: %d",
      sprintf_result);
  }

  return 0;

error:

  return -1;
}
