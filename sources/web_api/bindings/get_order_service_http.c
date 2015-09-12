#include <jansson.h>
#include <sqlite3.h>
#include <string.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/regex/regex.h"
#include "../../web_api/bindings/get_order_request_http.h"
#include "../../web_api/bindings/get_order_response_http.h"
#include "../../web_api/bindings/get_order_service_http.h"
#include "../../web_api/services/get_order_service.h"

// returns whether a route matches the get order service
int get_order_service_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count)
{
  int matched_return = 0;
  char **url_tokens_return = NULL;
  int url_tokens_count_return = 0;

  check(method != NULL, "method: NULL");
  check(url != NULL, "url: NULL");

  if (strcmp(method, "GET") == 0)
  {
    int regex_match_result = regex_match(
      "^/orders/(\\d+)$",
      url,
      &matched_return,
      &url_tokens_return,
      &url_tokens_count_return);

    check(regex_match_result == 0, "regex_match_result: %d",
      regex_match_result);
  }
  else
  {
    matched_return = 0;
  }

  *matched = matched_return;
  *url_tokens = url_tokens_return;
  *url_tokens_count = url_tokens_count_return;

  return 0;

error:

  if (url_tokens_return != NULL) { array_free_string(url_tokens_return, url_tokens_count_return); }

  return -1;
}

// executes the get order service
int get_order_service_http(
  sqlite3 *sql_connection,
  char **url_tokens,
  int url_tokens_count,
  json_t *request_json,
  json_t **response_json,
  json_context_t *response_json_context)
{
  int return_value = 0;
  json_t *response_json_return = NULL;

  get_order_request_t *get_order_request = NULL;
  get_order_response_t *get_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;

  int get_order_request_http_parse_result = get_order_request_http_parse(
    url_tokens,
    url_tokens_count,
    request_json,
    &get_order_request);

  check(get_order_request_http_parse_result == 0, "get_order_request_http_parse_result: %d",
    get_order_request_http_parse_result);

  int get_order_service_result = get_order_service(
    sql_connection,
    get_order_request,
    &get_order_response,
    &validation_errors,
    &validation_errors_count);

  check(get_order_service_result == 0, "get_order_service_result: %d",
    get_order_service_result);

  if (validation_errors == NULL)
  {
    int get_order_response_json_format_result = get_order_response_json_format(
      get_order_response,
      &response_json_return,
      response_json_context);

    check(get_order_response_json_format_result == 0, "get_order_response_json_format_result: %d",
      get_order_response_json_format_result);
  }
  else
  {
    int get_order_request_json_format_errors_result = get_order_request_json_format_errors(
      validation_errors,
      validation_errors_count,
      &response_json_return,
      response_json_context);

    check(get_order_request_json_format_errors_result == 0, "get_order_request_json_format_errors_result: %d",
      get_order_request_json_format_errors_result);

    return_value = 1;
  }

  get_order_request_free(get_order_request);
  get_order_response_free(get_order_response);
  validation_errors_free(validation_errors, validation_errors_count);

  *response_json = response_json_return;

  return return_value;

error:

  if (get_order_request != NULL) { get_order_request_free(get_order_request); }
  if (get_order_response != NULL) { get_order_response_free(get_order_response); }
  if (validation_errors != NULL) { validation_errors_free(validation_errors, validation_errors_count); }
  if (response_json_return != NULL) { json_free(response_json_return); }

  return -1;
}
