#ifndef GET_ORDER_RESPONSE_ORDER_ITEM_HTTP_H
#define GET_ORDER_RESPONSE_ORDER_ITEM_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_order_response_order_item.h"

// formats a get order response order item to json
int get_order_response_order_item_json_format(
  get_order_response_order_item_t *get_order_response_order_item,
  json_t **json,
  json_context_t *json_context);

#endif
