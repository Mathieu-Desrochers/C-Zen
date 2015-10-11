#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/services/get_orders_response_order.h"

// allocates a get orders response order
get_orders_response_order_t *get_orders_response_order_malloc(
  int *order_id,
  char *customer_name)
{
  get_orders_response_order_t *get_orders_response_order = calloc(1, sizeof(get_orders_response_order_t));
  check_mem(get_orders_response_order);

  check_result(malloc_memcpy_int(&(get_orders_response_order->order_id), order_id), 0);
  check_result(malloc_memcpy_string(&(get_orders_response_order->customer_name), customer_name), 0);

  return get_orders_response_order;

error:

  if (get_orders_response_order != NULL) { get_orders_response_order_free(get_orders_response_order); }

  return NULL;
}

// frees a get orders response order
void get_orders_response_order_free(get_orders_response_order_t *get_orders_response_order)
{
  if (get_orders_response_order == NULL)
  {
    return;
  }

  if (get_orders_response_order->order_id != NULL) { free(get_orders_response_order->order_id); }
  if (get_orders_response_order->customer_name != NULL) { free(get_orders_response_order->customer_name); }

  free(get_orders_response_order);
}

// frees an array of get orders response orders
void get_orders_response_orders_free(get_orders_response_order_t **get_orders_response_orders, int count)
{
  if (get_orders_response_orders == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    get_orders_response_order_free(get_orders_response_orders[i]);
  }

  free(get_orders_response_orders);
}
