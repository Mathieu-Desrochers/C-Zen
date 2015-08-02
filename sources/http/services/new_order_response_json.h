#ifndef NEW_ORDER_RESPONSE_JSON_H
#define NEW_ORDER_RESPONSE_JSON_H

#include "../../core/services/new_order_response.h"
#include "../../infrastructure/json/json.h"

// formats a new order response to json
int new_order_response_json_format(new_order_response_t *new_order_response, json_t **json);

#endif
