#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"
#include "../../core/tables/shipping_schedule_row.h"

// allocates a shipping schedule row
shipping_schedule_row_t *shipping_schedule_row_malloc(
  int shipping_schedule_id,
  int day_of_week,
  time_t time,
  int is_refrigerated,
  double maximum_weight)
{
  shipping_schedule_row_t *shipping_schedule_row = malloc(sizeof(shipping_schedule_row_t));
  check_mem(shipping_schedule_row);

  shipping_schedule_row->shipping_schedule_id = shipping_schedule_id;
  shipping_schedule_row->day_of_week = day_of_week;
  shipping_schedule_row->time = time;
  shipping_schedule_row->is_refrigerated = is_refrigerated;
  shipping_schedule_row->maximum_weight = maximum_weight;

  return shipping_schedule_row;

error:

  if (shipping_schedule_row != NULL) { shipping_schedule_row_free(shipping_schedule_row); }

  return NULL;
}

// frees a shipping schedule row
void shipping_schedule_row_free(shipping_schedule_row_t *shipping_schedule_row)
{
  if (shipping_schedule_row == NULL)
  {
    return;
  }

  free(shipping_schedule_row);
}

// allocates an array of shipping_schedule rows
shipping_schedule_row_t **shipping_schedule_rows_realloc(shipping_schedule_row_t **shipping_schedule_rows, int count)
{
  shipping_schedule_row_t **shipping_schedule_rows_return = NULL;

  shipping_schedule_rows_return = realloc(shipping_schedule_rows, sizeof(shipping_schedule_row_t *) * count);
  check(shipping_schedule_rows_return != NULL || count == 0, "shipping_schedule_rows_return: %p | count: %d",
    shipping_schedule_rows_return, count);

  return shipping_schedule_rows_return;

error:

  if (shipping_schedule_rows_return != NULL) { free(shipping_schedule_rows_return); }

  return NULL;
}

// frees an array of shipping schedule rows
void shipping_schedule_rows_free(shipping_schedule_row_t **shipping_schedule_rows, int count)
{
  if (shipping_schedule_rows == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    shipping_schedule_row_free(shipping_schedule_rows[i]);
  }

  free(shipping_schedule_rows);
}
