#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_request_order_item.h"
#include "../../infrastructure/array/array.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/string/string.h"

// allocates a new order request
new_order_request_t *new_order_request_malloc(
  char *customer,
  int total)
{
  new_order_request_t *new_order_request = malloc(sizeof(new_order_request_t));
  check_mem(new_order_request);

  string_duplicate(new_order_request->customer, customer);

  new_order_request->order_items = (new_order_request_order_item_t **)array_malloc(0);
  check_mem(new_order_request->order_items);

  new_order_request->total = total;

  return new_order_request;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }

  return NULL;
}

// frees a new order request
void new_order_request_free(new_order_request_t *new_order_request)
{
  if (new_order_request == NULL)
  {
    return;
  }

  if (new_order_request->customer != NULL) { free(new_order_request->customer); }
  if (new_order_request->order_items != NULL) { array_free(new_order_request->order_items, new_order_request_order_item_free); }

  free(new_order_request);
}
