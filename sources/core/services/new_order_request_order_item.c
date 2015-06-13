#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_request_order_item.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/mem/mem.h"

// allocates an new order request order item
new_order_request_order_item_t *new_order_request_order_item_malloc(
  char *name,
  int quantity)
{
  new_order_request_order_item_t *new_order_request_order_item = malloc(sizeof(new_order_request_order_item_t));
  check_mem(new_order_request_order_item);

  int malloc_memcpy_name_result = malloc_memcpy_string(&(new_order_request_order_item->name), name);
  check(malloc_memcpy_name_result == 0, "malloc_memcpy_name_result: %d",
    malloc_memcpy_name_result);

  new_order_request_order_item->quantity = quantity;

  return new_order_request_order_item;

error:

  if (new_order_request_order_item != NULL) { new_order_request_order_item_free(new_order_request_order_item); }

  return NULL;
}

// frees a new order request order item
void new_order_request_order_item_free(new_order_request_order_item_t *new_order_request_order_item)
{
  if (new_order_request_order_item == NULL)
  {
    return;
  }

  if (new_order_request_order_item->name != NULL) { free(new_order_request_order_item->name); }

  free(new_order_request_order_item);
}

// frees an array of new order request order items
void new_order_request_order_items_free(new_order_request_order_item_t **new_order_request_order_items, int count)
{
  if (new_order_request_order_items == NULL)
  {
    return;
  }

  for (int i = 0; i < count; i++)
  {
    new_order_request_order_item_free(new_order_request_order_items[i]);
  }

  free(new_order_request_order_items);
}
