#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../http/new_order_request_json.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/json/json.h"

int main()
{
  char *json_string = "{\"customer-name\":\"Alice\",\"order-items\":[{\"name\":\"Pirate hat\",\"quantity\":1.25}],\"total\":100}";
  json_t *json = NULL;
  new_order_request_t *new_order_request = NULL;

  int json_parse_string_result = json_parse_string(json_string, &json);
  check(json_parse_string_result == 0, "json_parse_string_result: %d",
    json_parse_string_result);

  new_order_request_json_parse(json, &new_order_request);
  check(new_order_request != NULL, "new_order_request: NULL");

  if (new_order_request->customer_name != NULL)
  {
    printf("customer_name: %s\n", new_order_request->customer_name);
  }

  printf("order_items: %p\n", new_order_request->order_items);
  printf("order_items_count: %d\n", new_order_request->order_items_count);

  for (int i = 0; i < new_order_request->order_items_count; i++)
  {
    printf("  order_item[%d]: %p\n", i, new_order_request->order_items[i]);

    if (new_order_request->order_items[i] != NULL)
    {
      if (new_order_request->order_items[i]->name != NULL)
      {
        printf("  order_item[%d]->name: %s\n", i, new_order_request->order_items[i]->name);
      }

      if (new_order_request->order_items[i]->quantity != NULL)
      {
        printf("  order_item[%d]->quantity: %f\n", i, *(new_order_request->order_items[i]->quantity));
      }
    }
  }

  printf("total: %d\n", *(new_order_request->total));

  json_parse_string_free(json);
  new_order_request_free(new_order_request);

  return 0;

error:

  if (new_order_request != NULL) { new_order_request_free(new_order_request); }
  if (json != NULL) { json_parse_string_free(json); }

  return -1;
}
