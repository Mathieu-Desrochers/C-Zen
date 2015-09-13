#ifndef GET_ORDERS_RESPONSE_HTTP_H
#define GET_ORDERS_RESPONSE_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_orders_response.h"

// formats a get orders response
int get_orders_response_http_format(get_orders_response_t *get_orders_response, json_t **json, json_context_t *json_context);

#endif
