#ifndef GET_ORDER_RESPONSE_HTTP_H
#define GET_ORDER_RESPONSE_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_order_response.h"

// formats a get order response
int get_order_response_http_format(get_order_response_t *get_order_response, json_t **json, json_context_t *json_context);

#endif
