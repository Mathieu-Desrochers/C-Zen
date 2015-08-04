#include <jansson.h>
#include <sqlite3.h>
#include <string.h>

#include "../../http/services/new_order_request_http.h"
#include "../../http/services/new_order_response_http.h"
#include "../../core/services/new_order_service.h"
#include "../../http/services/new_order_service_http.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/regex/regex.h"

// parses a url and returns whether it matches the new order service
int new_order_service_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count)
{
  int matched_return = 0;
  char **url_tokens_return = NULL;
  int url_tokens_count_return = 0;

  if (strcmp(method, "GET") == 0)
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
int new_order_service_json(sqlite3 *sql_connection, char **url_tokens, int url_tokens_count, json_t *body_request, json_t **body_response)
{
  json_t *body_response_return = NULL;

  new_order_request_t *new_order_request = NULL;
  new_order_response_t *new_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;

  int new_order_request_json_parse_result = new_order_request_json_parse(body_request, &new_order_request);
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

  int new_order_response_json_format_result = new_order_response_json_format(new_order_response, &body_response_return);
  check(new_order_response_json_format_result == 0, "new_order_response_json_format_result: %d",
    new_order_response_json_format_result);

  new_order_request_free(new_order_request);
  new_order_response_free(new_order_response);
  validation_errors_free(validation_errors, validation_errors_count);

  *body_response = body_response_return;

  return 0;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }
  if (new_order_response != NULL) { new_order_response_free(new_order_response); }
  if (validation_errors != NULL) { validation_errors_free(validation_errors, validation_errors_count); }
  if (body_response_return != NULL) { json_free(body_response_return); }

  return -1;
}
