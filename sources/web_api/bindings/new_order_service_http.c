#include <jansson.h>
#include <sqlite3.h>
#include <string.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/regex/regex.h"
#include "../../web_api/bindings/new_order_request_http.h"
#include "../../web_api/bindings/new_order_response_http.h"
#include "../../web_api/bindings/new_order_service_http.h"
#include "../../web_api/services/new_order_service.h"

// returns whether a route matches the new order service
int new_order_service_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count)
{
  int matched_return = 0;
  char **url_tokens_return = NULL;
  int url_tokens_count_return = 0;

  check(method != NULL, "method: NULL");
  check(url != NULL, "url: NULL");

  if (strcmp(method, "POST") == 0)
  {
    int regex_match_result = regex_match(
      "^/new-order$",
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

// executes the new order service
int new_order_service_http(
  sqlite3 *sql_connection,
  char **url_tokens,
  int url_tokens_count,
  json_t *request_json,
  json_t **response_json,
  json_context_t *response_json_context)
{
  int return_value = 0;
  json_t *response_json_return = NULL;

  new_order_request_t *new_order_request = NULL;
  new_order_response_t *new_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;

  int new_order_request_json_parse_result = new_order_request_json_parse(request_json, &new_order_request);
  check(new_order_request_json_parse_result == 0, "new_order_request_json_parse_result: %d",
    new_order_request_json_parse_result);

  int new_order_service_result = new_order_service(
    sql_connection,
    new_order_request,
    &new_order_response,
    &validation_errors,
    &validation_errors_count);

  check(new_order_service_result == 0, "new_order_service_result: %d",
    new_order_service_result);

  if (validation_errors == NULL)
  {
    int new_order_response_json_format_result = new_order_response_json_format(
      new_order_response,
      &response_json_return,
      response_json_context);

    check(new_order_response_json_format_result == 0, "new_order_response_json_format_result: %d",
      new_order_response_json_format_result);
  }
  else
  {
    int new_order_request_json_format_errors_result = new_order_request_json_format_errors(
      validation_errors,
      validation_errors_count,
      &response_json_return,
      response_json_context);

    check(new_order_request_json_format_errors_result == 0, "new_order_request_json_format_errors_result: %d",
      new_order_request_json_format_errors_result);

    return_value = 1;
  }

  new_order_request_free(new_order_request);
  new_order_response_free(new_order_response);
  validation_errors_free(validation_errors, validation_errors_count);

  *response_json = response_json_return;

  return return_value;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }
  if (new_order_response != NULL) { new_order_response_free(new_order_response); }
  if (validation_errors != NULL) { validation_errors_free(validation_errors, validation_errors_count); }
  if (response_json_return != NULL) { json_free(response_json_return); }

  return -1;
}
