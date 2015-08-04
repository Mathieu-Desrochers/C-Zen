#include <stdlib.h>

#include "../../http/services/new_order_response_http.h"
#include "../../core/services/new_order_response.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"

// formats a new order response to json
int new_order_response_json_format(new_order_response_t *new_order_response, json_t **json)
{
  json_t *json_return = NULL;

  check(new_order_response != NULL, "new_order_response: NULL");
  check(json != NULL, "json: NULL");

  json_return = json_object_malloc();
  check(json_return != NULL, "json_return: NULL");

  int json_object_set_order_id_result = json_object_set_int(json_return, "order-id", new_order_response->order_id);
  check(json_object_set_order_id_result == 0, "json_object_set_order_id_result: %d",
    json_object_set_order_id_result);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}