#ifndef NEW_ORDER_RESPONSE_H
#define NEW_ORDER_RESPONSE_H

// represents a new order response
typedef struct new_order_response_t
{
  int *order_id;
} new_order_response_t;

// allocates a new order response
new_order_response_t *new_order_response_malloc(
  int *order_id);

// frees a new order response
void new_order_response_free(new_order_response_t *new_order_response);

#endif
