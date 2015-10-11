#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_order_request.h"

// allocates a get order request
get_order_request_t *get_order_request_malloc(
  int *order_id)
{
  get_order_request_t *get_order_request = calloc(1, sizeof(get_order_request_t));
  check_mem(get_order_request);

  check_result(malloc_memcpy_int(&(get_order_request->order_id), order_id), 0);

  return get_order_request;

error:

  if (get_order_request != NULL) { get_order_request_free(get_order_request); }

  return NULL;
}

// validates a get order request
int get_order_request_validate(
  get_order_request_t *get_order_request,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count)
{
  check_not_null(get_order_request);
  check_not_null(validation_errors);
  check_not_null(validation_errors_allocated_count);
  check_not_null(validation_errors_used_count);

  int validate_order_id_result = validate_int(get_order_request->order_id, 1, 1, 999999);
  if (validate_order_id_result != 0)
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        GET_ORDER_REQUEST_ORDER_ID, -1,
        validate_order_id_result),
      0);
  }

  return 0;

error:

  return -1;
}

// frees a get order request
void get_order_request_free(get_order_request_t *get_order_request)
{
  if (get_order_request == NULL)
  {
    return;
  }

  if (get_order_request->order_id != NULL) { free(get_order_request->order_id); }

  free(get_order_request);
}
