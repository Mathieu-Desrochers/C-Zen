#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation_json/validation_json.h"
#include "../../web_api/bindings/new_order_request_http.h"
#include "../../web_api/bindings/new_order_request_order_item_http.h"
#include "../../web_api/services/new_order_request.h"
#include "../../web_api/services/new_order_request_order_item.h"

// parses a new order request
int new_order_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  new_order_request_t **new_order_request)
{
  new_order_request_t *new_order_request_return = NULL;

  char *customer_name = NULL;
  new_order_request_order_item_t *new_order_request_order_item = NULL;
  new_order_request_order_item_t **new_order_request_order_items = NULL;
  int new_order_request_order_items_count = 0;
  int *total = NULL;

  check(new_order_request != NULL, "new_order_request: NULL");

  if (json == NULL)
  {
    new_order_request_return = new_order_request_malloc(NULL, NULL);
    check(new_order_request_return != NULL, "new_order_request_return: NULL");

    *new_order_request = new_order_request_return;

    return 0;
  }

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

  json_t *order_items_json = NULL;
  int order_items_json_count = 0;

  int json_object_get_order_items_result = json_object_get_array(
    json,
    "order-items",
    &order_items_json,
    &order_items_json_count);

  check(json_object_get_order_items_result == 0, "json_object_get_order_items_result: %d",
    json_object_get_order_items_result);

  if (order_items_json != NULL)
  {
    new_order_request_order_items = malloc(sizeof(new_order_request_order_item_t *) * order_items_json_count);
    check_mem(new_order_request_order_items);

    for (int i = 0; i < order_items_json_count; i++)
    {
      json_t *order_item_json = NULL;

      int json_array_get_order_item_result = json_array_get_object(order_items_json, i, &order_item_json);
      check(json_array_get_order_item_result == 0, "json_array_get_order_item_result: %d",
        json_array_get_order_item_result);

      if (order_item_json != NULL)
      {
        int new_order_request_order_item_json_parse_result = new_order_request_order_item_json_parse(
          order_item_json,
          &new_order_request_order_item);

        check(new_order_request_order_item_json_parse_result == 0, "new_order_request_order_item_json_parse_result: %d",
          new_order_request_order_item_json_parse_result);

        new_order_request_order_items[i] = new_order_request_order_item;
        new_order_request_order_items_count++;

        new_order_request_order_item = NULL;
      }
      else
      {
        new_order_request_order_items[i] = NULL;
        new_order_request_order_items_count++;
      }
    }
  }

  new_order_request_return->order_items = new_order_request_order_items;
  new_order_request_return->order_items_count = new_order_request_order_items_count;

  free(customer_name);
  free(total);

  *new_order_request = new_order_request_return;

  return 0;

error:

  if (new_order_request_return != NULL) { new_order_request_free(new_order_request_return); }
  if (customer_name != NULL) { free(customer_name); }
  if (new_order_request_order_item != NULL) { new_order_request_order_item_free(new_order_request_order_item); }
  if (total != NULL) { free(total); }

  if (new_order_request_order_items != NULL)
  {
    new_order_request_order_items_free(new_order_request_order_items, new_order_request_order_items_count);
  }

  return -1;
}

// formats new order request errors to json
int new_order_request_json_format_errors(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context)
{
  json_t *json_return = NULL;
  char *error_buffer = NULL;

  check(validation_errors != NULL, "validation_errors: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  json_return = json_array_malloc();
  check(json_return != NULL, "json_return: NULL");

  error_buffer = malloc(sizeof(char) * 256);
  check_mem(error_buffer);

  for (int i = 0; i < validation_errors_count; i++)
  {
    if (validation_errors[i]->validation_path->property == NEW_ORDER_REQUEST_CUSTOMER_NAME)
    {
      int sprintf_result = sprintf(error_buffer, "customer-name-%s",
        validation_errors_json[validation_errors[i]->error_code]);

      check(sprintf_result > 0, "sprintf_result: %d",
        sprintf_result);
    }

    if (validation_errors[i]->validation_path->property == NEW_ORDER_REQUEST_ORDER_ITEMS)
    {
      if (validation_errors[i]->validation_path->index == -1)
      {
        int sprintf_result = sprintf(error_buffer, "order-items-%s",
          validation_errors_json[validation_errors[i]->error_code]);

        check(sprintf_result > 0, "sprintf_result: %d",
          sprintf_result);
      }
      else
      {
        int order_item_format_error_result = new_order_request_order_item_json_format_error(
          validation_errors[i], error_buffer);

        check(order_item_format_error_result == 0, "order_item_format_error_result: %d",
          order_item_format_error_result);
      }
    }

    if (validation_errors[i]->validation_path->property == NEW_ORDER_REQUEST_TOTAL)
    {
      int sprintf_result = sprintf(error_buffer, "total-%s",
        validation_errors_json[validation_errors[i]->error_code]);

      check(sprintf_result > 0, "sprintf_result: %d",
        sprintf_result);
    }

    int json_array_add_string_result = json_array_add_string(json_return, error_buffer, json_context);
    check(json_array_add_string_result == 0, "json_array_add_string_result: %d",
      json_array_add_string_result);
  }

  free(error_buffer);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (error_buffer != NULL) { free(error_buffer); }

  return -1;
}
