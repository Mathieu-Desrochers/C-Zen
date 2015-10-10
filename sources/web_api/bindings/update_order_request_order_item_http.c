#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation_json/validation_json.h"
#include "../../web_api/bindings/update_order_request_order_item_http.h"
#include "../../web_api/services/update_order_request_order_item.h"

// parses an update order request order item
int update_order_request_order_item_http_parse(json_t *json, update_order_request_order_item_t **update_order_request_order_item)
{
  update_order_request_order_item_t *update_order_request_order_item_return = NULL;

  int *order_item_id = NULL;
  char *name = NULL;
  double *quantity = NULL;

  check_not_null(json);
  check_not_null(update_order_request_order_item);

  check_result(json_object_get_int(json, "order-item-id", &order_item_id), 0);
  check_result(json_object_get_string(json, "name", &name), 0);
  check_result(json_object_get_double(json, "quantity", &quantity), 0);

  update_order_request_order_item_return = update_order_request_order_item_malloc(
    order_item_id,
    name,
    quantity);

  check_not_null(update_order_request_order_item_return);

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

// parses an array of update order request order items
int update_order_request_order_items_http_parse(
  json_t *json,
  update_order_request_order_item_t ***update_order_request_order_items,
  int *update_order_request_order_items_count)
{
  update_order_request_order_item_t **update_order_request_order_items_return = NULL;
  int update_order_request_order_items_count_return = 0;

  update_order_request_order_item_t *update_order_request_order_item = NULL;

  json_t *order_items_json = NULL;
  int order_items_json_count = 0;

  check_result(
    json_object_get_array(
      json,
      "order-items",
      &order_items_json,
      &order_items_json_count),
    0);

  if (order_items_json != NULL)
  {
    update_order_request_order_items_return = malloc(sizeof(update_order_request_order_item_t *) * order_items_json_count);
    check_mem(update_order_request_order_items_return);

    for (int i = 0; i < order_items_json_count; i++)
    {
      json_t *order_item_json = NULL;

      check_result(json_array_get_object(order_items_json, i, &order_item_json), 0);

      if (order_item_json != NULL)
      {
        check_result(
          update_order_request_order_item_http_parse(
            order_item_json,
            &update_order_request_order_item),
          0);

        update_order_request_order_items_return[i] = update_order_request_order_item;
        update_order_request_order_items_count_return++;

        update_order_request_order_item = NULL;
      }
      else
      {
        update_order_request_order_items_return[i] = NULL;
        update_order_request_order_items_count_return++;
      }
    }
  }

  *update_order_request_order_items = update_order_request_order_items_return;
  *update_order_request_order_items_count = update_order_request_order_items_count_return;

  return 0;

error:

  if (update_order_request_order_item != NULL) { update_order_request_order_item_free(update_order_request_order_item); }

  if (update_order_request_order_items_return != NULL)
  {
    update_order_request_order_items_free(
      update_order_request_order_items_return,
      update_order_request_order_items_count_return);
  }

  return -1;
}

// formats an update order request order item error
int update_order_request_order_item_http_format_error(
  validation_error_t *validation_error,
  char *validation_error_code)
{
  check_not_null(validation_error);
  check_not_null(validation_error_code);

  char *validation_error_json = validation_errors_json[validation_error->error_code];

  if (validation_error->validation_path->next->property == UPDATE_ORDER_REQUEST_ORDER_ITEM_ID)
  {
    check_result_greater(
      sprintf(
        validation_error_code,
        "order-items-%d-order-item-id-%s",
        validation_error->validation_path->index,
        validation_error_json),
      0);
  }
  else if (validation_error->validation_path->next->property == UPDATE_ORDER_REQUEST_ORDER_ITEM_NAME)
  {
    check_result_greater(
      sprintf(
        validation_error_code,
        "order-items-%d-name-%s",
        validation_error->validation_path->index,
        validation_error_json),
      0);
  }
  else if (validation_error->validation_path->next->property == UPDATE_ORDER_REQUEST_ORDER_ITEM_QUANTITY)
  {
    check_result_greater(
      sprintf(
        validation_error_code,
        "order-items-%d-quantity-%s",
        validation_error->validation_path->index,
        validation_error_json),
      0);
  }
  else
  {
    sentinel("validation_path->next->property: %d", validation_error->validation_path->next->property);
  }

  return 0;

error:

  return -1;
}
