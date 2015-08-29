#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"
#include "../../http/services/new_order_response_http.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"

// formats a new order response to json
int new_order_response_json_format(new_order_response_t *new_order_response, json_t **json, json_context_t *json_context)
{
  json_t *json_return = NULL;

  check(new_order_response != NULL, "new_order_response: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  int json_object_malloc_result = json_object_malloc(&json_return);
  check(json_object_malloc_result == 0, "json_object_malloc_result: %d",
    json_object_malloc_result);

  int *order_id = new_order_response->order_id;
  int json_object_set_order_id_result = json_object_set_int(json_return, "order-id", order_id, json_context);
  check(json_object_set_order_id_result == 0, "json_object_set_order_id_result: %d",
    json_object_set_order_id_result);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}

// formats new order validation errors to json
int new_order_validation_errors_json_format(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context)
{
  char *validation_errors_json[] =
  {
    "ok",
    "required",
    "too-low",
    "too-high",
    "too-short",
    "too-long",
    "too-few",
    "too-many",
    "duplicate"
  };

  json_t *json_return = NULL;
  char *buffer = NULL;

  check(validation_errors != NULL, "validation_errors: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  int json_array_malloc_result = json_array_malloc(&json_return);
  check(json_array_malloc_result == 0, "json_array_malloc_result: %d",
    json_array_malloc_result);

  for (int i = 0; i < validation_errors_count; i++)
  {
    buffer = malloc(sizeof(char) * 128);

    if (validation_errors[i]->validation_path->property == NEW_ORDER_REQUEST_CUSTOMER_NAME)
    {
      sprintf(buffer, "customer-name-%s", validation_errors_json[validation_errors[i]->error_code]);
      json_array_add_string(json_return, buffer, json_context);
    }

    free(buffer);

    buffer = NULL;
  }

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (buffer != NULL) { free(buffer); }

  return -1;
}
