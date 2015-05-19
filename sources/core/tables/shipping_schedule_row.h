#ifndef SHIPPING_SCHEDULE_ROW_H
#define SHIPPING_SCHEDULE_ROW_H

#include <time.h>

// represents a shipping schedule row
typedef struct shipping_schedule_row_t
{
  int shipping_schedule_id;
  int day_of_week;
  time_t time;
  int is_refrigerated;
  double maximum_weight;
} shipping_schedule_row_t;

// allocates a shipping schedule row
shipping_schedule_row_t *shipping_schedule_row_malloc(
  int shipping_schedule_id,
  int day_of_week,
  time_t time,
  int is_refrigerated,
  double maximum_weight);

// frees a shipping schedule row
void shipping_schedule_row_free(shipping_schedule_row_t *shipping_schedule_row);

// allocates an array of shipping_schedule rows
shipping_schedule_row_t **shipping_schedule_rows_realloc(shipping_schedule_row_t **shipping_schedule_rows, int count);

// frees an array of shipping schedule rows
void shipping_schedule_rows_free(shipping_schedule_row_t **shipping_schedule_rows, int count);

#endif
