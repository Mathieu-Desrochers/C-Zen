#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/new_order_request.h"
#include "../../web_api/services/new_order_request_order_item.h"

// allocates a new order request
new_order_request_t *new_order_request_malloc(
  char *customer_name,
  int *total)
{
  new_order_request_t *new_order_request = calloc(1, sizeof(new_order_request_t));
  check_mem(new_order_request);

  check_result(malloc_memcpy_string(&(new_order_request->customer_name), customer_name), 0);
  check_result(malloc_memcpy_int(&(new_order_request->total), total), 0);

  return new_order_request;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }

  return NULL;
}

// validates a new order request
int new_order_request_validate(
  new_order_request_t *new_order_request,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count)
{
  check_not_null(new_order_request);
  check_not_null(validation_errors);
  check_not_null(validation_errors_allocated_count);
  check_not_null(validation_errors_used_count);

  int validate_customer_name_result = validate_string(new_order_request->customer_name, 1, 1, 100);
  if (validate_customer_name_result != 0)
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        NEW_ORDER_REQUEST_CUSTOMER_NAME, -1,
        validate_customer_name_result),
      0);
  }

  int validate_order_items_result = validate_array(
    (void **)(new_order_request->order_items),
    new_order_request->order_items_count,
    1, 1, 100);

  if (validate_order_items_result == 0)
  {
    for (int i = 0; i < new_order_request->order_items_count; i++)
    {
      check_result(
        new_order_request_order_item_validate(
          new_order_request->order_items[i], i,
          validation_errors,
          validation_errors_allocated_count,
          validation_errors_used_count),
        0);
    }
  }
  else
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        NEW_ORDER_REQUEST_ORDER_ITEMS, -1,
        validate_order_items_result),
      0);
  }

  int validate_total_result = validate_int(new_order_request->total, 1, 0, 999999);
  if (validate_total_result != 0)
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        NEW_ORDER_REQUEST_TOTAL, -1,
        validate_total_result),
      0);
  }

  return 0;

error:

  return -1;
}

// frees a new order request
void new_order_request_free(new_order_request_t *new_order_request)
{
  if (new_order_request == NULL)
  {
    return;
  }

  if (new_order_request->customer_name != NULL) { free(new_order_request->customer_name); }
  if (new_order_request->total != NULL) { free(new_order_request->total); }

  if (new_order_request->order_items != NULL)
  {
    new_order_request_order_items_free(
      new_order_request->order_items,
      new_order_request->order_items_count);
  }

  free(new_order_request);
}
