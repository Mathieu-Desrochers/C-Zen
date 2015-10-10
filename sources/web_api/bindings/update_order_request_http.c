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
  update_order_request_order_item_t **update_order_request_order_items = NULL;
  int update_order_request_order_items_count = 0;
  int *total = NULL;

  check_not_null(update_order_request);

  if (json == NULL)
  {
    update_order_request_return = update_order_request_malloc(NULL, NULL, NULL);
    check_not_null(update_order_request_return);

    *update_order_request = update_order_request_return;

    return 0;
  }

  check_result(json_object_get_int(json, "order-id", &order_id), 0);
  check_result(json_object_get_string(json, "customer-name", &customer_name), 0);
  check_result(json_object_get_int(json, "total", &total), 0);

  update_order_request_return = update_order_request_malloc(
    order_id,
    customer_name,
    total);

  check_not_null(update_order_request_return);

  check_result(
    update_order_request_order_items_http_parse(
      json,
      &update_order_request_order_items,
      &update_order_request_order_items_count),
    0);

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

  check_not_null(validation_errors);
  check_not_null(json);
  check_not_null(json_context);

  json_return = json_array_malloc();
  check_not_null(json_return);

  validation_error_code = calloc(1024, sizeof(char));
  check_mem(validation_error_code);

  for (int i = 0; i < validation_errors_count; i++)
  {
    char *validation_error_json = validation_errors_json[validation_errors[i]->error_code];

    if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_ORDER_ID)
    {
      check_result_greater(sprintf(validation_error_code, "order-id-%s", validation_error_json), 0);
    }
    else if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_CUSTOMER_NAME)
    {
      check_result_greater(sprintf(validation_error_code, "customer-name-%s", validation_error_json), 0);
    }
    else if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_ORDER_ITEMS)
    {
      if (validation_errors[i]->validation_path->index == -1)
      {
        check_result_greater(sprintf(validation_error_code, "order-items-%s", validation_error_json), 0);
      }
      else
      {
        check_result(
          update_order_request_order_item_http_format_error(
            validation_errors[i],
            validation_error_code),
          0);
      }
    }
    else if (validation_errors[i]->validation_path->property == UPDATE_ORDER_REQUEST_TOTAL)
    {
      check_result_greater(sprintf(validation_error_code, "total-%s", validation_error_json), 0);
    }
    else
    {
      sentinel("validation_path->property: %d", validation_errors[i]->validation_path->property);
    }

    check_result(
      json_array_add_string(
        json_return,
        validation_error_code,
        json_context),
      0);
  }

  free(validation_error_code);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (validation_error_code != NULL) { free(validation_error_code); }

  return -1;
}
