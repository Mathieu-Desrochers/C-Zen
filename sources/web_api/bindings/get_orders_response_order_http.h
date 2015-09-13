#ifndef GET_ORDERS_RESPONSE_ORDER_HTTP_H
#define GET_ORDERS_RESPONSE_ORDER_HTTP_H

#include "../../infrastructure/json/json.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_orders_response_order.h"

// formats a get orders response order
int get_orders_response_order_http_format(
  get_orders_response_order_t *get_orders_response_order,
  json_t **json,
  json_context_t *json_context);

#endif
