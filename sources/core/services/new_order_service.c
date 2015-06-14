#include <sqlite3.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"
#include "../../core/services/new_order_service.h"

// executes the new order service
int new_order_service(
  sqlite3 *sql_connection,
  new_order_request_t *new_order_request,
  new_order_response_t **new_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count)
{
  *new_order_response = new_order_response_malloc(1);
  return 0;
}
