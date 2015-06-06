#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"
#include "../../core/tables/order_item_row.h"

// allocates an order item row
order_item_row_t *order_item_row_malloc(
  int order_item_id,
  int order_id,
  char *name,
  double quantity,
  time_t shipping_date,
  time_t shipping_time_before,
  time_t shipping_time_after)
{
  order_item_row_t *order_item_row = malloc(sizeof(order_item_row_t));
  check_mem(order_item_row);

  order_item_row->order_item_id = order_item_id;
  order_item_row->order_id = order_id;

  order_item_row->name = strdup(name);
  check_mem(order_item_row->name);

  order_item_row->quantity = quantity;
  order_item_row->shipping_date = shipping_date;
  order_item_row->shipping_time_before = shipping_time_before;
  order_item_row->shipping_time_after = shipping_time_after;

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
