#ifndef NEW_ORDER_REQUEST_ORDER_ITEM_H
#define NEW_ORDER_REQUEST_ORDER_ITEM_H

#include "../../infrastructure/validation/validation.h"

#define NEW_ORDER_REQUEST_ORDER_ITEM_NAME 1
#define NEW_ORDER_REQUEST_ORDER_ITEM_QUANTITY 2

// represents a new order request order item
typedef struct new_order_request_order_item_t
{
  char *name;
  double *quantity;
} new_order_request_order_item_t;

// allocates an new order request order item
new_order_request_order_item_t *new_order_request_order_item_malloc(
  char *name,
  double *quantity);

// validates a new order request order item
int new_order_request_order_item_validate(
  new_order_request_order_item_t *new_order_request_order_item,
  int index,
  validation_error_t ***validation_errors_return,
  int *allocated_errors_count,
  int *detected_errors_count);

// frees a new order request order item
void new_order_request_order_item_free(new_order_request_order_item_t *new_order_request_order_item);

// frees an array of new order request order items
void new_order_request_order_items_free(new_order_request_order_item_t **new_order_request_order_item, int count);

#endif
