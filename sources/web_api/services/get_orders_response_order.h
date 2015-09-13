#ifndef GET_ORDERS_RESPONSE_ORDER_H
#define GET_ORDERS_RESPONSE_ORDER_H

// represents a get orders response order
typedef struct get_orders_response_order_t
{
  int *order_id;
  char *customer_name;
} get_orders_response_order_t;

// allocates a get orders response order
get_orders_response_order_t *get_orders_response_order_malloc(
  int *order_id,
  char *customer_name);

// frees a get orders response order
void get_orders_response_order_free(get_orders_response_order_t *get_orders_response_order);

// frees an array of get orders response orders
void get_orders_response_orders_free(get_orders_response_order_t **get_orders_response_order, int count);

#endif
