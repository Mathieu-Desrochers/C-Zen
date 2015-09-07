#ifndef GET_ORDER_SERVICE_H
#define GET_ORDER_SERVICE_H

#include <sqlite3.h>

#include "../../web_api/services/get_order_request.h"
#include "../../web_api/services/get_order_response.h"

// executes the get order service
int get_order_service(
  sqlite3 *sql_connection,
  get_order_request_t *get_order_request,
  get_order_response_t **get_order_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count);

#endif
