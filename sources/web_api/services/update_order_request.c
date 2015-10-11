#include <stdlib.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../infrastructure/validation/validation.h"
#include "../../web_api/services/update_order_request.h"
#include "../../web_api/services/update_order_request_order_item.h"

// allocates an update order request
update_order_request_t *update_order_request_malloc(
  int *order_id,
  char *customer_name,
  int *total)
{
  update_order_request_t *update_order_request = calloc(1, sizeof(update_order_request_t));
  check_mem(update_order_request);

  check_result(malloc_memcpy_int(&(update_order_request->order_id), order_id), 0);
  check_result(malloc_memcpy_string(&(update_order_request->customer_name), customer_name), 0);
  check_result(malloc_memcpy_int(&(update_order_request->total), total), 0);

  return update_order_request;

error:

  if (update_order_request != NULL) { update_order_request_free(update_order_request); }

  return NULL;
}

// validates an update order request
int update_order_request_validate(
  update_order_request_t *update_order_request,
  validation_error_t ***validation_errors,
  int *validation_errors_allocated_count,
  int *validation_errors_used_count)
{
  int **order_item_ids = NULL;
  int *duplicate_indexes = NULL;
  int duplicate_indexes_count = 0;

  check_not_null(update_order_request);
  check_not_null(validation_errors);
  check_not_null(validation_errors_allocated_count);
  check_not_null(validation_errors_used_count);

  int validate_order_id_result = validate_int(update_order_request->order_id, 1, 1, 999999);
  if (validate_order_id_result != 0)
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        UPDATE_ORDER_REQUEST_ORDER_ID, -1,
        validate_order_id_result),
      0);
  }

  int validate_customer_name_result = validate_string(update_order_request->customer_name, 1, 1, 100);
  if (validate_customer_name_result != 0)
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        UPDATE_ORDER_REQUEST_CUSTOMER_NAME, -1,
        validate_customer_name_result),
      0);
  }

  int validate_order_items_result = validate_array(
    (void **)(update_order_request->order_items),
    update_order_request->order_items_count,
    1, 1, 100);

  if (validate_order_items_result == 0)
  {
    int prior_errors_count = *validation_errors_used_count;

    for (int i = 0; i < update_order_request->order_items_count; i++)
    {
      check_result(
        update_order_request_order_item_validate(
          update_order_request->order_items[i], i,
          validation_errors,
          validation_errors_allocated_count,
          validation_errors_used_count),
        0);
    }

    int all_order_items_valid = (*validation_errors_used_count == prior_errors_count);
    if (all_order_items_valid)
    {
      order_item_ids = malloc(sizeof(int *) * (update_order_request->order_items_count));
      check_mem(order_item_ids);

      for (int i = 0; i < update_order_request->order_items_count; i++)
      {
        order_item_ids[i] = update_order_request->order_items[i]->order_item_id;
      }

      check_result(
        array_find_duplicates_int(
          order_item_ids,
          update_order_request->order_items_count,
          &duplicate_indexes,
          &duplicate_indexes_count),
        0);

      for (int i = 0; i < duplicate_indexes_count; i++)
      {
        check_result(
          validation_errors_add_level_2(
            validation_errors, validation_errors_allocated_count, validation_errors_used_count,
            UPDATE_ORDER_REQUEST_ORDER_ITEMS, duplicate_indexes[i],
            UPDATE_ORDER_REQUEST_ORDER_ITEM_ID, -1,
            VALIDATION_RESULT_DUPLICATE),
          0);
      }
    }
  }
  else
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        UPDATE_ORDER_REQUEST_ORDER_ITEMS, -1,
        validate_order_items_result),
      0);
  }

  int validate_total_result = validate_int(update_order_request->total, 1, 0, 999999);
  if (validate_total_result != 0)
  {
    check_result(
      validation_errors_add_level_1(
        validation_errors, validation_errors_allocated_count, validation_errors_used_count,
        UPDATE_ORDER_REQUEST_TOTAL, -1,
        validate_total_result),
      0);
  }

  free(duplicate_indexes);
  free(order_item_ids);

  return 0;

error:

  if (duplicate_indexes != NULL) { free(duplicate_indexes); }
  if (order_item_ids != NULL) { free(order_item_ids); }

  return -1;
}

// frees an update order request
void update_order_request_free(update_order_request_t *update_order_request)
{
  if (update_order_request == NULL)
  {
    return;
  }

  if (update_order_request->order_id != NULL) { free(update_order_request->order_id); }
  if (update_order_request->customer_name != NULL) { free(update_order_request->customer_name); }
  if (update_order_request->total != NULL) { free(update_order_request->total); }

  if (update_order_request->order_items != NULL)
  {
    update_order_request_order_items_free(
      update_order_request->order_items,
      update_order_request->order_items_count);
  }

  free(update_order_request);
}
