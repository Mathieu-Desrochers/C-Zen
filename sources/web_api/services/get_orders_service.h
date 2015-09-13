#ifndef GET_ORDERS_SERVICE_H
#define GET_ORDERS_SERVICE_H

#include <sqlite3.h>

#include "../../web_api/services/get_orders_request.h"
#include "../../web_api/services/get_orders_response.h"

// executes the get orders service
int get_orders_service(
  sqlite3 *sql_connection,
  get_orders_request_t *get_orders_request,
  get_orders_response_t **get_orders_response,
  validation_error_t ***validation_errors,
  int *validation_errors_count);

#endif
