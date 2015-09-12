#ifndef UPDATE_ORDER_REQUEST_HTTP_H
#define UPDATE_ORDER_REQUEST_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../web_api/services/update_order_request.h"

// parses an update order request
int update_order_request_http_parse(
  char **url_tokens,
  int url_tokens_count,
  json_t *json,
  update_order_request_t **update_order_request);

// formats update order request errors to json
int update_order_request_json_format_errors(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context);

#endif
