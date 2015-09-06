#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation_json/validation_json.h"
#include "../../web_api/bindings/update_order_request_order_item_http.h"
#include "../../web_api/services/update_order_request_order_item.h"

// parses an update order request order item from json
int update_order_request_order_item_json_parse(json_t *json, update_order_request_order_item_t **update_order_request_order_item)
{
  update_order_request_order_item_t *update_order_request_order_item_return = NULL;

  int *order_item_id = NULL;
  char *name = NULL;
  double *quantity = NULL;

  check(json != NULL, "json: NULL");
  check(update_order_request_order_item != NULL, "update_order_request_order_item: NULL");

  int json_object_get_order_item_id_result = json_object_get_int(json, "order-item-id", &order_item_id);
  check(json_object_get_order_item_id_result == 0, "json_object_get_order_item_id_result: %d",
    json_object_get_order_item_id_result);

  int json_object_get_name_result = json_object_get_string(json, "name", &name);
  check(json_object_get_name_result == 0, "json_object_get_name_result: %d",
    json_object_get_name_result);

  int json_object_get_quantity_result = json_object_get_double(json, "quantity", &quantity);
  check(json_object_get_quantity_result == 0, "json_object_get_quantity_result: %d",
    json_object_get_quantity_result);

  update_order_request_order_item_return = update_order_request_order_item_malloc(
    order_item_id,
    name,
    quantity);

  check(update_order_request_order_item_return != NULL, "update_order_request_order_item_return: NULL");

  free(order_item_id);
  free(name);
  free(quantity);

  *update_order_request_order_item = update_order_request_order_item_return;

  return 0;

error:

  if (update_order_request_order_item_return != NULL)
  {
    update_order_request_order_item_free(update_order_request_order_item_return);
  }

  if (order_item_id != NULL) { free(order_item_id); }
  if (name != NULL) { free(name); }
  if (quantity != NULL) { free(quantity); }

  return -1;
}

// formats an update order request order item error to json
int update_order_request_order_item_json_format_error(
  validation_error_t *validation_error,
  char *error_buffer)
{
  check(validation_error != NULL, "validation_error: NULL");
  check(error_buffer != NULL, "error_buffer: NULL");

  if (validation_error->validation_path->next->property == UPDATE_ORDER_REQUEST_ORDER_ITEM_ID)
  {
    int sprintf_result = sprintf(error_buffer, "order-items-%d-order-item-id-%s",
      validation_error->validation_path->index,
      validation_errors_json[validation_error->error_code]);

    check(sprintf_result > 0, "sprintf_result: %d",
      sprintf_result);
  }

  if (validation_error->validation_path->next->property == UPDATE_ORDER_REQUEST_ORDER_ITEM_NAME)
  {
    int sprintf_result = sprintf(error_buffer, "order-items-%d-name-%s",
      validation_error->validation_path->index,
      validation_errors_json[validation_error->error_code]);

    check(sprintf_result > 0, "sprintf_result: %d",
      sprintf_result);
  }

  if (validation_error->validation_path->next->property == UPDATE_ORDER_REQUEST_ORDER_ITEM_QUANTITY)
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
