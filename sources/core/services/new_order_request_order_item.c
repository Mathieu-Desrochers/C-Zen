#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_request_order_item.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"

// allocates an order item
new_order_request_order_item_t *new_order_request_order_item_malloc(
  char *name,
  int quantity)
{
  new_order_request_order_item_t *new_order_request_order_item = malloc(sizeof(new_order_request_order_item_t));
  check_mem(new_order_request_order_item);

  string_duplicate(new_order_request_order_item->name, name);
  new_order_request_order_item->quantity = quantity;

  return new_order_request_order_item;

error:

  if (new_order_request_order_item != NULL) { new_order_request_order_item_free(new_order_request_order_item); }

  return NULL;
}

// frees an order item
void new_order_request_order_item_free(new_order_request_order_item_t *new_order_request_order_item)
{
  if (new_order_request_order_item == NULL)
  {
    return;
  }

  if (new_order_request_order_item->name != NULL) { free(new_order_request_order_item->name); }

  free(new_order_request_order_item);
}
