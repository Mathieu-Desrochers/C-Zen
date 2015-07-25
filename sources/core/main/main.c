#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../http/new_order_request_json.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"

int main()
{
  char *json_string = "{\"customer-name\":\"Alice\",\"total\":100}";
  json_t *json = NULL;
  new_order_request_t *new_order_request = NULL;

  int json_parse_string_result = json_parse_string(json_string, &json);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  new_order_request_json_parse(json, &new_order_request);
  check(new_order_request != NULL, "new_order_request: NULL");

  printf("%s\n", new_order_request->customer_name);
  printf("%d\n", *(new_order_request->total));

  json_parse_string_free(json);
  new_order_request_free(new_order_request);

  return 0;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }
  if (json != NULL) { json_parse_string_free(json); }

  return -1;
}
