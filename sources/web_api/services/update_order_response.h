#ifndef UPDATE_ORDER_RESPONSE_H
#define UPDATE_ORDER_RESPONSE_H

// represents an update order response
typedef struct update_order_response_t
{
} update_order_response_t;

// allocates an update order response
update_order_response_t *update_order_response_malloc();

// frees an update order response
void update_order_response_free(update_order_response_t *update_order_response);

#endif
