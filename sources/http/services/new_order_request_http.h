#ifndef NEW_ORDER_REQUEST_HTTP_H
#define NEW_ORDER_REQUEST_HTTP_H

#include "../../core/services/new_order_request.h"
#include "../../infrastructure/json/json.h"

// parses a new order request from json
int new_order_request_json_parse(json_t *json, new_order_request_t **new_order_request);

// formats new order request errors to json
int new_order_request_json_format_errors(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context);

#endif
