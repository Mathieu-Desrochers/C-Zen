#ifndef UPDATE_ORDER_SERVICE_H
#define UPDATE_ORDER_SERVICE_H

#include <sqlite3.h>

#include "../../web_api/services/update_order_request.h"
#include "../../web_api/services/update_order_response.h"

// executes the update order service
int update_order_service(
  sqlite3 *sql_connection,
  update_order_request_t *update_order_request,
  update_order_response_t **update_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count);

#endif
