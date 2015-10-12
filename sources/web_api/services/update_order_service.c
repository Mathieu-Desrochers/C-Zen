#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_table.h"
#include "../../infrastructure/time/time.h"
#include "../../web_api/services/update_order_request.h"
#include "../../web_api/services/update_order_response.h"
#include "../../web_api/services/update_order_service.h"
#include "../../web_api/tables/order_row.h"
#include "../../web_api/tables/orders_table.h"
#include "../../web_api/tables/order_item_row.h"
#include "../../web_api/tables/order_items_table.h"

// executes the update order service
int update_order_service(
  sqlite3 *sql_connection,
  update_order_request_t *update_order_request,
  update_order_response_t **update_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count)
{
  update_order_response_t *update_order_response_return = NULL;

  validation_error_t **validation_errors_return = NULL;
  int validation_errors_allocated_count = 0;
  int validation_errors_used_count = 0;

  int exit_code = 0;

  order_row_t *order_row = NULL;
  order_item_row_t **order_item_rows = NULL;
  int order_item_rows_count = 0;

  order_row_t *updated_order_row = NULL;
  order_item_row_t *inserted_order_item_row = NULL;
  order_item_row_t *updated_order_item_row = NULL;

  int **update_order_request_order_item_ids = NULL;
  int **order_item_row_order_item_ids = NULL;
  int *unknown_order_item_id_indexes = NULL;
  int unknown_order_item_id_indexes_count = 0;
  int *deleted_order_item_id_indexes = NULL;
  int deleted_order_item_id_indexes_count = 0;

  hash_table_t *order_item_rows_hash_table = NULL;

  check_not_null(sql_connection);
  check_not_null(update_order_request);
  check_not_null(update_order_response);
  check_not_null(validation_errors);
  check_not_null(validation_errors_count);

  check_result(
    update_order_request_validate(
      update_order_request,
      &validation_errors_return,
      &validation_errors_allocated_count,
      &validation_errors_used_count),
    0);

  if (validation_errors_return != NULL)
  {
    *update_order_response = update_order_response_return;

    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_used_count;

    goto cleanup;
  }

  check_result(
    orders_table_select_by_order_id(
      sql_connection,
      *(update_order_request->order_id),
      &order_row),
    0);

  if (order_row == NULL)
  {
    check_result(
      validation_errors_add_level_1(
        &validation_errors_return, &validation_errors_allocated_count, &validation_errors_used_count,
        UPDATE_ORDER_REQUEST_ORDER_ID, -1,
        VALIDATION_RESULT_UNKNOWN),
      0);

    *update_order_response = update_order_response_return;

    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_used_count;

    goto cleanup;
  }

  check_result(
    order_items_table_select_by_order_id(
      sql_connection,
      *(update_order_request->order_id),
      &order_item_rows,
      &order_item_rows_count),
    0);

  update_order_request_order_item_ids = malloc(sizeof(int *) * (update_order_request->order_items_count));
  check_mem(update_order_request_order_item_ids);

  for (int i = 0; i < update_order_request->order_items_count; i++)
  {
    update_order_request_order_item_ids[i] = update_order_request->order_items[i]->order_item_id;
  }

  order_item_row_order_item_ids = malloc(sizeof(int *) * (order_item_rows_count));
  check_mem(order_item_row_order_item_ids);

  for (int i = 0; i < order_item_rows_count; i++)
  {
    order_item_row_order_item_ids[i] = order_item_rows[i]->order_item_id;
  }

  check_result(
    array_find_unknowns_int(
      update_order_request_order_item_ids,
      update_order_request->order_items_count,
      order_item_row_order_item_ids,
      order_item_rows_count,
      &unknown_order_item_id_indexes,
      &unknown_order_item_id_indexes_count),
    0);

  for (int i = 0; i < unknown_order_item_id_indexes_count; i++)
  {
    check_result(
      validation_errors_add_level_2(
        &validation_errors_return, &validation_errors_allocated_count, &validation_errors_used_count,
        UPDATE_ORDER_REQUEST_ORDER_ITEMS, unknown_order_item_id_indexes[i],
        UPDATE_ORDER_REQUEST_ORDER_ITEM_ID, -1,
        VALIDATION_RESULT_UNKNOWN),
      0);
  }

  if (validation_errors_return != NULL)
  {
    *update_order_response = update_order_response_return;

    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_used_count;

    goto cleanup;
  }

  updated_order_row = order_row_malloc(
    update_order_request->order_id,
    update_order_request->customer_name,
    order_row->placed_on_date_time,
    update_order_request->total);

  check_not_null(updated_order_row);

  check_result(orders_table_update(sql_connection, updated_order_row), 0);

  order_item_rows_hash_table = hash_table_malloc(order_item_rows_count);
  check_not_null(order_item_rows_hash_table);

  for (int i = 0; i < order_item_rows_count; i++)
  {
    check_result(
      hash_table_add_int_pointer(
        order_item_rows_hash_table,
        *(order_item_rows[i]->order_item_id),
        order_item_rows[i]),
      0);
  }

  for (int i = 0; i < update_order_request->order_items_count; i++)
  {
    if (update_order_request->order_items[i]->order_item_id == NULL)
    {
      inserted_order_item_row = order_item_row_malloc(
        NULL,
        update_order_request->order_id,
        update_order_request->order_items[i]->name,
        update_order_request->order_items[i]->quantity);

      check_not_null(inserted_order_item_row);

      check_result(order_items_table_insert(sql_connection, inserted_order_item_row), 0);

      order_item_row_free(inserted_order_item_row);
      inserted_order_item_row = NULL;
    }
    else
    {
      order_item_row_t **values = NULL;
      int values_count = 0;

      check_result(
        hash_table_get_int_pointer(
          order_item_rows_hash_table,
          *(update_order_request->order_items[i]->order_item_id),
          (void ***)&values, &values_count),
        0);

      order_item_row_t *existing_order_item_row = values[0];

      updated_order_item_row = order_item_row_malloc(
        existing_order_item_row->order_item_id,
        existing_order_item_row->order_id,
        update_order_request->order_items[i]->name,
        update_order_request->order_items[i]->quantity);

      check_not_null(updated_order_item_row);

      check_result(order_items_table_update(sql_connection, updated_order_item_row), 0);

      order_item_row_free(updated_order_item_row);
      updated_order_item_row = NULL;
    }
  }

  check_result(
    array_find_unknowns_int(
      order_item_row_order_item_ids,
      order_item_rows_count,
      update_order_request_order_item_ids,
      update_order_request->order_items_count,
      &deleted_order_item_id_indexes,
      &deleted_order_item_id_indexes_count),
    0);

  for (int i = 0; i < deleted_order_item_id_indexes_count; i++)
  {
    check_result(order_items_table_delete(sql_connection, order_item_rows[deleted_order_item_id_indexes[i]]), 0);
  }

  update_order_response_return = update_order_response_malloc();
  check_not_null(update_order_response);

  check_mem(NULL);

  *update_order_response = update_order_response_return;

  goto cleanup;

error:

  if (update_order_response_return != NULL) { update_order_response_free(update_order_response_return); }

  if (validation_errors_return != NULL)
  {
    validation_errors_free(validation_errors_return, validation_errors_used_count);
  }

  exit_code = -1;

cleanup:

  if (order_row != NULL) { order_row_free(order_row); }
  if (order_item_rows != NULL) { order_item_rows_free(order_item_rows, order_item_rows_count); }
  if (updated_order_row != NULL) { order_row_free(updated_order_row); }
  if (inserted_order_item_row != NULL) { order_item_row_free(inserted_order_item_row); }
  if (updated_order_item_row != NULL) { order_item_row_free(updated_order_item_row); }
  if (update_order_request_order_item_ids != NULL) { free(update_order_request_order_item_ids); }
  if (order_item_row_order_item_ids != NULL) { free(order_item_row_order_item_ids); }
  if (unknown_order_item_id_indexes != NULL) { free(unknown_order_item_id_indexes); }
  if (deleted_order_item_id_indexes != NULL) { free(deleted_order_item_id_indexes); }
  if (order_item_rows_hash_table != NULL) { hash_table_free(order_item_rows_hash_table); }

  return exit_code;
}
