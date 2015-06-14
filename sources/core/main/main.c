#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../infrastructure/dbg/dbg.h"

void print_error(validation_error_t *validation_error)
{
  validation_path_t *validation_path = validation_error->validation_path;

  while (validation_path != NULL)
  {
    printf("property: %d\tindex: %d\t", validation_path->property, validation_path->index);
    validation_path = validation_path->next;
  }

  printf("error_code: %d\n", validation_error->error_code);
}

int main()
{
  char *customer_name = "";
  int total = -1;

  new_order_request_t *new_order_request = new_order_request_malloc(
    customer_name,
    &total);

  char *name = "";
  int quantity = 9999999;

  new_order_request_order_item_t *new_order_request_order_item_1 = new_order_request_order_item_malloc(
    name,
    &quantity);

  new_order_request_order_item_t *new_order_request_order_item_2 = new_order_request_order_item_malloc(
    name,
    &quantity);

  new_order_request->order_items = malloc(sizeof(new_order_request_order_item_t *) * 2);
  new_order_request->order_items[0] = new_order_request_order_item_1;
  new_order_request->order_items[1] = new_order_request_order_item_2;
  new_order_request->order_items_count = 2;

  validation_error_t **validation_errors = NULL;
  int count = 0;

  int result = new_order_request_validate(new_order_request, &validation_errors, &count);
  check(result == 0, "result: %d", result);

  printf("count: %d\n", count);

  for (int i = 0; i < count; i++)
  {
    print_error(validation_errors[i]);
  }

  validation_errors_free(validation_errors, count);

  new_order_request_free(new_order_request);

  return 0;

error:

  if (validation_errors != NULL) { validation_errors_free(validation_errors, count); }
  if (new_order_request != NULL) {new_order_request_free(new_order_request); }

  return -1;
}
