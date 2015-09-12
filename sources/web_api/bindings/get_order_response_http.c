#include <stdlib.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"
#include "../../web_api/bindings/get_order_response_http.h"
#include "../../web_api/services/get_order_response.h"

// formats a get order response to json
int get_order_response_json_format(get_order_response_t *get_order_response, json_t **json, json_context_t *json_context)
{
  json_t *json_return = NULL;

  check(get_order_response != NULL, "get_order_response: NULL");
  check(json != NULL, "json: NULL");
  check(json_context != NULL, "json_context: NULL");

  json_return = json_object_malloc();
  check(json_return != NULL, "json_return: NULL");

  int *order_id = get_order_response->order_id;
  int json_object_set_order_id_result = json_object_set_int(json_return, "order-id", order_id, json_context);
  check(json_object_set_order_id_result == 0, "json_object_set_order_id_result: %d",
    json_object_set_order_id_result);

  *json = json_return;

  return 0;

error:

  if (json_return != NULL) { json_free(json_return); }

  return -1;
}
