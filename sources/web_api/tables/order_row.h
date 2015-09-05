#ifndef ORDER_ROW_H
#define ORDER_ROW_H

#include <time.h>

// represents an order row
typedef struct order_row_t
{
  int *order_id;
  char *customer_name;
  time_t *placed_on_date_time;
  int *total;
} order_row_t;

// allocates an order row
order_row_t *order_row_malloc(
  int *order_id,
  char *customer_name,
  time_t *placed_on_date_time,
  int *total);

// frees an order row
void order_row_free(order_row_t *order_row);

// frees an array of order rows
void order_rows_free(order_row_t **order_rows, int count);

#endif
