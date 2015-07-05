#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../core/tables/order_row.h"

// allocates an order row
order_row_t *order_row_malloc(
  int *order_id,
  char *customer_name,
  time_t *placed_on_date_time,
  int *total)
{
  order_row_t *order_row = malloc(sizeof(order_row_t));
  check_mem(order_row);

  int malloc_memcpy_order_id_result = malloc_memcpy_int(&(order_row->order_id), order_id);
  check(malloc_memcpy_order_id_result == 0, "malloc_memcpy_order_id_result: %d",
    malloc_memcpy_order_id_result);

  int malloc_memcpy_customer_name_result = malloc_memcpy_string(&(order_row->customer_name), customer_name);
  check(malloc_memcpy_customer_name_result == 0, "malloc_memcpy_customer_name_result: %d",
    malloc_memcpy_customer_name_result);

  int malloc_memcpy_placed_on_date_time_result = malloc_memcpy_time(&(order_row->placed_on_date_time), placed_on_date_time);
  check(malloc_memcpy_placed_on_date_time_result == 0, "malloc_memcpy_placed_on_date_time_result: %d",
    malloc_memcpy_placed_on_date_time_result);

  int malloc_memcpy_total_result = malloc_memcpy_int(&(order_row->total), total);
  check(malloc_memcpy_total_result == 0, "malloc_memcpy_total_result: %d",
    malloc_memcpy_total_result);

  return order_row;

error:

  if (order_row != NULL) { order_row_free(order_row); }

  return NULL;
}

// frees an order row
void order_row_free(order_row_t *order_row)
{
  if (order_row == NULL)
  {
    return;
  }

  if (order_row->order_id != NULL) { free(order_row->order_id); }
  if (order_row->customer_name != NULL) { free(order_row->customer_name); }
  if (order_row->placed_on_date_time != NULL) { free(order_row->placed_on_date_time); }
  if (order_row->total != NULL) { free(order_row->total); }

  free(order_row);
}

// frees an array of order rows
void order_rows_free(order_row_t **order_rows, int count)
{
  if (order_rows == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    order_row_free(order_rows[i]);
  }

  free(order_rows);
}
