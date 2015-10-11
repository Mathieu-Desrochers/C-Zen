#ifndef GET_ORDER_RESPONSE_H
#define GET_ORDER_RESPONSE_H

#include "../../infrastructure/time/time.h"
#include "../../web_api/services/get_order_response_order_item.h"

// represents a get order response
typedef struct get_order_response_t
{
  int *order_id;
  char *customer_name;
  time_t *placed_on_date_time;
  get_order_response_order_item_t **order_items;
  int order_items_count;
  int *total;
} get_order_response_t;

// allocates a get order response
get_order_response_t *get_order_response_malloc(
  int *order_id,
  char *customer_name,
  time_t *placed_on_date_time,
  int *total);

// frees a get order response
void get_order_response_free(get_order_response_t *get_order_response);

#endif
