#ifndef NEW_ORDER_REQUEST_ORDER_ITEM_HTTP_H
#define NEW_ORDER_REQUEST_ORDER_ITEM_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../web_api/services/new_order_request_order_item.h"

// parses a new order request order item
int new_order_request_order_item_http_parse(json_t *json, new_order_request_order_item_t **new_order_request_order_item);

// parses an array of new order request order items
int new_order_request_order_items_http_parse(
  json_t *order_items_json,
  int order_items_json_count,
  new_order_request_order_item_t ***new_order_request_order_items,
  int *new_order_request_order_items_count);

// formats a new order request order item error
int new_order_request_order_item_http_format_error(
  validation_error_t *validation_error,
  char *validation_error_code);

#endif
