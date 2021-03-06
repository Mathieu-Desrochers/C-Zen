#include <jansson.h>
#include <sqlite3.h>
#include <string.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/regex/regex.h"
#include "../../web_api/bindings/update_order_request_http.h"
#include "../../web_api/bindings/update_order_response_http.h"
#include "../../web_api/bindings/update_order_service_http.h"
#include "../../web_api/services/update_order_service.h"

// returns whether a route matches the update order service
int update_order_service_parse_url(char *method, char *url, int *matched, char ***url_tokens, int *url_tokens_count)
{
  int matched_return = 0;
  char **url_tokens_return = NULL;
  int url_tokens_count_return = 0;

  check_not_null(method);
  check_not_null(url);

  if (strcmp(method, "PUT") == 0)
  {
    check_result(
      regex_match(
        "^/orders$",
        url,
        &matched_return,
        &url_tokens_return,
        &url_tokens_count_return),
      0);
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

// executes the update order service
int update_order_service_http(
  sqlite3 *sql_connection,
  char **url_tokens,
  int url_tokens_count,
  json_t *request_json,
  json_t **response_json,
  json_context_t *response_json_context)
{
  int return_value = 0;
  json_t *response_json_return = NULL;

  update_order_request_t *update_order_request = NULL;
  update_order_response_t *update_order_response = NULL;
  validation_error_t **validation_errors = NULL;
  int validation_errors_count = 0;

  check_not_null(sql_connection);
  check_not_null(response_json);
  check_not_null(response_json_context);

  check_result(
    update_order_request_http_parse(
      url_tokens,
      url_tokens_count,
      request_json,
      &update_order_request),
    0);

  check_result(
    update_order_service(
      sql_connection,
      update_order_request,
      &update_order_response,
      &validation_errors,
      &validation_errors_count),
    0);

  if (validation_errors == NULL)
  {
    check_result(
      update_order_response_http_format(
        update_order_response,
        &response_json_return,
        response_json_context),
      0);
  }
  else
  {
    check_result(
      update_order_request_http_format_errors(
        validation_errors,
        validation_errors_count,
        &response_json_return,
        response_json_context),
      0);

    return_value = 1;
  }

  update_order_request_free(update_order_request);
  update_order_response_free(update_order_response);
  validation_errors_free(validation_errors, validation_errors_count);

  *response_json = response_json_return;

  return return_value;

error:

  if (update_order_request != NULL) { update_order_request_free(update_order_request); }
  if (update_order_response != NULL) { update_order_response_free(update_order_response); }
  if (validation_errors != NULL) { validation_errors_free(validation_errors, validation_errors_count); }
  if (response_json_return != NULL) { json_free(response_json_return); }

  return -1;
}
