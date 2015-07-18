#ifndef UPDATE_ORDER_REQUEST_H
#define UPDATE_ORDER_REQUEST_H

#include "../../core/services/update_order_request_order_item.h"
#include "../../infrastructure/validation/validation.h"

#define UPDATE_ORDER_REQUEST_ORDER_ID 1
#define UPDATE_ORDER_REQUEST_CUSTOMER_NAME 2
#define UPDATE_ORDER_REQUEST_ORDER_ITEMS 3
#define UPDATE_ORDER_REQUEST_TOTAL 4

// represents an update order request
typedef struct update_order_request_t
{
  int *order_id;
  char *customer_name;
  update_order_request_order_item_t **order_items;
  int order_items_count;
  int *total;
} update_order_request_t;

// allocates an update order request
update_order_request_t *update_order_request_malloc(
  int *order_id,
  char *customer_name,
  int *total);

// validates an update order request
int update_order_request_validate(
  update_order_request_t *update_order_request,
  validation_error_t ***validation_errors,
  int *allocated_errors_count,
  int *used_errors_count);

// frees an update order request
void update_order_request_free(update_order_request_t *update_order_request);

#endif
