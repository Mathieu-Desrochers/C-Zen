#ifndef GET_ORDER_REQUEST_H
#define GET_ORDER_REQUEST_H

#include "../../infrastructure/validation/validation.h"

#define GET_ORDER_REQUEST_ORDER_ID 1

// represents a get order request
typedef struct get_order_request_t
{
  int *order_id;
} get_order_request_t;

// allocates a get order request
get_order_request_t *get_order_request_malloc(
  int *order_id);

// validates a get order request
int get_order_request_validate(
  get_order_request_t *get_order_request,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count);

// frees a get order request
void get_order_request_free(get_order_request_t *get_order_request);

#endif
