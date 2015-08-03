#ifndef NEW_ORDER_REQUEST_ORDER_ITEM_HTTP_H
#define NEW_ORDER_REQUEST_ORDER_ITEM_HTTP_H

#include "../../core/services/new_order_request_order_item.h"
#include "../../infrastructure/json/json.h"

// parses a new order request order item from json
int new_order_request_order_item_json_parse(json_t *json, new_order_request_order_item_t **new_order_request_order_item);

#endif
