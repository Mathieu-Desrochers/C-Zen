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

  check_result(malloc_memcpy_int(&(order_item_row->order_item_id), order_item_id), 0);
  check_result(malloc_memcpy_int(&(order_item_row->order_id), order_id), 0);
  check_result(malloc_memcpy_string(&(order_item_row->name), name), 0);
  check_result(malloc_memcpy_double(&(order_item_row->quantity), quantity), 0);

  return order_item_row;

error:

  if (order_item_row != NULL) { order_item_row_free(order_item_row); }

  return NULL;
}

// compares two order item rows
int order_item_rows_compare_order_item_id(const void *x, const void *y)
{
  order_item_row_t *order_item_row_x = (order_item_row_t *)x;
  order_item_row_t *order_item_row_y = (order_item_row_t *)y;

  int order_item_id_x = *(order_item_row_x->order_item_id);
  int order_item_id_y = *(order_item_row_y->order_item_id);

  return order_item_id_x - order_item_id_y;
}

// sorts an array of order item rows
int order_item_rows_sort_by_order_item_id(order_item_row_t **order_item_rows, int count)
{
  check_not_null(order_item_rows);

  qsort(order_item_rows, count, sizeof(order_item_row_t *), order_item_rows_compare_order_item_id);

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
