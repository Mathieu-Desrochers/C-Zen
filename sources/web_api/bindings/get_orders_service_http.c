#include <jansson.h>
#include <sqlite3.h>
#include <string.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/regex/regex.h"
#include "../../web_api/bindings/get_orders_request_http.h"
#include "../../web_api/bindings/get_orders_response_http.h"
#include "../../web_api/bindings/get_orders_service_http.h"
#include "../../web_api/services/get_orders_service.h"

// returns whether a route matches the get orders service
int get_orders_service_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count)
{
  int matched_return = 0;
  char **url_tokens_return = NULL;
  int url_tokens_count_return = 0;

  check(method != NULL, "method: NULL");
  check(url != NULL, "url: NULL");

  if (strcmp(method, "GET") == 0)
  {
    int regex_match_result = regex_match(
      "^/orders$",
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

// executes the get orders service
int get_orders_service_http(
  sqlite3 *sql_connection,
  char **url_tokens,
  int url_tokens_count,
  json_t *request_json,
  json_t **response_json,
  json_context_t *response_json_context)
{
  int return_value = 0;
  json_t *response_json_return = NULL;

  get_orders_request_t *get_orders_request = NULL;
  get_orders_response_t *get_orders_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(response_json != NULL, "response_json: NULL");
  check(response_json_context != NULL, "response_json_context: NULL");

  int get_orders_request_http_parse_result = get_orders_request_http_parse(
    url_tokens,
    url_tokens_count,
    request_json,
    &get_orders_request);

  check(get_orders_request_http_parse_result == 0, "get_orders_request_http_parse_result: %d",
    get_orders_request_http_parse_result);

  int get_orders_service_result = get_orders_service(
    sql_connection,
    get_orders_request,
    &get_orders_response,
    &validation_errors,
    &validation_errors_count);

  check(get_orders_service_result == 0, "get_orders_service_result: %d",
    get_orders_service_result);

  if (validation_errors == NULL)
  {
    int get_orders_response_http_format_result = get_orders_response_http_format(
      get_orders_response,
      &response_json_return,
      response_json_context);

    check(get_orders_response_http_format_result == 0, "get_orders_response_http_format_result: %d",
      get_orders_response_http_format_result);
  }
  else
  {
    int get_orders_request_http_format_errors_result = get_orders_request_http_format_errors(
      validation_errors,
      validation_errors_count,
      &response_json_return,
      response_json_context);

    check(get_orders_request_http_format_errors_result == 0, "get_orders_request_http_format_errors_result: %d",
      get_orders_request_http_format_errors_result);

    return_value = 1;
  }

  get_orders_request_free(get_orders_request);
  get_orders_response_free(get_orders_response);
  validation_errors_free(validation_errors, validation_errors_count);

  *response_json = response_json_return;

  return return_value;

error:

  if (get_orders_request != NULL) { get_orders_request_free(get_orders_request); }
  if (get_orders_response != NULL) { get_orders_response_free(get_orders_response); }
  if (validation_errors != NULL) { validation_errors_free(validation_errors, validation_errors_count); }
  if (response_json_return != NULL) { json_free(response_json_return); }

  return -1;
}