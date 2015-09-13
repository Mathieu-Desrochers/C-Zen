#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/tables/order_item_row.h"

// allocates an order item row
order_item_row_t *order_item_row_malloc(
  int *order_item_id,
  int *order_id,
  char *name,
  double *quantity)
{
  order_item_row_t *order_item_row = calloc(1, sizeof(order_item_row_t));
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

  return order_item_row;

error:

  if (order_item_row != NULL) { order_item_row_free(order_item_row); }

  return NULL;
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
