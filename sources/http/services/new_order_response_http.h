#ifndef NEW_ORDER_RESPONSE_HTTP_H
#define NEW_ORDER_RESPONSE_HTTP_H

#include "../../core/services/new_order_response.h"
#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"

// formats a new order response to json
int new_order_response_json_format(new_order_response_t *new_order_response, json_t **json, json_context_t *json_context);

// formats validation errors to json
int new_order_validation_errors_json_format(
  validation_error_t **validation_errors,
  int validation_errors_count,
  json_t **json,
  json_context_t *json_context);

#endif
