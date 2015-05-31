#ifndef NEW_ORDER_REQUEST_H
#define NEW_ORDER_REQUEST_H

#include "../../core/services/new_order_request_order_item.h"
#include "../../infrastructure/validation/validation.h"

// represents a new order request
typedef struct new_order_request_t
{
  char *customer;
  new_order_request_order_item_t **order_items;
  int order_items_count;
  int total;
} new_order_request_t;

// allocates a new order request
new_order_request_t *new_order_request_malloc(
  char *customer,
  int total);

// validates a new order request
int new_order_request_validate(new_order_request_t *new_order_request, validation_error_t ***validation_errors, int *count);

// frees a new order request
void new_order_request_free(new_order_request_t *new_order_request);

// frees an array of new order requests
void new_order_requests_free(new_order_request_t **new_order_requests, int count);

#endif
