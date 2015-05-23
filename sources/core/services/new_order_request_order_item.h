#ifndef NEW_ORDER_REQUEST_ORDER_ITEM_H
#define NEW_ORDER_REQUEST_ORDER_ITEM_H

// represents an order item
typedef struct new_order_request_order_item_t
{
  char *name;
  int quantity;
} new_order_request_order_item_t;

// allocates an order item
new_order_request_order_item_t *new_order_request_order_item_malloc(
  char *name,
  int quantity);

// frees an order item
void new_order_request_order_item_free(new_order_request_order_item_t *new_order_request_order_item);

#endif
