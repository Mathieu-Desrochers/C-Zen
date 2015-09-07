#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/get_order_request.h"

// allocates a get order request
get_order_request_t *get_order_request_malloc(
  int *order_id)
{
  get_order_request_t *get_order_request = malloc(sizeof(get_order_request_t));
  check_mem(get_order_request);

  get_order_request->order_id = NULL;

  int malloc_memcpy_order_id_result = malloc_memcpy_int(&(get_order_request->order_id), order_id);
  check(malloc_memcpy_order_id_result == 0, "malloc_memcpy_order_id_result: %d",
    malloc_memcpy_order_id_result);

  return get_order_request;

error:

  if (get_order_request != NULL) { get_order_request_free(get_order_request); }

  return NULL;
}

// validates a get order request
int get_order_request_validate(
  get_order_request_t *get_order_request,
  validation_error_t ***validation_errors,
  int *allocated_errors_count,
  int *used_errors_count)
{
  check(get_order_request != NULL, "get_order_request: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(allocated_errors_count != NULL, "allocated_errors_count: NULL");
  check(used_errors_count != NULL, "used_errors_count: NULL");

  int validate_order_id_result = validate_int(get_order_request->order_id, 1, 1, 999999);
  if (validate_order_id_result != 0)
  {
    int validation_errors_add_result = validation_errors_add_level_1(
      validation_errors, allocated_errors_count, used_errors_count,
      GET_ORDER_REQUEST_ORDER_ID, -1,
      validate_order_id_result);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
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
