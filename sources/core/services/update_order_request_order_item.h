#ifndef UPDATE_ORDER_REQUEST_ORDER_ITEM_H
#define UPDATE_ORDER_REQUEST_ORDER_ITEM_H

#include "../../infrastructure/validation/validation.h"

#define UPDATE_ORDER_REQUEST_ORDER_ITEM_ID 1
#define UPDATE_ORDER_REQUEST_ORDER_ITEM_NAME 2
#define UPDATE_ORDER_REQUEST_ORDER_ITEM_QUANTITY 3

// represents an update order request order item
typedef struct update_order_request_order_item_t
{
  int *order_item_id;
  char *name;
  double *quantity;
} update_order_request_order_item_t;

// allocates an update order request order item
update_order_request_order_item_t *update_order_request_order_item_malloc(
  int *order_item_id,
  char *name,
  double *quantity);

// validates an update order request order item
int update_order_request_order_item_validate(
  update_order_request_order_item_t *update_order_request_order_item,
  int index,
  validation_error_t ***validation_errors,
  int *allocated_errors_count,
  int *used_errors_count);

// frees an update order request order item
void update_order_request_order_item_free(update_order_request_order_item_t *update_order_request_order_item);

// frees an array of update order request order items
void update_order_request_order_items_free(update_order_request_order_item_t **update_order_request_order_item, int count);

#endif
