#include <stdlib.h>

#include "../../core/services/new_order_request.h"
#include "../../core/services/new_order_response.h"
#include "../../core/services/new_order_service.h"
#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/sql/sql.h"

int main()
{
  sqlite3 *sql_connection = NULL;
  new_order_request_t *new_order_request = NULL;
  new_order_response_t *new_order_response = NULL;

  int sql_open_connection_result = sql_open_connection("/var/c-zen/c-zen.db", &sql_connection);
  check(sql_open_connection_result == 0, "sql_open_connection_result: %d",
    sql_open_connection_result);

  new_order_request = new_order_request_malloc(
    "Alice",
    100);

  int new_order_service_result = new_order_service(sql_connection, new_order_request, &new_order_response);
  check(new_order_service_result == 0, "new_order_service_result: %d",
    new_order_service_result);

  new_order_response_free(new_order_response);
  new_order_request_free(new_order_request);
  sql_close_connection(sql_connection);

  return 0;

error:

  if (new_order_response != NULL) { new_order_response_free(new_order_response); }
  if (new_order_request != NULL) { new_order_request_free(new_order_request); }
  if (sql_connection != NULL) { sql_close_connection(sql_connection); }
}
