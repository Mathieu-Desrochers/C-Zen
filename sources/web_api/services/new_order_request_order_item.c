#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/new_order_request.h"
#include "../../web_api/services/new_order_request_order_item.h"

// allocates an new order request order item
new_order_request_order_item_t *new_order_request_order_item_malloc(
  char *name,
  double *quantity)
{
  new_order_request_order_item_t *new_order_request_order_item = calloc(1, sizeof(new_order_request_order_item_t));
  check_mem(new_order_request_order_item);

  check_result(malloc_memcpy_string(&(new_order_request_order_item->name), name), 0);
  check_result(malloc_memcpy_double(&(new_order_request_order_item->quantity), quantity), 0);

  return new_order_request_order_item;

error:

  if (new_order_request_order_item != NULL) { new_order_request_order_item_free(new_order_request_order_item); }

  return NULL;
}

// validates a new order request order item
int new_order_request_order_item_validate(
  new_order_request_order_item_t *new_order_request_order_item,
  int index,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count)
{
  check_not_null(validation_errors);
  check_not_null(validation_errors_allocated_count);
  check_not_null(validation_errors_used_count);

  if (new_order_request_order_item != NULL)
  {
    int validate_name_result = validate_string(new_order_request_order_item->name, 1, 1, 100);
    if (validate_name_result != 0)
    {
      check_result(
        validation_errors_add_level_2(
          validation_errors, validation_errors_allocated_count, validation_errors_used_count,
          NEW_ORDER_REQUEST_ORDER_ITEMS, index,
          NEW_ORDER_REQUEST_ORDER_ITEM_NAME, -1,
          validate_name_result),
        0);
    }

    int validate_quantity_result = validate_double(new_order_request_order_item->quantity, 1, 1, 999999);
    if (validate_quantity_result != 0)
    {
      check_result(
        validation_errors_add_level_2(
          validation_errors, validation_errors_allocated_count, validation_errors_used_count,
          NEW_ORDER_REQUEST_ORDER_ITEMS, index,
          NEW_ORDER_REQUEST_ORDER_ITEM_QUANTITY, -1,
          validate_quantity_result),
        0);
    }
  }
  else
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        NEW_ORDER_REQUEST_ORDER_ITEMS, index,
        VALIDATION_RESULT_REQUIRED),
      0);
  }

  return 0;

error:

  return -1;
}

// frees a new order request order item
void new_order_request_order_item_free(new_order_request_order_item_t *new_order_request_order_item)
{
  if (new_order_request_order_item == NULL)
  {
    return;
  }

  if (new_order_request_order_item->name != NULL) { free(new_order_request_order_item->name); }
  if (new_order_request_order_item->quantity != NULL) { free(new_order_request_order_item->quantity); }

  free(new_order_request_order_item);
}

// frees an array of new order request order items
void new_order_request_order_items_free(new_order_request_order_item_t **new_order_request_order_items, int count)
{
  if (new_order_request_order_items == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    new_order_request_order_item_free(new_order_request_order_items[i]);
  }

  free(new_order_request_order_items);
}
