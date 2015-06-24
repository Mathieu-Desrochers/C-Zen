#ifndef ORDER_ITEM_ROW_H
#define ORDER_ITEM_ROW_H

#include <time.h>

// represents an order item row
typedef struct order_item_row_t
{
  int *order_item_id;
  int *order_id;
  char *name;
  double *quantity;
  time_t *shipping_date;
  time_t *shipping_time_before;
  time_t *shipping_time_after;
} order_item_row_t;

// allocates an order item row
order_item_row_t *order_item_row_malloc(
  int *order_item_id,
  int *order_id,
  char *name,
  double *quantity,
  time_t *shipping_date,
  time_t *shipping_time_before,
  time_t *shipping_time_after);

// adds an order item row to an array
int order_item_rows_add(order_item_row_t ***order_item_rows, int *allocated_count, int *used_count, order_item_row_t *order_item_row);

// frees an order item row
void order_item_row_free(order_item_row_t *order_item_row);

// frees an array of order item rows
void order_item_rows_free(order_item_row_t **order_item_rows, int count);

#endif
