#ifndef ORDER_ROW_H
#define ORDER_ROW_H

#include <time.h>

// represents an order row
typedef struct order_row_t
{
  int order_id;
  int customer_id;
  int total;
  time_t placed_on_date_time;
  time_t shipped_on_date;
} order_row_t;

// allocates an order row
order_row_t *order_row_malloc(
  int order_id,
  int customer_id,
  int total,
  time_t placed_on_date_time,
  time_t shipped_on_date);

// frees an order row
void order_row_free(order_row_t *order_row);

// frees an array of order rows
void order_rows_free(order_row_t **order_rows, int count);

#endif
