#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/string/string.h"
#include "../../infrastructure/validation_json/validation_json.h"
#include "../../web_api/bindings/get_orders_request_http.h"
#include "../../web_api/services/get_orders_request.h"

// parses a get orders request
int get_orders_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  get_orders_request_t **get_orders_request)
{
  get_orders_request_t *get_orders_request_return = NULL;

  check(get_orders_request != NULL, "get_orders_request: NULL");

  get_orders_request_return = get_orders_request_malloc();
  check(get_orders_request_return != NULL, "get_orders_request_return: NULL");

  *get_orders_request = get_orders_request_return;

  return 0;

error:

  if (get_orders_request_return != NULL) { get_orders_request_free(get_orders_request_return); }

  return -1;
}

// formats get orders request errors
int get_orders_request_http_format_errors(
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
