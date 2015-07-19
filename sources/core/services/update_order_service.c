#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>

#include "../../core/services/update_order_request.h"
#include "../../core/services/update_order_response.h"
#include "../../core/services/update_order_service.h"
#include "../../core/tables/order_row.h"
#include "../../core/tables/orders_table.h"
#include "../../core/tables/order_item_row.h"
#include "../../core/tables/order_items_table.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/hash/hash_table.h"
#include "../../infrastructure/time/time.h"

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
  int allocated_errors_count = 0;
  int used_errors_count = 0;

  order_row_t *order_row = NULL;
  order_item_row_t **order_item_rows = NULL;
  int order_item_rows_count = 0;

  hash_table_t *order_item_rows_hash_table = NULL;

  int **update_order_request_order_item_ids = NULL;
  int *order_item_row_order_item_ids = NULL;
  int *unknown_order_item_id_indexes = NULL;
  int unknown_order_item_id_indexes_count = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(update_order_request != NULL, "update_order_request: NULL");
  check(update_order_response != NULL, "update_order_response: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(validation_errors_count != NULL, "validation_errors_count: NULL");

  int update_order_request_validate_result = update_order_request_validate(
    update_order_request,
    &validation_errors_return,
    &allocated_errors_count,
    &used_errors_count);

  check(update_order_request_validate_result == 0, "update_order_request_validate_result: %d",
    update_order_request_validate_result);

  if (validation_errors_return != NULL)
  {
    *validation_errors = validation_errors_return;
    *validation_errors_count = used_errors_count;

    return 0;
  }

  int orders_table_select_by_order_id_result = orders_table_select_by_order_id(
    sql_connection,
    *(update_order_request->order_id),
    &order_row);

  check(orders_table_select_by_order_id_result == 0, "orders_table_select_by_order_id_result: %d",
    orders_table_select_by_order_id_result);

  if (order_row == NULL)
  {
    int validation_errors_add_result = validation_errors_add_level_1(
      &validation_errors_return, &allocated_errors_count, &used_errors_count,
      UPDATE_ORDER_REQUEST_ORDER_ID, -1,
      UPDATE_ORDER_SERVICE_UNKNOWN_ORDER_ID);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);

    *validation_errors = validation_errors_return;
    *validation_errors_count = used_errors_count;

    return 0;
  }

  int order_items_table_select_by_order_id_result = order_items_table_select_by_order_id(
    sql_connection,
    *(update_order_request->order_id),
    &order_item_rows,
    &order_item_rows_count);

  check(order_items_table_select_by_order_id_result == 0, "order_items_table_select_by_order_id_result: %d",
    order_items_table_select_by_order_id_result);

  update_order_request_order_item_ids = malloc(sizeof(int *) * (update_order_request->order_items_count));
  check_mem(update_order_request_order_item_ids);

  for (int i = 0; i < update_order_request->order_items_count; i++)
  {
    update_order_request_order_item_ids[i] = update_order_request->order_items[i]->order_item_id;
  }

  order_item_row_order_item_ids = malloc(sizeof(int) * (order_item_rows_count));
  check_mem(order_item_row_order_item_ids);

  for (int i = 0; i < order_item_rows_count; i++)
  {
    order_item_row_order_item_ids[i] = *(order_item_rows[i]->order_item_id);
  }

  int array_find_unknowns_result = array_find_unknowns_int(
    update_order_request_order_item_ids,
    update_order_request->order_items_count,
    order_item_row_order_item_ids,
    order_item_rows_count,
    &unknown_order_item_id_indexes,
    &unknown_order_item_id_indexes_count);

  check(array_find_unknowns_result == 0, "array_find_unknowns_result: %d",
    array_find_unknowns_result);

  for (int i = 0; i < unknown_order_item_id_indexes_count; i++)
  {
    int validation_errors_add_result = validation_errors_add_level_2(
      &validation_errors_return, &allocated_errors_count, &used_errors_count,
      UPDATE_ORDER_REQUEST_ORDER_ITEMS, unknown_order_item_id_indexes[i],
      UPDATE_ORDER_REQUEST_ORDER_ITEM_ID, -1,
      UPDATE_ORDER_SERVICE_UNKNOWN_ORDER_ITEM_ID);

    check(validation_errors_add_result == 0, "validation_errors_add_result: %d",
      validation_errors_add_result);
  }

  if (validation_errors_return != NULL)
  {
    order_row_free(order_row);
    order_item_rows_free(order_item_rows, order_item_rows_count);

    *validation_errors = validation_errors_return;
    *validation_errors_count = used_errors_count;

    return 0;
  }

  int order_item_rows_hash_table_malloc_result = hash_table_malloc(&order_item_rows_hash_table, order_item_rows_count);
  check(order_item_rows_hash_table_malloc_result == 0, "order_item_rows_hash_table_malloc_result: %d",
    order_item_rows_hash_table_malloc_result);

  for (int i = 0; i < order_item_rows_count; i++)
  {
    int hash_table_add_result = hash_table_add_int_pointer(
      order_item_rows_hash_table,
      *(order_item_rows[i]->order_item_id),
      order_item_rows[i]);

    check(hash_table_add_result == 0, "hash_table_add_result: %d",
      hash_table_add_result);
  }

  order_row_free(order_row);
  order_item_rows_free(order_item_rows, order_item_rows_count);

  free(update_order_request_order_item_ids);
  free(order_item_row_order_item_ids);
  free(unknown_order_item_id_indexes);

  hash_table_free(order_item_rows_hash_table);

  *update_order_response = update_order_response_return;

  return 0;

error:

  if (order_item_rows_hash_table != NULL) { hash_table_free(order_item_rows_hash_table); }
  if (unknown_order_item_id_indexes != NULL) { free(unknown_order_item_id_indexes); }
  if (order_item_row_order_item_ids != NULL) { free(order_item_row_order_item_ids); }
  if (update_order_request_order_item_ids != NULL) { free(update_order_request_order_item_ids); }
  if (order_item_rows != NULL) { order_item_rows_free(order_item_rows, order_item_rows_count); }
  if (order_row != NULL) { order_row_free(order_row); }
  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, used_errors_count); }
  if (update_order_response_return != NULL) { update_order_response_free(update_order_response_return); }

  return -1;
}
