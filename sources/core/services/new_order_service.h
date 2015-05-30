#ifndef NEW_ORDER_SERVICE_H
#define NEW_ORDER_SERVICE_H

#include <sqlite3.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"

// executes the new order service
int new_order_service(sqlite3 *sql_connection, new_order_request_t *new_order_request, new_order_response_t **new_order_response);

#endif
