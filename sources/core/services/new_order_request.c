#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_request_order_item.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"

// allocates a new order request
new_order_request_t *new_order_request_malloc(
  char *customer_name,
  int *total)
{
  new_order_request_t *new_order_request = malloc(sizeof(new_order_request_t));
  check_mem(new_order_request);

  int malloc_memcpy_customer_name_result = malloc_memcpy_string(&(new_order_request->customer_name), customer_name);
  check(malloc_memcpy_customer_name_result == 0, "malloc_memcpy_customer_name_result: %d",
    malloc_memcpy_customer_name_result);

  int malloc_memcpy_total_result = malloc_memcpy_int(&(new_order_request->total), total);
  check(malloc_memcpy_total_result == 0, "malloc_memcpy_total_result: %d",
    malloc_memcpy_total_result);

  new_order_request->order_items = NULL;
  new_order_request->order_items_count = 0;

  return new_order_request;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }

  return NULL;
}

// validates a new order request
int new_order_request_validate(
  new_order_request_t *new_order_request,
  validation_error_t ***validation_errors,
  int *count)
{
  validation_error_t **validation_errors_return = NULL;

  int allocated_errors_count = 0;
  int used_errors_count = 0;

  check(new_order_request != NULL, "new_order_request: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(count != NULL, "count: NULL");

  int validate_customer_name_result = validate_string(new_order_request->customer_name, 1, 1, 100);
  if (validate_customer_name_result != 0)
  {
    int validation_errors_add_result = validation_errors_add(
      &validation_errors_return, &allocated_errors_count, &used_errors_count,
      NEW_ORDER_REQUEST_CUSTOMER_NAME, -1, validate_customer_name_result);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }

  int validate_order_items_result = validate_array(
    (void **)(new_order_request->order_items),
    new_order_request->order_items_count,
    1, 1, 100);

  if (validate_order_items_result == 0)
  {
    if (new_order_request->order_items != NULL)
    {
      for (int index = 0; index < new_order_request->order_items_count; index++)
      {
        int new_order_request_order_item_validate_result = new_order_request_order_item_validate(
          new_order_request->order_items[index],
          index,
          &validation_errors_return,
          &allocated_errors_count,
          &used_errors_count);

        check(new_order_request_order_item_validate_result == 0, "new_order_request_order_item_validate_result: %d",
          new_order_request_order_item_validate_result);
      }
    }
  }
  else
  {
    int validation_errors_add_result = validation_errors_add(
      &validation_errors_return, &allocated_errors_count, &used_errors_count,
      NEW_ORDER_REQUEST_ORDER_ITEMS, -1, validate_order_items_result);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }

  int validate_total_result = validate_int(new_order_request->total, 1, 0, 999999);
  if (validate_total_result != 0)
  {
    int validation_errors_add_result = validation_errors_add(
      &validation_errors_return, &allocated_errors_count, &used_errors_count,
      NEW_ORDER_REQUEST_TOTAL, -1, validate_total_result);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }

  *validation_errors = validation_errors_return;
  *count = used_errors_count;

  return 0;

error:

  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, used_errors_count); }

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
    new_order_request_order_items_free(new_order_request->order_items, new_order_request->order_items_count);
  }

  free(new_order_request);
}
