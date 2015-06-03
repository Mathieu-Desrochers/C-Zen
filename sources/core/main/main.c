#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"
#include "../../core/services/new_order_service.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  new_order_request_t *new_order_request = NULL;

  validation_error_t **validation_errors = NULL;
  int count = 0;

  new_order_request = new_order_request_malloc(
    "Alice",
    100);

  check(new_order_request != NULL, "new_order_request: NULL");

  int new_order_request_validate_result = new_order_request_validate(new_order_request, &validation_errors, &count);
  check(new_order_request_validate_result == 0, "new_order_request_validate_result: %d",
    new_order_request_validate_result);

  printf("%d\n", count);

  validation_errors_free(validation_errors, count);
  new_order_request_free(new_order_request);

  return 0;

error:

  if (validation_errors != NULL) { validation_errors_free(validation_errors, count); }
  if (new_order_request != NULL) { new_order_request_free(new_order_request); }

  return -1;
}
