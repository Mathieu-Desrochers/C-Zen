#ifndef NEW_ORDER_RESPONSE_HTTP_H
#define NEW_ORDER_RESPONSE_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/new_order_response.h"

// formats a new order response
int new_order_response_http_format(new_order_response_t *new_order_response, json_t **json, json_context_t *json_context);

#endif
