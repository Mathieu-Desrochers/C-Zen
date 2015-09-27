#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../web_api/bindings/get_order_response_order_item_http.h"
#include "../../web_api/services/get_order_response_order_item.h"

// formats a get order response order item
int get_order_response_order_item_http_format(
  get_order_response_order_item_t *get_order_response_order_item,
  json_t **json,
  json_context_t *json_context)
{
  json_t *json_return = NULL;

  check_not_null(get_order_response_order_item);
  check_not_null(json);
  check_not_null(json_context);

  json_return = json_object_malloc();
  check_not_null(json_return);

  int *order_item_id = get_order_response_order_item->order_item_id;
  check_result(json_object_set_int(json_return, "order-item-id", order_item_id, json_context), 0);

  char *name = get_order_response_order_item->name;
  check_result(json_object_set_string(json_return, "name", name, json_context), 0);

  double *quantity = get_order_response_order_item->quantity;
  check_result(json_object_set_double(json_return, "quantity", quantity, json_context), 0);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}
