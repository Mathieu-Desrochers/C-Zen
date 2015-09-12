#ifndef UPDATE_ORDER_REQUEST_ORDER_ITEM_HTTP_H
#define UPDATE_ORDER_REQUEST_ORDER_ITEM_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../web_api/services/update_order_request_order_item.h"

// parses an update order request order item
int update_order_request_order_item_http_parse(json_t *json, update_order_request_order_item_t **update_order_request_order_item);

// formats an update order request order item error
int update_order_request_order_item_http_format_error(
  validation_error_t *validation_error,
  char *error_buffer);

#endif
