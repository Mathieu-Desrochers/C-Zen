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

  check_not_null(url_tokens);
  check_not_null(get_order_request);

  check_int(url_tokens_count, 1);

  order_id = malloc(sizeof(int));
  check_mem(order_id);

  check_result(string_parse_int(url_tokens[0], order_id), 0);

  get_order_request_return = get_order_request_malloc(order_id);
  check_not_null(get_order_request_return);

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

    if (validation_errors[i]->validation_path->property == GET_ORDER_REQUEST_ORDER_ID)
    {
      check_result_greater(sprintf(validation_error_code, "order-id-%s", validation_error_json), 0);
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
