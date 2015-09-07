#ifndef GET_ORDER_RESPONSE_ORDER_ITEM_H
#define GET_ORDER_RESPONSE_ORDER_ITEM_H

// represents a get order response order item
typedef struct get_order_response_order_item_t
{
  int *order_item_id;
  char *name;
  double *quantity;
} get_order_response_order_item_t;

// allocates a get order response order item
get_order_response_order_item_t *get_order_response_order_item_malloc(
  int *order_item_id,
  char *name,
  double *quantity);

// frees a get order response order item
void get_order_response_order_item_free(get_order_response_order_item_t *get_order_response_order_item);

// frees an array of get order response order items
void get_order_response_order_items_free(get_order_response_order_item_t **get_order_response_order_item, int count);

#endif
