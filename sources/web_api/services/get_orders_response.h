#ifndef GET_ORDERS_RESPONSE_H
#define GET_ORDERS_RESPONSE_H

#include "../../web_api/services/get_orders_response_order.h"

// represents a get orders response
typedef struct get_orders_response_t
{
  get_orders_response_order_t **orders;
  int orders_count;
} get_orders_response_t;

// allocates a get orders response
get_orders_response_t *get_orders_response_malloc();

// frees a get orders response
void get_orders_response_free(get_orders_response_t *get_orders_response);

#endif
