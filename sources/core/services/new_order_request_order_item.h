#ifndef NEW_ORDER_REQUEST_ORDER_ITEM_H
#define NEW_ORDER_REQUEST_ORDER_ITEM_H

// represents an order item
typedef struct new_order_request_order_item_t
{
  char *name;
  int quantity;
} new_order_request_order_item_t;

// allocates an new order request order item
new_order_request_order_item_t *new_order_request_order_item_malloc(
  char *name,
  int quantity);

// frees a new order request order item
void new_order_request_order_item_free(new_order_request_order_item_t *new_order_request_order_item);

// frees an array of new order request order items
void new_order_request_order_items_free(new_order_request_order_item_t **new_order_request_order_item, int count);

#endif
