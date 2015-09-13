#ifndef GET_ORDERS_REQUEST_H
#define GET_ORDERS_REQUEST_H

#include "../../infrastructure/validation/validation.h"

// represents a get orders request
typedef struct get_orders_request_t
{
} get_orders_request_t;

// allocates a get orders request
get_orders_request_t *get_orders_request_malloc();

// validates a get orders request
int get_orders_request_validate(
  get_orders_request_t *get_orders_request,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count);

// frees a get orders request
void get_orders_request_free(get_orders_request_t *get_orders_request);

#endif
