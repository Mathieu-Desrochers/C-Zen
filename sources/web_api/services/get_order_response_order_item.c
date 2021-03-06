#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"
#include "../../web_api/services/get_order_response_order_item.h"

// allocates a get order response order item
get_order_response_order_item_t *get_order_response_order_item_malloc(
  int *order_item_id,
  char *name,
  double *quantity)
{
  get_order_response_order_item_t *get_order_response_order_item = calloc(1, sizeof(get_order_response_order_item_t));
  check_mem(get_order_response_order_item);

  check_result(malloc_memcpy_int(&(get_order_response_order_item->order_item_id), order_item_id), 0);
  check_result(malloc_memcpy_string(&(get_order_response_order_item->name), name), 0);
  check_result(malloc_memcpy_double(&(get_order_response_order_item->quantity), quantity), 0);

  return get_order_response_order_item;

error:

  if (get_order_response_order_item != NULL) { get_order_response_order_item_free(get_order_response_order_item); }

  return NULL;
}

// frees a get order response order item
void get_order_response_order_item_free(get_order_response_order_item_t *get_order_response_order_item)
{
  if (get_order_response_order_item == NULL)
  {
    return;
  }

  if (get_order_response_order_item->order_item_id != NULL) { free(get_order_response_order_item->order_item_id); }
  if (get_order_response_order_item->name != NULL) { free(get_order_response_order_item->name); }
  if (get_order_response_order_item->quantity != NULL) { free(get_order_response_order_item->quantity); }

  free(get_order_response_order_item);
}

// frees an array of get order response order items
void get_order_response_order_items_free(get_order_response_order_item_t **get_order_response_order_items, int count)
{
  if (get_order_response_order_items == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    get_order_response_order_item_free(get_order_response_order_items[i]);
  }

  free(get_order_response_order_items);
}
