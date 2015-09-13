#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/update_order_request.h"
#include "../../web_api/services/update_order_request_order_item.h"

// allocates an update order request order item
update_order_request_order_item_t *update_order_request_order_item_malloc(
  int *order_item_id,
  char *name,
  double *quantity)
{
  update_order_request_order_item_t *update_order_request_order_item = calloc(1, sizeof(update_order_request_order_item_t));
  check_mem(update_order_request_order_item);

  int malloc_memcpy_order_item_id_result = malloc_memcpy_int(&(update_order_request_order_item->order_item_id), order_item_id);
  check(malloc_memcpy_order_item_id_result == 0, "malloc_memcpy_order_item_id_result: %d",
    malloc_memcpy_order_item_id_result);

  int malloc_memcpy_name_result = malloc_memcpy_string(&(update_order_request_order_item->name), name);
  check(malloc_memcpy_name_result == 0, "malloc_memcpy_name_result: %d",
    malloc_memcpy_name_result);

  int malloc_memcpy_quantity_result = malloc_memcpy_double(&(update_order_request_order_item->quantity), quantity);
  check(malloc_memcpy_quantity_result == 0, "malloc_memcpy_quantity_result: %d",
    malloc_memcpy_quantity_result);

  return update_order_request_order_item;

error:

  if (update_order_request_order_item != NULL) { update_order_request_order_item_free(update_order_request_order_item); }

  return NULL;
}

// validates an update order request order item
int update_order_request_order_item_validate(
  update_order_request_order_item_t *update_order_request_order_item,
  int index,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count)
{
  check(validation_errors != NULL, "validation_errors: NULL");
  check(validation_errors_allocated_count != NULL, "validation_errors_allocated_count: NULL");
  check(validation_errors_used_count != NULL, "validation_errors_used_count: NULL");

  if (update_order_request_order_item != NULL)
  {
    int validate_order_item_id_result = validate_int(update_order_request_order_item->order_item_id, 0, 1, 999999);
    if (validate_order_item_id_result != 0)
    {
      int validation_errors_add_result = validation_errors_add_level_2(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        UPDATE_ORDER_REQUEST_ORDER_ITEMS, index,
        UPDATE_ORDER_REQUEST_ORDER_ITEM_ID, -1,
        validate_order_item_id_result);

      check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
        validation_errors_add_result);
    }

    int validate_name_result = validate_string(update_order_request_order_item->name, 1, 1, 100);
    if (validate_name_result != 0)
    {
      int validation_errors_add_result = validation_errors_add_level_2(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        UPDATE_ORDER_REQUEST_ORDER_ITEMS, index,
        UPDATE_ORDER_REQUEST_ORDER_ITEM_NAME, -1,
        validate_name_result);

      check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
        validation_errors_add_result);
    }

    int validate_quantity_result = validate_double(update_order_request_order_item->quantity, 1, 1, 999999);
    if (validate_quantity_result != 0)
    {
      int validation_errors_add_result = validation_errors_add_level_2(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        UPDATE_ORDER_REQUEST_ORDER_ITEMS, index,
        UPDATE_ORDER_REQUEST_ORDER_ITEM_QUANTITY, -1,
        validate_quantity_result);

      check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
        validation_errors_add_result);
    }
  }
  else
  {
    int validation_errors_add_result = validation_errors_add_level_1(
      validation_errors, validation_errors_allocated_count, validation_errors_used_count,
      UPDATE_ORDER_REQUEST_ORDER_ITEMS, index,
      VALIDATION_RESULT_REQUIRED);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }

  return 0;

error:

  return -1;
}

// frees an update order request order item
void update_order_request_order_item_free(update_order_request_order_item_t *update_order_request_order_item)
{
  if (update_order_request_order_item == NULL)
  {
    return;
  }

  if (update_order_request_order_item->order_item_id != NULL) { free(update_order_request_order_item->order_item_id); }
  if (update_order_request_order_item->name != NULL) { free(update_order_request_order_item->name); }
  if (update_order_request_order_item->quantity != NULL) { free(update_order_request_order_item->quantity); }

  free(update_order_request_order_item);
}

// frees an array of update order request order items
void update_order_request_order_items_free(update_order_request_order_item_t **update_order_request_order_items, int count)
{
  if (update_order_request_order_items == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    update_order_request_order_item_free(update_order_request_order_items[i]);
  }

  free(update_order_request_order_items);
}
