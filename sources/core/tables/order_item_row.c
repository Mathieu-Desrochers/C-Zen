#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../core/tables/order_item_row.h"

// allocates an order item row
order_item_row_t *order_item_row_malloc(
  int *order_item_id,
  int *order_id,
  char *name,
  double *quantity,
  time_t *shipping_date,
  time_t *shipping_time_before,
  time_t *shipping_time_after)
{
  order_item_row_t *order_item_row = malloc(sizeof(order_item_row_t));
  check_mem(order_item_row);

  int malloc_memcpy_order_item_id_result = malloc_memcpy_int(&(order_item_row->order_item_id), order_item_id);
  check(malloc_memcpy_order_item_id_result == 0, "malloc_memcpy_order_item_id_result: %d",
    malloc_memcpy_order_item_id_result);

  int malloc_memcpy_order_id_result = malloc_memcpy_int(&(order_item_row->order_id), order_id);
  check(malloc_memcpy_order_id_result == 0, "malloc_memcpy_order_id_result: %d",
    malloc_memcpy_order_id_result);

  int malloc_memcpy_name_result = malloc_memcpy_string(&(order_item_row->name), name);
  check(malloc_memcpy_name_result == 0, "malloc_memcpy_name_result: %d",
    malloc_memcpy_name_result);

  int malloc_memcpy_quantity_result = malloc_memcpy_double(&(order_item_row->quantity), quantity);
  check(malloc_memcpy_quantity_result == 0, "malloc_memcpy_quantity_result: %d",
    malloc_memcpy_quantity_result);

  int malloc_memcpy_shipping_date_result = malloc_memcpy_time(&(order_item_row->shipping_date), shipping_date);
  check(malloc_memcpy_shipping_date_result == 0, "malloc_memcpy_shipping_date_result: %d",
    malloc_memcpy_shipping_date_result);

  int malloc_memcpy_shipping_time_before_result = malloc_memcpy_time(&(order_item_row->shipping_time_before), shipping_time_before);
  check(malloc_memcpy_shipping_time_before_result == 0, "malloc_memcpy_shipping_time_before_result: %d",
    malloc_memcpy_shipping_time_before_result);

  int malloc_memcpy_shipping_time_after_result = malloc_memcpy_time(&(order_item_row->shipping_time_after), shipping_time_after);
  check(malloc_memcpy_shipping_time_after_result == 0, "malloc_memcpy_shipping_time_after_result: %d",
    malloc_memcpy_shipping_time_after_result);

  return order_item_row;

error:

  if (order_item_row != NULL) { order_item_row_free(order_item_row); }

  return NULL;
}

// adds an order item row to an array
int order_item_rows_add(order_item_row_t ***order_item_rows, int *allocated_count, int *used_count, order_item_row_t *order_item_row)
{
  order_item_row_t **reallocated_order_item_rows = NULL;

  if (*allocated_count == *used_count)
  {
    int reallocated_count = (*allocated_count) == 0 ? 4 : ((*allocated_count) * 2);
    reallocated_order_item_rows = realloc(*order_item_rows, sizeof(order_item_row_t *) * reallocated_count);
    check_mem(reallocated_order_item_rows);
    *order_item_rows = reallocated_order_item_rows;
    *allocated_count = reallocated_count;
  }

  (*order_item_rows)[*used_count] = order_item_row;
  (*used_count)++;

  return 0;

error:

  return -1;
}

// frees an order item row
void order_item_row_free(order_item_row_t *order_item_row)
{
  if (order_item_row == NULL)
  {
    return;
  }

  if (order_item_row->order_item_id != NULL) { free(order_item_row->order_item_id); }
  if (order_item_row->order_id != NULL) { free(order_item_row->order_id); }
  if (order_item_row->name != NULL) { free(order_item_row->name); }
  if (order_item_row->quantity != NULL) { free(order_item_row->quantity); }
  if (order_item_row->shipping_date != NULL) { free(order_item_row->shipping_date); }
  if (order_item_row->shipping_time_before != NULL) { free(order_item_row->shipping_time_before); }
  if (order_item_row->shipping_time_after != NULL) { free(order_item_row->shipping_time_after); }

  free(order_item_row);
}

// frees an array of order item rows
void order_item_rows_free(order_item_row_t **order_item_rows, int count)
{
  if (order_item_rows == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    order_item_row_free(order_item_rows[i]);
  }

  free(order_item_rows);
}
