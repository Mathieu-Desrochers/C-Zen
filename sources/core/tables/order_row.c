#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"
#include "../../core/tables/order_row.h"

// allocates an order row
order_row_t *order_row_malloc(
  int order_id,
  char *customer_name,
  time_t placed_on_date_time,
  int total)
{
  order_row_t *order_row = malloc(sizeof(order_row_t));
  check_mem(order_row);

  order_row->order_id = order_id;

  order_row->customer_name = strdup(customer_name);
  check_mem(order_row->customer_name);

  order_row->placed_on_date_time = placed_on_date_time;
  order_row->total = total;

  return order_row;

error:

  if (order_row != NULL) { order_row_free(order_row); }

  return NULL;
}

// adds an order row to an array
int order_rows_add(order_row_t ***order_rows, int *count, int *used, order_row_t *order_row)
{
  order_row_t **reallocated_order_rows = NULL;

  if (*count == *used)
  {
    int new_count = (*count) == 0 ? 4 : ((*count) * 2);
    reallocated_order_rows = realloc(*order_rows, sizeof(order_row_t *) * new_count);
    check_mem(reallocated_order_rows);
    *order_rows = reallocated_order_rows;
    *count = new_count;
  }

  (*order_rows)[*used] = order_row;
  (*used)++;

  return 0;

error:

  return -1;
}

// frees an order row
void order_row_free(order_row_t *order_row)
{
  if (order_row == NULL)
  {
    return;
  }

  if (order_row->customer_name != NULL) { free(order_row->customer_name); }

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
