#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"
#include "../../core/tables/order_row.h"

// allocates an order row
order_row_t *order_row_malloc(
  int order_id,
  int customer_id,
  int total,
  time_t placed_on_date_time,
  time_t shipped_on_date)
{
  order_row_t *order_row = malloc(sizeof(order_row_t));
  check_mem(order_row);

  order_row->order_id = order_id;
  order_row->customer_id = customer_id;
  order_row->total = total;
  order_row->placed_on_date_time = placed_on_date_time;
  order_row->shipped_on_date = shipped_on_date;

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

  free(order_row);
}

// allocates an array of order rows
order_row_t **order_rows_realloc(order_row_t **order_rows, int count)
{
  order_row_t **order_rows_return = NULL;

  order_rows_return = realloc(order_rows, sizeof(order_row_t *) * count);
  check(order_rows_return != NULL || count == 0, "order_rows_return: %p | count: %d",
    order_rows_return, count);

  return order_rows_return;

error:

  if (order_rows_return != NULL) { free(order_rows_return); }

  return NULL;
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
