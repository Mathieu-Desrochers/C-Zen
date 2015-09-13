#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/string/string.h"
#include "../../infrastructure/validation_json/validation_json.h"
#include "../../web_api/bindings/get_order_request_http.h"
#include "../../web_api/services/get_order_request.h"

// parses a get order request
int get_order_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  get_order_request_t **get_order_request)
{
  get_order_request_t *get_order_request_return = NULL;

  int *order_id = NULL;

  check(url_tokens != NULL, "url_tokens: NULL");
  check(url_tokens_count == 1, "url_tokens_count: %d", url_tokens_count);
  check(get_order_request != NULL, "get_order_request: NULL");

  order_id = malloc(sizeof(int));
  check_mem(order_id);

  int string_parse_int_result = string_parse_int(url_tokens[0], order_id);
  check(string_parse_int_result == 0, "string_parse_int_result: %d",
    string_parse_int_result);

  get_order_request_return = get_order_request_malloc(order_id);
  check(get_order_request_return != NULL, "get_order_request_return: NULL");

  free(order_id);

  *get_order_request = get_order_request_return;

  return 0;

error:

  if (get_order_request_return != NULL) { get_order_request_free(get_order_request_return); }
  if (order_id != NULL) { free(order_id); }

  return -1;
}

// formats get order request errors
int get_order_request_http_format_errors(
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
    if (validation_errors[i]->validation_path->property == GET_ORDER_REQUEST_ORDER_ID)
    {
      int sprintf_result = sprintf(validation_error_code, "order-id-%s",
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
