#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation_json/validation_json.h"
#include "../../web_api/bindings/new_order_request_order_item_http.h"
#include "../../web_api/services/new_order_request_order_item.h"

// parses a new order request order item
int new_order_request_order_item_http_parse(json_t *json, new_order_request_order_item_t **new_order_request_order_item)
{
  new_order_request_order_item_t *new_order_request_order_item_return = NULL;

  char *name = NULL;
  double *quantity = NULL;

  check_not_null(json);
  check_not_null(new_order_request_order_item);

  check_result(json_object_get_string(json, "name", &name), 0);
  check_result(json_object_get_double(json, "quantity", &quantity), 0);

  new_order_request_order_item_return = new_order_request_order_item_malloc(
    name,
    quantity);

  check_not_null(new_order_request_order_item_return);

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

// parses an array of new order request order items
int new_order_request_order_items_http_parse(
  json_t *json,
  new_order_request_order_item_t ***new_order_request_order_items,
  int *new_order_request_order_items_count)
{
  new_order_request_order_item_t **new_order_request_order_items_return = NULL;
  int new_order_request_order_items_count_return = 0;

  new_order_request_order_item_t *new_order_request_order_item = NULL;

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
    new_order_request_order_items_return = malloc(sizeof(new_order_request_order_item_t *) * order_items_json_count);
    check_mem(new_order_request_order_items_return);

    for (int i = 0; i < order_items_json_count; i++)
    {
      json_t *order_item_json = NULL;

      check_result(json_array_get_object(order_items_json, i, &order_item_json), 0);

      if (order_item_json != NULL)
      {
        check_result(
          new_order_request_order_item_http_parse(
            order_item_json,
            &new_order_request_order_item),
          0);

        new_order_request_order_items_return[i] = new_order_request_order_item;
        new_order_request_order_items_count_return++;

        new_order_request_order_item = NULL;
      }
      else
      {
        new_order_request_order_items_return[i] = NULL;
        new_order_request_order_items_count_return++;
      }
    }
  }

  *new_order_request_order_items = new_order_request_order_items_return;
  *new_order_request_order_items_count = new_order_request_order_items_count_return;

  return 0;

error:

  if (new_order_request_order_item != NULL) { new_order_request_order_item_free(new_order_request_order_item); }

  if (new_order_request_order_items_return != NULL)
  {
    new_order_request_order_items_free(
      new_order_request_order_items_return,
      new_order_request_order_items_count_return);
  }

  return -1;
}

// formats a new order request order item error
int new_order_request_order_item_http_format_error(
  validation_error_t *validation_error,
  char *validation_error_code)
{
  check_not_null(validation_error);
  check_not_null(validation_error_code);

  char *validation_error_json = validation_errors_json[validation_error->error_code];

  if (validation_error->validation_path->next->property == NEW_ORDER_REQUEST_ORDER_ITEM_NAME)
  {
    check_result_greater(
      sprintf(
        validation_error_code,
        "order-items-%d-name-%s",
        validation_error->validation_path->index,
        validation_error_json),
      0);
  }
  else if (validation_error->validation_path->next->property == NEW_ORDER_REQUEST_ORDER_ITEM_QUANTITY)
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
