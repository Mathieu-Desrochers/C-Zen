#ifndef UPDATE_ORDER_RESPONSE_HTTP_H
#define UPDATE_ORDER_RESPONSE_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/update_order_response.h"

// formats an update order response
int update_order_response_http_format(update_order_response_t *update_order_response, json_t **json, json_context_t *json_context);

#endif
