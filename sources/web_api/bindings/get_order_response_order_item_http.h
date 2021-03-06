#ifndef GET_ORDER_RESPONSE_ORDER_ITEM_HTTP_H
#define GET_ORDER_RESPONSE_ORDER_ITEM_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_order_response.h"
#include "../../web_api/services/get_order_response_order_item.h"

// formats a get order response order item
int get_order_response_order_item_http_format(
  get_order_response_order_item_t *get_order_response_order_item,
  json_t **json,
  json_context_t *json_context);

// formats an array of get order response order items
int get_order_response_order_items_http_format(
  get_order_response_order_item_t **get_order_response_order_items,
  int get_order_response_order_items_count,
  json_t **json,
  json_context_t *json_context);

#endif
