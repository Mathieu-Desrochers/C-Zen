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

  check_not_null(get_orders_request);

  get_orders_request_return = get_orders_request_malloc();
  check_not_null(get_orders_request_return);

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
    sentinel("validation_path->property: %d", validation_errors[i]->validation_path->property);
  }

  free(validation_error_code);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }
  if (validation_error_code != NULL) { free(validation_error_code); }

  return -1;
}
