#ifndef NEW_ORDER_REQUEST_H
#define NEW_ORDER_REQUEST_H

#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/new_order_request_order_item.h"

#define NEW_ORDER_REQUEST_CUSTOMER_NAME 1
#define NEW_ORDER_REQUEST_ORDER_ITEMS 2
#define NEW_ORDER_REQUEST_TOTAL 3

// represents a new order request
typedef struct new_order_request_t
{
  char *customer_name;
  new_order_request_order_item_t **order_items;
  int order_items_count;
  int *total;
} new_order_request_t;

// allocates a new order request
new_order_request_t *new_order_request_malloc(
  char *customer_name,
  int *total);

// validates a new order request
int new_order_request_validate(
  new_order_request_t *new_order_request,
  validation_error_t ***validation_errors,
  int *allocated_errors_count,
  int *used_errors_count);

// frees a new order request
void new_order_request_free(new_order_request_t *new_order_request);

#endif
