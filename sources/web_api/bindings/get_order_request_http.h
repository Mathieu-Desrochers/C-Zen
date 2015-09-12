#ifndef GET_ORDER_REQUEST_HTTP_H
#define GET_ORDER_REQUEST_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../web_api/services/get_order_request.h"

// parses a get order request
int get_order_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  get_order_request_t **get_order_request);

// formats get order request errors
int get_order_request_http_format_errors(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context);

#endif
