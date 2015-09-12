#ifndef NEW_ORDER_REQUEST_HTTP_H
#define NEW_ORDER_REQUEST_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../web_api/services/new_order_request.h"

// parses a new order request
int new_order_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  new_order_request_t **new_order_request);

// formats new order request errors
int new_order_request_http_format_errors(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context);

#endif
