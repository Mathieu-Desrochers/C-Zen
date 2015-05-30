#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_request_order_item.h"

int main()
{
  new_order_request_t *new_order_request = new_order_request_malloc("Alice", 100);
  check(new_order_request != NULL, "new_order_request: NULL");

  new_order_request->order_items = malloc(sizeof(new_order_request_order_item_t *) * 2);
  check_mem(new_order_request->order_items);

  new_order_request->order_items_count = 2;

  new_order_request->order_items[0] = new_order_request_order_item_malloc("Skateboard", 2);
  check(new_order_request->order_items[0] != NULL, "new_order_request->order_items[0]: NULL");

  new_order_request->order_items[1] = new_order_request_order_item_malloc("Pirate sword", 5);
  check(new_order_request->order_items[1] != NULL, "new_order_request->order_items[1]: NULL");

  new_order_request_free(new_order_request);

  return 0;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }

  return -1;
}
