#ifndef NEW_ORDER_REQUEST_HTTP_H
#define NEW_ORDER_REQUEST_HTTP_H

#include "../../core/services/new_order_request.h"
#include "../../infrastructure/json/json.h"

// parses a new order request from json
int new_order_request_json_parse(json_t *json, new_order_request_t **new_order_request);

#endif
