#ifndef GET_ORDERS_REQUEST_HTTP_H
#define GET_ORDERS_REQUEST_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../web_api/services/get_orders_request.h"

// parses a get orders request
int get_orders_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  get_orders_request_t **get_orders_request);

// formats get orders request errors
int get_orders_request_http_format_errors(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context);

#endif
