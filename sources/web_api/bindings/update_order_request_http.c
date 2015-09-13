#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation_json/validation_json.h"
#include "../../web_api/bindings/update_order_request_http.h"
#include "../../web_api/bindings/update_order_request_order_item_http.h"
#include "../../web_api/services/update_order_request.h"
#include "../../web_api/services/update_order_request_order_item.h"
#include "../../web_api/services/update_order_service.h"

// parses an update order request
int update_order_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  update_order_request_t **update_order_request)
{
  update_order_request_t *update_order_request_return = NULL;

  int *order_id = NULL;
  char *customer_name = NULL;
  update_order_request_order_item_t *update_order_request_order_item = NULL;
  update_order_request_order_item_t **update_order_request_order_items = NULL;
  int update_order_request_order_items_count = 0;
  int *total = NULL;

  check(update_order_request != NULL, "update_order_request: NULL");

  if (json == NULL)
  {
    update_order_request_return = update_order_request_malloc(NULL, NULL, NULL);
    check(update_order_request_return != NULL, "update_order_request_return: NULL");

    *update_order_request = update_order_request_return;

    return 0;
  }

  int json_object_get_order_id_result = json_object_get_int(json, "order-id", &order_id);
  check(json_object_get_order_id_result == 0, "json_object_get_order_id_result: %d",
    json_object_get_order_id_result);

  int json_object_get_customer_name_result = json_object_get_string(json, "customer-name", &customer_name);
  check(json_object_get_customer_name_result == 0, "json_object_get_customer_name_result: %d",
    json_object_get_customer_name_result);

  int json_object_get_total_result = json_object_get_int(json, "total", &total);
  check(json_object_get_total_result == 0, "json_object_get_total_result: %d",
    json_object_get_total_result);

  update_order_request_return = update_order_request_malloc(
    order_id,
    customer_name,
    total);

  check(update_order_request_return != NULL, "update_order_request_return: NULL");

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
    update_order_request_order_items = malloc(sizeof(update_order_request_order_item_t *) * order_items_json_count);
    check_mem(update_order_request_order_items);

    for (int i = 0; i < order_items_json_count; i++)
    {
      json_t *order_item_json = NULL;

      int json_array_get_order_item_result = json_array_get_object(order_items_json, i, &order_item_json);
      check(json_array_get_order_item_result == 0, "json_array_get_order_item_result: %d",
        json_array_get_order_item_result);

      if (order_item_json != NULL)
      {
        int update_order_request_order_item_http_parse_result = update_order_request_order_item_http_parse(
          order_item_json,
          &update_order_request_order_item);

        check(update_order_request_order_item_http_parse_result == 0, "update_order_request_order_item_http_parse_result: %d",
          update_order_request_order_item_http_parse_result);

        update_order_request_order_items[i] = update_order_request_order_item;
        update_order_request_order_items_count++;

        update_order_request_order_item = NULL;
      }
      else
      {
        update_order_request_order_items[i] = NULL;
        update_order_request_order_items_count++;
      }
    }
  }

  update_order_request_return->order_items = update_order_request_order_items;
  update_order_request_return->order_items_count = update_order_request_order_items_count;

  free(order_id);
  free(customer_name);
  free(total);

  *update_order_request = update_order_request_return;

  return 0;

error:

  if (update_order_request_return != NULL) { update_order_request_free(update_order_request_return); }
  if (order_id != NULL) { free(order_id); }
  if (customer_name != NULL) { free(customer_name); }
  if (update_order_request_order_item != NULL) { update_order_request_order_item_free(update_order_request_order_item); }
  if (total != NULL) { free(total); }

  if (update_order_request_order_items != NULL)
  {
    update_order_request_order_items_free(update_order_request_order_items, update_order_request_order_items_count);
  }

  return -1;
}

// formats update order request errors
int update_order_request_http_format_errors(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context)
{
  json_t *json_return = NULL;
  char *validation_error_code = NULL;

  check(validation_errors != NULL, "validation_errors: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  json_return = json_array_malloc();
  check(json_return != NULL, "json_return: NULL");

  validation_error_code = calloc(1024, sizeof(char));
  check_mem(validation_error_code);

  for (int i = 0; i < validation_errors_count; i++)
  {
    if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_ORDER_ID)
    {
      int sprintf_result = sprintf(validation_error_code, "order-id-%s",
        validation_errors_json[validation_errors[i]->error_code]);

      check(sprintf_result > 0, "sprintf_result: %d",
        sprintf_result);
    }

    if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_CUSTOMER_NAME)
    {
      int sprintf_result = sprintf(validation_error_code, "customer-name-%s",
        validation_errors_json[validation_errors[i]->error_code]);

      check(sprintf_result > 0, "sprintf_result: %d",
        sprintf_result);
    }

    if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_ORDER_ITEMS)
    {
      if (validation_errors[i]->validation_path->index == -1)
      {
        int sprintf_result = sprintf(validation_error_code, "order-items-%s",
          validation_errors_json[validation_errors[i]->error_code]);

        check(sprintf_result > 0, "sprintf_result: %d",
          sprintf_result);
      }
      else
      {
        int update_order_request_order_item_http_format_error_result = update_order_request_order_item_http_format_error(
          validation_errors[i], validation_error_code);

        check(update_order_request_order_item_http_format_error_result == 0,
          "update_order_request_order_item_http_format_error_result: %d",
          update_order_request_order_item_http_format_error_result);
      }
    }

    if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_TOTAL)
    {
      int sprintf_result = sprintf(validation_error_code, "total-%s",
        validation_errors_json[validation_errors[i]->error_code]);

      check(sprintf_result > 0, "sprintf_result: %d",
        sprintf_result);
    }

    check(validation_error_code[0] != '\0', "validation_error_code: '%s'",
      validation_error_code);

    int json_array_add_string_result = json_array_add_string(json_return, validation_error_code, json_context);
    check(json_array_add_string_result == 0, "json_array_add_string_result: %d",
      json_array_add_string_result);

    validation_error_code[0] = '\0';
  }

  free(validation_error_code);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (validation_error_code != NULL) { free(validation_error_code); }

  return -1;
}
