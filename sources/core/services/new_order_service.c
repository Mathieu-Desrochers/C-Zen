#include <sqlite3.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"
#include "../../core/services/new_order_service.h"
#include "../../infrastructure/dbg/dbg.h"

// executes the new order service
int new_order_service(
  sqlite3 *sql_connection,
  new_order_request_t *new_order_request,
  new_order_response_t **new_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count)
{
  validation_error_t **validation_errors_return = NULL;
  int validation_errors_count_return = 0;

  check(sql_connection != NULL, "sql_connection: NULL");
  check(new_order_request != NULL, "new_order_request: NULL");
  check(new_order_response != NULL, "new_order_response: NULL");
  check(validation_errors != NULL, "validation_errors: NULL");
  check(validation_errors_count != NULL, "validation_errors_count: NULL");

  int new_order_request_validate_result = new_order_request_validate(
    new_order_request,
    &validation_errors_return,
    &validation_errors_count_return);

  check(new_order_request_validate_result == 0, "new_order_request_validate_result: %d",
    new_order_request_validate_result);

  if (validation_errors_count_return != 0)
  {
    *validation_errors = validation_errors_return;
    *validation_errors_count = validation_errors_count_return;
    return 0;
  }

  *new_order_response = new_order_response_malloc(NULL);
  return 0;

error:

  if (validation_errors_return != NULL) { validation_errors_free(validation_errors_return, validation_errors_count_return); }

  return -1;
}
