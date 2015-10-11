#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_orders_request.h"

// allocates a get orders request
get_orders_request_t *get_orders_request_malloc()
{
  get_orders_request_t *get_orders_request = calloc(1, sizeof(get_orders_request_t));
  check_mem(get_orders_request);

  return get_orders_request;

error:

  if (get_orders_request != NULL) { get_orders_request_free(get_orders_request); }

  return NULL;
}

// validates a get orders request
int get_orders_request_validate(
  get_orders_request_t *get_orders_request,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count)
{
  check_not_null(get_orders_request);
  check_not_null(validation_errors);
  check_not_null(validation_errors_allocated_count);
  check_not_null(validation_errors_used_count);

  return 0;

error:

  return -1;
}

// frees a get orders request
void get_orders_request_free(get_orders_request_t *get_orders_request)
{
  if (get_orders_request == NULL)
  {
    return;
  }

  free(get_orders_request);
}
