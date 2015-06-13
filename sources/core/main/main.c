#include <stdio.h>
#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../infrastructure/dbg/dbg.h"

int main()
{
  char *customer_name = "";
  int total = -1;

  new_order_request_t *new_order_request = new_order_request_malloc(
    customer_name,
    &total);

  validation_error_t **validation_errors = NULL;
  int count = 0;

  int result = new_order_request_validate(new_order_request, &validation_errors, &count);
  check(result == 0, "result: %d", result);

  printf("%d\n", count);

  printf("%d\t%d\t%d\n",
    validation_errors[0]->validation_path->property,
    validation_errors[0]->validation_path->index,
    validation_errors[0]->error_code);

  printf("%d\t%d\t%d\n",
    validation_errors[1]->validation_path->property,
    validation_errors[1]->validation_path->index,
    validation_errors[1]->error_code);

  validation_errors_free(validation_errors, count);

  new_order_request_free(new_order_request);

  return 0;

error:

  return -1;
}
