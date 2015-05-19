#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"
#include "../../core/tables/order_item_row.h"

// allocates an order item row
order_item_row_t *order_item_row_malloc(
  int order_item_id,
  int order_id,
  char *name,
  int quantity)
{
  order_item_row_t *order_item_row = malloc(sizeof(order_item_row_t));
  check_mem(order_item_row);

  order_item_row->order_item_id = order_item_id;
  order_item_row->order_id = order_id;
  string_duplicate(order_item_row->name, name);
  order_item_row->quantity = quantity;

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

  if (order_item_row->name != NULL) { free(order_item_row->name); }

  free(order_item_row);
}

// allocates an array of order_item rows
order_item_row_t **order_item_rows_realloc(order_item_row_t **order_item_rows, int count)
{
  order_item_row_t **order_item_rows_return = NULL;

  order_item_rows_return = realloc(order_item_rows, sizeof(order_item_row_t *) * count);
  check(order_item_rows_return != NULL || count == 0, "order_item_rows_return: %p | count: %d",
    order_item_rows_return, count);

  return order_item_rows_return;

error:

  if (order_item_rows_return != NULL) { free(order_item_rows_return); }

  return NULL;
}

// frees an array of order_item rows
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
